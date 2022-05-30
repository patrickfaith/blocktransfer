#include <string.h>
#include <btdefs.h>
#include <btlist.h>
#include <btblocks.h>
#include <btgraph.h>
#include <btproto.h>

HPEN colorNewPen(float btEnergy, BTgraph *graph)
{
  HPEN newPen;
  
  float depth ;

  depth = BTex.theBook->threeD.at.Z  - graph->bookPos1[2] ; // v1->Z) ;

  if ( depth > 1800 )
  {   
    newPen = CreatePen(PS_SOLID,1,RGB(100,100,100));  // dark grey
  }
  else if ( depth > 500 )
  {
    if      (btEnergy < .0001 )
      newPen = CreatePen(PS_SOLID,1,RGB(100,0,0));  // dark red
    else if (btEnergy < .01 )
      newPen = CreatePen(PS_SOLID,1,RGB(0,100,0)); // dark green
    else
      newPen = CreatePen(PS_SOLID,1,RGB(0,0,100)); // dark blue
  }
  else
  {
    if      (btEnergy < .0001 )
      newPen = CreatePen(PS_SOLID,1,RGB(255,0,0));  // light red
    else if (btEnergy < .01 )
      newPen = CreatePen(PS_SOLID,1,RGB(0,255,0)); // light green
    else
      newPen = CreatePen(PS_SOLID,1,RGB(0,0,255)); // light blue
  }
 
  return newPen;
}

// ------------

void DisplayGraphic(HDC hDC, float btEnergy, BTgraph *graph)
{
  BTgraph *grfPtr;

  grfPtr = graph;
  while ( grfPtr != NULL )
  {
    if ( grfPtr->bookPix1[0] < 1000 )
    {
      HPEN newPen;
      HPEN hOldPen;

      newPen = colorNewPen(btEnergy,grfPtr) ;

      hOldPen = SelectObject(hDC,newPen);
      MoveTo(hDC,grfPtr->bookPix1[0],grfPtr->bookPix1[1] ) ;
      LineTo(hDC,grfPtr->bookPix2[0],grfPtr->bookPix2[1] ) ;
      SelectObject(hDC,hOldPen);
      DeleteObject(newPen);
    }
    grfPtr = grfPtr->next;
  }
}

void DisplayText(HDC hDC, float btEnergy, BTgraph *graph, char *text)
{
  BTgraph *grfPtr;

  grfPtr = graph;
  if ( grfPtr != NULL )
  {
    if ( grfPtr->bookPix1[0] < 1000 )
    {

      SetBkMode(hDC,TRANSPARENT);

      float depth ;
      depth = BTex.theBook->threeD.at.Z  - graph->bookPos1[2] ;

      int TextHt = 10 ; // HIWORD( GetTextExtent(hDC,"Test",4) );
      int TextWd = 10 ; // HIWORD( GetTextExtent(hDC,"Test",4) );
      int TextWeight = 500 ;

      
      if ( depth > 1600 )
      {
	TextHt -= 9;
	TextWd -= 9;
	TextWeight -= 9 * 40 ;
	SetTextColor(hDC,RGB(100,100,100));
      }
      else if ( depth < 700 )
      {
        SetTextColor(hDC,RGB(255,255,255));
        TextWeight = 600 ; // just do the default everything
      }
      else
      {
        SetTextColor(hDC,RGB(255,255,255));
        depth -= 700 ;        // make depth a number between 0 and 9
	depth /= 150 ;

	TextHt -= depth;
	TextWd -= depth;
        TextWeight -= depth * 40 ; 
      }

      HFONT hFont = CreateFont( TextHt, TextWd,
         0 , 0  , TextWeight , 0 , 0 , 0 ,
	 ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
	 DEFAULT_QUALITY, FF_SWISS, "Helv" ); //  FF_ROMAN, "Roman" );

     

      SelectObject(hDC, hFont );

      TextOut(hDC,grfPtr->bookPix1[0],grfPtr->bookPix1[1],
	      text,lstrlen(text) );

      DeleteObject(hFont);

    }
    grfPtr = grfPtr->next;
  }
}

