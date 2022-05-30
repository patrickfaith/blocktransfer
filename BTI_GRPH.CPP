#include <btdefs.h>
#include <btlist.h>
#include <btblocks.h>
#include <btgraph.h>
#include <btproto.h>

BTspace::BTspace()
{
  Trans[0] = 0; Trans[1]=0; Trans[2] = 0;
  Scale[0] = FIX_ONE;
  Scale[1] = FIX_ONE;
  Scale[2] = FIX_ONE;
  Rot[0] = 0 ; Rot[1] = 0 ; Rot[2] = 0;

  Scale3D(Scale[0], Scale[1], Scale[2], transform) ;
}
BTplace::BTplace()
{
  // pos[0] = theta, [1] = z, [2] = radius
  // shape[0] = theta speed, shape[1] = Z speed , shape[2] = gap
  // curve[0] = curviness, curve[1] = frequency factor of curve
  // curve[2] = rate of slope change
  // calculate 1st point of block with new theta
  pos[0] = 0;
  pos[1] = 256;
  pos[2] = FIX_ONE ;
  shape[0] = 7; // was 7
  shape[1] = 0; // was 128
  shape[2] = 1024;
  curve[0] = 0;
  curve[1] = 0;
  curve[2] = 0;
}