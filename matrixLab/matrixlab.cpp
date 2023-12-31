#include "formulae.h"
#include <fstream>
#include <iostream>
#include <stack>
#include <algorithm>

enum Instructions
{
	_UNKNOWN = 0,
	_DISP,
	_ZEROS,
	_ONES,
	_EYE,
	_LINSPACE,
	_VERTCAT,
	_HORZCAT,
	_TRANSPOSE,
	_DET,
	_INV,
	_SUM,
	_PROD,
	_MIN,
	_MAX,
};

bool IsOp(char c) {
	switch (c) {
	case '+':
	case '-':
	case '*':
	case '/':
	case '=':
		return true;
	default:
		return false;
	}
}

Instructions _getInstructionsCode(std::string const & func) {
	std::string const & temp = func;
	Instructions result = _UNKNOWN;
	
	if (temp == "disp") {
		result = _DISP;
	}
	else if (temp == "zeros") {
		result = _ZEROS;
	}
	else if (temp == "ones") {
		result = _ONES;
	}
	else if (temp == "linspace") {
		result = _LINSPACE;
	}
	else if (temp == "vertcat") {
		result = _VERTCAT;
	}
	else if (temp == "horzcat") {
		result = _HORZCAT;
	}
	else if (temp == "transpose") {
		result = _TRANSPOSE;
	}
	else if (temp == "det") {
		result = _DET;
	}
	else if (temp == "inv") {
		result = _INV;
	}
	else if (temp == "sum") {
		result = _SUM;
	}
	else if (temp == "prod") {
		result = _PROD;
	}
	else if (temp == "max") {
		result = _MAX;
	}
	else if (temp == "min") {
		result = _MIN;
	}
	else if (temp == "eye") {
		result = _EYE;
	}
	else {
		result = _UNKNOWN;
	}

	return result;
}

bool _isNumber(char c) {
	return static_cast<bool>(isdigit(c));
}

