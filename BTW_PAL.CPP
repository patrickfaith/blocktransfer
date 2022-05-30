#include <btdefs.h>
#include <btlist.h>
#include <btblocks.h>
#include <btgraph.h>
#include <btacmdef.h>
#include <btacmstr.h>
#include <btproto.h>

HANDLE SetupPallete() // HDC hDC)
{
/*  #define NUMENTRIES 128
  HPALETTE hpal;
  PALETTEENTRY ape[NUMENTRIES];

  plgpl = (LOGPALETTE*) LocalAlloc(LPTR,
        sizeof(LOGPALETTE) + cColors * sizeof(PALETTEENTRY));

  plgpl->palNumEntries = cColors;
  plgpl->palVersion = 0x300;

  for (i = 0, red = 0, green = 127, blue = 127; i < NUMENTRIES;
        i++, red += 1, green += 1, blue += 1)
  {
    ape[i].peRed =
        plgpl->palPalEntry[i].peRed = LOBYTE(red);

    ape[i].peGreen =
        plgpl->palPalEntry[i].peGreen = LOBYTE(green);
    ape[i].peBlue =
        plgpl->palPalEntry[i].peBlue = LOBYTE(blue);
    ape[i].peFlags =
        plgpl->palPalEntry[i].peFlags = PC_RESERVED;
  }
  hpal = CreatePalette(plgpl);
  LocalFree((HLOCAL) plgpl);
  return hPal;
*/
  HANDLE hPal = NULL;
  int numColors = 64 ; // +4*16 ;

  LOGPALETTE *logPal;

  logPal = (LPLOGPALETTE)(new char[sizeof(LOGPALETTE)+
				   (sizeof(PALETTEENTRY)*numColors)]);
  logPal->palVersion = 0x300;
  logPal->palNumEntries = numColors;

  int i;
  for (i=0; i<64; i++)
  {
    logPal->palPalEntry[i].peRed   = 0 ; // i*2;
    logPal->palPalEntry[i].peGreen = 0 ; //i*2;
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
  int shit;
  if ( theBook->hPal == NULL )
  {
    theBook->hPal = SetupPallete() ;
  }  
  oldhPal = SelectPalette(hDC,theBook->hPal,0);
  if (RealizePalette(hDC) == NULL)
  {
    shit = 1;
  }
   // MessageBox(hwnd, "Can't realize palette", "Error", MB_OK)
  // RealizePalette(hDC);

  return oldhPal;
}

