#include <string.h>
#include <btdefs.h>
#include <btlist.h>
#include <btblocks.h>
#include <btproto.h>

void BTblock::blockBlockEnergy(BTfix energy)
{
  btEnergy = normalF(mulF(blockRatio,energy)) + btEnergy;
  return;
}
void BTblock::blockTransferEnergy(BTfix energy)
{
  btEnergy = normalF(mulF(blockRatio,energy))+ btEnergy ;
  return;
}

BTfix BTblock::transferTransferEnergy()
{ 
  BTfix temp;
  temp = normalF(mulF(transferRatio,btEnergy)) ;
  return temp;
}

BTfix BTblock::transferBlockEnergy()
{ 
  BTfix temp; 
  temp = normalF(mulF(transferRatio,btEnergy));
  return temp;
}

void BTblock::transfer(BTtrans *trans)
{
  BTblock	*blockPtr;
  BTfix tempE;

  BTlistIterator transOutList( transferOutput );
  while ( (blockPtr = transOutList.Iterate()) != NULL )
  {
    // energy from here
    tempE = normalF(mulF(btEnergy,transferRatio));
    // energy able to accept
    tempE = normalF(mulF(tempE,blockPtr->blockRatio));
    // remove energy accepted
    btEnergy -= tempE;
    // add energy accpeted to parent
    blockPtr->btEnergy += tempE;
  }


  BTlistIterator transChildList( blockChildren );
  while ( (blockPtr = transChildList.Iterate()) != NULL )
  {
    // energy from here
    tempE = normalF(mulF(btEnergy,transferRatio));
    // energy able to accept
    tempE = normalF(mulF(tempE,blockPtr->blockRatio));
    // remove energy accepted
    btEnergy -= tempE;
    // add energy accpeted to parent
    blockPtr->btEnergy += tempE;
  }
  return ;
}

void BTpageBlock::transfer(BTtrans *trans)
{
  BTblock	*blockPtr;
 
  BTlistIterator transChildList( blockChildren );
  while ( (blockPtr = transChildList.Iterate()) != NULL )
    blockPtr->transfer( NULL );

  return ;
}

void BTbookBlock::transfer(BTtrans *trans)
{
  BTblock	*blockPtr;

  BTlistIterator transChildList( blockChildren );
  while ( (blockPtr = transChildList.Iterate()) != NULL )
    blockPtr->transfer( NULL );

  return ;
}

void BTblock::block(BTtrans *trans)
{
  BTblock	*childPtr; 
  BTfix		tempE;
  BTlistIterator childHead( blockChildren );
  while ( (childPtr = childHead.Iterate()) != NULL)
  {
    // energy from internal
    tempE = childPtr->btEnergy*childPtr->transferRatio;
    // energy able to accept
    tempE *= blockRatio;
    // remove energy accepted from child
    childPtr->btEnergy -= tempE;
    // add energy accpeted to parent
    btEnergy += tempE;
  }
  return ;
}

void BTchunkBlock::block(BTtrans *trans)
{
  BTblock	*childPtr; 
  BTfix		tempE;
  BTlistIterator childHead( blockChildren );
  while ( (childPtr = childHead.Iterate()) != NULL)
  {
    // energy from internal
    tempE = 
      normalF(mulF(childPtr->btEnergy,
                   childPtr->transferRatio));
    // energy able to accept
    tempE = normalF(mulF(tempE,blockRatio));
    // remove energy accepted from child
    childPtr->btEnergy -= tempE;
    // add energy accpeted to parent
    btEnergy += tempE;
  }
  return ;
}

void BTpageBlock::block(BTtrans *trans)
{
  BTblock	*childPtr; 

  BTlistIterator childHead( blockChildren );
  while ( (childPtr = childHead.Iterate()) != NULL)
  {
    childPtr->block(NULL);
  }
  return ;
}

void BTbookBlock::block(BTtrans *trans)
{
  BTblock	*childPtr; 

  BTlistIterator childHead( blockChildren );
  while ( (childPtr = childHead.Iterate()) != NULL)
    childPtr->block(NULL);
  return ;
}

short BTblock::compare(BTtrans *trans)
{
  // if the block has a name do a comparison
  BTblock *childPtr;

  char *word1,*word2;

  short status;

  status = 0;

  if ( trans == NULL ) return status;
  if ( ( (word1 = Name()) != NULL )  &&
       ( (word2 = trans->Name()) != NULL ) &&
       ( strcmpi(word1,word2) == 0 )  )
    status = 1;           //i.e. valid comparison
  BTlistIterator childHead( trans->blockChildren );
  if ( word1 != NULL )
    while ( (childPtr = childHead.Iterate()) != NULL)
      if ( (word2 = childPtr->Name()) != NULL )
	if ( strcmpi(word1,word2) == 0 )
	{
          status = 1;
          return status;
	}
  return status;
}

