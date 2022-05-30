
#include <string.h>
#include <btdefs.h>
#include <btlist.h>
#include <btblocks.h>
#include <btgraph.h>
#include <btproto.h>

void BTbookBlock::sendBlockAscii(BTtrans *trans)
{
  BTblock	*blockPtr ;

  /// write to the stream until no more blocks left

  // store page header stuff
  char          file[30];
  strcpy(file,name);
  char *txt=".blk";
  strcat(file,txt);

  BTtransFile  transOut(file);

  transOut.openExPort();

  sendUnique(&transOut);
  sendEvent(&transOut);
  sendParents(&transOut);
  sendChildren(&transOut);
  sendTransInput(&transOut);
  sendTransOutput(&transOut); 

  transOut.closePort();
  BTlistIterator pageHead( blockChildren );
  while ( (blockPtr = pageHead.Iterate()) != NULL )
    blockPtr->sendBlockAscii( &transOut );
  return;
}

void BTbookBlock::sendUnique(BTtrans *trans)
{
  return ;
}

void BTpageBlock::sendBlockAscii(BTtrans *trans)
{
  BTblock	*blockPtr ;
  /// write to the stream until no more blocks left
  // setup trans ptr for low level calls
  
  char          file[30];
  strcpy(file,name);
  char *txt=".blk";
  strcat(file,txt);

  BTtransFile  transOut(file);
  transOut.pagePtr = this;
  transOut.openExPort();
  sendUnique(&transOut);
  sendEvent(&transOut);

  BTlistIterator pageHead( blockChildren );
  while ( (blockPtr = pageHead.IterateLast()) != NULL )
    blockPtr->sendBlockAscii( &transOut);
  transOut.closePort();
  return;
}
void BTpageBlock::sendUnique(BTtrans *trans)
{
  char buffer[132];

  sprintf(buffer,"name %s \n", name );
  trans->exPort(this,buffer);
  
  /*
  sprintf(buffer,"matTrans %d %d %d \n",xTrans,yTrans,zTrans );
  trans->exPort(this,buffer);

  sprintf(buffer,"matScale %d %d %d \n",xScale,yScale,zScale );
  trans->exPort(this,buffer);

  sprintf(buffer,"matRot %d %d %d \n",xRot,yRot,zRot );
  trans->exPort(this,buffer);
  
  if (place != NULL )
  {
    sprintf(buffer,"place %d %d %d %d \n",
      place->Type(),
      place->Z,
      place->blockLength,
      place->radius );
    trans->exPort(this,buffer);
  }
  */
  if ( facade != NULL )
  {
    if ( facade->Type() ==  BT_BMP_GRAPH )
    {
      BTbmpGraph *bmpGraph = (BTbmpGraph *) facade ;
      sprintf(buffer,"facade %s %d %d %d %d %d \n",
			    bmpGraph->bmpName,
                            bmpGraph->Type(),
			    bmpGraph->displayType,
			    bmpGraph->mapType,
			    bmpGraph->heigth,
                            bmpGraph->width
			     );

      trans->exPort(this,buffer);
    }
  }
  
  BTWindow *windPtr;
  if ( ( windPtr = WindowPtr() ) != NULL )
  {
    sprintf(buffer,"window %d %d %d %d \n",
			    windPtr->X,
                windPtr->Y,
			    windPtr->Width,
			    windPtr->Height
			  );
    trans->exPort(this,buffer);
  }
  
  BTbuff *buffPtr;
  if ( ( buffPtr = BuffPtr() ) != NULL )
  {
    sprintf(buffer,"buffer %d %d \n",
			    buffPtr->columnTot,
			    buffPtr->rowTot
			  );
    trans->exPort(this,buffer);
  }
  return ;
}
// ------------
// receive blocks
// ------------

void BTbookBlock::receiveBlockAscii(BTtrans *trans)
{
  BTblock	*blockPtr ;

  /// write to the stream until no more blocks left

  // store page header stuff
  char          file[30];
  strcpy(file,name);
  char *txt=".blk";
  strcat(file,txt);

  BTtransFile  transIn(file);

  transIn.openInPort();

  receiveUnique(&transIn);
  receiveEvent(&transIn);
  receiveParents(&transIn);
  receiveChildren(&transIn); // needs to be different for book
			     // because it needs to create the initial
			     // pageBlocks - with name
			     // (pages need to create all blocks that
			     //  point to self, and lower levels just
			     //  set the pageId,blockId crap if nuthing
                             //  is hooked up.
  receiveTransInput(&transIn);
  receiveTransOutput(&transIn); 

  transIn.closePort();

  // load pages
  BTlistIterator pageHead( blockChildren );
  while ( (blockPtr = pageHead.Iterate()) != NULL )
    blockPtr->receiveBlockAscii( this );
// set up pages children pointers
  pageHead.Reset();
  while ( (blockPtr = pageHead.Iterate()) != NULL )
    BlockPaging(blockPtr->blockChildren,this);
// load in dna
  pageHead.Reset();
  BTpageBlock *pageBlkPtr;
  while ( (pageBlkPtr = (BTpageBlock *) pageHead.Iterate()) != NULL )
    pageBlkPtr->receiveChannel(this);

// init in dna
  pageHead.Reset();
  while ( (pageBlkPtr = (BTpageBlock *) pageHead.Iterate()) != NULL )
    pageBlkPtr->initializeDna(this);

  initializeDna(this);
  return;
}

