#include <iostream>

#include "./lexer/lexer.h"


using namespace std;

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

    code = R"(let x = 5;
    let y = 10;
    let foobar = 838383;)";

    lexer lex = *new lexer(code);
    vector<token> tokens = lex.parse_input();
    for (token t: tokens) {
        cout << t.to_string();
    }
    return 0;
}

