// copyright (c) p l faith
//
// math routines prototypes
//

BTfix normalFix(double x);
BTfix initSinF(double theta);
BTfix initCosF(double theta);
// inline long mulF(BTfix x, BTfix y );
// inline BTfix normalF(long x);
BTfix normalF(BTfix x);
BTfix normalFAngle(BTfix x);
BTfix sinF(BTfix x);
BTfix cosF(BTfix x);
BTfix sawF(BTfix x);
void ZeroMatrix(Matx4x4 &A);
void ZeroVector(TDA &A );
void Translate3D(BTfix tx, BTfix ty, BTfix tz, Matx4x4 &A);
void Scale3D(BTfix sx, BTfix sy, BTfix sz, Matx4x4 &A);
void Rotate3D(int m, BTfix Theta, Matx4x4 &A);
void Multiply3DMatricies(Matx4x4 &A, Matx4x4 &B, Matx4x4 &C);
void MatCopy(Matx4x4 &a, Matx4x4 &b);
void PrepareMatrix(BTfix Tx, BTfix Ty, BTfix Tz,
		   BTfix Sx, BTfix Sy, BTfix Sz,
		   BTfix Rx, BTfix Ry, BTfix Rz,
		   Matx4x4 &XForm) ;
void Transform(TDA &A, Matx4x4 &M, TDA &B);
// pixel stuff
void VecToPixel(Matx4x4 &mat,TDA &vec,TDA &zox,TDX &pix,BTWindow *wndw) ;

// btpaging.c

void BlockPaging(BTlist *list,BTblock *book) ;

// btpgrght.c

BTblock *SaveCFaceChunk(BTblock *trans,BTcFaceBlock *chunk);

// mdi somewhere

HPEN colorNewPen(float energy, BTgraph *graph);

void OutText(char *strng);

// btcfcfix.c

BTblock *BlockHighList(BTlist *list) ;
BTblock *AnyBlockHighList(BTlist *list) ;

// btbots.c

void BotsCommand(char *buffer, BTbookBlock *theBook) ;

// btcommnds.c

void SellectCommand(char *buffer, BTbookBlock *theBook) ;
void MoveCommand(char *buffer, BTbookBlock *theBook) ;
void DisplayEveryThing();
void DisplayTheFocus();

// btcomblk.c
void ExeCommand(char *buffer, BTbookBlock *theBook) ;
void DelCommand(char *buffer, BTbookBlock *theBook);

// btcom_io.c

void InputCommand(char *buffer, BTbookBlock *theBook) ;
void OutputCommand(char *buffer, BTbookBlock *theBook);
void DispCommand(char *buffer, BTbookBlock *theBook);

// btdispfc.c

void DisplayFacade(HDC hDC, BTblock *block, BTgraph *graph) ;

void LoadFacadePallette( BTblock *block, BTgraph *graph) ;
// btdispla.c
HANDLE ActivatePallete(HDC hDC, BTbookBlock *theBook);
HANDLE SetupPallete() ;
//void DisplayGraphic(HDC hDC, float btEnergy, BTgraph *graph);
void DisplayBlockTransfers(HDC hDC, BTblock *chunkBlk);
void DisplayText(HDC hDC, float btEnergy, BTgraph *graph, char *text);

// btcomfac.c

void FacadeCommand(char *buffer, BTbookBlock *theBook);
void DeactivateCommand(char *buffer, BTbookBlock *theBook); 

// 
BTviewBlock *FindView(HWND hWnd) ;
BTviewBlock *NameView(char *name);
BTcommandModes *ParseCommandMode(char *text);
BTblock *BotsCommandText(char *buffer,BTaccum *accum);
void initCommandData();
void testCrap();
int ProcessDNACommandBuffer(BTblock *block, char *buffer);

// storage routines
void SaveGraphicalObjects(BTtrans *trans, BTblock *block);
void LoadGraphicalObjects(BTtrans *trans, BTblock *block);
void CommandString(char *sent, BTdnaBlock *blockPtr);

BTdnaBlock *ParseDnaCommand(BTaccum *accum, BTdnaBlock *block, char *text);

int ProcessDDEexecute(HWND hWnd, HWND client, BTblock *block, char *buffer);
char *DuplicateName2(char *namePtr,char *newNamePtr);

int StringToBlocks(char *strng, BTblock *newGroup, BTblock *wordPagePtr);
// some wierd kernal and button stuff

BTblock *KernalRegisterGet(BTaccum *accum, int reg );
void KernalRegisterSet(BTaccum *accum, int reg, BTblock *block);

void LeftButtonUp( HWND hWnd, WPARAM wParam, LPARAM LParam);
void LeftButtonDown(HWND hWnd, WPARAM wParam, LPARAM LParam);
void RightButtonUp(HWND hWnd, WPARAM wParam, LPARAM LParam);
void RightButtonDown(HWND hWnd, WPARAM wParam, LPARAM LParam);
inline long mulF(BTfix x, BTfix y )
{ 
  long xl = x;
  long yl = y;
  long r = (xl * yl ) >> 13 ;
  return r ;
}

inline BTfix normalF(long x)
{
  if ( x > MAX_FIX )      { x = MAX_FIX ; }
  else if ( x < MIN_FIX ) { x = MIN_FIX ; }
  else                    { x = (int) x; }
  return (short) x;
}