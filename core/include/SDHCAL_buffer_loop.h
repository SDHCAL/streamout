#pragma once

#include "SDHCAL_buffer.h"
#include "SDHCAL_buffer_LoopCounter.h"
#include "SDHCAL_RawBuffer_Navigator.h"
#include <cassert>
#include <ostream>
#include <iostream>
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

template <typename SOURCE, typename DESTINATION> class SDHCAL_buffer_loop
{
public:
  SDHCAL_buffer_loop(SOURCE &source, DESTINATION &dest, bool debug=false, std::ostream& out=std::cout, bool verbose=false, std::ostream& verbose_out=std::cout):m_Source(source),m_Destination(dest),m_Debug(debug),m_DebugOut(out),m_Verbose(verbose),m_VerboseOut(verbose_out){}
  void loop(const std::int32_t& m_NbrEventsToProcess=0)
  {
    m_Destination.start();
    SDHCAL_buffer_LoopCounter c;
  
    while( m_Source.next() && m_NbrEventsToProcess>=m_NbrEvents)
    {
      SDHCAL_buffer buffer=m_Source.getSDHCALBuffer();
      unsigned char* debug_variable_1=buffer.endOfBuffer();
      SDHCAL_RawBuffer_Navigator bufferNavigator(buffer);
      unsigned char* debug_variable_2=bufferNavigator.getDIFBuffer().endOfBuffer();
      if (m_Verbose) m_VerboseOut << "DIF BUFFER END " << (unsigned int *) debug_variable_1 << " " << (unsigned int *) debug_variable_2 << std::endl;
      if (m_Debug) assert (debug_variable_1 == debug_variable_2);
      uint32_t idstart=bufferNavigator.getStartOfDIF();
      if (m_Debug && idstart==0) buffer.printBuffer();
      c.DIFStarter[idstart]++;
      if (!  bufferNavigator.validBuffer() ) continue;
      DIFPtr *d=bufferNavigator.getDIFPtr();
      if (m_Debug) assert(d!=NULL);
      if (d!=NULL)
      {
        c.DIFPtrValueAtReturnedPos[bufferNavigator.getDIFBufferStart()[d->getGetFramePtrReturn()] ]++;
        if (m_Debug) assert( bufferNavigator.getDIFBufferStart()[d->getGetFramePtrReturn()]==0xa0 );
      }
      c.SizeAfterDIFPtr[bufferNavigator.getSizeAfterDIFPtr()]++;
      m_Destination.processDIF(d);
      for (uint32_t i=0;i<d->getNumberOfFrames();i++)
      {
        m_Destination.processFrame(d,i);
        for (uint32_t j=0;j<64;j++) m_Destination.processPadInFrame(d,i,j);
      }
    
      bool processSC=false;
      if (bufferNavigator.hasSlowControlData()) {c.hasSlowControl++;processSC=true;}
      if (bufferNavigator.badSCData()) {c.hasBadSlowControl++;processSC=false;}
      if (processSC) {m_Destination.processSlowControl(bufferNavigator.getSCBuffer());}
    
      SDHCAL_buffer eod=bufferNavigator.getEndOfAllData();
      c.SizeAfterAllData[eod.getsize()]++;
      unsigned char* debug_variable_3=eod.endOfBuffer();
      if (m_Verbose) m_VerboseOut << "END DATA BUFFER END " << (unsigned int *) debug_variable_1 << " " << (unsigned int *) debug_variable_3 << std::endl;
      if (m_Debug) assert (debug_variable_1 == debug_variable_3);
      if (m_Verbose) {m_VerboseOut << "End of Data remaining stuff : "; eod.printBuffer();}
    
      int nonzeroCount=0;
      for (unsigned char* it=eod.buffer(); it != eod.endOfBuffer(); it++)
      if (int(*it) !=0) nonzeroCount++;
      c.NonZeroValusAtEndOfData[nonzeroCount]++;
      m_NbrEvents++;
    } //end of while loop
  
    m_Destination.end();
    c.printAllCounters(m_DebugOut);
  }
private:
  SOURCE& m_Source;
  DESTINATION& m_Destination;
  bool m_Debug{false};
  std::ostream& m_DebugOut{std::cout};
  bool m_Verbose{false};
  std::ostream& m_VerboseOut{std::cout};
  std::uint32_t m_NbrEvents{1};
};
