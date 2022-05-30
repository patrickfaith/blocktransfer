//
// BTweight ... gets list of higher weighted blocks
//
// note ! trans in the makeWeightList must be a Cface
//

/*#include <stdio.h>
#include <fstream.h>
#include <strstrea.h>
#include <iomanip.h>
*/
#include <string.h>
#include <btdefs.h>
#include <btlist.h>
#include <btblocks.h>
//#include <btcface.h>

void BTcFaceBlock::addWeightList(BTtrans *trans)
{
  BTlistIterator listHead( blockChildren );
  
  // overfilled so make room
  if ( transferRatio >= blockRatio )
  {
    listHead.Iterate(); // current is on first block now
    blockChildren = listHead.removeCurrent();
    listHead.Reset();
  }
  // insert blocks in weight ordered childrens list
  BTblock *blockPtr;
  while ( (blockPtr = listHead.Iterate()) != NULL )
    if ( btEnergy <= blockPtr->btEnergy )     
      break;

  blockChildren = listHead.addCurrent(trans);

  // figure lowest weight
  if ( transferRatio >= blockRatio )
  {
    listHead.Reset();
    blockPtr = listHead.Iterate();
    if (blockPtr != NULL ) btEnergy = blockPtr->btEnergy;
  }
  else  // increment number of things in list  
    transferRatio += 1;

  return;
}

void BTblock::makeWeightList(BTtrans *trans)
{
  BTcFaceBlock *realTrans ;

  if ( this == trans ) return ; // error state
  if ( ( btEnergy > trans->btEnergy ) &&
       ( Type() == BT_TYPE_CHUNK) )
  {
    realTrans = (BTcFaceBlock *) trans ;
    realTrans->addWeightList(this);
  }
  return;
}


void BTpageBlock::makeWeightList(BTtrans *trans)
{
  BTblock *blockPtr;

  if ( blockRatio < .1 ) return ;

  BTlistIterator childHead( blockChildren );
  while ( (blockPtr = childHead.Iterate()) != NULL )
    blockPtr->makeWeightList(trans);
  return;
}

void BTbookBlock::makeWeightList(BTtrans *trans)
{
  BTblock *blockPtr;

  BTlistIterator childHead( blockChildren );
  while ( (blockPtr = childHead.Iterate()) != NULL )
    blockPtr->makeWeightList(trans);
  return;
}
