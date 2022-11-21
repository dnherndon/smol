/*   smol is a small C compiler
 *   Copyright (C) 2022  D. Nigh Herndon
 *
 *   This program is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */
#include "smol.h"

// Function prototypes
int direct_declarator(TOKEN** token);
int declarator(TOKEN** token);
int type_specifier(TOKEN** token);
int declaration_specifier(TOKEN** token);
int function_definition(TOKEN** token);
int expression(TOKEN** token);
int assignment_expression(TOKEN** token);
int conditional_expression(TOKEN** token);
int logical_or_expression(TOKEN** token);
int logical_and_expression(TOKEN** token);
int inclusive_or_expression(TOKEN** token);
int exclusive_or_expression(TOKEN** token);
int and_expression(TOKEN** token);
int equality_expression(TOKEN** token);
int relational_expression(TOKEN** token);
int shift_expression(TOKEN** token);
int additive_expression(TOKEN** token);
int multiplicative_expression(TOKEN** token);
int cast_expression(TOKEN** token);
int unary_expression(TOKEN** token);
int postfix_expression(TOKEN** token);
int primary_expression(TOKEN** token);
int constant(TOKEN** token);
int assignment_expression(TOKEN** token);
int jump_statement(TOKEN** token);
int expression(TOKEN** token);
int statement(TOKEN** token);
int init_declarator(TOKEN** token);
int initializer(TOKEN** token);

// Look ahead some number of tokens
TOKEN* look_ahead(TOKEN** token, int how_far){
    if (how_far == 0){
        printf("Same token!\n");
        exit(1);
    }
    if (how_far > 4){
        printf("Too many tokens ahead\n");
        exit(1);
    }
    if (how_far == 1){
        return ((*token)->next);
    }
    else if (how_far == 2){
        return ((*token)->next->next);
    }
    else if (how_far == 3){
        return ((*token)->next->next->next);
    }
    else{
        return ((*token)->next->next->next->next);
    }
}

// Consumes the token passed
int consume_token(TOKEN** token){
    *token = (*token)->next;
    return 1;
}
// <assignment-operator> ::= =
//                         | *=
//                         | /=
//                         | %=
//                         | +=
//                         | -=
//                         | <<=
//                         | >>=
//                         | &=
//                         | ^=
//                         | |=
int assignment_operator(TOKEN** token){

    return 1;
}
// <initializer> ::= <assignment-expression>
//                 | { <initializer-list> }
//                 | { <initializer-list> , }
int initializer(TOKEN** token){
    assignment_expression(token);
    return 1;
}
// <conditional-expression> ::= <logical-or-expression> {"?" <expression> ":" <conditional-expression>}?
int conditional_expression(TOKEN** token){
    logical_or_expression(token);
    return 1;
}
// <logical-or-expression> ::= <logical-and-expression> {"||" <logical-and-expression>}*
int logical_or_expression(TOKEN** token){
    logical_and_expression(token);
    return 1;
}
// <logical-and-expression> ::= <inclusive-or-expression> {"&&" <inclusive-or-expression>}*
int logical_and_expression(TOKEN** token){
    inclusive_or_expression(token);
    return 1;
}
// <inclusive-or-expression> ::= <exclusive-or-expression> {"|" <exclusive-or-expression>}*
int inclusive_or_expression(TOKEN** token){
    exclusive_or_expression(token);
    return 1;
}
// <exclusive-or-expression> ::= <and-expression> {"^" <and-expression>}*
int exclusive_or_expression(TOKEN** token){
    and_expression(token);
    return 1;
}
// <and-expression> ::= <equality-expression> {"&" <equality-expression>}*
int and_expression(TOKEN** token){
    equality_expression(token);
    return 1;
}

