// simple search for string block - so far can't be a page.

/*#include <stdio.h>
#include <fstream.h>
#include <strstrea.h>
#include <iomanip.h>
*/
#include <string.h>
#include <btdefs.h>
#include <btlist.h>
#include <btblocks.h>

BTblock * BTblock::SearchForWord(char *word)
{
  return NULL ; // standard blocks don't do anything
}


BTblock *BTstringBlock::SearchForWord(char *word)
{

  if (strcmpi(word,Name()) == 0 )
    return this;
  else
    return NULL;
}

BTblock *BTpageBlock::SearchForWord(char *word)
{
  BTblock	*blockPtr, *foundBlock ;

  if ( word == NULL ) return NULL;
  BTlistIterator pageHead( blockChildren );

  while ( (blockPtr = pageHead.Iterate()) != NULL )
  {
    foundBlock = blockPtr->SearchForWord(word);
    if (foundBlock != NULL ) return foundBlock;
  }
  return NULL;
}

BTblock *BTbookBlock::SearchForWord(char *word)
{
  BTblock	*blockPtr, *foundBlock ;

  if ( word == NULL ) return NULL;
  BTlistIterator pageHead( blockChildren );

  while ( (blockPtr = pageHead.Iterate()) != NULL )
  {
    foundBlock = blockPtr->SearchForWord(word);
    if (foundBlock != NULL ) return foundBlock;
  }
  return NULL;
}