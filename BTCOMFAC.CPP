#include <string.h>
#include <btdefs.h>
#include <btlist.h>
#include <btblocks.h>
#include <btgraph.h>
#include <btproto.h>

void FacadeCommand(char *buffer, BTbookBlock *theBook)
{
  BTblock *newBlock=NULL;


  if ( theBook->Focus() == NULL )  return ;

  if (theBook->Focus()->Type() != BT_TYPE_PAGE) return ;

  BTpageBlock *thePage = (BTpageBlock *) theBook->Focus();

  if (strncmpi(buffer,     "@facade file",12) == 0 )
  {
    if ( thePage->facade == NULL )
      thePage->facade = new BTbmpGraph();
    BTbmpGraph *bmpGraph = (BTbmpGraph *) thePage->facade ;

    strcpy(bmpGraph->bmpName,buffer+13) ;

    bmpGraph->mapType = 1 ;
    bmpGraph->displayType = 1;
  }
  else if (strncmpi(buffer,"@facade delete",14) == 0 )
  {
    if ( thePage->facade != NULL )
    {
      delete thePage->facade ;  
      thePage->facade = NULL ;
    }
  }
  else if (strncmpi(buffer,"@facade pal",11) == 0 )
  {
    if ( thePage->facade != NULL )
    {
      LoadFacadePallette( thePage, thePage->facade) ;
    }
  }
  else if (strncmpi(buffer,"@facade background",17) == 0 )
  {
   // if (facade != NULL )
  }
  else if (strncmpi(buffer,"@facade z-buffer",17) == 0 )
  {
    // if (facade != NULL )  
  }

}

void DeactivateCommand(char *buffer, BTbookBlock *theBook)
{


}
