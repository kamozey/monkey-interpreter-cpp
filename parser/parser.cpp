//
// Created by rahultumpala on 7/4/23.
//

#include "parser.h"

ast parser::parse_input() {
    while (index < inputLen) {
        token curToken = tokens[index];
        switch (curToken.type) {
            case tokenType::let:
                parse_let_statement();
        }
    }
}

ast parser::parse_let_statement() {
    token curToken = tokens[++index];
    if (index == inputLen || tokens[index].type != tokenType::identifier)
        throw std::runtime_error("invalid let statement");
    ast::astNode::expression::identifier id = *new ast::astNode::expression::identifier();

}
