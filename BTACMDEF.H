// BTacmdef.h
//  definition for B4 accumulator
//
 
#define STATUS_BAD		0
#define STATUS_OK		1
#define STATUS_BREAK	3
#define STATUS_CYCLE	5 
 
#define EMPTY 0
#define MAX_MATH_REGISTERS 9
#define MAX_NUM_COMMANDS   127

// address mode parameters

#define MODE_NONE			0
#define MODE_PLACE			2	//  [] a place holder
#define MODE_REGISTER		3	//  [...]
#define MODE_INDIRECT		4   // i[...]
#define MODE_COMPLEX		5   // c[...]
#define MODE_COMPLEX_IND    6	// ci[...] 4.1
#define MODE_FIXED			7   //  f[0..9]
#define MODE_STRING			8   //  b[0]
#define MODE_KERNAL         9   //  k[

// accumulator indexes
#define ACCUM_SELECT		0
#define ACCUM_BLOCK			1
#define ACCUM_TRANSFER		2
#define ACCUM_RESULT		3
#define ACCUM_PC			4
#define ACCUM_STACK  		5
#define ACCUM_EXP			6
#define ACCUM_DEBUG			7
#define ACCUM_PAGE			8
#define ACCUM_INT_BSEL		9	// spatial trans
#define ACCUM_INT_BEXE      10  // spat.. block
#define ACCUM_INT_DRAG		11
#define ACCUM_INT_DROP		12
#define ACCUM_INT_BUFF      13
#define ACCUM_INT_DISP		14
#define ACCUM_REGISTER_TOT  15

// accumulator text identifieres
#define ACCUM_SELECT_ID		"[sel]"
#define ACCUM_BLOCK_ID		"[blk]"
#define ACCUM_TRANSFER_ID	"[trn]"
#define ACCUM_RESULT_ID		"[rsl]"
#define ACCUM_PC_ID			"[pc]"
#define ACCUM_STACK_ID 		"[stk]"
#define ACCUM_EXP_ID		"[exp]"
#define ACCUM_INT_BSEL_ID	"[bsl]"	// was bsl spatial trans
#define ACCUM_INT_BEXE_ID   "[bex]"  //was bex spat.. block
#define ACCUM_INT_DRAG_ID	"[drg]"	// was bsl spatial trans
#define ACCUM_INT_DROP_ID   "[drp]"  //was bex spat.. block
#define ACCUM_INT_BUFF_ID   "[buf]"
#define ACCUM_INT_DISP_ID	"[dsp]"
#define ACCUM_DEBUG_ID		"[dbg]"
#define ACCUM_PAGE_ID		"[pg]"

#define REG_SYS_DRAGVIEW	1
#define REG_SYS_DROPVIEW    2
#define REG_SYS_DRAGBLOCK   3
#define REG_SYS_DROPBLOCK   4
#define REG_SYS_SELVIEW     5
#define REG_SYS_SELBLOCK    6
#define REG_SYS_SCHEDULE	7
#define REG_SYS_R_BUTTON    8
#define REG_SYS_M_BUTTON    9
#define REG_SYS_L_BUTTON    10
#define REG_SYS_DDEACCUM    11
#define REG_SYS_DICTIONARY  12

// operations

#define OPER_NOOP			0  // just skip over
#define OPER_LOAD_BLOCK		1  // @load block [x] -string as child or name
#define OPER_LOAD_NULL		2  // @load this [x] - high weight child gets loaded
#define OPER_LOAD_STRING	2  // @load string - string as child or name
#define OPER_LOAD_MATH		3  // @load math ?[1..9] a_number
#define OPER_LOAD_PAGE		4

#define OPER_ITER_CHILD     10 // @iter child [x]
#define OPER_ITER_PARENT	11 // @iter parent [x]
#define OPER_ITER_INPUT		12 // @iter input [x]
#define OPER_ITER_OUTPUT	13 // @iter output [x]
#define OPER_ITER_NEXT		14 // @iter next [x]
#define OPER_ITER_PREV		15 // @iter prev [x]
#define OPER_ITER_DEL		16 // @iter del [x]
#define OPER_ITER_INSERT	17 // @iter add [x] i[x]
#define OPER_ITER_REPLACE	18 // @iter repl [x] i[x]
#define OPER_ITER_HIGH		19 // @iter high [x]
#define OPER_ITER_LOW		20 // @iter low  [x]
#define OPER_ITER_FIND		21 // @iter find [x] x[x]
#define OPER_ITER_MATCH     22 // @iter match x[x] x[x]
#define OPER_ITER_TOUCH		23 // @iter touch x[x] x[x]
#define OPER_ITER_ADD_END   24 // @iter add_end [x] i[x]
#define OPER_ITER_ADD		25 // @iter add [x] i[x]

