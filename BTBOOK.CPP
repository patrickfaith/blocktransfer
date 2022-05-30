// copyright (c) 1993 P L Faith - all rights reserved
// btbook.c - high level interface routines
//
#include <stdio.h>
#include <string.h>
#include <btdefs.h>
#include <btlist.h>
#include <btblocks.h>
#include <btacmdef.h>
#include <btacmstr.h>

BTbookBlock::BTbookBlock(char *bookName)
{
  pagePtr = NULL ; blockId = 0; graph = NULL;
  blockParents = NULL ; blockChildren = NULL;
  transferInput = NULL; transferOutput = NULL;
  blockViews = NULL ;
  currentBlock = NULL;
  btEnergy = 0; blockRatio = FIX_ONE; transferRatio= FIX_ONE;
  startPage = NULL;
  memBit = NULL;
  hPal = NULL ;
  
  int i;
  for (i=0;i < MAX_NUM_SYS_REGS ; i++)
    sysRegs[i] = NULL;

  trigButFocus   = NULL;
  trigButExecute = NULL;

  accum = new BTaccum(this,this);
  if (bookName != NULL)
  {
    name = new char[strlen(bookName)+1];
    _fstrcpy(name,bookName);
  }
  // need to load in book now
}

BTbookBlock::~BTbookBlock()  // delete all pages
{
  if ( memBit != NULL )
    DeleteObject(memBit);
  if (hPal != NULL )
    DeleteObject(hPal);
  BTlistIterator blockList( blockChildren );
  while ( blockList.IterateNull() != NULL )
    delete blockList.CurrentPtr() ;
}

// take a string and place it into the book
void BTbookBlock::WriteInBook(char *pageName, char *strng)
{
 // removed b4
}
void BTbookBlock::addChild(BTblock *blkPtr)
{
  short pageId = 0;
  BTlistIterator listHead( blockChildren );

  if ( pagePtr != NULL ) pageId =  pagePtr->blockId;

  blockChildren = listHead.addEnd( blkPtr,blkPtr->blockId, pageId );
  return;
}

char *BTbookBlock::ReadFromBook()
{
  // removed b4
  return NULL;
}

