/**
*  \file main_test_exe.cc
*  \copyright 2022 G.Grenier F.Lagarde
*/

#include "RawdataReader.h"

#include "CLI/CLI.hpp"
#include "SDHCAL_buffer_loop.h"
#include "textDump.h"

#include <iostream>

int main(int argc, char** argv)
{
  CLI::App    app{"SDHCAL buffer loop with textDump destination"};
  std::string filename{""};
  app.add_option("-f,--filename", filename, "Path of the file");
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
  RawdataReader                               source(filename.c_str());
  textDump                                    destination;
  SDHCAL_buffer_loop<RawdataReader, textDump> toto(source, destination, debug, std::cout, verbose);
  toto.loop(eventNbr);
  std::cout << "******************************" << std::endl;
  toto.printAllCounters();
  std::cout << "******************************" << std::endl;
  return 0;
}
