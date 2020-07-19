#include "SDHCAL_buffer_loop.h"
#include "textDump.h"
#include "DIFdataExample.h"

int main()
{
  DIFdataExample source;
  textDump destination;
  SDHCAL_buffer_loop(source,destination,false,std::cout,true);
  return 0;
}
