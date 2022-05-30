#include <string.h>
#include <btdefs.h>
#include <btlist.h>
#include <btblocks.h>
#include <btgraph.h>
#include <btacmdef.h>
#include <btacmstr.h>
#include "btproto.h"

char *DuplicateName(char *namePtr,char *newNamePtr)
{
  if ( newNamePtr != NULL ) delete newNamePtr;
  if (namePtr == NULL )
  {
    newNamePtr = NULL;
  }
  else
  {
    newNamePtr = new char [strlen(namePtr) + 1] ;
    strcpy(newNamePtr,namePtr);
  }
  return newNamePtr;
}

void BTaccum::LoadString(BTblock *blockPtr, BTcommandModes  *cmdModes )
{ 
  char *readBuffer;
  int deleteBuffer = 0;

  readBuffer = blockPtr->lineToString();
  if ( readBuffer != NULL )
    nameBuff = DuplicateName(readBuffer,nameBuff);    

  delete readBuffer; 
  return ;
}

// XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
// THIS ROUTINE IS NOT CURRENTLY USED - REPLACED BY  BotsCommandText
// xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
BTblock *BTaccum::parseBlockLoad(char *buffer)
{
  char pageName[80];
  short blkId;
  BTblock *pagePtr ;
  BTblock *blockPtr;
 
  if (sscanf(buffer,"%s %D ",pageName,&blkId) == 1 )   //  "page_name [blockId]"
    blkId = 0;
    
  // scan book for page, then search through it
  BTlistIterator pageList(theBook->blockChildren );
  while ( (pagePtr = pageList.Iterate()) != NULL )
  {
    if ( strcmpi(pageName,pagePtr->Name()) == 0 )
    {
      if ( blkId == 0 )
      {
        return pagePtr;
      }
      else
      {
        BTlistIterator blockList(pagePtr->blockChildren );
	    while ( (blockPtr = blockList.Iterate()) != NULL )
	      if ( blockPtr->blockId == blkId )
	        return blockPtr;	      
      }
      break ;  
    }
  }
  return NULL ;
}

void BTaccum::LoadBlock(BTblock *blockPtr, BTcommandModes  *cmdModes )
{
  int deleteBuffer = 0;
  char *readBuffer = NULL;  

  
  if ( cmdModes->blockMode == MODE_COMPLEX )
  { 
    BTblock *iBlockPtr;
    if ( ( iBlockPtr = blockPtr->Internal() )  == NULL )
    {
      readBuffer = blockPtr->Name();
      if ( readBuffer != NULL )
        iBlockPtr = BotsCommandText(readBuffer,this); 
    }
    if ( cmdModes->transferMode == MODE_REGISTER )
      registers[cmdModes->transferRegister] = iBlockPtr  ;
    else if ( cmdModes->transferMode == MODE_KERNAL )
      KernalRegisterSet(this, cmdModes->transferRegister,iBlockPtr );
    return ;
  }

  if (blockPtr->Type() == BT_TYPE_CHUNK )
  {
    readBuffer = blockPtr->lineToString();
    deleteBuffer = 1;
  }
  else
  {
    readBuffer = blockPtr->Name();
  }
   
  // convert buffer into block ptr
  if ( readBuffer != NULL )
    registers[cmdModes->blockRegister] = BotsCommandText(readBuffer,this); 

  if (deleteBuffer == 1)
    delete readBuffer; 
  return ;
}

void BTaccum::LoadPage(BTblock *block, BTcommandModes  *cmdModes )
{

  BTblock *blockPtr = NULL;
      
  switch (cmdModes->blockMode)
  {
    case MODE_REGISTER:
      blockPtr = registers[cmdModes->blockRegister];
      break ;
    case MODE_INDIRECT:
      blockPtr = iter[cmdModes->blockRegister].CurrentPtr();
      break ;

  }
  if ( blockPtr == NULL ) return ;
  switch (cmdModes->transferMode)
  {
    case MODE_REGISTER:
      registers[cmdModes->transferRegister] = blockPtr->pagePtr;
      break ;  
  }
  return ;
}

void BTaccum::LoadMath(BTblock *blockPtr, BTcommandModes  *cmdModes )
{
  int deleteBuffer = 0;
  char *readBuffer = NULL;  

  if (blockPtr->Type() == BT_TYPE_CHUNK )
  {
    readBuffer = blockPtr->lineToString();
    deleteBuffer = 1;
  }
  else
  {
    readBuffer = blockPtr->Name();
  }
  // convert buffer into some integer value
  BTval data = 0;
  if ( readBuffer != NULL )
    sscanf(readBuffer,"%D",&data) ;
  mathRegs[cmdModes->blockRegister] = data ;
  if (deleteBuffer == 1)
    delete readBuffer; 
  return ;
}

void BTaccum::LoadNull(BTcommandModes  *cmdModes)
{
  BTblock *blockPtr = NULL;
  
  switch (cmdModes->blockMode)
  {
    case MODE_REGISTER:
      registers[cmdModes->blockRegister] = NULL;
      break ;
    case MODE_INDIRECT:
      break ;
    case MODE_COMPLEX:
      if ( ( blockPtr = registers[cmdModes->blockRegister] ) != NULL ) 
        blockPtr->Internal(NULL);
      break ;
    case MODE_FIXED:
      mathRegs[cmdModes->blockRegister] =  0;
      break ;
	case MODE_STRING:
      if ( nameBuff != NULL )
        delete nameBuff;
	  nameBuff = NULL ;
	  break;    
  }
}