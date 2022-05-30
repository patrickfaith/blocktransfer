// copyright (c) p l faith
#include <string.h>
#include <btdefs.h>
#include <btlist.h>
#include <btblocks.h>
#include <btgraph.h>
#include <btacmdef.h>
#include <btacmstr.h>
#include "btproto.h"

void BTpageBlock::receiveChannel(BTtrans *trans)
{
  if (channelType == 0 ) return ;
  char          file[30];
  strcpy(file,name);
  char *txt=".dna";
  strcat(file,txt);
  // setup trans ptr for low level calls
  BTtransFile  transIn(file);
  transIn.openInPort();

  char returnChar[3] = "\n";
  char *returnPtr;
  char *buffer;
  BookPtr()->accum->registers[ACCUM_PAGE] = this;
  
  BTblock *newBlock = NULL;
  BTblock *oldBlock = NULL;

  while ( (buffer = transIn.inPort(this) ) != NULL )
  {
    if ( strlen(buffer) < 1 ) {delete buffer; break ; } ;
    if ( ( returnPtr = strstr(buffer,returnChar) ) != NULL )
      *returnPtr = 0;
    if ( strncmpi( buffer,"//",2) != 0 )
    { 
      oldBlock = newBlock;
      newBlock = BookPtr()->accum->ParseCommand(buffer) ;
      if ( newBlock == NULL )
      { 
        BTblock *wordPagePtr =  bookPtr->sysRegs[REG_SYS_DICTIONARY];
        newBlock = new BTchunkBlock(this);

        StringToBlocks(buffer, newBlock, wordPagePtr) ;
      }
  
      if (oldBlock != NULL )
        oldBlock->addOutput(newBlock);     
    }
    delete buffer;
  }
  transIn.closePort();
}
