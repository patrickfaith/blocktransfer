#include <btdefs.h>
#include <btlist.h>
#include <btblocks.h>
#include <btgraph.h>
#include <btacmdef.h>
#include <btacmstr.h>
#include <btproto.h>

BTcommandModes  *BTblock::CommandModes(BTcommandModes  *cmds)
{
  return NULL;
}
BTcommandModes  *BTchunkBlock::CommandModes(BTcommandModes  *cmds)
{
  return NULL;
/*  if (cmds != NULL ) 
  {
    delete cmds;
    return NULL;
  }
  char *lineStrng;
  lineStrng = lineToString();
  cmds = ParseCommandMode(lineStrng);
  if (lineStrng != NULL )
    delete lineStrng;
  return cmds;
 */
}
BTcommandModes  *BTdnaBlock::CommandModes(BTcommandModes  *cmds)
{
  return cmdModes;
}