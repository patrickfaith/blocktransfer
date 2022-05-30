#include <string.h>
#include <btdefs.h>
#include <btlist.h>
#include <btblocks.h>
#include <btgraph.h>
#include <btacmdef.h>
#include <btacmstr.h>
#include "btproto.h"

char *DuplicateName2(char *namePtr,char *newNamePtr)
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

char *wordEnd(char *strng)
{
  while (( *strng != '\0' ) && (*strng != ' ')) strng++;
  return strng;
}

char *wordStart(char *strng)
{
  while (*strng == ' ') strng++;
  return strng;
}


int StringToBlocks(char *strng, BTblock *newGroup, BTblock *wordPagePtr)
{

  char          *tStr;
  char          *tempStr;
  BTblock       *foundStringBlock;
  char			*word;
  size_t        length;
  char			*endPtr;
  
  if ( wordPagePtr == NULL ) return 0 ;
  // make a sentence
  if (strng==NULL) return 0;
  if (newGroup==NULL) return 0;

  // strip the string into words
  tStr = new char[strlen(strng)+2];
  strcpy(tStr,strng);
  tempStr = tStr;
  while ( *tempStr != '\0')
  {
    tempStr = wordStart(tempStr); 		// find word
    endPtr = wordEnd(tempStr);         // find end of word
    length = endPtr - tempStr ;
    word = new char[length+1] ;      // make new word
    _fstrncpy(word,tempStr,length);
    word[length]='\0';

    tempStr = tempStr + length  ;  // go to next word

    // put words into a string block and associate to sentence

    if ( ( foundStringBlock = wordPagePtr->SearchForWord(word) ) != NULL )
    {
      newGroup->addChild(foundStringBlock);
      delete word;
    }
    else
    {
      newGroup->addChild(new BTstringBlock(word,wordPagePtr));
    }
    
  }
  
  delete tStr;
  return 1;
}


void BTaccum::MoveString( BTcommandModes  *cmdModes )
{ 
  BTblock *blockPtr = NULL;
  char *namePtr = NULL;
  char *newNamePtr = NULL;
  int deleteBuffer = 0;
  char *readBuffer = NULL;  
  switch (cmdModes->blockMode)
  {
    case MODE_REGISTER:
      blockPtr = registers[cmdModes->blockRegister];
      if (blockPtr->Type() == BT_TYPE_CHUNK )
      {
        readBuffer = blockPtr->lineToString();
        deleteBuffer = 1;
      }
      else
        readBuffer = blockPtr->Name();
      break ;
    case MODE_INDIRECT:
      blockPtr = iter[cmdModes->blockRegister].CurrentPtr();
      if (blockPtr->Type() == BT_TYPE_CHUNK )
      {
        readBuffer = blockPtr->lineToString();
        deleteBuffer = 1;
      }
      else
        readBuffer = blockPtr->Name();
      break ;
    case MODE_COMPLEX:
      break ;
    case MODE_FIXED:
      // create a string that is associated with the fixed point math
      readBuffer = new char[10];
      sprintf(readBuffer,"%d", mathRegs[cmdModes->blockRegister]);
      break ;
	case MODE_STRING:
	  readBuffer = nameBuff ;
	  break;    
  }
  if ( readBuffer  == NULL ) return ;
  switch (cmdModes->transferMode)
  {
    case MODE_REGISTER:
      blockPtr = registers[cmdModes->transferRegister];
      if (blockPtr->Type() == BT_TYPE_CHUNK )
      { 
        BTblock *wordPagePtr =  theBook->sysRegs[REG_SYS_DICTIONARY];
        if (cmdModes->resultMode != 0 )
        {
          wordPagePtr = registers[cmdModes->resultRegister] ;
        }   
        StringToBlocks(readBuffer, blockPtr,wordPagePtr) ;
      }
      else
      {
        newNamePtr = blockPtr->Name();
        newNamePtr = DuplicateName2(readBuffer,newNamePtr);
        blockPtr->Name(newNamePtr);
      }
      break ;
    case MODE_INDIRECT:
      blockPtr = iter[cmdModes->transferRegister].CurrentPtr();
      newNamePtr = blockPtr->Name();
      newNamePtr = DuplicateName2(readBuffer,newNamePtr);
      blockPtr->Name(newNamePtr);
      break ;
    case MODE_COMPLEX:
      break ;
    case MODE_FIXED:
      sscanf(readBuffer,"%d",&mathRegs[cmdModes->transferRegister]);
      break ;
	case MODE_STRING:
	  nameBuff = DuplicateName2(readBuffer,nameBuff);
	  break;    
  }
  if (deleteBuffer == 1)
    delete readBuffer; 
  return ;
}

