// copyright (c) p l faith
//
// frontal lobe routines
//
#include <stdio.h>
#include <string.h>
#include <btdefs.h>
#include <btlist.h>
#include <btblocks.h>


// need transfer routines to cface
//   from all block types (including page )
//   list merge routines
//   list zap dups - maybe
//   list copy routines
// need block definition and basic
//  stuff for the BTcfaceBlock
// 
//
//
// cface block points (x)face block through special pointer
//
void BTchunkBlock::transferBlockToCface(BTtrans *trans)
{

  BTcFaceBlock *lastHoldPtr,*holdPtr;
  BTblock      *blockPtr;
  lastHoldPtr = NULL ;
  BTlistIterator childList( blockChildren );
  while ( (blockPtr = childList.Iterate()) != NULL )
  {
     holdPtr = new BTcFaceBlock(trans);

     holdPtr->internalPtr = blockPtr;
     holdPtr->BlockType = blockPtr->blockType();
     holdPtr->TransferType = blockPtr->transferType();
     holdPtr->btEnergy = btEnergy * 2; // 2 is very small
//     transedBlockPtr->addChild(holdPtr);
//     holdPtr->addParent( transedBlockPtr );
     if ( lastHoldPtr != NULL )
     {
       lastHoldPtr->addOutput(holdPtr);
       holdPtr->addInput(lastHoldPtr);
     }
     lastHoldPtr = holdPtr;
  }
}

void BTblock::transferBlockToCface(BTtrans *trans)
{
  /*

  BTcFaceBlock *transedBlockPtr;
  transedBlockPtr = new BTcFaceBlock(trans);
  transedBlockPtr->internalPtr = this;
  transedBlockPtr->BlockType = blockType();
  transedBlockPtr->TransferType = transferType();

  BTcFaceBlock *lastHoldPtr,*holdPtr;
  BTblock      *blockPtr;
  lastHoldPtr = NULL ;
  BTlistIterator childList( blockChildren );
  while ( (blockPtr = childList.Iterate()) != NULL )
  {
     holdPtr = new BTcFaceBlock(trans);
     holdPtr->internalPtr = blockPtr;
     holdPtr->BlockType = blockPtr->blockType();
     holdPtr->TransferType = blockPtr->transferType();
     transedBlockPtr->addChild(holdPtr);
     holdPtr->addParent( transedBlockPtr );
     if ( lastHoldPtr != NULL )
     {
       lastHoldPtr->addOutput(holdPtr);
       holdPtr->addInput(lastHoldPtr);
     }
     lastHoldPtr = holdPtr;
  }
  */
}

//  trans is something like a page of cfaces
//  add any transfers to it if they are not there already

void BTblock::transferTransferGetToCface(BTtrans *trans)
{
  BTblock *blockPtr; // block pointer internal transfer
  BTlistIterator transList( transferOutput );
  while ( (blockPtr = transList.Iterate()) != NULL )
  {
    short found;

    BTcFaceBlock *likeBlockPtr;  // cface block ptr - in page 

    BTlistIterator cFaceList(trans->blockChildren);
    found = 0;
    while ( (likeBlockPtr = (BTcFaceBlock *) cFaceList.Iterate()) != NULL )
    {
      if (likeBlockPtr->internalPtr == blockPtr )  // wrong
      {
	found = 1;
	break;
      }
    }

    if ( found == 0 ) // make a terminal block that points at transfer
    {
      BTcFaceBlock *holdPtr;
      holdPtr = new BTcFaceBlock(trans);
      holdPtr->internalPtr = blockPtr;
      holdPtr->BlockType = 0 ;
      holdPtr->TransferType = 0;
    }
  }
  return ;
}

// connect all cface transfers - trans doesn't do much except search

void BTblock::transferTransferConnectToCface(BTtrans *trans)
{
  BTblock *blockPtr;
  BTlistIterator transList( transferOutput );
  while ( (blockPtr = transList.Iterate()) != NULL )
  {
    BTcFaceBlock *likeBlockPtr;
    BTlistIterator cFaceList(trans->blockChildren);

    while ( (likeBlockPtr = (BTcFaceBlock *) cFaceList.Iterate()) != NULL )
    {
      if (likeBlockPtr->internalPtr == blockPtr )
      {
	addOutput( likeBlockPtr) ;
	break;
      }
    }
  }
  return ;
}

// associate like children into a grouping element

