//
// BTgraph.c ... block routines for graphing blocks :)
//
#include <stdio.h>
#include <fstream.h>
#include <strstrea.h>
#include <iomanip.h>
#include <string.h>
#include <btdefs.h>
#include <btlist.h>
#include <btblocks.h>
//
// fixation routines
//


void BTblock::highWeight(BTtrans *trace)
{
  if ( btEnergy > trace->btEnergy )
  {
    trace->tracer(this);
  }
  return;
}

void BTpageBlock::highWeight(BTtrans *trace)
{
  BTblock *blockPtr;

  BTlistIterator<BTblock> childHead( blockChildren );
  while ( (blockPtr = childHead.Iterate()) != NULL )
    blockPtr->blockWeight(trace);
  return;
}

void BTtraceBlock::highWeight(BTtrans *trans)
{
  if ( counter >= countTot )
  {
    BTblock *blockPtr;
    BTlistIterator<BTblock> listHead( blockChildren );
    while ( (blockPtr = listHead.Iterate()) != NULL )
      if ( btlowestEnergy >= blockPtr->btEnergy )
      {
        blockchildren = listHead.removeCurrent(); break ;
      } 
  }

  addChild(trans);
  counter+=1;

  if ( btLowestEnergy > trans->btEnergy )
    btLowestEnergy = trans->btEnergy;

  return;
}

void BTbookBlock::highWeight(BTtrans *trace)
{
  // gather higher wieghted
  BTblock *blockPtr;
  BTlistIterator<BTblock> listHead( blockChildren );
  while ( (blockPtr = listHead.Iterate()) != NULL )
    blockPtr->highWeight(  ???? 
}

BTblock * BTblock::highList(BTlist<BTblock> *list)
{
  BTblock *blockPtr, *highBlockPtr;

  highBlockPtr = NULL;

  BTlistIterator<BTblock> listHead( list );
  while ( (blockPtr = listHead.Iterate()) != NULL )
  {
    if (highBlockPtr == NULL )
      highBlockPtr = blockPtr;
    else if (highBlockPtr->btEnergy < blockPtr->btEnergy )
      highBlockPtr = blockPtr;
  }
  return highBlockPtr;
}

void BTblock::leftCheck(BTtrans *trans)
{

}
//  normal blocks don't do anything on left thinks
void BTblock::leftThink(BTtrans *trans)
{
  return ;
}

//
// try to find best element in group and put
// it on the trace list
//
BTblock *BTblock::leftThinkSameScan(BTtrans trans)
{
  return NULL;
}

BTblock *BTchunkBlock::leftThinkSameScan(BTtrans trans)
{
  BTblock *blockPtr;
  BTlistIterator<BTblock> childsHead( blockChildren );
  while ( (blockPtr = childsHead.Iterate()) != NULL )
    if ( blockPtr == trans )
    {
      return this;
    }
  return NULL;
}

void BTgroupBlock::leftThink(BTtrans *trace)
{
  BTblock *blockPtr, *highBlockPtr;

  highBlockPtr = highList(blockChildren);

  if ( highBlockPtr == NULL ) return;
  trace->addChildren(highBlockPtr)
  // scan for chunks in tracers page that reference highBlock
  if ( trace->pagePtr != NULL )
  {
    BTlistIterator<BTblock> pageList( trace->pagePtr->blockChildren );
    while ( (blockPtr = pageList.Iterate()) != NULL )
      if ( blockPtr != this )
	if ( blockPtr->leftThinkSameScan(highBlockPtr) != NULL )
	  trace->addOuput(blockPtr);
  }
  return;
}

//
// children are elements (no graph but trace)
// output/input are possible graph extensions
//
void BTchunkBlock::leftThink(BTtrans *trans)
{
}
//
// elements that may be placed on the list
//
void BTstringBlock::leftThink(BTtrans *trans)
{
}
//
// a perty tricky routine ... figures out which direction to head
// i.e. which is the next block to go to
//
void BTtraceBlock::leftThinkDirection(BTtrans *trans)
{
// only go through the inputs once ... so if there are any
// chirdrn you know only to use the outputs
  if ( blockChildren == NULL )
  {
  }
//
  else // build forward sentence
  {
    BTblock	*blockPtr;
    BTlistIterator<BTblock> outputHead( outputTransfer );
    while ( (blockPtr = outputHead.Iterate()) != NULL )
    {

    }
    
  }


}

//
// do a straight simple linear forward search
//
void BTbookBlock::leftThink(BTtrans *trans)
{
  BTtraceBlock tracy(this);

  tracy.countTot = 100;
  highWeight(&tracy); // gets a list of the 100 highest blocks in book

  // using the highlist, figure 5 fixation starting points

  // loop on fixations

    // fix on next thing
    // if fixations meet assign transfers between them

    // determine if fixed up yet - i.e. are we done looping

  // do fixations linear

}

void BTleftFixBlock::block(BTtrans *transFix )
{
  // transfix hear is something that like the book block
  // place new blocks on the page transOutput
  //


  // determine importance

  // if important then transfigure to transFix

  // figure out where to move next

  // either put movement on transfix or fixate subblock

}