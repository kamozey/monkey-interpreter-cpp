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


class parser {
public:
    typedef astNs::expression *(parser::*prefixParseFn)();

    typedef astNs::expression *(parser::*infixParseFn)(astNs::expression *);

    vector<token *> tokens;
    int index;
    int inputLen;
    map<tokenType, astNs::expression *(parser::*)()> prefixParseFns;
    map<tokenType, astNs::expression *(parser::*)(astNs::expression *)> infixParseFns;

    explicit parser(const vector<token *> &tokens) {
        index = 0;
        inputLen = (int) tokens.size();
        this->tokens = tokens;
        register_prefix_parsefn(tokenType::integer, &parser::parse_integer_literal);
    }

    astNs::ast *parse_input();

    astNs::astNode *parse_let_statement();

    astNs::expression *parse_expression(precedence pre);

    astNs::astNode *parse_return_statement();

    astNs::astNode *parse_expression_statement();

    void register_prefix_parsefn(tokenType t, prefixParseFn);

    void register_infix_parsefn(tokenType t, infixParseFn);

    astNs::expression *parse_integer_literal();

};


#endif //MONKEYINTERPRETER_PARSER_H
