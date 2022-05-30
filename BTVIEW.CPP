//
// copyright (c) p l faith 1993 all rights reserved
//
#include <string.h>
#include <btdefs.h>
#include <btlist.h>
#include <btblocks.h>
#include <btproto.h>
#include <btacmdef.h>
#include <btacmstr.h>

/*BTviewBlock::BTviewBlock() // : BTpageBlock()
  {
  trigButExecute = NULL;
  buffer = NULL ;
}
*/


BTviewBlock::BTviewBlock( BTtrans *trans, char *strng) 
{ 
  trigInput = NULL ;
  trigKeyReturn  = NULL;
  trigComReturn  = NULL;
  trigButFocus   = NULL;
  trigButExecute = NULL;

  blockId = 0;
  nextBlockId = blockId + 1;
  deActivate = NULL ;
  channelType = 0;
  facade = NULL ;

  bookPtr = trans->BookPtr();

  if ( bookPtr != NULL )
  {
    BTlistIterator pageHead( bookPtr->blockChildren );
    BTblock *blockPtr;
    while ( (blockPtr = pageHead.Iterate()) != NULL )
    {
      if ( blockId <= blockPtr->blockId )
	blockId = blockPtr->blockId + 1;
    }
  }

  loaded = 1;

  pagePtr = this;
  name = new char[_fstrlen(strng)+1] ;

  _fstrcpy(name,strng);

  blockParents 	= NULL;
  blockChildren	= NULL;
  transferInput	= NULL;
  transferOutput= NULL;

  place = NULL ;

  if ( trans != NULL )
  {
    // don't tie cface to children lists
    if ( (strncmpi( strng,"cLFace", 6) != 0 ) &&
	 (strncmpi( strng,"cRFace", 6) != 0 ) )
    {
      bookPtr = trans->BookPtr();
      if ( bookPtr != NULL )
        bookPtr->addChild(this);
    }
  }

  // need to define things for the transform

  space = new BTspace();

  blockRatio = 0;
  transferRatio = 0;
  
  graphWin = new BTWindow() ;
  buffer = NULL ; // new BTbuff() ;
  accum = new BTaccum(bookPtr,this);
}


BTviewBlock::~BTviewBlock()
{
  // remove all children
  // BTblock	*blockPtr ;
  blockChildren = NULL; // crap thing ... but ??????
  // BTlistIterator pageHead( blockChildren );

  // while ( (blockPtr = pageHead.Iterate()) != NULL )
  //{ 
  //  if (blockPtr
  //  delete blockPtr;
  //}

  // if ( bookPtr != NULL )
  //   bookPtr->removeChild(this) ;
  
  if ( graphWin != NULL )
    delete   graphWin ;
  if ( buffer != NULL )
    delete   buffer;
  //if ( space != NULL )
  //  delete space;
  buffer = NULL;
  //space = NULL;
  graphWin = NULL;
}

void OpenViewWindow(BTviewBlock *viewBlk)
{
  viewBlk->graphWin->HWindow = CreateWindow (
           BTex.AppName,        // class name
           viewBlk->name,       // window name
           WS_OVERLAPPEDWINDOW, // window style
           viewBlk->graphWin->X,
           viewBlk->graphWin->Y,
           viewBlk->graphWin->Width,
           viewBlk->graphWin->Height,
           NULL,
           NULL,
           BTex.hInstance,
           NULL);
  ShowWindow(viewBlk->graphWin->HWindow,
   SW_SHOW);
  UpdateWindow( viewBlk->graphWin->HWindow );
  
  RECT rect;
  GetClientRect( viewBlk->graphWin->HWindow,&rect);
  viewBlk->graphWin->Height = rect.bottom ;
}
// open/create a window for the view
void BTviewBlock::openPort()
{
  OpenViewWindow(this);
  return;
} 

void BTbookBlock::openPort()
{
  BTblock *blockPtr;  
  BTlistIterator pageHead( blockChildren );
  while ( (blockPtr = pageHead.Iterate()) != NULL )
  {
    blockPtr->openPort();
  }
  
  BTlistIterator anotherPageHead( blockChildren );
  while ( (blockPtr = anotherPageHead.Iterate()) != NULL )
  {
    blockPtr->inPort(this);
  }  
  return;
} 

// closes graphics windows in this case ... hmm need to add something here
void BTviewBlock::closePort()
{return;} 


BTviewBlock *FindView(HWND hWnd)
{
  BTblock *pagePtr;
  BTlistIterator pageList( BTex.theBook->blockChildren );
  while ( (pagePtr = pageList.Iterate()) != NULL )
  {
    if (pagePtr->Type() == BT_TYPE_VIEW)
    {
      BTviewBlock  * theView = (BTviewBlock *) pagePtr;

      if ( theView->graphWin != NULL )
	    if ( theView->graphWin->HWindow == hWnd )
          return theView;
        else if (hWnd == NULL )
          return theView;
    }
  }
  return NULL ;
}

BTviewBlock *NameView(char *name)
{
  BTblock *pagePtr;
  BTlistIterator pageList( BTex.theBook->blockChildren );
  while ( (pagePtr = pageList.Iterate()) != NULL )
    if (pagePtr->Type() == BT_TYPE_VIEW) 
      if ( pagePtr->Name() != NULL )
        if (strcmp(pagePtr->Name(),name) == 0 )
        { 
          BTviewBlock  * theView = (BTviewBlock *) pagePtr;
          return theView;
        }
  return NULL ;
}


//  virtual void transOutRoute(BTblock *trans) ;
