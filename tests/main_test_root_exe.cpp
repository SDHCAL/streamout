#include "SDHCAL_buffer_loop.h"
#include "ROOTtreeDest.h"
#include "DIFdataExample.h"

#include "TFile.h"
#include <iostream>
#include "CLI.hpp"

int main(int argc, char** argv)
{
  CLI::App app{"SDHCAL buffer loop with ROOT file destination"};
  std::uint32_t eventNbr{0};
  app.add_option("-e,--events",eventNbr, "Event number to process",0)->expected(1)->check(CLI::PositiveNumber);
  std::uint32_t bitsToSkip{0};
  app.add_option("-s,--skip",bitsToSkip, "Number of bits to skip from the DIF buffer",92)->expected(1)->check(CLI::PositiveNumber);
  std::string outputFileName{"output.root"};
  app.add_option("-o,--output",outputFileName, "Output file name","output.root")->expected(1);
  std::string writeOption{"RECREATE"};
  //To do ask CLI11 to check only the relevant strings
  app.add_option("-w,--writeOption",writeOption, "Root writing option","RECREATE")->expected(1);
  std::string title{"Test for SDHCAL raw data file"};
  app.add_option("-t,--title",title, "Root writing option","RECREATE")->expected(1);
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
  TFile hfile(outputFileName.c_str(),writeOption.c_str(),title.c_str());
  DIFdataExample source;
  ROOTtreeDest destination;
  SDHCAL_RawBuffer_Navigator::StartAt(bitsToSkip);
  SDHCAL_buffer_loop<DIFdataExample,ROOTtreeDest> toto(source,destination,debug,std::cout,verbose);
  toto.loop(eventNbr);
  std::cout<<"******************************"<<std::endl;
  toto.printAllCounters();
  std::cout<<"******************************"<<std::endl;
  hfile.Write();
  hfile.Close();
  return 0;
}
