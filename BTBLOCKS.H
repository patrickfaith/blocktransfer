// copyright (c) p l faith 1993 all rights reserved
// primary file for defining block & transfer structure
#ifndef _WINDOWS_H
#include <windows.h>
#endif
//#ifndef THREEDH
//#include <bt3d.h>
//#endif
#ifndef __STDIO_H
#include <stdio.h>
#endif
class BTaccum;
class BTWindow;
class BTblock;
class BTbookBlock;
class BTpageBlock;
class BTmap;
class BTgraph;
class BTbmpGraph;
class BTplace;
class BTcFaceBlock;
class BTcommandModes;
#include <btbuff.h>
class BTviewBlock ;
#ifndef __BTWINSTRC_H
#include <btwnstrc.h>
#endif
//typedef float BTval;
struct pixStruc
{
  int x;
  int y;
  int closest;
  int closestZ;
  BTblock *bestBlock;
} ;
class BTspace
{ 
  public:
  TDA    Trans;
  TDA    Scale; 
  TDA    Rot;
  Matx4x4  transform;
  BTspace();
};
class	BTblock
{
  public:
  BTpageBlock	*pagePtr;  // pointer to blocks page
  short			blockId;   // counter of the block
  char			deActivate;
  BTgraph		*graph;
  BTlist  		*blockParents,
				*blockChildren,
				*transferInput,
				*transferOutput;
  BTfix			btEnergy;
  BTfix		    blockRatio;
  BTfix			transferRatio;
  // creation & destruction functions
  BTblock();
  BTblock(BTtrans *trans);
  BTblock(BTtrans *trans, short blkId);
  virtual ~BTblock();
  // primary block and transfer routines
  virtual void block(BTblock *trans);
  virtual void transfer(BTblock *trans);
  virtual short compare(BTtrans *trans);
  virtual BTval transferType(){ return 1.0 ; } ;  // default always transfer
  virtual BTval blockType(){ return 1.0 ; } ;   // default always block
  // routines to initially process events
  virtual void transInRoute(BTblock *trans){ return ; };
  virtual void transOutRoute(BTblock *trans){ return ; };
  virtual void Execute(BTblock *trans){ return ; };
  virtual BTblock* createBlock(char *pageName,short blkId,long blkType);
  // type of block & identifiers
  short Id() { return blockId ; } ;
  virtual void  SetNextBlockId(short blkId){return;};
  virtual short NextBlockId(BTtrans *trans);
  virtual long Type() { return BT_TYPE_BASIC; } ;
  virtual char *Name() { return NULL ; } ;
  virtual void  Name(char *str){};
  virtual short Loaded();
  virtual char *pageName();
  virtual BTbookBlock *BookPtr();
  virtual BTaccum *AccumPtr();
  virtual BTplace   *PlacePtr() { return NULL ;} ;
  virtual BTspace   *SpacePtr() { return NULL ;} ;
  virtual BTspace   *SpacePtr(BTspace *ptr) { return NULL ;} ;
  // functions used to add/delete blocks to the list
  virtual void addParent(BTblock *blkPtr);
  virtual void addChild(BTblock *blkPtr);
  virtual void addInput(BTblock *blkPtr);
  virtual void addOutput(BTblock *blkPtr);
  virtual void removeParent(BTblock *blkPtr);
  virtual void removeChild(BTblock *blkPtr);
  virtual void removeInput(BTblock *blkPtr);
  virtual void removeOutput(BTblock *blkPtr);
  // functions used to set and get block properties
  virtual void BTblock::blockBlockEnergy(BTfix energy);
  virtual void BTblock::blockTransferEnergy(BTfix energy);
  virtual BTfix BTblock::transferBlockEnergy();
  virtual BTfix BTblock::transferTransferEnergy();
  virtual BTval Time(){return 0;};
  virtual void  SetTime(BTval val){};
  virtual void SetId(short blkId){ blockId =blkId; return;};
  virtual void SetString(char *str){ return;};
  virtual void SetPagePtr(BTpageBlock* ptr){pagePtr = ptr;return;}
  virtual void writeString(BTtrans *trans);

