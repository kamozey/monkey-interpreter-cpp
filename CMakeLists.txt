cmake_minimum_required(VERSION 3.20..3.23)
project(MonkeyInterpreter)

set(CMAKE_CXX_STANDARD 17)

add_executable(MonkeyInterpreter main.cpp lexer/lexer.cpp lexer/lexer.h token/token.cpp token/token.h parser/parser.cpp parser/parser.h ast/ast.h)
