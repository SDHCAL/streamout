/** \file ROOTchecker.test.cc
*  \copyright 2022 G.Grenier F.Lagarde
*/

#include "AppVersion.h"
#include "CLI/CLI.hpp"
#include "DIF.h"
#include "Event.h"
#include "TBrowser.h"
#include "TFile.h"
#include "TTree.h"
#include "spdlog/spdlog.h"

int main(int argc, char** argv)
{
  CLI::App app{"ROOTfile checker"};
  app.set_version_flag("--version", streamout_version.to_string());
  std::string file{""};
  app.add_option("-f,--filename", file, "Path of the file")->required();
  try
  {
    app.parse(argc, argv);
  }
  catch(const CLI::ParseError& e)
  {
    return app.exit(e);
  }

  TFile  f(file.c_str());
  TTree* Run = static_cast<TTree*>(f.Get("RawData"));
  if(Run == nullptr || Run->IsZombie())
  {
    std::cout << "Problem Opening TTree \"RawData\" !!!" << std::endl;
    std::exit(-4);
  }
  Long64_t NEntries = Run->GetEntries();
  Event*   event{nullptr};
  if(Run->SetBranchAddress("Events", &event))
  {
    std::cout << "Error while SetBranchAddress !!!" << std::endl;
    std::exit(-5);
  }
  for(Long64_t evt = 0; evt < NEntries; ++evt)
  {
    // Loop on events see event.hpp
    event->clear();
    Run->GetEntry(evt);
    for(DIFs_const_iterator DIF = event->cbegin(); DIF != event->cend(); ++DIF)
    {
      spdlog::info("DIF_ID : {}, DTC : {}, GTC : {}, DIF BCID {}, Absolute BCID : {}", DIF->second.getID(), DIF->second.getDTC(), DIF->second.getGTC(), DIF->second.getDIFBCID(), DIF->second.getAbsoluteBCID());
      for(Hits_const_iterator hit = DIF->second.cbegin(); hit != DIF->second.cend(); ++hit)
      {
        spdlog::info("\tDIF ID : {}; ASIC ID : {}; Channel ID : {}; Threshold : {}; FrameBCID : {}; Timestamp : {}", hit->getDIFid(), hit->getASICid(), hit->getChannel(), hit->getThreshold(), hit->getFrameBCID(), hit->getTimestamp());
      }
    }
  }
}
