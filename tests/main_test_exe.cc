/**
*  \file main_test_exe.cc
*  \copyright 2022 G.Grenier F.Lagarde
*/

#include "CLI/CLI.hpp"
#include "DIFdataExample.h"
#include "SDHCAL_buffer_loop.h"
#include "textDump.h"

#include <spdlog/sinks/stdout_color_sinks.h>

int main(int argc, char** argv)
{
  CLI::App      app{"SDHCAL buffer loop with textDump destination"};
  std::uint32_t eventNbr{0};
  app.add_option("-e,--events", eventNbr, "Event number to process")->expected(1)->check(CLI::PositiveNumber);
  std::uint32_t bitsToSkip{92};
  app.add_option("-s,--skip", bitsToSkip, "Number of bits to skip from the DIF buffer")->expected(1)->check(CLI::PositiveNumber);
  bool verbose{false};
  app.add_flag("-v,--verbose", verbose, "Set verbosity");
  bool debug{false};
  app.add_flag("-d,--debug", verbose, "Set debug");
  try
  {
    app.parse(argc, argv);
  }
  catch(const CLI::ParseError& e)
  {
    return app.exit(e);
  }
  SDHCAL_RawBuffer_Navigator::StartAt(bitsToSkip);
  DIFdataExample                               source;
  textDump                                     destination;
  SDHCAL_buffer_loop<DIFdataExample, textDump> toto(source, destination, debug);
  toto.addSink(std::make_shared<spdlog::sinks::stdout_color_sink_mt>(), spdlog::level::info);
  toto.loop(eventNbr);
  toto.log()->info("******************************");
  toto.printAllCounters();
  toto.log()->info("******************************");
  return 0;
}
