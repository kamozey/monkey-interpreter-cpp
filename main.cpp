#include <iostream>

#include "./lexer/lexer.h"


using namespace std;

#include "./ast/ast.h"
#include "./parser/parser.h"

int main() {
    string code = R"(let five = 5;
      let ten = 1012389;

      let add = fn(x,y) {
        x+y;
      };

      let result = add(five, ten);

      !-/*5;
      5 < 10 > 5;
      if (5 < 10) {
      return true;
      } else {
      return false;
      }
      10 == 10;
      10 != 9;
      102 >= 1;
      1 <= 21;)";

    code = R"(let x = 5 + 5 - 4;
    let y = 10 + x / 2;
    let foobar = 838383 + y * 5;
    return 4 + (y+1);
    return 9+x;
    if(1+1==2){return 1;} else { return 2;})";

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
    printf("\n --- completed parsing...printing program --- ");
    printf("\n --- num nodes : %d \n", program->statements->size());
    printf("\n --- start program print: \n\n");
    for (astNs::statement *stmt: *program->statements) {
        cout << stmt->String() << endl;
    }
    printf("\n --- end program print: \n");

    return 0;
}

