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

class parser
{
public:
    typedef astNs::expression *(parser::*prefixParseFn)();

    typedef astNs::expression *(parser::*infixParseFn)(astNs::expression *);

    vector<token *> tokens;
    int index;
    int inputLen;
    map<tokenType, astNs::expression *(parser::*)()> prefixParseFns;
    map<tokenType, astNs::expression *(parser::*)(astNs::expression *)> infixParseFns;
    map<tokenType, precedence> precedences;

    explicit parser(const vector<token *> &tokens)
    {
        index = 0;
        inputLen = (int)tokens.size();
        this->tokens = tokens;
        perform_function_registrations();
        setup_precedences_table();
    }

    astNs::program *parse_input();

    astNs::statement *parse_let_statement();

    astNs::expression *parse_expression(precedence pre);

    astNs::statement *parse_return_statement();

    void perform_function_registrations();

    void setup_precedences_table();

    astNs::statement *parse_expression_statement();

    astNs::expression *parse_integer_literal();

    astNs::expression *parse_string_literal();

    astNs::expression *parse_identifier();

    astNs::expression *parse_prefix_expression();

    precedence get_precedence(tokenType t);

    astNs::expression *parse_infix_expression(astNs::expression *leftExpr);

    astNs::expression *parse_grouped_expression();

    astNs::expression *parse_if_expression();

    void expectToken(tokenType t);

    astNs::statement *parse_statement();

    astNs::blockStatement *parse_block_statement();

    astNs::expression *parse_function_expression();

    vector<astNs::expression *> parse_function_arguments();

    astNs::expression *parse_call_expression(astNs::expression *expr);

    astNs::expression *parse_boolean_expression();

    astNs::expression *parse_array_expression();

    astNs::expression *parse_hash_literal();

    astNs::expression *parse_array_access_expression(astNs::expression *leftExpr);

    bool validElementAccessTok(token *tok);
};

#endif // MONKEYINTERPRETER_PARSER_H
