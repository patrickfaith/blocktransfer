
#include <string.h>
#include <btdefs.h>
#include <btlist.h>
#include <btblocks.h>
#include <btgraph.h>
#include <btacmdef.h>
#include <btacmstr.h>
#include "btproto.h"

BTblock *BTaccum::CalculateAddress(char reg, char mode)
{ 
  BTblock *blockPtr = NULL;
  switch (mode)
  {
    case MODE_REGISTER:
      blockPtr = registers[reg];
    break ;
    case MODE_INDIRECT:
      blockPtr = iter[reg].CurrentPtr();;
    break ;
    case MODE_COMPLEX:
    break ;
  }
  return blockPtr;
}