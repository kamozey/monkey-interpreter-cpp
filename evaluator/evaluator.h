//
// Created by rahultumpala on 28/4/23.
//

#include "../object/object.h"
#include "../ast/ast.h"

#ifndef MONKEYINTERPRETER_EVALUATOR_H
#define MONKEYINTERPRETER_EVALUATOR_H


object *eval(astNs::astNode *node);

object *evalStatements(vector<astNs::statement *> statements);

#endif //MONKEYINTERPRETER_EVALUATOR_H
