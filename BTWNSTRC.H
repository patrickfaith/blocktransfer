// copyright (c) p l faith - all rights reserved

class BTWindow
{
  public:
  
  HANDLE HWindow;
  HANDLE remoteHWindow;
  HDC hDC;
  HDC memDC;
  int X,Y,Width,Height;
  BTfix scaler;
  short pointerToggle ;

  HCURSOR cursor0;
  HCURSOR cursor1;
  HCURSOR cursor2;

  BTviewBlock *view;

  BTWindow();

  void WMLButtonDown(tagMSG& Msg) ; // = [ WM_LBUTTONDOWN ];
  
  void WMRButtonDown(tagMSG& Msg) ; // = [ WM_RBUTTONDOWN ];
  
  void WMSetCursor(tagMSG& Msg)   ; // = [ WM_SETCURSOR ] ;

  void Paint(HDC hDC, PAINTSTRUCT&) ;
};


// global variables for my sanity

struct BTglobal
{
  HANDLE            hInstance;
  BTbookBlock *theBook;

  char        AppName[10] ;
} ;

extern BTglobal BTex;




