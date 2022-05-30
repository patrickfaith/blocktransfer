// calling links into the accumulator for graphic stuff
#include <btdefs.h>
#include <btlist.h>
#include <btblocks.h>
#include <btgraph.h>
#include <btacmdef.h>
#include <btacmstr.h>
#include <btproto.h>

int BTaccum::CreateEdge( BTcommandModes  *cmdModes )
{ 
  BTblock *blockPtr;
  blockPtr = registers[cmdModes->blockRegister];
  if ( blockPtr == NULL ) return 0;
  // transPtr = registers[cmdModes->transferRegister];
  if (blockPtr->graph == NULL )
    blockPtr->graph = new BTgraph();
  return 1;
}
int BTaccum::CreateFacet( BTcommandModes  *cmdModes )
{ 
  BTblock *blockPtr;
  blockPtr = registers[cmdModes->blockRegister];
  if ( blockPtr == NULL ) return 0;
  // transPtr = registers[cmdModes->transferRegister];
  if (blockPtr->graph == NULL )
    blockPtr->graph = new BTchipGraph();
  return 1;
}    
int BTaccum::MapSpiral( BTcommandModes  *cmdModes )
{ 
  BTblock *blockPtr,*transPtr;
  blockPtr = registers[cmdModes->blockRegister];
  if ( blockPtr == NULL ) return 0;
  transPtr = registers[cmdModes->transferRegister];
  // if f[0] is not -1 then load placement variables into placeptr
  BTplace *placePtr = blockPtr->PlacePtr();
  if ( (placePtr != NULL) && ( mathRegs[2] > 0 ) )
  {
    placePtr->pos[0]   = mathRegs[0];
    placePtr->pos[1]   = mathRegs[1];
    placePtr->pos[2]   = mathRegs[2];    
    placePtr->shape[0] = mathRegs[3];
    placePtr->shape[1] = mathRegs[4];
    placePtr->shape[2] = mathRegs[5];    
  }
  blockPtr->PlaceGraph(transPtr);
  return 1;
}
int BTaccum::MapView( BTcommandModes  *cmdModes )
{ 
  BTblock *blockPtr;

  blockPtr = registers[cmdModes->blockRegister];
  if ( blockPtr == NULL ) return 0;
  //   if ( cmdModes->transferMode == MODE_REGISTER )
  //{
  //  transPtr = registers[cmdModes->transferRegister];
  //  blockPtr->PageToBook(transPtr);
  //}
  //else    BTblock *transPtr;
  
  if ( blockPtr->Type() == BT_TYPE_VIEW )
    blockPtr->PageToBook(NULL);
  else
    blockPtr->PageToBook(theBlock);
  return 1;
}
int BTaccum::MapWindow( BTcommandModes  *cmdModes )
{ 
  BTblock *blockPtr=NULL,*transPtr=NULL;
  BTviewBlock *viewPtr;
  // check for register errors & setup variables
  if ( cmdModes->blockMode == MODE_REGISTER )
    blockPtr = registers[cmdModes->blockRegister];
  if ( cmdModes->transferMode == MODE_REGISTER )
    transPtr = registers[cmdModes->transferRegister];
  if ( blockPtr == NULL ) return 0;
  if ( transPtr == NULL )
  {
    if ( blockPtr->Type() != BT_TYPE_VIEW  )
      viewPtr = ( BTviewBlock *) theBlock;
    else                                      // dr1 march 1994 - not fixed
      viewPtr = ( BTviewBlock *) blockPtr;
  }
  else
  {
    if ( transPtr->Type() != BT_TYPE_VIEW  ) return 0;
    viewPtr = ( BTviewBlock *) transPtr;
  }
  // setup the call
  blockPtr->BlocksToPixels(viewPtr);
  return 1;
}

int BTaccum::DisplayGraphics( BTcommandModes  *cmdModes )
{
  BTblock *blockPtr;
  BTWindow *wndw;
  switch (cmdModes->blockMode)
  {
    case MODE_REGISTER:
      blockPtr = registers[cmdModes->blockRegister];
      if (blockPtr->Type() == BT_TYPE_VIEW )
        wndw = ((BTviewBlock *) blockPtr)->graphWin;
      else
      {
        blockPtr = theBlock;
        if (blockPtr->Type() == BT_TYPE_VIEW )
          wndw = ((BTviewBlock *) blockPtr)->graphWin;   
      }
      break ;
    case MODE_INDIRECT:
      blockPtr = iter[cmdModes->blockRegister].CurrentPtr();
      if (blockPtr->Type() == BT_TYPE_VIEW )
        wndw = ((BTviewBlock *) blockPtr)->graphWin;
      break ;
  }
  switch (cmdModes->transferMode)
  {
    case MODE_REGISTER:
      blockPtr = registers[cmdModes->transferRegister];
      break ;
    case MODE_INDIRECT:
      blockPtr = iter[cmdModes->blockRegister].CurrentPtr();
      break ;
  }
  if ( ( blockPtr != NULL ) && ( wndw != NULL ) )
    blockPtr->DisplayGraphics(wndw);
  return 1;
} 