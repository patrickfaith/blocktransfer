// BTacmstr.h
//  structures for B4 accumulator
//

class BTcommandData
{
  public:
  char *name;
  int length;
  BTcommandData () { name = NULL; length=0; };
  short blockModes,transModes;
};

struct BTcmdDataExternal
{ 
  BTfix cosFast[256];
  BTfix sinFast[256];
  BTcommandData cmdData[MAX_NUM_COMMANDS];
  BTcommandData sysRegs[MAX_NUM_SYS_REGS];
};

class BTaccum // ulator
{
  public:
  BTlistIterator iter[ACCUM_REGISTER_TOT];
  BTblock	*registers[ACCUM_REGISTER_TOT];
  BTval	mathRegs[MAX_MATH_REGISTERS];
  char      *nameBuff; // simple string
  BTbookBlock *theBook;
  BTblock	*theBlock;
  
  BTaccum(BTbookBlock *bookPtr,BTblock *blockPtr);
  
  BTblock *MakeDnaBlock(BTcommandModes *modes,char *dnaString);
  BTblock *CalculateAddress(char reg, char mode);
  void CreateBlock(BTcommandModes  *cmdModes,
                          BTblock *cmdBlock,
                          int		      blockType);
                          
  int DeleteBlock( BTcommandModes *cmdModes );  

  BTblock *ParseCommand(char *text);
  
  int CommandSwitch(BTblock *block);

  void IterChild( BTcommandModes  *cmdModes );
  void IterParent( BTcommandModes  *cmdModes );
  void IterInput( BTcommandModes  *cmdModes );
  void IterOutput( BTcommandModes  *cmdModes );
  void IterNext( BTcommandModes  *cmdModes );
  void IterPrev( BTcommandModes  *cmdModes );
  void IterDel( BTcommandModes  *cmdModes );
  void IterAdd( BTcommandModes  *cmdModes );
  void IterReplace( BTcommandModes  *cmdModes ) ;
  void IterHigh( BTcommandModes  *cmdModes );
  void IterFind( BTcommandModes  *cmdModes );

  void CompareName( BTcommandModes  *cmdModes );
  
  int FileLoad( BTcommandModes *cmdModes );
  int FileSave( BTcommandModes *cmdModes );  
  
  void LoadString(BTblock *blockPtr, BTcommandModes  *cmdModes );
  BTblock *parseBlockLoad(char *buffer);
  void LoadBlock(BTblock *blockPtr, BTcommandModes  *cmdModes );
  void LoadPage(BTblock *blockPtr, BTcommandModes  *cmdModes );
  void LoadMath(BTblock *blockPtr, BTcommandModes  *cmdModes );
  void LoadNull(BTcommandModes  *cmdModes );

  void MathAdd( BTcommandModes  *cmdModes );  
  void MathSubtract( BTcommandModes  *cmdModes );
  void MathMultiply( BTcommandModes  *cmdModes );
   
  int MapSpiral( BTcommandModes  *cmdModes );
  int MapView( BTcommandModes  *cmdModes );
  int MapWindow( BTcommandModes  *cmdModes );
  int DisplayGraphics( BTcommandModes  *cmdModes );
  int CreateEdge( BTcommandModes  *cmdModes );
  int CreateFacet( BTcommandModes  *cmdModes );
    
  void MoveString( BTcommandModes  *cmdModes );
  void MoveEnergy( BTcommandModes  *cmdModes );
  void MoveBratio( BTcommandModes  *cmdModes );
  void MoveTratio( BTcommandModes  *cmdModes );
  void MoveWeight( BTcommandModes  *cmdModes );
  void MoveBlock( BTcommandModes  *cmdModes );
  
  int MoveSize( BTcommandModes  *cmdModes );
  int MoveTrans( BTcommandModes  *cmdModes );
  int MoveRotate( BTcommandModes  *cmdModes );
  int MoveScale( BTcommandModes  *cmdModes );
  int MoveGraph( BTcommandModes  *cmdModes, int i );
   
  int Translate(BTblock *block, BTcommandModes  *cmdModes );
  int Process(BTblock *block,BTcommandModes  *cmdModes);
  int ProcessMessage(BTblock *block,BTcommandModes  *cmdModes); 
  int breakDna( BTcommandModes  *cmdModes );
  int cycleDna( BTcommandModes  *cmdModes );
  int createAccum( BTcommandModes  *cmdModes );
 
} ;

extern struct BTcmdDataExternal *BTax;

