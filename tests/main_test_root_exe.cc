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
  SDHCAL_buffer_loop(source,destination);
  hfile.Write();
  hfile.Close();
  return 0;
}
