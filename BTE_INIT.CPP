// copyright (c) p l faith
#include <string.h>
#include <btdefs.h>
#include <btlist.h>
#include <btblocks.h>
#include <btgraph.h>
#include <btacmdef.h>
#include <btacmstr.h>
#include "btproto.h"
BTaccum *BTblock::AccumPtr()
{ 
  BTbookBlock *bkPtr;
  if ( pagePtr != NULL )
  {
    if (pagePtr->accum != NULL)
      return pagePtr->accum;
  }
  if ((bkPtr = BookPtr()) != NULL )
    return bkPtr->accum;
  else
    return NULL;
};

int ProcessDNACommandBuffer(BTblock *block, char *buffer)
{
  BTaccum *accumPtr = block->AccumPtr();
  if ( accumPtr == NULL ) return 0;
  
  BTblock *newBlock;
      
  newBlock = accumPtr->ParseCommand(buffer) ;
  if (newBlock != NULL )
  { 
        accumPtr->CommandSwitch(newBlock);
        delete newBlock;
  }
  return 1;  
}

void BTpageBlock::initializeDna(BTtrans *trans)
{
  BTaccum *accumPtr = AccumPtr();
  if ( accumPtr == NULL )
  {
    accum = new BTaccum(bookPtr,this);
    accumPtr = accum;
  }

  char          file[30];
  strcpy(file,name);
  char *txt=".set";
  strcat(file,txt);
  // setup trans ptr for low level calls
  BTtransFile  transIn(file);
  transIn.openInPort();

  char returnChar[3] = "\n";
  char *returnPtr;
  char *buffer;
  
  accumPtr->registers[ACCUM_PAGE] = NULL;
  while ( (buffer = transIn.inPort(this) ) != NULL )
  { 
    if ( strlen(buffer) < 1 ) break ;
    if ( ( returnPtr = strstr(buffer,returnChar) ) != NULL )
      *returnPtr = 0;
    if ( strncmpi( buffer,"//",2) != 0 )
    { 
      BTblock *newBlock;
      
      newBlock = accumPtr->ParseCommand(buffer) ;
      if (newBlock != NULL )
      { 
        accumPtr->CommandSwitch(newBlock);
        delete newBlock;
      }  
    }
  }
  transIn.closePort();
}
void BTbookBlock::initializeDna(BTtrans *trans)
{ 
  BTaccum *accumPtr = AccumPtr();
  if ( accumPtr == NULL ) return ;

  char          file[30];
  strcpy(file,name);
  char *txt=".set";
  strcat(file,txt);
  // setup trans ptr for low level calls
  BTtransFile  transIn(file);
  transIn.openInPort();

  char returnChar[3] = "\n";
  char *returnPtr;
  char *buffer;
  
  accumPtr->registers[ACCUM_PAGE] = NULL;
  while ( (buffer = transIn.inPort(this) ) != NULL )
  { 
    if ( strlen(buffer) < 1 ) break ;
    if ( ( returnPtr = strstr(buffer,returnChar) ) != NULL )
      *returnPtr = 0;
    if ( strncmpi( buffer,"//",2) != 0 )
    { 
      BTblock *newBlock;
      
      newBlock = accumPtr->ParseCommand(buffer) ;
      if (newBlock != NULL )
      { 
        accumPtr->CommandSwitch(newBlock);
        delete newBlock;
      }  
    }
  }
  transIn.closePort();
}
