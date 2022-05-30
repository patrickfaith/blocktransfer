// BTacmprs.cpp
// parses and translates text into comands
#include <string.h>
#include <btdefs.h>
#include <btlist.h>
#include <btblocks.h>
#include <btgraph.h>
#include <btacmdef.h>
#include <btacmstr.h>
#include "btproto.h"

struct BTcmdDataExternal *BTax;


char DetermineRegister(char *word)
{
  char type;
  if ( strstr(word, ACCUM_SELECT_ID) != 0 )
    type = ACCUM_SELECT;
  else if ( strstr(word, ACCUM_BLOCK_ID) != 0 )
  	type =  ACCUM_BLOCK;
  else if ( strstr(word, ACCUM_TRANSFER_ID) != 0 )
    type = ACCUM_TRANSFER;
  else if ( strstr(word, ACCUM_RESULT_ID) != 0 )
    type = ACCUM_RESULT;
  else if ( strstr(word, ACCUM_PC_ID) != 0 )
    type = ACCUM_PC;
  else if ( strstr(word, ACCUM_STACK_ID) != 0 )
    type = ACCUM_STACK;
  else if ( strstr(word, ACCUM_EXP_ID) != 0 )
    type = ACCUM_EXP;
  else if ( strstr(word, ACCUM_DEBUG_ID) != 0 )
    type = ACCUM_DEBUG; 
  else if ( strstr(word, ACCUM_PAGE_ID) != 0 )
    type = ACCUM_PAGE;
  else if ( strstr(word, ACCUM_INT_BSEL_ID) != 0 )
    type = ACCUM_INT_BSEL;
  else if ( strstr(word, ACCUM_INT_BEXE_ID) != 0 )
    type = ACCUM_INT_DRAG;
  else if ( strstr(word, ACCUM_INT_DRAG_ID) != 0 )
    type = ACCUM_INT_DROP;
  else if ( strstr(word, ACCUM_INT_DROP_ID) != 0 )
    type = ACCUM_INT_BEXE;
  else if ( strstr(word, ACCUM_INT_BUFF_ID) != 0 )
    type = ACCUM_INT_BUFF;
  else if ( strstr(word, ACCUM_INT_DISP_ID) != 0 )
    type = ACCUM_INT_DISP;
  else
    type = 0;
  return type;  
}
void ParseModes(char *word, BTcommandModes *modes, short btr)
{
  char type=0,reg=0,i;
  if ( strstr(word,"i[") != 0 )
  {
    type = MODE_INDIRECT ;
    reg  = DetermineRegister(word+1);  
  }
  else if ( strstr(word,"c[") != 0 )
  {
    type = MODE_COMPLEX ;
    reg  = DetermineRegister(word+1); 
  }
  else if ( strstr(word,"f[") != 0 )
  {
    type = MODE_FIXED;
    reg  = *(word+2) - '0'; 
  }
  else if ( strstr(word,"s[") != 0 )
  {
    type = MODE_STRING;
    reg = 0;	
  }
  else if ( strstr(word,"[]") != 0 )
  {
    type = MODE_PLACE;
    reg  = 0;  
  }
  else if ( strstr(word,"k[") != 0 )
  { 
    
    type = MODE_KERNAL;
    for ( i=0;i<MAX_NUM_SYS_REGS;i++)
    
      if ( ( BTax->sysRegs[i].length != 0 ) &&
           (strncmp(word, BTax->sysRegs[i].name,BTax->sysRegs[i].length) == 0 ) )
      {
        reg  = i ;
        break;
      };  
  }
  else if ( strstr(word,"[") != 0 )
  {
    type = MODE_REGISTER;
    reg  = DetermineRegister(word);  
  } 
  if ( btr == 1 )
  {
    modes->blockMode = type;
    modes->blockRegister = reg;
  }
  else if ( btr == 2 )
  {
    modes->transferMode = type;
    modes->transferRegister = reg;
  }
  else if ( btr == 3 )
  {  
    modes->resultMode = type;
    modes->resultRegister = reg;  
  }
  return ;
}

int setCommandData(int id,char *cmd)
{ 
  if (cmd == NULL) return 0 ;
  if ( id >  MAX_NUM_COMMANDS ) return 0;
  BTax->cmdData[id].name = new char [strlen(cmd)+1];
  strcpy( BTax->cmdData[id].name, cmd);
  BTax->cmdData[id].length = strlen(cmd);
  return 1;
}

