

#include <string.h>
#include <btdefs.h>
#include <btlist.h>
#include <btblocks.h>
#include <btproto.h>

/// specific routines to the "readBlock"

char *BTreadBlock::inPort(BTtrans *trans)
{
   BTblock	*newGroup;
   char		*word;

   /// read from the stream until it is finnished
   while ( (word = trans->inPort(this)) != NULL)
   {
      /// each line of the stream is broken into words

      newGroup = new BTgroupBlock(trans);
      newGroup->addChild(new BTstringBlock(word,newGroup));

      while ( (word = trans->inPort(this)) != NULL )
      {
        if ( strcmpi(word,".") == 0 ) break ;
	newGroup->addChild(new BTstringBlock(word,newGroup));
      }
      addOutput(newGroup);
   }
   return NULL;
}

void BTreadBlock::exPort(BTtrans *trans)
{   
  BTblock	*blockPtr;

  /// write to the stream until no more blocks left 

  BTlistIterator paraHead( transferOutput );

  while ( (blockPtr = paraHead.Iterate()) != NULL )
  {
    blockPtr->writeString( trans);
  }
  return ;
}
///
///	page block routines
///

BTpageBlock::BTpageBlock( BTtrans *trans,char *strng)
{

  blockId = 0;
  nextBlockId = blockId + 1;
  deActivate = NULL ;
  channelType = 0;
  facade = NULL ;
  bookPtr = NULL;
  if ( trans != NULL )
    bookPtr = trans->BookPtr();

  if ( bookPtr != NULL )
  {
    BTlistIterator pageHead( bookPtr->blockChildren );
    BTblock *blockPtr;
    while ( (blockPtr = pageHead.Iterate()) != NULL )
    {
      if ( blockId <= blockPtr->blockId )
	blockId = blockPtr->blockId + 1;
    }
  }
  loaded = 1;

  pagePtr = this;
  name = new char[_fstrlen(strng)+1] ;

  _fstrcpy(name,strng);

  blockParents 	= NULL;
  blockChildren	= NULL;
  transferInput	= NULL;
  transferOutput= NULL;

  place = NULL ;

  if ( trans != NULL )
  {
    // don't tie cface to children lists
    if ( (strncmpi( strng,"cLFace", 6) != 0 ) &&
	 (strncmpi( strng,"cRFace", 6) != 0 ) )
    {
      bookPtr = trans->BookPtr();
      if ( bookPtr != NULL )
        bookPtr->addChild(this);
    }
  }

  // need to define things for the transform
  blockRatio = 0;
  transferRatio = 0;
  space = new BTspace();
  accum = NULL;
}

BTpageBlock::~BTpageBlock()
{
  // remove all children
  BTblock	*blockPtr ;
  BTlistIterator pageHead( blockChildren );

  while ( (blockPtr = pageHead.Iterate()) != NULL )
  {
    delete blockPtr;
  }

  if ( bookPtr != NULL )
    bookPtr->removeChild(this) ;
  
  if ( space != NULL )
    delete space;
}

BTbookBlock* BTpageBlock::BookPtr()
{
  return bookPtr;
}

// incorperate the strng into the page - i.e. create blocks etc ...
// moves a simple string into a list of blocks
//
/*
char *wordEnd(char *strng)
{
  while (( *strng != '\0' ) && (*strng != ' ')) strng++;
  return strng;
}

char *wordStart(char *strng)
{
  while (*strng == ' ') strng++;
  return strng;
}

BTblock *BTpageBlock::StringToBlocks(char *strng)
{

  char          tStr[256];
  char          *tempStr;
  BTblock	*newGroup;
  BTblock       *foundStringBlock;
  char		*word;

  size_t        length;
  char		*endPtr;

  // make a sentence
  if (strng==NULL) return NULL;
  newGroup = new BTchunkBlock(this);

  // strip the string into words

  strcpy(tStr,strng);
  tempStr = tStr;
  while ( *tempStr != '\0')
  {
    tempStr = wordStart(tempStr); 		// find word
    endPtr = wordEnd(tempStr);         // find end of word
    length = endPtr - tempStr ;
    word = new char[length+1] ;      // make new word
    _fstrncpy(word,tempStr,length);
    word[length]='\0';

    tempStr = tempStr + length  ;  // go to next word

    // put words into a string block and associate to sentence

    if ( ( foundStringBlock = bookPtr->SearchForWord(word) ) != NULL )
    {
      newGroup->addChild(foundStringBlock);
      delete word;
    }
    else
    {
      newGroup->addChild(new BTstringBlock(word,newGroup));
    }
    
  }

  return newGroup;
}
*/
// routines to initially process events

//
// if the page supports the message do something with it 
//
void BTpageBlock::transInRoute(BTblock *trans)
{
//  if (inRoute.PartOf(trans->Type()) != 0 )
//    trans->Execute(this);
  return;
}

void BTpageBlock::transOutRoute(BTblock *trans)
{
//  if (outRoute.PartOf(trans->Type()) != 0 )
//    trans->Execute(this);
  return;
}

short BTpageBlock::NextBlockId(BTtrans *trans)
{
  nextBlockId+=1;
  return nextBlockId; 
}