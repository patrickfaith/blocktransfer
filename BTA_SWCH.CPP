// BTacmdsp.cpp
// dispatches commands to the appropriate routine
#include <btdefs.h>
#include <btlist.h>
#include <btblocks.h>
#include <btgraph.h>
#include <btacmdef.h>
#include <btacmstr.h>
#include "btproto.h"

int BTaccum::CommandSwitch(BTblock *block)
{
  int status = 1;
  BTcommandModes  *cmdModes = block->CommandModes(NULL);
  BTcommandPtrs   *cmdPtrs = NULL;
  registers[ACCUM_PC] = block;
  // BTcommandModes *cmdPtrs  = block->CommandPtrs(cmd); // not needed in many cases
  if ( cmdModes == NULL ) 
    return status;
  switch (cmdModes->opCode)
  { 
    case OPER_ITER_CHILD:		// @iter child [x]
    IterChild( cmdModes );
    break;
    case OPER_ITER_PARENT:		// @iter parent [x]
    IterParent( cmdModes );
    break;
    case OPER_ITER_INPUT:		// @iter input [x]
    IterInput( cmdModes );
    break;
    case OPER_ITER_OUTPUT:		// @iter output [x]
    IterInput( cmdModes );
    break;
    case OPER_ITER_NEXT:		// @iter next [x]
    IterNext( cmdModes );
    break;
    case OPER_ITER_PREV:		// @iter prev [x]
    IterPrev( cmdModes );
    break;
    case OPER_ITER_DEL:			// @iter del [x]
    IterDel( cmdModes );
    break;
    case OPER_ITER_ADD:			// @iter add [x] x[x]
    IterAdd( cmdModes );
    break;
    case OPER_ITER_REPLACE:		// @iter repl [x] x[x]
    IterReplace( cmdModes );
    break;
    case OPER_ITER_HIGH:		// @iter high [x]
    IterHigh( cmdModes );
    break;
    case OPER_ITER_LOW:			// @iter low  [x]
    break;
    case OPER_ITER_FIND:		// @iter find [x] x[x]
    IterFind( cmdModes );
    break;
    case OPER_ITER_MATCH:    	// @iter match x[x] x[x]
      break;
    case OPER_ITER_TOUCH:		// @iter touch x[x] x[x]
      break;
    case OPER_LOAD_BLOCK:
      LoadBlock(block, cmdModes );
      break;
    case OPER_LOAD_NULL:
      LoadNull(cmdModes );
      break;                       
    case OPER_LOAD_PAGE:
      LoadPage(block, cmdModes );
      break;                       

    case OPER_MOVE_STRING:				// @move x[x] [x]
      MoveString( cmdModes );
      break;
    case OPER_MOVE_ENERGY:
      MoveEnergy( cmdModes );				
      break;
    case OPER_MOVE_BRATIO:
      MoveBratio( cmdModes );				
      break;
    case OPER_MOVE_TRATIO:
      MoveTratio( cmdModes );				
      break;
    case OPER_MOVE_WEIGHT:				
      MoveWeight( cmdModes );
      break;
    case OPER_MOVE_VSIZ:				
      MoveSize( cmdModes );
      break;
    case OPER_MOVE_ROTATE:
      MoveRotate( cmdModes );
      break;
    case OPER_MOVE_SCALE:
      MoveScale( cmdModes );
      break;
    case OPER_MOVE_BLOCK:				
      MoveBlock( cmdModes );
      break;
    case OPER_MOVE_EDGE:
      MoveGraph( cmdModes, mathRegs[8] ) ;
      break;
    case OPER_MOVE_POSIT:
      MoveTrans( cmdModes ) ;
      break;          
    case OPER_BREAK:
      status = STATUS_BREAK ;
      break;
    case OPER_BREAK_NULL: 		// @break null x[x]
      status = breakDna(cmdModes); 
      break;
    case OPER_BREAK_NONULL:		// @break nonull x[x]
      status = breakDna(cmdModes) ;
      break;
    case OPER_CYCLE:
      status = STATUS_CYCLE  ;
      break;
    case OPER_BOT:				// @bot x[x]
      Translate(block,cmdModes) ;
      break;
    case OPER_PROCESS:
      status = Process(block,cmdModes) ;
      break;
    case OPER_PROCESS_MSG:
      status = ProcessMessage(block,cmdModes) ;
      break;
    case OPER_COMPARE_NAME:
      CompareName( cmdModes ) ;
      break ;
      
// where x = f(ixed point), v(ector fixed point), i(integer)
    case OPER_MATH_LOAD:		// @math load x[m] string_value
    break;
    case OPER_MATH_ADD:			// @math add x[m] x[m] x[m]
      MathAdd( cmdModes );
      break;
    case OPER_MATH_SUB:			// @math sub x[m] x[m] x[m]
      MathSubtract( cmdModes );
      break;
    case OPER_MATH_DIV:			// @math div x[m] x[m] x[m]
      MathMultiply( cmdModes );
      break;
    case OPER_MATH_MUL:			// @math mul x[m] x[m] x[m]
      MathMultiply( cmdModes );
      break;
    case OPER_MATH_DIS:			// @math dis v[m] v[m] f[m]
    break;

    case OPER_BMP_FILE:			// @bmp file x[x] string_value
    break;
    case OPER_BMP_DELETE:		// @bmp del x[x]
    break;
    case OPER_BMP_PAL:			// @bmp pal x[x]
    break;
    case OPER_BMP_BACK:			// @bmp back x[x]
    break;

    case OPER_FILE_SAVE:
      FileSave( cmdModes );
      break;
    case OPER_FILE_LOAD:
      FileLoad( cmdModes );
      break;
    case OPER_GRAPH_CONSTRUCT:    
      MapSpiral( cmdModes );
      break ;
    case OPER_GRAPH_PAGETOVIEW:    
      MapView( cmdModes );
      break ; 
    case OPER_GRAPH_VIEWTOPIX:    
      MapWindow( cmdModes );
      break ;
    case OPER_GRAPH_DISPLAY:    
      DisplayGraphics( cmdModes ) ;
      break ; 
    case OPER_GRAPH_EDGE:    
      CreateEdge( cmdModes );
      break ; 
    case OPER_GRAPH_FACET:    
      CreateFacet( cmdModes );
      break ; 

    case OPER_DELETE_BLOCK:
      DeleteBlock(cmdModes);
      break;
    case OPER_CREATE_BASIC:		// @create block (object_type) [x]
      CreateBlock(cmdModes,block,BT_TYPE_BASIC);
      break;
    case OPER_CREATE_GROUP:
      CreateBlock(cmdModes,block,BT_TYPE_GROUP);
      break;
    case OPER_CREATE_CHUNK:
      CreateBlock(cmdModes,block,BT_TYPE_CHUNK);
      break;
    case OPER_CREATE_TRANSFER:
      CreateBlock(cmdModes,block,BT_TYPE_TRANSFER);
      break;
    case OPER_CREATE_STRING:
      CreateBlock(cmdModes,block,BT_TYPE_STRING);
      break;
    case OPER_CREATE_READ:
      CreateBlock(cmdModes,block,BT_TYPE_READ);
      break;
    case OPER_CREATE_PAGE:
      CreateBlock(cmdModes,block,BT_TYPE_PAGE);
      break;
    case OPER_CREATE_BOOK:
      CreateBlock(cmdModes,block,BT_TYPE_BOOK);
      break;
    case OPER_CREATE_CFACE:
      CreateBlock(cmdModes,block,BT_TYPE_CFACE);
      break;
    case OPER_CREATE_BOTS:
      CreateBlock(cmdModes,block,BT_TYPE_BOTS);
      break;
    case OPER_CREATE_VIEW:
      CreateBlock(cmdModes,block,BT_TYPE_VIEW);
      break;
    case OPER_CREATE_FIELD:
      CreateBlock(cmdModes,block,BT_TYPE_FIELD);
      break;
    case OPER_CREATE_MESSAGE:
      CreateBlock(cmdModes,block,BT_TYPE_MESSAGE);
      break;
    case OPER_CREATE_ACCUM:
      createAccum(cmdModes);
      break;

  }
  return status;
}