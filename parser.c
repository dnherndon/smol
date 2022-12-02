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
NODE** block_item(TOKEN** token, NODE** node);
NODE** block_item_list(TOKEN** token, NODE** node);
NODE** block_item_list2(TOKEN** token, NODE** node);
NODE** direct_declarator(TOKEN** token, NODE** node);
NODE** direct_declarator2(TOKEN** token, NODE** node);
NODE** declarator(TOKEN** token, NODE** node);
NODE** type_specifier(TOKEN** token, NODE** node);
NODE** declaration_specifiers(TOKEN** token, NODE** node);
NODE** function_definition(TOKEN** token, NODE** node);
NODE** expression(TOKEN** token, NODE** node);
NODE** assignment_expression(TOKEN** token, NODE** node);
NODE** conditional_expression(TOKEN** token, NODE** node);
NODE** conditional_expression2(TOKEN** token, NODE** node);
NODE** logical_or_expression(TOKEN** token, NODE** node);
NODE** logical_or_expression2(TOKEN** token, NODE** node);
NODE** logical_and_expression(TOKEN** token, NODE** node);
NODE** logical_and_expression2(TOKEN** token, NODE** node);
NODE** inclusive_or_expression(TOKEN** token, NODE** node);
NODE** inclusive_or_expression2(TOKEN** token, NODE** node);
NODE** exclusive_or_expression(TOKEN** token, NODE** node);
NODE** exclusive_or_expression2(TOKEN** token, NODE** node);
NODE** and_expression(TOKEN** token, NODE** node);
NODE** and_expression2(TOKEN** token, NODE** node);
NODE** equality_expression(TOKEN** token, NODE** node);
NODE** equality_expression2(TOKEN** token, NODE** node);
NODE** relational_expression(TOKEN** token, NODE** node);
NODE** relational_expression2(TOKEN** token, NODE** node);
NODE** shift_expression(TOKEN** token, NODE** node);
NODE** shift_expression2(TOKEN** token, NODE** node);
NODE** additive_expression(TOKEN** token, NODE** node);
NODE** additive_expression2(TOKEN** token, NODE** node);
NODE** multiplicative_expression(TOKEN** token, NODE** node);
NODE** multiplicative_expression2(TOKEN** token, NODE** node);
NODE** cast_expression(TOKEN** token, NODE** node);
NODE** cast_expression2(TOKEN** token, NODE** node);
NODE** unary_expression(TOKEN** token, NODE** node);
NODE** unary_expression2(TOKEN** token, NODE** node);
NODE** postfix_expression(TOKEN** token, NODE** node);
NODE** postfix_expression2(TOKEN** token, NODE** node);
NODE** primary_expression(TOKEN** token, NODE** node);
NODE** constant(TOKEN** token, NODE** node);
NODE** assignment_expression(TOKEN** token, NODE** node);
NODE** jump_statement(TOKEN** token, NODE** node);
NODE** expression2(TOKEN** token, NODE** node);
NODE** statement(TOKEN** token, NODE** node);
NODE** init_declarator_list(TOKEN** token, NODE** node);
NODE** init_declarator_list2(TOKEN** token, NODE** node);
NODE** initializer(TOKEN** token, NODE** node);
NODE** init_declarator(TOKEN** token, NODE** node);
NODE** type_name(TOKEN** token, NODE** node);
NODE** initializer_list(TOKEN** token, NODE** node);
NODE** unary_operator(TOKEN** token, NODE** node);
NODE** argument_expression_list(TOKEN** token, NODE** node);
NODE** translation_unit(TOKEN** token, NODE** node);
NODE** translation_unit2(TOKEN** token, NODE** node);
NODE** external_declaration(TOKEN** token, NODE** node);
NODE** expression_statement(TOKEN** token, NODE** node);
NODE** compound_statement(TOKEN** token, NODE** node);

// Place holders
NODE** type_name(TOKEN** token, NODE** node){
    return NULL;
}
NODE** initializer_list(TOKEN** token, NODE** node){
    return NULL;
}
NODE** argument_expression_list(TOKEN** token, NODE** node){
    return NULL;
}

