/* Authors:
Richard Snyder
Camilo Gomez
*/

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include "lex.h"

char token;
FILE *fp;
int quit;
char ident_buffer[IDENT_MAX_LENGTH + 1];
char *code;
int code_length, code_alloc;
TOKEN *start, *end;

int line;
int col;
int prev_col;
int prev_line;

int ident_or_reserved();
void print_lexeme_list();
void add_to_lexeme(int type, int lex);
void print_lexeme_table();
int number();
static int get_token();
void remove_comments();
int is_symbol();
void print_code();

char *reserved[] = { "const","var", "procedure", "call", "begin", "end", "if", "then", "else", "while", "do", "read", "write", "odd" };
int reserved_lex[] = { const_sym, var_sym, proc_sym, call_sym, begin_sym, end_sym, if_sym, then_sym, else_sym, while_sym, do_sym,
read_sym, write_sym, odd_sym };

char *symbols[] = { "+", "-", "*", "/", "(", ")", "=", ",", ".", "<", ">", ";", ":=", "<=", ">=", "<>" };
int symbol_lex[] = { plus_sym, minus_sym, mult_sym, slash_sym, lparent_sym, rparent_sym, eql_sym, comma_sym, period_sym, less_sym,
gtr_sym, semicol_sym, becomes_sym, leq_sym, geq_sym, neq_sym };

TOKEN *run_lexical_analyzer(char *filename, int print_flag) {
	int error = 0;
	int ret = 0;

	line = 1;
	col = 0;
	quit = 0;

	fp = fopen(filename, "r");

	if (fp == NULL) {
		printf("Error: %s does not exist!\n", filename);
		return NULL;
	}

	code_alloc = 500;
	code_length = 0;
	code = calloc(500, 1);

	if (get_token()) {
		printf("Error: File Empty!\n");
		return NULL;
	}

	while (!quit) {
		//printf("while !quit\n");
		if (isalpha(token)) {						//ident or reserved
			if((ret = ident_or_reserved()))
				goto MAIN_EXIT;
		} else if (isdigit(token)) {		//number
			if((ret = number()))
				goto MAIN_EXIT;
		} else if (token == ' ' || iscntrl(token)) {
			if(get_token())
				quit = 1;
			continue;
		} else if (token == '/') {
			remove_comments();
		} else {
			if ((ret = is_symbol()))							//Symbol
				goto MAIN_EXIT;
		}
		memset(ident_buffer, 0, IDENT_MAX_LENGTH + 1);
	}

	if (print_flag) {
		//print_code();
		//print_lexeme_table();
		print_lexeme_list();
	}

MAIN_EXIT:
	free(code);

	if(ret == 0)
		return start;

	free_tokens(start);

	return NULL;
}

int is_symbol() {
	int i, is_single = 0, is_doubl = 0;
	char single[2], doubl[3];
	single[1] = 0;
	doubl[2] = 0;

	single[0] = token;
	doubl[0] = token;

	//if end of file put in garbage for doubl
	if (get_token()) {
		doubl[1] = ']';
	} else {
		doubl[1] = token;
	}

	for (i = 0; i < NUM_SYMBOLS; i++) {
		if (strcmp(symbols[i], single) == 0) {
			is_single = i + 1;
		}
		if (strcmp(symbols[i], doubl) == 0) {
			is_doubl = i + 1;
		}
	}

	if (!is_single && !is_doubl) {
		printf("Error: Unknown Symbol [%s] at %d:%d\n", single, prev_line, prev_col);
		return -1;
	}

	if (is_doubl) {
		strcpy(ident_buffer, doubl);
		add_to_lexeme(1, is_doubl - 1);
		get_token();
	} else {
		strcpy(ident_buffer, single);
		add_to_lexeme(1, is_single - 1);
	}

	return 0;
}

void print_code() {
	int i;

	printf("\nSource Program:\n");

	for (i = 0; i < code_length; i++) {
		printf("%c", code[i]);
	}

	printf("\n");
}

