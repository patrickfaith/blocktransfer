// general bots routines - copyright (c) p l faith

#include <string.h>
#include <btdefs.h>
#include <btlist.h>
#include <btblocks.h>
#include <btgraph.h>
#include <btacmdef.h>
#include <btacmstr.h>
#include <btproto.h>


// struct BTcmdDataExternal *BTax;


void setRegister(char *word, int reg )
{
  switch (reg)
  {
    case ACCUM_SELECT:
      strcat(word, ACCUM_SELECT_ID);
      break;
    case ACCUM_BLOCK:
      strcat(word, ACCUM_BLOCK_ID);
  	  break;
  	case ACCUM_TRANSFER:
  	  strcat(word, ACCUM_TRANSFER_ID);
      break;
    case ACCUM_RESULT:
      strcat(word, ACCUM_RESULT_ID);
      break;
    case ACCUM_PC:
      strcat(word, ACCUM_PC_ID);
      break;
    case ACCUM_STACK:
      strcat(word, ACCUM_STACK_ID);
      break;
    case ACCUM_EXP:
      strcat(word, ACCUM_EXP_ID);
      break;
    case ACCUM_DEBUG:
      strcat(word, ACCUM_DEBUG_ID);
      break;
    case ACCUM_PAGE:
      strcat(word, ACCUM_PAGE_ID);
      break;
    case ACCUM_INT_BSEL:
      strcat(word, ACCUM_INT_BSEL_ID);
      break;
    case ACCUM_INT_BEXE:
      strcat(word, ACCUM_INT_BEXE_ID);
      break;
    case ACCUM_INT_DRAG:
      strcat(word, ACCUM_INT_DRAG_ID);
      break;
    case ACCUM_INT_DROP:
      strcat(word, ACCUM_INT_DROP_ID);
      break;
    case ACCUM_INT_BUFF:
      strcat(word, ACCUM_INT_BUFF_ID);
      break;
    case ACCUM_INT_DISP:
      strcat(word, ACCUM_INT_DISP_ID);
      break;
  }
  return;  
}

void SetModes(char *word, int type, int reg )
{
  switch  (type )
  {
    case MODE_INDIRECT:
      strcpy(word," i[");
      setRegister(word,reg);
      strcat(word,"]");  
      break;

    case MODE_COMPLEX:
      strcpy(word," c[");
      setRegister(word,reg);
      strcat(word,"]");  
      break;

    case MODE_FIXED:
      { 
        char fch[2];
        fch[0]= '0' + reg ;
        fch[1]= 0;
        strcpy(word," f[");
        strcat(word,fch);
        strcat(word,"]");  
      }
      break;
      
    case MODE_STRING:
      strcpy(word," s[]");
      break;
      
    case MODE_PLACE:
      strcpy(word," []");
      break;           
      
    case MODE_REGISTER:
      strcpy(word," [");
      setRegister(word,reg);
      strcat(word,"]");  
      break;
  }
  return ;
}

void CommandString(char *sent, BTdnaBlock *blockPtr)
{ 
  char word[16];
  
  BTcommandModes *modes = blockPtr->cmdModes;
 
  strcpy(sent,BTax->cmdData[modes->opCode].name );
  
  if (modes->blockMode != MODE_NONE)
  {
    SetModes(word,modes->blockMode,modes->blockRegister );   
    strcat(sent,word);
  }
  if (modes->transferMode != MODE_NONE)
  {
    SetModes(word,modes->transferMode,modes->transferRegister );   
    strcat(sent,word);
  }
  if (modes->resultMode != MODE_NONE)
  {
    SetModes(word,modes->resultMode,modes->resultRegister );   
    strcat(sent,word);
  }
  
  if (blockPtr->string != NULL )
  {
   
    strcat(sent," :") ;
    strcat(sent,blockPtr->string);
  }
  
  return;  
}

void BTdnaBlock::sendUnique(BTtrans *trans)
{ 
  char buffer[132];


  CommandString(buffer, this);
  strcat(buffer,"\n");
  trans->exPort(this,buffer);
 
  return;
}

void BTdnaBlock::receiveUnique(BTtrans *trans)
{
  char *buffer; 
  BTblock *same;
  buffer = trans->inPort(this);
  if (strncmpi(buffer,"@",1) == 0 )
  {

    same = ParseDnaCommand(NULL, this , buffer) ;
  }
  else
  {
    trans->exPort(this,buffer);
  }
  delete buffer;
  return;
}