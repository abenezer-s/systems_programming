#ifndef TOKEN
#define TOKEN
#include <stdbool.h>


enum Redirect {NONE, STDOUT, STDIN};
enum TokenType {ARG, REDIRECTION, PIPE, isFILE};
typedef struct Token *token_t;
typedef struct TokenList *tokenList_t;

struct TokenList{
	bool pipe_exists;
	int num_tokens;
	token_t tokens; // a pointer to the first token in the list
};

struct Token{
	enum  TokenType type;
	char *val;
	enum Redirect redir;
	token_t next;
};

tokenList_t create_token_list(void);

token_t create_token(enum TokenType type, char *value, enum Redirect redirection);

#endif