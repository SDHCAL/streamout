#include "SDHCAL_buffer_loop.h"
#include "ROOTtreeDest.h"
#include "DIFdataExample.h"

#include "TFile.h"
#include <iostream>
int main()
{
  TFile hfile("Test.root","RECREATE","Test for SDHCAL raw data file");
  DIFdataExample source;
  ROOTtreeDest destination;
  SDHCAL_buffer_loop<DIFdataExample,ROOTtreeDest> toto(source,destination,true,std::cout,true);
  toto.loop(2);
  hfile.Write();
  hfile.Close();
  return 0;
}
