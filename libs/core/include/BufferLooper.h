/** \file BufferLooper.h
*  \copyright 2022 G.Grenier F.Lagarde
*/

#pragma once

#include "AppVersion.h"
#include "Buffer.h"
#include "BufferLooperCounter.h"
#include "DetectorId.h"
#include "Formatters.h"
#include "PayloadLoader.h"
#include "RawBufferNavigator.h"
#include "Timer.h"
#include "Words.h"

#include <algorithm>
#include <cassert>
#include <fmt/color.h>
#include <map>
#include <memory>
#include <spdlog/sinks/null_sink.h>
#include <spdlog/spdlog.h>
#include <string>
#include <vector>
// function to loop on buffers

template<typename SOURCE, typename DESTINATION> class BufferLooper
{
public:
  BufferLooper(SOURCE& source, DESTINATION& dest, bool debug = false) : m_Source(source), m_Destination(dest), m_Debug(debug)
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

  void loop(const std::uint32_t& m_NbrEventsToProcess = 0)
  {
    // clang-format off
    fmt::print(fg(fmt::color::medium_orchid) | fmt::emphasis::bold,
            "\n"
" SSSSSSSSSSSSSSS      tttt                                                                                                                                   tttt\n"
"SS:::::::::::::::S  ttt:::t                                                                                                                                ttt:::t\n"
"S:::::SSSSSS::::::S  t:::::t                                                                                                                                t:::::t\n"
"S:::::S     SSSSSSS  t:::::t                                                                                                                                t:::::t\n"
"S:::::S        ttttttt:::::ttttttt   rrrrr   rrrrrrrrr       eeeeeeeeeeee    aaaaaaaaaaaaa      mmmmmmm    mmmmmmm      ooooooooooo   uuuuuu    uuuuuuttttttt:::::ttttttt\n"
"S:::::S        t:::::::::::::::::t   r::::rrr:::::::::r    ee::::::::::::ee  a::::::::::::a   mm:::::::m  m:::::::mm  oo:::::::::::oo u::::u    u::::ut:::::::::::::::::t\n"
" S::::SSSS     t:::::::::::::::::t   r:::::::::::::::::r  e::::::eeeee:::::eeaaaaaaaaa:::::a m::::::::::mm::::::::::mo:::::::::::::::ou::::u    u::::ut:::::::::::::::::t\n"
"  SS::::::SSSSStttttt:::::::tttttt   rr::::::rrrrr::::::re::::::e     e:::::e         a::::a m::::::::::::::::::::::mo:::::ooooo:::::ou::::u    u::::utttttt:::::::tttttt\n"
"    SSS::::::::SS    t:::::t          r:::::r     r:::::re:::::::eeeee::::::e  aaaaaaa:::::a m:::::mmm::::::mmm:::::mo::::o     o::::ou::::u    u::::u      t:::::t\n"
"       SSSSSS::::S   t:::::t          r:::::r     rrrrrrre:::::::::::::::::e aa::::::::::::a m::::m   m::::m   m::::mo::::o     o::::ou::::u    u::::u      t:::::t\n"
"            S:::::S  t:::::t          r:::::r            e::::::eeeeeeeeeee a::::aaaa::::::a m::::m   m::::m   m::::mo::::o     o::::ou::::u    u::::u      t:::::t\n"
"            S:::::S  t:::::t    ttttttr:::::r            e:::::::e         a::::a    a:::::a m::::m   m::::m   m::::mo::::o     o::::ou:::::uuuu:::::u      t:::::t    tttttt\n"
"SSSSSSS     S:::::S  t::::::tttt:::::tr:::::r            e::::::::e        a::::a    a:::::a m::::m   m::::m   m::::mo:::::ooooo:::::ou:::::::::::::::uu    t::::::tttt:::::t\n"
"S::::::SSSSSS:::::S  tt::::::::::::::tr:::::r             e::::::::eeeeeeeea:::::aaaa::::::a m::::m   m::::m   m::::mo:::::::::::::::o u:::::::::::::::u    tt::::::::::::::t\n"
"S:::::::::::::::SS     tt:::::::::::ttr:::::r              ee:::::::::::::e a::::::::::aa:::am::::m   m::::m   m::::m oo:::::::::::oo   uu::::::::uu:::u      tt:::::::::::tt\n"
" SSSSSSSSSSSSSSS         ttttttttttt  rrrrrrr                eeeeeeeeeeeeee  aaaaaaaaaa  aaaammmmmm   mmmmmm   mmmmmm   ooooooooooo       uuuuuuuu  uuuu        ttttttttttt {}\n"
"\n",
fmt::format(fg(fmt::color::red) | fmt::emphasis::bold, "v{}", streamout_version.to_string()));
    // clang-format on
    log()->info("*******************************************************************");
    log()->info("Streamout Version : {}", streamout_version.to_string());
    log()->info("Using InterfaceReader {} version {}", m_Source.getName(), m_Source.getVersion().to_string());
    log()->info("Using InterfaceWriter {} version {}", m_Destination.getName(), m_Destination.getVersion().to_string());

    if(!m_Destination.checkCompatibility(m_Source.getName(), m_Source.getVersion().to_string()))
    {
      log()->critical("{} version {} is not compatible with {} version {} ! ", m_Source.getName(), m_Source.getVersion().to_string(), m_Destination.getName(), m_Destination.getVersion().to_string());
      log()->info("Compatible Interfaces for {} are", m_Destination.getName());
      for(std::map<std::string, std::string>::iterator it = m_Destination.getCompatibility().begin(); it != m_Destination.getCompatibility().end(); ++it) { log()->info("{} version {}", it->first, it->second); }
      std::exit(-1);
    }
    if(!m_DetectorIDs.empty())
    {
      std::string ids;
      for(std::vector<DetectorID>::const_iterator it = m_DetectorIDs.cbegin(); it != m_DetectorIDs.cend(); ++it) ids += std::to_string(static_cast<std::uint16_t>(*it)) + ";";
      log()->info("Detector ID(s) other than {} will be ignored", ids);
    }
    log()->info("*******************************************************************");
    RawBufferNavigator bufferNavigator;
    Timer              timer;
    timer.start();
    m_Source.start();
    m_Destination.start();
    while(m_Source.nextEvent() && m_NbrEventsToProcess >= m_NbrEvents)
    {
      /*******************/
      /*** START EVENT ***/
      m_Source.startEvent();
      m_Destination.startEvent();
      /*******************/

      m_Logger->warn("===*** Event {} ***===", m_NbrEvents);
      while(m_Source.nextDIFbuffer())
      {
        const Buffer& buffer = m_Source.getBuffer();

        bufferNavigator.setBuffer(buffer);
        if(std::find(m_DetectorIDs.begin(), m_DetectorIDs.end(), static_cast<DetectorID>(bufferNavigator.getDetectorID())) == m_DetectorIDs.end())
        {
          m_Logger->debug("Ignoring detector ID : {}", bufferNavigator.getDetectorID());
          continue;
        }

        std::int32_t idstart = bufferNavigator.getStartOfPayload();
        if(m_Debug && idstart == -1) m_Logger->info(to_hex(buffer));
        c.DIFStarter[idstart]++;
        if(!bufferNavigator.validPayload())
        {
          m_Logger->error("!bufferNavigator.validBuffer()");
          continue;
        }

        /******************/
        /*** START DIF ***/
        m_Source.startDIF();
        m_Destination.startDIF();
        /******************/

        PayloadLoader payloadLoader;

        Payload100* d = payloadLoader.getPayload(bufferNavigator.getDetectorID());
        if(d == nullptr)
        {
          m_Logger->error("streamout don't know how to parse the payload for detector_id {} ! SKIPPING !", bufferNavigator.getDetectorID());
          continue;
        }

        // This is really a big error so skip DIF entirely if exception occurs
        try
        {
          d->setBuffer(bufferNavigator.getPayload());
        }
        catch(const Exception& e)
        {
          m_Logger->error("{}", e.what());
          continue;
        }

        if(buffer.end() != d->end()) m_Logger->error("DIF BUFFER END {} {}", fmt::ptr(buffer.end()), fmt::ptr(d->end()));
        assert(buffer.end() == d->end());

        c.DIFPtrValueAtReturnedPos[d->begin()[d->getEndOfDIFData() - 3]]++;
        assert(d->begin()[d->getEndOfDIFData() - 3] == 0xa0);

        c.SizeAfterDIFPtr[d->getSizeAfterDIFPtr()]++;
        m_Destination.processDIF(*d);
        for(std::size_t i = 0; i < d->getNumberOfFrames(); ++i)
        {
          /*******************/
          /*** START FRAME ***/
          m_Source.startFrame();
          m_Destination.startFrame();
          /*******************/
          m_Destination.processFrame(*d, i);
          for(std::size_t j = 0; j < static_cast<std::size_t>(Hardware::NUMBER_PAD); ++j)
          {
            if(d->getThresholdStatus(i, j) != 0)
            {
              m_Source.startPad();
              m_Destination.startPad();
              m_Destination.processPadInFrame(*d, i, j);
              m_Source.endPad();
              m_Destination.endPad();
            }
          }
          /*****************/
          /*** END FRAME ***/
          m_Source.endFrame();
          m_Destination.endFrame();
          /*****************/
        }
        // If I want SlowControl I need to check for it first, If there is an error then it's not a big deal just continue and say is bad SlowControl
        /*try
        {
          d.setSCBuffer();
        }
        catch(const Exception& e)
        {
          m_Logger->error("{}", e.what());
        }

        bool processSC = false;
        if(d.hasSlowControl())
        {
          c.hasSlowControl++;
          processSC = true;
        }
        if(d.badSCData())
        {
          c.hasBadSlowControl++;
          processSC = false;
        }
        if(processSC) { m_Destination.processSlowControl(d.getSCBuffer()); }*/

        // Buffer eod = d.getEndOfAllData();
        // c.SizeAfterAllData[eod.size()]++;
        // bit8_t* debug_variable_3 = eod.end();
        // if(buffer.end() != debug_variable_3) m_Logger->info("END DATA BUFFER END {} {}", fmt::ptr(buffer.end()), fmt::ptr(debug_variable_3));
        // assert(buffer.end() == debug_variable_3);
        // if(eod.size() != 0) m_Logger->info("End of Data remaining stuff : {}", to_hex(eod));*/

        /*int nonzeroCount = 0;
        for(bit8_t* it = eod.begin(); it != eod.end(); it++)
          if(static_cast<int>(*it) != 0) nonzeroCount++;
        c.NonZeroValusAtEndOfData[nonzeroCount]++;*/

        /***************/
        /*** END DIF ***/
        m_Source.endDIF();
        m_Destination.endDIF();
        /***************/
      }  // end of DIF while loop
      m_Logger->warn("===*** Event {} ***===", m_NbrEvents);
      m_NbrEvents++;
      /*****************/
      /*** END EVENT ***/
      m_Source.endEvent();
      m_Destination.endEvent();
      /*****************/
    }  // end of event while loop
    m_Destination.end();
    m_Source.end();
    timer.stop();
    fmt::print(fg(fmt::color::green) | fmt::emphasis::bold, "=== elapsed time {}ms ({}ms/event) ===\n", timer.getElapsedTime() / 1000, timer.getElapsedTime() / (1000 * m_NbrEvents));
  }
  void                            printAllCounters() { c.printAllCounters(); }
  std::shared_ptr<spdlog::logger> log() { return m_Logger; }

  void setDetectorIDs(const std::vector<DetectorID>& detectorIDs) { m_DetectorIDs = detectorIDs; }

private:
  std::vector<DetectorID>         m_DetectorIDs;
  std::shared_ptr<spdlog::logger> m_Logger{nullptr};
  std::vector<spdlog::sink_ptr>   m_Sinks;
  BufferLooperCounter             c;
  SOURCE&                         m_Source{nullptr};
  DESTINATION&                    m_Destination{nullptr};
  bool                            m_Debug{false};
  std::uint32_t                   m_NbrEvents{1};
};
