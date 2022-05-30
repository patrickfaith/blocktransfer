
#include <string.h>
#include <btdefs.h>
#include <btlist.h>
#include <btblocks.h>
#include <btgraph.h>

void PageingConnect(BTblock *bookPtr,BTlist *list)
{
  BTlist *nextPtr;

  BTblock *pageBlkPtr;
  BTblock *blkPtr;

  if ( bookPtr == NULL ) return ;

  nextPtr = list ;
  while (nextPtr != NULL )
  {
    BTlistIterator pageList(bookPtr->blockChildren );
    while ( ( pageBlkPtr = pageList.Iterate() ) != NULL )      
    {
      if ( pageBlkPtr->blockId == nextPtr->pageId )
      {
        BTlistIterator blockList(pageBlkPtr->blockChildren);
        while ( ( blkPtr = blockList.Iterate() ) != NULL )
        {       
	  if ( blkPtr->blockId == nextPtr->blockId )
	  {
            nextPtr->ptr = blkPtr;
	    break;
          }
	}
        break ;
      }
    }
    nextPtr = nextPtr->next;
  }
}

void PageingDisConnect(BTlist *list)
{
  BTlist *nextPtr;

  nextPtr = list ;
  while (nextPtr != NULL )
  {
    nextPtr->ptr = NULL ;
    nextPtr = nextPtr->next;
  }
}
void BlockPaging(BTlist *list,BTblock *book)
{
  // go through parents,children,inputs & outputs of all blocks
  BTblock *blockPtr;

  BTlistIterator pageList(list);
  while ( ( blockPtr = pageList.Iterate()) != NULL )
  {
    PageingDisConnect(blockPtr->blockChildren);
    PageingDisConnect(blockPtr->blockParents);
    PageingDisConnect(blockPtr->transferInput);
    PageingDisConnect(blockPtr->transferOutput);
  }

  pageList.Reset();
  while ( ( blockPtr = pageList.Iterate()) != NULL )
  {
    PageingConnect(book,blockPtr->blockChildren);
    PageingConnect(book,blockPtr->blockParents);
    PageingConnect(book,blockPtr->transferInput);
    PageingConnect(book,blockPtr->transferOutput);
  }
}