int setSysRegData(int id,char *cmd)
{ 
  if (cmd == NULL) return 0 ;
  if ( id >  MAX_NUM_SYS_REGS ) return 0;
  BTax->sysRegs[id].name = new char [strlen(cmd)+1];
  strcpy( BTax->sysRegs[id].name, cmd);
  BTax->sysRegs[id].length = strlen(cmd);
  return 1;
} 

BTdnaBlock *ParseDnaCommand(BTaccum *accum, BTdnaBlock *block, char *text)
{
  BTcommandModes *modes = NULL;
  
  char *cmdString = NULL;
  char *dnaString = NULL;
  int i;
  for ( i=0;i<MAX_NUM_COMMANDS;i++)
  { 
    // find correct opcode to string match
    if ( ( BTax->cmdData[i].length != 0 ) && 
    
          (strncmp(text,
                 BTax->cmdData[i].name,
                 BTax->cmdData[i].length) == 0 ) )
    {
      modes = new BTcommandModes();
      modes->opCode = i;
      char s1[90],s2[90],s3[90],scount;
      char *regText = text + BTax->cmdData[i].length ;
      scount = sscanf(regText,"%s %s %s",s1,s2,s3);
      if (scount > 0 )
        ParseModes(s1,modes,1);
      if (scount > 1 )
        ParseModes(s2,modes,2);
      if (scount > 2 )
        ParseModes(s3,modes,3);      
      // figure out text string
      cmdString = strstr(text,":");
     if ( cmdString != NULL )
      { 
        cmdString += 1;
        int count =  strlen ( cmdString ) + 1 ;
        dnaString = new char [ count ];
        strcpy ( dnaString,cmdString);
      }      
      break;    
    }
  }
  if ( modes == NULL ) return NULL;
  
  if (block != NULL )
  {
    block->cmdModes = modes;
    block->string = dnaString;
  }
  else
  {
    block = (BTdnaBlock *) accum->MakeDnaBlock(modes,dnaString);
  }

  return block;
  
}


