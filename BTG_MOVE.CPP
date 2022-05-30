// calling links into the accumulator for graphic stuff
#include <btdefs.h>
#include <btlist.h>
#include <btblocks.h>
#include <btgraph.h>
#include <btacmdef.h>
#include <btacmstr.h>
#include <btproto.h>
    
int BTaccum::MoveSize( BTcommandModes  *cmdModes )
{ 
  BTblock *blockPtr = NULL ;
  BTfix blkX=0,blkY=0,blkZ=0;
  BTspace *spac = NULL;
  switch (cmdModes->blockMode)
  { 
    case MODE_INDIRECT:
      blockPtr = iter[cmdModes->blockRegister].CurrentPtr();
      if (blockPtr == NULL )   return STATUS_BAD;
    case MODE_REGISTER:
      if (blockPtr == NULL )
        blockPtr = registers[cmdModes->blockRegister];
      if (blockPtr == NULL )   return STATUS_BAD;

      if (blockPtr->Type() == BT_TYPE_VIEW )
      { 
        BTWindow *graphWin = ((BTviewBlock *) blockPtr)->graphWin;
        blkX = graphWin->Width;
        blkY = graphWin->Height;      
      }
      else
      { 
        spac = blockPtr->SpacePtr();
        if ( spac == NULL ) return STATUS_BAD;
        blkX = spac->Scale[0];
        blkY = spac->Scale[1];
        blkZ = spac->Scale[2];
      }
      break ;
    case MODE_FIXED:
      blkX = mathRegs[cmdModes->blockRegister];
      blkY = mathRegs[cmdModes->blockRegister+1];
      break ;
  }
  switch (cmdModes->transferMode)
  { 
        case MODE_INDIRECT:
      blockPtr = iter[cmdModes->transferRegister].CurrentPtr();
      if (blockPtr == NULL )   return STATUS_BAD;
    case MODE_REGISTER:
      if (blockPtr == NULL )
        blockPtr = registers[cmdModes->transferRegister];
      if (blockPtr == NULL )   return STATUS_BAD;

      if (blockPtr->Type() == BT_TYPE_VIEW )
      { 
        BTWindow *graphWin = ((BTviewBlock *) blockPtr)->graphWin;
        graphWin->Width = blkX;
        graphWin->Height = blkY;      
      }
      else
      { 
        spac = blockPtr->SpacePtr();
        if ( spac == NULL ) return STATUS_BAD;
        spac->Scale[0] = blkX;
        spac->Scale[1] = blkY;
        spac->Scale[2] = blkZ;
      }      
      break ;
 
    case MODE_FIXED:
      mathRegs[cmdModes->transferRegister] = blkX;
      mathRegs[cmdModes->transferRegister+1] = blkY;
      break ;
  }
  return 1;
}

int BTaccum::MoveScale( BTcommandModes  *cmdModes )
{ 
  BTblock *blockPtr = NULL ;
  BTfix blkX=0,blkY=0,blkZ=0;
  BTspace *spac = NULL;
  switch (cmdModes->blockMode)
  { 
    case MODE_INDIRECT:
      blockPtr = iter[cmdModes->blockRegister].CurrentPtr();
      if (blockPtr == NULL )   return STATUS_BAD;
    case MODE_REGISTER:
      if (blockPtr == NULL )
        blockPtr = registers[cmdModes->blockRegister];
      if (blockPtr == NULL )   return STATUS_BAD;

      spac = blockPtr->SpacePtr();
      if ( spac == NULL ) return STATUS_BAD;
      blkX = spac->Scale[0];
      blkY = spac->Scale[1];
      blkZ = spac->Scale[2];
      
      break ;
    case MODE_FIXED:
      blkX = mathRegs[cmdModes->blockRegister];
      blkY = mathRegs[cmdModes->blockRegister+1];
      blkZ = mathRegs[cmdModes->blockRegister+2];
      break ;
  }
  switch (cmdModes->transferMode)
  { 
    case MODE_INDIRECT:
      blockPtr = iter[cmdModes->transferRegister].CurrentPtr();
      if (blockPtr == NULL )   return STATUS_BAD;
    case MODE_REGISTER:
      if (blockPtr == NULL )
        blockPtr = registers[cmdModes->transferRegister];
      if (blockPtr == NULL )   return STATUS_BAD;


      spac = blockPtr->SpacePtr();
      if ( spac == NULL ) return STATUS_BAD;
      spac->Scale[0] = blkX;
      spac->Scale[1] = blkY;
      spac->Scale[2] = blkZ;
          
      break ;
 
    case MODE_FIXED:
      mathRegs[cmdModes->transferRegister] = blkX;
      mathRegs[cmdModes->transferRegister+1] = blkY;
      mathRegs[cmdModes->transferRegister+2] = blkZ;
      break ;
  }
  return 1;
}

