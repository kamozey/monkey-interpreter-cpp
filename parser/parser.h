//
// Created by rahultumpala on 7/4/23.
//

#ifndef MONKEYINTERPRETER_PARSER_H
#define MONKEYINTERPRETER_PARSER_H


#include "../token/token.h"
#include "../ast/ast.h";
#include <utility>
#include <vector>

class parser {
public:

    vector<token> tokens;
    int index;
    int inputLen;

    explicit parser(vector<token> tokens) {
        index = 0;
        inputLen = tokens.size();
        this->tokens = std::move(tokens);
    }

    ast parse_input();

    ast parse_let_statement();

};


#endif //MONKEYINTERPRETER_PARSER_H
