#include <string.h>
#include <btdefs.h>
#include <btlist.h>
#include <btwndef.h>
#include <btblocks.h>
#include <btgraph.h>
#include <btacmdef.h>
#include <btacmstr.h>
#include <btproto.h>

void LeftButtonDown(HWND hWnd, WPARAM wParam, LPARAM LParam)
{
  if ( BTex.theBook != NULL )
  { 
    BTblock *theView = FindView(hWnd);
    if ( theView != NULL )
    {
      pixStruc pix; pix.x = LOWORD( LParam ); pix.y = HIWORD( LParam );
      pix.closest = 10000; pix.bestBlock = NULL; pix.closestZ = -32000;
      
      
      BTex.theBook->sysRegs[REG_SYS_SELBLOCK] = theView->PixelToBlock(&pix);
      BTex.theBook->sysRegs[REG_SYS_L_BUTTON] =
             BTex.theBook->sysRegs[REG_SYS_SELBLOCK]; 
      BTex.theBook->sysRegs[REG_SYS_DRAGBLOCK] =        
             BTex.theBook->sysRegs[REG_SYS_SELBLOCK];
             
      BTex.theBook->sysRegs[REG_SYS_DRAGVIEW] =  theView;       
      BTex.theBook->sysRegs[REG_SYS_SELVIEW] =  theView;       
       
      ProcessDNACommandBuffer(BTex.theBook, "@bot [bsl]");
    }
  }
}

void LeftButtonUp( HWND hWnd, WPARAM wParam, LPARAM LParam)
{
  if ( BTex.theBook != NULL )
  { 
    BTex.theBook->sysRegs[REG_SYS_L_BUTTON] = NULL ;
    BTblock *theView = FindView(hWnd);
    if ( theView != NULL )
    {
      pixStruc pix; pix.x = LOWORD( LParam ); pix.y = HIWORD( LParam );
      pix.closest = 10000; pix.bestBlock = NULL; pix.closestZ = -32000;

      BTblock *block= theView->PixelToBlock(&pix);
      short doDrop = 0;
      if ( block != BTex.theBook->sysRegs[REG_SYS_SELBLOCK] )
        doDrop = 1;

      BTex.theBook->sysRegs[REG_SYS_SELBLOCK] = block ;
      BTex.theBook->sysRegs[REG_SYS_DROPBLOCK] = block;
             
      BTex.theBook->sysRegs[REG_SYS_DROPVIEW] =  theView;       
      BTex.theBook->sysRegs[REG_SYS_SELVIEW] =  theView;       
      
      if ( doDrop == 1)
      {
        ProcessDNACommandBuffer(BTex.theBook, "@bot [drg]");
        ProcessDNACommandBuffer(BTex.theBook, "@bot [drp]");
      }
    }
  }
}

void RightButtonUp( HWND hWnd, WPARAM wParam, LPARAM LParam)
{
  if ( BTex.theBook != NULL )
  { 
    BTex.theBook->sysRegs[REG_SYS_R_BUTTON] = NULL ;
    BTblock *theView = FindView(hWnd);
    if ( theView != NULL )
    {
      pixStruc pix; pix.x = LOWORD( LParam ); pix.y = HIWORD( LParam );
      pix.closest = 10000; pix.bestBlock = NULL; pix.closestZ = -32000;

      BTblock *block= theView->PixelToBlock(&pix);
      short doDrop = 0;
      if ( block != BTex.theBook->sysRegs[REG_SYS_DRAGBLOCK] )
        doDrop = 1;

      BTex.theBook->sysRegs[REG_SYS_SELBLOCK] = block ;
      BTex.theBook->sysRegs[REG_SYS_DROPBLOCK] = block;
             
      BTex.theBook->sysRegs[REG_SYS_DROPVIEW] =  theView;       
      BTex.theBook->sysRegs[REG_SYS_SELVIEW] =  theView;       
      
      if ( doDrop == 1)
      {
        ProcessDNACommandBuffer(BTex.theBook, "@bot [drg]");
        ProcessDNACommandBuffer(BTex.theBook, "@bot [drp]");
      }
    }
  }
}

void RightButtonDown( HWND hWnd, WPARAM wParam, LPARAM LParam)
{
  if ( BTex.theBook != NULL )
  { 
    BTblock *theView = FindView(hWnd);
    if ( theView != NULL )
    {
      pixStruc pix; pix.x = LOWORD( LParam ); pix.y = HIWORD( LParam );
      pix.closest = 10000; pix.bestBlock = NULL; pix.closestZ = -32000;

      BTex.theBook->sysRegs[REG_SYS_R_BUTTON] = theView->PixelToBlock(&pix);
      BTex.theBook->sysRegs[REG_SYS_SELVIEW] =  theView;
    }
  }
}

