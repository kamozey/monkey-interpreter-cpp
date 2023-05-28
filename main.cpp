#include <iostream>

#include "./lexer/lexer.h"


using namespace std;

#include "./ast/ast.h"
#include "./parser/parser.h"
#include "./object/object.h"
#include "./evaluator/evaluator.cpp"

int main() {

    string code = R"(let counter = fn(x) {
            if (x > 100000) {
            return true;
            } else {
                let foobar = 9999;
                counter(x + 1);
                }
            };
            counter(0);)"; // this code eats up memory, an example to verify if GC works
    // TODO: write a GC
    code = R"(let x = 5 + 5 - 4;
    let y = 10 + x / 2;
    let foobar = 838383 + y * 5;
    return 4 + (y+1);
    return 9+x;
    let z = if(1+1==2){return 1;} else { return true;}
    let f = fn(x,y,z){return x+y+z;}
    let value = add(z,f);)";
    code = "let f = fn(x) { fn(y) { x+y }; }; let m = f(999); m(1)";
    code = R"(let x = "hello"; return x + " " + x; )";
    code = R"(let array = ["hello", 1, fn(x){ return x;}]; array[2](7);  )";

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
    Environment *env = new Environment();
    object *obj = eval(program, env);
    cout << obj->inspect() << endl;
    return 0;
}
