// BT3D.H: Header file for BT3D.CPP.
#ifndef THREEDH
#define THREEDH
#endif

#define PCL 0                // Borders of viewing region on
#define PCR 500             // the screen. These four variables
#define PCT 0                // correspond to the left, right,
#define PCB 500             // top, and bottom pixel boundaries
			     // of the viewing region.

// BTVECTR.H: Header file for VECTOR.CPP.

#ifndef VECTORH
#define VECTORH

struct VECTOR {
  float X, Y, Z;
};

//float Mag(VECTOR *V);
//VECTOR Subtract(VECTOR *V1, VECTOR *V2);
//VECTOR Cross(VECTOR *V1, VECTOR *V2);
//VECTOR Divide(VECTOR *V, float Num);
//void Normalize(VECTOR *V);
//float Dot(VECTOR *V1, VECTOR *V2);

#endif


// A three-dimensional viewing class
class TThreeD {
public:

  VECTOR  at;               // Viewing parameters

  float offsx, offsy, offsz;        // Transform variables

  TThreeD();
  void Display(HDC hDC, RECT& rect); // High level display routine

  void TransformSeg( VECTOR *v1, VECTOR *v2,
    POINT& pc1, POINT& pc2);
};
typedef TThreeD *PTThreeD;
