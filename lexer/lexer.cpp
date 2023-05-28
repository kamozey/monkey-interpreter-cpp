//
// Created by rahultumpala on 6/4/23.
//

#include <iostream>
#include "lexer.h"

vector<token *> lexer::parse_input() {
    vector<token *> tokens;
    while (index < inputLen) {
        string cur = scanNext();
        token *tok = tokenize_string(cur);
        tokens.push_back(tok);
    }
    if(tokens.size() > 0 && tokens[tokens.size() -1]->type != tokenType::eof){
        tokens.push_back(newToken("", tokenType::eof));
    }
    return tokens;
}

token *lexer::tokenize_string(const std::string &cur, bool requesting_peek_token) {
    if(cur == ""){
        return newToken("", tokenType::eof);
    }
    if (cur == "+") {
        return newToken("+", tokenType::plus);
    } else if (cur == "-") {
        return newToken("-", tokenType::minus);
    } else if (cur == "*") {
        return newToken("*", tokenType::asterisk);
    } else if (cur == ";") {
        return newToken(";", tokenType::semicolon);
    } else if (cur == "(") {
        return newToken("(", tokenType::lparen);
    } else if (cur == ")") {
        return newToken(")", tokenType::rparen);
    } else if (cur == "{") {
        return newToken("{", tokenType::lbrace);
    } else if (cur == "}") {
        return newToken("}", tokenType::rbrace);
    } else if (cur == ",") {
        return newToken(",", tokenType::comma);
    } else if (cur == "/") {
        return newToken("/", tokenType::division);
    } else if (cur == ">") {
        // requesting_peek_token is used when there are 2 character tokens
        // such as <=, >=, ==, !=
        if (!requesting_peek_token) {
            string peek = scanNext(1);
            token peek_token = *tokenize_string(peek, 1);
            if (peek_token.type == tokenType::assign) {
                index += peek_token.value.length();
                return newToken(cur + peek, tokenType::gte);
            }
        }
        return newToken(cur, tokenType::gt);

    } else if (cur == "<") {
        if (!requesting_peek_token) {
            string peek = scanNext(1);
            token peek_token = *tokenize_string(peek, 1);
            if (peek_token.type == tokenType::assign) {
                index += peek_token.value.length();
                return newToken(cur + peek, tokenType::lte);
            }
        }
        return newToken(cur, tokenType::lt);

    } else if (cur == "=") {
        if (!requesting_peek_token) {
            string peek = scanNext(1);
            token peek_token = *tokenize_string(peek, 1);
            if (peek_token.type == tokenType::assign) {
                index += peek_token.value.length();
                return newToken(cur + peek, tokenType::eq);
            }
        }
        return newToken(cur, tokenType::assign);

    } else if (cur == "!") {
        if (!requesting_peek_token) {
            string peek = scanNext(1);
            token peek_token = *tokenize_string(peek, 1);
            if (peek_token.type == tokenType::assign) {
                index += peek_token.value.length();
                return newToken(cur + peek, tokenType::neq);
            }
        }
        return newToken(cur, tokenType::bang);

    } else if (is_number(cur)) {
        return newToken(cur, tokenType::integer);
    } else if (token::is_reserved_keyword(cur)) {
        return token::get_keyword_token(cur);
    } else if(cur == "\""){
        string value  = readString(requesting_peek_token);
        return newToken(value, tokenType::stringToken);
    } else if(cur == "["){
        return newToken(cur, tokenType::lbracket);
    } else if(cur == "]"){
        return newToken(cur, tokenType::rbracket);
    }

    // TODO: instead of returning :identifier as default, think how to handle :illegal token case
    return newToken(cur, tokenType::identifier);
}

bool lexer::is_number(string value) {
    char *p;
    std::strtol(value.c_str(), &p, 10);
    return *p == 0;
}

string lexer::scanNext(bool peek) {
    while (index < inputLen &&
           (code[index] == '\n' || code[index] == '\t' || code[index] == ' ' || code[index] == '\r'))
        index++;
    int curIndex = index;
    string value;
    if (curIndex == inputLen) {
        index = inputLen; // we reached the end
        return "";
    }
    char cur = code[curIndex];
    if (cur == '+') {
        value += "+";
        curIndex++;
    } else if (cur == '-') {
        value += "-";
        curIndex++;
    } else if (cur == ',') {
        value += ",";
        curIndex++;
    } else if (cur == '*') {
        value += "*";
        curIndex++;
    } else if (cur == '/') {
        value += "/";
        curIndex++;
    } else if (cur == '=') {
        value += "=";
        curIndex++;
    } else if (cur == '>') {
        value += ">";
        curIndex++;
    } else if (cur == '<') {
        value += "<";
        curIndex++;
    } else if (cur == '!') {
        value += "!";
        curIndex++;
    } else if (cur == '(') {
        value += "(";
        curIndex++;
    } else if (cur == ')') {
        value += ")";
        curIndex++;
    } else if (cur == '{') {
        value += "{";
        curIndex++;
    } else if (cur == '}') {
        value += "}";
        curIndex++;
    } else if (cur == ';') {
        value += ";";
        curIndex++;
    } else if (cur >= '0' && cur <= '9') {
        while (code[curIndex] >= '0' && code[curIndex] <= '9')
            value += code[curIndex++];
    } else if (cur >= 'a' && cur <= 'z') {
        while (code[curIndex] >= 'a' && code[curIndex] <= 'z')
            value += code[curIndex++];
    } else if(cur == '"'){
        value += cur;
        // index = curIndex; // not incrementing index here because readString() will handle it
    } else if(cur == '['){
        value += cur;
        curIndex++;
    } else if(cur == ']'){
        value += cur;
        curIndex++;
    } 
    if (!peek) index = curIndex;
    return value;
}


token *lexer::newToken(std::string value, tokenType type) {
    return new token(value, type);
}

string lexer::readString(bool peek){
    int curIndex = index;
    while (curIndex < inputLen &&
           (code[curIndex] == '\n' || code[curIndex] == '\t' || code[curIndex] == ' ' || code[curIndex] == '\r'))
        curIndex++;
    string value;
    curIndex++; // to step over opening double quotes
    while(code[curIndex] != '"')
        value += code[curIndex++];
    curIndex++; // to step over closing double quotes
    if(!peek) index = curIndex;
    return value;
}