// Consumes the token passed
void consume_token(TOKEN** token){
    *token = (*token)->next;
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
NODE** assignment_operator(TOKEN** token, NODE** node){
    return node;
}
// <initializer> ::= <assignment-expression>
//                 | { <initializer-list> }
//                 | { <initializer-list> , }
NODE** initializer(TOKEN** token, NODE** node){
    if (assignment_expression(token, node) != NULL){
        return node;
    }
    else{
        printf("Initializer lists not yet supported\n");
        exit(1);
    }
}
//  conditional_expression
//  	: logical_or_expression conditional_expression2
//  	;
NODE** conditional_expression(TOKEN** token, NODE** node){
    if (logical_or_expression(token, node) != NULL){
        if (conditional_expression2(token, node) != NULL){
            return node;
        }
    }
    return NULL;
}
//  conditional_expression2
//  	| '?' expression ':' conditional_expression conditional_expression2
//      | epsilon
//  	;
NODE** conditional_expression2(TOKEN** token, NODE** node){
    if ((*token)->punctType == QUESTION){
        consume_token(token);
        if (expression(token, node) != NULL){
            if ((*token)->punctType == COLON){
                consume_token(token);
                if (conditional_expression(token, node) != NULL){
                    if (conditional_expression2(token, node) != NULL){
                        return node;
                    }
                }
                errorAt(token);
                printf("Expected expression\n");
                exit(1);
                return NULL;
            }
        }
        errorAt(token);
        printf("Expected expression\n");
        exit(1);
        return NULL;
    }
    return node;
}
//  logical_or_expression
//  	: logical_and_expression logical_or_expression2
//  	;
NODE** logical_or_expression(TOKEN** token, NODE** node){
    if (logical_and_expression(token, node) != NULL){
        if (logical_or_expression2(token, node) != NULL){
            return node;
        }
    }
    return NULL;
}
//  logical_or_expression2
//  	| '||' logical_and_expression logical_or_expression2
//      | epsilon
//  	;
NODE** logical_or_expression2(TOKEN** token, NODE** node){
    if ((*token)->punctType == LOGOR){
        consume_token(token);
        if (logical_and_expression(token, node) != NULL){
            if (logical_or_expression2(token, node) != NULL){
                return node;
            }
        }
        errorAt(token);
        printf("Expected expression\n");
        exit(1);
        return NULL;
    }
    return node;
}
//  logical_and_expression
//  	: inclusive_or_expression logical_and_expression2
//  	;
NODE** logical_and_expression(TOKEN** token, NODE** node){
    if (inclusive_or_expression(token, node) != NULL){
        if (logical_and_expression2(token, node) != NULL){
            return node;
        }
    }
    return NULL;
}
//  logical_and_expression
//  	| '&&' inclusive_or_expression logical_and_expression2
//      | epsilon
//  	;
NODE** logical_and_expression2(TOKEN** token, NODE** node){
    if ((*token)->punctType == LOGAND){
        consume_token(token);
        if (inclusive_or_expression(token, node) != NULL){
            if (logical_and_expression2(token, node) != NULL){
                return node;
            }
        }
        errorAt(token);
        printf("Expected expression\n");
        exit(1);
        return NULL;
    }
    return node;
}
//  inclusive_or_expression
//  	: exclusive_or_expression inclusive_or_expression2
//  	;
NODE** inclusive_or_expression(TOKEN** token, NODE** node){
    if (exclusive_or_expression(token, node) != NULL){
        if (inclusive_or_expression2(token, node) != NULL){
            return node;
        }
    }
    return NULL;
}
//  inclusive_or_expression
//  	| '|' exclusive_or_expression inclusive_or_expression2
//      | epsilon
//  	;
NODE** inclusive_or_expression2(TOKEN** token, NODE** node){
    if ((*token)->punctType == BITOR){
        consume_token(token);
        if (exclusive_or_expression(token, node) != NULL){
            if (inclusive_or_expression2(token, node) != NULL){
                return node;
            }
        }
        errorAt(token);
        printf("Expected expression\n");
        exit(1);
        return NULL;
    }
    return node;
}
//  exclusive_or_expression
//  	: and_expression exclusive_or_expression2
//  	;
NODE** exclusive_or_expression(TOKEN** token, NODE** node){
    if (and_expression(token, node) != NULL){
        if (exclusive_or_expression2(token, node) != NULL){
            return node;
        }
    }
    return NULL;
}
//  exclusive_or_expression2
//  	: '^' and_expression exclusive_or_expression2
//      | epsilon
//  	;
NODE** exclusive_or_expression2(TOKEN** token, NODE** node){
    if ((*token)->punctType == BITXOR){
        consume_token(token);
        if (and_expression(token, node) != NULL){
            if (exclusive_or_expression2(token, node) != NULL){
                return node;
            }
        }
        errorAt(token);
        printf("Expected expression\n");
        exit(1);
        return NULL;
    }
    return node;
}
//  and_expression
//  	: equality_expression and_expression2
//  	;
NODE** and_expression(TOKEN** token, NODE** node){
    if (equality_expression(token, node) != NULL){
        if (and_expression2(token, node) != NULL){
            return node;
        }
    }
    return NULL;
}
//  and_expression2
//  	| '&' equality_expression and_expression2
//      | epsilon
//  	;
NODE** and_expression2(TOKEN** token, NODE** node){
    if ((*token)->punctType == BITAND){
        consume_token(token);
        if (equality_expression(token, node) != NULL){
            if (and_expression2(token, node) != NULL){
                return node;
            }
        }
        errorAt(token);
        printf("Expected expression\n");
        exit(1);
        return NULL;
    }
    return node;
}
//  equality_expression
//  	: relational_expression equality_expression2
//  	;
NODE** equality_expression(TOKEN** token, NODE** node){
    if (relational_expression(token, node) != NULL){
        if (equality_expression2(token, node) != NULL){
            return node;
        }
    }
    return NULL;
}
//  equality_expression2
//  	| '==' relational_expression equality_expression2
//  	| '!=' relational_expression equality_expression2
//      | epsilon
//  	;
NODE** equality_expression2(TOKEN** token, NODE** node){
    if ((*token)->punctType == EQUIV){
        consume_token(token);
        if (relational_expression(token, node) != NULL){
            if (equality_expression2(token, node) != NULL){
                return node;
            }
        }
        errorAt(token);
        printf("Expected expression\n");
        exit(1);
        return NULL;
    }
    else if ((*token)->punctType == NOTEQUIV){
        consume_token(token);
        if (relational_expression(token, node) != NULL){
            if (equality_expression2(token, node) != NULL){
                return node;
            }
        }
        errorAt(token);
        printf("Expected expression\n");
        exit(1);
        return NULL;
    }
    return node;
}
//  relational_expression
//  	: shift_expression relational_expression2
//  	;
NODE** relational_expression(TOKEN** token, NODE** node){
    if (shift_expression(token, node) != NULL){
        if (relational_expression2(token, node) != NULL){
            return node;
        }
    }
    return NULL;
}
//  relational_expression2
//  	| '<' shift_expression relational_expression2
//  	| '>' shift_expression relational_expression2
//  	| '<=' shift_expression relational_expression2
//  	| '>=' shift_expression relational_expression2
//      | epsilon
//  	;
NODE** relational_expression2(TOKEN** token, NODE** node){
    if ((*token)->punctType == LT){
        consume_token(token);
        if (shift_expression(token, node) != NULL){
            if (relational_expression2(token, node) != NULL){
                return node;
            }
        }
        errorAt(token);
        printf("Expected expression\n");
        exit(1);
        return NULL;
    }
    else if ((*token)->punctType == GT){
        consume_token(token);
        if (shift_expression(token, node) != NULL){
            if (relational_expression2(token, node) != NULL){
                return node;
            }
        }
        printf("Expected expression\n");
        exit(1);
        return NULL;
    }
    else if ((*token)->punctType == LE){
        consume_token(token);
        if (shift_expression(token, node) != NULL){
            if (relational_expression2(token, node) != NULL){
                return node;
            }
        }
        printf("Expected expression\n");
        exit(1);
        return NULL;
    }
    else if ((*token)->punctType == GE){
        consume_token(token);
        if (shift_expression(token, node) != NULL){
            if (relational_expression2(token, node) != NULL){
                return node;
            }
        }
        printf("Expected expression\n");
        exit(1);
        return NULL;
    }
    return node;
}
//  shift_expression
//  	: additive_expression shift_expression2
//  	;
NODE** shift_expression(TOKEN** token, NODE** node){
    if (additive_expression(token, node) != NULL){
        if (shift_expression2(token, node) != NULL){
            return node;
        }
    }
    return NULL;
}
//  shift_expression2
//  	: '<<' additive_expression shift_expression2
//  	| '>>' additive_expression shift_expression2
//      | epsilon
//  	;
NODE** shift_expression2(TOKEN** token, NODE** node){
    if ((*token)->punctType == LSHFT){
        consume_token(token);
        if (additive_expression(token, node) != NULL){
            if (shift_expression2(token, node) != NULL){
                return node;
            }
        }
        printf("Expected expression\n");
        exit(1);
        return NULL;
    }
    if ((*token)->punctType == RSHFT){
        consume_token(token);
        if (additive_expression(token, node) != NULL){
            if (shift_expression2(token, node) != NULL){
                return node;
            }
        }
        printf("Expected expression\n");
        exit(1);
        return NULL;
    }
    return node;
}
//  additive_expression
//  	: multiplicative_expression additive_expression2
//  	;
NODE** additive_expression(TOKEN** token, NODE** node){
    if (multiplicative_expression(token, node) != NULL){
        if (additive_expression2(token, node) != NULL){
            return node;
        }
    }
    return NULL;
}
//  additive_expression2
//  	: '+' multiplicative_expression additive_expression2
//  	| '-' multiplicative_expression additive_expression2
//      | epsilons
//  	;
NODE** additive_expression2(TOKEN** token, NODE** node){
    if ((*token)->punctType == PLUS){
        NODE* temp = calloc(1, sizeof(NODE));
        temp->left = (*node);
        temp->kind = NODE_ADD;
        consume_token(token);
        if (multiplicative_expression(token, node) != NULL){
            temp->right = (*node);
            (*node) = temp;
            if (additive_expression2(token, node) != NULL){
                return node;
            }
        }
        errorAt(token);
        printf("Expected expression\n");
        exit(1);
        return NULL;
    }
    else if ((*token)->punctType == MINUS){
        consume_token(token);
        if (multiplicative_expression(token, node) != NULL){
            if (additive_expression2(token, node) != NULL){
                return node;
            }
        }
        errorAt(token);
        printf("Expected expression\n");
        exit(1);
        return NULL;
    }
    return node;
}
//  multiplicative_expression
//  	: cast_expression multiplicative_expression2
//  	;
NODE** multiplicative_expression(TOKEN** token, NODE** node){
    if (cast_expression(token, node) != NULL){
        if (multiplicative_expression2(token, node) != NULL){
            return node;
        }
    }
    return NULL;
}
//  multiplicative_expression2
//  	: '*' cast_expression multiplicative_expression2
//  	| '/' cast_expression multiplicative_expression2
//  	| '%' cast_expression multiplicative_expression2
//      | epsilon
//  	;
NODE** multiplicative_expression2(TOKEN** token, NODE** node){
    if ((*token)->punctType == STAR){
        NODE* temp = calloc(1, sizeof(NODE));
        temp->left = (*node);
        temp->kind = NODE_MUL;
        consume_token(token);
        if (cast_expression(token, node) != NULL){
            temp->right = (*node);
            (*node) = temp;
            if (multiplicative_expression2(token, node) != NULL){
                return node;
            }
        }
        errorAt(token);
        printf("Expected expression\n");
        exit(1);
        return NULL;
    }
    if ((*token)->punctType == FWDSLSH){
        consume_token(token);
        if (cast_expression(token, node) != NULL){
            if (multiplicative_expression2(token, node) != NULL){
                return node;
            }
        }
        errorAt(token);
        printf("Expected expression\n");
        exit(1);
        return NULL;
    }
    if ((*token)->punctType == PERCENT){
        consume_token(token);
        if (cast_expression(token, node) != NULL){
            if (multiplicative_expression2(token, node) != NULL){
                return node;
            }
        }
        errorAt(token);
        printf("Expected expression\n");
        exit(1);
        return NULL;
    }
    return node;
}
//  cast_expression
//  	: unary_expression
//  	| '(' type_name ')' cast_expression
//  	;
NODE** cast_expression(TOKEN** token, NODE** node){
    if (unary_expression(token, node) != NULL){
        return node;
    }
    else if ((*token)->punctType == LPAR){
        consume_token(token);
        if (type_name(token, node) != NULL){
            if ((*token)->punctType == RPAR){
                consume_token(token);
                if (cast_expression(token, node) != NULL){
                    return node;
                }
            }
            errorAt(token);
            printf("Mismatched parentheses\n");
            exit(1);
        }
    }
    return NULL;
}
//  unary_expression
//  	: postfix_expression
//  	| '++' unary_expression
//  	| '--' unary_expression
//  	| unary_operator cast_expression
//  	| 'sizeof' unary_expression
//  	| 'sizeof' '(' type_name ')'
//  	| 'alignof' '(' type_name ')'
//  	;
NODE** unary_expression(TOKEN** token, NODE** node){
    if (postfix_expression(token, node) != NULL){
        return node;
    }
    else if ((*token)->punctType == INCREMENT){
        consume_token(token);
        if (unary_expression(token, node) != NULL){
            return node;
        }
        errorAt(token);
        printf("Expected expression\n");
        exit(1);
        return NULL;
    }
    else if ((*token)->punctType == DECREMENT){
        consume_token(token);
        if (unary_expression(token, node) != NULL){
            return node;
        }
        errorAt(token);
        printf("Expected expression\n");
        exit(1);
        return NULL;
    }
    else if (unary_operator(token, node) != NULL){
        if (cast_expression(token, node) != NULL){
            return node;
        }
    }
    else if ((*token)->keywdType == SIZEOF){
        consume_token(token);
        if (unary_expression(token, node) != NULL){
            return node;
        }
    }
    else if ((*token)->keywdType == SIZEOF){
        consume_token(token);
        if ((*token)->punctType == LPAR){
            consume_token(token);
            if (type_name(token, node) != NULL){
                if((*token)->punctType == RPAR){
                    consume_token(token);
                    return node;
                }
                errorAt(token);
                printf("Mismatched parentheses\n");
                exit(1);
            }
        }
    }
    else if ((*token)->keywdType == _ALIGNOF){
        consume_token(token);
        if ((*token)->punctType == LPAR){
            consume_token(token);
            if (type_name(token, node) != NULL){
                if((*token)->punctType == RPAR){
                    consume_token(token);
                    return node;
                }
                errorAt(token);
                printf("Mismatched parentheses\n");
                exit(1);
            }
        }
    }
    return NULL;
}
//  unary_operator
//  	: '&'
//  	| '*'
//  	| '+'
//  	| '-'
//  	| '~'
//  	| '!'
//  	;
NODE** unary_operator(TOKEN** token, NODE** node){
    if ((*token)->punctType == BITAND){
        consume_token(token);
        return node;
    }
    else if ((*token)->punctType == STAR){
        consume_token(token);
        return node;
    }
    else if ((*token)->punctType == PLUS){
        consume_token(token);
        return node;
    }
    else if ((*token)->punctType == MINUS){
        consume_token(token);
        return node;
    }
    else if ((*token)->punctType == BITNOT){
        consume_token(token);
        return node;
    }
    else if ((*token)->punctType == EXCLAMATION){
        consume_token(token);
        return node;
    }
    return NULL;
}
//  postfix_expression
//  	: primary_expression postfix_expression2
//  	| '(' type_name ')' '{' initializer_list '}' postfix_expression2
//  	| '(' type_name ')' '{' initializer_list ',' '}' postfix_expression2
//  	;
NODE** postfix_expression(TOKEN** token, NODE** node){
    if (primary_expression(token, node) != NULL){
        if (postfix_expression2(token, node) != NULL){
            return node;
        }
    }
    else if ((*token)->punctType == LPAR){
        consume_token(token);
        if (type_name(token, node) != NULL){
            if ((*token)->punctType == RPAR){
                consume_token(token);
                if ((*token)->punctType == LBRACE){
                    consume_token(token);
                    if (initializer_list(token, node) != NULL){
                        if ((*token)->punctType == RBRACE){
                            consume_token(token);
                            if (postfix_expression2(token, node) != NULL){
                                return node;
                            }
                        }
                        else if ((*token)->punctType == COMMA){
                            consume_token(token);
                            if ((*token)->punctType == RBRACE){
                                consume_token(token);
                                if (postfix_expression2(token, node) != NULL){
                                    return node;
                                }
                            }
                            else{
                                errorAt(token);
                                printf("Mismatched braces\n");
                                exit(1);
                            }
                        }
                        else{
                            errorAt(token);
                            printf("Expected brace or comma\n");
                            exit(1);
                        }
                    }
                }
            }
            else{
                errorAt(token);
                printf("Mismatched parentheses\n");
                exit(1);
            }
        }
    }
    return NULL;
}
//  postfix_expression2
//  	| '[' expression ']' postfix_expression2
//  	| '(' ')' postfix_expression2
//  	| '(' argument_expression_list ')' postfix_expression2
//  	| '.' IDENTIFIER postfix_expression2
//  	| '->' IDENTIFIER postfix_expression2
//  	| '++' postfix_expression2
//  	| '--' postfix_expression2
//      | epsilon
//  	;
NODE** postfix_expression2(TOKEN** token, NODE** node){
    if ((*token)->punctType == LBRACK){
        consume_token(token);
        if (expression(token, node) != NULL){
            if ((*token)->punctType == RBRACK){
                consume_token(token);
                if (postfix_expression2(token, node) != NULL){
                    return node;
                }
            }
            else{
                errorAt(token);
                printf("Mismatched brackets\n");
                exit(1);
            }
        }
    }
    else if ((*token)->punctType == LPAR){
        consume_token(token);
        if((*token)->punctType == RPAR){
            consume_token(token);
            if (postfix_expression2(token, node) != NULL){
                return node;
            }
        }
        else if (argument_expression_list(token, node) != NULL){
            if ((*token)->punctType == RPAR){
                consume_token(token);
                if (postfix_expression2(token, node) != NULL){
                    return node;
                }
            }
            errorAt(token);
            printf("Mismatched parentheses\n");
            exit(1);
        }
        else{
            errorAt(token);
            printf("Mismatched parentheses\n");
            exit(1);
        }
    }
    else if ((*token)->punctType == DOT){
        consume_token(token);
        if ((*token)->lexElem == IDNTFR){
            consume_token(token);
            if (postfix_expression2(token, node) != NULL){
                return node;
            }
        }
        errorAt(token);
        printf("Expected expression\n");
        exit(1);
        return NULL;
    }
    else if ((*token)->punctType == DEREFERENCE){
        consume_token(token);
        if ((*token)->lexElem == IDNTFR){
            consume_token(token);
            if (postfix_expression2(token, node) != NULL){
                return node;
            }
        }
        errorAt(token);
        printf("Expected expression\n");
        exit(1);
        return NULL;
    }
    else if ((*token)->punctType == INCREMENT){
        consume_token(token);
        if (postfix_expression2(token, node) != NULL){
            return node;
        }
        errorAt(token);
        printf("Expected expression\n");
        exit(1);
        return NULL;
    }
    else if ((*token)->punctType == DECREMENT){
        consume_token(token);
        if (postfix_expression2(token, node) != NULL){
            return node;
        }
        errorAt(token);
        printf("Expected expression\n");
        exit(1);
        return NULL;
    }
    return node;
}
//  primary_expression
//  	: IDENTIFIER
//  	| constant
//  	| string //TODO:
//  	| '(' expression ')'
//  	| generic_selection //TODO:
//  	;
NODE** primary_expression(TOKEN** token, NODE** node){
    if ((*token)->lexElem == IDNTFR){
        consume_token(token);
        return node;
    }
    else if (constant(token, node) != NULL){
        return node;
    }
    else if ((*token)->punctType == LPAR){
        consume_token(token);
        if (expression(token, node) != NULL){
            if((*token)->punctType == RPAR){
                consume_token(token);
                return node;
            }
            else{
                errorAt(token);
                printf("Mismatched parentheses\n");
                exit(1);
            }
        }
    }
    return NULL;
}
//  constant
//  	: I_CONSTANT		/* includes character_constant */
//  	| F_CONSTANT //TODO:
//  	| ENUMERATION_CONSTANT	//TODO: /* after it has been defined as such */
//  	;
NODE** constant(TOKEN** token, NODE** node){
    if ((*token)->lexElem == CONSTANTS){
        (*node) = calloc(1, sizeof(NODE));
        (*node)->kind = NODE_INT;
        (*node)->constantVal = (*token)->constantVal;
        consume_token(token);
        return node;
    }
    return NULL;
}
//  assignment_expression
//  	: conditional_expression
//  	| unary_expression assignment_operator assignment_expression
//  	;
NODE** assignment_expression(TOKEN** token, NODE** node){
    if(conditional_expression(token, node) != NULL){
        return node;
    }
    if (unary_expression(token, node) != NULL){
        if (assignment_operator(token, node) != NULL){
            if (assignment_expression(token, node) != NULL){
                return node;
            }
        }
    }
    return NULL;
}
//  jump_statement
//  	: GOTO IDENTIFIER ';'
//  	| CONTINUE ';'
//  	| BREAK ';'
//  	| RETURN ';'
//  	| RETURN expression ';'
//  	;
NODE** jump_statement(TOKEN** token, NODE** node){
    if ((*token)->keywdType == RETURN){
        consume_token(token);
        if (expression(token, node) != NULL){
            if ((*token)->punctType == SEMICOLON){
                consume_token(token);
                return node;
            }
            errorAt(token);
            printf("Expected ;\n");
            exit(1);
        }
    }
    return NULL;
}
//  expression
//  	: assignment_expression expression2
//  	;
NODE** expression(TOKEN** token, NODE** node){
    if (assignment_expression(token, node) != NULL){
        if (expression2(token, node) != NULL){
            return node;
        }
        return NULL;
    }
    return NULL;
}
//  expression2
//  	| ',' assignment_expression expression2
//      | epsilon
//  	;
NODE** expression2(TOKEN** token, NODE** node){
    if ((*token)->punctType == COMMA){
        if (assignment_expression(token, node) != NULL){
            if (expression2(token, node) != NULL){
                return node;
            }
        }
    }
    return node;
}

//  expression_statement
//  	: ';'
//  	| expression ';'
//  	;
NODE** expression_statement(TOKEN** token, NODE** node){
    if ((*token)->punctType == SEMICOLON){
        consume_token(token);
        return node;
    }
    if (expression(token, node) != NULL){
        if ((*token)->punctType == SEMICOLON){
            consume_token(token);
            return node;
        }
        errorAt(token);
        printf("Expected ;\n");
        exit(1);
    }
    return NULL;
}
//  statement
//  	: labeled_statement
//  	| compound_statement
//  	| expression_statement
//  	| selection_statement
//  	| iteration_statement
//  	| jump_statement
//  	;
NODE** statement(TOKEN** token, NODE** node){
    if (compound_statement(token, node) != NULL){
        return node;
    }
    if (expression_statement(token, node) != NULL){
        return node;
    }
    if (jump_statement(token, node) != NULL){
        return node;
    }
    return NULL;
}
//  init_declarator
//  	: declarator '=' initializer
//  	| declarator
//  	;
NODE** init_declarator(TOKEN** token, NODE** node){
    if (declarator(token, node) != NULL){
        if ((*token)->punctType == EQUAL){
            consume_token(token);
            if (initializer(token, node) != NULL){
                return node;
            }
            errorAt(token);
            printf("Expected initializer\n");
            exit(1);
        }
        return node;
    }
    return NULL;
}
//  init_declarator_list
//  	: init_declarator init_declarator_list2
NODE** init_declarator_list(TOKEN** token, NODE** node){
    if(init_declarator(token, node) != NULL){
        if(init_declarator_list2(token, node) != NULL){
            return node;
        }
        return NULL;
    }
    return NULL;
}
//  init_declarator_list2
//  	: ',' init_declarator init_declarator_list2
//      | epsilon
NODE** init_declarator_list2(TOKEN** token, NODE** node){
    if ((*token)->punctType == COMMA){
        consume_token(token);
        if (init_declarator(token, node) != NULL){
            if (init_declarator_list2(token, node) != NULL){
                return node;
            }
            return NULL;
        }
        return NULL;
    }
    return node;
}
//  declaration
//  	: declaration_specifiers ';'
//  	| declaration_specifiers init_declarator_list ';'
//  	| static_assert_declaration
//  	;
NODE** declaration(TOKEN** token, NODE** node){
    if (declaration_specifiers(token, node) != NULL){
        if ((*token)->punctType == SEMICOLON){
            consume_token(token);
            return node;
        }
        if (init_declarator_list(token, node) != NULL){
            if ((*token)->punctType == SEMICOLON){
                consume_token(token);
                return node;
            }
            else{
                errorAt(token);
                printf("Expected ;\n");
                exit(1);
            }
        }
        return NULL;
    }
    return NULL;
}
//  block_item_list
//  	: block_item block_item_list2
//  	;
NODE** block_item_list(TOKEN** token, NODE** node){
    if(block_item(token, node) != NULL){
        block_item_list2(token, node);
        return node;
    }
    return NULL;
}
//  block_item_list2
//  	: block_item block_item_list2
//      | epsilon
//  	;
NODE** block_item_list2(TOKEN** token, NODE** node){
    if (block_item(token, node) != NULL){
        block_item_list2(token, node);
        return node;
    }
    return node;
}

//  block_item
//  	: declaration
//  	| statement
//  	;
NODE** block_item(TOKEN** token, NODE** node){
    if (declaration(token, node) != NULL){
        return node;
    }
    if (statement(token, node) != NULL){
        return node;
    }
    return NULL;
}

//  compound_statement
//  	: '{' '}'
//  	| '{'  block_item_list '}'
//  	;
//  
NODE** compound_statement(TOKEN** token, NODE** node){
    if ((*token)->punctType == LBRACE){
        consume_token(token);
        if ((*token)->punctType == RBRACE){
            consume_token(token);
            return node;
        }
        if (block_item_list(token, node) != NULL){
            if ((*token)->punctType == RBRACE){
                consume_token(token);
                return node;
            }
            else{
                errorAt(token);
                printf("Expected }\n");
                exit(1);
            }
        }
    }
    return NULL;
}

//  direct_declarator
//  	: IDENTIFIER direct_declarator2
//  	| '(' declarator ')' direct_declarator2
//  	;
NODE** direct_declarator(TOKEN** token, NODE** node){
    if ((*token)->lexElem == IDNTFR){
        consume_token(token);
        if (direct_declarator2(token, node) != NULL){
            return node;
        }
    }
    else if ((*token)->punctType == LPAR){
        consume_token(token);
        if (declarator(token, node) != NULL){
            if ((*token)->punctType == RPAR){
                consume_token(token);
                if (direct_declarator2(token, node) != NULL){
                    return node;
                }
            }
            errorAt(token);
            printf("Mismatched parentheses\n");
            exit(1);
        }
    }
    errorAt(token);
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
NODE** direct_declarator2(TOKEN** token, NODE** node){
    if ((*token)->punctType == LPAR){
        consume_token(token);
        if ((*token)->punctType == RPAR){
            consume_token(token);
            if(direct_declarator2(token, node) != NULL){
                return node;
            }
        }
    }
    return node;
}

//  declarator
//  	: pointer direct_declarator
//  	| direct_declarator
//  	;
NODE** declarator(TOKEN** token, NODE** node){
    if (direct_declarator(token, node) != NULL){
        return node;
    }
    return NULL;
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
NODE** type_specifier(TOKEN** token, NODE** node){
    if ((*token)->lexElem == KEYWRD){
        if((*token)->keywdType == VOID){
            consume_token(token);
            return node;
        }
        else if ((*token)->keywdType == CHAR){
            consume_token(token);
            return node;
        }
        else if ((*token)->keywdType == SHORT){
            consume_token(token);
            return node;
        }
        else if ((*token)->keywdType == INT){                               
            consume_token(token);
            return node;
        }
        else if ((*token)->keywdType == LONG){
            consume_token(token);
            return node;
        }
        else if ((*token)->keywdType == FLOAT){
            consume_token(token);
            return node;
        }
        else if ((*token)->keywdType == DOUBLE){
            consume_token(token);
            return node;
        }
        else if ((*token)->keywdType == SIGNED){
            consume_token(token);
            return node;
        }
        else if ((*token)->keywdType == UNSIGNED){
            consume_token(token);
            return node;
        }
        else{
            return NULL;
        }
    }
    else{
        return NULL;
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
NODE** declaration_specifiers(TOKEN** token, NODE** node){
    if (type_specifier(token, node) != NULL){
        declaration_specifiers(token, node);
        return node;
    }
    else{
        return NULL;
    }
}
//  function_definition
//  	: declaration_specifiers declarator declaration_list compound_statement
//  	| declaration_specifiers declarator compound_statement
//  	;
NODE** function_definition(TOKEN** token, NODE** node){
    if (declaration_specifiers(token, node) != NULL){
        if (declarator(token, node) != NULL){
            if (compound_statement(token, node) != NULL){
                return node;
            }
        }
    }
    return NULL;
}
//  external_declaration
//  	: function_definition
//  	| declaration
//  	;
NODE** external_declaration(TOKEN** token, NODE** node){
    if (function_definition(token, node) != NULL){
        return node;
    }
    if (declaration(token, node) != NULL){
        return node;
    }
    return NULL;
}

//  translation_unit
//  	: external_declaration translation_unit2
//  	;
NODE** translation_unit(TOKEN** token, NODE** node){
    if (external_declaration(token, node) != NULL){
        if (translation_unit2(token, node) != NULL){
            return node;
        }
    }
    return NULL;
}
//  translation_unit2
//  	: external_declaration translation_unit2
//      | epsilon
//  	;
NODE** translation_unit2(TOKEN** token, NODE** node){
    if (external_declaration(token, node) != NULL){
        if (translation_unit2(token, node) != NULL){
            return node;
        }
    }
    return node;
}


NODE* parse(TOKEN* token){
    NODE head = {};
    NODE* node = &head;
    for(;;){
        if (translation_unit(&token, &node) == NULL){
            break;
        }
    }
    return node;
}
