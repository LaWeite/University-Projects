#include "formulanodes.h"

namespace global {
    AVLTree<ParamValue> Workspace;

    void nextBool() {
        for (auto it = Workspace.rbegin(); it != Workspace.rend(); ++it) {
            if (!it->getValue()) {
                it->value = true;
                return;
            }
            
            it->value = false;
        }
    }
}
