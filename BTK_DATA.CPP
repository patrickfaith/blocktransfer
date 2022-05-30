#include <string.h>
#include <btdefs.h>
#include <btlist.h>
#include <btblocks.h>
#include <btgraph.h>
#include <btacmdef.h>
#include <btacmstr.h>
#include "btproto.h"

BTblock *KernalRegisterGet(BTaccum *accum, int reg )
{
  BTbookBlock *bookPtr = accum->theBook;
  if (bookPtr == NULL ) return NULL;
  
  return bookPtr->sysRegs[reg] ;

}

void KernalRegisterSet(BTaccum *accum, int reg, BTblock *block)
{
  BTbookBlock *bookPtr = accum->theBook;
  if (bookPtr == NULL ) return;
  
  bookPtr->sysRegs[reg] = block;

}