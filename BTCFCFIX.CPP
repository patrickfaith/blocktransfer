// copyright (c) p l faith
//
// frontal lobe routines
//
#include <stdio.h>
#include <string.h>
#include <btdefs.h>
#include <btlist.h>
#include <btblocks.h>
#include <btproto.h>
// #include <btcface.h>

//
// routines to move through the c-face
//

void BTpageBlock::BandT(BTtrans *trans)
{
  BTblock *blockPtr;
  BTcFaceBlock *cBlockPtr;

  BTlistIterator transChildList( blockChildren );
  while ( (blockPtr = transChildList.Iterate()) != NULL )
  {
    cBlockPtr = (BTcFaceBlock *) blockPtr;
    cBlockPtr->BandT( NULL );
  }
}

void BTcFaceBlock::BandT(BTtrans *trans)
{
  BTblock       *blockPtr;

  BTlistIterator transOutList( transferOutput );
  while ( (blockPtr = transOutList.Iterate()) != NULL )
  {
    blockPtr->btEnergy+= btEnergy;
  }

  BTlistIterator transInputList( transferInput );
  while ( (blockPtr = transInputList.Iterate()) != NULL )
  {
    blockPtr->btEnergy+= btEnergy;
  }
  return ;
}

// simply find highest energy block in list
//
// A NORMAL FUNCTION !!!
//

BTblock *BlockHighList(BTlist *list)
{
  BTblock *blockPtr;
  BTblock *highBlockPtr;

  if (list == NULL) return NULL ;
  highBlockPtr = NULL;
  BTlistIterator highList( list );
  while ( (blockPtr = highList.Iterate()) != NULL )
  {
    if ( blockPtr->blockChildren != NULL ) continue ;
    if ( highBlockPtr == NULL )
      highBlockPtr = blockPtr;
    else if ( highBlockPtr->btEnergy < blockPtr->btEnergy )
      highBlockPtr = blockPtr;
  }

  return (BTcFaceBlock *) highBlockPtr;
}

BTblock *AnyBlockHighList(BTlist *list)
{
  BTblock *blockPtr;
  BTblock *highBlockPtr;

  if (list == NULL) return NULL ;
  highBlockPtr = NULL;
  BTlistIterator highList( list );
  while ( (blockPtr = highList.Iterate()) != NULL )
  {
    if ( highBlockPtr == NULL )
      highBlockPtr = blockPtr;
    else if ( highBlockPtr->btEnergy < blockPtr->btEnergy )
      highBlockPtr = blockPtr;
  }

  return (BTcFaceBlock *) highBlockPtr;
}

float compareListForWord(BTlist *list1, BTlist *list2)
{

  if ( ( list1 == NULL ) && ( list2 == NULL ) )
    return 1.0;

  if ( ( list1 == NULL ) && ( list2 != NULL ) )
    return 0.1;

  if ( ( list1 != NULL ) && ( list2 == NULL ) )
    return 0.1;

  char *word1 = list1->ptr->Word();
  char *word2 = list2->ptr->Word();

  if ( ( word1 != NULL ) && ( word2 != NULL ) )
  {
     if ( strcmp(word1,word2) == 0 )
       return 1.0;
     else
       return 0.5;
  }
  else
    return 0.1;
}

BTblock *BlockHighListNext(BTlist *list,BTblock *crntBlockPtr)
{
  BTblock *blockPtr;
  BTblock *highBlockPtr;

  float lowerFactor;

  if (list == NULL) return NULL ;
  highBlockPtr = crntBlockPtr;
  BTlistIterator highList( list );
  while ( (blockPtr = highList.Iterate()) != NULL )
  {
    if ( highBlockPtr == NULL )
      highBlockPtr = blockPtr;
    else
    {
      lowerFactor = 1.0;
       
      lowerFactor *= compareListForWord( highBlockPtr->transferOutput,
					 blockPtr->transferOutput  ) ;
	      
      lowerFactor *= compareListForWord(highBlockPtr->transferInput,
					blockPtr->transferInput ) ;

      if ( highBlockPtr->btEnergy  < (blockPtr->btEnergy * lowerFactor) )   
	highBlockPtr = blockPtr;
    }
  }
  if ( highBlockPtr != NULL )
    highBlockPtr->btEnergy = 0;
  return (BTcFaceBlock *) highBlockPtr;
}

//
// returns the best line given weighting relationships
// in a cface - from children
//

BTblock *BTpageBlock::findBestLeftLine(BTtrans *trans)
{
 // removed b4
 return NULL;
}

// given a cface line, returns a string of associated words

char *BTblock::lineToString()
{
  BTblock   *blockPtr;
  char           *newPhrase;
  char           *phrase;

  phrase = new char [256];
  *phrase = '\0';

  newPhrase = Name();
  if ( newPhrase != NULL )
  {
    //_fstrcat(phrase," ");
    _fstrcat(phrase,newPhrase);
  }
  else
  {
    BTlistIterator lineList( blockChildren );
    while ((blockPtr =  lineList.Iterate()) != NULL)
    {
      newPhrase = blockPtr->lineToString();
      //_fstrcat(phrase," ");
      
      if ( newPhrase != NULL )
      {
	_fstrcat(newPhrase," ");
	_fstrcat(phrase,newPhrase);
      }
      delete newPhrase;
      if (strlen(phrase) > 220 ) break ;
    }
  }

  return phrase;
}
