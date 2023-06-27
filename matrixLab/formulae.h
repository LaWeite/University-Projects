#include <string>
#include <stack>
#include "matrix.h"
#include <map>

class FormulaNode {
public:
	virtual Matrix calc() const = 0;
	virtual bool bracket() const = 0;
	virtual std::string str() const = 0;
	virtual ~FormulaNode() {}
};
//-------------------------------------------
class NumNode : public FormulaNode {
	Matrix num;
public:
	NumNode(Matrix x) : num(x) {}
	Matrix calc() const { return num; }
	bool bracket() const { return false; }
	std::string str() const { return "0"; }
};
//-------------------------------------------
namespace global {
	extern std::map<std::string, Matrix> Workspace;
}
//-------------------------------------------
class ParamNode : public FormulaNode {
	const std::string param;
public:
	ParamNode(std::string name) : param(name) {}
	Matrix calc() const {
		return global::Workspace[param];
	}
	bool bracket() const { return false; }
	std::string str() const { return param; }
};

class BracketNode : public FormulaNode {
	const std::string bracket_;
public:
	BracketNode(std::string name) : bracket_(name) {};
	Matrix calc() const { return Matrix(0); }
	bool bracket() const { return true; }
	std::string str() const { return bracket_; }
};
