// copyright (c) p l faith

#include <string.h>
#include <btdefs.h>
#include <btlist.h>
#include <btblocks.h>
#include <btgraph.h>
#include <btproto.h>


void BTpageBlock::receiveChannel(BTtrans *trans)
{
  if (channelType == 0 ) return ;
  char          file[30];
  strcpy(file,name);
  char *txt=".txt";
  strcat(file,txt);
  // setup trans ptr for low level calls
  BTtransFile  transIn(file);
  transIn.openInPort();

  char returnChar[3] = "\n";
  char *returnPtr;
  char *buffer;
  theBook->accum->registers[ACCUM_PAGE] = this;
  while ( (buffer = transIn.inPort(this) ) != NULL )
  { 
    if ( strlen(buffer) < 1 ) break ;
    if ( ( returnPtr = strstr(buffer,returnChar) ) != NULL )
      *returnPtr = 0;

    if ( strncmpi( buffer,"//",2) != 0 )
    {
      theBook->accum->ParseCommand(buffer) ;
    }
  }

  transIn.closePort();
}
