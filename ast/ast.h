//
// Created by rahultumpala on 7/4/23.
//

#ifndef MONKEYINTERPRETER_AST_H
#define MONKEYINTERPRETER_AST_H

#include <utility>
#include <vector>
#include <string>
#include "../token/token.h"

namespace astNs {

    class astNode {
    public:
        virtual string tokenLiteral() = 0;

        virtual string String() = 0;
    };

    class expression : public astNode {
    public:
        virtual void expressionNode() = 0;
    };

    class statement : public astNode {
    public:
        virtual void statmentNode() = 0;
    };

    class identifier : public expression {
    public:
        std::string value;
        token *tok;


        identifier(class token *t, string v) {
            this->tok = t;
            this->value = std::move(v);
        }

        string tokenLiteral() override {
            return tok->token_literal();
        }

        string String() override {
            return value;
        }

        void expressionNode() override {

        }
    };

    class integerLiteral : public expression {
    public:
        token *tok;
        int value;

        integerLiteral(class token *t, int v) {
            tok = t;
            value = v;
        }

        string tokenLiteral() override {
            return tok->token_literal();
        }

        string String() override {
            return to_string(value);
        }

        void expressionNode() override {

        }

    };

    class stringLiteral : public expression
    {
    public:
        token *tok;
        string value;

        stringLiteral(class token *t, string v)
        {
            tok = t;
            value = v;
        }

        string tokenLiteral() override
        {
            return tok->token_literal();
        }

        string String() override
        {
            return  "\"" + value + "\"";
        }

        void expressionNode() override
        {
        }
    };

    class booleanLiteral : public expression
    {
    public:
        token *tok;
        bool value;

        booleanLiteral(token *tok, bool val) {
            this->tok = tok;
            this->value = val;
        }

        string tokenLiteral() override {
            return tok->token_literal();
        }

        string String() override {
            return value ? "true" : "false";
        }

        void expressionNode() override {
        }
    };

    class letStatement : public statement {
    public:
        token *tok;
        identifier *name;
        expression *value;

        letStatement(class token *t, identifier *name, expression *value) {
            this->tok = t;
            this->name = name;
            this->value = value;
        }

        string tokenLiteral() override {
            return tok->token_literal();
        }

        string String() override {
            return tokenLiteral() + " " + name->String() + " = " + value->String() + " ;";
        }

        void statmentNode() override {
        }
    };

    class returnStatement : public statement {
    public:
        token *tok;
        expression *returnValue;

        returnStatement(class token *tok, expression *returnValue) {
            this->tok = tok;
            this->returnValue = returnValue;
        }

        string tokenLiteral() override {
            return tok->token_literal();
        }

        string String() override {
            return tokenLiteral() + " " + returnValue->String() + " ;";
        }

        void statmentNode() override {

        }
    };

    class expressionStatement : public statement {
    public:
        token *tok;
        expression *expr;

        expressionStatement(class token *tok, expression *expr) {
            this->tok = tok;
            this->expr = expr;
        }

        string tokenLiteral() override {
            return tok->token_literal();
        }

        string String() override {
            return expr->String();
        }

        void statmentNode() override {
        }
    };

    class prefixExpression : public expression {
    public:
        token *tok;
        string prefixOperator;
        expression *right;

        prefixExpression(token *tok, const string &prefixOperator) {
            this->tok = tok;
            this->prefixOperator = prefixOperator;
        }

        string tokenLiteral() override {
            return tok->token_literal();
        }

        string String() override {
            return "(" + prefixOperator + right->String() + ")";
        }

        void expressionNode() override {

        }
    };

    class infixExpression : public expression {
    public:
        token *tok;
        expression *left;
        expression *right;
        string infixOperator;

        infixExpression(token *tok, string op, expression *leftExpr) {
            this->tok = tok;
            this->infixOperator = op;
            this->left = leftExpr;
        }