  virtual void OutString(char *strng){return;};
  virtual char *Word(){return NULL;};
  virtual void openPort(){return;};
  virtual void closePort(){return;};
  virtual BTblock *TriggerBlock(short triggerType);
  virtual void exPort(BTtrans *trans){return;};
  virtual void exPort(BTtrans *trans, char *data) {return;};
  virtual char *inPort(BTtrans *trans ) { return NULL; };
  // functions used to move blocks beteens pages/streams
  virtual void sendId(BTtrans *trans);
  virtual void sendEvent(BTtrans *trans);
  virtual void sendParents(BTtrans *trans);
  virtual void sendChildren(BTtrans *trans);
  virtual void sendTransInput(BTtrans *trans);
  virtual void sendTransOutput(BTtrans *trans);
  virtual void sendUnique(BTtrans *trans){return;}; // unique block data
  virtual void sendAsciiStream(BTtrans *trans, char *data){return;};
  virtual void sendStream(BTtrans *trans);
  virtual BTblock* receiveId(BTlist **listPtr, BTtrans *trans);
  virtual void receiveEvent(BTtrans *trans);
  virtual void receiveParents(BTtrans *trans);
  virtual void receiveChildren(BTtrans *trans);
  virtual void receiveTransInput(BTtrans *trans);
  virtual void receiveTransOutput(BTtrans *trans);
  virtual void receiveUnique(BTtrans *trans){return;}; // unique block data
  virtual void sendBlockAscii(BTtrans *trans);
  virtual void receiveBlockAscii(BTtrans *trans);
  // BTcfcgen.c
  virtual void transferBlockToCface(BTtrans *trans);
  virtual void transferTransferGetToCface(BTtrans *trans);
  virtual void transferTransferConnectToCface(BTtrans *trans);
  // BTratio.c
  void DetermineBlockRatio();
  void DetermineTransferRatio( BTblock *initPtr, BTblock *resultPtr) ;
  BTblock *ResultConnection(BTblock *initPtr,
			    BTblock *resultPtr,
			    BTblock *trans);
  virtual void makeWeightList(BTtrans *trace);  // btweight.c
  virtual BTblock *SearchForWord(char *word); // BTsrchwr.c
  virtual void RelaxNetwork(BTval melow) {} ; // BTrest.c
  virtual void FocusNetwork(BTval melow) {} ; // BTfocus.c
  virtual void DeFocusNetwork(BTval melow) {} ; // BTfocus.c
  void setPtrId( BTlist *listItem) ; // btblocks
  void NormalizeNetwork(){}; //btfocus.c
  void NormalizeEnergy(){}; //btfocus.c
  // graphics crap
  void AddTransGraph(BTgraph *grph);   // btgraph.c
  void AddBlockGraph(BTgraph *grph);
  void DeleteGraph(BTgraph *grph);
  void DeleteGraph();
  BTgraph *ReturnBlockGraph() ;
  virtual void PageToBook(BTtrans *trans);
  virtual void BlocksToPixels(BTviewBlock *view);
  virtual void PlaceGraph(BTtrans *trans);
  virtual void DisplayGraphics(BTWindow *wndw);
  virtual char *lineToString();  // cfix
  virtual BTblock *PixelToBlock( pixStruc *pix );
  // buffer routines
  virtual  BTbuff *BuffPtr(){return NULL;};
  virtual  BTbuff *BuffPtr(BTbuff *ptr){return NULL;};
  virtual  BTblock *Internal(){return NULL;};
  virtual  BTblock *Internal(BTblock *ptr){return NULL;};
  
  virtual BTcommandModes  *CommandModes(BTcommandModes  *cmds); 
};
class	BTgroupBlock : public BTblock
{
  public: 
  BTgroupBlock(BTtrans *trans) : BTblock(trans) {};
  BTgroupBlock(BTtrans *trans,short blkId):BTblock(trans,blkId){};
  virtual long Type() { return BT_TYPE_GROUP; } ;
  virtual void writeString(BTtrans *trans);
};
class	BTchunkBlock : public BTblock
{
  public:
  char phase;
  BTchunkBlock(BTtrans *trans,short blkId):BTblock(trans,blkId){};
  BTchunkBlock(BTtrans *trans):BTblock(trans){};
  virtual void addChild(BTblock *blkPtr);
  virtual long Type() { return BT_TYPE_CHUNK; } ;
  virtual void block(BTtrans *trans);
  virtual void transferBlockToCface(BTtrans *trans);
  virtual BTcommandModes  *CommandModes(BTcommandModes  *cmds); 
};

