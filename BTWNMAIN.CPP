
#include <string.h>
#include <btdefs.h>
#include <btlist.h>
#include <btwndef.h>
#include <btblocks.h>
#include <btgraph.h>
#include <btacmdef.h>
#include <btacmstr.h>
#include <btproto.h>
#include <dde.h>

long FAR PASCAL WndProc (HWND, UINT, WPARAM, LPARAM) ;// ( HWND, unsigned, WORD, LONG) ;
struct BTglobal BTex;

int PASCAL WinMain ( HANDLE hInstance, HANDLE HPrevInstance,
		     LPSTR LpszCmdLine, int nCmdShor)
{
  MSG msg;
  WNDCLASS wndClass;
  BOOL fContinue;
  _fstrcpy( BTex.AppName, LpszCmdLine) ;
  
  BTex.hInstance = hInstance;
  
  wndClass.style =  CS_HREDRAW | CS_VREDRAW | CS_BYTEALIGNWINDOW ;
  wndClass.lpfnWndProc = WndProc ; // lpfnWndProc;
  wndClass.cbClsExtra  = 0 ;
  wndClass.cbWndExtra = 0;
  wndClass.hInstance = hInstance;
  wndClass.hIcon     = LoadIcon(hInstance,BTex.AppName);
  wndClass.hCursor   = LoadCursor(NULL,IDC_ARROW ) ;
  wndClass.hbrBackground = GetStockObject(BLACK_BRUSH);
  wndClass.lpszMenuName = BTex.AppName ;
  wndClass.lpszClassName = BTex.AppName ;
  if (!RegisterClass(&wndClass) )
    if ( HPrevInstance == NULL )
      return FALSE ;
  initCommandData();
  BTblock *block;
  block = new BTblock();
  BTex.theBook = new BTbookBlock(BTex.AppName); // "book"
  BTex.theBook->receiveBlockAscii(NULL);
  BTex.theBook->openPort();
  
  BTviewBlock * view = (BTviewBlock *) BTex.theBook->sysRegs[REG_SYS_DROPVIEW];
  

  // WinHelp( view->graphWin->HWindow,"vdna.hlp",HELP_FORCEFILE,NULL);

  fContinue = TRUE ;
  while ( fContinue )
  {
    if ( PeekMessage ( &msg, NULL, 0, 0, PM_REMOVE ) )
    {
      if ( msg.message != WM_QUIT )
      {
        TranslateMessage (&msg);
        DispatchMessage(&msg);
      }
      else
      { 
        delete BTex.theBook ;
        BTex.theBook = NULL;
        fContinue = FALSE ;
      }
    }
    else
    { 
      if ( BTex.theBook->sysRegs[REG_SYS_SCHEDULE] != NULL )
        BTex.theBook->accum->CommandSwitch(
           BTex.theBook->sysRegs[REG_SYS_SCHEDULE] );
    }  
  }
  
// old way  while ( GetMessage ( &msg, NULL, 0, 0)  )
//  {
//    TranslateMessage (&msg);
//    DispatchMessage(&msg);
//  }
  return msg.wParam;
}

