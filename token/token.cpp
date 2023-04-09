//
// Created by rahultumpala on 6/4/23.
//

#include "token.h"


map<string, tokenType> token::reserved_keywords = {{"let",    tokenType::let},
                                                   {"fn",     tokenType::fn},
                                                   {"return", tokenType::returnToken}};

bool token::is_reserved_keyword(std::string value) {
    return reserved_keywords.find(value) != reserved_keywords.end();
}

token *token::get_keyword_token(std::string value) {
    tokenType type = reserved_keywords.at(value);
    return new token(value, type);
}

string token::to_string() {
    return "Token( " + value + ", " + token::token_type_string(type) + " )\n";
}

string token::token_literal() {
    return token_type_string(type);
}

string token::token_type_string(tokenType t) {
    switch (t) {
        case tokenType::identifier:
            return "identifier";
        case tokenType::let:
            return "let";
        case tokenType::integer:
            return "integer";
        case tokenType::fn:
            return "fn";
        case tokenType::semicolon:
            return "semicolon";
        case tokenType::lparen:
            return "lparen";
        case tokenType::rparen:
            return "rparen";
        case tokenType::lbrace:
            return "lbrace";
        case tokenType::rbrace:
            return "rbrace";
        case tokenType::comma:
            return "comma";
        case tokenType::plus:
            return "plus";
        case tokenType::minus:
            return "minus";
        case tokenType::asterisk:
            return "asterisk";
        case tokenType::returnToken:
            return "return";
        case tokenType::division:
            return "division";
        case tokenType::eof:
            return "eof";
        case tokenType::neq:
            return "neq";
        case tokenType::eq:
            return "eq";
        case tokenType::gte:
            return "gte";
        case tokenType::lte:
            return "lte";
        case tokenType::assign:
            return "assign";
        case tokenType::lt:
            return "lt";
        case tokenType::gt:
            return "gt";
        case tokenType::bang:
            return "bang";
    }
}