int main(int argc, char*argv[]) {
	std::ifstream input(argv[1], std::ios::binary);
	std::ofstream output(argv[2], std::ios::binary);

	std::stack<FormulaNode*> S;

	std::string str;
	std::string name;
	char operand = '\0';
	
	Matrix left;
	Matrix right;
	Matrix result;
	int n;
	int m;
	int x0;
	int x1;
	int count_string = 0;
	while (std::getline(input, str))
	{
		try {

			for (int i = static_cast<int>(str.length()) - 1; i >= 0; --i)
			{
				while (static_cast<bool>(isspace(str[i]))) {
					--i;
				}

				if (str[i] == '(')
				{
					--i;
					continue;
				}
				if (str[i] == ')')
				{
					S.push(new BracketNode(")"));
				}
				else if (static_cast<bool>(_isNumber(str[i]))) {
					bool check = false;
					while (static_cast<bool>(_isNumber(str[i])) || static_cast<bool>(isalpha(str[i]))) {
						if (static_cast<bool>(isalpha(str[i])))
						{
							check = true;
						}
						name += str[i];
						--i;
					}

					std::reverse(name.begin(), name.end());
					if (check)
					{
						S.push(new ParamNode(name));
					}
					else {
						S.push(new NumNode(Matrix(std::stoi(name))));
					}
					name.clear();
				}
				else if (static_cast<bool>(isalpha(str[i])))
				{
					while (static_cast<bool>(isalpha(str[i])) || static_cast<bool>(isdigit(str[i]))) {
						name += str[i];
						--i;
					}
					std::reverse(name.begin(), name.end());
					switch (_getInstructionsCode(name))
					{
					case _UNKNOWN:
						if (name.length() < 31)
						{
							S.push(new ParamNode(name));
						}
						else {
							throw "error in line: " + std::to_string(count_string);
						}

						name.clear();
						break;
					case _DISP:
					{
						left = S.top()->calc();
						std::string parametr = S.top()->str();
						delete S.top();
						S.pop();
						if (parametr == "0")
						{
							parametr = "ans";
						}

						bool check = false;
						if (left.n_ == 1 && left.m_ == 1)
						{
							output << parametr << " = ";
						}
						else {
							check = true;
							output << parametr << " = [" << '\n';
						}
						disp(output, left);
						if (check)
						{
							output << "\n]";
						}
						
						delete S.top();
						S.pop();

						output << '\n';

						break;
					}
					case _ZEROS:
						n = S.top()->calc(); 
						delete S.top();
						S.pop();
						m = S.top()->calc(); 
						delete S.top();
						S.pop();

						zeros(result, n, m);
						
						delete S.top();
						S.pop();
						S.push(new NumNode(result));
						break;
					case _ONES:
						n = static_cast<int>(S.top()->calc());
						delete S.top();
						S.pop();
						m = static_cast<int>(S.top()->calc());
						delete S.top();
						S.pop();

						ones(result, n, m);

						delete S.top();
						S.pop();
						S.push(new NumNode(result));
						break;
					case _EYE:
						n = static_cast<int>(S.top()->calc());
						delete S.top();
						S.pop();

						eye(result, n);

						delete S.top();
						S.pop();
						S.push(new NumNode(result));
						break;
					case _LINSPACE:
						x0 = static_cast<int>(S.top()->calc());
						delete S.top();
						S.pop();
						x1 = static_cast<int>(S.top()->calc());
						delete S.top();
						S.pop();
						n = static_cast<int>(S.top()->calc());
						delete S.top();
						S.pop();

						linspace(result, x0, x1, n);

						delete S.top();
						S.pop();
						S.push(new NumNode(result));
						break;
					case _VERTCAT:
						left = S.top()->calc();
						delete S.top();
						S.pop();
						right = S.top()->calc();
						delete S.top();
						S.pop();
						result = vertcat(left, right);
						while (true)
						{
							if (S.top()->bracket())
							{
								delete S.top();
								S.pop();
								break;
							}
							right = S.top()->calc();
							delete S.top();
							S.pop();
							result = vertcat(result, right);
						}

						S.push(new NumNode(result));
						break;
					case _HORZCAT:
						left = S.top()->calc();
						delete S.top();
						S.pop();
						right = S.top()->calc();
						delete S.top();
						S.pop();
						result = horzcat(left, right);
						while (true)
						{
							if (S.top()->bracket())
							{
								delete S.top();
								S.pop();
								break;
							}
							right = S.top()->calc();
							delete S.top();
							S.pop();
							result = horzcat(result, right);
						}

						S.push(new NumNode(result));
						break;
					case _TRANSPOSE:
						left = S.top()->calc();
						delete S.top();
						S.pop();
						delete S.top();
						S.pop();
						result = transpose(left);

						S.push(new NumNode(result));
						break;
					case _DET:
					{
						left = S.top()->calc();
						delete S.top();
						S.pop();
						Matrix det_result = det(left);

						delete S.top();
						S.pop();
						S.push(new NumNode(det_result));
						break;
					}
					case _INV:
					{
						left = S.top()->calc();
						delete S.top();
						S.pop();
						Matrix inv_result = inv(left);

						delete S.top();
						S.pop();
						S.push(new NumNode(inv_result));
						break;
					}
					case _SUM:
					{
						left = S.top()->calc();
						delete S.top();
						S.pop();
						Matrix sum_of_elements = sum(left);

						delete S.top();
						S.pop();
						S.push(new NumNode(sum_of_elements));
						break;
					}
					case _PROD:
					{
						left = S.top()->calc();
						delete S.top();
						S.pop();
						Matrix prod_of_elements = prod(left);

						delete S.top();
						S.pop();
						S.push(new NumNode(prod_of_elements));
						break;
					}
					case _MAX:
						left = S.top()->calc();
						delete S.top();
						S.pop();
						right = S.top()->calc();
						delete S.top();
						S.pop();

						result = max(left, right);

						delete S.top();
						S.pop();
						S.push(new NumNode(result));
						break;
					case _MIN:
						left = S.top()->calc();
						delete S.top();
						S.pop();
						right = S.top()->calc();
						delete S.top();
						S.pop();

						result = min(left, right);

						delete S.top();
						S.pop();
						S.push(new NumNode(result));
						break;
					default:
						throw std::runtime_error("Unknown Token Found.");
						break;
					}

					name.clear();
				}
				else if (IsOp(str[i])) {
					operand = str[i];
					switch (operand)
					{
					case '+':
						left = S.top()->calc();
						delete S.top();
						S.pop();
						if (!S.top()->bracket())
						{
							right = S.top()->calc();
							delete S.top();
							S.pop();
							result = left + right;
						}
						else {
							result = +left;
						}

						if (!S.empty())
						{
							delete S.top();
							S.pop();
						}
						S.push(new NumNode(result));
						break;
					case '-':
						left = S.top()->calc();
						delete S.top();
						S.pop();
						if (!S.top()->bracket())
						{
							right = S.top()->calc();
							delete S.top();
							S.pop();
							result = left - right;
						}
						else {
							result = -left;
						}

						if (!S.empty())
						{
							delete S.top();
							S.pop();
						}
						S.push(new NumNode(result));
						break;
					case '*':
						left = S.top()->calc();
						delete S.top();
						S.pop();
						right = S.top()->calc();
						delete S.top();
						S.pop();
						if (str[i - 1] == '.' || ((left.n_ == 1 && left.m_ == 1) || (right.n_ == 1 && right.m_ == 1)))
						{
							i -= 1;
							result = left * right;
						}
						else {
							result = matrix_prod(left, right);
						}

						if (!S.empty())
						{
							delete S.top();
							S.pop();
						}
						S.push(new NumNode(result));
						break;
					case '/':
						left = S.top()->calc();
						delete S.top();
						S.pop();
						right = S.top()->calc();
						delete S.top();
						S.pop();
						if (i != 0)
						{
							if (str[i - 1] == '.' || ((left.n_ == 1 && left.m_ == 1) || (right.n_ == 1 && right.m_ == 1)))
							{
								i -= 1;
								result = left / right;
							}
							else {
								result = matrix_del(left, right);
							}
						}
						else {
							result = matrix_del(left, right);
						}

						if (!S.empty())
						{
							delete S.top();
							S.pop();
						}
						S.push(new NumNode(result));
						break;
					case '=':
					{
						std::string parametr = S.top()->str();
						delete S.top();
						S.pop();
						left = S.top()->calc();
						delete S.top();
						S.pop();
						global::Workspace[parametr] = left;
						break;
					}
					default:
						break;
					}

					--i;
				}
				else {
					throw std::runtime_error("Invaild Expression.");
					break;
				}
			}

			++count_string;
		}
		catch (...) {
			while (!S.empty())
			{
				delete S.top();
				S.pop();
			}
			input.close();
			output.close();
			throw "error in line: " + std::to_string(count_string + 1);
		}
	}
	
	while (!S.empty())
	{
		delete S.top();
		S.pop();
	}
	input.close();
	output.close();
	return 0;
}
