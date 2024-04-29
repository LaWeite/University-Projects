#include <iostream>
#include <fstream>
#include <sstream>

struct Node {
    char type;
    unsigned int nodeNumber;
    int numKeys;
    int* keys;
    int numChildren;
    unsigned int* children;

    Node() : nodeNumber(0), type(' '), numKeys(0), numChildren(0), keys(nullptr), children(nullptr) {}
    ~Node() {
        delete[] keys;
        delete[] children;
    }
};

static bool verifyChildrenKeys(Node* nodes, int numNodes, int index, int left, int right) {
    for (int i = 0; i < nodes[index].numKeys; ++i)
    {
        if (nodes[index].keys[i] < left || nodes[index].keys[i] > right)
        {
            return false;
        }
    }

    if (nodes[index].type == 'l') {
        return true;
    }

    for (int i = 0; i < nodes[index].numChildren; ++i)
    {
        int newIndexOfNode = -1;
        for (int j = 0; j < numNodes; ++j)
        {
            if (nodes[j].nodeNumber == nodes[index].children[i])
            {
                newIndexOfNode = j;
                break;
            }
        }

        if (newIndexOfNode == -1)
        {
            return false;
        }

        int newLeft = i != 0 ? nodes[index].keys[i - 1] : left;
        int newRight = i != nodes[index].numChildren - 1 ? nodes[index].keys[i] : right;

        if (!verifyChildrenKeys(nodes, numNodes, newIndexOfNode, newLeft, newRight))
        {
            return false;
        }
    }
    
    return true;
}

static int getNodeLevel(Node* nodes, int N, int root, unsigned int targetNode) {
    if (root == targetNode) {
        return 0;
    }

    for (int i = 0; i < N; ++i) {
        if (nodes[i].nodeNumber == targetNode) {
            for (int j = 0; j < nodes[i].numChildren; ++j) {
                int childLevel = getNodeLevel(nodes, N, root, nodes[i].children[j]);
                if (childLevel != -1) {
                    return 1 + childLevel;
                }
            }
        }
    }

    return -1;
}

static bool checkLeafNodesSameLevel(Node* nodes, int N, int root) {
    int leafLevel = -1;
    bool firstLeafFound = false;

    for (int i = 0; i < N; ++i) {
        if (nodes[i].type == 'l') {
            if (!firstLeafFound) {
                leafLevel = getNodeLevel(nodes, N, root, nodes[i].nodeNumber);
                firstLeafFound = true;
            }
            else {
                if (leafLevel != getNodeLevel(nodes, N, root, nodes[i].nodeNumber)) {
                    return false;
                }
            }
        }
    }

    return true;
}

static bool checkBTreeProperties(Node* nodes, int t, int root, int N) {
    for (int i = 0; i < N; ++i) {
        if (nodes[i].nodeNumber != root && nodes[i].numKeys < t - 1) {
            return false;
        }

        if (nodes[i].numChildren != nodes[i].numKeys + 1 && nodes[i].type != 'l')
        {
            return false;
        }

        if (nodes[i].type == 'b' && nodes[i].numChildren < t && nodes[i].nodeNumber != root) {
            return false;
        }

        if (nodes[i].nodeNumber == root) {
            if (nodes[i].numKeys < 1) {
                return false;
            }
        }

        if (nodes[i].nodeNumber != root && (nodes[i].numKeys > 2 * t - 1 || nodes[i].numChildren > 2 * t)) {
            return false;
        }

        if (nodes[i].nodeNumber == root && (nodes[i].numKeys < 1 || nodes[i].numKeys > 2 * t - 1 || nodes[i].numChildren < 2 || nodes[i].numChildren > 2 * t) && N > 1) {
            return false;
        }
    }

    if (!verifyChildrenKeys(nodes, N, 0, -2147483647, 2147483647)) {
        return false;
    }

    if (!checkLeafNodesSameLevel(nodes, N, root)) {
        return false;
    }
    
    return true;
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        return 1;
    }

    std::ifstream inputFile(argv[1]);
    if (!inputFile.is_open()) {
        std::cerr << "Error opening file." << std::endl;
        return 1;
    }

    int N, t, root;
    inputFile >> N >> t >> root;

    Node* nodes = new Node[N];

    char str[1024];
    int indexOfNodes = 0;
    while (inputFile.getline(str, 1024, '\n') && indexOfNodes < N) {
        if (str[0] == '\0')
        {
            continue;
        }

        std::istringstream iss(str);

        char nodeType;
        iss >> nodeType;

        if (nodeType == 'l') {
            nodes[indexOfNodes].type = 'l';

            iss.ignore(7);
            iss >> std::dec >> nodes[indexOfNodes].nodeNumber;

            iss.ignore(2);
            iss >> std::dec >> nodes[indexOfNodes].numKeys;

            iss.ignore(1);

            int key;
            int prev = 0;
            nodes[indexOfNodes].keys = new int[nodes[indexOfNodes].numKeys];
            for (int i = 0; i < nodes[indexOfNodes].numKeys; ++i) {
                iss >> std::dec >> key;
                if (i != 0 && prev > key)
                {
                    inputFile.close();
                    std::cout << "no";
                    delete[] nodes;
                    return 0;
                }
                prev = key;
                nodes[indexOfNodes].keys[i] = key;
            }
        }
        else if (nodeType == 'b') {
            nodes[indexOfNodes].type = 'b';

            iss.ignore(9);
            iss >> std::dec >> nodes[indexOfNodes].nodeNumber;

            iss.ignore(2);

            iss >> nodes[indexOfNodes].numKeys;

            iss.ignore(1);

            int key;
            int prev = 0;
            nodes[indexOfNodes].keys = new int[nodes[indexOfNodes].numKeys];
            for (int i = 0; i < nodes[indexOfNodes].numKeys; ++i) {
                iss >> std::dec >> key;
                if (i != 0 && prev > key)
                {
                    inputFile.close();
                    delete[] nodes;
                    std::cout << "no";
                    return 0;
                }
                prev = key;
                nodes[indexOfNodes].keys[i] = key;
            }

            iss.ignore(3);

            iss >> nodes[indexOfNodes].numChildren;

            iss.ignore(1);

            unsigned int child;
            nodes[indexOfNodes].children = new unsigned int[nodes[indexOfNodes].numChildren];
            for (int i = 0; i < nodes[indexOfNodes].numChildren; ++i) {
                iss >> std::dec >> child;
                nodes[indexOfNodes].children[i] = child;
            }
        }

        iss.ignore(1);

        char err[1024];
        err[0] = ' ';
        iss >> err;
        if (err[0] != ' ')
        {
            inputFile.close();
            delete[] nodes;
            std::cout << "no";
            return 0;
        }

        ++indexOfNodes;
    }

    inputFile.close();

    if (checkBTreeProperties(nodes, t, root, N)) {
        std::cout << "yes";
    }
    else {
        std::cout << "no";
    }

    delete[] nodes;
    return 0;
}
