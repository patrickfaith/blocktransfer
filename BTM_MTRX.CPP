// BTm_mtrx.cpp - normal fixed point vector mathematics

#include <math.h>
#include <btdefs.h>
#include <btlist.h>
#include <btblocks.h>
#include <btgraph.h>
#include <btacmdef.h>
#include <btacmstr.h>
#include "btproto.h"

BTfix normalFix(double x)
{ 
  BTfix ret;
  if ( x > 1.0 )  x = 1.0;
  if ( x < -1.0 ) x = -1.0;
  
  ret = x*FLOAT_TO_FIX;
  return ret;
}
BTfix initSinF(double theta)
{
  double sinFloat;
  BTfix   sinFix ;
  sinFloat = sin ( theta * 2 * 3.14159 / 360.0 ) ;
  sinFix = normalFix(sinFloat);
  return sinFix; 
}
BTfix initCosF(double theta)
{
  double cosFloat;
  BTfix   cosFix ;
  cosFloat = cos ( theta * 2 * 3.14159 / 360.0 ) ;
  cosFix = normalFix(cosFloat);
  return cosFix; 
}


BTfix normalF(BTfix x)
{
  if ( x > MAX_FIX )
    x = MAX_FIX ;
  if ( x < MIN_FIX )
    x = MIN_FIX ;
  return x;
}

BTfix normalFAngle(BTfix x)
{
  if ( x > 255 )
  {
    x = x & 255;
  }
  else if ( x < 0 )
  {
    x = 255 + x ;
  }
  return x;
}
BTfix sinF(BTfix x)
{ 
  BTfix res;
  res = x & 255;
  res = BTax->sinFast[res];  
  return res;
}
BTfix cosF(BTfix x)
{
  BTfix res;
  res = x & 255;
  res = BTax->cosFast[res];  
  return res;
}
BTfix sawF(BTfix x)
{
  return x;
}
void ZeroMatrix(Matx4x4 &A)
{
  int i,j;

  for(i=0; i<4; i++)
  {
    for(j=0; j<4; j++)
      A[i][j]= 0 ;
  }
}
void ZeroVector(TDA &A )
{
  A[0] = 0;
  A[1] = 0;
  A[2] = 0;
}
void Translate3D(BTfix tx, BTfix ty, BTfix tz, Matx4x4 &A)
{
  int i;

  ZeroMatrix(A);
  for(i=0; i<4; i++)
    A[i][i]=FIX_ONE;
  A[0][3]=-tx;
  A[1][3]=-ty;
  A[2][3]=-tz;
}
void Scale3D(BTfix sx, BTfix sy, BTfix sz, Matx4x4 &A)
{
  ZeroMatrix(A);
  A[0][0]=sx;
  A[1][1]=sy;
  A[2][2]=sz;
  A[3][3]= FIX_ONE;
}
void Rotate3D(int m, BTfix Theta, Matx4x4 &A)
{
  int   m1,m2;
  BTfix c,s;

  ZeroMatrix(A);
  A[m-1][m-1]= FIX_ONE;
  A[3][3]= FIX_ONE;
  m1=(m % 3) + 1;
  m2=(m1 % 3);
  m1-=1;
  c = cosF(Theta);
  s = sinF(Theta);
  A[m1][m1]=c;
  A[m1][m2]=s;
  A[m2][m2]=c;
  A[m2][m1]= -s;
}
void Multiply3DMatricies(Matx4x4 &A, Matx4x4 &B, Matx4x4 &C)
{
  int   i,j,k;
  long ab;
  for(i=0; i<4; i++)
  {
    for(j=0; j<4; j++)
    {
      ab =  0 ;
      for(k=0; k<4; k++)
	    ab=ab + mulF(A[i][k],B[k][j]);
	  ab = normalF(ab);
      C[i][j]= (short) ab;
    }
  }
}
void MatCopy(Matx4x4 &a, Matx4x4 &b)
{
  Byte i, j;

  for(i=0; i<4; i++)
  {
    for(j=0; j<4; j++)
      b[i][j]=a[i][j];
  }
}
void PrepareMatrix(BTfix Tx, BTfix Ty, BTfix Tz,
		   BTfix Sx, BTfix Sy, BTfix Sz,
		   BTfix Rx, BTfix Ry, BTfix Rz,
		   Matx4x4 &XForm)
{
  Matx4x4 M1, M2, M3, M4, M5, M6, M7, M8, M9;

  Scale3D(Sx, Sy, Sz, M1);
  Rotate3D(1, Rx, M2);
  Rotate3D(2, Ry, M3);
  Rotate3D(3, Rz, M4);
  Translate3D(Tx, Ty, Tz, M5);
  Multiply3DMatricies(M2, M1, M6);
  Multiply3DMatricies(M3, M6, M7);
  Multiply3DMatricies(M4, M7, M8);
  Multiply3DMatricies(M5, M8, M9);
  MatCopy(M9, XForm);
}

void Transform(TDA &A, Matx4x4 &M, TDA &B)
{ 
  B[0]=normalF ( mulF(M[0][0],A[0]) +
       mulF(M[0][1],A[1]) +
       mulF(M[0][2],A[2]) +
       M[0][3] ) ;
  B[1]=normalF ( mulF(M[1][0],A[0]) +
       mulF(M[1][1],A[1]) +
       mulF(M[1][2],A[2]) + 
       M[1][3] );
  B[2]=normalF ( mulF(M[2][0],A[0])+
       mulF(M[2][1],A[1])+
       mulF(M[2][2],A[2])+
       M[2][3] );
}