void BTaccum::MoveEnergy( BTcommandModes  *cmdModes )
{ 
  BTblock *blockPtr = NULL;
  BTval enrgy = 0 ;
    
  switch (cmdModes->blockMode)
  {
    case MODE_REGISTER:
      blockPtr = registers[cmdModes->blockRegister];
      if (blockPtr != NULL ) 
        enrgy = blockPtr->btEnergy;
      break ;
    case MODE_INDIRECT:
      blockPtr = iter[cmdModes->blockRegister].CurrentPtr();
      if (blockPtr != NULL ) 
        enrgy = blockPtr->btEnergy;
      break ;
    case MODE_COMPLEX:
      break ;
    case MODE_FIXED:
      enrgy = mathRegs[cmdModes->blockRegister];
      break ;
  }

  switch (cmdModes->transferMode)
  {
    case MODE_REGISTER:
      blockPtr = registers[cmdModes->transferRegister];
      if (blockPtr != NULL )
        blockPtr->btEnergy = enrgy;
      break ;
    case MODE_INDIRECT:
      blockPtr = iter[cmdModes->transferRegister].CurrentPtr();
      if (blockPtr != NULL )
        blockPtr->btEnergy = enrgy;
      break ;
    case MODE_COMPLEX:
      break ;
    case MODE_FIXED:
      mathRegs[cmdModes->transferRegister] = enrgy;
      break ;   
  }
  return ;
}
void BTaccum::MoveBlock( BTcommandModes  *cmdModes )
{ 
  BTblock *blockPtr = NULL;
  BTblock *transPtr;    
  switch (cmdModes->blockMode)
  {
    case MODE_REGISTER:
      blockPtr = registers[cmdModes->blockRegister];
      break ;
    case MODE_INDIRECT:
      blockPtr = iter[cmdModes->blockRegister].CurrentPtr();
      break ;
    case MODE_COMPLEX:
      blockPtr = registers[cmdModes->blockRegister];
      if ( blockPtr != NULL )
        blockPtr = blockPtr->Internal();
      break ;
    case MODE_FIXED:
      break ;
    case MODE_KERNAL:
      blockPtr = KernalRegisterGet(this, cmdModes->blockRegister );
      break ;
  }

  switch (cmdModes->transferMode)
  {
    case MODE_REGISTER:
      registers[cmdModes->transferRegister] = blockPtr;
      break ;
    case MODE_INDIRECT:
      break ;
    case MODE_COMPLEX:
      transPtr = registers[cmdModes->transferRegister];
      if ( transPtr != NULL )
        transPtr->Internal(blockPtr);
      break ;
    case MODE_FIXED:
      break ;
    case MODE_KERNAL:
      KernalRegisterSet(this, cmdModes->transferRegister,blockPtr );
      break ;
    
  }
  return ;
}
void BTaccum::MoveBratio( BTcommandModes  *cmdModes )
{ 
  BTblock *blockPtr = NULL;
  BTval enrgy = 0 ;
      
  switch (cmdModes->blockMode)
  {
    case MODE_REGISTER:
      blockPtr = registers[cmdModes->blockRegister];
      if (blockPtr != NULL ) 
        enrgy = blockPtr->blockRatio;
      break ;
    case MODE_INDIRECT:
      blockPtr = iter[cmdModes->blockRegister].CurrentPtr();
      if (blockPtr != NULL ) 
        enrgy = blockPtr->blockRatio;
      break ;
    case MODE_COMPLEX:
      break ;
    case MODE_FIXED:
      enrgy = mathRegs[cmdModes->blockRegister];
      break ;
  }

  switch (cmdModes->transferMode)
  {
    case MODE_REGISTER:
      blockPtr = registers[cmdModes->transferRegister];
      if (blockPtr != NULL )
        blockPtr->blockRatio = enrgy;
      break ;
    case MODE_INDIRECT:
      blockPtr = iter[cmdModes->transferRegister].CurrentPtr();
      if (blockPtr != NULL )
        blockPtr->blockRatio = enrgy;
      break ;
    case MODE_COMPLEX:
      break ;
    case MODE_FIXED:
      mathRegs[cmdModes->transferRegister] = enrgy;
      break ;   
  }
  return ;
}
void BTaccum::MoveTratio( BTcommandModes  *cmdModes )
{ 
  BTblock *blockPtr = NULL;
  BTval enrgy = 0 ;
      
  switch (cmdModes->blockMode)
  {
    case MODE_REGISTER:
      blockPtr = registers[cmdModes->blockRegister];
      if (blockPtr != NULL ) 
        enrgy = blockPtr->transferRatio;
      break ;
    case MODE_INDIRECT:
      blockPtr = iter[cmdModes->blockRegister].CurrentPtr();
      if (blockPtr != NULL ) 
        enrgy = blockPtr->transferRatio;
      break ;
    case MODE_COMPLEX:
      break ;
    case MODE_FIXED:
      enrgy = mathRegs[cmdModes->blockRegister];
      break ;
  }

  switch (cmdModes->transferMode)
  {
    case MODE_REGISTER:
      blockPtr = registers[cmdModes->transferRegister];
      if (blockPtr != NULL )
        blockPtr->transferRatio = enrgy;
      break ;
    case MODE_INDIRECT:
      blockPtr = iter[cmdModes->transferRegister].CurrentPtr();
      if (blockPtr != NULL )
        blockPtr->transferRatio = enrgy;
      break ;
    case MODE_COMPLEX:
      break ;
    case MODE_FIXED:
      mathRegs[cmdModes->transferRegister] = enrgy;
      break ;   
  }
  return ;
}
void BTaccum::MoveWeight( BTcommandModes  *cmdModes )
{ 
  BTblock *blockPtr = NULL;
  BTval enrgy = 0 ;
      
  switch (cmdModes->blockMode)
  {
    case MODE_INDIRECT:
      enrgy = iter[cmdModes->blockRegister].CurrentWeight();
      break ;
    case MODE_FIXED:
      enrgy = mathRegs[cmdModes->blockRegister];
      break ;
  }

  switch (cmdModes->transferMode)
  {
    case MODE_INDIRECT:
      iter[cmdModes->transferRegister].SetWeight(enrgy);
      break ;
    case MODE_FIXED:
      mathRegs[cmdModes->transferRegister] = enrgy;
      break ;   
  }
  return ;
}