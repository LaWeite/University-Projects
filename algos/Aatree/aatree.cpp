#include <iostream>
#include <fstream>

struct AANode {
    int key;
    int level;
    AANode* left;
    AANode* right;

    AANode(int k) : key(k), level(1), left(nullptr), right(nullptr) {}
};

class AATree {
public:
    AATree() : root(nullptr) {}
    ~AATree() {
        clear(root);
    }

    void insert(int key) {
        root = insert(root, key);
    }

    void remove(int key) {
        root = remove(root, key);
    }

    bool search(int key) {
        return search(root, key) != nullptr;
    }

    int getRootLevel() {
        return root ? root->level : 0;
    }

private:
    AANode* root;

    AANode* skew(AANode* node) {
        if (!node || !node->left || node->left->level != node->level)
            return node;
        AANode* L = node->left;
        node->left = L->right;
        L->right = node;
        return L;
    }

    AANode* split(AANode* node) {
        if (!node || !node->right || !node->right->right || node->right->right->level != node->level)
            return node;
        AANode* R = node->right;
        node->right = R->left;
        R->left = node;
        R->level++;
        return R;
    }

    AANode* insert(AANode* node, int key) {
        if (!node) return new AANode(key);
        if (key < node->key)
            node->left = insert(node->left, key);
        else if (key > node->key)
            node->right = insert(node->right, key);
        node = skew(node);
        node = split(node);
        return node;
    }

    AANode* remove(AANode* node, int key) {
        if (!node) return nullptr;
        if (key < node->key) {
            node->left = remove(node->left, key);
        }
        else if (key > node->key) {
            node->right = remove(node->right, key);
        }
        else {
            if (!node->left && !node->right) {
                delete node;
                return nullptr;
            }
            if (!node->right) {
                AANode* predecessor = node->left;
                while (predecessor->right) predecessor = predecessor->right;
                node->key = predecessor->key;
                node->left = remove(node->left, predecessor->key);
            }
            else {
                AANode* successor = node->right;
                while (successor->left) successor = successor->left;
                node->key = successor->key;
                node->right = remove(node->right, successor->key);
            }
        }
        node = decreaseLevel(node);
        node = skew(node);
        if (node->right) {
            node->right = skew(node->right);
            if (node->right->right) node->right->right = skew(node->right->right);
        }
        node = split(node);
        if (node->right) node->right = split(node->right);
        return node;
    }

    AANode* decreaseLevel(AANode* node) {
        if (node == nullptr) return nullptr;

        int leftLevel = (node->left != nullptr) ? node->left->level : 0;
        int rightLevel = (node->right != nullptr) ? node->right->level : 0;
        int shouldBe = std::min(leftLevel, rightLevel) + 1;

        if (shouldBe < node->level) {
            node->level = shouldBe;
            if (node->right && shouldBe < node->right->level) {
                node->right->level = shouldBe;
            }
        }
        return node;
    }

    AANode* search(AANode* node, int key) {
        if (!node) return nullptr;
        if (key < node->key)
            return search(node->left, key);
        else if (key > node->key)
            return search(node->right, key);
        else
            return node;
    }

    void clear(AANode* node) {
        if (node) {
            clear(node->left);
            clear(node->right);
            delete node;
        }
    }
};

int main(int argc, char* argv[])
{
    std::ifstream input_file(argv[1]);
    std::ofstream output_file(argv[2]);

    if (!input_file.is_open() || !output_file.is_open())
    {
        return 1;
    }

    int amount;
    input_file >> amount;

    AATree l;

    char argument;
    int number;
    for (int i = 0; i < amount; ++i)
    {
        input_file >> argument;
        if (argument == '+')
        {
            input_file >> number;
            l.insert(number);
            output_file << l.getRootLevel();
        }
        else if (argument == '-')
        {
            input_file >> number;
            l.remove(number);
            output_file << l.getRootLevel();
        }
        else if (argument == '?')
        {
            input_file >> number;

            (l.search(number)) ? output_file << "true" : output_file << "false";
        }

        output_file << std::endl;
    }


    input_file.close();
    output_file.close();

    return 0;
}
