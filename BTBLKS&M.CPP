
#include <string.h>
#include <btdefs.h>
#include <btlist.h>
#include <btblocks.h>
#include <btgraph.h>
#include <btproto.h>

// routines to send & mangle blocks

// functions used to move blocks beteens pages/streams

//void BTblock::sendType(BTtrans *trans)
// {
//  char buffer[132];
//
//  sprintf(buffer,"type %d \n",Type() );
//  trans->exPort(this,buffer);
//
//  return ;
// }

void BTblock::sendId(BTtrans *trans)
{
  char buffer[132];

  sprintf(buffer,"id %s %d %d \n",pagePtr->Name(),Id(),Type() );
  trans->exPort(this,buffer);

  return ;
}

void BTblock::sendEvent(BTtrans *trans)
{
  char buffer[132];
  sprintf(buffer,"event %d %d %d\n",blockRatio,transferRatio,btEnergy);
  trans->exPort(this,buffer);
  return ;
}

void BTblock::sendParents(BTtrans *trans)
{
  BTblock			*blockPtr;
  BTlistIterator 	sentHead( blockParents );
  char buffer[132];

  while ( (blockPtr = sentHead.Iterate()) != NULL )
  {
    sprintf(buffer,"parent ");trans->exPort(this,buffer);
    blockPtr->sendId(trans) ;
  }
  
  return ;
}

void BTblock::sendChildren(BTtrans *trans)
{
  BTblock			*blockPtr;
  BTlistIterator 	sentHead( blockChildren );
  char buffer[132];

  while ( (blockPtr = sentHead.Iterate()) != NULL )
  {
    sprintf(buffer,"child ");trans->exPort(this,buffer);
    blockPtr->sendId(trans) ;
  }
  return ;
}

void BTblock::sendTransInput(BTtrans *trans)
{
  BTblock			*blockPtr;
  BTlistIterator 	sentHead( transferInput );
  char buffer[132];

  while ( (blockPtr = sentHead.Iterate()) != NULL )
  {
    sprintf(buffer,"input ");trans->exPort(this,buffer);
    blockPtr->sendId(trans) ;
  }
  return ;
}

void BTblock::sendTransOutput(BTtrans *trans)
{
  BTblock			*blockPtr;
  BTlistIterator 	sentHead( transferOutput );
  char buffer[132];

  while ( (blockPtr = sentHead.Iterate()) != NULL )
  {
    sprintf(buffer,"output ");trans->exPort(this,buffer);
    blockPtr->sendId(trans) ;
  }
  return ;
}

void BTblock::sendBlockAscii(BTtrans *trans)
{
  // sendType(trans);
  sendId(trans);
  sendEvent(trans);
  SaveGraphicalObjects(trans,this);
  sendParents(trans);
  sendChildren(trans);
  sendTransInput(trans);
  sendTransOutput(trans);
  sendUnique(trans);

  return ;
}

void BTblock::sendStream(BTtrans *trans)
{
  return;
}

/// specific routines to the stringBlock
void BTstringBlock::sendUnique(BTtrans *trans)
{
  char buffer[132];

  sprintf(buffer,"string %s \n", string );
  trans->exPort(this,buffer);

  return ;
}

/// specific routines to the stringBlock
void BTcFaceBlock::sendUnique(BTtrans *trans)
{
  char buffer[132];
  char *strng;

  if (internalPtr != NULL)
  {
    strng = internalPtr->Name();
    if ( strng != NULL )
    {
      sprintf(buffer,"string %s \n", strng );
      trans->exPort(this,buffer);
    }
  }
  return ;
}
// ************ receive block info ******************


BTblock* BTblock::createBlock(char *pageName,short blkId,long blkType)

{
  //   BTblock *thePage;
  BTblock *newBlock = NULL;

  if ( blkType == BT_TYPE_BASIC )
  {
    newBlock = new BTblock(this, blkId);
  }
  else if ( blkType == BT_TYPE_GROUP )
  {
    newBlock = new BTgroupBlock(this, blkId);
  }
  else if (blkType == BT_TYPE_CHUNK )
  {
    newBlock = new BTchunkBlock(this, blkId);
  }
  else if (blkType == BT_TYPE_STRING )
    newBlock = new BTstringBlock(this, blkId);
  else if (blkType == BT_TYPE_READ )
    newBlock = new BTreadBlock(this, blkId);
  else if (blkType == BT_TYPE_PAGE )
    newBlock = new BTpageBlock(this, pageName);
  else if (blkType == BT_TYPE_FIND )
    newBlock = new BTfindBlock(this, blkId);

  return newBlock;
}

