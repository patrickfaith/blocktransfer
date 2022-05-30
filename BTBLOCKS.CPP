
#include <btdefs.h>
#include <btlist.h>
#include <btblocks.h>

/// constructors for base class BTblock
BTblock::BTblock()
{
  deActivate = NULL ;
  pagePtr = NULL;
  blockId = 0;
  blockParents 	= NULL;
  blockChildren	= NULL;
  transferInput	= NULL;
  transferOutput	= NULL;
  btEnergy = 0 ;
  blockRatio = FIX_ONE;
  transferRatio = FIX_ONE;
  graph = NULL;
}
short BTblock::NextBlockId(BTtrans *trans)
{
  if ( trans->pagePtr != NULL )
    return pagePtr->NextBlockId(this);
  else
    return 1;
}
char *BTblock::pageName()
{
  if ( pagePtr != NULL )
    return pagePtr->Name();
  else
    return NULL;
}

BTbookBlock* BTblock::BookPtr()
{
  if ( pagePtr == NULL ) return NULL;
  return pagePtr->BookPtr();
}

short BTblock::Loaded()
{
  if ( pagePtr != NULL )
    return pagePtr->Loaded();
  else
    return 0;
}
BTblock::BTblock(BTtrans *trans)
{
  deActivate = NULL ;
  blockId = 1;
  pagePtr = NULL;
  if ( trans != NULL )
  {
    pagePtr = trans->pagePtr;
    if ( pagePtr != NULL )
    {
      blockId = NextBlockId(trans);
      pagePtr->addChild(this);
    }
  }
  blockParents 	= NULL;
  blockChildren	= NULL;
  transferInput	= NULL;
  transferOutput= NULL;
  btEnergy = 0 ;
  if ( pagePtr != NULL )
  {
    blockRatio = pagePtr->blockRatio;
    transferRatio = pagePtr->transferRatio;
  }
  else
  {
    blockRatio = FIX_ONE;
    transferRatio = FIX_ONE;
  }
  graph = NULL;
}

BTblock::BTblock(BTtrans *trans, short blkId)
{
  blockId = blkId;
  pagePtr = NULL;
  deActivate = NULL ;
  if ( trans != NULL )
  {
    pagePtr = trans->pagePtr;
    if ( pagePtr != NULL )
    {
      pagePtr->SetNextBlockId(blockId + 1);
      pagePtr->addChild(this);
    }
  }
  graph = NULL;
  blockParents 	= NULL;
  blockChildren	= NULL;
  transferInput	= NULL;
  transferOutput= NULL;
  blockRatio = 0.5 ;
  transferRatio = 0.1;
}

BTblock::~BTblock()
{
  BTlistIterator  zapParents( blockParents );
  zapParents.erase();

  BTlistIterator  zapChildren( blockChildren );
  zapChildren.erase();

  BTlistIterator  zapInputs( transferInput );
  zapInputs.erase();

  BTlistIterator  zapOutputs( transferOutput );
  zapOutputs.erase();

  if ( Type() != BT_TYPE_BOOK )
    if ( pagePtr != NULL )
      pagePtr->removeChild(this);
}

void BTblock::writeString(BTtrans *trans)
{
   return;
}


/// functions to add blocks to the pointers

void BTblock::setPtrId( BTlist *listItem)
{
  listItem->blockId = blockId;
  if (pagePtr != NULL) listItem->pageId = pagePtr->blockId;
}

void BTblock::addParent(BTblock *blkPtr)
{
   blockParents = new BTlist(blkPtr,blockParents);
   if (blkPtr != NULL) blkPtr->setPtrId(blockParents);
   return;
}
void BTblock::addChild(BTblock *blkPtr)
{
   blockChildren = new BTlist(blkPtr,blockChildren);
   if (blkPtr != NULL) blkPtr->setPtrId(blockChildren);
   return;
}
void BTblock::addInput(BTblock *blkPtr)
{
   transferInput = new BTlist(blkPtr,transferInput);
   if (blkPtr != NULL) blkPtr->setPtrId(transferInput);
   return;
}
void BTblock::addOutput(BTblock *blkPtr)
{
   transferOutput = new BTlist(blkPtr,transferOutput);
   if (blkPtr != NULL) blkPtr->setPtrId(transferOutput);
   return;
}

void BTblock::removeChild(BTblock *blkPtr)
{
  BTblock			*blockPtr;

  BTlistIterator 	listHead( blockChildren );

  while ( (blockPtr = listHead.Iterate()) != NULL )
  {
    if ( blockPtr == blkPtr )
    {
      blockChildren = listHead.removeCurrent();
      return ;
    }  
  }
  return ;
}

void BTblock::removeParent(BTblock *blkPtr)
{
  BTblock			*blockPtr;

  BTlistIterator 	listHead( blockParents);

  while ( (blockPtr = listHead.Iterate()) != NULL )
  {
    if ( blockPtr == blkPtr )
    {
      blockParents = listHead.removeCurrent();
      return ;
    }  
  }
  return ;
}
void BTblock::removeInput(BTblock *blkPtr)
{
  BTblock			*blockPtr;

  BTlistIterator 	listHead( transferInput );

  while ( (blockPtr = listHead.Iterate()) != NULL )
  {
    if ( blockPtr == blkPtr )
    {
      transferInput = listHead.removeCurrent();
      return ;
    }  
  }
  return ;
}
void BTblock::removeOutput(BTblock *blkPtr)
{
  BTblock			*blockPtr;

  BTlistIterator 	listHead( transferOutput );

  while ( (blockPtr = listHead.Iterate()) != NULL )
  {
    if ( blockPtr == blkPtr )
    {
      transferOutput = listHead.removeCurrent();
      return ;
    }  
  }
  return ;
}
void BTstringBlock::writeString(BTtrans *trans)
{
   trans->OutString(string); trans->OutString(" ");
   return;
}

/// specific routines to the "groupBlock"

void BTchunkBlock::addChild(BTblock *blkPtr)
{
  short pageId = 0;
  BTlistIterator listHead( blockChildren );

  if ( pagePtr != NULL ) pageId =  pagePtr->blockId;

  blockChildren = listHead.addEnd( blkPtr,blkPtr->blockId, pageId );
  return;
}

void BTcFaceBlock::addChild(BTblock *blkPtr)
{
  short pageId = 0;
  BTlistIterator listHead( blockChildren );

  if ( pagePtr != NULL ) pageId =  pagePtr->blockId;

  blockChildren = listHead.addEnd( blkPtr,blkPtr->blockId, pageId );
  return;
}

void BTgroupBlock::writeString(BTtrans *trans)
{
  BTblock			*blockPtr;

  BTlistIterator 	sentHead( blockChildren );

  while ( (blockPtr = sentHead.Iterate()) != NULL )
  {
    blockPtr->writeString(trans);
  }

  trans->OutString("\n");
  return;
}