#define OPER_MOVE_STRING    27   // @move energy f[1] i[blk]
#define OPER_MOVE_ENERGY    OPER_MOVE_STRING+1 // @move energy x[x] [x]
#define OPER_MOVE_BRATIO    OPER_MOVE_ENERGY+1
#define OPER_MOVE_TRATIO    OPER_MOVE_BRATIO+1
#define OPER_MOVE_WEIGHT    OPER_MOVE_TRATIO+1   // @move weight (i,f)[x] (i,f)[x]
#define OPER_MOVE_VPOS      OPER_MOVE_WEIGHT+1
#define OPER_MOVE_VSIZ      OPER_MOVE_VPOS+1
#define OPER_MOVE_PAGE      OPER_MOVE_VSIZ+1
#define OPER_MOVE_BOOK      OPER_MOVE_PAGE+1
#define OPER_MOVE_ROTATE    OPER_MOVE_BOOK+1
#define OPER_MOVE_VECTOR    OPER_MOVE_ROTATE+1
#define OPER_MOVE_SCALE		OPER_MOVE_VECTOR+1
#define OPER_MOVE_BLOCK		OPER_MOVE_SCALE+1
#define OPER_MOVE_EDGE      OPER_MOVE_BLOCK+1
#define OPER_MOVE_POSIT      OPER_MOVE_EDGE+1

#define OPER_BREAK_NULL		50 // @break null x[x]
#define OPER_BREAK_NONULL   51 // @break nonull x[x]
#define OPER_BREAK_LOW		52 // @break low f[x] <= 0
#define OPER_BREAK_HIGH		53 // @break high f[x] > 0
#define OPER_BREAK			54 // @break

#define OPER_CYCLE			57

#define OPER_BOT			60 // @bot x[x]
#define OPER_PROCESS		61
#define OPER_PROCESS_MSG	62

#define OPER_COMPARE_NAME   65

// where x = f(ixed point), v(ector fixed point), i(integer)
#define OPER_MATH_LOAD		70 // @math load x[m] string_value
#define OPER_MATH_ADD		71 // @math add x[m] x[m] x[m]
#define OPER_MATH_SUB		72 // @math sub x[m] x[m] x[m]
#define OPER_MATH_DIV		73 // @math div x[m] x[m] x[m]
#define OPER_MATH_MUL		74 // @math mul x[m] x[m] x[m]
#define OPER_MATH_DIS		75 // @math dis v[m] v[m] f[m]

#define OPER_BMP_FILE       80 // @bmp file x[x] string_value
#define OPER_BMP_DELETE		81 // @bmp del x[x]
#define OPER_BMP_PAL		82 // @bmp pal x[x]
#define OPER_BMP_BACK		83 // @bmp back x[x]

#define OPER_FILE_SAVE		85
#define OPER_FILE_LOAD		86


#define OPER_GRAPH_CONSTRUCT	90
#define OPER_GRAPH_PAGETOVIEW	OPER_GRAPH_CONSTRUCT+1
#define OPER_GRAPH_VIEWTOPIX	OPER_GRAPH_PAGETOVIEW+1
#define OPER_GRAPH_DISPLAY		OPER_GRAPH_VIEWTOPIX+1
#define OPER_GRAPH_EDGE	    	OPER_GRAPH_DISPLAY+1
#define OPER_GRAPH_FACET	    OPER_GRAPH_EDGE+1

#define OPER_DELETE_BLOCK		110
#define OPER_CREATE_BASIC		111  // @create block (object_type) [x]
#define OPER_CREATE_GROUP		112
#define OPER_CREATE_CHUNK		113
#define OPER_CREATE_TRANSFER	114
#define OPER_CREATE_STRING		115
#define OPER_CREATE_READ		116
#define OPER_CREATE_PAGE		117
#define OPER_CREATE_BOOK		118
#define OPER_CREATE_CFACE       119
#define OPER_CREATE_OPERATE     120
#define OPER_CREATE_BOTS        121
#define OPER_CREATE_VIEW        122
#define OPER_CREATE_FIELD       123
#define OPER_CREATE_MESSAGE     124
#define OPER_CREATE_ACCUM		125

 



 