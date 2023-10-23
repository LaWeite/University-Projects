#include <stack>
#include <vector>
#include "formulanodes.h"

class BooleanExpression {
    unsigned int size;
    unsigned int truth_table_size;
    bool* truth_table;
    FormulaNode* root;
    std::vector<int> zhegalkinCoefficients;

    static FormulaNode* Postfix2Tree(const char*);

    static void Infix2Postfix(const char*, char*);

    static void InfixFilter(const char*, char*);

    static int ActionsRowNumber(char ch);

    static int ActionsColNumber(char ch);

    static const unsigned char ActionsTable[][7];

    void calcTruthTable();
public:
    BooleanExpression(const char* str) : truth_table(nullptr), truth_table_size(0), size(0) {
        unsigned long length = std::max(20UL, static_cast<unsigned long>(2 * std::strlen(str)));
        char* str_infix = new char[length];
        char* str_postfix = new char[length];

        try {
            InfixFilter(str, str_infix);
            Infix2Postfix(str_infix, str_postfix);
            root = Postfix2Tree(str_postfix);
        }
        catch (...) {
            delete[] str_postfix;
            delete[] str_infix;
            root = nullptr;
            throw "error";
        }

        root->ParamToTree();
        size = global::Workspace.size();
        truth_table_size = 1 << size;
        global::Workspace.clear();
        delete[] str_infix;
        delete[] str_postfix;
    }

    explicit BooleanExpression(FormulaNode* root);

    std::string str() const;

    BooleanExpression(BooleanExpression&& other) noexcept;

    explicit operator std::string() const;

    BooleanExpression dnf();

    BooleanExpression cnf();

    BooleanExpression zhegalkin();

    void checkClass(int post[5]);

    ~BooleanExpression();
};

bool isFullSystem(std::vector<BooleanExpression>&);
