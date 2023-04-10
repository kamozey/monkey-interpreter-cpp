//
// Created by rahultumpala on 7/4/23.
//

#ifndef MONKEYINTERPRETER_PARSER_H
#define MONKEYINTERPRETER_PARSER_H


#include "../token/token.h"
#include "../ast/ast.h"
#include <utility>
#include <vector>

class parser {
public:

    vector<token *> tokens;
    int index;
    int inputLen;

    explicit parser(const vector<token *> &tokens) {
        index = 0;
        inputLen = (int) tokens.size();
        this->tokens = tokens;
    }

    astNs::ast *parse_input();

    astNs::astNode *parse_let_statement();

    astNs::expression *parse_expression();

    astNs::astNode *parse_return_statement();

};


#endif //MONKEYINTERPRETER_PARSER_H
