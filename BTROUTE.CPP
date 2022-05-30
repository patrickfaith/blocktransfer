#include <btdefs.h>
#include <btlist.h>
#include <btblocks.h>
#include <btproto.h>
void BTbookBlock::transInRoute(BTblock *trans)
{
}
void BTbookBlock::transOutRoute(BTblock *trans)
{
}
void BTviewBlock::transOutRoute(BTblock *trans)
{
}
void ProcessTrigger(BTbookBlock *theBook, BTblock *focalBlk, char *data)
{
  if ( focalBlk == NULL ) return ;
  theBook->pointerToggle = 0;
  if ( focalBlk->Type() == BT_TYPE_PAGE )
  {
    if ( focalBlk->blockChildren != NULL )
      if ( focalBlk->blockChildren->ptr != NULL )
        if ( focalBlk->blockChildren->ptr->Type() == BT_TYPE_BOTS )
	    {
	      focalBlk->blockChildren->ptr->exPort(theBook,data);
        }
  }
  else if ( focalBlk->Type() == BT_TYPE_BOTS )
  {
    focalBlk->exPort(theBook,data);
  }
  else if ( focalBlk->Type() == BT_TYPE_CHUNK )
  {
    // move chunk to buffer - if buffer is command execute it
    char *subBuffer = focalBlk->lineToString();
    theBook->ProcessCommandBuffer( subBuffer ) ;
    delete subBuffer;
  }
  theBook->pointerToggle = 1;
}

void BTviewBlock::transInRoute(BTblock *trans)
{
  if ( trans == NULL ) return ;
  if ( trans->Type() == BT_TYPE_MESSAGE )
  {
    BTmessageBlock *msgBlk = (BTmessageBlock *) trans ;
    if ( msgBlk->iMessage == WM_CHAR )
    { 
      if ( ( trigKeyReturn != NULL ) &&
           (msgBlk->wParam == '\n') )
        ProcessTrigger( bookPtr, trigKeyReturn, NULL );        
      else if ( trigInput != NULL )
        trigInput->transInRoute(trans) ;
      else if ( buffer != NULL )
        buffer->ProccessKeystroke(msgBlk->wParam);
    }
  }
  return ;
}

void BTfieldBlock::transInRoute(BTblock *trans)
{
  if ( trans == NULL ) return ;
  if ( trans->Type() == BT_TYPE_MESSAGE )
  {
    BTmessageBlock *msgBlk = (BTmessageBlock *) trans ;
    if ( msgBlk->iMessage == WM_CHAR )
    { 
      if ( trigInput != NULL )
        trigInput->transInRoute(trans) ;
      else if ( buffer != NULL )
        buffer->ProccessKeystroke(msgBlk->wParam);
    }
  }
  return ;
}