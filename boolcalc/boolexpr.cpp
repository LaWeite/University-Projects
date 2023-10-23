#include <iomanip>
#include "boolexpr.h"

FormulaNode* BooleanExpression::Postfix2Tree(const char* str) {
    char ch;
    unsigned variableIndex;
    int index = 0;
    std::stack<FormulaNode*> S;
    FormulaNode* result;
    FormulaNode* left;
    FormulaNode* right;

    while ((ch = str[index]) != '\0') {
        left = right = nullptr;
        switch (ch) {
        case '~':
            if (S.empty()) { throw 1; } left = S.top(); S.pop();
            result = new DisNode(left);
            break;
        case '&':
            if (S.empty()) { throw 1; } right = S.top(); S.pop();
            if (S.empty()) { throw 1; } left = S.top(); S.pop();
            result = new ConjunctionNode(left, right);
            break;
        case 'v':
            if (S.empty()) { throw 1; } right = S.top(); S.pop();
            if (S.empty()) { throw 1; } left = S.top(); S.pop();
            result = new DisjunctionNode(left, right);
            break;
        case '>':
            if (S.empty()) { throw 1; } right = S.top(); S.pop();
            if (S.empty()) { throw 1; }left = S.top(); S.pop();
            result = new ImplicationNode(left, right);
            break;
        case '<':
            if (S.empty()) { throw 1; } right = S.top(); S.pop();
            if (S.empty()) { throw 1; } left = S.top(); S.pop();
            result = new ConverseImplicationNode(left, right);
            break;
        case '+':
            if (S.empty()) { throw 1; } right = S.top(); S.pop();
            if (S.empty()) { throw 1; } left = S.top(); S.pop();
            result = new ExclusiveOrNode(left, right);
            break;
        case '=':
            if (S.empty()) { throw 1; } right = S.top(); S.pop();
            if (S.empty()) { throw 1; } left = S.top(); S.pop();
            result = new EqualNode(left, right);
            break;
        case '|':
            if (S.empty()) { throw 1; } right = S.top(); S.pop();
            if (S.empty()) { throw 1; } left = S.top(); S.pop();
            result = new ShefferStrokeNode(left, right);
            break;
        case '^':
            if (S.empty()) { throw 1; } right = S.top(); S.pop();
            if (S.empty()) { throw 1; } left = S.top(); S.pop();
            result = new PierceArrowNode(left, right);
            break;
        case 'x':
            ++index;
            variableIndex = str[index] - '0';
            result = new ParamNode(variableIndex);
            break;
        default:
            if (ch == '0' || ch == '1') { 
                result = new boolNode(static_cast<bool>(ch - '0'));
            }
            else if ((ch >= 'a' && ch < 'v') || (ch > 'v' && ch <= 'z') ||
                (ch >= 'A' && ch <= 'Z')) {
                result = new ParamNode(ch);
            }
            else { if (S.empty()) { throw 1; }; S.pop(); }
        }
        S.push(result);
        ++index;
    }
    if (S.size() > 1) {
        left = right = nullptr;
        throw 1;
    }
    result = S.top();
    return result;
}

const unsigned char BooleanExpression::ActionsTable[][7] = {
    //\0  ~  &  O  (  )  P
      {7, 2, 2, 2, 2, 5, 1},// \0
      {3, 2, 3, 3, 2, 3, 1},// ~
      {3, 2, 3, 3, 2, 3, 1},// &
      {3, 2, 2, 3, 2, 3, 1},// O
      {6, 2, 2, 2, 2, 4, 1},// (
};

int BooleanExpression::ActionsRowNumber(char ch) {
    switch (ch) {
    case 0:
        return 0;
    case '~':
        return 1;
    case '&':
        return 2;
    case '(':
        return 4;
    }
    return 3;
}

int BooleanExpression::ActionsColNumber(char ch) {
    switch (ch) {
    case 0:
        return 0;
    case '~':
        return 1;
    case '&':
        return 2;
    case '(':
        return 4;
    case ')':
        return 5;
    }
    if ((ch >= 'a' && ch < 'v') || (ch > 'v' && ch <= 'z'))
    {
        return 6;
    }
    if (ch >= 'A' && ch <= 'Z') {
        return 6;
    }
    if (ch >= '0' && ch <= '9') {
        return 6;
    }

    return 3;
}

