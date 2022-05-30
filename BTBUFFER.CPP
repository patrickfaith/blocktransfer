#include <string.h>
#include <ctype.h>
#include <btdefs.h>
#include <btlist.h>
#include <btblocks.h>
#include <btgraph.h>
#include <btproto.h>
 
  BTbuff::BTbuff()
  {
    bytIt = NULL; rowTot = 0; columnTot = 0;
    curRow = 0; curCol = 0; markRow = 0; markCol = 0;  
  }
  BTbuff::BTbuff( short columns,short rows)
  {
    columnTot = columns; rowTot = rows;
    bytIt = new char [ rows*columns  ] ;
    curRow = 0; curCol = 0; markRow = 0; markCol = 0;
    Blank();
  }

  void BTbuff::Setup(short columns,short rows)
  { 
    if ( bytIt != NULL )
      delete bytIt;
    bytIt = new char [ rows*columns ] ;  
    columnTot = columns; rowTot = rows;
    curRow = 0; curCol = 0; markRow = 0; markCol = 0;
    Blank();
  }

  void BTbuff::Blank()
  {
      char *ch = bytIt;
      short i = 0 ;
      for  ( i = 0 ; i <  rowTot*columnTot ; i++ )
      {
        *ch = ' ';
         ch++;
      }      
  } 
  
  char *BTbuff::Line(short i) 
  { 
    if ( i >= rowTot ) return NULL ;
    return  bytIt + i * columnTot ;
  } 
 
  void BTbuff::scroll()
  { 
    if (rowTot > 1 )
    { 
      // first move all the rows
     
      char *ch = bytIt + (rowTot-1)*columnTot ;
      while ( ch >= bytIt )
      {
        *(ch+columnTot) = *ch;
        ch--;
      }

      // then blank fill current row  
      ch = bytIt;
      while ( ch < bytIt+columnTot )
      {
        *ch = ' ';
        ch++;
      }      
    }
    else
    {
      Blank();
    }
  }
  
  void BTbuff::checkColumn()
  {
    if (curCol >= columnTot )
    {
      scroll() ;
      curRow = 0;
      curCol = 0;
    }
  }
  void BTbuff::addCurrentChar(char ch)
  { 
    if (bytIt != NULL )
    { 
      char *chPtr = bytIt + curRow*columnTot + curCol; 
      *chPtr  = ch;
      curCol += 1;
      checkColumn();
    }
  }
  
void BTbuff::ProccessKeystroke(WPARAM wParam)
{
  if ( isprint(wParam) )
  {
    char ch;
    ch = (char) wParam;
    addCurrentChar(ch);
  }
  else if ( wParam == '\b' )  // backspoace
  {
   Left();addCurrentChar(' ');Left();
  }  
}

 void BTbuff::addCurrentString(char *text)
 {
   // strc (lines[curRow],text);
 }
 
 
 void BTbuff::DisplayBuff(HDC hDC,BTWindow *wndw, BTgraph *graph,int dispType)
 {
 
   if (wndw == NULL) return ;
   int x = 0 ;
   int y = 0 ;
   int dx = 10;
   
  // just start at the bottem of the screen

  if ( graph != NULL )
  {
    if ( graph->bookPix1[0] < 1000 )
    {
      char *linePtr = NULL;
      if ( (linePtr = Line(0)) != NULL )
      {
        TextOut(hDC,graph->bookPix1[0],graph->bookPix1[1],
                linePtr,columnTot);
      }
    }
  }
  else
  { 
    int height = 15; // HIWORD( GetTextExtent(hDC,"Test",4) );
    SetBkMode(hDC,TRANSPARENT);
    SetTextColor(hDC,RGB(255,255,255));
    HFONT hFont = CreateFont( height, 0, //10
         0 , 0  , 0 , 0 , 0 , 0 ,
	     ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
	     DEFAULT_QUALITY, FF_MODERN, NULL );
    SelectObject(hDC, hFont );
    
    int posY = wndw->Height ;
    char *linePtr = NULL;
    int line =0 ;
    while ( posY > 0 )
    {
      posY -= height;
      if ( (linePtr = Line(line)) == NULL ) break ;
      line++;
      TextOut(hDC,1,posY,linePtr,columnTot);
    }
    DeleteObject(hFont);
  }
}