BTblock* BTblock::receiveId(BTlist **addList,BTtrans *trans)
{
  char *buffer;

  char pageName[80];
  char idJunk[80];
  BTlist *newListPtr;
  short blkId;
  short blkType;

  BTbookBlock *bookBlkPtr ;

 //  BTblock *newBlock;

  buffer = trans->inPort(this);
  sscanf(buffer,"%s %s %d %d ",idJunk,pageName,&blkId,&blkType);

  // scan book for page
  BTblock *blkPagePtr = NULL ;
  bookBlkPtr = BookPtr();
  if ( bookBlkPtr != NULL )
  {
    BTlistIterator pageList(bookBlkPtr->blockChildren );
    while ( (blkPagePtr = pageList.Iterate()) != NULL )
      if ( strcmpi(pageName,blkPagePtr->Name()) == 0 )
        break ;
  }
  else
    return  NULL ;
    // a must to have a parent book
    // a must to have a parent page

  // if page not found ... create page "unloaded"

  if ( bookBlkPtr == this )
  {
    if ( blkPagePtr == NULL )
    { 
      if ( blkType == BT_TYPE_VIEW )
        blkPagePtr = new BTviewBlock(bookBlkPtr,pageName);
      else
        blkPagePtr = new BTpageBlock(bookBlkPtr,pageName);
      blkPagePtr->blockId = blkId;
      return NULL;
    }     

    newListPtr = new BTlist(blkPagePtr,0,blkId);

    if ( *addList != NULL )
    {
      BTlist *nextPtr;
      nextPtr = *addList ;
      while ( nextPtr->next != NULL )
        nextPtr= nextPtr->next;
      nextPtr->next = newListPtr;
    }
    else
      *addList = newListPtr;
  }
  else
  {
    if ( blkPagePtr == NULL )
    {
      blkPagePtr = new BTpageBlock(bookBlkPtr,pageName);
    }


    newListPtr = new BTlist(NULL,blkId,blkPagePtr->blockId);

    if ( *addList != NULL )
    {
      BTlist *nextPtr;
      nextPtr = *addList ;
      while ( nextPtr->next != NULL )
        nextPtr= nextPtr->next;
      nextPtr->next = newListPtr;
    }
    else
      *addList = newListPtr;  
  }
  return NULL;
}  

void BTblock::receiveEvent(BTtrans *trans)
{
  char *buffer,junk[30]; //,str[80];

  buffer = trans->inPort(this);
  if (strncmpi(buffer,"event",5) == 0 )
  { 
    int a,b,c;
    sscanf(buffer,"%s %d %d %d ",junk,&a,&b,&c);
    
    blockRatio = a;
    transferRatio = b;
    btEnergy= c;
    
  }
  else
  {
    btEnergy = 0;
    blockRatio = FIX_ONE;
    transferRatio = FIX_ONE;
    trans->exPort(this,buffer);
  }
  delete buffer;

  return ;
}

void BTblock::receiveParents(BTtrans *trans)
{
  BTblock *blockPtr;
  
  char *buffer,*idBuf;
  short done;

  done = 0;
  while ( done == 0 )
  {
    buffer = trans->inPort(this);
    if (strcmpi(buffer,"parent") == 0 )  
    {
      // dump first 6 chars then shove it back into the trans
      idBuf = buffer + 6;
      trans->exPort(this,idBuf);
      blockPtr = receiveId(&blockParents,trans) ;
    }
    else
    {
      done = 1;
      trans->exPort(this,buffer);
    }
    delete buffer;
  }
  return ;
}


void BTblock::receiveChildren(BTtrans *trans)
{
  BTblock			*blockPtr;
  
  char *buffer,*idBuf;
  short done;

  done = 0;
  while ( done == 0 )
  {
    buffer = trans->inPort(this);
    if (strncmpi(buffer,"child",5) == 0 )  
    {
      // dump first 6 chars then shove it back into the trans
      idBuf = buffer + 5;
      trans->exPort(this,idBuf);
      blockPtr= receiveId(&blockChildren,trans) ;
    }
    else
    {
      done = 1;
      trans->exPort(this,buffer);
    }
    delete buffer;
  }
  return ;
}

void BTblock::receiveTransInput(BTtrans *trans)
{
  BTblock			*blockPtr;
  
  char *buffer,*idBuf;
  short done;

  done = 0;
  while ( done == 0 )
  {
    buffer = trans->inPort(this);
    if (strncmpi(buffer,"input",5) == 0 )  
    {
      // dump first 5 chars then shove it back into the trans
      idBuf = buffer + 5;
      trans->exPort(this,idBuf);
      blockPtr= receiveId(&transferInput,trans) ;
    }
    else
    {
      done = 1;
      trans->exPort(this,buffer);
    }
    delete buffer;
  }
  return ;
}

void BTblock::receiveTransOutput(BTtrans *trans)
{
  BTblock			*blockPtr;
  
  char *buffer,*idBuf;
  short done;

  done = 0;
  while ( done == 0 )
  {
    buffer = trans->inPort(this);
    if (strncmpi(buffer,"output",6) == 0 )  
    {
      // dump first 6 chars then shove it back into the trans
      idBuf = buffer + 6;
      trans->exPort(this,idBuf);
      blockPtr= receiveId(&transferOutput,trans) ;
    }
    else
    {
      done = 1;
      trans->exPort(this,buffer);
    }
    delete buffer;
  }
  return ;
}


void BTblock::receiveBlockAscii(BTtrans *trans)
{
  // receiveId(trans);
  receiveEvent(trans);
  LoadGraphicalObjects(trans,this);
  receiveParents(trans);
  receiveChildren(trans);
  receiveTransInput(trans);
  receiveTransOutput(trans);
  receiveUnique(trans);

  return ;
}

// specific routines to the stringBlock

void BTstringBlock::receiveUnique(BTtrans *trans)
{
  char *buffer,junk[30],str[80];

  buffer = trans->inPort(this);
  if (strncmpi(buffer,"string",6) == 0 )
  {
    sscanf(buffer,"%s %s ",junk,str);

    string = new char[strlen(str)+1];
    strcpy(string,str);

  }
  else
  {
    trans->exPort(this,buffer);
  }
  delete buffer;
  return ;
}


