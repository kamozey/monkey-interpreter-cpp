//
// Created by rahultumpala on 7/4/23.
//

#ifndef MONKEYINTERPRETER_AST_H
#define MONKEYINTERPRETER_AST_H

#include <vector>
#include <string>
#include "../token/token.h"


class ast {
public:
    ast();

    class astNode {
    public:
        astNode();

        class expression {
        public:
            expression();

            class identifier {
            public:
                identifier();

                std::string value;

                identifier(string v) {
                    this->value = v;
                }
            };

            class integerLiteral {
            public:
                token token;
                int value;

                integerLiteral(int v, class token t) : token(token) {
                    token = t;
                    value = v;
                }
            };
        };

        class letStatement {
        public:
            expression::identifier identifier;
            expression value;
        };

    };

    class program {
    public:
        std::vector<astNode> body;
    };
};

#endif //MONKEYINTERPRETER_AST_H
