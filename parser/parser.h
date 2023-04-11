//
// Created by rahultumpala on 7/4/23.
//

#ifndef MONKEYINTERPRETER_PARSER_H
#define MONKEYINTERPRETER_PARSER_H


#include "../token/token.h"
#include "../ast/ast.h"
#include "../precedence/precedence.h"
#include <vector>
#include <map>
#include <functional>

typedef astNs::expression* (*prefixParseFn)();
typedef astNs::expression* (*infixParseFn)(astNs::expression*);

class parser {
public:

    vector<token *> tokens;
    int index;
    int inputLen;
    map<tokenType,astNs::expression* (*)()> prefixParseFns;
    map<tokenType, astNs::expression* (*)(astNs::expression*)> infixParseFns;

    explicit parser(const vector<token *> &tokens) {
        index = 0;
        inputLen = (int) tokens.size();
        this->tokens = tokens;
    }

    astNs::ast *parse_input();

    astNs::astNode *parse_let_statement();

    astNs::expression *parse_expression(precedence pre);

    astNs::astNode *parse_return_statement();

    astNs::astNode *parse_expression_statement();

    void register_prefix_parsefn(tokenType t, prefixParseFn);

    void register_infix_parsefn(tokenType t, infixParseFn);

};


#endif //MONKEYINTERPRETER_PARSER_H
