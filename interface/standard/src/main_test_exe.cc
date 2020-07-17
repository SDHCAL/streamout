#include "SDHCAL_buffer.h"
#include "textDump.h"
#include "DIFdataExample.h"

int main()
{
  DIFdataExample source;
  textDump destination;
  SDHCAL_buffer_loop(source,destination);
  return 0;
}
