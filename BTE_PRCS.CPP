// general bots routines - copyright (c) p l faith

#include <string.h>
#include <btdefs.h>
#include <btlist.h>
#include <btblocks.h>
#include <btgraph.h>
#include <btacmdef.h>
#include <btacmstr.h>
#include <btproto.h>

int BTaccum::Process(BTblock *block,BTcommandModes  *cmdModes)
{ 
  int status;
  if (cmdModes == NULL ) 
    cmdModes = block->CommandModes(NULL);
  BTblock *blockPtr = NULL;
  BTblock *tranPtr = NULL;
  switch (cmdModes->blockMode)
  { 
    case MODE_REGISTER:
      blockPtr = registers[cmdModes->blockRegister];
      break ;
    case MODE_INDIRECT:
      blockPtr = iter[cmdModes->blockRegister].CurrentPtr();
      break ;
    case MODE_COMPLEX:
      BTblock *cBlockPtr = registers[cmdModes->blockRegister]; 
      if ( cBlockPtr == NULL ) break;
      if ( ( blockPtr = cBlockPtr->Internal() )  == NULL )
	  {
	    if ( cBlockPtr->Name() != NULL )
	      blockPtr = BotsCommandText( cBlockPtr->Name(), this );         
        else
          blockPtr = AnyBlockHighList(cBlockPtr->blockChildren);
      }
      break ;
       
  }
  
  if ( blockPtr == NULL ) return STATUS_BAD ;
  BTaccum *accum = blockPtr->AccumPtr();
  if (accum == NULL) return STATUS_BAD;


  switch (cmdModes->transferMode)
  { 
    default:
      tranPtr = blockPtr;
      break; 
    case MODE_REGISTER:
      tranPtr = accum->registers[cmdModes->transferRegister];
      break ;
    case MODE_INDIRECT:
      tranPtr = accum->iter[cmdModes->transferRegister].CurrentPtr();
      break ;
    case MODE_COMPLEX:
      BTblock *cBlockPtr = accum->registers[cmdModes->transferRegister]; 
      if ( cBlockPtr == NULL ) break;
      if ( ( tranPtr = cBlockPtr->Internal() )  == NULL )
	  {
	    if ( cBlockPtr->Name() != NULL )
	      tranPtr = BotsCommandText( cBlockPtr->Name(),accum );         
        else
          tranPtr = AnyBlockHighList(cBlockPtr->blockChildren);
        cBlockPtr->Internal(blockPtr);
      }
      break ;
  }
  if ( tranPtr == NULL ) return STATUS_BAD;

  BTblock *opPtr = tranPtr;  
  do
  {
    status = accum->CommandSwitch(opPtr);
    if ( status != STATUS_OK )
    {
      if ( status == STATUS_CYCLE )
        opPtr = blockPtr;
      else if ( status == STATUS_BREAK )
        return STATUS_OK;
      else
        return STATUS_BAD;
    }
    else
    {
      opPtr = AnyBlockHighList(opPtr->transferOutput);
    }
  } while ( opPtr != NULL );
  return STATUS_OK ;
}

int BTaccum::ProcessMessage(BTblock *block,BTcommandModes  *cmdModes)
{ 
  BTblock *cBlockPtr;
  if (cmdModes == NULL ) 
    cmdModes = block->CommandModes(NULL);
  BTblock *blockPtr = NULL; 
  
  BTblock *tranPtr = NULL;

  switch (cmdModes->blockMode)
  { 
    case MODE_REGISTER:
      blockPtr = registers[cmdModes->blockRegister];
      break ;
    case MODE_INDIRECT:
      blockPtr = iter[cmdModes->blockRegister].CurrentPtr();
      break ;
    case MODE_COMPLEX:
      cBlockPtr = registers[cmdModes->blockRegister]; 
      if ( cBlockPtr == NULL ) break;
      if ( ( blockPtr = cBlockPtr->Internal() )  == NULL )
	  {
	    if ( cBlockPtr->Name() != NULL )
	      blockPtr = BotsCommandText( cBlockPtr->Name(),this );         
        else
          blockPtr = AnyBlockHighList(cBlockPtr->blockChildren);
        cBlockPtr->Internal(blockPtr);
      }
      break ; 
  }

  if ( blockPtr == NULL ) return STATUS_BAD ;
  BTaccum *accum = blockPtr->AccumPtr();
  if (accum == NULL) return STATUS_BAD;

  switch (cmdModes->transferMode)
  { 
    case MODE_REGISTER:
      tranPtr = registers[cmdModes->transferRegister];
      if (cmdModes->resultMode == MODE_REGISTER )
        accum->registers[cmdModes->resultRegister] = tranPtr;
      else
        accum->registers[cmdModes->transferRegister] = tranPtr;
      break ;
    case MODE_FIXED:
      {
      BTfix val = mathRegs[cmdModes->transferRegister];
      if (cmdModes->resultMode == MODE_REGISTER )
        accum->mathRegs[cmdModes->resultRegister] = val;
      else
        accum->mathRegs[cmdModes->transferRegister] = val;
      }
      break ;
    case MODE_STRING:
	  accum->nameBuff = DuplicateName2(nameBuff,accum->nameBuff);	  
	  break; 
  }

  return STATUS_OK ;
}

int BTaccum::createAccum(BTcommandModes  *cmdModes)
{ 

  BTblock *blockPtr = NULL;
  BTblock *cBlockPtr = NULL;
  switch (cmdModes->blockMode)
  { 
    case MODE_REGISTER:
      blockPtr = registers[cmdModes->blockRegister];
      break ;
    case MODE_INDIRECT:
      blockPtr = iter[cmdModes->blockRegister].CurrentPtr();
      break ;
    case MODE_COMPLEX:  
      BTblock *cBlockPtr = registers[cmdModes->transferRegister]; 
      if ( cBlockPtr == NULL ) break;

	  if ( cBlockPtr->Name() != NULL )
	    blockPtr = BotsCommandText( cBlockPtr->Name(),this );         
      else
        blockPtr = AnyBlockHighList(cBlockPtr->blockChildren);
      break ;     
  }

  if ( blockPtr == NULL ) return STATUS_BAD ;
  if ( blockPtr->Type() == BT_TYPE_PAGE )
  { 
    BTpageBlock *pagePtr = (BTpageBlock *) blockPtr;
    if (pagePtr->accum != NULL )
      delete pagePtr->accum;
    pagePtr->accum = new BTaccum(theBook,pagePtr);
  }   
  return STATUS_OK ;
}


