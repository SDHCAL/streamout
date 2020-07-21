#include "SDHCAL_buffer_loop.h"
#include "textDump.h"
#include "DIFdataExample.h"
#include <iostream>

int main()
{
  DIFdataExample source;
  textDump destination;
  //SDHCAL_RawBuffer_Navigator::StartAt(92);
  SDHCAL_buffer_loop<DIFdataExample,textDump> toto(source,destination,true,std::cout,true);
  toto.loop(2);
  std::cout<<"******************************"<<std::endl;
  toto.printAllCounters();
  std::cout<<"******************************"<<std::endl;
  return 0;
}
