#ifndef __VALIDATION_H__
#define __VALIDATION_H__

#include "ASTNode.hh"

namespace sieve {

typedef bool (*ValidationFunctionPtr)(const ASTNode *);

template <class T>
bool nodeIsType(const ASTNode *node) {
    const T *t_node = dynamic_cast<const T*>(node);
    return t_node != NULL;
}

}

#endif
