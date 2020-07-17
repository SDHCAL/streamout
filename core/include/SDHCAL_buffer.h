//
// Code is extracted from Antoine Pingault Trivent4HEP
// see https://github.com/apingault/Trivent4HEP
//
// Code originally written by Laurent Mirabito
//

#ifndef SDHCAL_buffer_h
#define SDHCAL_buffer_h

#include <utility>
#include <iostream>
#include <assert.h>
#include "DIFSlowControl.h"


class SDHCAL_buffer : public std::pair<unsigned char*, uint32_t>
{
 public:
  SDHCAL_buffer(unsigned char* b, uint32_t i) : std::pair<unsigned char*, uint32_t>(b,i) {;}
  unsigned char* buffer() {return first;}
  unsigned char* endOfBuffer() {return first+second;}
  uint32_t getsize() {return second;}
  void printBuffer(uint32_t start, uint32_t stop,std::ostream& flux=std::cout); 
  void printBuffer(uint32_t start=0,std::ostream& flux=std::cout) {printBuffer(start,getsize());}
};

//class to navigate in the raw data buffer
class SDHCAL_RawBuffer_Navigator
{
 public:
  SDHCAL_RawBuffer_Navigator(SDHCAL_buffer b);
  ~SDHCAL_RawBuffer_Navigator() {if (NULL!=_theDIFPtr) delete _theDIFPtr;}
  bool validBuffer() {return _DIFstartIndex != 0;}
  uint32_t getStartOfDIF() {return _DIFstartIndex;}
  unsigned char* getDIFBufferStart() {return &(_buffer.buffer()[_DIFstartIndex]);}
  uint32_t getDIFBufferSize() {return _buffer.getsize()-_DIFstartIndex;}
  SDHCAL_buffer getDIFBuffer() { return SDHCAL_buffer(getDIFBufferStart(),getDIFBufferSize());}
  DIFPtr* getDIFPtr();
  uint32_t getEndOfDIFData() { return getDIFPtr()->getGetFramePtrReturn()+3;} 
  uint32_t getSizeAfterDIFPtr() { return getDIFBufferSize()-getDIFPtr()->getGetFramePtrReturn();}
  uint32_t getDIF_CRC();
  bool hasSlowControlData() { return getDIFBufferStart()[getEndOfDIFData()]==0xb1;}
  SDHCAL_buffer getSCBuffer() { setSCBuffer(); return _SCbuffer;}
  bool badSCData() {setSCBuffer(); return _badSCdata;}
  SDHCAL_buffer getEndOfAllData();


 private:
  void setSCBuffer();
  SDHCAL_buffer _buffer,_SCbuffer;
  uint32_t _DIFstartIndex;
  DIFPtr* _theDIFPtr;
  bool _badSCdata;
};


struct SDHCAL_buffer_LoopCounter
{
public:
  int hasSlowControl=0;
  int hasBadSlowControl=0;
  std::map<int,int> DIFStarter;
  std::map<int,int> DIFPtrValueAtReturnedPos;
  std::map<int,int> SizeAfterDIFPtr;
  std::map<int,int> SizeAfterAllData;
  std::map<int,int> NonZeroValusAtEndOfData;

  void printCounter(std::string description, std::map<int,int> & m, std::ostream& out=std::cout);
  void printAllCounters(std::ostream& out=std::cout);
};

//function to loop on buffers
//
// template class should implement
// bool SOURCE::next();
// SDHCAL_buffer SOURCE::getSDHCALBuffer();
//
// void DESTINATION::start();
// void DESTINATION::processDIF(DIFPtr*);
// void DESTINATION::processFrame(DIFPtr*,uint32_t frameIndex);
// void DESTINATION::processPadInFrame(DIFPtr*,uint32_t frameIndex, uint32_t channelIndex);
// void DESTINATION::processSlowControl(SDHCAL_buffer);
// void DESTINATION::end();
//

template <typename SOURCE, typename DESTINATION>
  void SDHCAL_buffer_loop( SOURCE &source, DESTINATION &dest, bool debug=false, std::ostream& out=std::cout, bool verbose=false, std::ostream& verbose_out=std::cout)
{
  dest.start();
  SDHCAL_buffer_LoopCounter c;

  while ( source.next() )
    {
      SDHCAL_buffer buffer=source.getSDHCALBuffer();
      unsigned char* debug_variable_1=buffer.endOfBuffer();
      SDHCAL_RawBuffer_Navigator bufferNavigator(buffer);
      unsigned char* debug_variable_2=bufferNavigator.getDIFBuffer().endOfBuffer();
      if (verbose) verbose_out << "DIF BUFFER END " << (unsigned int *) debug_variable_1 << " " << (unsigned int *) debug_variable_2 << std::endl;
      if (debug) assert (debug_variable_1 == debug_variable_2);
      uint32_t idstart=bufferNavigator.getStartOfDIF();
      if (debug && idstart==0) buffer.printBuffer();
      c.DIFStarter[idstart]++;
      if (!  bufferNavigator.validBuffer() ) continue;
      DIFPtr *d=bufferNavigator.getDIFPtr();
      if (debug) assert(d!=NULL);
      if (d!=NULL)
	{
	  c.DIFPtrValueAtReturnedPos[bufferNavigator.getDIFBufferStart()[d->getGetFramePtrReturn()] ]++;
	  if (debug) assert( bufferNavigator.getDIFBufferStart()[d->getGetFramePtrReturn()]==0xa0 );
	}
      c.SizeAfterDIFPtr[bufferNavigator.getSizeAfterDIFPtr()]++;
      dest.processDIF(d);
      for (uint32_t i=0;i<d->getNumberOfFrames();i++)
	{
	  dest.processFrame(d,i);
	  for (uint32_t j=0;j<64;j++) dest.processPadInFrame(d,i,j);
	}

      if (bufferNavigator.hasSlowControlData()) c.hasSlowControl++;
      if (bufferNavigator.badSCData()) {c.hasBadSlowControl++;}
      else {dest.processSlowControl(bufferNavigator.getSCBuffer());}

      SDHCAL_buffer eod=bufferNavigator.getEndOfAllData();
      c.SizeAfterAllData[eod.getsize()]++;
      unsigned char* debug_variable_3=eod.endOfBuffer();
      if (verbose) verbose_out << "END DATA BUFFER END " << (unsigned int *) debug_variable_1 << " " << (unsigned int *) debug_variable_3 << std::endl;
      if (debug) assert (debug_variable_1 == debug_variable_3);
      if (verbose) {verbose_out << "End of Data remaining stuff : "; eod.printBuffer();}

      int nonzeroCount=0;
      for (unsigned char* it=eod.buffer(); it != eod.endOfBuffer(); it++)
	if (int(*it) !=0) nonzeroCount++;
      c.NonZeroValusAtEndOfData[nonzeroCount]++;
    } //end of while loop

  c.printAllCounters(out);
  dest.end();
}


#endif
