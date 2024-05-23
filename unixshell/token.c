#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdbool.h>
#include <sys/wait.h>
#include <errno.h>
#include <sys/types.h>
#include "dynarray.h"
#include "lex_analysis.h"
#include "process_managment.h"
#include "token.h"

#define MAX_TOKENS 562

//create the token array and return it
tokenList_t create_token_list(void)
{
	
	tokenList_t oTokenList = malloc(sizeof(struct TokenList));
	if(!oTokenList){
		perror("malloc");
		exit(EXIT_FAILURE);
	}
	return oTokenList;
}

//take in create a token and return it
token_t create_token(enum TokenType type, char *value, enum Redirect redirection){
	token_t token = malloc(sizeof(struct Token));
	if(!token){
		perror("malloc");
		exit(EXIT_FAILURE);
	}
	token->val = malloc(MAX_LINE_SIZE * (sizeof(char)));
	if(!token->val){
		perror("malloc");
		exit(EXIT_FAILURE);
	}
	// set structure's members
	token->type = type;
	strcpy(token->val, value);
	token->redir = redirection;
	
	return token;
}