/** \file SDHCAL_buffer_loop.h
 *  \copyright 2022 G.Grenier F.Lagarde
 */

#pragma once

#include "Buffer.h"
#include "Formatters.h"
#include "SDHCAL_RawBuffer_Navigator.h"
#include "SDHCAL_buffer_LoopCounter.h"

#include <cassert>
#include <memory>
#include <spdlog/sinks/null_sink.h>
#include <spdlog/spdlog.h>
#include <vector>

// function to loop on buffers
//
// template class should implement
// void SOURCE::start();
// bool SOURCE::next();
// void SOURCE::end();
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
  SDHCAL_buffer_loop(SOURCE& source, DESTINATION& dest, bool debug = false) : m_Source(source), m_Destination(dest), m_Debug(debug)
  {
    m_Logger = spdlog::create<spdlog::sinks::null_sink_mt>("streamout");
    if(!spdlog::get("streamout")) { spdlog::register_logger(m_Logger); }
    m_Source.setLogger(m_Logger);
    m_Destination.setLogger(m_Logger);
  }

  void addSink(const spdlog::sink_ptr& sink, const spdlog::level::level_enum& level = spdlog::get_level())
  {
    sink->set_level(level);
    m_Sinks.push_back(sink);
    m_Logger = std::make_shared<spdlog::logger>("streamout", begin(m_Sinks), end(m_Sinks));
    m_Source.setLogger(m_Logger);
    m_Destination.setLogger(m_Logger);
  }

  void loop(const std::int32_t& m_NbrEventsToProcess = 0)
  {
    m_Source.start();
    m_Destination.start();
    while(m_Source.nextEvent() && (m_NbrEventsToProcess == 0 || m_NbrEventsToProcess >= m_NbrEvents))
    {
      m_Logger->warn("===*** Event number {} ***===", m_NbrEvents);
      while(m_Source.nextDIFbuffer())
      {
        const Buffer&              buffer           = m_Source.getSDHCALBuffer();
        bit8_t*             debug_variable_1 = buffer.end();
        SDHCAL_RawBuffer_Navigator bufferNavigator(buffer);
        bit8_t*             debug_variable_2 = bufferNavigator.getDIFBuffer().end();
        m_Logger->info("DIF BUFFER END {} {}", fmt::ptr(debug_variable_1), fmt::ptr(debug_variable_2));
        if(m_Debug) assert(debug_variable_1 == debug_variable_2);
        uint32_t idstart = bufferNavigator.getStartOfDIF();
        if(m_Debug && idstart == 0) m_Logger->info(to_hex(buffer));
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
        m_Logger->info("END DATA BUFFER END {} {}", fmt::ptr(debug_variable_1), fmt::ptr(debug_variable_3));
        if(m_Debug) assert(debug_variable_1 == debug_variable_3);
        m_Logger->info("End of Data remaining stuff : {}", to_hex(eod));

        int nonzeroCount = 0;
        for(unsigned char* it = eod.begin(); it != eod.end(); it++)
          if(static_cast<int>(*it) != 0) nonzeroCount++;
        c.NonZeroValusAtEndOfData[nonzeroCount]++;
      }  // end of DIF while loop
      m_Logger->warn("***=== Event number {} ===***", m_NbrEvents);
      m_NbrEvents++;
    }  // end of event while loop
    m_Destination.end();
    m_Source.end();
  }
  void                            printAllCounters() { c.printAllCounters(m_Logger); }
  std::shared_ptr<spdlog::logger> log() { return m_Logger; }

private:
  std::shared_ptr<spdlog::logger> m_Logger{nullptr};
  std::vector<spdlog::sink_ptr>   m_Sinks;
  SDHCAL_buffer_LoopCounter       c;
  SOURCE&                         m_Source{nullptr};
  DESTINATION&                    m_Destination{nullptr};
  bool                            m_Debug{false};
  std::uint32_t                   m_NbrEvents{1};
};
