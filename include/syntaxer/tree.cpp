#include "tree.hpp"
#include "../visitor.hpp"

Node::Node(const NodeType& type) : type(type) {}

Node::~Node() {}

Declaration::Declaration(const NodeType& type) : Node(type) {}

Declaration::~Declaration() {}


FuncDeclaration::FuncDeclaration(const FuncType& type)
	: Declaration(Declaration::FUNCDECL),
	  type(type) {}



FuncProt::FuncProt(const string& return_type, const string& func_name, vector<unique_ptr<VarDeclaration>>&& args)
	: FuncDeclaration(FuncDeclaration::PROT),
	  return_type(return_type),
	  func_name(func_name),
	  args(move(args)) {}


void FuncProt::accept(ConstVisitor& v) {
	v.visit(*this);
}


FuncDef::FuncDef(unique_ptr<FuncProt>&& prot, unique_ptr<Block>&& block)
	: FuncDeclaration(FuncDeclaration::DEF),
	  prot(move(prot)),
	  block(move(block)) {}

void FuncDef::accept(ConstVisitor& v) {
	v.visit(*this);
}



Statement::Statement(const NodeType& type) : Node(type) {}

Statement::~Statement() {}



Block::Block(vector<unique_ptr<Statement>>&& statements, const int& level)
	: Statement(Statement::BLOCK),
	  statements(move(statements)),
	  level(level) {}


void Block::accept(ConstVisitor& v) {
	v.visit(*this);
}



VarDeclaration::VarDeclaration(const string& datatype, const string& name, unique_ptr<Expr>&& value)
    : Declaration(Declaration::VARDECL),
	  Statement(Statement::VARDECL),
	  datatype(datatype), 
      var_name(name),
	  value(move(value)) {}


VarDeclaration::VarDeclaration(const string& datatype, const string& name)
    : Declaration(Declaration::VARDECL),
	  Statement(Statement::VARDECL),
	  datatype(datatype), 
      var_name(name) { value = nullptr; }


void VarDeclaration::accept(ConstVisitor& v) {
	v.visit(*this);
}


const unordered_map<string, function<double(const double&)>> Expr::unary_op_dict = {
	{"+", [](const double& x) { return x; }},
	{"-", [](const double& x) { return -x; }}
};

const unordered_map<string, function<double(const double&, const double&)>> Expr::binary_op_dict = {
	{"+", [](const double& x, const double& y) { return x + y; }},
	{"-", [](const double& x, const double& y) { return x - y; }},
	{"*", [](const double& x, const double& y) { return x * y; }},
	{"/", [](const double& x, const double& y) { return x / y; }},
	{"**", [](const double& x, const double& y) { return pow(x, y); }}
};

const unordered_map<string, function<double(const vector<double>&)>> Expr::math_function_dict = {
	{"pow", [](const vector<double>& args) { return pow(args[0], args[1]); }},
	{"exp", [](const vector<double>& args) { return exp(args[0]); }},
	{"log", [](const vector<double>& args) { return log(args[0]); }},
	{"sin", [](const vector<double>& args) { return sin(args[0]); }},
	{"cos", [](const vector<double>& args) { return cos(args[0]); }}
};


Expr::Expr(const ExprType& type, const string& value, vector<unique_ptr<Expr>>&& branches)
	: Statement(Statement::EXPRESSION),
	  type(type),
	  value(value),
	  branches(move(branches)) {}


Expr::Expr(const ExprType& type, const string& value)
	: Statement(Statement::EXPRESSION),
	  type(type),
	  value(value) {}


double Expr::eval(const double& x) {
	switch (type) {
		case CONST:
			return stod(value);
		case VAR:
			return x;
		case PRE_UNARY_OP:
		case POST_UNARY_OP:
			return unary_op_dict.at(value)(branches[0]->eval(x));
		case BINARY_OP:
			return binary_op_dict.at(value)(branches[0]->eval(x), branches[1]->eval(x));
		case BUILTIN_FUNC: // встроенные функции print, scan, abs, sgn
			return stod(value);
		case FUNC: // пользовательские функции
			return stod(value);
		case MATH_FUNC: // мат функции sin, cos, log, exp, pow
			vector<double> args;
			for (const auto& it : branches)
				args.push_back(it->eval(x));
			return math_function_dict.at(value)(args);
	}
	return 0.0;
}


void Expr::accept(ConstVisitor& v) {
	v.visit(*this);
}


Conditional::Conditional(const ConditionType& type, unique_ptr<Expr>&& condition, unique_ptr<Block>&& block)
	: Statement(Statement::CONDITIONAL),
	  type(type),
	  condition(move(condition)),
	  block(move(block)) {}


Conditional::Conditional(const ConditionType& type, unique_ptr<Block>&& block)
	: Statement(Statement::CONDITIONAL),
	  type(type),
	  block(move(block)) {}


void Conditional::accept(ConstVisitor& v) {
	v.visit(*this);
}


Loop::Loop(unique_ptr<Expr>&& condition, unique_ptr<Block>&& block)
	: Statement(Statement::LOOP),
	  condition(move(condition)),
	  block(move(block)) {}


void Loop::accept(ConstVisitor& v) {
	v.visit(*this);
}



Return::Return(unique_ptr<Expr>&& ret) : Statement(Statement::RETURN), ret_expr(move(ret)) {}


void Return::accept(ConstVisitor& v) {
	v.visit(*this);
}



Jump::Jump(const JumpType& type) : Statement(Statement::JUMP), type(type) {}


void Jump::accept(ConstVisitor& v) {
	v.visit(*this);
}

void AST::accept(ConstVisitor& v) {
	v.visit(*this);
}

