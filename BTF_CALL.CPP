#include <string.h>
#include <btdefs.h>
#include <btlist.h>
#include <btblocks.h>
#include <btgraph.h>
#include <btacmdef.h>
#include <btacmstr.h>
#include "btproto.h"

int BTaccum::FileLoad(BTcommandModes  *cmdModes )
{

  BTblock *blockPtr = NULL;
  BTblock *cBlockPtr;
  BTblock *transPtr = NULL;  
  switch (cmdModes->blockMode)
  { 
    case MODE_REGISTER:
      blockPtr = registers[cmdModes->blockRegister];
      break ;
    case MODE_INDIRECT:
      blockPtr = iter[cmdModes->blockRegister].CurrentPtr();
      break ;
    case MODE_COMPLEX:  // never references the internal
      cBlockPtr = registers[cmdModes->blockRegister]; 
      if ( cBlockPtr == NULL ) break;

	  if ( cBlockPtr->Name() != NULL )
	    blockPtr = BotsCommandText( cBlockPtr->Name(),this );         
      else
        blockPtr = AnyBlockHighList(cBlockPtr->blockChildren);
      break ;
  }
  
  if (blockPtr != NULL );
  
  switch (cmdModes->transferMode)
  {
    case MODE_REGISTER:
      transPtr = registers[cmdModes->transferRegister];
      break ;
    case MODE_INDIRECT:
      transPtr = iter[cmdModes->transferRegister].CurrentPtr();
      break ;
    case MODE_COMPLEX:
      break ;  
  }
  blockPtr->receiveBlockAscii(transPtr);
  return 1;
}
int BTaccum::FileSave(BTcommandModes  *cmdModes )
{

  BTblock *blockPtr = NULL;
  BTblock *cBlockPtr;
  BTblock *transPtr = NULL;  
  switch (cmdModes->blockMode)
  { 
    case MODE_REGISTER:
      blockPtr = registers[cmdModes->blockRegister];
      break ;
    case MODE_INDIRECT:
      blockPtr = iter[cmdModes->blockRegister].CurrentPtr();
      break ;
    case MODE_COMPLEX:  // never references the internal
      cBlockPtr = registers[cmdModes->blockRegister]; 
      if ( cBlockPtr == NULL ) break;

	  if ( cBlockPtr->Name() != NULL )
	    blockPtr = BotsCommandText( cBlockPtr->Name(),this );         
      else
        blockPtr = AnyBlockHighList(cBlockPtr->blockChildren);
      break ;
  }
  
  if (blockPtr != NULL );
  
  switch (cmdModes->transferMode)
  {
    case MODE_REGISTER:
      transPtr = registers[cmdModes->transferRegister];
      break ;
    case MODE_INDIRECT:
      transPtr = iter[cmdModes->transferRegister].CurrentPtr();
      break ;
    case MODE_COMPLEX:
      break ;  
  }
  blockPtr->sendBlockAscii(transPtr);
  return 1;
}