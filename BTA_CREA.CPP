#include <string.h>
#include <btdefs.h>
#include <btlist.h>
#include <btblocks.h>
#include <btgraph.h>
#include <btacmdef.h>
#include <btacmstr.h>
#include "btproto.h"

int BTaccum::DeleteBlock(BTcommandModes  *cmdModes )
{

  BTblock *blockPtr = NULL;
  BTblock *cBlockPtr;
  
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
  
  if (blockPtr != NULL )
    delete blockPtr;
    
  return 1;
  
}
void BTaccum::CreateBlock(BTcommandModes  *cmdModes,
                          BTblock         *cmdBlock,
                          int		      blockType)
{ 
  char *nameBuffer =  nameBuff ;
  if ( cmdBlock->Name() != NULL )
    nameBuffer = cmdBlock->Name() ;
  
  BTblock *newBlock=NULL;
  
  // if (registers[ACCUM_PAGE]== NULL ) return ; // ?error?
  
  switch (blockType)
  {
    case BT_TYPE_BASIC:		// @create block (object_type) [x]
    newBlock = new BTblock(registers[ACCUM_PAGE]);
    break;
    case BT_TYPE_GROUP:
    newBlock = new BTgroupBlock(registers[ACCUM_PAGE]);
    break;
    case BT_TYPE_CHUNK:
      newBlock = new BTchunkBlock(registers[ACCUM_PAGE]);
      break;
    case BT_TYPE_STRING:
      newBlock = new BTstringBlock(nameBuff,registers[ACCUM_PAGE]);
      break;
    case BT_TYPE_READ:
      newBlock = new BTreadBlock();
      break;
    case BT_TYPE_PAGE:
      newBlock = new BTpageBlock(theBook,nameBuffer);
      break;
    case BT_TYPE_CFACE:
      newBlock = new BTcFaceBlock(registers[ACCUM_PAGE]);
      break;
    case BT_TYPE_VIEW:
      newBlock = new BTviewBlock(theBook,nameBuffer);
      break;
    case BT_TYPE_MESSAGE:
      newBlock = new BTmessageBlock();
      break;
  }
  if ( newBlock != NULL ) 
  {
    if ( cmdModes->blockRegister != EMPTY ) 
      registers[cmdModes->blockRegister]= newBlock;  
    if ( cmdModes->transferRegister != EMPTY )
      registers[cmdModes->transferRegister]->addChild(newBlock);
  }
  return ; 
}    
