#include <btdefs.h>
#include <btlist.h>
#include <btblocks.h>
#include <btgraph.h>
#include <btproto.h>
BTWindow::BTWindow()
{
  HWindow = NULL;
  hDC = NULL;
  X = 0;
  Y = 0;
  Width = 0;
  Height = 0;
  scaler = 32 ; // was 2048;
  pointerToggle = 0;

  cursor0 = NULL;
  cursor1 = NULL;
  cursor2 = NULL;

  view = NULL;
}
