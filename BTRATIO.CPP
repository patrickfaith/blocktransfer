//  BTratio.c - general routines
//    that set the block/transfer ratios
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

void BTblock::DetermineBlockRatio()
{
  if (blockChildren == NULL) return ;

  BTblock *blockPtr ;

  BTval blockAve = 0,transAve = 0 ;
  int   totAve = 0;

  BTlistIterator blockList( blockChildren );
  while ( (blockPtr = blockList.Iterate()) != NULL )
  {
    totAve += 1;
    blockAve += blockPtr->blockRatio;
    transAve += blockPtr->transferRatio;
  }
  blockRatio = blockAve / totAve ;  // divide ??? how does this work with fix
  transferRatio = transAve / totAve ;
  return ;
}

void  BTblock::DetermineTransferRatio(BTblock *initPtr, BTblock *resultPtr)
{
  blockRatio=(initPtr->blockRatio+resultPtr->transferRatio)/2;
  transferRatio=(initPtr->transferRatio+resultPtr->transferRatio)/2;
  return ;
}

BTblock *BTblock::ResultConnection( BTblock *initPtr,
				    BTblock *resultPtr,
				    BTtrans *trans )
{
  BTblock *blockPtr;
  BTblock *transPtr;

  if ( resultPtr == NULL ) return NULL ;

  BTlistIterator blockList( resultPtr->blockChildren );
  while ( (blockPtr = blockList.Iterate()) != NULL )
  {
    blockPtr->DetermineBlockRatio();
  }

  resultPtr->DetermineBlockRatio();

  transPtr = new BTblock(trans);
  transPtr->DetermineTransferRatio(initPtr,resultPtr);

  return transPtr ;

}