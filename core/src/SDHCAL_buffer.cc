#include "SDHCAL_buffer.h"

#include "DIFUnpacker.h"

void SDHCAL_buffer::printBuffer(unsigned int start, unsigned int stop,std::ostream& flux)
{
  flux << std::hex;
  for (unsigned int k=start; k<stop; k++)
    flux << (unsigned int)(first[k]) << " - ";
  flux << std::dec <<  std::endl;
}

SDHCAL_RawBuffer_Navigator::SDHCAL_RawBuffer_Navigator(SDHCAL_buffer b) :
  _buffer(b),_SCbuffer(0,0)
{
  _DIFstartIndex=DIFUnpacker::getStartOfDIF(_buffer.buffer(),_buffer.getsize(),92);
  _theDIFPtr=NULL;
  _badSCdata=false;
}


DIFPtr* SDHCAL_RawBuffer_Navigator::getDIFPtr()
{
  if (NULL==_theDIFPtr) _theDIFPtr=new DIFPtr(getDIFBufferStart(),getDIFBufferSize());
  return _theDIFPtr;
}

uint32_t SDHCAL_RawBuffer_Navigator::getDIF_CRC()
{
  uint32_t i=getEndOfDIFData();
  uint32_t ret=0;
  ret |= ( (_buffer.buffer()[i-2])<<8 );
  ret |= _buffer.buffer()[i-1];
  return ret;
}

void SDHCAL_RawBuffer_Navigator::setSCBuffer()
{
  if (! hasSlowControlData() ) return;
  if (0 != _SCbuffer.getsize() ) return; //deja fait
  if (_badSCdata) return;
  _SCbuffer.first=&(getDIFBufferStart()[getEndOfDIFData()]);
  //compute Slow Control size
  uint32_t maxsize=_buffer.getsize()-_DIFstartIndex-getEndOfDIFData()+1; // should I +1 here ?
  uint32_t k=1; //SC Header
  uint32_t dif_ID=_SCbuffer.first[1];
  uint32_t chipSize=_SCbuffer.first[3];
    while ( (dif_ID != 0xa1 && _SCbuffer.first[k] != 0xa1 && k <maxsize) ||
	    (dif_ID == 0xa1 && _SCbuffer.first[k+2]==chipSize && k<maxsize) )
    {
      k+=2; //DIF ID + ASIC Header
      uint32_t scsize=_SCbuffer.first[k];
      if (scsize != 74 && scsize != 109)
	{
	  std::cout << "PROBLEM WITH SC SIZE " << scsize << std::endl;
	  k=0;
	  _badSCdata=true;
	  break;
	}
      k++; //skip size bit
      k+=scsize; // skip the data
    }
  if (_SCbuffer.first[k] == 0xa1 && !_badSCdata ) _SCbuffer.second=k+1; //add the trailer
  else
    {
      _badSCdata=true;
      std::cout << "PROBLEM SC TRAILER NOT FOUND " << std::endl;
    }
}


SDHCAL_buffer SDHCAL_RawBuffer_Navigator::getEndOfAllData()
{
  setSCBuffer();
  if (hasSlowControlData() && !_badSCdata)
    {
      return SDHCAL_buffer( &(_SCbuffer.buffer()[_SCbuffer.getsize()]), getSizeAfterDIFPtr()-3-_SCbuffer.getsize() );
    }
  else
    return SDHCAL_buffer( &(getDIFBufferStart()[getEndOfDIFData()]), getSizeAfterDIFPtr()-3 ); //remove the 2 bytes for CRC and the DIF trailer
}

void SDHCAL_buffer_LoopCounter::printCounter(std::string description, std::map<int,int> &m , std::ostream& out)
{
  out << " statistics for " << description << " : ";
  for (std::map<int,int>::iterator it=m.begin(); it != m.end(); it++)
    {
      if (it != m.begin() ) out << ",";
      out << " [" << it->first << "]=" << it->second;
    }
  out << std::endl;
}

void SDHCAL_buffer_LoopCounter::printAllCounters(std::ostream& out)
{
  out << "BUFFER LOOP FINAL STATISTICS : " << std::endl;
  printCounter("Start of DIF header",DIFStarter, out);
  printCounter("Value after DIF data are processed",DIFPtrValueAtReturnedPos, out);
  printCounter("Size remaining in buffer after end of DIF data",  SizeAfterDIFPtr ,out);
  out << "Number of Slow Control found " << hasSlowControl << " out of which " << hasBadSlowControl << " are bad" << std::endl;
  printCounter("Size remaining after all of data have been processed",SizeAfterAllData, out );
  printCounter("Number on non zero values in end of data buffer",NonZeroValusAtEndOfData, out);
}