int BTaccum::MoveRotate( BTcommandModes  *cmdModes )
{ 
  BTblock *blockPtr;
  BTfix f1,f2,f3;
  BTspace *spac = NULL;
  switch (cmdModes->blockMode)
  {
    case MODE_REGISTER:
      blockPtr = registers[cmdModes->blockRegister];
      if ( blockPtr == NULL ) return STATUS_BAD;
      spac = blockPtr->SpacePtr();
      if ( spac == NULL ) return STATUS_BAD;
      f1 = spac->Rot[0];
      f2 = spac->Rot[1];
      f3 = spac->Rot[2];
      break ;
    case MODE_INDIRECT:
      blockPtr = iter[cmdModes->blockRegister].CurrentPtr();
      if ( blockPtr == NULL ) return STATUS_BAD;
      spac = blockPtr->SpacePtr();
      if ( spac == NULL ) return STATUS_BAD;
      f1 = spac->Rot[0];
      f2 = spac->Rot[1];
      f3 = spac->Rot[2];
      break ;
    case MODE_FIXED:
      f1 = mathRegs[cmdModes->blockRegister];
      f2 = mathRegs[cmdModes->blockRegister+1];
      f3 = mathRegs[cmdModes->blockRegister+2];
      break ;
  }
  switch (cmdModes->transferMode)
  {
    case MODE_REGISTER:
      blockPtr = registers[cmdModes->transferRegister];
      if ( blockPtr == NULL ) return STATUS_BAD;
      spac = blockPtr->SpacePtr();
      if ( spac == NULL ) return STATUS_BAD;
      spac->Rot[0] = normalFAngle(f1);
      spac->Rot[1] = normalFAngle(f2);
      spac->Rot[2] = normalFAngle(f3);
      
      PrepareMatrix(spac->Trans[0],spac->Trans[1],spac->Trans[2],
		   spac->Scale[0], spac->Scale[1], spac->Scale[2],
		   spac->Rot[0]  , spac->Rot[1],   spac->Rot[2],
		   spac->transform);
      
      break ;
    case MODE_INDIRECT:
      blockPtr = iter[cmdModes->blockRegister].CurrentPtr();
      if ( blockPtr == NULL ) return STATUS_BAD;
      spac = blockPtr->SpacePtr();
      if ( spac == NULL ) return STATUS_BAD;
      spac->Rot[0] = normalFAngle(f1);
      spac->Rot[1] = normalFAngle(f2);
      spac->Rot[2] = normalFAngle(f3);
      
      PrepareMatrix(spac->Trans[0],spac->Trans[1],spac->Trans[2],
		   spac->Scale[0], spac->Scale[1], spac->Scale[2],
		   spac->Rot[0]  , spac->Rot[1],   spac->Rot[2],
		   spac->transform);

      break ;
    case MODE_FIXED:
      mathRegs[cmdModes->transferRegister]   = normalFAngle(f1);
      mathRegs[cmdModes->transferRegister+1] = normalFAngle(f2);
      mathRegs[cmdModes->transferRegister+2] = normalFAngle(f3);
      break ;
  }
  return 1;
}

