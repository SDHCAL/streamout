#include "ROOTtreeDest.h"

#include "TTree.h"

void myFuncDoingNothing()
{
  TTree t("A","B");
  t.Fill();
}
