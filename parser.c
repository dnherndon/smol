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

// A big thank you to the author of this website https://www.quut.com/c/ANSI-C-grammar-y.html
#include "smol.h"

// Function prototypes
int block_item(TOKEN** token);
int block_item_list(TOKEN** token);
int block_item_list2(TOKEN** token);
int direct_declarator(TOKEN** token);
int direct_declarator2(TOKEN** token);
int declarator(TOKEN** token);
int type_specifier(TOKEN** token);
int declaration_specifier(TOKEN** token);
int function_definition(TOKEN** token);
int expression(TOKEN** token);
int assignment_expression(TOKEN** token);
int conditional_expression(TOKEN** token);
int conditional_expression2(TOKEN** token);
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
int expression2(TOKEN** token);
int statement(TOKEN** token);
int init_declarator_list(TOKEN** token);
int init_declarator_list2(TOKEN** token);
int initializer(TOKEN** token);
int init_declarator(TOKEN** token);

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
    if (assignment_expression(token) == 1){
        return 1;
    }
    else{
        printf("Initializer lists not yet supported\n");
        exit(1);
    }
}
//  conditional_expression
//  	: logical_or_expression conditional_expression2
//  	;
int conditional_expression(TOKEN** token){
    if (logical_or_expression(token) == 1){
        if (conditional_expression2(token) == 1){
            return 1;
        }
    }
    return 0;
}
//  conditional_expression2
//  	| '?' expression ':' conditional_expression conditional_expression2
//      | epsilon
//  	;
int conditional_expression2(TOKEN** token){
    if ((*token)->punctType == QUESTION){
        consume_token(token);
        if (expression(token) == 1){
            if ((*token)->punctType == COLON){
                consume_token(token);
                if (conditional_expression(token) == 1){
                    if (conditional_expression2(token) == 1){
                        return 1;
                    }
                }
            }
        }
    }
    return 1;
}
//  logical_or_expression
//  	: logical_and_expression
//  	| logical_or_expression OR_OP logical_and_expression
//  	;
int logical_or_expression(TOKEN** token){

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
    if ((*token)->constType == INTEGERS){
        consume_token(token);
        return 1;
    }
    return 0;
}
//  assignment_expression
//  	: conditional_expression
//  	| unary_expression assignment_operator assignment_expression
//  	;
int assignment_expression(TOKEN** token){
    if(conditional_expression(token) == 1){
        return 1;
    }
    if (unary_expression(token) == 1){
        if (assignment_operator(token) == 1){
            if (assignment_expression(token) == 1){
                return 1;
            }
        }
    }
    return 0;
}
//  jump_statement
//  	: GOTO IDENTIFIER ';'
//  	| CONTINUE ';'
//  	| BREAK ';'
//  	| RETURN ';'
//  	| RETURN expression ';'
//  	;
int jump_statement(TOKEN** token){
    if ((*token)->keywdType == RETURN){
        consume_token(token);
        if (expression(token) == 1){
            if ((*token)->punctType == SEMICOLON){
                consume_token(token);
                return 1;
            }
            printf("Expected ;\n");
            exit(1);
        }
    }
    return 0;
}
//  expression
//  	: assignment_expression expression2
//  	;
int expression(TOKEN** token){
    if (assignment_expression(token) == 1){
        if (expression2(token) == 1){
            return 1;
        }
        return 0;
    }
    return 0;
}
//  expression2
//  	| ',' assignment_expression expression2
//      | epsilon
//  	;
int expression2(TOKEN** token){
    if ((*token)->punctType == COMMA){
        if (assignment_expression(token) == 1){
            if (expression2(token) == 1){
                return 1;
            }
        }
    }
    return 1;
}
//  statement
//  	: labeled_statement
//  	| compound_statement
//  	| expression_statement
//  	| selection_statement
//  	| iteration_statement
//  	| jump_statement
//  	;
int statement(TOKEN** token){
    if (jump_statement(token) == 1){
        return 1;
    }
    else{
        return 0;
    }

}
//  init_declarator
//  	: declarator '=' initializer
//  	| declarator
//  	;
int init_declarator(TOKEN** token){
    if (declarator(token) == 1){
        if ((*token)->punctType == EQUAL){
            consume_token(token);
            if (initializer(token) == 1){
                return 1;
            }
            printf("Expected initializer\n");
            exit(1);
        }
        return 1;
    }
    return 0;
}
//  init_declarator_list
//  	: init_declarator init_declarator_list2
int init_declarator_list(TOKEN** token){
    if(init_declarator(token) == 1){
        if(init_declarator_list2(token) == 1){
            return 1;
        }
        return 0;
    }
    return 0;
}
//  init_declarator_list2
//  	: ',' init_declarator init_declarator_list2
//      | epsilon
int init_declarator_list2(TOKEN** token){
    if ((*token)->punctType == COMMA){
        consume_token(token);
        if (init_declarator(token) == 1){
            if (init_declarator_list2(token) == 1){
                return 1;
            }
            return 0;
        }
        return 0;
    }
    return 1;
}
//  declaration
//  	: declaration_specifiers ';'
//  	| declaration_specifiers init_declarator_list ';'
//  	| static_assert_declaration
//  	;
int declaration(TOKEN** token){
    if (declaration_specifier(token) == 1){
        if ((*token)->punctType == SEMICOLON){
            consume_token(token);
            return 1;
        }
        if (init_declarator_list(token) == 1){
            if ((*token)->punctType == SEMICOLON){
                consume_token(token);
                return 1;
            }
            else{
                printf("Expected ;\n");
                exit(1);
            }
        }
        return 0;
    }
    return 0;
}
//  block_item_list
//  	: block_item block_item_list2
//  	;
int block_item_list(TOKEN** token){
    if(block_item(token) == 1){
        block_item_list2(token);
        return 1;
    }
    return 0;
}
//  block_item_list2
//  	: block_item block_item_list2
//      | epsilon
//  	;
int block_item_list2(TOKEN** token){
    if (block_item(token) == 1){
        block_item_list2(token);
        return 1;
    }
    return 1;
}

