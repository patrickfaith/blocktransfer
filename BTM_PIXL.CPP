#include <math.h>
#include <BTdefs.h>
#include <btlist.h>
#include <btblocks.h> 
#include <btgraph.h>
#include <btproto.h>

// wndw[0] = #x pixels, wndw[1] = #y pixels, wndw[2] = fixed point scaler
// pix[0] = -1 is vector not mapped
void VecToPixel(Matx4x4 &mat, TDA &vec, TDA &zox, TDX &pix,BTWindow *wndw)
{ 
  BTfix sFactor;
  Transform ( vec , mat , zox );

  if ( ( zox[2] > FIX_ONE ) || ( zox[2] < FIX_ONE_MINUS ) )
  {
    pix[0] = -1;
  }
  else
  { 

   // zox[2] += 3*FIX_ONE; // z value perspecive junk
   // sFactor = mulF(zox[2],wndw->scaler) ; // 2 holds conversion and scaler

    sFactor = wndw->scaler * 6 ; // normal projection
    pix[0] = (BTfix) mulF(zox[0],sFactor); // pix not fixed point
    pix[0] += (wndw->Width>>1);  // 0 holds the pixels of window width 
    if ( ( pix[0] < 0 ) || ( pix[0] > wndw->Width ) ) 
    {
      pix[0] = -1 ;
      return ;
    }
    pix[1] = (BTfix) mulF(zox[1],sFactor); // pix not fixed point
    pix[1] = (wndw->Height>>1) - pix[1];  
    if ( ( pix[1] < 0 ) || ( pix[1] > wndw->Height ) )
    {
      pix[0] = -1 ;
      return ;
    }
  }    
}
void BTplace::PlaceGraph(BTgraph *graph )
{ 
  // pos[0] = theta, [1] = z, [2] = radius
  // shape[0] = theta gap, shape[1] = Z gap , shape[2] = r gap
  // curve[0] = curviness, curve[1] = frequency factor of curve
  // curve[2] = rate of slope change
  // calculate 1st point of block with new theta
  TDA P;

  if (graph == NULL) return ;
  
  BTfix z0,z1,r0,r1,theta0,theta1;

  theta0 = pos[0] + shape[0] ; 
  theta1 = theta0 + shape[0] ;
  
  z0 = pos[1] + shape[1];
  z1 = z0;
  
  // radius calculations
  //if ((curve[0] == 0) && (curve[1] == 0 )) // straight column
  //{
  r0 = pos[2] - shape[2] ;
  r1 = pos[2];
  /*}
  else if ((curve[0] == 0) && (curve[1] != 0 )) // straight cone
  {
    r0 = pos[2];
    r1 = sawF(z1/curve[1]) ;  
  }
  else if ((curve[0] != 0) && (curve[1] != 0 )) // straight cone
  {
    r0 = pos[2];
    r1 = cosF(z1/curve[1]) ;
  }
  */

  // bottem left
  P[0] = normalF(mulF(cosF(theta0),r0));
  P[1] = normalF(mulF(sinF(theta0),r0));
  P[2] = z0;
  graph->SetPosInPage(1,P);
  // bottem right
  P[0] = normalF(mulF(cosF(theta1),r0));
  P[1] = normalF(mulF(sinF(theta1),r0));
  P[2] = z0;
  graph->SetPosInPage(2,P);
  // top right
  if ( graph->Facets() > 3 )
  {
    P[0] = normalF(mulF(cosF(theta1),r1));
    P[1] = normalF(mulF(sinF(theta1),r1));
    P[2] = z1;
    graph->SetPosInPage(3,P);
    // top left
    P[0] = normalF(mulF(cosF(theta0),r1));
    P[1] = normalF(mulF(sinF(theta0),r1));
    P[2] = z1;
    graph->SetPosInPage(4,P);
  }
  pos[0] = theta1;
  pos[1] = z1;
  return;
}