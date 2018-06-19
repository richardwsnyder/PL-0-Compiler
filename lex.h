//Austin Peace
//Sebastian De La Cruz
#define IDENT_MAX_LENGTH 11
#define NUM_MAX_LENGTH 5
#define NUM_SYMBOLS 16
#define NUM_RESERVED 14

typedef struct TOKEN {
	int type;
	char value[10];
	struct TOKEN *next;
}TOKEN;

TOKEN *run_lexical_analyzer(char *filename, int print_flag);

void free_tokens(TOKEN *tokens);

enum lex {
	nul_sym = 1, ident_sym, num_sym, plus_sym, minus_sym, mult_sym, slash_sym, odd_sym, eql_sym, neq_sym, less_sym, leq_sym,
	gtr_sym, geq_sym, lparent_sym, rparent_sym, comma_sym, semicol_sym, period_sym, becomes_sym, begin_sym, end_sym, if_sym,
	then_sym, while_sym, do_sym, call_sym, const_sym, var_sym, proc_sym, write_sym, read_sym, else_sym
};


extern char *reserved[];
extern int reserved_lex[];

extern char *symbols[];
extern int symbol_lex[];