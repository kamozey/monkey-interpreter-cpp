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

    class ifExpression : public expression {
    public:
        token *tok;
        expression *condition;
        statement *evalTrue;
        statement *evalFalse;

        ifExpression(token *tok) {
            this->tok = tok;
        }

        string tokenLiteral() override {
            return tok->token_literal();
        }

        string String() override {
            return "if (" + condition->String() + ") {\n" + evalTrue->String() + "\n}" +
                   (evalFalse != nullptr ? "else {\n" + evalFalse->String() + "\n}" : "");
        }

        void expressionNode() override {
        }
    };

    class blockStatement : public statement {
    public:
        token *tok;
        vector<statement *> stmts;
        bool containsReturnStatement;

        blockStatement(token *tok) {
            this->tok = tok;
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

    class ast {
    public:
        vector<astNode *> program;

        ast(vector<astNode *> pr) {
            this->program = std::move(pr);
        }
    };
}
#endif //MONKEYINTERPRETER_AST_H
