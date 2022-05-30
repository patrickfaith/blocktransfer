// BTgraph.h
class BTgraph
{
  public:
  BTgraph *next;
  TDA  bookPos1; // points after mapped by the page transform
  TDA  bookPos2;
  TDX  bookPix1; // pixel location of the line
  TDX  bookPix2;
  TDA  pagePos1;
  TDA  pagePos2;
  TDA  zox;
  TDA  color;
  BTgraph();
  
  virtual int Type() { return BT_LINE_GRAPH ; } ;
  virtual void SetPosInPage( TDA Pt1, TDA Pt2);
  virtual void SetPosInPage( int i, TDA& Pt);
  virtual void GetPosInPage( int i, TDA& Pt);
  virtual void DisplayGraphics(BTWindow *wndw);
  virtual void PageToBook(BTspace *spacePtr);
  virtual void BlocksToPixels(BTviewBlock *view);
  void PlaceGraph(BTgraph *graph ) ;

  virtual int Facets() { return 2 ; } ;

};

class BTchipGraph : public BTgraph
{
  public:
  
  TDA  bookPos3; // points after mapped by the page transform
  TDA  bookPos4;
  TDX  bookPix3; // pixel location of the line
  TDX  bookPix4;
  TDA  pagePos3;
  TDA  pagePos4;
  
  virtual int Type() { return BT_SQUARE_GRAPH ; } ;
  virtual void SetPosInPage( int i, TDA& Pt);
  virtual void GetPosInPage( int i, TDA& Pt);
  virtual int Facets() { return 4 ; } ;
  virtual void PageToBook(BTspace *spacePtr);
  virtual void BlocksToPixels(BTviewBlock *view);
  virtual void DisplayGraphics(BTWindow *wndw);  
}; 
class BTbmpGraph : public BTgraph
{
  public:
  char displayType;
  char mapType;
  short width,heigth;
  char bmpName[10];
  BTbmpGraph():BTgraph(){};  // initialization - basic
  virtual int Type() { return BT_BMP_GRAPH; } ;
};
// spatial initialization and placement object functions
class BTplace
{
  public:
  TDA	pos;
  TDA	shape;
  TDA	curve;
  BTplace(); 
  virtual short Type() { return 1;};
  virtual void PlaceGraph(BTgraph *graf);
} ;
