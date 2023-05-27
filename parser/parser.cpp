//
// Created by rahultumpala on 7/4/23.
//

#include <stdexcept>
#include "parser.h"
#include<iostream>

astNs::program *parser::parse_input() {
    vector<astNs::statement *> statements;
    while (index < inputLen && tokens[index]->type != tokenType::eof) {
        astNs::statement *stmt = parse_statement();
        statements.push_back(stmt);
    }
    // todo : passing statements as reference here -> may give nullptr exception if vector is empty-> figure out how to handle that
    astNs::program *program = new astNs::program(statements);
    return program;
}

astNs::statement *parser::parse_statement() {
    token *curToken = tokens[index];
    switch (curToken->type) {
        case tokenType::let: {
            return parse_let_statement();
        }
        case tokenType::returnToken: {
            return parse_return_statement();
        }
        case tokenType::eof:
            index++;
        default:
            return parse_expression_statement();
    }
}

astNs::statement *parser::parse_let_statement() {
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

astNs::statement *parser::parse_return_statement() {
    token *returnToken = tokens[index++];
    astNs::expression *expr = parse_expression(precedence::lowest);
    astNs::statement *statement = new astNs::returnStatement(returnToken, expr);
    return statement;
}

astNs::statement *parser::parse_expression_statement() {
    token *exprToken = tokens[index];
    // if (token::is_reserved_keyword(exprToken->token_literal())) {
    //     index++;
    // }
    astNs::expression *expr = parse_expression(precedence::lowest);
    astNs::statement *stmt = new astNs::expressionStatement(exprToken, expr);
    if (tokens[index]->type == tokenType::semicolon) {
        index++;
    }
    return stmt;
}

astNs::expression *parser::parse_expression(precedence precdnc) {
    token *curToken = tokens[index];
    if (prefixParseFns.find(curToken->type) == prefixParseFns.end()) {
        throw std::runtime_error(
                "no prefix parse function registered for token type " + token::token_type_string(curToken->type));
    }
    astNs::expression *(parser::*left_expr_fptr)() = prefixParseFns[curToken->type];
    astNs::expression *left_expr = (this->*left_expr_fptr)();

    // no need to increment since invoking the left_expr_fptr will automatically advance the tokens
    token *nextToken = tokens[index];
    while (nextToken->type != tokenType::semicolon && precdnc < get_precedence(nextToken->type)) {
        // this block is executed only if `nextToken` is pointing to an infix operator
        if (infixParseFns.find(nextToken->type) == infixParseFns.end()) {
            // no infix parse fn registered for the next token's tokenType
            return left_expr;
        }
        astNs::expression *(parser::*infix_expr_fptr)(astNs::expression *) = infixParseFns[nextToken->type];
        // overriding left_expr with the result of parsing the infix expression with left operand as left_expr
        // and right operand as the result of parse_expression(infix_operator_precedence) starting after infix operator
        left_expr = (this->*infix_expr_fptr)(left_expr);
        nextToken = tokens[index];
    }
    if (tokens[index]->type == tokenType::semicolon) index++;
    prevExpr = left_expr;
    return left_expr;
}

astNs::expression *parser::parse_integer_literal() {
    token *curToken = tokens[index++];
    int value = stoi(curToken->value);
    return new astNs::integerLiteral(curToken, value);
}

void parser::perform_function_registrations() {
    prefixParseFns[tokenType::integer] = &parser::parse_integer_literal;
    prefixParseFns[tokenType::identifier] = &parser::parse_identifier;
    prefixParseFns[tokenType::bang] = &parser::parse_prefix_expression;
    prefixParseFns[tokenType::minus] = &parser::parse_prefix_expression;
    prefixParseFns[tokenType::lparen] = &parser::parse_grouped_expression;
    prefixParseFns[tokenType::ifToken] = &parser::parse_if_expression;
    prefixParseFns[tokenType::fn] = &parser::parse_function_expression;
    prefixParseFns[tokenType::booleanToken] = &parser::parse_boolean_expression;
    prefixParseFns[tokenType::stringToken] = &parser::parse_string_literal;
    prefixParseFns[tokenType::lbracket] = &parser::parse_array_expression;

    infixParseFns[tokenType::plus] = &parser::parse_infix_expression;
    infixParseFns[tokenType::minus] = &parser::parse_infix_expression;
    infixParseFns[tokenType::division] = &parser::parse_infix_expression;
    infixParseFns[tokenType::asterisk] = &parser::parse_infix_expression;
    infixParseFns[tokenType::eq] = &parser::parse_infix_expression;
    infixParseFns[tokenType::neq] = &parser::parse_infix_expression;
    infixParseFns[tokenType::lt] = &parser::parse_infix_expression;
    infixParseFns[tokenType::lte] = &parser::parse_infix_expression;
    infixParseFns[tokenType::gt] = &parser::parse_infix_expression;
    infixParseFns[tokenType::gte] = &parser::parse_infix_expression;
    infixParseFns[tokenType::lparen] = &parser::parse_call_expression;
    infixParseFns[tokenType::lbracket] = &parser::parse_array_expression;
}

astNs::expression *parser::parse_identifier() {
    token *curToken = tokens[index++];
    return new astNs::identifier(curToken, curToken->value);
}

astNs::expression *parser::parse_prefix_expression() {
    token *curToken = tokens[index++];
    astNs::prefixExpression *expr = new astNs::prefixExpression(curToken, curToken->token_literal());
    expr->right = parse_expression(precedence::prefix);
    return expr;
}

astNs::expression *parser::parse_infix_expression(astNs::expression *leftExpr) {
    token *curToken = tokens[index]; // infix operator
    index++; // tokens[index] set to right operand
    astNs::infixExpression *expr = new astNs::infixExpression(curToken, curToken->token_literal(), leftExpr);
    precedence precdnc = get_precedence(curToken->type);
    expr->right = parse_expression(precdnc);
    return expr;
}

void parser::setup_precedences_table() {
    precedences[tokenType::eq] = precedence::equals;
    precedences[tokenType::neq] = precedence::equals;
    precedences[tokenType::gte] = precedence::equals;
    precedences[tokenType::lte] = precedence::equals;
    precedences[tokenType::lt] = precedence::lessgreater;
    precedences[tokenType::gt] = precedence::lessgreater;
    precedences[tokenType::plus] = precedence::sum;
    precedences[tokenType::minus] = precedence::sum;
    precedences[tokenType::division] = precedence::product;
    precedences[tokenType::asterisk] = precedence::product;
    precedences[tokenType::lparen] = precedence::call;
    precedences[tokenType::lbracket] = precedence::call;
}

precedence parser::get_precedence(tokenType t) {
    if (precedences.find(t) != precedences.end()) return precedences[t];
    return precedence::lowest;
}

astNs::expression *parser::parse_grouped_expression() {
    // tokens[index] now points to tokenType::lparen
    index++;
    precedence precdnc = get_precedence(tokens[index]->type);
    astNs::expression *expr = parse_expression(precdnc);
    expectToken(tokenType::rparen);
    index++;
    return expr;
}

astNs::expression *parser::parse_if_expression() {
    // if expression need not have return statement. if there then ok else last line is return value;
    astNs::ifExpression *ifExpr = new astNs::ifExpression(tokens[index]);
    index++;
    expectToken(tokenType::lparen);
    index++;
    ifExpr->condition = parse_expression(precedence::lowest);
    expectToken(tokenType::rparen);
    index++;
    expectToken(tokenType::lbrace);
    ifExpr->evalTrue = parse_block_statement();
    if (tokens[index]->type == tokenType::elseToken) {
        index++;
        expectToken(tokenType::lbrace);
        ifExpr->evalFalse = parse_block_statement();
    }
    return ifExpr;
}

void parser::expectToken(tokenType t) {
    if (tokens[index]->type != t)
        throw std::runtime_error(
                "expected " + token::token_type_string(t) + " but got " +
                token::token_type_string(tokens[index]->type));
}

astNs::blockStatement *parser::parse_block_statement() {
    token *lbraceToken = tokens[index];
    index++; // move past lbrace
    vector<astNs::statement *> stmts;
    while (index < inputLen && tokens[index]->type != tokenType::rbrace) {
        stmts.push_back(parse_statement());
    }
    expectToken(tokenType::rbrace);
    index++;
    astNs::blockStatement *blockStatement = new astNs::blockStatement(lbraceToken, stmts);
    return blockStatement;
}

astNs::expression *parser::parse_function_expression() {
    token *fnToken = tokens[index];
    index++;
    vector<astNs::expression *> vec = parse_function_arguments();
    astNs::functionLiteral *fnLiteral = new astNs::functionLiteral(fnToken, vec);
    expectToken(tokenType::lbrace);
    fnLiteral->body = parse_block_statement();
    return fnLiteral;
}

vector<astNs::expression *> parser::parse_function_arguments() {
    expectToken(tokenType::lparen);
    index++;
    vector<astNs::expression *> vec;
    while (tokens[index]->type != tokenType::rparen) {
        astNs::expression *expr = parse_expression(precedence::lowest);
        vec.push_back(expr);
        if (tokens[index]->type == tokenType::comma) index++;
    }
    expectToken(tokenType::rparen);
    index++;
    return vec;
}

astNs::expression *parser::parse_call_expression(astNs::expression *leftExpr) {
    expectToken(tokenType::lparen);
    token *curToken = tokens[index];
    vector<astNs::expression *> vec = parse_function_arguments();
    astNs::callExpression *callExpr = new astNs::callExpression(curToken, vec);
    callExpr->name = leftExpr;

    return callExpr;
}

astNs::expression *parser::parse_boolean_expression(){
    expectToken(tokenType::booleanToken);
    token *tok = tokens[index++];
    astNs::expression *expr = tok->value == "true" ?  new astNs::booleanLiteral(tok, true) : new astNs::booleanLiteral(tok, false);
    return expr;
}

astNs::expression *parser::parse_string_literal(){
    expectToken(tokenType::stringToken);
    token *tok = tokens[index++];
    astNs::expression *expr = new astNs::stringLiteral(tok, tok->value);
    return expr;
}

astNs::expression *parser::parse_array_expression(){
    expectToken(tokenType::lbracket);
    astNs::arrayAccessExpr *arrayAccessExpr;

    if(  validArrayAccessTok(tokens[index-1] ) ){
        arrayAccessExpr = new astNs::arrayAccessExpr(prevExpr);
        arrayAccessExpr->tok = tokens[index++]; //assign lbracket to tok and step over it

        astNs::expression *accessIdx = parse_expression(precedence::lowest);
        expectToken(tokenType::rbracket);
        index++; // step over rbracket
        arrayAccessExpr->itemIndex = accessIdx;
        return arrayAccessExpr;
    }

    astNs::arrayExpression *arrayExpr = new astNs::arrayExpression(tokens[index]);
    index++; // step over lbracket
    vector<astNs::expression *> items;
    while(index < tokens.size() ){
        token *tok = tokens[index];
        if(tok->type == tokenType::eof || tok->type == tokenType::rbracket){
            index++; // step over eof || rbracket
            break;
        } 
        if(tok->type == tokenType::comma){
            index++; // step over comma
            continue;
        }
        astNs::expression *expr = parse_expression(precedence::lowest);
        items.push_back(expr);
    }
    arrayExpr->items = items;
    return arrayExpr;
}

bool parser::validArrayAccessTok(token *tok){
    switch (tok->type)
    {
        case tokenType::identifier: // ident[expr]
            return true;
        case tokenType::stringToken: // string[expr]
            return true;
        case tokenType::rbracket: // [...][expr]
            return true;
        case tokenType::rbrace: // fn(...){[...]}[expr]
            return true;
    }
    return false;
}