        string tokenLiteral() override {
            return tok->token_literal();
        }

        string String() override {
            return "(" + left->String() + " " + infixOperator + " " + right->String() + ")";
        }

        void expressionNode() override {

        }

    };

    class blockStatement : public statement {
    public:
        token *tok;
        vector<statement *> stmts;

        blockStatement(token *tok, vector<statement *> &stmts) {
            this->tok = tok;
            this->stmts = stmts;
        }

        string tokenLiteral() override {
            return tok->token_literal();
        }

        string String() override {
            string s;
            for (statement *stmt: stmts) {
                s += stmt->String();
                s += "\n";
            }
            return s;
        }

        void statmentNode() override {
        }
    };

    class ifExpression : public expression {
    public:
        token *tok;
        expression *condition;
        blockStatement *evalTrue;
        blockStatement *evalFalse;

        ifExpression(token *tok) {
            this->tok = tok;
        }

        string tokenLiteral() override {
            return tok->token_literal();
        }

        string String() override {
            return "if (" + condition->String() + ") {\n\t" + evalTrue->String() + "}" +
                   (evalFalse != nullptr ? "else {\n\t" + evalFalse->String() + "}" : "");
        }

        void expressionNode() override {
        }
    };

    class arrayExpression : public expression {
    public:
        token *tok;
        vector<expression *> items;

        arrayExpression(token *tok) {
            this->tok = tok;
            this->items = items;
        }

        string tokenLiteral() override {
            return tok->token_literal();
        }

        string String() override {
            string s;
            s += "[";
            for (int i = 0; i < items.size(); i++) {
                s += items[i]->String();
                s += i != items.size() - 1 ? "," : "";
            }
            s += "]";
            return s;
        }

        void expressionNode() override {

        }
    };

    class arrayAccessExpr: public expression {
    public:
        token *tok;
        expression *itemIndex; // any expression that evaluates to Integer
        expression *arrayExpr; // any expression that evaluates to Array

        arrayAccessExpr(expression *arrayExpr) {
            this->arrayExpr = arrayExpr;
        }

        string tokenLiteral() override {
            return tok->token_literal();
        }

        string String() override {
            return arrayExpr->String() + "[" + itemIndex->String() + "]";
        }

        void expressionNode() override {
        }
    };

    class functionLiteral : public expression {
    public:
        token *tok;
        vector<expression *> args;
        blockStatement *body;

        functionLiteral(token *tok, vector<expression *> &args) {
            this->tok = tok;
            this->args = args;
        }

        string tokenLiteral() override {
            return tok->token_literal();
        }

        string String() override {
            string s;
            s += tok->token_literal() + "(";
            for (int i = 0; i < args.size(); i++) {
                s += args[i]->String();
                s += i != args.size() - 1 ? "," : "";
            }
            s += ") {\n\t";
            s += body->String();
            s += "}";
            return s;
        }

        void expressionNode() override {

        }
    };

    class callExpression : public expression {
    public:
        token *tok;
        expression *name;
        vector<expression *> args;

        callExpression(token *tok, vector<expression *> args) {
            this->tok = tok;
            this->args = args;
        }

        string tokenLiteral() override {
            return tok->token_literal();
        }

        string String() override {
            string s = name->String() + "(";
            for (int i = 0; i < args.size(); i++) {
                s += args[i]->String();
                s += i != args.size() - 1 ? "," : "";
            }
            s += ")";
            return s;
        }

        void expressionNode() override {

        }
    };

    class program : public astNode {
    public:
        vector<statement *> statements;

        program(vector<statement *> &statements) {
            this->statements = statements;
        }

        string tokenLiteral() override {
            return std::string();
        }

        string String() override {
            return std::string();
        }
    };


    class ast {
    public:
        program *program_ptr;

        ast(program *program_ptr) {
            this->program_ptr = program_ptr;
        }
    };
}
#endif //MONKEYINTERPRETER_AST_H
