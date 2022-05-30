// copyright (c) p l faith
#include <string.h>
#include <btdefs.h>
#include <btlist.h>
#include <btblocks.h>
#include <btgraph.h>
#include <btacmdef.h>
#include <btacmstr.h>
#include "btproto.h"

void BTWindow::WMLButtonDown(tagMSG& Msg)
{
 //  int x,y;

  pixStruc pix;

  pix.x = LOWORD( Msg.lParam );
  pix.y = HIWORD( Msg.lParam );

  pix.closest = 10000;
  pix.bestBlock = NULL;
  pix.closestZ = -32000;
  if ( BTex.theBook != NULL )
  {
    BTblock *foundBlock = BTex.theBook->PixelToBlock(&pix);
  }
}

void BTWindow::WMRButtonDown(tagMSG& Msg)
{
  if ( pointerToggle == 0 )
  {
    pointerToggle = 1 ;
  }
  else if ( pointerToggle == 1 )
  {
    pointerToggle = 2 ;
  }
  else
  {
    pointerToggle = 0 ;
  }
}

// void TThreeDWindow::WMSetCursor(TMessage& Msg)
void BTWindow::WMSetCursor(tagMSG& Msg)
{
  if ( pointerToggle == 1 )
    SetCursor(cursor1);
  else if ( pointerToggle == 2 )
    SetCursor(cursor2);
  else
    SetCursor(cursor0);

  if (BTex.theBook != NULL )
    BTex.theBook->pointerToggle = pointerToggle;
}


BTblock *BTviewBlock::PixelToBlock( pixStruc *pix )
{
  BTblock *blockPtr = NULL;

  BTlistIterator  blockList(blockChildren);
  while ( (blockPtr=blockList.Iterate())!=NULL )
  {
    blockPtr->PixelToBlock( pix) ;
  }
 
  return pix->bestBlock;
}

int butDiff( pixStruc *pix, BTgraph *graph)
{

  int diff0,diff1;
  int diffTot1,diffTot2;

  int diff = 10000;
  if ( graph != NULL )
  {
    if ( graph->bookPix1[0] < 1000 )
    {
      if ( (diff0 = graph->bookPix1[0] - pix->x )  < 0 )
	diff0 = - diff0;
      if ( (diff1 = graph->bookPix1[1] - pix->y )  < 0 )
	diff1 = - diff1;
      diffTot1 = diff0 + diff1;

      if ( (diff0 = graph->bookPix2[0] - pix->x )  < 0 )
	diff0 = - diff0;
      if ( (diff1 = graph->bookPix2[1] - pix->y )  < 0 )
	diff1 = - diff1;
      diffTot2 = diff0 + diff1;

      if ( diffTot2 < diffTot1 )
	diff = diffTot2;
      else
        diff = diffTot1;
    }
  }
  return diff;
}

int butPageDiff( pixStruc *pix, BTgraph *graph, BTgraph *facade)
{

  int diff = 10000;
  if ( graph == NULL ) return diff ;

  if ( facade != NULL )
  {
    int theType;
    theType = facade->Type() ;
    if ( theType ==  BT_BMP_GRAPH )
    {
      BTbmpGraph *bmpGraph = (BTbmpGraph *) facade ;
      // do x check
      if ( ( pix->x > graph->bookPix1[0] - bmpGraph->width ) &&
	   ( pix->x < graph->bookPix1[0]  ) &&
	   ( pix->y > graph->bookPix1[1] - bmpGraph->heigth ) &&
	   ( pix->y < graph->bookPix1[1] ) )
      {
        diff = 0 ;
      }
    }
  } 
  else if ( graph != NULL )
  {
    if ( graph->bookPix1[0] < 1000 )
    {
      // first check if the button press is over the label

      if ( ( graph->bookPix1[0] < pix->x ) &&
	   ( graph->bookPix1[0] + 13 > pix->x ) &&
	   ( graph->bookPix1[1] < pix->y ) &&
	   ( graph->bookPix1[1] + 13 > pix->y ) )
      {
        diff = 0 ; 
      }
      else
        diff = butDiff(pix,graph);
    }
  }
  return diff;
}

BTblock *BTpageBlock::PixelToBlock(  pixStruc *pix )
{
  BTblock *blockPtr = NULL;

  int diff ;
  diff = butPageDiff(pix,graph,facade) ;

  if (( diff == 0) && (pix->closest == 0))
  {
    if ( graph->bookPos1[2] > pix->closestZ )
    {
      pix->closest = diff;
      pix->bestBlock = this ;
      pix->closestZ = graph->bookPos1[2] ;   
    }
  }
  else if ( diff < pix->closest )
  {
    pix->closest = diff;
    pix->bestBlock = this ;
    pix->closestZ = graph->bookPos1[2] ;
  }

  BTlistIterator  blockList(blockChildren);
  while ( (blockPtr=blockList.Iterate())!=NULL )
  {
    blockPtr->PixelToBlock( pix ) ;
  }
  return NULL;
}

BTblock *BTblock::PixelToBlock( pixStruc *pix )
{
  int diff ;

  diff = butDiff(pix,graph) ;

  if ( diff < pix->closest )
  {
    pix->closest = diff;
    pix->bestBlock = this ;
    return NULL;
  }
  return NULL;
}