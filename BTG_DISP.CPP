// calling links into the accumulator for graphic stuff
#include <btdefs.h>
#include <btlist.h>
#include <btblocks.h>
#include <btgraph.h>
#include <btacmdef.h>
#include <btacmstr.h>
#include <btproto.h>

// @graph display <blk>  <trn>
// 		where <blk> is the view
//      and <trn> is the block to be viewed

HPEN colorNewPen(int depth)
{ 
  char r,g,b;
  COLORREF cRef;
  HPEN newPen;
  if ( depth > 0 )
  { 
    b = (depth>>1) & 255;
    r = b >>1;
    g = b >>1 ;
    cRef = RGB( r,g,255) ;
    newPen = CreatePen(PS_SOLID,1,cRef);
  }
  else
  { 
    b = depth & 127;
    r = b>>1;
    g = b>>1; // b >>1 ;
    cRef = RGB( r,g,120) ;
    newPen = CreatePen(PS_SOLID,1,cRef);
  }
/*  if ( depth > 0 )   
    newPen = CreatePen(PS_SOLID,1,RGB(255,255,255));  // white
  else if ( depth > -3500)
    newPen = CreatePen(PS_SOLID,1,RGB(200,200,200));  // grey
  else if ( depth > -8000)
    newPen = CreatePen(PS_SOLID,1,RGB(100,100,100));  // grey
  else
    newPen = CreatePen(PS_SOLID,1,RGB(25,25,25));  // grey
*/ 
//   if ( depth == 1 )
//    newPen = CreatePen(PS_SOLID,1,RGB(100,0,0));  // dark red
//  else if ( depth == 2 )
//    newPen = CreatePen(PS_SOLID,1,RGB(0,100,0)); // dark green
//  else
//    newPen = CreatePen(PS_SOLID,1,RGB(0,0,100)); // dark blue
  return newPen;
}

/* does nuthing
void DisplayGraphic(HDC hDC, float btEnergy, BTgraph *graph)
{
  BTgraph *grfPtr;
  grfPtr = graph;
  while ( grfPtr != NULL )
  {
    if ( grfPtr->bookPix1[0] < 1000 )
    {

    }
    grfPtr = grfPtr->next;
  }
}
*/

void BTgraph::DisplayGraphics(BTWindow *wndw)
{
  if ( ( bookPix1[0] <= 0 ) ||
       ( bookPix2[0] <= 0 ) )  return ;
  HDC hDC = wndw->memDC;
  HPEN newPen,hOldPen;

  newPen = colorNewPen(zox[2]) ;
  hOldPen = SelectObject(hDC,newPen);
  MoveTo(hDC,bookPix1[0],bookPix1[1] ) ;
  LineTo(hDC,bookPix2[0],bookPix2[1] ) ;
  SelectObject(hDC,hOldPen);
  DeleteObject(newPen);  
}
void BTchipGraph::DisplayGraphics(BTWindow *wndw)
{ 
  if ( ( bookPix1[0] <= 0 ) ||
       ( bookPix2[0] <= 0 ) ||
       ( bookPix3[0] <= 0 ) ||
       ( bookPix4[0] <= 0 ) )
   return ;
  
  HDC hDC = wndw->memDC;
  POINT pts[5];
  HPEN newPen,hOldPen;
  
  pts[0].x = bookPix1[0];
  pts[0].y = bookPix1[1];
  pts[1].x = bookPix2[0];
  pts[1].y = bookPix2[1];
  pts[2].x = bookPix3[0];
  pts[2].y = bookPix3[1];
  pts[3].x = bookPix4[0];
  pts[3].y = bookPix4[1];
  pts[4].x = bookPix1[0];
  pts[4].y = bookPix1[1];
  
  newPen = colorNewPen(0) ;
  hOldPen = SelectObject(hDC,newPen);
  Polygon(hDC,pts,5);

  SelectObject(hDC,hOldPen);
  DeleteObject(newPen);
}
void BTblock::DisplayGraphics(BTWindow *wndw)
{
  if ( graph != NULL )
    graph->DisplayGraphics(wndw); 
}
void BTpageBlock::DisplayGraphics(BTWindow *wndw)
{ 
  BTblock *blockPtr;
  BTlistIterator  dispList(blockChildren);
  while ( (blockPtr=dispList.Iterate())!=NULL )
    blockPtr->DisplayGraphics( wndw );

  if ( graph != NULL )
    graph->DisplayGraphics(wndw); 
}

void BTviewBlock::DisplayGraphics(BTWindow *wndw)
{ 

  RECT rect;
  
  if ( graphWin->HWindow == NULL ) return ;
  graphWin->hDC = GetDC(graphWin->HWindow);
  
  BTbookBlock *bkPtr = BookPtr();
  ActivatePallete(graphWin->hDC, bkPtr);
  
  GetClientRect(graphWin->HWindow,&rect);
  graphWin->Width = rect.right;
  graphWin->Height = rect.bottom;
  graphWin->memDC = CreateCompatibleDC(graphWin->hDC);
  HBITMAP memBit = CreateCompatibleBitmap(graphWin->hDC,rect.right,rect.bottom);
  HBITMAP oldBitmap = SelectObject(graphWin->memDC,memBit);
  
  FillRect(graphWin->memDC,&rect,GetStockObject(BLACK_BRUSH));
  ActivatePallete(graphWin->memDC, bkPtr); 
     
  BTblock *blockPtr;
  BTlistIterator  dispList(blockChildren);
  while ( (blockPtr=dispList.Iterate())!=NULL )
    blockPtr->DisplayGraphics( graphWin );

  BitBlt(graphWin->hDC,0,0,rect.right,rect.bottom, graphWin->memDC,0,0,SRCCOPY);

  SelectObject(graphWin->memDC,oldBitmap);
  DeleteDC(graphWin->memDC);
  DeleteObject(memBit);
  
  ReleaseDC(graphWin->HWindow,graphWin->hDC);
}
 