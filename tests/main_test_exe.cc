#include "SDHCAL_buffer_loop.h"
#include "textDump.h"
#include "DIFdataExample.h"
#include <iostream>
#include "CLI.hpp"

int main(int argc, char** argv)
{
  CLI::App app{"SDHCAL buffer loop with textDump destination"};
  std::uint32_t eventNbr{0};
  app.add_option("-e,--events",eventNbr, "Event number to process",0)->expected(1)->check(CLI::PositiveNumber);
  std::uint32_t bitsToSkip{0};
  app.add_option("-s,--skip",bitsToSkip, "Number of bits to skip from the DIF buffer",92)->expected(1)->check(CLI::PositiveNumber);
  bool verbose{false};
  app.add_flag("-v,--verbose",verbose,"Set verbosity");
  bool debug{false};
  app.add_flag("-d,--debug",verbose,"Set debug");
  try
  {
    app.parse(argc, argv);
  }
  catch (const CLI::ParseError &e)
  {
    return app.exit(e);
  }
  SDHCAL_RawBuffer_Navigator::StartAt(bitsToSkip);
  DIFdataExample source;
  textDump destination;
  SDHCAL_buffer_loop<DIFdataExample,textDump> toto(source,destination,debug,std::cout,verbose);
  toto.loop(eventNbr);
  std::cout<<"******************************"<<std::endl;
  toto.printAllCounters();
  std::cout<<"******************************"<<std::endl;
  return 0;
}
