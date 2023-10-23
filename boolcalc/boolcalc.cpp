#include <fstream>
#include <iostream>
#include <vector>
#include "boolexpr.h"

int main(int argc, char* argv[]) {
    std::ifstream in(argv[2]);
    if (std::strcmp(argv[1], "-h") == 0 || std::strcmp(argv[1], "?") == 0) {
        std::cout << "-h, ? - help message\n"
            "-cnf - build cnf\n"
            "-dnf - build dnf\n"
            "zh - build Zhegalkin polynomial\n"
            "-isfull - checking the completeness of the system\n";

        return 0;
    }

    std::ofstream out(argv[3]);
    try
    {
        char str[256];
        if (std::strcmp(argv[1], "-cnf") == 0) {
            while (in.peek() != EOF) {
                in.getline(str, 256);
                if (str[0] == '0' || str[0] == '1') {
                    out << str << std::endl;
                    continue;
                }
                BooleanExpression expr(str);
                out << expr.cnf().str();
                if (in.peek() != EOF)
                {
                    out << std::endl;
                }
            }
        }
        else if (std::strcmp(argv[1], "-dnf") == 0) {
            while (in.peek() != EOF) {
                in.getline(str, 256);
                if (str[0] == '0' || str[0] == '1') {
                    out << str << std::endl;
                    continue;
                }
                BooleanExpression expr(str);
                out << expr.dnf().str() << std::endl;
            }
        }
        else if (std::strcmp(argv[1], "-zh") == 0) {
            while (in.peek() != EOF) {
                in.getline(str, 256);
                if (str[0] == '0' || str[0] == '1') {
                    out << str << std::endl;
                    continue;
                }
                BooleanExpression expr(str);
                out << expr.zhegalkin().str() << std::endl;
            }
        }
        else if (std::strcmp(argv[1], "-isfull") == 0) {
            std::vector<BooleanExpression> expressions;
            char str[256];
            while (in.peek() != EOF) {
                in.getline(str, 256);
                expressions.emplace_back(str);
            }

            if (isFullSystem(expressions))
            {
                out << "yes";
            }
            else {
                out << "no";
            }
        }
        else {
            out << "error" << std::endl;
            return 0;
        }
    }
    catch (...)
    {
        out << "error";
        out.close();
        in.close();
        return 0;
    }

    in.close();
    out.close();
    return 0;
}
