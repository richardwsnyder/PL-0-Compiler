//Austin Peace
//Sebastian De La Cruz

#include <stdio.h>
#define MAX_STACK_HEIGHT 2000
#define MAX_CODE_LENGTH 500
#define MAX_LEXI_LEVELS 3
#define NUM_OP_CODES 23
#define MAX_OUTPUT_BUFFER_LENGTH 500
#define MAX_REGISTERS 8

int run_VM(char *filename, int input_flag);

static char *opcodes[] = {
	"ERROR",
	"LIT",
	"RTN",
	"LOD",
	"STO",
	"CAL",
	"INC",
	"JMP",
	"JPC",
	"SIO",
	"NEG",
	"ADD",
	"SUB",
	"MUL",
	"DIV",
	"ODD",
	"MOD",
	"EQL",
	"NEQ",
	"LSS",
	"LEQ",
	"GTR",
	"GEQ"
};

static char *opcodeslowercase[] = {
	"error",
	"lit",
	"rtn",
	"lod",
	"sto",
	"cal",
	"inc",
	"jmp",
	"jpc",
	"sio",
	"neg",
	"add",
	"sub",
	"mul",
	"div",
	"odd",
	"mod",
	"eql",
	"neq",
	"lss",
	"leq",
	"gtr",
	"geq"
};

enum ISA {
	zero,
	LIT,
	RTN,
	LOD,
	STO,
	CAL,
	INC,
	JMP,
	JPC,
	SIO,
	NEG,
	ADD,
	SUB,
	MUL,
	DIV,
	ODD,
	MOD,
	EQL,
	NEQ,
	LSS,
	LEQ,
	GTR,
	GEQ
};

typedef struct {
	int op;
	int r;
	int l;
	int m;
}INSTRUCTION;

typedef struct {
	int sp;
	int bp;
	int pc;
	INSTRUCTION ir;
}CPU;