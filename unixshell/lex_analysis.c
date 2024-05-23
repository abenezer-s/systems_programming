// make modular
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

#define MAX_LINE_SIZE 1024

char temp_string[MAX_LINE_SIZE];

void redirect_r(tokenList_t *token_list_ptr, 
						token_t *prev_token_ptr,
						token_t *cur_token_ptr, 
						int i, int j){


	if(j != 0 && i != 0){
		temp_string[j] = '\0';
		j = 0;

		if(*prev_token_ptr == NULL){
			*cur_token_ptr = create_token(ARG, temp_string, NONE);
			(*token_list_ptr)->num_tokens++;
			(*token_list_ptr)->tokens = *cur_token_ptr;
			*prev_token_ptr = *cur_token_ptr;
		
		} else{
			*cur_token_ptr = create_token(ARG, temp_string, NONE);
			(*token_list_ptr)->num_tokens++;
			(*prev_token_ptr)->next = *cur_token_ptr;
			*prev_token_ptr = *cur_token_ptr;
		}

		} 
			
		temp_string[j++] = '>';
		//add terminating null char
		temp_string[j] = '\0';
		j = 0;
		//redirection out
		if(prev_token_ptr == NULL){
				*cur_token_ptr = create_token(REDIRECTION, temp_string, STDOUT);
				(*token_list_ptr)->num_tokens++;
				(*token_list_ptr)->tokens = *cur_token_ptr;
				*prev_token_ptr = *cur_token_ptr;
			
			} else{
				*cur_token_ptr = create_token(REDIRECTION, temp_string, STDOUT);
				(*token_list_ptr)->num_tokens++;
				(*prev_token_ptr)->next = *cur_token_ptr;
				*prev_token_ptr = *cur_token_ptr;
			}
	

}

int wrapper_create_token(bool initial,
						tokenList_t *token_list,
					 	token_t *prev_token, 
					 	enum TokenType type,
					 	char *value,
						enum Redirect redirection){
	token_t cur_token;
	//intial token creation
	if(initial){
		cur_token = create_token(type, value, redirection);
		(*token_list)->num_tokens++;
		(*token_list)->tokens = cur_token;
		(*prev_token) = cur_token;
	} else if(type == PIPE){
		cur_token = create_token(PIPE, temp_string, NONE);
		(*token_list)->num_tokens++;
		(*prev_token)->next = cur_token;
		(*prev_token) = cur_token;
		(*token_list)->pipe_exists = true;
	} else{
		// creation of a token that is an arguement
		cur_token = create_token(type, value, redirection);
		(*token_list)->num_tokens++;
		(*prev_token)->next = cur_token;
		(*prev_token) = cur_token;}

	
}
// takes in a string analyzes lexically and returns a token array.
tokenList_t lex(char *string, int *Num_Pipes)
{
	int i, j, num_pipe = 0;
	token_t  cur_token;
	token_t prev_token = NULL;
	tokenList_t token_list = create_token_list();
	token_list->pipe_exists = false;
	token_list->num_tokens = 0;
	bool quote_opened = false;
	i = 0, j = 0;

	while(1)
	{
		//iterate the string, if not a whitespace create a token
		char c = string[i];
	
		if(isspace(c))
		{
			if(c == ' '){
				//add terminating null char
				if(j == 0){
					temp_string[j++] = c;
				}
				temp_string[j] = '\0';
				j = 0;
				int iL = 0;
				int length = 0;
				while(1){
					if(temp_string[iL++] == '\0'){
						break;
					}else{
						length++;
					}
				}

				if(length == 1 && temp_string[0] == ' '){
					i++;
					continue;
				}

				//create token
				if(prev_token == NULL){
					wrapper_create_token(true, &token_list,&prev_token,ARG, temp_string,NONE);
					
				} else if(prev_token->type == REDIRECTION && 
							prev_token != NULL){
						wrapper_create_token(false, &token_list, &prev_token, isFILE, temp_string, NONE);

				}else{
					wrapper_create_token(false, &token_list, &prev_token,ARG, temp_string,NONE);
				}
				
			}else if(c == '\n'){
				wrapper_create_token(false, &token_list, &prev_token,ARG, temp_string,NONE);
				break;
			} 
		}else if(c == '>'){

			redirect_r(&token_list, &prev_token, &cur_token, i, j);
			
			i++;
			
			continue;

		}else if(c == '<'){
			if(j != 0){
				temp_string[j] = '\0';
				j = 0;
				wrapper_create_token(false, &token_list, &prev_token, ARG, temp_string,NONE);
			} 
			temp_string[j++] = '<';
			//add terminating null char
			temp_string[j] = '\0';
			j = 0;
			//redirection out
			wrapper_create_token(false, &token_list, &prev_token, REDIRECTION, temp_string,STDIN);
			i += 2;
			continue;

		} else if(c == '\"'){
			//quote opened
			quote_opened = true;
			while(1){
				i++;
				c = string[i];
				if(c == '\"'){
					//quote closed
					quote_opened = false;
					break;
					
				}else if(c == '\n'){
					//error
					if(quote_opened){
						fprintf(stderr, "ERROR: Wrong Quotation Uasage\n");
						return NULL;
					}
				}else{
					//store
					temp_string[j++] = c;
				}
			}
			
		}else if(c == '|'){
				temp_string[j++] = c;
				temp_string[j] = '\0';
				j = 0;
				wrapper_create_token(false, &token_list, &prev_token, PIPE, temp_string, NONE);
				num_pipe++;
		}else if(c == '\0'){
			//add terminating null char
			if(j == 0){
				cur_token->next = NULL;
				break;
			}
			temp_string[j] = '\0';
			j = 0;
			//create token
			if(prev_token == NULL){
				cur_token = create_token(ARG, temp_string, NONE);
				token_list->num_tokens++;
				cur_token->next = NULL;
				token_list->tokens = cur_token;
				prev_token = cur_token;
				
			} else if(prev_token->type == REDIRECTION && 
						prev_token != NULL){
						cur_token = create_token(isFILE, temp_string, NONE);
						token_list->num_tokens++;
						cur_token->next = NULL;
						prev_token->next = cur_token;
						prev_token = cur_token;
			}else{
				cur_token = create_token(ARG, temp_string, NONE);
				token_list->num_tokens++;
				cur_token->next = NULL;
				prev_token->next = cur_token;
				prev_token = cur_token;
			}
			break;
		}else{

				if(!isspace(c) && c != '\"')
				{	
					//store 
					temp_string[j++] = c;
				}else{ 
					if(c == '\"'){
						i++;
					}
					continue;
				}
			}
		i++;
	}

	*Num_Pipes = num_pipe;
	return token_list;
}