// <equality-expression> ::= <relational-expression> {"==" <relational-expression> | "!=" <relational-expression>}*
int equality_expression(TOKEN** token){
    relational_expression(token);
    return 1;
}
// <relational-expression> ::= <shift-expression>
//                          {| "<" <shift-expression>
//                           | ">" <shift-expression>
//                           | "<=" <shift-expression>
//                           | ">=" <shift-expression>}*
int relational_expression(TOKEN** token){
    shift_expression(token);
    return 1;
}
// <shift-expression> ::= <additive-expression>
//                     {| "<<" <additive-expression>
//                      | ">>" <additive-expression>}*
int shift_expression(TOKEN** token){
    additive_expression(token);
    return 1;
}
// <additive-expression> ::= <multiplicative-expression>
//                        {| "+" <multiplicative-expression>
//                         | "-" <multiplicative-expression>}*
int additive_expression(TOKEN** token){
    multiplicative_expression(token);
    return 1;
}
// <multiplicative-expression> ::= <cast-expression>
//                              {| "*" <cast-expression>
//                               | "/" <cast-expression>
//                               | "%" <cast-expression>}*
int multiplicative_expression(TOKEN** token){
    cast_expression(token);
    return 1;
}
// <cast-expression> ::= <unary-expression>
//                     | ( <type-name> ) <cast-expression>
int cast_expression(TOKEN** token){
    unary_expression(token);
    return 1;
}
// <unary-expression> ::= <postfix-expression>
//                      | ++ <unary-expression>
//                      | -- <unary-expression>
//                      | <unary-operator> <cast-expression>
//                      | sizeof <unary-expression>
//                      | sizeof <type-name>
int unary_expression(TOKEN** token){
    postfix_expression(token);
    return 1;
}
// <postfix-expression> ::= <primary-expression>
//                       {| "[" <expression> ]
//                        | "(" {<assignment-expression>}* )
//                        | "."  <identifier>
//                        | "->" <identifier>
//                        | "++"
//                        | "--"}*
int postfix_expression(TOKEN** token){
    primary_expression(token);
    return 1;
}
// <primary-expression> ::= <identifier>
//                        | <constant>
//                        | <string>
//                        | ( <expression> )
int primary_expression(TOKEN** token){
    constant(token);
    return 1;
}
// <constant> ::= <integer-constant>
//              | <character-constant>
//              | <floating-constant>
//              | <enumeration-constant>
int constant(TOKEN** token){
    if ((*token)->constType == INTEGERS)
        printf("    mov eax, ");
        print_token(*token);
        consume_token(token);
        printf("\n");
    return 1;
}
// <assignment-expression> ::= <conditional-expression>
//                           | <unary-expression> <assignment-operator> <assignment-expression>
int assignment_expression(TOKEN** token){
    conditional_expression(token);
    unary_expression(token);
    return 1;  
}
// <jump-statement> ::= goto <identifier> ;
//                    | continue ;
//                    | break ;
//                    | return {<expression>}? ;
int jump_statement(TOKEN** token){
    if ((*token)->keywdType == RETURN){
        consume_token(token);
        expression(token);
        if ((*token)->punctType == SEMICOLON){
            consume_token(token);
            printf("    ret\n");
            return 1;
        }
    }
    return 1;
}
// <expression> ::= <assignment-expression>
//                | <expression> , <assignment-expression>
int expression(TOKEN** token){
    assignment_expression(token);
    return 1;
}
// <statement> ::= <labeled-statement>
//               | <expression-statement>
//               | <compound-statement>
//               | <selection-statement>
//               | <iteration-statement>
//               | <jump-statement>
int statement(TOKEN** token){
    jump_statement(token);
    return 1;
}
// <init-declarator> ::= <declarator>
//                     | <declarator> = <initializer>
int init_declarator(TOKEN** token){
    if (declarator(token) == 1){
        consume_token(token);
    }
    if((*token)->punctType == EQUAL){
        initializer(token);
    }
    return 1;
}
// <declaration> ::=  {<declaration-specifier>}+ {<init-declarator>}* ";"
int declaration(TOKEN** token){
    declaration_specifier(token);
    init_declarator(token);
    if((*token)->punctType != SEMICOLON){
        printf("Expected ;\n");
        exit(1);
    }
    return 1;
}

