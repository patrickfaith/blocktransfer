#include <btdefs.h>
#include <btlist.h>
#include <btblocks.h>
#include <btgraph.h>
#include <btproto.h>

BTdnaBlock::~BTdnaBlock()
{
  delete string;  
  delete cmdModes;  
}