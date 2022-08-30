#include "SdhcalPmrAccess.hh"
#include "pmBuffer.hh"

#include <algorithm>  // std::copy
#include <fstream>
#include <iostream>
#include <iterator>  // std::ostream_iterator
#include <map>
#include <set>

//#include "TFile.h"
//#include "TTree.h"

void usage(const char* prog)
{
  std::cout << "usage : " << prog << " InputFileName OutputFileName" << std::endl;
  std::cout << "InputFileName is SDHCAL 2022 raw" << std::endl;
  std::cout << "OutputFileName is ROOT  (not yet implemented)" << std::endl;
}

void print(const char* header, std::set<uint32_t>& s)
{
  std::ostream_iterator<uint32_t> out_it(std::cout, ", ");
  std::cout << header << " : ";
  std::copy(s.begin(), s.end(), out_it);
  std::cout << std::endl;
}

void printMap(const char* header, std::map<uint32_t, uint32_t>& m)
{
  std::cout << header << " : ";
  for(const auto& x: m) std::cout << x.first << "=" << x.second << " // ";
  std::cout << std::endl;
}

int main(int argc, char** argv)
{
  if(argc != 3)
  {
    usage(argv[0]);
    return 1;
  }

  std::set<uint32_t>           DetectorSet;
  std::set<uint32_t>           DIFSet;
  std::set<uint32_t>           idstartSet, nbASICSet, difFormatSet;
  std::map<uint32_t, uint32_t> DIFstat, ASICstat, ChannelStat, ThresholdStat;

  //open data file
  const char*   inputFile = argv[1];
  std::ifstream inputStream(inputFile, std::ios::binary);
  if(!inputStream.good())
  {
    std::cout << "Can't open input file" << std::endl;
    return 2;
  }

  pm::buffer theDataBuffer(0x100000);

  unsigned int readouts       = 0;
  uint32_t     run            = 0;  //NB run n'est pas fonctionnel pour le moment
  uint32_t     readoutNumber  = 0;
  uint32_t     theNumberOfDIF = 0;
  uint64_t     bunchCrossingIdFirst(0);

  bool moreToRead = true;

  while(moreToRead)
  {
    moreToRead = false;

    //read event number
    inputStream.read((char*)&readoutNumber, sizeof(uint32_t));
    if(!inputStream.good())
    {
      std::cout << " can't read more event" << std::endl;
      break;
    }
    if(readoutNumber % 100 == 0) std::cout << "Readout number read " << readoutNumber << std::endl;

    //read number of DIF
    inputStream.read((char*)&theNumberOfDIF, sizeof(uint32_t));
    if(!inputStream.good())
    {
      std::cout << " can't read more number of DIF" << std::endl;
      break;
    }
    if(readoutNumber % 100 == 0) std::cout << "Readout read " << readoutNumber << " with " << theNumberOfDIF << " DIF." << std::endl;

    //loop to read DIF data
    bool DIFreadingIsOK = true;
    for(uint32_t idif = 0; idif < theNumberOfDIF; ++idif)
    {
      DIFreadingIsOK = false;
      uint32_t bsize = 0;
      inputStream.read((char*)&bsize, sizeof(uint32_t));
      if(!inputStream.good())
      {
        std::cout << " can't read more DIF buffer size" << std::endl;
        break;
      }

      inputStream.read(theDataBuffer.ptr(), bsize);
      if(!inputStream.good())
      {
        std::cout << " can't read more DIF buffer data" << std::endl;
        break;
      }

      // Uncompress buffer
      theDataBuffer.setPayloadSize(bsize - (3 * sizeof(uint32_t) + sizeof(uint64_t)));
      theDataBuffer.uncompress();

      theDataBuffer.setDetectorId(theDataBuffer.detectorId() & 0xFF);
      if(bunchCrossingIdFirst == 0) bunchCrossingIdFirst = theDataBuffer.bxId();
      DetectorSet.insert(theDataBuffer.detectorId());
      DIFSet.insert(theDataBuffer.dataSourceId());
      if(theDataBuffer.detectorId() == 150)
      {
        uint8_t* theBufferAsChar = (uint8_t*)theDataBuffer.payload();
        uint32_t idstart         = sdhcal::PMRUnpacker::getStartOfPMR(theBufferAsChar, theDataBuffer.payloadSize(), 0);
        idstartSet.insert(idstart);
        if(theBufferAsChar[idstart] != DU_START_OF_DIF) std::cout << " Issue with start of DIF" << std::endl;

        uint32_t difId     = (theDataBuffer.dataSourceId()) & 0xFF;
        uint32_t difId_raw = sdhcal::PMRUnpacker::getID(theBufferAsChar, idstart);
        if(difId != difId_raw) std::cout << " Problem with dif ID " << difId << " vs " << difId_raw << std::endl;

        uint8_t nbASIC = theBufferAsChar[idstart + PMR_NBASIC_SHIFT];
        nbASICSet.insert(nbASIC);
        uint8_t difFormat = theBufferAsChar[idstart + PMR_FORMAT_SHIFT];
        difFormatSet.insert(difFormat);
        uint32_t GTC             = sdhcal::PMRUnpacker::getGTC(theBufferAsChar, idstart);
        uint64_t AbsBCID         = sdhcal::PMRUnpacker::getAbsoluteBCID(theBufferAsChar, idstart);
        uint32_t difBCID         = sdhcal::PMRUnpacker::getBCID(theBufferAsChar, idstart);
        uint32_t LastTriggerBCID = sdhcal::PMRUnpacker::getLastTriggerBCID(theBufferAsChar, idstart);

        // Cet octet 19 est toujours a zero
        //if (theBufferAsChar[idstart+PMR_HEADER_SHIFT-1] != DU_START_OF_FRAME) std::cout << " Issue with start of frame : " << (int) *(theBufferAsChar+(idstart+19)) << std::endl;

        std::vector<unsigned char*> theFrames, theLines;
        uint32_t                    endOfDifBuffer = sdhcal::PMRUnpacker::getFramePtr(theFrames, theLines, theDataBuffer.payloadSize() - idstart, theBufferAsChar, idstart);
        if(theBufferAsChar[endOfDifBuffer] != DU_END_OF_DIF) std::cout << "Problems with end of DIF" << std::endl;

        for(std::vector<unsigned char*>::iterator it = theFrames.begin(); it != theFrames.end(); ++it)
        {
          unsigned char* theFramePtr = *it;
          uint32_t       AsicID      = sdhcal::PMRUnpacker::getFrameAsicHeader(theFramePtr);
          uint32_t       frameBCID   = sdhcal::PMRUnpacker::getFrameBCID(theFramePtr);
          for(int ipad = 0; ipad < 64; ++ipad)
          {
            uint32_t thresh = 0;
            if(sdhcal::PMRUnpacker::getFrameLevel(theFramePtr, ipad, 0)) thresh += 2;
            if(sdhcal::PMRUnpacker::getFrameLevel(theFramePtr, ipad, 1)) thresh += 1;
            if(thresh != 0)
            {
              DIFstat[difId]++;
              ASICstat[AsicID]++;
              ChannelStat[ipad]++;
              ThresholdStat[thresh]++;
            }
          }
        }
      }
      DIFreadingIsOK = true;
    }

    if(not DIFreadingIsOK) break;
    ++readouts;
    moreToRead = true;
  }
  std::cout << "Have processed " << readouts << " readouts." << std::endl;
  print(" Detector List", DetectorSet);
  print(" DIF List", DIFSet);
  print(" idstart", idstartSet);
  print(" nbASIC", nbASICSet);
  print(" DIF format", difFormatSet);
  printMap(" DIF stat", DIFstat);
  printMap(" ASIC stat", ASICstat);
  printMap(" channel stat", ChannelStat);
  printMap(" threshold stat", ThresholdStat);
  return 0;
}