void BooleanExpression::InfixFilter(const char* instr, char* outstr) {
    int i = 0;
    int j = 0;
    char ch = 0;
    char previous = 0;
    char buffer[256];
    int buffer_length = 0;
    while ((ch = instr[i++]) != 0) {
        if ((ch < '0' || ch > '9') && previous == 'x') {
            throw 1;
        }
        if (ch == ' ' || ch == '\t') { continue; }
        if (ch >= '0' && ch <= '9') {
            if (previous != 'x') {
                buffer[buffer_length++] = ch;
                continue;
            }
            outstr[j++] = ch;
        }
        else {
            if (buffer_length == 1) {
                outstr[j++] = buffer[0];
                buffer_length = 0;
            }
            outstr[j++] = ch;
        }
        previous = ch;
    }
    if (buffer_length > 0) {
        outstr[j++] = buffer[0];
    }
    outstr[j] = '\0';
}

void BooleanExpression::Infix2Postfix(const char* instr, char* outstr) {
    int i = 0;
    int j = 0;
    int row;
    int col;
    unsigned char action;
    std::stack<char> S;
    bool isP = static_cast<bool>(0);
    do {
        col = ActionsColNumber(instr[i]);
        row = S.empty() ? 0 : ActionsRowNumber(S.top());
        action = ActionsTable[row][col];
        switch (action) {
        case 1:
            if (j > 0 && ((outstr[j - 1] >= '0') && (outstr[j - 1] <= '9')))
            {
                throw "error";
            }
            outstr[j] = instr[i]; ++j; ++i; isP = static_cast<bool>(1); break;
        case 2: S.push(instr[i]); ++i;
            if (isP) {
                outstr[j] = '.';
                ++j;
                isP = static_cast<bool>(0);
            }
            break;
        case 3: outstr[j] = S.top(); ++j; S.pop(); break;
        case 4: S.pop(); ++i; break;
        case 7: outstr[j] = '\0'; break;
        default:
            throw "error"; 
            break;
        }
    } while (action != 7);
}

BooleanExpression::BooleanExpression(FormulaNode* r) : truth_table(nullptr), truth_table_size(0), size(0), root(r) {}

void BooleanExpression::calcTruthTable() {
    if (truth_table != nullptr) {
        return;
    }

    root->ParamToTree();
    truth_table = new bool[truth_table_size];
    for (int i = 0; i < truth_table_size; ++i) {
        truth_table[i] = root->calc();
        global::nextBool();
    }
    global::Workspace.clear();
}

std::string BooleanExpression::str() const {
    if (root != nullptr) {
        return root->str();
    }

    return { "" };
}

BooleanExpression::operator std::string() const {
    if (root != nullptr) {
        return root->str();
    }

    return { "" };
}

BooleanExpression BooleanExpression::dnf() {
    FormulaNode* result = nullptr;
    root->ParamToTree();
    for (int i = 0; i < truth_table_size; ++i) {
        if (root->calc()) {
            FormulaNode* temp = nullptr;
            auto it = global::Workspace.begin();
            while (it != global::Workspace.end()) {
                if (temp == nullptr) {
                    temp = new ParamNode(it->getParam());
                    if (!it->getValue()) {
                        temp = new DisNode(temp);
                    }
                    ++it;
                    continue;
                }
                
                if (!it->getValue())
                {
                    temp = new ConjunctionNode(temp, new DisNode(new ParamNode(it->getParam())));
                }
                else {
                    temp = new ConjunctionNode(temp, new ParamNode(it->getParam()));
                }
                
                ++it;
            }
            if (result == nullptr)
            {
                result = temp;
            }
            else {
                result = new DisjunctionNode(result, temp);
            }
        }
        global::nextBool();
    }
    global::Workspace.clear();
    return BooleanExpression(result);
}

