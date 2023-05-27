//
// Created by rahultumpala on 6/4/23.
//

#include <map>
#include <utility>
#include "string"

#ifndef MONKEYINTERPRETER_TOKEN_H
#define MONKEYINTERPRETER_TOKEN_H

using namespace std;

enum tokenType {
    identifier,
    let,
    integer,
    fn,
    semicolon,
    lparen,
    rparen,
    lbrace,
    rbrace,
    comma,
    plus,
    minus,
    asterisk,
    returnToken,
    division,
    eof,
    neq,
    eq,
    gte,
    lte,
    illegal,
    assign,
    lt,
    gt,
    bang,
    ifToken,
    elseToken,
    booleanToken,
    stringToken
};

class token {
public:
    string value;
    tokenType type;

    static map<string, tokenType> reserved_keywords;

    token(string value, tokenType type) {
        this->value = std::move(value);
        this->type = type;
    }

    static bool is_reserved_keyword(string value);

    static token *get_keyword_token(string value);

    string to_string();

    string token_literal();

    static string token_type_string(tokenType t);
};


#endif //MONKEYINTERPRETER_TOKEN_H
