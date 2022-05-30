#include <btdefs.h>
#include <btlist.h>
#include <btblocks.h>
#include <btgraph.h>
#include <btacmdef.h>
#include <btacmstr.h>
#include "btproto.h"

void BTaccum::MathAdd( BTcommandModes  *cmdModes )
{ 
  BTval a=0,b=0; // result = a - b      
  if ( cmdModes->blockMode == MODE_FIXED )
    a = mathRegs[cmdModes->blockRegister];

  if ( cmdModes->transferMode == MODE_FIXED )
    b = mathRegs[cmdModes->transferRegister];
      
  if (cmdModes->resultMode == MODE_FIXED )
    mathRegs[cmdModes->resultRegister] = a+b;
  else
    mathRegs[cmdModes->blockRegister] = a+b;
  return ;
}
void BTaccum::MathSubtract( BTcommandModes  *cmdModes )
{ 
  BTval a=0,b=0; // result = a - b      
  if ( cmdModes->blockMode == MODE_FIXED )
    a = mathRegs[cmdModes->blockRegister];

  if ( cmdModes->transferMode == MODE_FIXED )
    b = mathRegs[cmdModes->transferRegister];
      
  if (cmdModes->resultMode == MODE_FIXED )
    mathRegs[cmdModes->resultRegister] = a-b;
  else
    mathRegs[cmdModes->blockRegister] = a-b;
  return ;
}
void BTaccum::MathMultiply( BTcommandModes  *cmdModes )
{ 
  BTval a=0,b=0; // result = a - b      
  if ( cmdModes->blockMode == MODE_FIXED )
    a = mathRegs[cmdModes->blockRegister];

  if ( cmdModes->transferMode == MODE_FIXED )
    b = mathRegs[cmdModes->transferRegister];
      
  if (cmdModes->resultMode == MODE_FIXED )
    mathRegs[cmdModes->resultRegister] = normalF(mulF(a,b));
  else
    mathRegs[cmdModes->blockRegister] = normalF(mulF(a,b));
  return ;
}