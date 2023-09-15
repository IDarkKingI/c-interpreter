cmake_minimum_required(VERSION 3.0.0)
project(proglang VERSION 0.1.0)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}")

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
set(CMAKE_CXX_STANDARD 20)

add_executable(main main.cpp
                        include/lexer/lexer.hpp
                        include/lexer/lexer.cpp
                        include/lexer/alphabet.hpp
                        include/lexer/alphabet.cpp
                        include/lexer/dictionary.hpp
                        include/lexer/dictionary.cpp

                        include/syntaxer/tree.hpp
                        include/syntaxer/tree.cpp
                        include/syntaxer/syntaxer.hpp
                        include/syntaxer/syntaxer.cpp

                        include/visitor.hpp
                        include/visitor.cpp
                        include/interpreter.hpp
                        include/interpreter.cpp
                        )
