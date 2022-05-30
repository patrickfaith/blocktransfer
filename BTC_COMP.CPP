#include <string.h>
#include <btdefs.h>
#include <btlist.h>
#include <btblocks.h>
#include <btgraph.h>
#include <btacmdef.h>
#include <btacmstr.h>
#include "btproto.h"


void BTaccum::CompareName( BTcommandModes  *cmdModes )
{
  int buff1Len,buff2Len;

  BTblock *blockPtr;
  
  char *buffer1 = NULL;
  char *buffer2 = NULL;  
  char *buff1 = NULL;

  
  switch (cmdModes->blockMode)
  {
    case MODE_REGISTER:
      blockPtr = registers[cmdModes->blockRegister]; 
      buffer1 = blockPtr->Name();     
      break ;
    case MODE_INDIRECT:
      blockPtr = iter[cmdModes->blockRegister].CurrentPtr();
      buffer1 = blockPtr->Name();
      break ;
	case MODE_STRING:
	  buffer1 = nameBuff ;
	  break;    
  }
  
  if (buffer1 == NULL )
  {
   mathRegs[cmdModes->resultRegister] = -1 ;
   return;
  }
  buff1Len = strlen(buffer1);  
  buff1 = new char[buff1Len+1];
  strcpy(buff1,buffer1);   

  switch (cmdModes->transferMode)
  {
    case MODE_REGISTER:
      blockPtr = registers[cmdModes->transferRegister];
      buffer2 = blockPtr->Name();
      break ;
    case MODE_INDIRECT:
      blockPtr = iter[cmdModes->transferRegister].CurrentPtr();
      buffer2 = blockPtr->Name();
      break ;
	case MODE_STRING:
	  buffer2 =  nameBuff ;
	  break;    
  }

  if (buffer2 == NULL )
  {
    delete [] buff1;
   mathRegs[cmdModes->resultRegister] = -1 ;
   return;
  }
  buff2Len = strlen(buffer2);

  char seps[]   = " ,\t\n";
  char *token;
  
  int numberOfWords = 0;
  int numberOfMatches = 0;
  
  token = strtok( buff1, seps );
  while( token != NULL )
  {
    if ( strstr(token,buffer2) != 0 )
      numberOfMatches+=1;      
    /* Get next token: */
    token = strtok( NULL, seps );
    numberOfWords += 1;    
  }
   
  BTfix val = -1 ; 
  
  if ( numberOfWords == numberOfMatches )
    val = FIX_ONE;
  else if (numberOfWords != 0 )
    val += FIX_ONE / numberOfWords * numberOfMatches ;
    
  if  (buff1Len != buff2Len)
    val /= 2;
  
  mathRegs[cmdModes->resultRegister] = val ;
  
  delete [] buff1;
  
  return ;    
}

