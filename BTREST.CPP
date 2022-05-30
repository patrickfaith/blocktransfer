// used to rest-release energy from network

#include <string.h>
#include <btdefs.h>
#include <btlist.h>
#include <btblocks.h>
// #include <btcface.h>


void BTbookBlock::RelaxNetwork(BTval melow)
{
  BTblock *blockPtr;
  BTlistIterator pageList( blockChildren );
  while ( (blockPtr = pageList.Iterate()) != NULL )
    blockPtr->RelaxNetwork(melow);
}

void BTpageBlock::RelaxNetwork(BTval melow)
{
  BTblock *blockPtr;
  BTlistIterator pageList( blockChildren );
  while ( (blockPtr = pageList.Iterate()) != NULL )
    blockPtr->RelaxNetwork(melow);
}

void BTblock::RelaxNetwork(BTval melow)
{
  if ( melow > 0.0 )
    btEnergy /= melow;
}



void BTbookBlock::FocusNetwork(BTval energy)
{
  BTblock *blockPtr;
  BTlistIterator pageList( blockChildren );
  while ( (blockPtr = pageList.Iterate()) != NULL )
    blockPtr->FocusNetwork(energy);
}
// eventually would really like to factor
// in the overall focus of the page/and its
// energy.
// focus network on the current block
void BTpageBlock::FocusNetwork(BTval energy)
{
  if ( currentBlock != NULL )
    currentBlock->FocusNetwork(energy);
}

// for now just set the block to some value
void BTblock::FocusNetwork(BTval energy)
{
  btEnergy += energy;
}

//
// de-emphasize the current areas
//
void BTbookBlock::DeFocusNetwork(BTval energy)
{
  BTblock *blockPtr;

  if ( currentBlock != NULL )
    currentBlock->DeFocusNetwork(energy);

  BTlistIterator pageList( blockChildren );
  while ( (blockPtr = pageList.Iterate()) != NULL )
    blockPtr->DeFocusNetwork(energy);
}

void BTpageBlock::DeFocusNetwork(BTval energy)
{
  if ( currentBlock != NULL )
    currentBlock->DeFocusNetwork(energy);
}

void BTblock::DeFocusNetwork(BTval energy)
{
  BTblock *blockPtr;

  btEnergy = btEnergy/energy;

  BTlistIterator pageList( blockChildren );
  while ( (blockPtr = pageList.Iterate()) != NULL )
    blockPtr->DeFocusNetwork(energy);
}

void BTblock::NormalizeEnergy()
{
  if ( btEnergy < 0 ) btEnergy = 0 ;
  return ;
  /*
  if ( fabs( btEnergy ) < 5 )
  {
    btEnergy = 5;
    blockRatio *= 2.0;
    if (blockRatio > 100) blockRatio = 100;
    transferRatio *= 1.1;
    if ( transferRatio > 100.0 ) transferRatio = 100.0;
  }
  else if ( btEnergy > 1000000.0 )
  {
    blockRatio *= .5 ;
    if (blockRatio < .0001) blockRatio = .0001;
    transferRatio *= .9;
    if (transferRatio < .0001) transferRatio = .0001;
    btEnergy = 500000.0;
  }
  else if ( btEnergy < -1000000.0 )
  {
    blockRatio *= .5 ;
    btEnergy = -500000.0;
  }
  */
}


void BTblock::NormalizeNetwork()
{

  if  ( (Type() == BT_TYPE_PAGE)||(Type() == BT_TYPE_BOOK) )
  {
    BTblock *blockPtr;
    BTlistIterator pageList( blockChildren );
    while ( (blockPtr = pageList.Iterate()) != NULL )
      blockPtr->NormalizeNetwork();
  }
  NormalizeEnergy();
}

