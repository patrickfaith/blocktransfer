#include <string.h>
#include <btdefs.h>
#include <btlist.h>
#include <btblocks.h>
#include <btgraph.h>
#include <btacmdef.h>
#include <btacmstr.h>
#include "btproto.h"

void BTaccum::IterChild( BTcommandModes  *cmdModes )
{ 
  int reg = cmdModes->blockRegister ;
  if ( registers[reg] == NULL ) return ;
  BTlistHnd listHnd;
  listHnd = & (registers[reg]->blockChildren);
  iter[reg].Reset(listHnd);
  return ;
}
void BTaccum::IterParent( BTcommandModes  *cmdModes )
{ 
  int reg = cmdModes->blockRegister ;
  if ( registers[reg] == NULL ) return ;
  BTlistHnd listHnd;
  listHnd = & (registers[reg]->blockParents);
  iter[reg].Reset(listHnd);
  return ;
}
void BTaccum::IterInput( BTcommandModes  *cmdModes )
{ 
  int reg = cmdModes->blockRegister ;
  if ( registers[reg] == NULL ) return ;
  BTlistHnd listHnd;
  listHnd = & (registers[reg]->transferInput);
  iter[reg].Reset(listHnd);
  return ;
}
void BTaccum::IterOutput( BTcommandModes  *cmdModes )
{ 
  int reg = cmdModes->blockRegister ;
  if ( registers[reg] == NULL ) return ;
  BTlistHnd listHnd;
  listHnd = & (registers[reg]->transferOutput);
  iter[reg].Reset(listHnd);
  return ;
}
void BTaccum::IterNext( BTcommandModes  *cmdModes )
{
  iter[cmdModes->blockRegister].Iterate();
  return ;
}
void BTaccum::IterPrev( BTcommandModes  *cmdModes )
{
  iter[cmdModes->blockRegister].IterateLast();
  return ;
}

void BTaccum::IterDel( BTcommandModes  *cmdModes )
{
  iter[cmdModes->blockRegister].erase();
  return ;
}

void BTaccum::IterAdd( BTcommandModes  *cmdModes )
{                         
  BTblock *blkPtr = CalculateAddress(cmdModes->transferRegister,
                                     cmdModes->transferMode);
  iter[cmdModes->blockRegister].add( blkPtr );
  return ;
}

void BTaccum::IterReplace( BTcommandModes  *cmdModes )
{                         
  BTblock *blkPtr = CalculateAddress(cmdModes->transferRegister,
                                     cmdModes->transferMode);
  iter[cmdModes->blockRegister].ReplaceCurrentAtom(blkPtr);
  return ;
}

void BTaccum::IterHigh( BTcommandModes  *cmdModes )
{ 
  BTlist *list = iter[cmdModes->blockRegister].Head();                        
  registers[cmdModes->transferRegister] = AnyBlockHighList(list);
  return ;
}

void BTaccum::IterFind( BTcommandModes  *cmdModes )
{ 
  BTblock *blkPtr = CalculateAddress(cmdModes->transferRegister,
                                     cmdModes->transferMode);
  iter[cmdModes->blockRegister].FindAtom(blkPtr) ;
  return ;
}

