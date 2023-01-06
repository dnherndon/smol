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

// Symbol table
symbolTable* currentTable;

// Function prototypes
int block_item(TOKEN** token, NODE** node);
int block_item_list(TOKEN** token, NODE** node);
int block_item_list2(TOKEN** token, NODE** node);
int direct_declarator(TOKEN** token, NODE** node);
int direct_declarator2(TOKEN** token, NODE** node);
int declarator(TOKEN** token, NODE** node);
int type_specifier(TOKEN** token, NODE** node);
int declaration_specifiers(TOKEN** token, NODE** node);
int function_definition(TOKEN** token, NODE** node);
int expression(TOKEN** token, NODE** node);
int assignment_expression(TOKEN** token, NODE** node);
int conditional_expression(TOKEN** token, NODE** node);
int conditional_expression2(TOKEN** token, NODE** node);
int logical_or_expression(TOKEN** token, NODE** node);
int logical_or_expression2(TOKEN** token, NODE** node);
int logical_and_expression(TOKEN** token, NODE** node);
int logical_and_expression2(TOKEN** token, NODE** node);
int inclusive_or_expression(TOKEN** token, NODE** node);
int inclusive_or_expression2(TOKEN** token, NODE** node);
int exclusive_or_expression(TOKEN** token, NODE** node);
int exclusive_or_expression2(TOKEN** token, NODE** node);
int and_expression(TOKEN** token, NODE** node);
int and_expression2(TOKEN** token, NODE** node);
int equality_expression(TOKEN** token, NODE** node);
int equality_expression2(TOKEN** token, NODE** node);
int relational_expression(TOKEN** token, NODE** node);
int relational_expression2(TOKEN** token, NODE** node);
int shift_expression(TOKEN** token, NODE** node);
int shift_expression2(TOKEN** token, NODE** node);
int additive_expression(TOKEN** token, NODE** node);
int additive_expression2(TOKEN** token, NODE** node);
int multiplicative_expression(TOKEN** token, NODE** node);
int multiplicative_expression2(TOKEN** token, NODE** node);
int cast_expression(TOKEN** token, NODE** node);
int cast_expression2(TOKEN** token, NODE** node);
int unary_expression(TOKEN** token, NODE** node);
int unary_expression2(TOKEN** token, NODE** node);
int postfix_expression(TOKEN** token, NODE** node);
int postfix_expression2(TOKEN** token, NODE** node);
int primary_expression(TOKEN** token, NODE** node);
int constant(TOKEN** token, NODE** node);
int assignment_expression(TOKEN** token, NODE** node);
int jump_statement(TOKEN** token, NODE** node);
int expression2(TOKEN** token, NODE** node);
int statement(TOKEN** token, NODE** node);
int init_declarator_list(TOKEN** token, NODE** node);
int init_declarator_list2(TOKEN** token, NODE** node);
int initializer(TOKEN** token, NODE** node);
int init_declarator(TOKEN** token, NODE** node);
int type_name(TOKEN** token, NODE** node);
int initializer_list(TOKEN** token, NODE** node);
int argument_expression_list(TOKEN** token, NODE** node);
int translation_unit(TOKEN** token, NODE** node);
int translation_unit2(TOKEN** token, NODE** node);
int external_declaration(TOKEN** token, NODE** node);
int expression_statement(TOKEN** token, NODE** node);
int compound_statement(TOKEN** token, NODE** node);

// Node creation functions
NODE* newNode(nodeKind kind){
    NODE* temp = calloc(1, sizeof(NODE));
    if (temp == NULL){
        printf("Node memory not allocated\n");
        exit(1);
    }
    temp->kind = kind;
    return temp;
}
// Symbol creation function
SYMBOL* newSymbol(symbolKind kind, char* name)
{
    SYMBOL* temp = calloc(1, sizeof(SYMBOL));
     if (temp == NULL){
        printf("Symbol memory not allocated\n");
        exit(1);
    }
    temp->kind = kind;
    temp->name = name;
    return temp;
}
// Unary nodes
NODE* unaryOne(NODE** node, nodeKind kind)
{
    NODE* temp = newNode(kind);
    return temp;
}
void unaryTwo(NODE** node, NODE* unaryOneNode)
{
    unaryOneNode->left = (*node);
    (*node) = unaryOneNode;
}
// Binary Nodes
NODE* binaryOne(NODE** node, nodeKind kind)
{
    NODE* temp = newNode(kind);
    temp->left = (*node);
    return temp;
}
void binaryTwo(NODE** node, NODE* binaryOneNode)
{
    binaryOneNode->right = (*node);
    (*node) = binaryOneNode;
    return;
}
// Statement Nodes
NODE* statementOne(NODE** node, nodeKind kind)
{
    NODE* temp = newNode(kind);
    temp->right = (*node);
    return temp;
}
void statementTwo(NODE** node, NODE* statementOneNode)
{
    statementOneNode->left = (*node);
    (*node) = statementOneNode;
    return;
}
// Declaration Nodes
NODE* declarationOne(NODE** node, nodeKind kind)
{
    NODE* temp = newNode(kind);
    temp->right = (*node);
    return temp;
}
void declarationTwo(NODE** node, NODE* declarationOneNode)
{
    declarationOneNode->left = (*node);
    (*node) = declarationOneNode;
    return;
}
// Constant Node
void constantNode(NODE** node, nodeKind kind)
{
    NODE* temp = newNode(kind);
    (*node) = temp;
}
// Identifier Node
void identifierNode(NODE** node, nodeKind kind)
{
    NODE* temp = newNode(kind);
    (*node) = temp;
}

