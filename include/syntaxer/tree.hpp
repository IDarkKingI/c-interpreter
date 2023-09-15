#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <functional>
#include <unordered_map>
#include <cmath>
#include <algorithm>
#include <memory>


using namespace std;


class ConstVisitor;
class VarDeclaration;
class Block;
class Expr;


class Node {
public:
	enum NodeType {
		STRING,
		VARDECL,
		FUNCDECL,
		BLOCK,
		EXPRESSION,
		CONDITIONAL,
		LOOP,
		RETURN,
		JUMP
	};

	NodeType type;
	Node(const NodeType&);
	virtual ~Node();

	virtual void accept(ConstVisitor&) = 0;
};



class Declaration : public Node {
public:
	Declaration(const NodeType&);
	virtual ~Declaration();
};



class FuncDeclaration : public Declaration {
public:
	enum FuncType { PROT, DEF };
	FuncType type;

	FuncDeclaration(const FuncType&);
};



class FuncProt : public FuncDeclaration {
public:
	string return_type;
	string func_name;
	vector<unique_ptr<VarDeclaration>> args;

	FuncProt(const string&, const string&, vector<unique_ptr<VarDeclaration>>&&);
	virtual void accept(ConstVisitor&) override;
};


class FuncDef : public FuncDeclaration {
public:
	unique_ptr<FuncProt> prot;
	unique_ptr<Block> block;

	FuncDef(unique_ptr<FuncProt>&&, unique_ptr<Block>&&);
	virtual void accept(ConstVisitor&) override;
};



class Statement : public Node {
public:
	Statement(const NodeType&);
	virtual ~Statement();
};



class Block : public Statement {
public:
	int level;

	vector<unique_ptr<Statement>> statements;
	Block(vector<unique_ptr<Statement>>&&, const int&);
	
	virtual void accept(ConstVisitor&) override;
};



class VarDeclaration : public Declaration, public Statement {
public:
	string datatype;
	string var_name;
	unique_ptr<Expr> value;

	VarDeclaration(const string&, const string&, unique_ptr<Expr>&&);
	VarDeclaration(const string&, const string&);
	
	virtual void accept(ConstVisitor&) override;
};



class Expr : public Statement {
public:
	enum ExprType {
		CONST,
		STRING,
		CHAR,
		VAR,
		FUNC,
		BUILTIN_FUNC,
		MATH_FUNC,
		BINARY_OP,
		PRE_UNARY_OP,
		POST_UNARY_OP
	};

	ExprType type;
	string value;
	vector<unique_ptr<Expr>> branches;

	Expr(const ExprType&, const string&, vector<unique_ptr<Expr>>&&);
	Expr(const ExprType&, const string&);

	double eval(const double& x = 0);
	
	virtual void accept(ConstVisitor&) override;

private:
	static const unordered_map<string, function<double(const double&)>> unary_op_dict;
	static const unordered_map<string, function<double(const double&, const double&)>> binary_op_dict;
	static const unordered_map<string, function<double(const vector<double>&)>> math_function_dict;
};



class Conditional : public Statement {
public:
	enum ConditionType { IF, ELIF, ELSE };

	ConditionType type;
	unique_ptr<Expr> condition;
	unique_ptr<Block> block;

	Conditional(const ConditionType&, unique_ptr<Expr>&&, unique_ptr<Block>&&);
	Conditional(const ConditionType&, unique_ptr<Block>&&);

	virtual void accept(ConstVisitor&) override;
};



class Loop : public Statement {
public:
	unique_ptr<Expr> condition;
	unique_ptr<Block> block;

	Loop(unique_ptr<Expr>&&, unique_ptr<Block>&&);

	virtual void accept(ConstVisitor&) override;
};



class Return : public Statement {
public:
	unique_ptr<Expr> ret_expr;

	Return(unique_ptr<Expr>&&);

	virtual void accept(ConstVisitor&) override;
};



class Jump : public Statement {
public:
	enum JumpType { CONTINUE, BREAK };
	JumpType type;

	Jump(const JumpType&);

	virtual void accept(ConstVisitor&) override;
};

class AST {
public:
	vector<unique_ptr<Declaration>> declarations;

	void accept(ConstVisitor&);
};