// <compound-statement> ::= "{" {<declaration>}* {<statement>}* "}"
int compound_statement(TOKEN** token){
    if ((*token)->punctType == LBRACE){
        consume_token(token);
        declaration(token);
        statement(token);
        if ((*token)->punctType == RBRACE){
            consume_token(token);
            return 1;
        }
        else{
            printf("Expected }\n");
            exit(1);
        }
    }
    else{
        printf("Expected {\n");
        exit(1);
    }
    return 1;
}
// <direct-declarator> ::= <identifier>
//                       | "(" <declarator> ")"
//                      {  "[" {<constant-expression>}? "]"
//                       | "(" <parameter-type-list> ")"
//                       | "(" {<identifier>}* ")"}*
int direct_declarator(TOKEN** token){
    if ((*token)->lexElem == IDNTFR){
        if(look_ahead(token, 1)->punctType == LPAR){
            if(look_ahead(token,2)->lexElem == IDNTFR){
                printf("Not supported right now. Sorry\n");
                exit(1);
            }
            else if(look_ahead(token,2)->punctType == RPAR){
                return 2;
            }
        }
    }
    else if ((*token)->punctType == LPAR){
        declarator(token);
        if ((*token)->punctType == RPAR){
            return 1;
        }
        else{
            printf("Unmatched parentheses\n");
            exit(1);
        }
    }
    print_token(*token);
    printf("Expected identifier\n");
    exit(1);
    return 1;
}
// <declarator> ::= {<pointer>}? <direct-declarator>
int declarator(TOKEN** token){
    return direct_declarator(token);
}
// <type-specifier> ::= void
//                    | char
//                    | short
//                    | int
//                    | long
//                    | float
//                    | double
//                    | signed
//                    | unsigned
//                    | <struct-or-union-specifier>
//                    | <enum-specifier>
//                    | <typedef-name>
int type_specifier(TOKEN** token){
    if ((*token)->lexElem == KEYWRD){
        if((*token)->keywdType == VOID){
            consume_token(token);
            return 1;
        }
        else if ((*token)->keywdType == CHAR){
            consume_token(token);
            return 1;
        }
        else if ((*token)->keywdType == SHORT){
            consume_token(token);
            return 1;
        }
        else if ((*token)->keywdType == INT){                               
            consume_token(token);
            return 1;
        }
        else if ((*token)->keywdType == LONG){
            consume_token(token);
            return 1;
        }
        else if ((*token)->keywdType == FLOAT){
            consume_token(token);
            return 1;
        }
        else if ((*token)->keywdType == DOUBLE){
            consume_token(token);
            return 1;
        }
        else if ((*token)->keywdType == SIGNED){
            consume_token(token);
            return 1;
        }
        else if ((*token)->keywdType == UNSIGNED){
            consume_token(token);
            return 1;
        }
        else{
            return 0;
        }
    }
    else{
        return 0;
    }
}
// <declaration-specifier> ::= <storage-class-specifier>
//                           | <type-specifier>
//                           | <type-qualifier>
int declaration_specifier(TOKEN** token){
    //storage_class_specifier();
    int tmp = type_specifier(token);
    //type_qualifer();
    return tmp;
}
// <function-definition> ::= {<declaration-specifier>}* <declarator> {<declaration>}* <compound-statement>
int function_definition(TOKEN** token){
    // C standard says we can have multiple declaration specifiers
    for (;;){
        int tmp = declaration_specifier(token);
        if(tmp == 0) break;
    }
    if (declarator(token) == 2){
        printf("global _");
        print_token(*token);
        printf("\n_");
        print_token(*token);
        consume_token(token);
        consume_token(token);
        consume_token(token);
        printf(":\n");
    }
    declaration(token);
    compound_statement(token);
    return 1;
}


NODE* parse(TOKEN* token){
    NODE head = {};
    NODE* curr_node = &head;
    while(token->lexElem != END){
        // C says that the program starts with either 
        // a function-definition or a declaration
        // function-definitions and declarations both begin with declaration-specifiers
        function_definition(&token);
    }
    return head.next;
}