long FAR PASCAL WndProc( HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM LParam)
{ 
  PAINTSTRUCT ps;
  HDC         hDC;
  BTmessageBlock blkMsg;
  LPSTR lpCommand;
  int junk = 0;
  switch (iMessage)
  { 
    case WM_PAINT:
      hDC = BeginPaint(hWnd,&ps);
      if ( BTex.theBook != NULL )
      {
        BTblock *theView = FindView(hWnd);
        theView->DisplayGraphics(NULL);
      }
      EndPaint(hWnd,&ps);
      break ;
      
    case WM_LBUTTONDOWN:
      LeftButtonDown(hWnd, wParam, LParam) ;
      break;
    case WM_LBUTTONUP:
      LeftButtonUp(hWnd, wParam, LParam) ;
      break;
    case WM_RBUTTONDOWN:
      RightButtonDown(hWnd, wParam, LParam) ;
      break;
    case WM_RBUTTONUP:
      RightButtonUp(hWnd, wParam, LParam) ;
    case WM_CHAR:
     // BTex.theBook->NormalChar(hWnd, wParam);
     if ( BTex.theBook != NULL )
     {      
       blkMsg.wParam = wParam;
       blkMsg.iMessage = iMessage;
       BTblock *theView = FindView(hWnd);
       theView->transInRoute(&blkMsg);
       InvalidateRect(hWnd,NULL,TRUE);
     }
     break ;

    case WM_KEYDOWN:
     // BTex.theBook->FunctionChar(hWnd, wParam);
     break ;

    case WM_COMMAND:
      switch(wParam) 
      {
	    case CM_QUIT:
	      WinHelp(hWnd, "vdna.hlp", HELP_QUIT,NULL);
	      DestroyWindow(hWnd);
	    break; 
      }
      break;

    case WM_DESTROY:
      PostQuitMessage(0);
      break;
    
    case WM_SIZE:
      if ( BTex.theBook != NULL )
      {
        BTviewBlock *theView = (BTviewBlock *) FindView(hWnd);
        BTWindow *windPtr;
        windPtr = theView->WindowPtr();
        if ( windPtr != NULL )
        {
          RECT rect;
          GetClientRect( windPtr->HWindow,&rect);
          windPtr->Height = rect.bottom ;
        }
      }
      break;
    case WM_DDE_EXECUTE:
      if ( BTex.theBook != NULL )
      { 
        HANDLE hCommand;
        hCommand = HIWORD(LParam);
        if ( ! (lpCommand = GlobalLock(hCommand)))
        {
          PostMessage(wParam,WM_DDE_ACK,hWnd, MAKELONG(0,hCommand) ) ;
          break;
        }
        if ( (junk == 0 ) && ( lpCommand[29] != 32  ) )
           junk = 1;
        // char *pCommand;
        // pCommand = new char[_fstrlen(lpCommand) + 1 ];
        // _fstrcpy(pCommand,lpCommand);
        // BTblock *theView =  FindView(hWnd);
        ProcessDDEexecute(hWnd, wParam, FindView(hWnd) , lpCommand) ;  // was btex.thebook
        // delete [] pCommand;      
        GlobalUnlock(hCommand);
       
        PostMessage(wParam,WM_DDE_ACK,hWnd, MAKELONG(0x8000,hCommand) ) ;
      }
      break;
    case WM_DDE_INITIATE:
      if ( BTex.theBook != NULL )
      { 

        ATOM  aApplication,aTopic;
        lpCommand = 0;
        aApplication = GlobalAddAtom("vDNA");
        char szBuf[80];
        GlobalGetAtomName(HIWORD(LParam), szBuf, sizeof(szBuf));
        BTviewBlock *theView = (BTviewBlock *) NameView(szBuf);

        if (( LOWORD(LParam) == aApplication || !LOWORD(LParam)) &&
           ( theView != NULL ) )
//            (HIWORD(LParam)  == aTopic || !HIWORD(LParam)))
        {
          // MessageBox(hWnd, szBuf, "vDNA Server Connected To", MB_OK);
          aTopic       = GlobalAddAtom(szBuf);

          //BTviewBlock *theView = (BTviewBlock *) FindView(NULL);
          theView->graphWin->remoteHWindow = wParam ;
          if ( !PostMessage(wParam,WM_DDE_ACK,
                              theView->graphWin->HWindow,
                              MAKELONG(aApplication,aTopic) ) )
          {
              // GlobalDeleteAtom(aApplication);
              // GlobalDeleteAtom(aTopic);   
          }
        }
      }
      break;
    default:
      return DefWindowProc(hWnd, iMessage,wParam,LParam);
  }
  return (0L);
}