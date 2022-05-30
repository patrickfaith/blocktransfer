/*
#include <fstream.h>
#include <strstrea.h>
#include <iomanip.h>
*/
#include <string.h>
#include <stdio.h>
#include <btdefs.h>
#include <btlist.h>
#include <btblocks.h>
BTtransFile::BTtransFile(char *name) : BTblock()
{
  inf = NULL;
  outf = NULL;
  file = name;
  *inBuf = '\0';
}
BTtransFile::BTtransFile(BTblock *trans) : BTblock()
{
  inf = NULL;
  outf = NULL;
  file = NULL;

  *inBuf = '\0';

  if (trans->Name() != NULL )
  {
    file = trans->Name();

//    if ( trans->Type() == BT_TYPE_TRANS_IN )
//      openInPort();
    if ( trans->Type() == BT_TYPE_TRANS_OUT )
      openExPort();
  }
}

void BTtransFile::openInPort()
{
  if ( file != NULL )
    inf = fopen(file,"r");
  return;
}

void BTtransFile::openExPort()
{
  if ( file != NULL )
    outf =  fopen(file,"w"); // new ofstream(file);
  return;
}

void BTtransFile::exPort(BTtrans *trans, char *data)
{
  if ( outf == NULL )
  {
    strcpy(inBuf,data);
    return ;
  }
 
  fputs(data,outf) ;

  return ;
}


char *BTtransFile::inPort(BTtrans *trans)
{

  char *str = NULL;

  if ( inf == NULL )
  {
    str = new char [ strlen(inBuf)+ 1];
    strcpy(str,inBuf);
    *inBuf = '\0';
    return str;
  }

  // read in buffer if it is empty
  if ( strlen(inBuf) == 0 )
  {
    fgets(inBuf,131,inf) ; //       ->getline(inBuf,131);
  }
  if ( strlen(inBuf) != 0 )
  {
    str = new char [ strlen(inBuf)+ 1];
    strcpy(str,inBuf);
  }
  else // if buffer is still empy return a null string
  {
    str = new char [1];
    *str ='\0';
  }

  *inBuf = '\0';
  return str;

}

void BTtransFile::closePort()
{
  if ( outf != NULL )
  {
    fclose(outf) ;
    outf = NULL;
  }
  if ( inf != NULL )
  {
    fclose(inf);
    inf = NULL;
  }
  return;
}

void BTtransFile::OutString(char *strng)
{
  // cout << strng; removed this when took out >> crap
}


