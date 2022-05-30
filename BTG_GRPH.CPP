#include <btdefs.h>
#include <btlist.h>
#include <btblocks.h>
#include <btgraph.h>
#include <btproto.h>

BTgraph::BTgraph()
{
  next = NULL ;
  ZeroVector(bookPos1);
  ZeroVector(bookPos2);
  bookPix1[0] = -1; bookPix1[1] = 0;
  bookPix2[0] = 0; bookPix2[1] = 0;
}
void BTgraph::PageToBook( BTspace *spacePtr )
{
  Transform(pagePos1,spacePtr->transform,bookPos1 );
  Transform(pagePos2,spacePtr->transform,bookPos2 );
}
void BTchipGraph::PageToBook( BTspace *spacePtr )
{
  Transform(pagePos1,spacePtr->transform,bookPos1 );
  Transform(pagePos2,spacePtr->transform,bookPos2 );
  Transform(pagePos3,spacePtr->transform,bookPos3 );
  Transform(pagePos4,spacePtr->transform,bookPos4 );
}
void BTgraph::BlocksToPixels(BTviewBlock *view)
{ 
  //TDA zox;
  VecToPixel(view->space->transform,
             bookPos1,zox,bookPix1,view->graphWin);
  VecToPixel(view->space->transform,
             bookPos2,zox,bookPix2,view->graphWin);
}
void BTchipGraph::BlocksToPixels(BTviewBlock *view)
{ 
  //TDA zox;
  VecToPixel(view->space->transform,
             bookPos1,zox,bookPix1,view->graphWin);
  VecToPixel(view->space->transform,
             bookPos2,zox,bookPix2,view->graphWin);
  VecToPixel(view->space->transform,
             bookPos3,zox,bookPix3,view->graphWin);
  VecToPixel(view->space->transform,
             bookPos4,zox,bookPix4,view->graphWin);
}
void BTgraph::SetPosInPage( TDA Pt1, TDA Pt2)
{
  pagePos1[0] = Pt1[0];
  pagePos1[1] = Pt1[1];
  pagePos1[2] = Pt1[2];

  pagePos2[0] = Pt2[0];
  pagePos2[1] = Pt2[1];
  pagePos2[2] = Pt2[2];
}


void BTchipGraph::SetPosInPage( int i, TDA& Pt)
{
  switch (i)
  {
    case 1:
    pagePos1[0] = Pt[0];
    pagePos1[1] = Pt[1];
    pagePos1[2] = Pt[2];
    break;
    case 2:
    pagePos2[0] = Pt[0];
    pagePos2[1] = Pt[1];
    pagePos2[2] = Pt[2];
    break;
    case 3:
    pagePos3[0] = Pt[0];
    pagePos3[1] = Pt[1];
    pagePos3[2] = Pt[2];
    break;
    case 4:
    pagePos4[0] = Pt[0];
    pagePos4[1] = Pt[1];
    pagePos4[2] = Pt[2];
    break;
  }
}

void BTchipGraph::GetPosInPage( int i, TDA& Pt)
{
  switch (i)
  {
    case 1:
      Pt[0] = pagePos1[0];
      Pt[1] = pagePos1[1];
      Pt[2] = pagePos1[2];
    break;
    case 2:
      Pt[0] = pagePos2[0];
      Pt[1] = pagePos2[1];
      Pt[2] = pagePos2[2];
    break;
    case 3:
      Pt[0] = pagePos3[0];
      Pt[1] = pagePos3[1];
      Pt[2] = pagePos3[2];
    break;
    case 4:
      Pt[0] = pagePos4[0];
      Pt[1] = pagePos4[1];
      Pt[2] = pagePos4[2];

    break;
  }
}
void BTgraph::SetPosInPage( int i, TDA& Pt)
{
  switch (i)
  {
    case 1:
    pagePos1[0] = Pt[0];
    pagePos1[1] = Pt[1];
    pagePos1[2] = Pt[2];
    break;
    case 2:
    pagePos2[0] = Pt[0];
    pagePos2[1] = Pt[1];
    pagePos2[2] = Pt[2];
    break;
  }
}

void BTgraph::GetPosInPage( int i, TDA& Pt)
{
  switch (i)
  {
    case 1:
      Pt[0] = pagePos1[0];
      Pt[1] = pagePos1[1];
      Pt[2] = pagePos1[2];
    break;
    case 2:
      Pt[0] = pagePos2[0];
      Pt[1] = pagePos2[1];
      Pt[2] = pagePos2[2];
    break;
  }
}
