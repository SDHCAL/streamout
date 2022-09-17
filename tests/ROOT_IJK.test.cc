/** \file ROOTchecker.test.cc
*  \copyright 2022 G.Grenier F.Lagarde
*/

#include "AppVersion.h"
#include "CLI/CLI.hpp"
#include "DIF.h"
#include "Event.h"
#include "Filesystem.h"
#include "TBrowser.h"
#include "TFile.h"
#include "TTree.h"
#include "spdlog/spdlog.h"

#include <map>

//      (I Axis)
// 7  |46|63|61|59|58|56|54|52|
// 6  |43|45|62|60|57|55|53|51|
// 5  |42|44|47|48|49|50|41|40|
// 4  |32|33|34|35|36|37|38|39|
// 3  |31|30|29|28|27|26|25|24|         TOP VIEW (ASICs SIDE)
// 2  |20|19|16|15|14|21|22|23|
// 1  |18|00|02|04|06|09|11|13|
// 0  |17|01|03|05|07|08|10|12|
//     0  1  2  3  4  5  6  7    (J Axis)  ----->
//        |   |
//        |DIFF|
//        |____|
//=====================================

const std::int8_t MapILargeHR2[64] = {1, 0, 1, 0, 1, 0, 1, 0, 0, 1, 0, 1, 0, 1, 2, 2, 2, 0, 1, 2, 2, 2, 2, 2, 3, 3, 3, 3, 3, 3, 3, 3, 4, 4, 4, 4, 4, 4, 4, 4, 5, 5, 5, 6, 5, 6, 7, 5, 5, 5, 5, 6, 7, 6, 7, 6, 7, 6, 7, 7, 6, 7, 6, 7};
const std::int8_t AsicShiftI[49]   = {0, 0, 0, 0, 0, 8, 8, 8, 8, 16, 16, 16, 16, 24, 24, 24, 24, 32, 32, 32, 32, 40, 40, 40, 40, 48, 48, 48, 48, 56, 56, 56, 56, 64, 64, 64, 64, 72, 72, 72, 72, 80, 80, 80, 80, 88, 88, 88, 88};

const std::int8_t MapJLargeHR2[64] = {1, 1, 2, 2, 3, 3, 4, 4, 5, 5, 6, 6, 7, 7, 4, 3, 2, 0, 0, 1, 0, 5, 6, 7, 7, 6, 5, 4, 3, 2, 1, 0, 0, 1, 2, 3, 4, 5, 6, 7, 7, 6, 0, 0, 1, 1, 0, 2, 3, 4, 5, 7, 7, 6, 6, 5, 5, 4, 4, 3, 3, 2, 2, 1};
const std::int8_t AsicShiftJ[49]   = {0, 0, 8, 16, 24, 24, 16, 8, 0, 0, 8, 16, 24, 24, 16, 8, 0, 0, 8, 16, 24, 24, 16, 8, 0, 0, 8, 16, 24, 24, 16, 8, 0, 0, 8, 16, 24, 24, 16, 8, 0, 0, 8, 16, 24, 24, 16, 8, 0};

const std::int8_t MapChannelLargeHR2[8][8] = {{17, 1, 3, 5, 7, 8, 10, 12},      {18, 0, 2, 4, 6, 9, 11, 13},      {20, 19, 16, 15, 14, 21, 22, 23}, {31, 30, 29, 28, 27, 26, 25, 24},
                                              {32, 33, 34, 35, 36, 37, 38, 39}, {42, 44, 47, 48, 49, 50, 41, 40}, {43, 45, 62, 60, 57, 55, 53, 51}, {46, 63, 61, 59, 58, 56, 54, 52}};

const std::int8_t MapAsicLargeHR2[12][4] = {{4, 3, 2, 1}, {5, 6, 7, 8}, {12, 11, 10, 9}, {13, 14, 15, 16}, {20, 19, 18, 17}, {21, 22, 23, 24}, {28, 27, 26, 25}, {29, 30, 31, 32}, {36, 35, 34, 33}, {37, 38, 39, 40}, {44, 43, 42, 41}, {45, 46, 47, 48}};

