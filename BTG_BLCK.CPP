#include <btdefs.h>
#include <btlist.h>
#include <btblocks.h>
#include <btgraph.h>
#include <btproto.h>

void BTblock::PlaceGraph(BTtrans *trans)
{ 
  BTplace *placePtr;
  if ( (placePtr =  trans->PlacePtr()) == NULL  )
    return ;
  placePtr->PlaceGraph(graph);
}
void BTpageBlock::PlaceGraph(BTtrans *trans)
{
  BTblock *blockPtr;
  if ( place == NULL  )
  {
    place = new  BTplace();
  }
  place->PlaceGraph(graph);
  BTlistIterator blockList(blockChildren);
  while ( (blockPtr = blockList.Iterate()) != NULL )
    blockPtr->PlaceGraph(this);
}
// block level routines to page position to book position
void BTblock::PageToBook(BTtrans *trans) // trans must have matrix
{
  if ( graph == NULL ) return ;
  graph->PageToBook(trans->SpacePtr());   
}
void BTpageBlock::PageToBook(BTtrans *trans) // trans not used
{ 
  BTblock *blockPtr;
  if (graph != NULL )
    graph->PageToBook(SpacePtr());
  BTlistIterator blockList(blockChildren);
  while ( (blockPtr = blockList.Iterate()) != NULL )
    blockPtr->PageToBook(this);
}
// block level routines to move book info to pixels
void BTblock::BlocksToPixels(BTviewBlock *view) // trans must be view
{ 
  if (graph != NULL )
    graph->BlocksToPixels(view);
}    
void BTpageBlock::BlocksToPixels(BTviewBlock *view)
{
  BTblock *blockPtr;
  if (graph != NULL )
    graph->BlocksToPixels(view);
    // if ((facade!=NULL)&&((deActivate & DISP_FACADE_FLAG)==0)) return ;
  BTlistIterator blockList(blockChildren);
  while ( (blockPtr = blockList.Iterate()) != NULL )
    blockPtr->BlocksToPixels(view);
}
// block routines that support BTgraph objects
void BTblock::AddBlockGraph(BTgraph *grph)
{
  grph->next = graph;
  graph = grph;
}
void BTblock::DeleteGraph()
{
  BTgraph *deleteGraph;
  while ( graph != NULL )
  {
    deleteGraph = graph;
    graph = graph->next;
    delete deleteGraph;
  }
}