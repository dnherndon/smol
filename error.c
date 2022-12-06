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

void unexpected_token_error(char* scanned){
    printf("Unexpected token: %c\n", *scanned);
    exit(1);
}
void errorAt(TOKEN** token, const char* format, ...){
    va_list args;
    if(((*token) == NULL) || (*token)->last == NULL){
        printf("Null token\n");
        print_token(*token);
        exit(1);
    }
    int col = (*token)->last->location + (*token)->last->length - (*token)->last->tokenLinePos;
    printf("Error at L%d:C%d\n", (*token)->last->tokenLineNumber, col);
    printf("%.*s\n", col, (*token)->last->tokenLinePos);
    printf("%*s", col, "");
    printf("^\n");
    va_start(args, format);
    printf(format, args);
    va_end(args);
    exit(1);
}