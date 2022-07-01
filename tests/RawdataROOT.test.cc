/**
*  \file RawdataROOT.test.cc
*  \copyright 2022 G.Grenier F.Lagarde
*/

#include "BufferLooper.h"
#include "CLI/CLI.hpp"
#include "Filesystem.h"
#include "ROOTWriter.h"
#include "RawdataReader.h"

#include <limits>
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>

int main(int argc, char** argv)
{
  CLI::App app{"SDHCAL buffer loop with textDump destination"};
  app.set_version_flag("--version", streamout_version.to_string());
  std::string file{""};
  app.add_option("-f,--filename", file, "Path of the file");
  std::uint32_t eventNbr{std::numeric_limits<std::uint32_t>::max()};
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

  std::vector<DetectorID> detectorIDs{DetectorID::HARDROC, DetectorID::RUNHEADER};
  app.add_option("--detectorID", detectorIDs, "Detector IDs")
    ->transform(CLI::CheckedTransformer(std::map<std::string, DetectorID>({{"HARDROC", DetectorID::HARDROC}, {"HARDROC_NEW", DetectorID::HARDROC_NEW}, {"RUNHEADER", DetectorID::RUNHEADER}}), CLI::ignore_case));

  std::string output_path{path(argv[0])};
  app.add_option("--output_path", output_path, "Output path")->envname("STREAMOUT_OUTPUT_PATH");

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
  RawdataReader source(file.c_str());
  ROOTWriter    destination;
  destination.setFilename((output_path + "/" + filename(file) + ".root").c_str());
  BufferLooper<RawdataReader, ROOTWriter> toto(source, destination, debug);
  toto.setDetectorIDs(detectorIDs);
  toto.addSink(std::make_shared<spdlog::sinks::stdout_color_sink_mt>());
  // toto.addSink(std::make_shared<spdlog::sinks::basic_file_sink_mt>(filename + ".txt", true), spdlog::level::trace);
  toto.loop(eventNbr);
  toto.log()->warn("*** Counters ***");
  toto.printAllCounters();
  toto.log()->warn("****************");
}