BooleanExpression BooleanExpression::cnf() {
    FormulaNode* result = nullptr;
    root->ParamToTree();
    for (int i = 0; i < truth_table_size; ++i) {
        if (!root->calc()) {
            FormulaNode* temp = nullptr;
            auto it = global::Workspace.begin();
            while (it != global::Workspace.end()) {
                if (temp == nullptr) {
                    temp = new ParamNode(it->getParam());
                    if (it->getValue()) {
                        temp = new DisNode(temp);
                    }
                    ++it;
                    continue;
                }
                
                if (!it->getValue())
                {
                    temp = new DisjunctionNode(temp, new ParamNode(it->getParam()));
                }
                else {
                    temp = new DisjunctionNode(temp, new DisNode(new ParamNode(it->getParam())));
                }
                
                ++it;
            }
            if (result == nullptr)
            {
                result = temp;
            }
            else {
                result = new ConjunctionNode(result, temp);
            }
        }
        global::nextBool();
    }
    global::Workspace.clear();
    return BooleanExpression(result);
}

void findZhegalkinPoly(int n, bool*& a, std::vector<int>& res)
{
    std::vector<std::vector<int> > f(n + 1, std::vector<int>(1 << n));

    for (int i = 0; i < (1 << n); i++) {
        f[0][i] = static_cast<int>(a[i]);
    }

    for (int i = 1; i <= n; i++) {
        for (int j = 0; j < (1 << n); j++) {
            f[i][j] = f[i - 1][j];
            if (static_cast<bool>(j & (1 << (i - 1)))) {
                f[i][j] ^= f[i - 1][j ^ (1 << (i - 1))];
            }
        }
    }

    for (int i = 0; i < (1 << n); i++) {
        res.push_back(f[n][i]);
    }
}

BooleanExpression BooleanExpression::zhegalkin() {
    calcTruthTable();

    findZhegalkinPoly(static_cast<int>(size), truth_table, zhegalkinCoefficients);

    FormulaNode* result = nullptr;
    root->ParamToTree();
    for (int i = 0; i < truth_table_size; ++i) {
        if (static_cast<bool>(zhegalkinCoefficients[i])) {
            FormulaNode* current = nullptr;
            auto it = global::Workspace.begin();
            while (it != global::Workspace.end()) {
                if (it->getValue()) {
                    if (current == nullptr)
                    {
                        current = static_cast<FormulaNode*>(new ParamNode(it->getParam()));
                    }
                    else {
                        current = static_cast<FormulaNode*>(new ConjunctionNode(current, new ParamNode(it->getParam())));
                    }
                }
                ++it;
            }
            if (current == nullptr) {
                current = new boolNode(true);
            }
            if (result == nullptr)
            {
                result = current;
            }
            else {
                result = new ExclusiveOrNode(current, result);
            }
        }
        global::nextBool();
    }
    global::Workspace.clear();
    return BooleanExpression(result);
}

BooleanExpression::BooleanExpression(BooleanExpression&& other) noexcept {
    truth_table_size = other.truth_table_size;
    size = other.size;
    truth_table = other.truth_table;
    zhegalkinCoefficients = other.zhegalkinCoefficients;
    root = other.root;
    other.truth_table_size = 0;
    other.size = 0;
    other.truth_table = nullptr;
    other.zhegalkinCoefficients.clear();
    other.root = nullptr;
}

void BooleanExpression::checkClass(int post[5]) {
    calcTruthTable();
    zhegalkin();

    if (truth_table[0]) {
        post[0] = 1;
    }
    if (!truth_table[truth_table_size - 1]) {
        post[1] = 1;
    }
    for (int i = 1; i < truth_table_size; ++i) {
        if (static_cast<int>(truth_table[i]) < static_cast<int>(truth_table[i - 1])) {
            post[2] = 1;
        }
    }
    for (int i = 0; i < truth_table_size / 2; ++i) {
        if (truth_table[i] != truth_table[truth_table_size - i - 1]) {
            post[3] = 1;
        }
    }
    for (int i = 3; i < truth_table_size; ++i) {
        if (((i & (i - 1)) != 0) && static_cast<bool>(zhegalkinCoefficients[i])) {
            post[4] = 1;
        }
    }
}

bool isFullSystem(std::vector<BooleanExpression>& expressions) {
    int post[5] = { 0,0,0,0,0 };
    for (auto& exp : expressions) {
        exp.checkClass(post);
    }

    for (int i = 0; i < 5; ++i)
    {
        if (post[i] == 0)
        {
            return false;
        }
    }

    return true;
}

BooleanExpression::~BooleanExpression() {
    delete[] truth_table;
    delete root;
}