class ASIC_Coordinate_Translator
{
public:
  // translate stuff for HR2 pad ASU
  static unsigned int I(unsigned int asic, unsigned int channel) { return 1 + MapILargeHR2[channel] + AsicShiftI[asic]; }
  static unsigned int J(unsigned int asic, unsigned int channel) { return 32 - (MapJLargeHR2[channel] + AsicShiftJ[asic]); }
  static unsigned int asic(unsigned int I, unsigned int J) { return MapAsicLargeHR2[(I - 1) / 8][(J - 1) / 8]; }
  static unsigned int channel(unsigned int I, unsigned int J) { return MapChannelLargeHR2[(I - 1) % 8][J % 8 == 0 ? 0 : 8 - J % 8]; }
};

class Flatter
{
public:
  Int_t event;
  Int_t dif;
  Int_t asic;
  Int_t channel;
  Int_t I;
  Int_t J;
  Int_t K;
  Int_t threshold;
  Int_t timestamp;
};

std::map<int, std::pair<int, int>> geometry{
  // Chamber
  {30, {1, 0}},
  {94, {1, 1}},
  {181, {1, 2}},
  //
  // Chamber
  {100, {2, 0}},
  {97, {2, 1}},
  {92, {2, 2}},
  //
  // Chamber
  {40, {3, 0}},
  {93, {3, 1}},
  {98, {3, 2}},
  //
  // Chamber
  {119, {4, 0}},
  {118, {4, 1}},
  {129, {4, 2}},
  //
  // Chamber
  {151, {5, 0}},
  {152, {5, 1}},
  {164, {5, 2}},
  //
  // Chamber
  {75, {6, 0}},
  {61, {6, 1}},
  {74, {6, 2}},
  //
  // Chamber
  {110, {7, 0}},
  {111, {7, 1}},
  {156, {7, 2}},
  //
  // Chamber
  {103, {8, 0}},
  {177, {8, 1}},
  {102, {8, 2}},
  //
  // Chamber
  {176, {9, 0}},
  {175, {9, 1}},
  {174, {9, 2}},
  //
  // Chamber
  {121, {10, 0}},
  {120, {10, 1}},
  {128, {10, 2}},
  //
  // Chamber
  {58, {11, 0}},
  {64, {11, 1}},
  {65, {11, 2}},
  //
  // Chamber
  {73, {12, 0}},
  {72, {12, 1}},
  {148, {12, 2}},
  //
  // Chamber
  {60, {13, 0}},
  {79, {13, 1}},
  {78, {13, 2}},
  //
  // Chamber
  {113, {14, 0}},
  {43, {14, 1}},
  {44, {14, 2}},
  //
  // Chamber
  {243, {15, 0}},
  {242, {15, 1}},
  {241, {15, 2}},
  //
  // Chamber
  {186, {16, 0}},
  {127, {16, 1}},
  {154, {16, 2}},
  //
  // Chamber
  {71, {17, 0}},
  {70, {17, 1}},
  {147, {17, 2}},
  //
  // Chamber
  {139, {18, 0}},
  {140, {18, 1}},
  {47, {18, 2}},
  //
  // Chamber
  {76, {19, 0}},
  {77, {19, 1}},
  {143, {19, 2}},
  //
  // Chamber
  {36, {20, 0}},
  {91, {20, 1}},
  {159, {20, 2}},
  //
  // Chamber
  {183, {21, 0}},
  {178, {21, 1}},
  {179, {21, 2}},
  //
  // Chamber
  {67, {22, 0}},
  {42, {22, 1}},
  {41, {22, 2}},
  //
  // Chamber
  {138, {23, 0}},
  {46, {23, 1}},
  {137, {23, 2}},
  //
  // Chamber
  {144, {24, 0}},
  {173, {24, 1}},
  {131, {24, 2}},
  //
  // Chamber
  {160, {25, 0}},
  {184, {25, 1}},
  {189, {25, 2}},
  //
  // Chamber
  {171, {26, 0}},
  {167, {26, 1}},
  {172, {26, 2}},
  //
  // Chamber
  {145, {27, 0}},
  {135, {27, 1}},
  {146, {27, 2}},
  //
  // Chamber
  {180, {28, 0}},
  {170, {28, 1}},
  {185, {28, 2}},
  //
  // Chamber
  {190, {29, 0}},
  {188, {29, 1}},
  {187, {29, 2}},
  //
  // Chamber
  {166, {30, 0}},
  {165, {30, 1}},
  {169, {30, 2}},
  //
  // Chamber
  {50, {31, 0}},
  {57, {31, 1}},
  {155, {31, 2}},
  //
  // Chamber
  {153, {32, 0}},
  {108, {32, 1}},
  {25, {32, 2}},
  //
  // Chamber
  {109, {33, 0}},
  {56, {33, 1}},
  {51, {33, 2}},
  //
  // Chamber
  {107, {34, 0}},
  {150, {34, 1}},
  {116, {34, 2}},
  //
  // Chamber
  {49, {35, 0}},
  {124, {35, 1}},
  {126, {35, 2}},
  //
  // Chamber
  {115, {36, 0}},
  {149, {36, 1}},
  {117, {36, 2}},
  //
  // Chamber
  {114, {37, 0}},
  {45, {37, 1}},
  {48, {37, 2}},
  //
  // Bifs
  {99, {-1, 0}},
  {128, {-2, 0}},
};