int BTaccum::MoveTrans( BTcommandModes  *cmdModes )
{ 
  BTblock *blockPtr;
  BTfix f1,f2,f3;
  BTspace *spac = NULL;
  switch (cmdModes->blockMode)
  {
    case MODE_REGISTER:
      blockPtr = registers[cmdModes->blockRegister];
      if ( blockPtr == NULL ) return STATUS_BAD;
      spac = blockPtr->SpacePtr();
      if ( spac == NULL ) return STATUS_BAD;
      f1 = spac->Trans[0];
      f2 = spac->Trans[1];
      f3 = spac->Trans[2];
      break ;
    case MODE_INDIRECT:
      blockPtr = iter[cmdModes->blockRegister].CurrentPtr();
      if ( blockPtr == NULL ) return STATUS_BAD;
      spac = blockPtr->SpacePtr();
      if ( spac == NULL ) return STATUS_BAD;
      f1 = spac->Trans[0];
      f2 = spac->Trans[1];
      f3 = spac->Trans[2];
      break ;
    case MODE_FIXED:
      f1 = mathRegs[cmdModes->blockRegister];
      f2 = mathRegs[cmdModes->blockRegister+1];
      f3 = mathRegs[cmdModes->blockRegister+2];
      break ;
  }
  switch (cmdModes->transferMode)
  {
    case MODE_REGISTER:
      blockPtr = registers[cmdModes->transferRegister];
      if ( blockPtr == NULL ) return STATUS_BAD;
      spac = blockPtr->SpacePtr();
      if ( spac == NULL ) return STATUS_BAD;
      spac->Trans[0] = f1;
      spac->Trans[1] = f2;
      spac->Trans[2] = f3;
      
      PrepareMatrix(spac->Trans[0],spac->Trans[1],spac->Trans[2],
		   spac->Scale[0], spac->Scale[1], spac->Scale[2],
		   spac->Rot[0]  , spac->Rot[1],   spac->Rot[2],
		   spac->transform);
      
      break ;
    case MODE_INDIRECT:
      blockPtr = iter[cmdModes->blockRegister].CurrentPtr();
      if ( blockPtr == NULL ) return STATUS_BAD;
      spac = blockPtr->SpacePtr();
      if ( spac == NULL ) return STATUS_BAD;
      spac->Trans[0] = f1;
      spac->Trans[1] = f2;
      spac->Trans[2] = f3;
      
      PrepareMatrix(spac->Trans[0],spac->Trans[1],spac->Trans[2],
		   spac->Scale[0], spac->Scale[1], spac->Scale[2],
		   spac->Rot[0]  , spac->Rot[1],   spac->Rot[2],
		   spac->transform);

      break ;
    case MODE_FIXED:
      mathRegs[cmdModes->transferRegister]   = f1;
      mathRegs[cmdModes->transferRegister+1] = f2;
      mathRegs[cmdModes->transferRegister+2] = f3;
      break ;
  }
  return 1;
}

int BTaccum::MoveGraph( BTcommandModes  *cmdModes, int i )
{ 
  BTblock *blockPtr;

  TDA pt;

  switch (cmdModes->blockMode)
  {
    case MODE_REGISTER:
      blockPtr = registers[cmdModes->blockRegister];
      if ( blockPtr == NULL ) return STATUS_BAD;
      if ( blockPtr->graph == NULL ) return STATUS_BAD;
      blockPtr->graph->GetPosInPage( i,pt);
      break ;
    case MODE_INDIRECT:
      blockPtr = iter[cmdModes->blockRegister].CurrentPtr();
      if ( blockPtr == NULL ) return STATUS_BAD;
      if ( blockPtr->graph == NULL ) return STATUS_BAD;
      blockPtr->graph->GetPosInPage( i,pt);
      break ;
    case MODE_FIXED:
      pt[0] = mathRegs[cmdModes->blockRegister];
      pt[1] = mathRegs[cmdModes->blockRegister+1];
      pt[2] = mathRegs[cmdModes->blockRegister+2];
      break ;
  }
  switch (cmdModes->transferMode)
  {
    case MODE_REGISTER:
      blockPtr = registers[cmdModes->transferRegister];
      if ( blockPtr == NULL ) return STATUS_BAD;
      if ( blockPtr->graph == NULL ) return STATUS_BAD;
      blockPtr->graph->SetPosInPage( i,pt);     
      break ;
    case MODE_INDIRECT:
      blockPtr = iter[cmdModes->blockRegister].CurrentPtr();
      if ( blockPtr == NULL ) return STATUS_BAD;
      if ( blockPtr->graph == NULL ) return STATUS_BAD;
      blockPtr->graph->SetPosInPage( i,pt); 
      break ;
    case MODE_FIXED:
      mathRegs[cmdModes->transferRegister]   = pt[0];
      mathRegs[cmdModes->transferRegister+1] = pt[1];
      mathRegs[cmdModes->transferRegister+2] = pt[2];
      break ;
  }
  return 1;
}


 