void BTpageBlock::receiveBlockAscii(BTtrans *trans)
{
  BTblock	*blockPtr ;
 
  /// write to the stream until no more blocks left
  // setup trans ptr for low level calls
  
  char          file[30];
  strcpy(file,name);
  char *txt=".blk";
  strcat(file,txt);

  BTtransFile  transIn(file);

  transIn.openInPort();

  receiveUnique(&transIn);
  receiveEvent(&transIn);
  do
  {
    blockPtr = receiveId(&blockChildren,&transIn);
    if ( blockPtr != NULL )
      blockPtr->receiveBlockAscii(&transIn);
  } while ( blockPtr != NULL ) ;
  transIn.closePort();
  return;
}


void BTpageBlock::receiveUnique(BTtrans *trans)
{
  short done = 0 ;
  char *buffer,junk[30],str[30];

  while ( done == 0 )
  {
    buffer = trans->inPort(this);
    if (strncmpi( buffer,"name", 4) == 0 )
    {
      sscanf(buffer,"%s %s ",junk,str);
      name = new char[strlen(str)+1];

      strcpy(name,str);
    }
    //else if (strncmpi( buffer,"matTrans" ,8) == 0 )
     // sscanf(buffer,"%s %d %d %d \n",junk,&xTrans,&yTrans,&zTrans );
    //else if (strncmpi( buffer,"matScale",8) == 0 )
     //  sscanf(buffer,"%s %d %d %d \n",junk,&xScale,&yScale,&zScale );
    //else if (strncmpi( buffer,"matRot",6) == 0 )
     //  sscanf(buffer,"%s %d %d %d \n",junk,&xRot,&yRot,&zRot );
    else if (strncmpi( buffer,"place",5) == 0 )
    {
      float placeZ;
      float placeBlockLength;
      float placeRadius;
      short placeType;
      sscanf(buffer,"%s %d %d %d \n",
        junk,
        &placeType,
        &placeZ,
        &placeBlockLength,
        &placeRadius );

      if ( place != NULL ) delete place ;

    //  if ( placeType == 3 )
	//    place = new BTplaceSteps();
    //  else 
	//    place = new BTplaceCylindar();
     // place->Z    = placeZ;
     // place->blockLength = placeBlockLength;
     // place->radius = placeRadius;
    }
    else if (strncmpi( buffer,"pipe",4) == 0 )
      sscanf(buffer,"%s %d \n",junk,&channelType );
    else if (strncmpi( buffer,"facade",4) == 0 )
    {
      facade = new BTbmpGraph(); 

      int i1,i2,i3,i4,i5;
      BTbmpGraph *bmpGraph = (BTbmpGraph *) facade ;
      sscanf(buffer,"%s %s %d %d %d %d %d ",
                            junk,str,&i1,&i2,&i3,&i4,&i5);
      strcpy(bmpGraph->bmpName,str);
      bmpGraph->displayType = i2;
      bmpGraph->mapType     = i3;
      bmpGraph->heigth      = i4;
      bmpGraph->width       = i5;
    }
    else if (strncmpi( buffer,"window",4) == 0 )
    { 
      BTWindow *windPtr; 
      if ( ( windPtr = WindowPtr() ) != NULL )
      {
        sscanf(buffer,"%s %d %d %d %d \n",
                junk,
			    &windPtr->X,
                &windPtr->Y,
			    &windPtr->Width,
			    &windPtr->Height );
	  }
    }
    else if (strncmpi( buffer,"buffer",6) == 0 )
    { 
      BTbuff *buffPtr = BuffPtr();
      if (buffPtr == NULL )
      {
        buffPtr = new BTbuff() ; 
        if ( BuffPtr(buffPtr) == NULL )
        {
          delete buffPtr; buffPtr=NULL ;
        }
      }
      if ( buffPtr != NULL )
      {
        sscanf(buffer,"%s %d %d \n",
                junk,
                &buffPtr->columnTot,
			    &buffPtr->rowTot
                );
        buffPtr->Setup(buffPtr->columnTot,buffPtr->rowTot) ;
	  }
    }
    else
    {
      trans->exPort(this,buffer);
      done = 1;
    }
    delete buffer;
  }
}


void BTbookBlock::receiveUnique(BTtrans *trans)
{

  char *buffer,junk[30],str[30];

  short done = 0;
  while ( done == 0 )
  {
    buffer = trans->inPort(this);
    if (strncmpi( buffer,"name",4) == 0 )
    {
      sscanf(buffer,"%s %s ",junk,str);
      name = new char[strlen(str)+1];

      strcpy(name,str);
    }
    else if (strncmpi( buffer,"ScrnAt",6) == 0 )
    {
    //  sscanf(buffer,"%s %f %f %f \n",
	//    junk, &(threeD.at.X), &(threeD.at.Y), &(threeD.at.Z) );
    }
    else
    {
      trans->exPort(this,buffer);
      done = 1;
    }
    delete buffer;
  }
}

BTblock* BTpageBlock::receiveId(BTlist **childList,BTtrans *trans)
{
  char *buffer;

  char pageName[80];
  char junk[80];
  int blkId;
  int  blkType;

  BTblock *newBlock;

  buffer = trans->inPort(this);

  if (strncmpi( buffer,"id",2) == 0) // this is a top level ID specifier
  {
    sscanf(buffer,"%s %s %d %d ",junk,pageName,&blkId,&blkType);

    // assume block is for current page
    // create

    newBlock = createBlock(pageName,blkId,blkType);

  }
  else
  {
    newBlock = NULL ;
  }

  delete buffer;

  return newBlock;
}


