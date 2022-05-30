//
// Copyright (c) 1993, P L Faith, all rights reserved
//  BT buffer thingies
//


class BTbuff
{
  public:

  char *bytIt;

  short rowTot;
  short columnTot;
  short curRow;
  short curCol;
  short markRow;
  short markCol;
  
  BTbuff() ;

  BTbuff( short columns,short rows) ;

  void Setup(short columns,short rows);

  ~BTbuff()
  {
    delete bytIt;
  }
  
  void Blank();
  
  char *Line(short i); 
 
  void scroll();
  
  void checkColumn();

  void addCurrentChar(char ch);

  void addCurrentString(char *text);

  void ProccessKeystroke(WPARAM wParam) ;
  
  void DisplayBuff(HDC hDC,BTWindow *wndw,BTgraph *graph,int dispType);
  
  void Up();
  void Down();
  void Left(){ curCol -= 1; if (curCol<0) curCol = 0; }
  void Right();
  void Mark();
  void Mark(short column, short row);
  void ScrollUp();
} ;
 