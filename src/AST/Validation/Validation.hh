#ifndef __VALIDATION_H__
#define __VALIDATION_H__

#include "ASTNode.hh"

namespace sieve {

typedef bool (*ValidationFunctionPtr)(const ASTNode *);

}

#endif
