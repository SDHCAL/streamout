/**
*  \file main_test_exe.cc
*  \copyright 2022 G.Grenier F.Lagarde
*/

#include "RawdataReader.h"

#include "CLI/CLI.hpp"
#include "SDHCAL_buffer_loop.h"
#include "textDump.h"

#include <iostream>
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>

int main(int argc, char** argv)
{
  CLI::App    app{"SDHCAL buffer loop with textDump destination"};
  std::string filename{""};
  app.add_option("-f,--filename", filename, "Path of the file");
  std::uint32_t eventNbr{0};
  app.add_option("-e,--events", eventNbr, "Event number to process")->expected(1)->check(CLI::PositiveNumber);
  std::uint32_t bitsToSkip{92};
  app.add_option("-s,--skip", bitsToSkip, "Number of bits to skip from the DIF buffer")->expected(1);
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

  RawBufferNavigator::StartAt(bitsToSkip);
  RawdataReader source(filename.c_str());
  textDump      destination;
  destination.setLevel(spdlog::level::trace);
  SDHCAL_buffer_loop<RawdataReader, textDump> toto(source, destination, debug);

  toto.addSink(std::make_shared<spdlog::sinks::stdout_color_sink_mt>());
  toto.addSink(std::make_shared<spdlog::sinks::basic_file_sink_mt>(filename + ".txt", true), spdlog::level::trace);
  toto.loop(eventNbr);
  std::cout << "******************************" << std::endl;
  toto.printAllCounters();
  std::cout << "******************************" << std::endl;
}
