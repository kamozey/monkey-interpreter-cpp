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
    map<tokenType, precedence> precedences;

    explicit parser(const vector<token *> &tokens) {
        index = 0;
        inputLen = (int) tokens.size();
        this->tokens = tokens;
        perform_function_registrations();
        setup_precedences_table();
    }

    astNs::ast *parse_input();

    astNs::astNode *parse_let_statement();

    astNs::expression *parse_expression(precedence pre);

    astNs::astNode *parse_return_statement();

    void perform_function_registrations();

    void setup_precedences_table();

    astNs::astNode *parse_expression_statement();

    astNs::expression *parse_integer_literal();

    astNs::expression *parse_identifier();

    astNs::expression *parse_prefix_expression();

    precedence get_precedence(tokenType t);

    astNs::expression *parse_infix_expression(astNs::expression *leftExpr);

    astNs::expression *parse_grouped_expression();

    astNs::expression *parse_if_expression();

    void expectToken(tokenType t);

    vector<astNs::astNode *> *parse_statements();

    astNs::astNode *parse_if_statement();
};


#endif //MONKEYINTERPRETER_PARSER_H