class	BTstringBlock : public BTblock
{
  public:
  char *string;
  virtual long Type() { return BT_TYPE_STRING; } ;
  BTstringBlock(BTtrans *trans,short blkId):BTblock(trans,blkId){};
  BTstringBlock(char *strng) : BTblock() { string = strng ; };
  BTstringBlock(char *strng, BTtrans *trans) : BTblock(trans)
     { string = strng ; };
  virtual char *Name() { return string ; } ;
  virtual void  Name(char *str){string = str;};
  virtual void writeString(BTtrans *trans);
  virtual void sendUnique(BTtrans *trans);
  virtual void receiveUnique(BTtrans *trans);
  void  inputString();
  virtual BTblock *SearchForWord(char *word); // BTsrchwr.c
  virtual void exPort(BTtrans *trans, char *data);
};
class	BTreadBlock : public BTblock
{
  public:
  BTreadBlock():BTblock(){};
  BTreadBlock(BTtrans *trans,short blkId):BTblock(trans,blkId){};
  virtual long Type() { return BT_TYPE_READ; } ;
  virtual char *inPort(BTtrans *trans);
  virtual void exPort(BTtrans *trans);
};
class	BTpageBlock : public BTblock
{
  public:
  short  		nextBlockId;
  BTbookBlock	*bookPtr;
  short			loaded;
  char 			*name;		// name of the page , which is file name
  BTgraph		*facade;
  BTplace		*place; 	// figures out where new blocks are stuck
  BTspace       *space;
  BTaccum 		*accum;
  
  int			channelType;
  // BTpageBlock(BTtrans *trans,short blkId):BTblock(trans,blkId){};
  BTpageBlock( BTtrans *trans, char *strng) ;
  BTpageBlock() : BTblock(){ name = NULL ; }; 
  virtual ~BTpageBlock();
  virtual short Loaded() { return loaded ; } ;
  virtual BTbookBlock *BookPtr();
  virtual BTplace   *PlacePtr() { return place ; } ;
  virtual BTspace   *SpacePtr() { return space ;} ;
  virtual BTaccum *AccumPtr(){return accum;};
  virtual BTspace   *SpacePtr(BTspace *ptr) {space=ptr;return space;};
  virtual short NextBlockId(BTtrans *trans);
  virtual void  SetNextBlockId(short blkId){ nextBlockId +=1;return;};
  virtual char *Name() { return name ; } ;
  virtual char *pageName() { return name; };
  virtual void  Name(char *str){name = str;};
  virtual long Type() { return BT_TYPE_PAGE; } ;
  // not used virtual void sendStream(BTtrans *trans);
  virtual BTWindow *WindowPtr(){ return NULL; } ;
  virtual void sendBlockAscii(BTtrans *trans);
  virtual void receiveBlockAscii(BTtrans *trans);
  virtual BTblock* receiveId(BTlist **listPtr, BTtrans *trans);
  // not ready yet virtual void readBlockAscii(BTtrans *trans);
  virtual void sendUnique(BTtrans *trans);
  virtual void receiveUnique(BTtrans *trans);
  // routines to initially process events
  virtual void transInRoute(BTblock *trans) ;
  virtual void transOutRoute(BTblock *trans) ;
  virtual void transfer(BTtrans *trans);  // BTblk&tr.c
  virtual void block(BTtrans *trans);     // "
  virtual void makeWeightList(BTtrans *trace);
  // BTblock *StringToBlocks(char *strng);  // BTpages.c
  virtual BTblock *SearchForWord(char *word); // BTsrchwr.c
  virtual void RelaxNetwork(BTval melow) {}; // BTrest.c
  virtual void FocusNetwork(BTval melow) {}; // BTfocus.c
  virtual void DeFocusNetwork(BTval melow) {} ; // BTfocus.c
  void transferToCface(BTtrans *trans ) ; // BTcfcgen.c
  BTblock *findBestLeftLine(BTtrans *trans) ; // BTcfcfix.c
  BTblock *findBestRightLine(BTtrans *trans);
  void BandT(BTtrans *trans) ; // BTcfcfix.c
  BTblock *BTpageBlock::highList(BTlist *list) ;  //BTcfcfix
  void associateLikeChildren(BTtrans *trans ) ;
  // graphics stuff
  virtual void PageToBook(BTtrans *trans);
  virtual void PlaceGraph(BTtrans *trans);
  virtual void DisplayGraphics(BTWindow *wndw);
  virtual void BlocksToPixels(BTviewBlock *view);
  // channel stuff
  void receiveChannel(BTtrans *trans);
  void initializeDna(BTtrans *trans);
  virtual BTblock *PixelToBlock( pixStruc *pix );
};
class	BTbookBlock : public BTblock
{
  public:
  BTblock     *currentBlock;

  char *name;
  BTaccum *accum;
  