int main(int argc, char** argv)
{
  CLI::App app{"ROOTfile checker"};
  app.set_version_flag("--version", streamout_version.to_string());
  std::string file{""};
  app.add_option("-f,--filename", file, "Path of the file");
  try
  {
    app.parse(argc, argv);
  }
  catch(const CLI::ParseError& e)
  {
    return app.exit(e);
  }

  TFile   f(file.c_str());
  TFile*  m_File = TFile::Open((filename(file) + "IJK.root").c_str(), "RECREATE", (filename(file) + "IJK.root").c_str(), ROOT::CompressionSettings(ROOT::kZLIB, 5));
  TTree*  m_Tree = new TTree("IJK", "Raw SDHCAL data tree with IJK");
  Flatter flatter;
  m_Tree->Branch("Events", &flatter, "event/I:dif/I:asic/I:channel/I:I/I:J/I:K/I:threshold/I:timestamp/I");
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
      // spdlog::info("DIF_ID : {}, DTC : {}, GTC : {}, DIF BCID {}, Absolute BCID : {}", DIF->second.getID(), DIF->second.getDTC(), DIF->second.getGTC(), DIF->second.getDIFBCID(), DIF->second.getAbsoluteBCID());
      for(Hits_const_iterator hit = DIF->second.cbegin(); hit != DIF->second.cend(); ++hit)
      {
        if(geometry.find(hit->getDIFid()) != geometry.end())
        {
          flatter.event     = event->getEventNumber();
          flatter.dif       = hit->getDIFid();
          flatter.asic      = hit->getASICid();
          flatter.channel   = hit->getChannel();
          flatter.I         = ASIC_Coordinate_Translator::I(hit->getASICid(), hit->getChannel());
          flatter.J         = ASIC_Coordinate_Translator::J(hit->getASICid(), hit->getChannel()) + 32 * geometry[hit->getDIFid()].second;
          flatter.K         = geometry[hit->getDIFid()].first;
          flatter.threshold = hit->getThreshold();
          flatter.timestamp = hit->getTimestamp();
          m_Tree->Fill();
        }
        else
          spdlog::critical("Dif id {} unknown in your geometry ! You are doing *!@#$&*^ ! Check it twice ! I will skip that hit !!!", hit->getDIFid());

        // spdlog::info("\tDIF ID : {}; ASIC ID : {}; Channel ID : {}; Threshold : {}; FrameBCID : {}; Timestamp : {}", hit->getDIFid(), hit->getASICid(), hit->getChannel(), hit->getThreshold(), hit->getFrameBCID(), hit->getTimestamp());
      }
    }
  }
  m_File->Write();
  m_File->Close();
  delete m_File;
}
