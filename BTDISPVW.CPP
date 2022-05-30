#include <string.h>
#include <btdefs.h>
#include <btlist.h>
#include <btblocks.h>
#include <btgraph.h>
#include <btproto.h>

// build up z-buffer list of thingies
// sort the pages into an order based on z-depth

BTlist *ZlistSort(BTlist *blockChildren)
{
  BTlist *listHead = NULL;
  BTlist *orderList = NULL;
  BTblock *blockPtr;
  
  BTlistIterator  blockList(blockChildren);
  while ( (blockPtr=blockList.Iterate())!=NULL )
    listHead = new BTlist(blockPtr,listHead);

  float bestZ;
  BTblock *bestBlock;
  do
  {
    bestZ = -90000;
    bestBlock = NULL;
    BTlistIterator  deleteList(listHead);
    while ( (blockPtr=deleteList.Iterate() ) != NULL )
    {
      if ( blockPtr->graph != NULL )
	    if ( ( blockPtr->graph->bookPos1[2] > bestZ ) &&
	         ( blockPtr->graph->bookPix1[0] < 1000 ) )
        {
	      bestZ =  blockPtr->graph->bookPos1[2] ;
          bestBlock = blockPtr;
	    }
    }
    if (bestBlock != NULL )  // add best block to order & delete from list
    {
      orderList = new BTlist(bestBlock,orderList);
      deleteList.Reset();
      while ( (blockPtr = deleteList.Iterate()) != NULL )
        if ( blockPtr == bestBlock )
        {
	      listHead = deleteList.removeCurrent();
          break ;
        }
    }       
  } while ( bestBlock != NULL ) ;
  
  BTlistIterator deleteHead(listHead);
  deleteHead.erase();

  return orderList ;
}

/*
void BTviewBlock::DisplayGraphics(BTWindow *wndw)
{ 

  BTblock *blockPtr;
  RECT rect;
  short zapDC = 0;

  graphWin->hDC = GetDC(graphWin->HWindow);
  zapDC = 1;


  GetClientRect(graphWin->HWindow,&rect);
 
  // b4 need to setup window structures ??threeD.Display(hDC,rect);
 
  // b4 old methods setup vector ?? BlocksToPixels(); // prepare blocks to have the pixels ready

  FillRect(graphWin->hDC,&rect,GetStockObject(BLACK_BRUSH));

 // if ( buffer!= NULL )
 //   buffer->DisplayBuff(hDC,graphWin,NULL,0);
     
  // b4 removal DisplayGraphic(hDC, btEnergy, graph);

  //  BTlist *orderList = ZlistSort(blockChildren);
  // BTlistIterator  dispList(orderList);
  
  BTlistIterator  dispList(blockChildren);
  while ( (blockPtr=dispList.Iterate())!=NULL )
    blockPtr->DisplayGraphics( graphWin );

  // BTlistIterator deleteOrder(orderList);
  // deleteOrder.erase();

  if ( zapDC != 0 )
    ReleaseDC(graphWin->HWindow,graphWin->hDC);
}
*/


