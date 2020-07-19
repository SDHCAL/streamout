#include "SDHCAL_RawBuffer_Navigator.h"
#include "Constants.h"

SDHCAL_RawBuffer_Navigator::SDHCAL_RawBuffer_Navigator(const SDHCAL_buffer& b) : m_Buffer(b),m_SCbuffer(0,0)
{
  std::uint32_t id0{0};
  for(std::uint32_t i=92;i<m_Buffer.getsize();i++)
  {
    if (m_Buffer.buffer()[i]!=DU_START_OF_DIF && m_Buffer.buffer()[i]!=DU_START_OF_DIF_TEMP) continue;
    id0=i;
    //if (cbuf[id0+DU_ID_SHIFT]>0xFF) continue;
    break;
  }
  m_DIFstartIndex=id0;
}

SDHCAL_RawBuffer_Navigator::~SDHCAL_RawBuffer_Navigator()
{
  if(m_TheDIFPtr!=nullptr) delete m_TheDIFPtr;
}

bool SDHCAL_RawBuffer_Navigator::validBuffer()
{ 
  return m_DIFstartIndex != 0;
}

std::uint32_t SDHCAL_RawBuffer_Navigator::getStartOfDIF() 
{
  return m_DIFstartIndex;
}

unsigned char* SDHCAL_RawBuffer_Navigator::getDIFBufferStart()
{
  return &(m_Buffer.buffer()[m_DIFstartIndex]);
}

std::uint32_t SDHCAL_RawBuffer_Navigator::getDIFBufferSize()
{
  return m_Buffer.getsize()-m_DIFstartIndex;
}

SDHCAL_buffer SDHCAL_RawBuffer_Navigator::getDIFBuffer() 
{ 
  return SDHCAL_buffer(getDIFBufferStart(),getDIFBufferSize());
}

DIFPtr* SDHCAL_RawBuffer_Navigator::getDIFPtr()
{
  if(m_TheDIFPtr==nullptr) m_TheDIFPtr=new DIFPtr(getDIFBufferStart(),getDIFBufferSize());
  return m_TheDIFPtr;
}

std::uint32_t SDHCAL_RawBuffer_Navigator::getEndOfDIFData()
{ 
  return getDIFPtr()->getGetFramePtrReturn()+3;
} 

std::uint32_t SDHCAL_RawBuffer_Navigator::getSizeAfterDIFPtr() 
{ 
  return getDIFBufferSize()-getDIFPtr()->getGetFramePtrReturn();
}

uint32_t SDHCAL_RawBuffer_Navigator::getDIF_CRC()
{
  uint32_t i{getEndOfDIFData()};
  uint32_t ret{0};
  ret |= ( (m_Buffer.buffer()[i-2])<<8 );
  ret |= m_Buffer.buffer()[i-1];
  return ret;
}

bool SDHCAL_RawBuffer_Navigator::hasSlowControlData()
{ 
  return getDIFBufferStart()[getEndOfDIFData()]==0xb1;
}

SDHCAL_buffer SDHCAL_RawBuffer_Navigator::getSCBuffer()
{ 
  setSCBuffer();
  return m_SCbuffer;
}

bool SDHCAL_RawBuffer_Navigator::badSCData()
{
  setSCBuffer();
  return m_BadSCdata;
}

void SDHCAL_RawBuffer_Navigator::setSCBuffer()
{
  if(! hasSlowControlData()) return;
  if(m_SCbuffer.getsize()!=0) return; //deja fait
  if(m_BadSCdata) return;
  m_SCbuffer.first=&(getDIFBufferStart()[getEndOfDIFData()]);
  //compute Slow Control size
  uint32_t maxsize{m_Buffer.getsize()-m_DIFstartIndex-getEndOfDIFData()+1}; // should I +1 here ?
  uint32_t k{1}; //SC Header
  uint32_t dif_ID{m_SCbuffer.first[1]};
  uint32_t chipSize{m_SCbuffer.first[3]};
  while( (dif_ID != 0xa1 && m_SCbuffer.first[k] != 0xa1 && k <maxsize) ||(dif_ID == 0xa1 && m_SCbuffer.first[k+2]==chipSize && k<maxsize) )
  {
    k+=2; //DIF ID + ASIC Header
    uint32_t scsize=m_SCbuffer.first[k];
    if (scsize != 74 && scsize != 109)
    {
      std::cout << "PROBLEM WITH SC SIZE " << scsize << std::endl;
      k=0;
      m_BadSCdata=true;
      break;
    }
    k++; //skip size bit
    k+=scsize; // skip the data
  }
  if (m_SCbuffer.first[k] == 0xa1 && !m_BadSCdata ) m_SCbuffer.second=k+1; //add the trailer
  else
  {
    m_BadSCdata=true;
    std::cout << "PROBLEM SC TRAILER NOT FOUND " << std::endl;
  }
}

SDHCAL_buffer SDHCAL_RawBuffer_Navigator::getEndOfAllData()
{
  setSCBuffer();
  if (hasSlowControlData() && !m_BadSCdata)
  {
    return SDHCAL_buffer( &(m_SCbuffer.buffer()[m_SCbuffer.getsize()]), getSizeAfterDIFPtr()-3-m_SCbuffer.getsize() );
  }
  else return SDHCAL_buffer( &(getDIFBufferStart()[getEndOfDIFData()]), getSizeAfterDIFPtr()-3 ); //remove the 2 bytes for CRC and the DIF trailer
}
