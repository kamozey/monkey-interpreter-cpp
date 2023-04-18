//
// Created by rahultumpala on 7/4/23.
//

#include <stdexcept>
#include "parser.h"
#include<iostream>

astNs::ast *parser::parse_input() {
    vector<astNs::astNode *> program;
    while (index < inputLen) {
        token *curToken = tokens[index];
        cout << endl << curToken->to_string() << endl;
        switch (curToken->type) {
            case tokenType::let: {
                astNs::astNode *node = parse_let_statement();
                cout << node->String() << endl;
                program.push_back(node);
                break;
            }
            case tokenType::returnToken: {
                astNs::astNode *node = parse_return_statement();
//                cout << node->String() << endl;
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

    infixParseFns[tokenType::plus] = &parser::parse_infix_expression;
    infixParseFns[tokenType::minus] = &parser::parse_infix_expression;
    infixParseFns[tokenType::division] = &parser::parse_infix_expression;
    infixParseFns[tokenType::asterisk] = &parser::parse_infix_expression;
    infixParseFns[tokenType::eq] = &parser::parse_infix_expression;
    infixParseFns[tokenType::neq] = &parser::parse_infix_expression;
    infixParseFns[tokenType::lt] = &parser::parse_infix_expression;
    infixParseFns[tokenType::gt] = &parser::parse_infix_expression;
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
    precedences[tokenType::lt] = precedence::lessgreater;
    precedences[tokenType::gt] = precedence::lessgreater;
    precedences[tokenType::plus] = precedence::sum;
    precedences[tokenType::minus] = precedence::sum;
    precedences[tokenType::division] = precedence::product;
    precedences[tokenType::asterisk] = precedence::product;
}

precedence parser::get_precedence(tokenType t) {
    if (precedences.find(t) != precedences.end()) return precedences[t];
    return precedence::lowest;
}

astNs::expression *parser::parse_grouped_expression(){
    // tokens[index] now points to tokenType::lparen
    index++;
    astNs::expression *expr = parse_expression(get_precedence(tokens[index]->type));
    if(tokens[index]->type != tokenType::rparen) return nullptr;
    index++;
    return expr;
}
