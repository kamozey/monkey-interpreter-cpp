#include <iostream>

#include "./lexer/lexer.h"


using namespace std;

#include "./ast/ast.h"
#include "./parser/parser.h"
#include "./object/object.h"
#include "./evaluator/evaluator.cpp"

int main() {

    string code = R"(let x = 5 + 5 - 4;
    let y = 10 + x / 2;
    let foobar = 838383 + y * 5;
    return 4 + (y+1);
    return 9+x;
    let z = if(1+1==2){return 1;} else { return true;}
    let f = fn(x,y,z){return x+y+z;}
    let value = add(z,f);)";
    code = "!true;";

    printf("\n --- start tokenizing --- ");
    lexer lex = *new lexer(code);
    vector<token *> tokens = lex.parse_input();
    printf("\n --- completed tokenizing...printing tokens --- \n\n");
    for (token *t: tokens) {
        cout << t->to_string();
    }

    printf("\n --- completed tokenizing...start parsing --- ");
    parser *prser = new parser(tokens);
    astNs::program *program = prser->parse_input();
    vector<astNs::statement *> statements = program->statements;
    printf("\n --- completed parsing...printing program --- ");
    printf("\n --- num nodes : %zu \n", statements.size());
    printf("\n --- start program print: \n\n");
    for (astNs::statement *stmt: statements) {
        cout << stmt->String() << endl;
    }
    printf("\n --- end program print: \n");
    printf("\n --- completed printing...start evaluating --- \n");
    object *obj = eval(program);
    cout << obj->inspect() << endl;
    return 0;
}
