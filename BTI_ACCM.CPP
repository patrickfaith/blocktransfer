#include <string.h>
#include <btdefs.h>
#include <btlist.h>
#include <btblocks.h>
#include <btgraph.h>
#include <btacmdef.h>
#include <btacmstr.h>
#include "btproto.h"

BTaccum::BTaccum(BTbookBlock *bookPtr,BTblock *blockPtr)
{ 
  int i ;
  for ( i=0;i<ACCUM_REGISTER_TOT;i++)
    registers[i] = NULL;
  for ( i=0;i<MAX_MATH_REGISTERS;i++)
    mathRegs[i] = 0;
  nameBuff = NULL ;
  theBook = bookPtr;
  theBlock = blockPtr;
}
BTcommandModes::BTcommandModes()
{
  opCode = 0 ;
  blockMode = 0 ;
  blockRegister = 0 ;
  transferMode = 0 ;
  transferRegister = 0 ;
  resultMode = 0 ;
  resultRegister = 0 ;
}

BTblock *BTaccum::MakeDnaBlock(BTcommandModes *modes,char *dnaString)
{
  BTdnaBlock *dnaBlock ;
  BTblock *block;
  block = registers[ACCUM_PAGE] ;
  dnaBlock = new BTdnaBlock(block);
  dnaBlock->cmdModes = modes;
  dnaBlock->string = dnaString;
  return dnaBlock; 
}