BTblock *BTaccum::ParseCommand(char *text)
{
  BTcommandModes *modes = NULL;
  BTblock *dnaPtr = NULL;
  
  
  dnaPtr = ParseDnaCommand(this, NULL, text);

  return dnaPtr;   
}
void initCommandData()
{
  BTax = new BTcmdDataExternal();
  int i;
  for (i=0;i<256;i++)
  {
    double iFloat = ((double) i) *360.0/255.0;
    BTax->cosFast[i] = initCosF(iFloat);
  }
  
  for (i=0;i<256;i++)
  {
    double iFloat = ((double) i) *360.0/255.0;
    BTax->sinFast[i] = initSinF(iFloat);
  }  
  
  
  setCommandData( OPER_NOOP,  "@noop" );
  setCommandData( OPER_ITER_CHILD,		"@iter child");
  setCommandData( OPER_ITER_PARENT,	    "@iter parent");
  setCommandData( OPER_ITER_INPUT,		"@iter input");
  setCommandData( OPER_ITER_OUTPUT,	    "@iter output");
  setCommandData( OPER_ITER_NEXT,		"@iter next");
  setCommandData( OPER_ITER_PREV,		"@iter prev");
  setCommandData( OPER_ITER_DEL,		"@iter del");
  setCommandData( OPER_ITER_INSERT,	    "@iter insert");
  setCommandData( OPER_ITER_REPLACE, 	"@iter replace");
  setCommandData( OPER_ITER_HIGH,		"@iter high");
  setCommandData( OPER_ITER_LOW,		"@iter low");
  setCommandData( OPER_ITER_FIND,		"@iter find");
  setCommandData( OPER_ITER_MATCH,      "@iter match");
  setCommandData( OPER_ITER_TOUCH,		"@iter touch");
  setCommandData( OPER_ITER_ADD_END,    "@iter addend");
  setCommandData( OPER_ITER_ADD,	    "@iter add");
  
  setCommandData( OPER_LOAD_BLOCK,		"@load block");
  setCommandData( OPER_LOAD_NULL,		"@load null");  
  setCommandData( OPER_LOAD_PAGE,		"@load page");
  
  setCommandData( OPER_FILE_LOAD,		"@file load");
  setCommandData( OPER_FILE_SAVE,		"@file save");


  setCommandData( OPER_MOVE_STRING,		"@move string");
  setCommandData( OPER_MOVE_ENERGY,     "@move energy");
  setCommandData( OPER_MOVE_BRATIO,     "@move bratio");
  setCommandData( OPER_MOVE_TRATIO,     "@move tratio");
  setCommandData( OPER_MOVE_WEIGHT,     "@move weight");

  setCommandData( OPER_MOVE_VPOS,		"@move position");
  setCommandData( OPER_MOVE_VSIZ,		"@move size");
  setCommandData( OPER_MOVE_ROTATE,     "@move rotate");
  setCommandData( OPER_MOVE_SCALE,      "@move scale");
  setCommandData( OPER_MOVE_BLOCK,      "@move block");
  setCommandData( OPER_MOVE_POSIT,      "@move place");
  setCommandData( OPER_MOVE_EDGE,       "@move edge");
      
  setCommandData( OPER_BREAK_NULL,     "@break null");
  setCommandData( OPER_BREAK_NONULL,   "@break nonull");
  setCommandData( OPER_BREAK_LOW,      "@break low");
  setCommandData( OPER_BREAK_HIGH,     "@break high");
  setCommandData( OPER_BREAK,          "@break");

  setCommandData( OPER_CYCLE,          "@cycle");
        
  setCommandData( OPER_BOT,     		"@bot");
  setCommandData(OPER_PROCESS,  		"@proc");
  setCommandData(OPER_PROCESS_MSG,		"@msg");
  
  setCommandData(OPER_COMPARE_NAME,		"@comp name");  

  setCommandData( OPER_MATH_ADD,     "@math add");  
  setCommandData( OPER_MATH_SUB,     "@math sub");  
  setCommandData( OPER_MATH_MUL,     "@math mul");
  
  setCommandData( OPER_GRAPH_CONSTRUCT,	"@graph construct");
  setCommandData( OPER_GRAPH_PAGETOVIEW,"@graph map");
  setCommandData( OPER_GRAPH_VIEWTOPIX, "@graph pixel");
  setCommandData( OPER_GRAPH_DISPLAY,   "@graph display");
  setCommandData( OPER_GRAPH_EDGE,   	"@graph edge");
  setCommandData( OPER_GRAPH_FACET,   	"@graph facet");

  setCommandData( OPER_DELETE_BLOCK,	"@delete block");
  setCommandData( OPER_CREATE_BASIC,	"@create basic");
  setCommandData( OPER_CREATE_GROUP,	"@create group");
  setCommandData( OPER_CREATE_CHUNK,    "@create chunk");
  setCommandData( OPER_CREATE_STRING,	"@create string");
  setCommandData( OPER_CREATE_READ,		"@create read");
  setCommandData( OPER_CREATE_PAGE,		"@create page");
  setCommandData( OPER_CREATE_BOOK,		"@create book");
  setCommandData( OPER_CREATE_CFACE,    "@create cface");
  setCommandData( OPER_CREATE_OPERATE,  "@create operate");      
  setCommandData( OPER_CREATE_BOTS,     "@create bot");      
  setCommandData( OPER_CREATE_VIEW,     "@create view");      
  setCommandData( OPER_CREATE_FIELD,    "@create field");      
  setCommandData( OPER_CREATE_MESSAGE,  "@create message");
  setCommandData( OPER_CREATE_ACCUM,    "@create accum");
  
  setSysRegData( REG_SYS_DRAGVIEW 		,"k[dragView]");
  setSysRegData( REG_SYS_DROPVIEW 		,"k[dropView]");
  setSysRegData( REG_SYS_DRAGBLOCK 		,"k[dragBlock]");
  setSysRegData( REG_SYS_DROPBLOCK 		,"k[dropBlock]");
  setSysRegData( REG_SYS_SELVIEW 		,"k[selView]");
  setSysRegData( REG_SYS_SELBLOCK 		,"k[selBlock]");
  setSysRegData( REG_SYS_SCHEDULE		,"k[schedule]");
  setSysRegData( REG_SYS_L_BUTTON		,"k[l_but]"); 
  setSysRegData( REG_SYS_M_BUTTON		,"k[m_but]");
  setSysRegData( REG_SYS_R_BUTTON		,"k[r_but]");
  setSysRegData( REG_SYS_DDEACCUM		,"k[dde]");
  setSysRegData( REG_SYS_DICTIONARY     ,"k[dictionary]");
}

void closeCommandData()
{
  delete BTax ;
}