void BTpageBlock::associateLikeChildren(BTtrans *trans )
{
  BTcFaceBlock *blockPtr1;
  BTcFaceBlock *blockPtr2;
  BTcFaceBlock *groupPtr;

  BTlist *childList1;
  BTlist *childList2;
  BTlist *childListTemp;
  BTlist *childHead;

  if ( blockChildren == NULL ) return ;

  // create a list that we can manipulate
  childHead = new BTlist(blockChildren->ptr);

  childListTemp = childHead;
  childList1 = blockChildren->next;

  while (childList1 != NULL )
  {
    childListTemp->next = new BTlist( childList1->ptr );
    childListTemp = childListTemp->next;

    childList1 = childList1->next;
  }

  childList1 = childHead;

  while ( childList1 != NULL )
  {
    blockPtr1 = (BTcFaceBlock *) childList1->ptr;  
    if (blockPtr1 == NULL)
    {
      childList1 = childList1->next;
      continue;
    }
    groupPtr = NULL;
    childList2 = childList1->next;  

    while ( childList2 != NULL )
    {
      blockPtr2 = (BTcFaceBlock *) childList2->ptr;
      if (blockPtr2 == NULL)
      {
	childList2 = childList2->next;
	continue;
      }
     
      if ( ( blockPtr1 != blockPtr2 ) &&          // pointing at same thing
	   ( blockPtr1->internalPtr == blockPtr2->internalPtr ) )
      {
	if (groupPtr == NULL)
	{
	  groupPtr = new BTcFaceBlock(this);
	  groupPtr->TransferType = 0;
	  groupPtr->BlockType = 1;
	}
	BTlistIterator groupList( groupPtr->blockChildren );

	if ( groupList.FindAtom(blockPtr1) == NULL )
	{
	  groupPtr->addChild(blockPtr1);
	  blockPtr1->addParent(groupPtr);
	}
	if ( groupList.FindAtom(blockPtr2) == NULL )
	{
	  groupPtr->addChild(blockPtr2);
	  blockPtr2->addParent(groupPtr);
	}

	childList2->ptr = NULL ;
      }
      childList2 = childList2->next;
    }
    childList1 = childList1->next;
  }

  childList1 = childHead;

  while (childList1 != NULL )
  { 
    childListTemp = childList1;
    childList1 = childList1->next;
    delete childListTemp;
  }

  return ;
}

/*
void BTpageBlock::associateLikeChildren(BTtrans *trans )
{
  BTcFaceBlock *blockPtr1,*blockPtr2,*groupPtr;

  BTlistIterator childList1( blockChildren );
  while ( (blockPtr1 = (BTcFaceBlock *) childList1.Iterate()) != NULL )
  {
    groupPtr = NULL ;
    BTlistIterator childList2( blockChildren );

    while ( (blockPtr2 = (BTcFaceBlock *) childList2.Iterate()) != NULL )
    {
      if ( ( blockPtr1 != blockPtr2 ) &&          // pointing at same thing
	   ( blockPtr1->internalPtr == blockPtr2->internalPtr ) )
      {
	if (groupPtr == NULL)
	{
	  groupPtr = new BTcFaceBlock(this);
	  groupPtr->TransferType = 0;
	  groupPtr->BlockType = 1;
	}
	BTlistIterator groupList( groupPtr->blockChildren );

	if ( groupList.FindAtom(blockPtr1) == NULL )
	  groupPtr->addChild(blockPtr1);
	if ( groupList.FindAtom(blockPtr2) == NULL )
	  groupPtr->addChild(blockPtr2);
      }
    }
  }
  return ;
}
*/

// load weights-score of internal blocks

void BTcFaceBlock::loadInternalWeight(BTtrans *trans)
{
  BTcFaceBlock *blockPtr;

  if ( internalPtr != NULL )
  {
    if ( internalPtr->Type() == BT_TYPE_CHUNK )
    {
      btEnergy = internalPtr->btEnergy;
      blockRatio = FIX_ONE;
      transferRatio = FIX_ONE ;
    }
    else
    {
      btEnergy += internalPtr->btEnergy;
      blockRatio = internalPtr->blockRatio;
      transferRatio = internalPtr->transferRatio;
    }
  }
  else
  {
    btEnergy = 0;
    blockRatio = FIX_ONE;
    transferRatio = FIX_ONE ;
  }
  BTlistIterator childList( blockChildren );
  while ( (blockPtr = (BTcFaceBlock *) childList.Iterate()) != NULL )
  {
    blockPtr->loadInternalWeight(trans); 
  }
  return ;
}

// given a input list on trans (children) build a cface
// this is the high level transfer to cface routine
void BTpageBlock::transferToCface(BTtrans *trans )
{
  BTblock *blockPtr;

  // currentBlock = NULL;
  BTlistIterator childList( trans->blockChildren );
  while ( (blockPtr = childList.Iterate()) != NULL )
  {
    blockPtr->transferBlockToCface(this);
  }

  //  now that the basic blocks are in place, figure transfer
  //  connections between them

  // first create transfer blocks into cface that are do not already
  // exist

 // BTlistIterator thisChildList( blockChildren );
 // while ( (blockPtr = thisChildList.Iterate()) != NULL )
 // {
 //   blockPtr->transferTransferGetToCface(this);
 // }

  // now connect all the block transfers

 // BTlistIterator anotherChildList( blockChildren );
 // while ( (blockPtr = anotherChildList.Iterate()) != NULL )
 // {
 //   blockPtr->transferTransferConnectToCface(this);
 // }

  // load in the attribute weights of all the blocks internals.
  // loadInternalWeight(this);

  BTcFaceBlock *cBlockPtr;

  BTlistIterator lastChildList( blockChildren );
  while ( (cBlockPtr = (BTcFaceBlock *) lastChildList.Iterate()) != NULL )
  {
    cBlockPtr->loadInternalWeight(trans); 
  }

  associateLikeChildren(this);
}

char *BTcFaceBlock::Name()
{
  if (internalPtr == NULL ) return NULL ;

  return internalPtr->Name();
}

BTcFaceBlock::BTcFaceBlock(BTtrans *trans)
{
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
  blockParents  = NULL;
  blockChildren = NULL;
  transferInput = NULL;
  transferOutput= NULL;
  btEnergy = 0 ;
  blockRatio = 0 ;
  transferRatio = 0;
  internalPtr = NULL;BlockType=0;TransferType=0 ;
  space = NULL;

}

  // : BTblock(trans)
 // { ;} ;



