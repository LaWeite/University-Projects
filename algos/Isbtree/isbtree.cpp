#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

struct Node {
    char type;
    unsigned int nodeNumber;
    int numKeys;
    std::vector<int> keys;
    int numChildren;
    std::vector<unsigned int> children;

    Node() : nodeNumber(0), type(' '), numKeys(0), numChildren(0) {}
};

int getNodeLevel(const std::vector<Node>& nodes, int root, unsigned int targetNode);
bool checkLeafNodesSameLevel(const std::vector<Node>& nodes, int root);

int getHeight(const std::vector<Node>& nodes, unsigned int currentNodeNumber) {
    int maxHeight = 0;
    for (const Node& node : nodes) {
        if (node.nodeNumber == currentNodeNumber) {
            if (node.numChildren == 0) {
                return 1;
            }
            else {
                int maxChildHeight = 0;
                for (unsigned int childNumber : node.children) {
                    int childHeight = getHeight(nodes, childNumber);
                    if (childHeight > maxChildHeight) {
                        maxChildHeight = childHeight;
                    }
                }
                return 1 + maxChildHeight;
            }
        }
    }
    return 0;
}

static bool checkBTreeProperties(const std::vector<Node>& nodes, int t, int root) {
    int height = getHeight(nodes, root);

    for (const Node& node : nodes) {
        // Check property 1: Each node, except the root, must have at least t-1 keys
        if (node.nodeNumber != root && node.numKeys < t - 1) {
            return false;
        }

        // Check property 2: Each internal node must have at least t children
        if (node.type == 'b' && node.numChildren < t && node.nodeNumber != root) {
            return false;
        }

        // Check property 3: The root must have at least one key
        if (node.nodeNumber == root) {
            if (node.numKeys < 1) {
                return false;
            }
        }

        // Check property 4: Each node, except the root, must have at most 2t-1 keys and 2t children
        if (node.nodeNumber != root && (node.numKeys > 2 * t - 1 || node.numChildren > 2 * t)) {
            return false;
        }

        // Check property 5: The root must have between 1 and 2t-1 keys and between 2 and 2t children for a height greater than 0
        if (node.nodeNumber == root && (node.numKeys < 1 || node.numKeys > 2 * t - 1 || node.numChildren < 2 || node.numChildren > 2 * t) && height != 1) {
            return false;
        }
    }

    // Check property 6: Validate the key range property within each node

    // Check property 7: Confirm that all leaf nodes are at the same level
    if (!checkLeafNodesSameLevel(nodes, root)) {
        return false;
    }
    
    return true;
}

bool checkLeafNodesSameLevel(const std::vector<Node>& nodes, int root) {
    int leafLevel = -1;
    bool firstLeafFound = false;

    for (const Node& node : nodes) {
        if (node.type == 'l') {
            if (!firstLeafFound) {
                leafLevel = getNodeLevel(nodes, root, node.nodeNumber);
                firstLeafFound = true;
            }
            else {
                if (leafLevel != getNodeLevel(nodes, root, node.nodeNumber)) {
                    return false;
                }
            }
        }
    }

    return true;
}

int getNodeLevel(const std::vector<Node>& nodes, int root, unsigned int targetNode) {
    if (root == targetNode) {
        return 0;
    }

    for (const Node& node : nodes) {
        if (node.nodeNumber == targetNode) {
            for (unsigned int child : node.children) {
                int childLevel = getNodeLevel(nodes, root, child);
                if (childLevel >= 0) {
                    return 1 + childLevel;
                }
            }
        }
    }

    return -1;
}

int main() {
    std::ifstream inputFile("input.txt");
    if (!inputFile.is_open()) {
        std::cerr << "Error opening file." << std::endl;
        return 1;
    }

    int N, t, root;
    inputFile >> N >> t >> root;

    std::vector<Node> nodes;

    std::string line;
    char str[1024];
    while (inputFile.getline(str, 1024, '\n')) {
        if (std::strcmp(str, "") == 0)
        {
            continue;
        }

        std::istringstream iss(str);
        Node node;

        char nodeType;
        iss >> nodeType;

        if (nodeType == 'l') {
            node.type = 'l';

            iss.ignore(7);
            iss >> std::dec >> node.nodeNumber;

            iss.ignore(2);
            iss >> std::dec >> node.numKeys;

            iss.ignore(1);

            int key;
            for (int i = 0; i < node.numKeys; ++i) {
                iss >> std::dec >> key;
                node.keys.push_back(key);
            }
        }
        else if (nodeType == 'b') {
            node.type = 'b';

            iss.ignore(9);
            iss >> std::dec >> node.nodeNumber;

            iss.ignore(2);

            iss >> node.numKeys;

            iss.ignore(1);

            int key;
            for (int i = 0; i < node.numKeys; ++i) {
                iss >> std::dec >> key;
                node.keys.push_back(key);
            }

            iss.ignore(3);

            iss >> node.numChildren;

            iss.ignore(1);

            unsigned int child;
            for (int i = 0; i < node.numChildren; ++i) {
                iss >> std::dec >> child;
                node.children.push_back(child);
            }
        }

        iss.ignore(1);

        char err[1024];
        iss >> err;
        if (std::strcmp(err, "") != 0)
        {
            inputFile.close();
            std::cout << "no";
            return 0;
        }

        nodes.push_back(node);
    }

    inputFile.close();

    if (checkBTreeProperties(nodes, t, root)) {
        std::cout << "yes";
    }
    else {
        std::cout << "no" << std::endl;
    }

    return 0;
}