// Place holders
int type_name(TOKEN** token, NODE** node)
{
    return 0;
}
int initializer_list(TOKEN** token, NODE** node)
{
    return 0;
}
int argument_expression_list(TOKEN** token, NODE** node)
{
    return 0;
}

// Consumes the token passed
void consume_token(TOKEN** token)
{
    *token = (*token)->next;
}
// Backtracks one token
void backtrack_token(TOKEN** token)
{
    *token = (*token)->last;
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
int assignment_operator(TOKEN** token, NODE** node)
{
    if ((*token)->punctType == EQUAL){
        consume_token(token);
        return 1;
    }
    return 0;
}
//  initializer
//  	: '{' initializer_list '}'
//  	| '{' initializer_list ',' '}'
//  	| assignment_expression
//  	;
int initializer(TOKEN** token, NODE** node)
{
    if (assignment_expression(token, node) == 1){
        return 1;
    } else{
        printf("Initializer lists not yet supported\n");
        exit(1);
    }
}
//  conditional_expression
//  	: logical_or_expression conditional_expression2
//  	;
int conditional_expression(TOKEN** token, NODE** node)
{
    if (logical_or_expression(token, node) == 1){
        if (conditional_expression2(token, node) == 1){
            return 1;
        }
    }
    return 0;
}
//  conditional_expression2
//  	| '?' expression ':' conditional_expression conditional_expression2
//      | epsilon
//  	;
int conditional_expression2(TOKEN** token, NODE** node)
{
    if ((*token)->punctType == QUESTION){
        consume_token(token);
        if (expression(token, node) == 1){
            if ((*token)->punctType == COLON){
                consume_token(token);
                if (conditional_expression(token, node) == 1){
                    if (conditional_expression2(token, node) == 1){
                        return 1;
                    }
                }
                errorAt(token, "Expected expression\n");
                return 0;
            }
        }
        errorAt(token, "Expected expression\n");
        return 0;
    }
    return 1;
}
//  logical_or_expression
//  	: logical_and_expression logical_or_expression2
//  	;
int logical_or_expression(TOKEN** token, NODE** node)
{
    if (logical_and_expression(token, node) == 1){
        if (logical_or_expression2(token, node) == 1){
            return 1;
        }
    }
    return 0;
}
//  logical_or_expression2
//  	| '||' logical_and_expression logical_or_expression2
//      | epsilon
//  	;
int logical_or_expression2(TOKEN** token, NODE** node)
{
    if ((*token)->punctType == LOGOR){
        consume_token(token);
        if (logical_and_expression(token, node) == 1){
            if (logical_or_expression2(token, node) == 1){
                return 1;
            }
        }
        errorAt(token, "Expected expression\n");
        return 0;
    }
    return 1;
}
//  logical_and_expression
//  	: inclusive_or_expression logical_and_expression2
//  	;
int logical_and_expression(TOKEN** token, NODE** node)
{
    if (inclusive_or_expression(token, node) == 1){
        if (logical_and_expression2(token, node) == 1){
            return 1;
        }
    }
    return 0;
}
//  logical_and_expression
//  	| '&&' inclusive_or_expression logical_and_expression2
//      | epsilon
//  	;
int logical_and_expression2(TOKEN** token, NODE** node)
{
    if ((*token)->punctType == LOGAND){
        NODE* temp = binaryOne(node, NODE_LOGAND);
        consume_token(token);
        if (inclusive_or_expression(token, node) == 1){
            binaryTwo(node, temp);
            if (logical_and_expression2(token, node) == 1){
                return 1;
            }
        }
        errorAt(token, "Expected expression\n");
        return 0;
    }
    return 1;
}
//  inclusive_or_expression
//  	: exclusive_or_expression inclusive_or_expression2
//  	;
int inclusive_or_expression(TOKEN** token, NODE** node)
{
    if (exclusive_or_expression(token, node) == 1){
        if (inclusive_or_expression2(token, node) == 1){
            return 1;
        }
    }
    return 0;
}
//  inclusive_or_expression
//  	| '|' exclusive_or_expression inclusive_or_expression2
//      | epsilon
//  	;
int inclusive_or_expression2(TOKEN** token, NODE** node)
{
    if ((*token)->punctType == BITOR){
        consume_token(token);
        if (exclusive_or_expression(token, node) == 1){
            if (inclusive_or_expression2(token, node) == 1){
                return 1;
            }
        }
        errorAt(token, "Expected expression\n");
        return 0;
    }
    return 1;
}
//  exclusive_or_expression
//  	: and_expression exclusive_or_expression2
//  	;
int exclusive_or_expression(TOKEN** token, NODE** node)
{
    if (and_expression(token, node) == 1){
        if (exclusive_or_expression2(token, node) == 1){
            return 1;
        }
    }
    return 0;
}
//  exclusive_or_expression2
//  	: '^' and_expression exclusive_or_expression2
//      | epsilon
//  	;
int exclusive_or_expression2(TOKEN** token, NODE** node)
{
    if ((*token)->punctType == BITXOR){
        consume_token(token);
        if (and_expression(token, node) == 1){
            if (exclusive_or_expression2(token, node) == 1){
                return 1;
            }
        }
        errorAt(token, "Expected expression\n");
        return 0;
    }
    return 1;
}
//  and_expression
//  	: equality_expression and_expression2
//  	;
int and_expression(TOKEN** token, NODE** node)
{
    if (equality_expression(token, node) == 1){
        if (and_expression2(token, node) == 1){
            return 1;
        }
    }
    return 0;
}
//  and_expression2
//  	| '&' equality_expression and_expression2
//      | epsilon
//  	;
int and_expression2(TOKEN** token, NODE** node)
{
    if ((*token)->punctType == BITAND){
        consume_token(token);
        if (equality_expression(token, node) == 1){
            if (and_expression2(token, node) == 1){
                return 1;
            }
        }
        errorAt(token, "Expected expression\n");
        return 0;
    }
    return 1;
}
//  equality_expression
//  	: relational_expression equality_expression2
//  	;
int equality_expression(TOKEN** token, NODE** node)
{
    if (relational_expression(token, node) == 1){
        if (equality_expression2(token, node) == 1){
            return 1;
        }
    }
    return 0;
}
//  equality_expression2
//  	| '==' relational_expression equality_expression2
//  	| '!=' relational_expression equality_expression2
//      | epsilon
//  	;
int equality_expression2(TOKEN** token, NODE** node)
{
    if ((*token)->punctType == EQUIV){
        NODE* temp = binaryOne(node, NODE_EQUIV);
        consume_token(token);
        if (relational_expression(token, node) == 1){
            binaryTwo(node, temp);
            if (equality_expression2(token, node) == 1){
                return 1;
            }
        }
        errorAt(token, "Expected expression\n");
        return 0;
    } else if ((*token)->punctType == NOTEQUIV){
        consume_token(token);
        NODE* temp = binaryOne(node, NODE_NEQUIV);
        if (relational_expression(token, node) == 1){
            binaryTwo(node, temp);
            if (equality_expression2(token, node) == 1){
                return 1;
            }
        }
        errorAt(token, "Expected expression\n");
        return 0;
    }
    return 1;
}
//  relational_expression
//  	: shift_expression relational_expression2
//  	;
int relational_expression(TOKEN** token, NODE** node)
{
    if (shift_expression(token, node) == 1){
        if (relational_expression2(token, node) == 1){
            return 1;
        }
    }
    return 0;
}
//  relational_expression2
//  	| '<' shift_expression relational_expression2
//  	| '>' shift_expression relational_expression2
//  	| '<=' shift_expression relational_expression2
//  	| '>=' shift_expression relational_expression2
//      | epsilon
//  	;
int relational_expression2(TOKEN** token, NODE** node)
{
    if ((*token)->punctType == LT){
        consume_token(token);
        if (shift_expression(token, node) == 1){
            if (relational_expression2(token, node) == 1){
                return 1;
            }
        }
        errorAt(token, "Expected expression\n");
        return 0;
    } else if ((*token)->punctType == GT){
        consume_token(token);
        if (shift_expression(token, node) == 1){
            if (relational_expression2(token, node) == 1){
                return 1;
            }
        }
        errorAt(token, "Expected expression\n");
        return 0;
    } else if ((*token)->punctType == LE){
        consume_token(token);
        if (shift_expression(token, node) == 1){
            if (relational_expression2(token, node) == 1){
                return 1;
            }
        }
        errorAt(token, "Expected expression\n");
        return 0;
    } else if ((*token)->punctType == GE){
        consume_token(token);
        if (shift_expression(token, node) == 1){
            if (relational_expression2(token, node) == 1){
                return 1;
            }
        }
        errorAt(token, "Expected expression\n");
        return 0;
    }
    return 1;
}
//  shift_expression
//  	: additive_expression shift_expression2
//  	;
int shift_expression(TOKEN** token, NODE** node)
{
    if (additive_expression(token, node) == 1){
        if (shift_expression2(token, node) == 1){
            return 1;
        }
    }
    return 0;
}
//  shift_expression2
//  	: '<<' additive_expression shift_expression2
//  	| '>>' additive_expression shift_expression2
//      | epsilon
//  	;
int shift_expression2(TOKEN** token, NODE** node)
{
    if ((*token)->punctType == LSHFT){
        consume_token(token);
        if (additive_expression(token, node) == 1){
            if (shift_expression2(token, node) == 1){
                return 1;
            }
        }
        errorAt(token, "Expected expression\n");
        return 0;
    }
    if ((*token)->punctType == RSHFT){
        consume_token(token);
        if (additive_expression(token, node) == 1){
            if (shift_expression2(token, node) == 1){
                return 1;
            }
        }
        errorAt(token, "Expected expression\n");
        return 0;
    }
    return 1;
}
//  additive_expression
//  	: multiplicative_expression additive_expression2
//  	;
int additive_expression(TOKEN** token, NODE** node)
{
    if (multiplicative_expression(token, node) == 1){
        if (additive_expression2(token, node) == 1){
            return 1;
        }
    }
    return 0;
}
//  additive_expression2
//  	: '+' multiplicative_expression additive_expression2
//  	| '-' multiplicative_expression additive_expression2
//      | epsilons
//  	;
int additive_expression2(TOKEN** token, NODE** node)
{
    if ((*token)->punctType == PLUS){
        NODE* temp = binaryOne(node, NODE_ADD);
        consume_token(token);
        if (multiplicative_expression(token, node) == 1){
            binaryTwo(node, temp);
            if (additive_expression2(token, node) == 1){
                return 1;
            }
        }
        errorAt(token, "Expected expression\n");
        return 0;
    } else if ((*token)->punctType == MINUS){
        NODE* temp = binaryOne(node, NODE_SUB);
        consume_token(token);
        if (multiplicative_expression(token, node) == 1){
            binaryTwo(node, temp);
            if (additive_expression2(token, node) == 1){
                return 1;
            }
        }
        errorAt(token, "Expected expression\n");
        return 0;
    }
    return 1;
}
//  multiplicative_expression
//  	: cast_expression multiplicative_expression2
//  	;
int multiplicative_expression(TOKEN** token, NODE** node)
{
    if (cast_expression(token, node) == 1){
        if (multiplicative_expression2(token, node) == 1){
            return 1;
        }
    }
    return 0;
}
//  multiplicative_expression2
//  	: '*' cast_expression multiplicative_expression2
//  	| '/' cast_expression multiplicative_expression2
//  	| '%' cast_expression multiplicative_expression2
//      | epsilon
//  	;
int multiplicative_expression2(TOKEN** token, NODE** node)
{
    if ((*token)->punctType == STAR){
        NODE* temp = binaryOne(node, NODE_MUL);
        consume_token(token);
        if (cast_expression(token, node) == 1){
            binaryTwo(node, temp);
            if (multiplicative_expression2(token, node) == 1){
                return 1;
            }
        }
        errorAt(token, "Expected expression\n");
        return 0;
    }
    if ((*token)->punctType == FWDSLSH){
        NODE* temp = binaryOne(node, NODE_DIV);
        consume_token(token);
        if (cast_expression(token, node) == 1){
            binaryTwo(node, temp);
            if (multiplicative_expression2(token, node) == 1){
                return 1;
            }
        }
        errorAt(token, "Expected expression\n");
        return 0;
    }
    if ((*token)->punctType == PERCENT){
        consume_token(token);
        NODE* temp = binaryOne(node, NODE_MOD);
        if (cast_expression(token, node) == 1){
            binaryTwo(node, temp);
            if (multiplicative_expression2(token, node) == 1){
                return 1;
            }
        }
        errorAt(token, "Expected expression\n");
        return 0;
    }
    return 1;
}
//  cast_expression
//  	: unary_expression
//  	| '(' type_name ')' cast_expression
//  	;
int cast_expression(TOKEN** token, NODE** node)
{
    if (unary_expression(token, node) == 1){
        return 1;
    } else if ((*token)->punctType == LPAR){
        consume_token(token);
        if (type_name(token, node) == 1){
            if ((*token)->punctType == RPAR){
                consume_token(token);
                if (cast_expression(token, node) == 1){
                    return 1;
                }
            }
            errorAt(token, "Mismatched parentheses. Expected \')\'\n");
            return 0;
        }
    }
    return 0;
}
//  unary_expression
//  	: postfix_expression
//  	| '++' unary_expression
//  	| '--' unary_expression
//  	: '&' cast_expression
//  	| '*' cast_expression
//  	| '+' cast_expression
//  	| '-' cast_expression
//  	| '~' cast_expression
//  	| '!' cast_expression
//  	| 'sizeof' unary_expression
//  	| 'sizeof' '(' type_name ')'
//  	| 'alignof' '(' type_name ')'
//  	;
int unary_expression(TOKEN** token, NODE** node)
{
    if (postfix_expression(token, node) == 1){
        return 1;
    } else if ((*token)->punctType == INCREMENT){
        consume_token(token);
        if (unary_expression(token, node) == 1){
            return 1;
        }
        errorAt(token, "Expected expression\n");
        return 0;
    } else if ((*token)->punctType == DECREMENT){
        consume_token(token);
        if (unary_expression(token, node) == 1){
            return 1;
        }
        errorAt(token, "Expected expression\n");
        return 0;
    } else if ((*token)->punctType == BITAND){
        consume_token(token);
        if (cast_expression(token, node) == 1){
            return 1;
        }
        errorAt(token, "Expected expression\n");
    } else if ((*token)->punctType == STAR){
        consume_token(token);
        if (cast_expression(token, node) == 1){
            return 1;
        }
        errorAt(token, "Expected expression\n");
    } else if ((*token)->punctType == PLUS){
        consume_token(token);
        if (cast_expression(token, node) == 1){
            return 1;
        }
        errorAt(token, "Expected expression\n");
    } else if ((*token)->punctType == MINUS){
        NODE* temp = unaryOne(node, NODE_NEGATE);
        consume_token(token);
        if (cast_expression(token, node) == 1){
            unaryTwo(node, temp);
            return 1;
        }
        errorAt(token, "Expected expression\n");
    } else if ((*token)->punctType == BITNOT){
        consume_token(token);
        if (cast_expression(token, node) == 1){
            return 1;
        }
        errorAt(token, "Expected expression\n");
    } else if ((*token)->punctType == EXCLAMATION){
        consume_token(token);
        if (cast_expression(token, node) == 1){
            return 1;
        }
        errorAt(token, "Expected expression\n");
    } else if ((*token)->keywdType == SIZEOF){
        consume_token(token);
        if (unary_expression(token, node) == 1){
            return 1;
        }
    } else if ((*token)->keywdType == SIZEOF){
        consume_token(token);
        if ((*token)->punctType == LPAR){
            consume_token(token);
            if (type_name(token, node) == 1){
                if((*token)->punctType == RPAR){
                    consume_token(token);
                    return 1;
                }
                errorAt(token, "Mismatched parentheses. Expected \')\'\n");
                return 0;
            }
        }
    } else if ((*token)->keywdType == _ALIGNOF){
        consume_token(token);
        if ((*token)->punctType == LPAR){
            consume_token(token);
            if (type_name(token, node) == 1){
                if((*token)->punctType == RPAR){
                    consume_token(token);
                    return 1;
                }
                errorAt(token, "Mismatched parentheses. Expected \')\'\n");
                return 0;
            }
        }
    }
    return 0;
}
//  postfix_expression
//  	: primary_expression postfix_expression2
//  	| '(' type_name ')' '{' initializer_list '}' postfix_expression2
//  	| '(' type_name ')' '{' initializer_list ',' '}' postfix_expression2
//  	;
int postfix_expression(TOKEN** token, NODE** node)
{
    if (primary_expression(token, node) == 1){
        if (postfix_expression2(token, node) == 1){
            return 1;
        }
    } else if ((*token)->punctType == LPAR){
        consume_token(token);
        if (type_name(token, node) == 1){
            if ((*token)->punctType == RPAR){
                consume_token(token);
                if ((*token)->punctType == LBRACE){
                    consume_token(token);
                    if (initializer_list(token, node) == 1){
                        if ((*token)->punctType == RBRACE){
                            consume_token(token);
                            if (postfix_expression2(token, node) == 1){
                                return 1;
                            }
                        } else if ((*token)->punctType == COMMA){
                            consume_token(token);
                            if ((*token)->punctType == RBRACE){
                                consume_token(token);
                                if (postfix_expression2(token, node) == 1){
                                    return 1;
                                }
                            } else{
                                errorAt(token, "Mismatched braces. Expected \'}\'\n");
                            }
                        } else{
                            errorAt(token, "Expected brace or comma\n");
                        }
                    }
                }
            } else{
                errorAt(token, "Mismatched parentheses. Expected \')\'\n");
            }
        }
    }
    return 0;
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
int postfix_expression2(TOKEN** token, NODE** node)
{
    if ((*token)->punctType == LBRACK){
        consume_token(token);
        if (expression(token, node) == 1){
            if ((*token)->punctType == RBRACK){
                consume_token(token);
                if (postfix_expression2(token, node) == 1){
                    return 1;
                }
            } else{
                errorAt(token, "Mismatched brackets. Expected \']\'\n");
            }
        }
    } else if ((*token)->punctType == LPAR){
        consume_token(token);
        if((*token)->punctType == RPAR){
            if (searchScope(currentTable, (*node)->symbolName) == NULL){
                errorAt(token, "Implicit function declaration of \'%s\'\n", (*node)->symbolName);    
            }
            (*node)->kind = NODE_FUNCALL;
            consume_token(token);
            if (postfix_expression2(token, node) == 1){
                return 1;
            }
        } else if (argument_expression_list(token, node) == 1){
            if ((*token)->punctType == RPAR){
                consume_token(token);
                if (postfix_expression2(token, node) == 1){
                    return 1;
                }
            }
            errorAt(token, "Mismatched parentheses. Expected \')\'\n");
        } else{
            errorAt(token, "Mismatched parentheses. Expected \')\'\n");
        }
    } else if ((*token)->punctType == DOT){
        consume_token(token);
        if ((*token)->lexElem == IDNTFR){
            consume_token(token);
            if (postfix_expression2(token, node) == 1){
                return 1;
            }
        }
        errorAt(token, "Expected expression\n");
        return 0;
    } else if ((*token)->punctType == DEREFERENCE){
        consume_token(token);
        if ((*token)->lexElem == IDNTFR){
            consume_token(token);
            if (postfix_expression2(token, node) == 1){
                return 1;
            }
        }
        errorAt(token, "Expected expression\n");
        return 0;
    } else if ((*token)->punctType == INCREMENT){
        consume_token(token);
        if (postfix_expression2(token, node) == 1){
            return 1;
        }
        errorAt(token, "Expected expression\n");
        return 0;
    } else if ((*token)->punctType == DECREMENT){
        consume_token(token);
        if (postfix_expression2(token, node) == 1){
            return 1;
        }
        errorAt(token, "Expected expression\n");
        return 0;
    }
    if ((*node)->kind == NODE_IDENTIFIER){
        // Epsilon case, must be a variable if not a func but is ident
        symTblEntry* entry = symTblGet(currentTable, (*node)->symbolName);
        if (entry == NULL){
            errorAt(token, "Symbol \'%s\' not defined in this scope\n", (*node)->symbolName);
        }
        (*node)->kind = NODE_VAR;
        return 1;
    }
    
    return 1;
}
//  primary_expression
//  	: IDENTIFIER
//  	| constant
//  	| string //TODO:
//  	| '(' expression ')'
//  	| generic_selection //TODO:
//  	;
int primary_expression(TOKEN** token, NODE** node)
{
    if ((*token)->lexElem == IDNTFR){
        identifierNode(node, NODE_IDENTIFIER);
        (*node)->symbolName = (*token)->tokenContent;
        consume_token(token);
        return 1;
    } else if (constant(token, node) == 1){
        return 1;
    } else if ((*token)->punctType == LPAR){
        consume_token(token);
        if (expression(token, node) == 1){
            if((*token)->punctType == RPAR){
                consume_token(token);
                return 1;
            } else{
                errorAt(token, "Mismatched parentheses. Expected \')\'\n");
            }
        }
    }
    return 0;
}
//  constant
//  	: I_CONSTANT		/* includes character_constant */
//  	| F_CONSTANT //TODO:
//  	| ENUMERATION_CONSTANT	//TODO: /* after it has been defined as such */
//  	;
int constant(TOKEN** token, NODE** node)
{
    if ((*token)->lexElem == CONSTANTS){
        constantNode(node, NODE_INT);
        (*node)->constantVal = (*token)->constantVal;
        consume_token(token);
        return 1;
    }
    return 0;
}
//  assignment_expression
//  	: conditional_expression
//  	| unary_expression assignment_operator assignment_expression
//  	;
int assignment_expression(TOKEN** token, NODE** node)
{
    // Unary expression is a subgroup of conditional expression
    // So, we need to find out if the conditional expression is an assignable
    // LVALUE or not
    if (conditional_expression(token, node) == 1){
        if (assignment_operator(token, node) == 1){
            NODE* temp = binaryOne(node, NODE_ASSIGN);
            if ((*node)->kind != NODE_VAR){
                printf("node: %d\n", (*node)->kind);
                errorAt(token, "Not a modifiable LVALUE\n");
            }
            if (assignment_expression(token, node) == 1){
                binaryTwo(node, temp);
                return 1;
            }
        }
        return 1;
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
int jump_statement(TOKEN** token, NODE** node)
{
    if ((*token)->keywdType == RETURN){
        NODE* temp = unaryOne(node, NODE_RETURN);
        consume_token(token);
        if (expression(token, node) == 1){
            if ((*token)->punctType == SEMICOLON){
                unaryTwo(node, temp);
                consume_token(token);
                return 1;
            }
            errorAt(token, "Terminator error. Expected \';\'\n");
        }
    }
    return 0;
}
//  expression
//  	: assignment_expression expression2
//  	;
int expression(TOKEN** token, NODE** node)
{
    if (assignment_expression(token, node) == 1){
        if (expression2(token, node) == 1){
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
int expression2(TOKEN** token, NODE** node)
{
    if ((*token)->punctType == COMMA){
        if (assignment_expression(token, node) == 1){
            if (expression2(token, node) == 1){
                return 1;
            }
        }
    }
    return 1;
}

//  expression_statement
//  	: ';'
//  	| expression ';'
//  	;
int expression_statement(TOKEN** token, NODE** node)
{
    if ((*token)->punctType == SEMICOLON){
        consume_token(token);
        return 1;
    }
    if (expression(token, node) == 1){
        if ((*token)->punctType == SEMICOLON){
            consume_token(token);
            return 1;
        }
        errorAt(token, "Terminator error. Expected \';\'\n");
    }
    return 0;
}
//  statement
//  	: labeled_statement
//  	| compound_statement
//  	| expression_statement
//  	| selection_statement
//  	| iteration_statement
//  	| jump_statement
//  	;
int statement(TOKEN** token, NODE** node)
{
    NODE* temp = statementOne(node, NODE_STATEMENT);
    if (compound_statement(token, node) == 1){
        statementTwo(node, temp);
        return 1;
    }
    if (expression_statement(token, node) == 1){
        statementTwo(node, temp);
        return 1;
    }
    if (jump_statement(token, node) == 1){
        statementTwo(node, temp);
        return 1;
    }
    return 0;
}
//  init_declarator
//  	: declarator '=' initializer
//  	| declarator
//  	;
int init_declarator(TOKEN** token, NODE** node)
{
    if (declarator(token, node) == 1){
        if ((*token)->punctType == EQUAL){
            // To get initializers to work, need to
            // go to before the equals sign.
            // This essentially treats what comes after
            // the declspec as just an assignment that
            // has the added semantic action of placing the
            // variable in the symbol table
            backtrack_token(token);
            if (initializer(token, node) == 1){
                return 1;
            }
            errorAt(token, "Expected initializer\n");
        }
        return 1;
    }
    return 0;
}
//  init_declarator_list
//  	: init_declarator init_declarator_list2
int init_declarator_list(TOKEN** token, NODE** node)
{
    if(init_declarator(token, node) == 1){
        if(init_declarator_list2(token, node) == 1){
            return 1;
        }
        return 0;
    }
    return 0;
}
//  init_declarator_list2
//  	: ',' init_declarator init_declarator_list2
//      | epsilon
int init_declarator_list2(TOKEN** token, NODE** node)
{
    if ((*token)->punctType == COMMA){
        consume_token(token);
        if (init_declarator(token, node) == 1){
            if (init_declarator_list2(token, node) == 1){
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
int declaration(TOKEN** token, NODE** node)
{
    if (declaration_specifiers(token, node) == 1){
        if ((*token)->punctType == SEMICOLON){
            consume_token(token);
            return 1;
        }
        NODE* temp = declarationOne(node, NODE_DECLARATION);
        (temp)->symbolName = (*token)->tokenContent;
        symTblEntry* entry = symTblInsert(currentTable, (temp)->symbolName, newSymbol(SYM_VAR, (temp)->symbolName)); 
        if (entry == NULL){
            errorAt(token, "Redefinition of symbol \'%s\' in this scope\n", (temp)->symbolName);
        }
        currentTable->totalLocalVars++;
        entry->value->offset = currentTable->totalLocalVars;
        NODE* decl = unaryOne(node, NODE_DECLASS);
        if (init_declarator_list(token, node) == 1){
            unaryTwo(node, decl);
            if ((*token)->punctType == SEMICOLON){
                declarationTwo(node, temp);
                consume_token(token);
                return 1;
            } else{
                errorAt(token, "Terminator error. Expected \';\'\n");
            }
        }
        return 0;
    }
    return 0;
}
//  block_item_list
//  	: block_item block_item_list2
//  	;
int block_item_list(TOKEN** token, NODE** node)
{
    if(block_item(token, node) == 1){
        block_item_list2(token, node);
        return 1;
    }
    return 0;
}
//  block_item_list2
//  	: block_item block_item_list2
//      | epsilon
//  	;
int block_item_list2(TOKEN** token, NODE** node)
{
    if (block_item(token, node) == 1){
        block_item_list2(token, node);
        return 1;
    }
    return 1;
}

//  block_item
//  	: declaration
//  	| statement
//  	;
int block_item(TOKEN** token, NODE** node)
{
    if (declaration(token, node) == 1){
        return 1;
    } else if (statement(token, node) == 1){
        return 1;
    }
    return 0;
}

//  compound_statement
//  	: '{' '}'
//  	| '{'  block_item_list '}'
//  	;
//  
int compound_statement(TOKEN** token, NODE** node)
{
    if ((*token)->punctType == LBRACE){
        consume_token(token);
        if ((*token)->punctType == RBRACE){
            consume_token(token);
            return 1;
        }
        if (block_item_list(token, node) == 1){
            if ((*token)->punctType == RBRACE){
                consume_token(token);
                return 1;
            } else{
                errorAt(token, "Brace mismatch. Expected \'}\'\n");
            }
        }
    }
    return 0;
}

//  direct_declarator
//  	: IDENTIFIER direct_declarator2
//  	| '(' declarator ')' direct_declarator2
//  	;
int direct_declarator(TOKEN** token, NODE** node)
{
    if ((*token)->lexElem == IDNTFR){
        consume_token(token);
        if (direct_declarator2(token, node) == 1){
            return 1;
        }
    } else if ((*token)->punctType == LPAR){
        consume_token(token);
        if (declarator(token, node) == 1){
            if ((*token)->punctType == RPAR){
                consume_token(token);
                if (direct_declarator2(token, node) == 1){
                    return 1;
                }
            }
            errorAt(token, "Mismatched parentheses. Expected \')\'\n");
        }
    }
    errorAt(token, "Expected an identifier\n");
    return 0;
}

//  direct_declarator2
//  	| '[' ']' direct_declarator2
//  	| '[' '*' ']' direct_declarator2
//  	| '[' STATIC type_qualifier_list assignment_expression ']' direct_declarator2
//  	| '[' STATIC assignment_expression ']' direct_declarator2
//  	| '[' type_qualifier_list '*' ']' direct_declarator2
//  	| '[' type_qualifier_list STATIC assignment_expression ']' direct_declarator2
//  	| '[' type_qualifier_list assignment_expression ']' direct_declarator2
//  	| '[' type_qualifier_list ']' direct_declarator2
//  	| '[' assignment_expression ']' direct_declarator2
//  	| '(' parameter_type_list ')' direct_declarator2
//  	| '(' ')' direct_declarator2
//  	| '(' identifier_list ')' direct_declarator2
//      | epsilon
int direct_declarator2(TOKEN** token, NODE** node)
{
    if ((*token)->punctType == LPAR){
        consume_token(token);
        if ((*token)->punctType == RPAR){
            consume_token(token);
            if(direct_declarator2(token, node) == 1){
                return 1;
            }
        }
    }
    return 1;
}

//  declarator
//  	: pointer direct_declarator
//  	| direct_declarator
//  	;
int declarator(TOKEN** token, NODE** node)
{
    if (direct_declarator(token, node) == 1){
        return 1;
    }
    return 0;
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
int type_specifier(TOKEN** token, NODE** node)
{
    if ((*token)->lexElem == KEYWRD){
        if((*token)->keywdType == VOID){
            consume_token(token);
            return 1;
        } else if ((*token)->keywdType == CHAR){
            consume_token(token);
            return 1;
        } else if ((*token)->keywdType == SHORT){
            consume_token(token);
            return 1;
        } else if ((*token)->keywdType == INT){                               
            consume_token(token);
            return 1;
        } else if ((*token)->keywdType == LONG){
            consume_token(token);
            return 1;
        } else if ((*token)->keywdType == FLOAT){
            consume_token(token);
            return 1;
        } else if ((*token)->keywdType == DOUBLE){
            consume_token(token);
            return 1;
        } else if ((*token)->keywdType == SIGNED){
            consume_token(token);
            return 1;
        } else if ((*token)->keywdType == UNSIGNED){
            consume_token(token);
            return 1;
        } else{
            return 0;
        }
    } else{
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
int declaration_specifiers(TOKEN** token, NODE** node)
{
    if (type_specifier(token, node) == 1){
        declaration_specifiers(token, node);
        return 1;
    } else{
        return 0;
    }
}
//  function_definition
//  	: declaration_specifiers declarator declaration_list compound_statement
//  	| declaration_specifiers declarator compound_statement
//  	;
int function_definition(TOKEN** token, NODE** node)
{
    NODE* temp = binaryOne(node, NODE_FUNCDEC);
    if (declaration_specifiers(token, node) == 1){
        temp->symbolName = (*token)->tokenContent;
        symTblEntry* entry = symTblInsert(currentTable, (*token)->tokenContent, newSymbol(SYM_FUNC, temp->symbolName));
        if (entry == NULL){
            errorAt(token, "Redefinition of symbol \'%s\' in this scope\n", (*token)->tokenContent);
        }
        entry->value->scope = createSymbolTable(16);
        enterScope(&currentTable, &entry->value->scope);
        if (declarator(token, node) == 1){
            if (compound_statement(token, node) == 1){
                binaryTwo(node, temp);
                exitScope(&currentTable);
                return 1;
            }
        }
    }
    return 0;
}
//  external_declaration
//  	: function_definition
//  	| declaration
//  	;
int external_declaration(TOKEN** token, NODE** node)
{
    if (function_definition(token, node) == 1){
        return 1;
    }
    //if (declaration(token, node) == 1){
    //    return 1;
    //}
    return 0;
}

//  translation_unit
//  	: external_declaration translation_unit2
//  	;
int translation_unit(TOKEN** token, NODE** node)
{
    if (external_declaration(token, node) == 1){
        if (translation_unit2(token, node) == 1){
            return 1;
        }
    }
    return 0;
}
//  translation_unit2
//  	: external_declaration translation_unit2
//      | epsilon
//  	;
int translation_unit2(TOKEN** token, NODE** node)
{
    if (external_declaration(token, node) == 1){
        if (translation_unit2(token, node) == 1){
            return 1;
        }
    }
    return 1;
}


NODE* parse(TOKEN* token, symbolTable* table)
{
    currentTable = table;
    NODE head = {};
    NODE* node = &head;
    for(;;){
        if (translation_unit(&token, &node) == 0){
            break;
        }
    }
    return node;
}
