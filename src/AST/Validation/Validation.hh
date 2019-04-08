#ifndef __VALIDATION_H__
#define __VALIDATION_H__

#include "ASTNode.hh"

namespace sieve {

typedef bool (*ValidationFunctionPtr)(const ASTNode *);

template <class T>
bool nodeIsType(ASTNode *node) {
    T *t_node = dynamic_cast<T*>(node);
    return t_node != NULL;
}

}

#endif
