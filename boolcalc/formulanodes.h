#include <string>
#include <iostream>
#include <cstring>
#include <cmath>
#include "avltree.h"

class ParamValue {
    const unsigned int ch;
public:
    bool value;
    ParamValue(const unsigned int c, const  bool value) : ch(c), value(value) {}
    unsigned int getParam() const { 
        return ch;
    }
    bool getValue() const { 
        return value;
    }
    bool operator<(const ParamValue& X)const {
        return ch < X.ch; 
    }
    bool operator==(const ParamValue& X) const { 
        return ch == X.ch; 
    }
};

namespace global {
    extern AVLTree<ParamValue> Workspace;

    extern void nextBool();
}

class FormulaNode {
public:
    virtual bool calc() const = 0;

    virtual void ParamToTree() const = 0;

    virtual std::string str() const = 0;

    virtual ~FormulaNode() = default;
};

class boolNode : public FormulaNode {
    const bool bool_par;
public:
    explicit boolNode(bool x) : bool_par(x) {}

    bool calc() const { 
        return bool_par; 
    }

    void ParamToTree() const {}

    std::string str() const {
        return std::to_string(static_cast<int>(bool_par));
    }
};

class ParamNode : public FormulaNode {
    const unsigned int ch;
public:
    explicit ParamNode(unsigned int s) : ch(s) {}

    bool calc() const {
        auto pos = global::Workspace.find(ParamValue(ch, static_cast<bool>(0)));
        if (pos != global::Workspace.end()) {
            return (*pos).getValue();
        }
        int temp;
        std::cout << ch << " = ";
        std::cin >> temp;
        std::cin.ignore();
        global::Workspace.insert(ParamValue(ch, static_cast<bool>(temp)));
        return static_cast<bool>(temp);
    }

    void ParamToTree() const {
        auto position = global::Workspace.find(ParamValue(ch, static_cast<bool>(0)));
        if (position == global::Workspace.end()) {
            global::Workspace.insert(ParamValue(ch, false));
        }
    }

    std::string str() const {
        return "x" + std::to_string(ch);
    }
};

class BinNode : public FormulaNode {
protected:
    FormulaNode* left;
    FormulaNode* right;
public:
    BinNode(FormulaNode* L, FormulaNode* R) : left(L), right(R) {}

    void ParamToTree() const {
        left->ParamToTree();
        right->ParamToTree();
    }

    ~BinNode() { 
        delete left; 
        delete right; 
        left = right = nullptr; 
    }
};

class ConjunctionNode : public BinNode {
public:
    ConjunctionNode(FormulaNode* L, FormulaNode* R) : BinNode(L, R) {}

    bool calc() const { 
        bool l = left->calc();
        bool r = right->calc();
        return l && r; 
    }

    std::string str() const {
        return "(" + left->str() + " & " + right->str() + ")";
    }
};

class DisjunctionNode : public BinNode {
public:
    DisjunctionNode(FormulaNode* L, FormulaNode* R) : BinNode(L, R) {}

    bool calc() const { 
        bool l = left->calc();
        bool r = right->calc();
        return l || r; 
    }

    std::string str() const {
        return "(" + left->str() + " v " + right->str() + ")";
    }
};

class ImplicationNode : public BinNode {
public:
    ImplicationNode(FormulaNode* L, FormulaNode* R) : BinNode(L, R) {}

    bool calc() const { 
        bool l = left->calc();
        bool r = right->calc();
        return !l || r;
    }

    std::string str() const {
        return "(" + left->str() + " > " + right->str() + ")";
    }
};

class ConverseImplicationNode : public BinNode {
public:
    ConverseImplicationNode(FormulaNode* L, FormulaNode* R) : BinNode(L, R) {}

    bool calc() const {
        bool l = left->calc();
        bool r = right->calc();
        return l || !r;
    }

    std::string str() const {
        return "(" + left->str() + " < " + right->str() + ")";
    }
};

class ExclusiveOrNode : public BinNode {
public:
    ExclusiveOrNode(FormulaNode* L, FormulaNode* R) : BinNode(L, R) {}

    bool calc() const {
        bool l = left->calc();
        bool r = right->calc();
        return (l && !r) || (!l && r);
    }

    std::string str() const {
        return "(" + left->str() + " + " + right->str() + ")";
    }
};

class EqualNode : public BinNode {
public:
    EqualNode(FormulaNode* L, FormulaNode* R) : BinNode(L, R) {}

    bool calc() const {
        bool l = left->calc();
        bool r = right->calc();
        return (!l || r) && (!r || l);
    }

    std::string str() const {
        return "(" + left->str() + " = " + right->str() + ")";
    }
};

class ShefferStrokeNode : public BinNode {
public:
    ShefferStrokeNode(FormulaNode* L, FormulaNode* R) : BinNode(L, R) {}

    bool calc() const {
        bool l = left->calc();
        bool r = right->calc();
        return !l || !r;
    }

    std::string str() const {
        return "(" + left->str() + " | " + right->str() + ")";
    }
};

class PierceArrowNode : public BinNode {
public:
    PierceArrowNode(FormulaNode* L, FormulaNode* R) : BinNode(L, R) {}

    bool calc() const {
        bool l = left->calc();
        bool r = right->calc();
        return !l && !r;
    }

    std::string str() const {
        return "(" + left->str() + " ^ " + right->str() + ")";
    }
};

class UnaryNode : public FormulaNode {
protected:
    FormulaNode* next;
public:
    explicit UnaryNode(FormulaNode* node) : next(node) {}

    void ParamToTree() const {
        next->ParamToTree();
    }

    ~UnaryNode() { 
        delete next; 
        next = nullptr; 
    }
};

class DisNode : public UnaryNode {
public:
    explicit DisNode(FormulaNode* node) : UnaryNode(node) {}

    bool calc() const { 
        return !next->calc(); 
    }

    std::string str() const {
        return "~" + next->str();
    }
};
