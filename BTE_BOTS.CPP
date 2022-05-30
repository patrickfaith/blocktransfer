// general bots routines - copyright (c) p l faith

#include <string.h>
#include <btdefs.h>
#include <btlist.h>
#include <btblocks.h>
#include <btgraph.h>
#include <btacmdef.h>
#include <btacmstr.h>
#include <btproto.h>
BTblock *BotsCommandText(char *buffer,BTaccum *accum)
{
  BTblock     *childPtr;
  char *lineStrng;
  BTblock *searchPtr = NULL;
  // BTcommandModes  *cmdModes;
  char s1[20] = " ";
  sscanf(buffer,"%s",s1);
  size_t nameLength = strlen(s1);
  

  if ( strncmp(s1,"this",4) == 0 )
  {
    searchPtr = accum->registers[ACCUM_PC];
    if (searchPtr != NULL) searchPtr = searchPtr->pagePtr;  
  }
  else if (strncmp(s1,"page ",5) == 0 )
  { 
    searchPtr = accum->registers[ACCUM_PAGE];   
  }  
  else
  { 
    
    BTlistIterator pageList( accum->theBook->blockChildren );
    while ( (childPtr = pageList.Iterate()) != NULL)
      if (strncmp(childPtr->Name(),s1,nameLength)== 0)
      {
        searchPtr = childPtr;
        break;
      }   
  }

  if (searchPtr == NULL) return NULL;
  // need to do a page search  if not just looking for the page name
  if ( strlen(buffer) <= strlen(s1) )
    return searchPtr;
    
  buffer= buffer + nameLength + 1;
  nameLength = strlen(buffer);
  BTlistIterator childHead( searchPtr->blockChildren );
  while ( (childPtr = childHead.Iterate()) != NULL)
  { 
    // cmdModes = childPtr->CommandModes(NULL);
    // if ( cmdModes != NULL ) if ( cmdModes->opCode == OPER_NOOP )
    lineStrng = childPtr->lineToString();
    if ( strncmp(lineStrng,buffer,nameLength) == NULL ) 
      if ( strlen(lineStrng) == nameLength )
      {
        delete lineStrng;
	    break ;
	  }
    delete lineStrng;
  }
  return childPtr;
}


int BTaccum::Translate(BTblock *block,BTcommandModes  *cmdModes)
{ 
  BTblock *opPtr;
  int status;
  
  if (cmdModes == NULL ) 
    cmdModes = block->CommandModes(NULL);
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
    case MODE_FIXED:
      break ;
	case MODE_STRING:
	  break;
	default:
	  blockPtr = AnyBlockHighList(block->blockChildren) ;
	  break;    
  }
  if ( blockPtr == NULL )
    return STATUS_BAD ;
  opPtr = blockPtr;  
  do
  {
    status = CommandSwitch(opPtr);
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

