%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
typedef enum {
    TOKEN_NUMBER,      
    TOKEN_OPERATOR,    
    TOKEN_LEFT_PAREN,  
    TOKEN_RIGHT_PAREN, 
    TOKEN_INVALID      
} TokenType;
typedef struct Token {
    TokenType type;    
    char value[32];    
    struct Token *next; 
} Token;

Token *token_head = NULL; 
Token *token_tail = NULL; 
void add_token(TokenType type, const char *value);
void print_tokens();
void free_tokens();

%}

DIGIT     [0-9]
HEX      0x[0-9a-fA-F]+      
BINARY   0b[01]+             
DECIMAL  0|([1-9][0-9]*)     
OPERATOR [-+*/]
WHITESPACE [ \t\n]           

%option noyywrap             

%%

{HEX}       { add_token(TOKEN_NUMBER, yytext); }     
{BINARY}    { add_token(TOKEN_NUMBER, yytext); }     
{DECIMAL}   { add_token(TOKEN_NUMBER, yytext); }     
{OPERATOR}  { add_token(TOKEN_OPERATOR, yytext); }   
"("         { add_token(TOKEN_LEFT_PAREN, yytext); } 
")"         { add_token(TOKEN_RIGHT_PAREN, yytext); }
{WHITESPACE} { /* 忽略空白符 */ }                     
.           { add_token(TOKEN_INVALID, yytext); }    

%%
void add_token(TokenType type, const char *value) {
    Token *new_token = (Token *)malloc(sizeof(Token));
    if (!new_token) {
        fprintf(stderr, "Memory allocation error!\n");
        exit(1);
    }
    new_token->type = type;
    strncpy(new_token->value, value, sizeof(new_token->value) - 1);
    new_token->value[sizeof(new_token->value) - 1] = '\0'; 
    new_token->next = NULL;
    if (!token_head) {
        token_head = token_tail = new_token;
    } else {
        token_tail->next = new_token;
        token_tail = new_token;
    }
}
void print_tokens() {
    Token *current = token_head;
    while (current) {
        const char *type_str;
        switch (current->type) {
            case TOKEN_NUMBER:     type_str = "NUMBER"; break;
            case TOKEN_OPERATOR:   type_str = "OPERATOR"; break;
            case TOKEN_LEFT_PAREN: type_str = "LEFT_PAREN"; break;
            case TOKEN_RIGHT_PAREN:type_str = "RIGHT_PAREN"; break;
            case TOKEN_INVALID:   type_str = "INVALID"; break;
            default:              type_str = "UNKNOWN"; break;
        }
        printf("[%s] %s\n", type_str, current->value);
        current = current->next;
    }
}
void free_tokens() {
    Token *current = token_head;
    while (current) {
        Token *temp = current;
        current = current->next;
        free(temp);
    }
    token_head = token_tail = NULL;
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <input-file>\n", argv[0]);
        return 1;
    }
    FILE *input_file = fopen(argv[1], "r");
    if (!input_file) {
        perror("Failed to open file");
        return 1;
    }
    yyin = input_file; 
    yylex();          
    fclose(input_file);
    print_tokens();    
    free_tokens();     

    return 0;
}
