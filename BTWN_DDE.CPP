
#include <string.h>
#include <btdefs.h>
#include <btlist.h>
#include <btwndef.h>
#include <btblocks.h>
#include <btgraph.h>
#include <btacmdef.h>
#include <btacmstr.h>
#include <btproto.h>

int ProcessDDEexecute(HWND hWnd, HWND client, BTblock *block, char *buffer)
{
  BTblock *accumBlock;
  BTaccum *accumPtr;
  char *buf;
  
//  if ( BTex.theBook->sysRegs[REG_SYS_DDEACCUM]  != NULL )
//    accumBlock = BTex.theBook->sysRegs[REG_SYS_DDEACCUM] ;
//  else
//    accumBlock = block;
//  if (accumBlock == NULL )
//    return 0;

  if ( ( accumPtr= block->AccumPtr()) == NULL )
    return 0;
 
  if (buffer[0] == '[' )
  {
    buf = buffer + 6 ; // "[exec]" assuming for now
    ProcessDNACommandBuffer(block,buf ) ; // was  - accumBlock, buffer);
    
    if ( (accumBlock = BTex.theBook->sysRegs[REG_SYS_DDEACCUM])  != NULL )
      accumPtr->CommandSwitch( accumBlock );
    
    if ( accumPtr->nameBuff != NULL)
      strcpy(buffer,accumPtr->nameBuff) ;
    else
      *buffer=0; 
  }
  //else
  //{
  //  accumPtr->nameBuff =
  //    DuplicateName2(buffer,accumPtr->nameBuff);
  //  accumPtr->CommandSwitch( accumBlock ); // was outside the else
  //}
  


  return 1; 
}