//  block_item
//  	: declaration
//  	| statement
//  	;
int block_item(TOKEN** token){
    if (declaration(token) == 1){
        return 1;
    }
    if (statement(token) == 1){
        return 1;
    }
    return 0;
}

//  compound_statement
//  	: '{' '}'
//  	| '{'  block_item_list '}'
//  	;
//  
int compound_statement(TOKEN** token){
    if ((*token)->punctType == LBRACE){
        consume_token(token);
        if ((*token)->punctType == RBRACE){
            consume_token(token);
            return 1;
        }
        if (block_item_list(token) == 1){
            if ((*token)->punctType == RBRACE){
                consume_token(token);
                return 1;
            }
            else{
                printf("Expected }");
                exit(1);
            }
        }
    }
    printf("Expected {");
    return 0;
}

//  direct_declarator
//  	: IDENTIFIER direct_declarator2
//  	| '(' declarator ')' direct_declarator2
//  	;
int direct_declarator(TOKEN** token){
    if ((*token)->lexElem == IDNTFR){
        consume_token(token);
        if (direct_declarator2(token) == 1){
            return 1;
        }
    }
    else if ((*token)->punctType == LPAR){
        consume_token(token);
        if (declarator(token) == 1){
            if ((*token)->punctType == RPAR){
                consume_token(token);
                if (direct_declarator2(token) == 1){
                    return 1;
                }
            }
            printf("Mismatched parentheses\n");
            exit(1);
        }
    }
    printf("Expected direct_declarator\n");
    exit(1);
}

//  direct_declarator2
//  	| [' ']' direct_declarator2
//  	| [' '*' ']' direct_declarator2
//  	| [' STATIC type_qualifier_list assignment_expression ']' direct_declarator2
//  	| [' STATIC assignment_expression ']' direct_declarator2
//  	| [' type_qualifier_list '*' ']' direct_declarator2
//  	| [' type_qualifier_list STATIC assignment_expression ']' direct_declarator2
//  	| [' type_qualifier_list assignment_expression ']' direct_declarator2
//  	| [' type_qualifier_list ']' direct_declarator2
//  	| [' assignment_expression ']' direct_declarator2
//  	| (' parameter_type_list ')' direct_declarator2
//  	| (' ')' direct_declarator2
//  	| (' identifier_list ')' direct_declarator2
//      | epsilon
int direct_declarator2(TOKEN** token){
    if ((*token)->punctType == LPAR){
        consume_token(token);
        if ((*token)->punctType == RPAR){
            consume_token(token);
            return direct_declarator2(token);
        }
    }
    return 1;
}

//  declarator
//  	: pointer direct_declarator
//  	| direct_declarator
//  	;
int declarator(TOKEN** token){
    return direct_declarator(token);
}

//  type_specifier
//  	: VOID
//  	| CHAR
//  	| SHORT
//  	| INT
//  	| LONG
//  	| FLOAT
//  	| DOUBLE
//  	| SIGNED
//  	| UNSIGNED
//  	| BOOL
//  	| COMPLEX
//  	| IMAGINARY	  	/* non-mandated extension */
//  	| atomic_type_specifier
//  	| struct_or_union_specifier
//  	| enum_specifier
//  	| TYPEDEF_NAME		/* after it has been defined as such */
//  	;
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
//  declaration_specifiers
//  	: storage_class_specifier declaration_specifiers
//  	| storage_class_specifier
//  	| type_specifier declaration_specifiers
//  	| type_specifier
//  	| type_qualifier declaration_specifiers
//  	| type_qualifier
//  	| function_specifier declaration_specifiers
//  	| function_specifier
//  	| alignment_specifier declaration_specifiers
//  	| alignment_specifier
//  	;
int declaration_specifier(TOKEN** token){
    if (type_specifier(token) == 1){
        declaration_specifier(token);
        return 1;
    }
    else{
        return 0;
    }
}
//  function_definition
//  	: declaration_specifiers declarator declaration_list compound_statement
//  	| declaration_specifiers declarator compound_statement
//  	;
int function_definition(TOKEN** token){
    if (declaration_specifier(token) != 1){
        printf("Expected declaration specifier\n");
    }
    
    if (declarator(token) != 1){
        printf("Expected declarator\n");
    }
    if (compound_statement(token) != 1){
        printf("Expected compound-statement\n");
    }
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