void print_lexeme_list() {
	TOKEN *tok = start;
	TOKEN *tok2 = start;
	int k;

	for(k = 0; k < 43; k++)
		printf("-");

	printf("\nLIST OF LEXEMES/TOKENS:\n\nInternal Representation:\n");

	while (tok != NULL) {
		if (tok->type == 2 || tok->type == 3)
			printf("%d %s  ", tok->type, tok->value);
		else
			printf("%d  ", tok->type);
		tok = tok->next;
	}

	printf("\n\nSymbolic Representation:\n");

	while (tok2 != NULL) {
		if (tok2->type == 1)
			printf("nulsym  ");
		else if (tok2->type == 2)
			printf("identsym %s  ", tok2->value);
		else if (tok2->type == 3)
			printf("numbersym %s  ", tok2->value);
		else if (tok2->type == 4)
			printf("plussym  ");
		else if (tok2->type == 5)
			printf("minussym  ");
		else if (tok2->type == 6)
			printf("multsym  ");
		else if (tok2->type == 7)
			printf("slashsym  ");
		else if (tok2->type == 8)
			printf("oddsym  ");
		else if (tok2->type == 9)
			printf("eqsym  ");
		else if (tok2->type == 10)
			printf("neqsym  ");
		else if (tok2->type == 11)
			printf("lessym  ");
		else if (tok2->type == 12)
			printf("leqsym  ");
		else if (tok2->type == 13)
			printf("gtrsym  ");
		else if (tok2->type == 14)
			printf("geqsym  ");
		else if (tok2->type == 15)
			printf("lparentsym  ");
		else if (tok2->type == 16)
			printf("rparentsym  ");
		else if (tok2->type == 17)
			printf("commasym  ");
		else if (tok2->type == 18)
			printf("semicolonsym  ");
		else if (tok2->type == 19)
			printf("periodsym  ");
		else if (tok2->type == 20)
			printf("becomessym  ");
		else if (tok2->type == 21)
			printf("beginsym  ");
		else if (tok2->type == 22)
			printf("endsym  ");
		else if (tok2->type == 23)
			printf("ifsym  ");
		else if (tok2->type == 24)
			printf("thensym  ");
		else if (tok2->type == 25)
			printf("whilesym  ");
		else if (tok2->type == 26)
			printf("dosym  ");
		else if (tok2->type == 27)
			printf("callsym  ");
		else if (tok2->type == 28)
			printf("constsym  ");
		else if (tok2->type == 29)
			printf("varsym  ");
		else if (tok2->type == 30)
			printf("procsym  ");
		else if (tok2->type == 31)
			printf("writesym  ");
		else if (tok2->type == 32)
			printf("readsym  ");
		else if (tok2->type == 33)
			printf("elsesym  ");

		tok2 = tok2->next;
	}

	printf("\n");
}

void print_lexeme_table() {
	TOKEN *tok = start;

	printf("\nLexeme Table:\nLexeme\t\tToken Type\n");

	while (tok != NULL) {
		printf("%s\t\t%d\n", tok->value, tok->type);
		tok = tok->next;
	}
}

void remove_comments() {

	int type = 0;

	ident_buffer[0] = token;

	if (get_token() == -1) {
		add_to_lexeme(1, 7);
		return;
	}

	if (token != '/' && token != '*') {
		//printf("token is: %c\n", token);
		add_to_lexeme(1, 3); // add slash to lexeme not part of comments
		return;
	}

	if (token == '/')
	{
		//printf("token is type 1\n");
		type = 1;
	}
	if (token == '*')
	{
		//printf("token is type 2\n");
		type = 2;
	}
	while (1) {
		if (get_token())
			return;

		if (type == 1 && token == '\n')
			break;
		if (type == 2 && token == '*') {
			if (get_token())
				return;
			if (token == '/')
				break;
		}
	}

	get_token();
}