// this displays the children block connections for some reason
// i would think it would display what ever is the current list ... but

void DisplayBlockTransfers(HDC hDC, BTblock *chunkBlk)
{
  BTgraph *grfPtr,*chunkGrfPtr;
  BTblock *blockPtr;
  if (chunkBlk == NULL) return;
  chunkGrfPtr = chunkBlk->graph;
  if (chunkGrfPtr == NULL) return ;
  if (chunkGrfPtr->bookPix1[0] > 1000 ) return ;

  BTlistIterator  blockList(chunkBlk->blockChildren);
  while ( (blockPtr=blockList.Iterate())!=NULL )
  { 
    grfPtr = blockPtr->graph;
    if ( grfPtr != NULL )
    {
      if ( grfPtr->bookPix1[0] < 1000 )
      {
	    HPEN newPen;
        HPEN hOldPen;

	    newPen = colorNewPen(blockPtr->btEnergy,grfPtr) ;
        hOldPen = SelectObject(hDC,newPen);
        MoveTo(hDC,grfPtr->bookPix1[0],grfPtr->bookPix1[1] ) ;
        LineTo(hDC,chunkGrfPtr->bookPix2[0],chunkGrfPtr->bookPix2[1] ) ;
        SelectObject(hDC,hOldPen);
        DeleteObject(newPen);
      }
    }
  }
}

HANDLE SetupPallete() // HDC hDC)
{
  HANDLE hPal = NULL;
  int numColors = 64+4*16 ;

  LOGPALETTE *logPal;

  logPal = (LPLOGPALETTE)(new char[sizeof(LOGPALETTE)+
				   sizeof(PALETTEENTRY)*(numColors-1)]);
  logPal->palVersion = 0x300;
  logPal->palNumEntries = numColors;

  int i;
  for (i=0; i<64; i++)
  {
    logPal->palPalEntry[i].peRed   = i*4;
    logPal->palPalEntry[i].peGreen = i*4;
    logPal->palPalEntry[i].peBlue  = i*4;
    logPal->palPalEntry[i].peFlags = 0;
  }
 
 
  hPal = CreatePalette(logPal);
  delete logPal;

  // this might really be the stuff that needs to occur here

  return hPal;

}

HANDLE ActivatePallete(HDC hDC, BTbookBlock *theBook)
{
  HANDLE oldhPal = NULL;

  if ( theBook->hPal == NULL )
  {
    theBook->hPal = SetupPallete() ;
  }  
  oldhPal = SelectPalette(hDC,theBook->hPal,0);
  RealizePalette(hDC);

  return oldhPal;

}

void BTbookBlock::DisplayGraphics(HDC hDC)
{ 
  BTblock *blockPtr;
  BTlistIterator  dispList(transferOutput);
  while ( (blockPtr=dispList.Iterate())!=NULL )
    blockPtr->DisplayGraphics(hDC);
}

void BTpageBlock::DisplayGraphics(HDC hDC)
{
  BTblock *blockPtr;

  if ( (deActivate & DISP_FLAG) != NULL ) return ;

  if ( ( facade != NULL ) &&
       ( (deActivate & DISP_FACADE_FLAG) == 0 ) )
  {
    DisplayFacade(hDC, this, facade);
    return ;
  }
  else
  {
    DisplayGraphic(hDC, btEnergy, graph);
    DisplayText(hDC,btEnergy,graph,name);
  }

  if ( (deActivate & DISP_CHILD_FLAG) != NULL ) return ;

  BTlistIterator  blockList(blockChildren);
  while ( (blockPtr=blockList.Iterate())!=NULL )
    blockPtr->DisplayGraphics(hDC);
}

void BTblock::DisplayGraphics(HDC hDC)
{
  DisplayGraphic(hDC, btEnergy, graph);
}
