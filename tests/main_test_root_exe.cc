#include "SDHCAL_buffer.h"
#include "ROOTtreeDest.h"
#include "DIFdataExample.h"

#include "TFile.h"

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
