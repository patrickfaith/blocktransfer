///
///  templates for lists and their manipulation
///
///
///  this is the list item base, used for providing list elements
///
class BTblock;

class BTlist
{
  public:

  BTlist	*next;
  BTblock	*ptr;
  short         blockId;
  short         pageId;
  BTfix		weight;
  /// function definitions
  
  BTlist::BTlist(BTblock *blkPtr)  ;  // create list element


  BTlist::BTlist(BTblock *blkPtr, short blockNumber, short pageNumber);
  
  BTlist::BTlist(BTblock *blkPtr,BTlist *nxt) ;   // create list element
  ~BTlist();

  // BTlist *Next() { return next; } // function to return next

  // T	    *Ptr() { return ptr; } // function to return ptr



  BTlist *add(BTblock *blkPtr)	;	   // function to creat list entry;

};

typedef BTlist **BTlistHnd;

///
///  the list iterator is used to manipulate entire lists
///

 class BTlistIterator
{
  public:

  BTlist	*head;
  BTlist	*previous;
  BTlist     *current;
  BTlist     *nextCurrent;
  BTlistHnd  root;

  /// functions that iterate upon a whole list

  BTlistIterator::BTlistIterator(); // initialize from list head

  BTlistIterator( BTlist *list); // initialize from list head
  
  BTlistIterator( BTlistHnd header) ;
  
  ~BTlistIterator();
  BTlist  *Head() { return head; }  // return the start of the list

  void add(BTblock *blkPtr) ; // add item to a list head


  BTlist * addCurrent(BTblock *blkPtr) ; // add item before current

  void ReplaceCurrentAtom(BTblock *blkPtr) ; // add item before current

  BTlist * addEnd(BTblock *blkPtr,short blkId,short pgId) ;  // add item to a list head

  BTblock *FindAtom(BTblock *blkPtr) ; // add item to a list head

  BTlist * removeCurrent() ; // delete atom currently pointed at

  BTblock *CurrentPtr() ; // return current pointer
  
  BTfix CurrentWeight();
  void SetWeight(BTfix wgt);
  
  short CurrentBlockId() ; // add item to a list head

  short CurrentPageId() ; // add item to a list head

  void CurrentSetId(short blockNumber, short pageNumber ) ; 

  void CurrentSetPtr(BTblock *blkPtr) ;

  void Reset() ;
  void Reset( BTlist *list ) ;
  void Reset( BTlistHnd header) ;
  void Null() ;

  BTblock  *Iterate() ;// keep returning next

  BTblock  *IterateLast();

  short IterateNull() ;// keep returning next

  void  BTlistIterator::erase() ;

};
