/**
*  \file main_test_exe.cc
*  \copyright 2022 G.Grenier F.Lagarde
*/

#include "BufferLooper.h"
#include "CLI/CLI.hpp"
#include "DIFdataExample.h"
#include "textDump.h"

#include <spdlog/sinks/stdout_color_sinks.h>

int main(int argc, char** argv)
{
  CLI::App app{"SDHCAL buffer loop with textDump destination"};
  app.set_version_flag("--version", streamout_version.to_string());
  std::uint32_t eventNbr{std::numeric_limits<std::uint32_t>::max()};
  app.add_option("-e,--events", eventNbr, "Event number to process")->expected(1)->check(CLI::PositiveNumber);
  std::uint32_t bitsToSkip{20};
  app.add_option("-s,--skip", bitsToSkip, "Number of bits to skip from the DIF buffer")->expected(1)->check(CLI::PositiveNumber);
  spdlog::level::level_enum verbosity{spdlog::level::trace};
  app.add_option("-v,--verbosity", verbosity, "Verbosity level.")
    ->transform(CLI::CheckedTransformer(
      std::map<std::string, spdlog::level::level_enum>(
        {{"off", spdlog::level::off}, {"trace", spdlog::level::trace}, {"debug", spdlog::level::debug}, {"info", spdlog::level::info}, {"warn", spdlog::level::warn}, {"error", spdlog::level::err}, {"critical", spdlog::level::critical}}),
      CLI::ignore_case));
  bool debug{false};
  app.add_flag("-d,--debug", debug, "Set debug");
  try
  {
    app.parse(argc, argv);
  }
  catch(const CLI::ParseError& e)
  {
    return app.exit(e);
  }

  spdlog::set_level(verbosity);

  RawBufferNavigator::startAt(bitsToSkip);
  DIFdataExample                         source;
  textDump                               destination;
  BufferLooper<DIFdataExample, textDump> toto(source, destination, debug);
  toto.addSink(std::make_shared<spdlog::sinks::stdout_color_sink_mt>());
  toto.loop(eventNbr);
  toto.printAllCounters();
  return 0;
}
