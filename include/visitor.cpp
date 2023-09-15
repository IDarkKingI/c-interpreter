#include "visitor.hpp"


const int Printer::tab_size = 4;
int Printer::level = 0;


void Printer::print_tabs(const int& N) const {
	cout << string(N * tab_size, ' ');
}


void Printer::visit(const AST& root) {
	for (auto& it : root.declarations)
		it->accept(*this);
}


void Printer::visit(const VarDeclaration& var) {
	if (var.value) {
		cout << var.datatype << " " << var.var_name << " = ";
		var.value->accept(*this);
	} else
		cout << var.datatype << " " << var.var_name;
	if (level == 0)
		cout << ";" << endl;
}


void Printer::visit(const FuncProt& prot) {
	cout << prot.return_type << " " << prot.func_name << "(";
	for (size_t i = 0, size = prot.args.size(); i < size; i++) {
		cout << prot.args[i]->datatype << " " << prot.args[i]->var_name;
		if (prot.args[i]->value) {
			cout << " = ";
			prot.args[i]->value->accept(*this);
		}
		if (i != size - 1)
			cout << ", ";
	}
	cout << ")";
}


void Printer::visit(const FuncDef& def) {
    def.prot->accept(*this);
	cout << " {" << endl;
	def.block->accept(*this);
	cout << "}" << endl;
	cout << endl;
}


void Printer::visit(const Block& block) {
	level = block.level;
	for (auto& it : block.statements) {
        print_tabs(block.level);
		it->accept(*this);
		if (it->type != Node::CONDITIONAL && it->type != Node::LOOP)
			cout << ";" << endl;
    }
}


void Printer::visit(const Expr& expr) {
    switch (expr.type) {
		case Expr::CONST:
		case Expr::VAR:
			cout << expr.value;
			break;
		case Expr::CHAR:
			cout << "\'" << expr.value << "\'";
			break;
		case Expr::STRING:
			cout << "\"" << expr.value << "\"";
			break;
		case Expr::PRE_UNARY_OP:
			cout << expr.value;
			expr.branches[0]->accept(*this);
			break;
		case Expr::POST_UNARY_OP:
			expr.branches[0]->accept(*this);
			cout << expr.value;
			break;
		case Expr::BINARY_OP:
			if (!expr.branches.empty()) {
				expr.branches[0]->accept(*this);
				cout << " " << expr.value << " ";
				expr.branches[1]->accept(*this);
			} else 
				cout << expr.value;
			break;
		case Expr::FUNC:
		case Expr::BUILTIN_FUNC:
		case Expr::MATH_FUNC:
			cout << expr.value << "(";
			for (size_t i = 0, size = expr.branches.size(); i < size; i++) {
				expr.branches[i]->accept(*this);
				if (i != size - 1)
					cout << ", ";
			}
			cout << ")";
			break;
	}
}


void Printer::visit(const Conditional& ptr) {
    if (ptr.type == Conditional::IF || ptr.type == Conditional::ELIF) {
		cout << (ptr.type == Conditional::IF ? "if (" : "elif (");
		ptr.condition->accept(*this);
		cout << ") {" << endl;
		ptr.block->accept(*this);
		print_tabs(ptr.block->level - 1);
		cout << "}" << endl;
	} else {
		cout << "else {" << endl;
		ptr.block->accept(*this);
		print_tabs(ptr.block->level - 1);
		cout << "}" << endl;
	}
}


void Printer::visit(const Loop& loop) {
    cout << "while (";
	loop.condition->accept(*this);
	cout << ") {" << endl;
	loop.block->accept(*this);
	print_tabs(loop.block->level - 1);
	cout << "}" << endl;
}


void Printer::visit(const Return& ret) {
    cout << "return ";
	ret.ret_expr->accept(*this);
}


void Printer::visit(const Jump& jump) {
	cout << (jump.type == Jump::BREAK ? "break" : "continue");
}
