///
///	test for stream functions
///

#include <fstream.h>
#include <strstrea.h>
#include <iomanip.h>
#include <string.h>
#include <btdefs.h>
#include <btlist.h>
#include <btblocks.h>

/// functions for the base class BTtrans

BTtrans2::BTtrans2()
{
   strm = new strstream();
   string = NULL;
   pagePtr = NULL;
}


BTtrans2::BTtrans2 ( char * inbuf )
{
   strm = new strstream(inbuf,strlen(inbuf),ios::app);
   string = inbuf;
   pagePtr = NULL;
}


char *BTtrans2::Word()
{
   char strng[132];
   char *word;

   if ( strm == NULL) return NULL ;

   if ( strm->eof() == 0  )
   {
      *strm >> strng ;
      word = strdup(strng);
   }
   else
   {
      word = NULL;
   }
   return (word);
}

void BTtrans2::openPort() { return;}

int BTtrans2::inPort() { return NULL;}

void BTtrans2::exPort()
{
   char strng[132];

   while ( strm != NULL )
   {
      *strm >> strng ;
      cout << strng << " ";
   }
   cout << "\n";
}

void BTtrans2::OutString(char *strng)
{
   char str[132];
   stpcpy(str,strng);
   cout << str << " "; return ;
}

/// functions for superclass Inf  - input file

void BTtransInf::BTtransInf(char *fileName) : BTtrans2()
{
   inf = new ifstream(fileName);
}

int BTtransInf::inPort()
{

   if ( inf == NULL )  return NULL;
   if ( inf->getline(inBuf,132) )
   {
      *strm << inBuf << "\n";
      return 1;
   }
   else
      return NULL;
}

/// functions for the superclass Outf


void BTtransOutf::BTtransOutf(char *fileName):BTtrans2()
{
   outf = new ofstream(fileName);
   if  ( !*outf ) cerr << "cannot open file \n";
}

void BTtransOutf::exPort()
{
   char ch;

   while ( outf && strm->get(ch) )
   {
      outf->put(ch);
   }
   return ;
}



