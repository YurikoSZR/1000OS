%{
#include <stdio.h> 
%}

DIGIT     [0-9]
INTEGER   {DIGIT}+
OPERATOR  [-+*/]
WHITESPACE [ \t\n] 
HEX       0x(0|([1-9a-fA-F][0-9a-fA-F]*))
BINARY    0b(0|([01][01]*))
DECIMAL   0|([1-9][0-9]*)
NUM    ({HEX}|{BINARY}|{DECIMAL})

%%
{INTEGER}    { printf("NUMBER: %s\n", yytext); }  
{OPERATOR}   { printf("OPERATOR: %s\n", yytext); }
"("          { printf("LEFT_PAREN\n"); }          
")"          { printf("RIGHT_PAREN\n"); }         
{WHITESPACE} { }                  
.            { printf("Invalid character: %s\n", yytext); }  
{NUM}        { printf("NUMBER: %s\n", yytext); }
%%

int main() {
    yylex(); 
    return 0;
}