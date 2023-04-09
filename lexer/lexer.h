//
// Created by rahultumpala on 6/4/23.
//

#ifndef MONKEYINTERPRETER_LEXER_H
#define MONKEYINTERPRETER_LEXER_H


#include "../token/token.h"
#include <vector>

class lexer {
private:
    string code;
    int index;
    int inputLen;
public:
    lexer(string code) {
        this->code = code;
        this->index = 0;
        this->inputLen = code.length();
    }

    vector<token *> parse_input();

    token *tokenize_string(const string &value, bool requesting_peek_token = false);

    token *newToken(string value, tokenType type);

    string scanNext(bool peek = false);

    bool is_number(string value);
};


#endif //MONKEYINTERPRETER_LEXER_H