  BTblock *sysRegs[MAX_NUM_SYS_REGS];
  char buff[BUFF_SIZE];
  char keyBuff[BUFF_SIZE];
  char comBuff[BUFF_SIZE];
  HBITMAP  memBit;
  BTlist  		*blockViews;
  BTpageBlock *startPage;
  char    selListType;
  short pointerToggle;

  HANDLE hPal;

  BTblock *exPortKeyboard;
  BTblock *exPortModem;
  BTblock *trigButFocus;
  BTblock *trigButExecute;
  // methods
  BTbookBlock(char *bookName);
  virtual ~BTbookBlock();  			// in btbook.c
  virtual long Type() { return BT_TYPE_BOOK; } ;
  virtual char *Name() { return name ; } ;
  virtual void  Name(char *str){name = str;};
  virtual void makeWeightList(BTtrans *trace);
  virtual BTbookBlock *BookPtr(){ return this;};
  virtual BTaccum *AccumPtr(){return accum;};
  virtual void transfer(BTtrans *trans);  // BTblk&tr.c
  virtual void block(BTtrans *trans);     // "
  virtual void RelaxNetwork(BTval melow) {}; // BTrest.c
  virtual void FocusNetwork(BTval melow) {}; // BTfocus.c
  virtual void DeFocusNetwork(BTval melow) {}; // BTfocus.c
  // stuff in btb&ps&m.c
  virtual void addChild(BTblock *blkPtr);
  virtual void sendBlockAscii(BTtrans *trans);
  virtual void receiveBlockAscii(BTtrans *trans);
  virtual void sendUnique(BTtrans *trans);
  virtual void receiveUnique(BTtrans *trans);
  // in BTbook.c
  void WriteInBook(char *pageName, char *strng);
  void DrawInBook(char *strng);
  void CommandBook(char *strng);
  char *ReadFromBook();
  void DrawFromBook();
  void RestBook(short R_E_M);
  void GlanceAtBook();
  virtual void transInRoute(BTblock *trans) ;
  virtual void transOutRoute(BTblock *trans) ;
  virtual BTblock *SearchForWord(char *word); // BTsrchwr.c
  // btcommnd.c 
  BTblock *ProcessCommandBuffer(char *buffer){return NULL;};
  void initializeDna(BTtrans *trans);
  // windows stuff
  void NormalChar(HWND hWnd, WPARAM wParam ) ;
  void FunctionChar(HWND hWnd, WPARAM wParam ) ;
  virtual void openPort();
};
class BTtransFile : public BTblock
{
  public:
  FILE     *inf;
  FILE     *outf;
  char	   *file;
  char      inBuf[132];
  BTtransFile(BTtrans *trans,short blkId):BTblock(trans,blkId){};
  BTtransFile(BTblock *trans);
  BTtransFile(char *name);
  virtual void exPort(BTtrans *trans, char *data);
  virtual char *inPort(BTtrans *trans);
  virtual void openInPort();
  virtual void openExPort();
  virtual void closePort();
  virtual void OutString(char *strng);
};
// findBlock - used for finding things
class	BTfindBlock : public BTblock
{
  public:
  char *string;
  BTfindBlock(BTtrans *trans,short blkId):BTblock(trans,blkId){};
  BTfindBlock(BTtrans *trans,char *blkNam,short blkId);
  virtual long Type() { return BT_TYPE_FIND; } ;
  BTfindBlock(char *strng) ;
  BTfindBlock(short id) ;
  virtual char *Name() { return string ; } ;
  virtual void  Name(char *str){string = str;};
};

