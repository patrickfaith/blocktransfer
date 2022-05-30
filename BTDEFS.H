///
/// file for parameters and definitions
///
typedef short  BTfix;
typedef BTfix BTval;
typedef class BTblock BTtrans;
typedef class BTblock BTfocusBlock;

#define MAX_NUM_SYS_REGS   16

// fixed point crap
#define MAX_FIX      	16384
#define MIN_FIX      	-16384
#define MAX_FIX_ANGLE	360
#define MIN_FIX_ANGLE	0
#define FLOAT_TO_FIX    8192
#define FIX_ONE 		8192
#define FIX_ONE_MINUS   -8192
#define BUFF_SIZE 1024
#define strncmpi strnicmp
/// types for all blocks
#define BT_TYPE_BASIC		1
#define BT_TYPE_GROUP		2
#define BT_TYPE_CHUNK		3
#define BT_TYPE_TRANSFER	4
#define BT_TYPE_STRING		5
#define BT_TYPE_READ		6
#define BT_TYPE_PAGE		7
#define BT_TYPE_BOOK		8
#define BT_TYPE_TRANS_IN	9
#define BT_TYPE_TRANS_OUT	10
#define BT_TYPE_TRANS_PARENT	11
#define BT_TYPE_TRANS_CHILD	12
#define BT_TYPE_FIND            13
#define BT_TYPE_CFACE           14
#define BT_TYPE_OPERATE         15
#define BT_TYPE_BOTS            16
#define BT_TYPE_VIEW            17
#define BT_TYPE_FIELD           18
#define BT_TYPE_MESSAGE         19
// deactivate flags
#define BLOCK_FLAG     		1
#define TRANS_FLAG        	2
#define DISP_CHILD_FLAG		4
#define DISP_FLAG		8
#define DISP_FACADE_FLAG        16
// graph types
#define BT_GRAPH 				0
#define BT_LINE_GRAPH			1
#define BT_SQUARE_GRAPH			2
#define BT_CIRCLE_GRAPH			3
#define BT_MEM_GRAPH			4
#define BT_BMP_GRAPH			5
#define BT_ICON_GRAPH			6

// bot execution flags
#define BREAK_FLAG	1
#define BUFF_FLAG	2
#define FOCUS_FLAG	4
#define SUB_FOCUS_FLAG	8
#define START_FLAG	16
#define FLAG_FLAG	32
//
#define BT_PARENT		1
#define BT_CHILD		2
#define BT_INPUT		3
#define BT_OUTPUT		4
//
#define BT_TRIG_KEY_RETURN	1
#define BT_TRIG_COM_RETURN      2
#define BT_TRIG_BUT_FOCUS       3
#define BT_TRIG_BUT_EXECUTE     4
// 3-D crap
#define FOCAL_LENGTH   700.0
// math defs
#define Ln10 2.30258509299405E+000
#define Pi 3.1415927
#define PiOver180 1.74532925199433E-002
#define PiUnder180 5.72957795130823E+001

// typedef enum {false, true} Boolean;
typedef unsigned char      Byte;
typedef unsigned int       Word;
// vector and matrix defs
typedef int   TDX[2];
typedef BTfix TDA[3];
typedef BTfix Matx4x4[4][4];
// some graphic constants
#define MaxXres 320
#define MaxYres 200
#define MaxX (MaxXres-1)
#define MaxY (MaxYres-1)

