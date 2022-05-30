// general bots routines - copyright (c) p l faith

#include <string.h>
#include <btdefs.h>
#include <btlist.h>
#include <btblocks.h>
#include <btgraph.h>
#include <btacmdef.h>
#include <btacmstr.h>
#include <btproto.h>

int BTaccum::breakDna(BTcommandModes  *cmdModes)
{ 
  int status = STATUS_OK ;
  BTblock *blockPtr;
  BTfix fixValue;
  switch (cmdModes->blockMode)
  { 
    case MODE_REGISTER:
      blockPtr = registers[cmdModes->blockRegister];
      goto doit;
    case MODE_INDIRECT:
      blockPtr = iter[cmdModes->blockRegister].CurrentPtr();
      doit:
      if ( cmdModes->opCode == OPER_BREAK_NULL )
      {
        if (blockPtr == NULL ) status = STATUS_BREAK ;
      }
      else
      {
        if (blockPtr != NULL ) status = STATUS_BREAK ;
      }
      break ;
    case MODE_FIXED:
      fixValue = mathRegs[cmdModes->blockRegister];
      if ( cmdModes->opCode == OPER_BREAK_NULL )
      {
        if (fixValue < 1 ) status = STATUS_BREAK ;
      }
      else
      {
        if (fixValue > 1 ) status = STATUS_BREAK ;
      }
      break ;
	case MODE_STRING:
	  if ( cmdModes->opCode == OPER_BREAK_NULL )
      {
        if (nameBuff == NULL ) status = STATUS_BREAK ;
      }
      else
      {
        if (nameBuff != NULL ) status = STATUS_BREAK ;
      }
	  break;    
  }
  return status;
}

int BTaccum::cycleDna(BTcommandModes  *cmdModes)
{
  return STATUS_CYCLE ; 
}