class BTcFaceBlock : public BTblock
{
  public:
  BTblock *internalPtr;
  BTspace  *space;
  BTval BlockType;
  BTval TransferType;
  BTcFaceBlock(BTtrans *trans) ; // : BTblock(trans)
  BTcFaceBlock() : BTblock()
  { internalPtr = NULL;BlockType=0;TransferType=0;} ;
  virtual long Type() { return BT_TYPE_CFACE; } ;
  void associateLikeChildren(BTtrans *trans ) ;
  void loadInternalWeight(BTtrans *trans ) ;
  void transferToCface(BTtrans *trans ) ;
  virtual char *Name(); // btcfcgen.c
  virtual BTspace   *SpacePtr() { return space ;} ;
  virtual BTspace   *SpacePtr(BTspace *ptr) {space=ptr;return space;};
  virtual void BandT(BTtrans *trans);
  BTcFaceBlock *findBestLeftLine(BTtrans *trans);
  void addWeightList(BTtrans *trans);
  virtual void BTcFaceBlock::addChild(BTblock *blkPtr) ;
  virtual void sendUnique(BTtrans *trans);
  virtual  BTblock *Internal(){return internalPtr;};
  virtual  BTblock *Internal(BTblock *ptr){internalPtr=ptr;return internalPtr;};

} ;
class	BTfieldBlock : public BTblock
{
  public:
  BTblock 	*trigKeyReturn;
  BTblock	*trigInput;
  BTblock	*trigButFocus;
  BTblock	*trigButExecute;
  BTbuff  	*buffer;
  virtual long Type() { return BT_TYPE_FIELD; } ;
  BTfieldBlock(char *strng, BTtrans *trans) ;
  virtual void sendUnique(BTtrans *trans);
  virtual void receiveUnique(BTtrans *trans);
  virtual void exPort(BTtrans *trans, char *data);
  virtual void PlaceBlockGraph(BTplace *placePtr);
  virtual void DisplayGraphics(BTWindow *wndw){};
  virtual void BlocksToPixels(BTviewBlock *view);
  virtual void transInRoute(BTblock *trans) ;
  virtual void transOutRoute(BTblock *trans) ;
  virtual  BTbuff *BuffPtr(){return buffer;}; 
  virtual  BTbuff *BuffPtr(BTbuff *ptr){buffer=ptr;return buffer;}; 
};
class BTviewBlock : public BTpageBlock   // a view into the book
{
  public:
  HBITMAP	memBit;
  // TThreeD	threeD;
  BTWindow	*graphWin;
  // BTspace	*space;
  BTbuff	*buffer;
  BTblock 	*trigKeyReturn;
  BTblock	*trigInput;
  BTblock	*trigComReturn; // when a return statement hits the fan
  BTblock	*trigButFocus;
  BTblock	*trigButExecute;
  int 		charPixX,charPixY;  // ?? may use for screen sizing to font
  // BTaccum *accum;
  
  // BTviewBlock() ;
  BTviewBlock( BTtrans *trans, char *strng) ;
  virtual ~BTviewBlock();
  virtual BTWindow *WindowPtr(){ return graphWin; } ;
  BTblock * BTviewBlock::TriggerBlock(short triggerType) ;
  virtual long Type() { return BT_TYPE_VIEW; } ;
  
  virtual BTspace   *SpacePtr() { return space ;} ;
  virtual BTspace   *SpacePtr(BTspace *ptr) {space=ptr;return space;};
  virtual void DisplayGraphics(BTWindow *wndw);
  void TextDisp(short pix,short piy,char *text);
  void BuffDisp();
  virtual  BTbuff *BuffPtr(){return buffer;}; 
  virtual  BTbuff *BuffPtr(BTbuff *ptr){buffer=ptr;return buffer;}; 
  virtual void openPort(); // opens graphics window in this case
  virtual void closePort(); // closes graphics windows in this case
  virtual void transInRoute(BTblock *trans) ;
  virtual void transOutRoute(BTblock *trans) ;
  BTblock *PixelToBlock( pixStruc *pix ) ;
} ;

class BTcommandModes
{ 
  public:
  short		opCode;
  char 		blockMode;
  char 		blockRegister;

  char 		transferMode;
  char 		transferRegister;

  char 		resultMode;
  char 		resultRegister;
  BTcommandModes();  
};

class BTcommandPtrs
{ 
  public:
  BTblock   *blockPtr;
  BTblock   *transferPtr;
  BTblock   *resultPtr;  
} ;

class BTcommand
{
  public:
  BTcommandPtrs *ptrs;
  BTcommandModes *modes;
};

class BTdnaBlock : public BTblock
{
  public:
  char *string;  
  BTcommandModes  *cmdModes;
  BTblock *internalPtr;
  // BTdnaBlock() : BTblock () { cmdModes = NULL;};
  
  BTdnaBlock( BTtrans *trans): BTblock(trans) 
    {string = NULL; cmdModes = NULL; internalPtr = NULL; };
  BTdnaBlock () : BTblock () {};
  virtual ~BTdnaBlock();  
  virtual void sendUnique(BTtrans *trans);
  virtual char *Name() { return string ; } ;
  virtual void  Name(char *str){string = str;};
  virtual void receiveUnique(BTtrans *trans);
  virtual BTcommandModes  *CommandModes(BTcommandModes  *cmds);
  virtual  BTblock *Internal(){return internalPtr;};
  virtual  BTblock *Internal(BTblock *ptr){internalPtr=ptr;return ptr;};
 
};
#include <btmsgblk.h>