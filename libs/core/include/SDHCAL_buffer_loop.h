/** \file SDHCAL_buffer_loop.h
 *  \copyright 2022 G.Grenier F.Lagarde
 */

#pragma once

#include "SDHCAL_RawBuffer_Navigator.h"
#include "Buffer.h"
#include "Formatters.h"
#include "SDHCAL_buffer_LoopCounter.h"

#include <cassert>
#include <iostream>
#include <ostream>
// function to loop on buffers
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

template<typename SOURCE, typename DESTINATION> class SDHCAL_buffer_loop
{
public:
  SDHCAL_buffer_loop(SOURCE& source, DESTINATION& dest, bool debug = false, std::ostream& out = std::cout, bool verbose = false, std::ostream& verbose_out = std::cout) :
    m_Source(source), m_Destination(dest), m_Debug(debug), m_DebugOut(out), m_Verbose(verbose), m_VerboseOut(verbose_out)
  {
  }
  void loop(const std::int32_t& m_NbrEventsToProcess = 0)
  {
    m_Destination.start();
    while(m_Source.nextEvent() && (m_NbrEventsToProcess == 0 || m_NbrEventsToProcess >= m_NbrEvents))
    {
      while(m_Source.nextDIFbuffer())
      {
        m_Source.getSDHCALBuffer(buffer);
        unsigned char*             debug_variable_1 = buffer.end();
        SDHCAL_RawBuffer_Navigator bufferNavigator(buffer);
        unsigned char*             debug_variable_2 = bufferNavigator.getDIFBuffer().end();
        if(m_Verbose) m_VerboseOut << "DIF BUFFER END " << (unsigned int*)debug_variable_1 << " " << (unsigned int*)debug_variable_2 << std::endl;
        if(m_Debug) assert(debug_variable_1 == debug_variable_2);
        uint32_t idstart = bufferNavigator.getStartOfDIF();
        if(m_Debug && idstart == 0) m_DebugOut << buffer <<std::endl;
        c.DIFStarter[idstart]++;
        if(!bufferNavigator.validBuffer()) continue;
        DIFPtr* d = bufferNavigator.getDIFPtr();
        if(m_Debug) assert(d != nullptr);
        if(d != nullptr)
        {
          c.DIFPtrValueAtReturnedPos[bufferNavigator.getDIFBufferStart()[d->getGetFramePtrReturn()]]++;
          if(m_Debug) assert(bufferNavigator.getDIFBufferStart()[d->getGetFramePtrReturn()] == 0xa0);
        }
        c.SizeAfterDIFPtr[bufferNavigator.getSizeAfterDIFPtr()]++;
        m_Destination.processDIF(d);
        for(uint32_t i = 0; i < d->getNumberOfFrames(); i++)
        {
          m_Destination.processFrame(d, i);
          for(uint32_t j = 0; j < 64; j++) m_Destination.processPadInFrame(d, i, j);
        }

        bool processSC = false;
        if(bufferNavigator.hasSlowControlData())
        {
          c.hasSlowControl++;
          processSC = true;
        }
        if(bufferNavigator.badSCData())
        {
          c.hasBadSlowControl++;
          processSC = false;
        }
        if(processSC) { m_Destination.processSlowControl(bufferNavigator.getSCBuffer()); }

        Buffer eod = bufferNavigator.getEndOfAllData();
        c.SizeAfterAllData[eod.size()]++;
        unsigned char* debug_variable_3 = eod.end();
        if(m_Verbose) m_VerboseOut << "END DATA BUFFER END " << (unsigned int*)debug_variable_1 << " " << (unsigned int*)debug_variable_3 << std::endl;
        if(m_Debug) assert(debug_variable_1 == debug_variable_3);
        if(m_Verbose)
        {
          m_VerboseOut << "End of Data remaining stuff : ";
          m_VerboseOut << eod <<std::endl;
        }

        int nonzeroCount = 0;
        for(unsigned char* it = eod.begin(); it != eod.end(); it++)
          if(static_cast<int>(*it) != 0) nonzeroCount++;
        c.NonZeroValusAtEndOfData[nonzeroCount]++;
      }  // end of DIF while loop
      m_NbrEvents++;
    }  // end of event while loop
    m_Destination.end();
  }
  void printAllCounters() { c.printAllCounters(m_DebugOut); }

private:
  Buffer buffer;
  SDHCAL_buffer_LoopCounter c;
  SOURCE&                   m_Source;
  DESTINATION&              m_Destination;
  bool                      m_Debug{false};
  std::ostream&             m_DebugOut{std::cout};
  bool                      m_Verbose{false};
  std::ostream&             m_VerboseOut{std::cout};
  std::uint32_t             m_NbrEvents{1};
};
