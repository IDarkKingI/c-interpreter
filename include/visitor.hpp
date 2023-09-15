#pragma once


#include "syntaxer/tree.hpp"


class ConstVisitor {
public:
    virtual void visit(const AST&) = 0;
    virtual void visit(const VarDeclaration&) = 0;
    virtual void visit(const FuncProt&) = 0;
    virtual void visit(const FuncDef&) = 0;
    virtual void visit(const Block&) = 0;
    virtual void visit(const Expr&) = 0;
    virtual void visit(const Conditional&) = 0;
    virtual void visit(const Loop&) = 0;
    virtual void visit(const Return&) = 0;
    virtual void visit(const Jump&) = 0;
};

class Printer : public ConstVisitor {
private:
    static const int tab_size;
    static int level;

public:
    void print_tabs(const int&) const;
    virtual void visit(const AST&) override;
    virtual void visit(const VarDeclaration&) override;
    virtual void visit(const FuncProt&) override;
    virtual void visit(const FuncDef&) override;
    virtual void visit(const Block&) override;
    virtual void visit(const Expr&) override;
    virtual void visit(const Conditional&) override;
    virtual void visit(const Loop&) override;
    virtual void visit(const Return&) override;
    virtual void visit(const Jump&) override;
};


