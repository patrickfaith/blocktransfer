///
// block/page management pointers, handles and list
///
#include <stdio.h>
#include <btdefs.h>
#include <BTlist.h>

///  this is the list item base, used for providing list elements
///

  BTlist::BTlist(BTblock *blkPtr)    // create list element
  {
    ptr     = blkPtr;
    next    = NULL;
    blockId = 0;
    pageId  = 0;
  }

  BTlist::BTlist(BTblock *blkPtr, short blockNumber, short pageNumber)
  {
    ptr  = blkPtr;
    next = NULL;
    blockId = blockNumber ;
    pageId  = pageNumber ;
  }

  BTlist::~BTlist() {};

  BTlist::BTlist(BTblock *blkPtr,BTlist *nxt)    // create list element
  {
    ptr = blkPtr;
    next = nxt;
  }

  

  BTlist *BTlist::add(BTblock *blkPtr)		   // function to creat list entry;
  {
    BTlist *nList;

    nList = new BTlist(blkPtr);
    return ( nList );
  }




///
///  the list iterator is used to manipulate entire lists
///



  BTlistIterator::BTlistIterator() // initialize from list head
  {
    head = NULL;
    current = NULL;
    previous = NULL;
    nextCurrent = NULL;
  }


  BTlistIterator::BTlistIterator( BTlist *list) // initialize from list head
  {
    nextCurrent = list;
    head    = list;
    previous = NULL;
    current  = NULL;
    root = NULL;
  }

  BTlistIterator::BTlistIterator( BTlistHnd header) // initialize from list head
  {
    nextCurrent = *header;
    head    = *header;
    previous = NULL;
    current  = NULL;
    root = header;
  }

  BTlistIterator::~BTlistIterator(){};

  void BTlistIterator::add(BTblock *blkPtr)  // add item to a list head
  {
    BTlist *newList;

    newList = new BTlist(blkPtr);

    newList->next = head;
    head = newList;
    if ( root != NULL )
      *root = head;
    return ;
  }
  

  BTlist * BTlistIterator::addCurrent(BTblock *blkPtr) // add item before current
  {
    BTlist *newList; //,*endList;

    newList = new BTlist(blkPtr);

    if ( head == NULL )
    {
      head = newList;
      current = newList;
      nextCurrent = current->next;
      previous = NULL;
      if ( root != NULL )
        *root = head;
    }
    else if ( previous == NULL )
    {
      newList->next = head;
      head = newList;
      current = newList ;
      nextCurrent = current->next;
      if ( root != NULL )
        *root = head;
    }
    else
    {
      newList->next = previous->next;
      previous->next = newList ;
      current = newList;
      nextCurrent = current->next;
    }
    return ( head );
  }
  

  void BTlistIterator::ReplaceCurrentAtom(BTblock *blkPtr) // add item before current
  {
    if ( current != NULL ) current->ptr = blkPtr ;
  }
   

  BTlist * BTlistIterator::addEnd(BTblock *blkPtr,short blkId,short pgId)  // add item to a list head
  {
    BTlist *newList,*endList;

    newList = new BTlist(blkPtr);
    newList->blockId = blkId;
    newList->pageId  = pgId;

    if ( head == NULL )
    {
      head = newList;
      if ( root != NULL )
        *root = head;
    }
    else     // look for end of list then insert
    {
       endList = head;
       while ( endList->next != NULL )
	     endList = endList->next;
       endList->next = newList;
     }
     return ( head );
  }
  

  BTblock *BTlistIterator::FindAtom(BTblock *blkPtr)  // add item to a list head
  {

    if ( head == NULL )
    {
      return NULL ;
    }
    else     // look for end of list then insert
    {
      BTlist *listPtr;

      listPtr = head;
      while ( listPtr != NULL )
      {
	    if (listPtr->ptr == blkPtr)
	      return listPtr->ptr ;
	    listPtr = listPtr->next;
      }
    }
    return ( NULL );
  }
  

  BTlist * BTlistIterator::removeCurrent()  // delete atom currently pointed at
  {
    // BTlist *newList; // ,*endList;
    if ((head != NULL) && (current != NULL))
    {
      if ( previous != NULL )  // in middle of list
      {
	    previous->next = current->next;
	    delete current;
	    current = previous->next;
        if (current != NULL )
	      nextCurrent = current->next;
	    else
          nextCurrent = NULL;
       }
       else // beginning of list
       {
	     head = current->next;
	     if ( root != NULL )
           *root = head;
	     delete current;
	     current = head;
         if (current != NULL )
	       nextCurrent = current->next;
	     else
           nextCurrent = NULL;
       }
     }
     return ( head );
  }
  
  BTblock *BTlistIterator::CurrentPtr()  // return current pointer
  {
    if ( current == NULL )
      return NULL ;
    else
      return current->ptr;
  }

  BTfix BTlistIterator::CurrentWeight()  // return current pointer
  {
    if ( current == NULL )
      return 0 ;
    else
      return current->weight;
  }
  void BTlistIterator::SetWeight(BTfix wgt)  // return current pointer
  {
    if ( current != NULL )
      current->weight = wgt;
  }    

  short BTlistIterator::CurrentBlockId()  // add item to a list head
  {

    if ( current == NULL )
      return 0 ;
    else     // look for end of list then insert
      return current->blockId;
  }
  

  short BTlistIterator::CurrentPageId()  // add item to a list head
  {
    if ( current == NULL )
      return 0 ;
    else
      return current->pageId;
  }
  

  void BTlistIterator::CurrentSetId(short blockNumber, short pageNumber )
  {
    if ( current != NULL )
    {
      current->blockId = blockNumber ;
      current->pageId  = pageNumber ;
    }
  }
  

  void BTlistIterator::CurrentSetPtr(BTblock *blkPtr)
  {
    if ( current != NULL )
      current->ptr = blkPtr;
  }
  void BTlistIterator::Reset( BTlist *list )
  {
    nextCurrent = list;
    head    = list;
    previous = NULL;
    current  = NULL;
  }
  
  void BTlistIterator::Reset( BTlistHnd listHnd )
  {
    nextCurrent = *listHnd;
    head    = *listHnd;
    root = listHnd;
    previous = NULL;
    current  = NULL;
  }

  void BTlistIterator::Reset()
  {
    current = head; // changed to head instead of NULL
    nextCurrent = head;
    previous = NULL;
    return;
  }
  

  void BTlistIterator::Null()
  {
    current = NULL;
    head    = NULL;
    previous = NULL;
    nextCurrent = NULL;
    return;
  }

  BTblock  *BTlistIterator::Iterate() // keep returning next
  {
    BTblock *atomPtr;
    if ( nextCurrent != NULL )
    {
       previous = current;
       current = nextCurrent;
       atomPtr = current->ptr;
       nextCurrent = current->next;
    }
    else
    {
       atomPtr = NULL;
       previous = current;
       current = nextCurrent;
       atomPtr = NULL;
       nextCurrent = NULL;
    }
    return atomPtr;
  }

  BTblock  *BTlistIterator::IterateLast() // keep returning next
  {
    BTblock *atomPtr = NULL;

    if (current == head ) return NULL;

    BTlist  *endList = head;

    while ( endList->next != current )
      endList = endList->next;

    current = endList;
    if (current != NULL )
      atomPtr = current->ptr;

    return atomPtr;

  }


  short BTlistIterator::IterateNull() // keep returning next
  {
    if ( nextCurrent != NULL )
    {
      previous = current;
      current  = nextCurrent;
      nextCurrent = current->next;
      return 1;
    }
    else
      return NULL;
  }
  
  
  void BTlistIterator::erase()
  {
    BTlist *zapList;

    current = head ;
    while ( current != NULL )
    {
      zapList = current;
      current = current->next;
      delete zapList; 
    }

    current=NULL;head=NULL;nextCurrent=NULL;previous=NULL;
    if (root != NULL )
      *root = head;
    return ;
  }
