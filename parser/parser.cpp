//
// Created by rahultumpala on 7/4/23.
//

#include <stdexcept>
#include "parser.h"

void parser::register_infix_parsefn(tokenType t, infixParseFn fn) {
    infixParseFns[t] = fn;
}

void parser::register_prefix_parsefn(tokenType t, prefixParseFn fn) {
    prefixParseFns[t] = fn;
}

astNs::ast *parser::parse_input() {
    vector<astNs::astNode *> program;
    while (index < inputLen) {
        token *curToken = tokens[index];
        switch (curToken->type) {
            case tokenType::let: {
                astNs::astNode *node = parse_let_statement();
                program.push_back(node);
                break;
            }
            case tokenType::returnToken: {
                astNs::astNode *node = parse_return_statement();
                program.push_back(node);
                break;
            }
            case tokenType::eof:
                index++;
                break;
        }
    }
    astNs::ast *ast = new astNs::ast(program);
    return ast;
}

astNs::astNode *parser::parse_let_statement() {
    if (index + 1 == inputLen) throw std::runtime_error("insufficient tokens to parse let statement");
    token *letToken = tokens[index];
    token *curToken = tokens[++index];
    if (tokens[index]->type != tokenType::identifier)
        throw std::runtime_error("invalid let statement");
    astNs::identifier *id = new astNs::identifier(curToken, curToken->value);

    curToken = tokens[++index]; // moving the index from id to assign
    if (curToken->type != tokenType::assign)
        throw std::runtime_error("expected assign but found something else in let stmt");
    index++; // moving from assign to start of expr

    astNs::expression *expr = parse_expression(precedence::lowest);
    astNs::statement *statement = new astNs::letStatement(letToken, id, expr);

    return statement;
}

astNs::astNode *parser::parse_return_statement() {
    token *returnToken = tokens[index++];
    astNs::expression *expr = parse_expression(precedence::lowest);
    astNs::statement *statement = new astNs::returnStatement(returnToken, expr);
    return statement;
}

astNs::astNode *parser::parse_expression_statement() {
    token *exprToken = tokens[index++];
    astNs::expression *expr = parse_expression(precedence::lowest);
    astNs::statement *stmt = new astNs::expressionStatement(exprToken, expr);
    if (tokens[index]->type == tokenType::semicolon) {
        index++;
    }
    return stmt;
}

astNs::expression *parser::parse_expression(precedence precedence) {
    astNs::expression *expr;
    astNs::expression *prev;
    while (index < inputLen && tokens[index]->type != tokenType::semicolon) {
        token *curToken = tokens[index];
        if (curToken->type == tokenType::integer) {
            int value = stoi(curToken->value);
            prev = new astNs::integerLiteral(curToken, value);
            expr = prev;
            index++;
        }
    }
    if (tokens[index]->type == tokenType::semicolon) index++; // to move ahead of semicolon token
    return expr;
}

astNs::expression *parser::parse_integer_literal() {
    token *curToken = tokens[index++];
    int value = stoi(curToken->value);
    return new astNs::integerLiteral(curToken, value);
}