static int get_token() {
	if (fscanf(fp, "%c", &token) == EOF) {
		quit = 1;
		return -1;
	}
	prev_col = col;
	prev_line = line;
	if (token == '\n') {
		line++;
		col = 0;
	} else if (token == '\t') {
		col += 5;
	} else {
		col++;
	}

	if (code_length > code_alloc) {
		code_alloc = code_length + 500;
		code = realloc(code, code_alloc * sizeof(int));
	}
	code[code_length++] = token;
	return 0;
}

int number() {
	int length = 1;

	ident_buffer[0] = token;

	//if end of file just assume what we have is an number
	if (get_token() == -1)
		goto NUMBER_EXIT;

	 while (length < NUM_MAX_LENGTH) {
		 if (isdigit(token)) {
			ident_buffer[length++] = token;
			if (get_token())
				goto NUMBER_EXIT;
			continue;
		}
		 break;
	}

	 if (isalpha(token)) {
		 printf("Error: Identifier cannot start with a number! %d:%d", line, col);
		 return -1;
	 }

	 if (length == NUM_MAX_LENGTH) {
		 if (isdigit(token)) {
			 printf("Error: Number [%s] at %d:%d exceeds max size...\n", ident_buffer, prev_line, (int)(prev_col- strlen(ident_buffer)));
			 return -1;
		}
	 }

NUMBER_EXIT:
	 add_to_lexeme(3, 0);
	 return 0;
}

int check_reserved() {
	int i;

	for (i = 0; i < NUM_RESERVED; i++) {
		if (strcmp(reserved[i], ident_buffer) == 0) {
			return i;
		}
	}
	return -1;
}

int ident_or_reserved() {
	int ret = 0;
	int length = 1;

	ident_buffer[0] = token;

	//if end of file just assume what we have is an ident
	if (get_token() == -1) {
		add_to_lexeme(2, 2);
		return 0;
	}

	while(length < IDENT_MAX_LENGTH) {
		if(isalpha(token) || isdigit(token)) {
			ident_buffer[length++] = token;
			if (get_token())
				goto IDENT_RES_EXIT;
			continue;
		}
		break;
	}

	if (length == IDENT_MAX_LENGTH) {
		if (isalpha(token) || isdigit(token)) {
			printf("Error: Identifier [%s] at %d:%d exceeds max size...\n", ident_buffer, prev_line, (int)(prev_col - strlen(ident_buffer)));
			return -1;
		}
	}

IDENT_RES_EXIT:
	if ((ret = check_reserved()) != -1) {
		//put reserved on stack
		add_to_lexeme(0, ret);
	} else {
		//its an identifier
		add_to_lexeme(2, 2);
	}

	return 0;
}

//type: 0 = Reserved word
//type: 1 = Symbol lex = index in symbol_lex
//type: 2 = identifier lex = Anyvalue
//type: 3 = number lex = Anyvalue
void add_to_lexeme(int type, int lex) {
	TOKEN *tok;

	tok = malloc(sizeof(TOKEN));
	tok->next = NULL;

	if (type == 0) {
		tok->type = reserved_lex[lex];
	} else if (type == 1) {
		tok->type = symbol_lex[lex];
	} else if (type == 2) {
		tok->type = ident_sym;
	} else if (type == 3) {
		tok->type = num_sym;
	}

	strcpy(tok->value, ident_buffer);

	if (start == NULL) {
		start = tok;
		end = tok;
	} else {
		end->next = tok;
		end = tok;
	}
}

void free_tokens(TOKEN *tokens) {
	TOKEN *temp;
	TOKEN *next;

	if(tokens == NULL)
		return;

	if(tokens->next == NULL) {
		free(tokens);
		return;
	}

	temp = tokens;
	next = tokens->next;

	while(next != NULL) {
		free(temp);
		temp = next;
		next = next->next;
	}

	free(temp);
}
