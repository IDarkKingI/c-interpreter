#include "include/interpreter.hpp"

int main() {
    Interpreter program("text.txt");
    program.parse_source();
    program.parse_syntax();
    program.print_ast();
    return 0;
}
