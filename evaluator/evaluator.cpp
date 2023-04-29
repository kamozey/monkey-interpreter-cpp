//
// Created by rahultumpala on 28/4/23.
//

#include "../object/object.h"
#include "evaluator.h"

object *eval(astNs::astNode *node) {
    astNs::program *program = dynamic_cast<astNs::program *>(node);
    if (program != nullptr)
        return evalStatements(program->statements);

    astNs::expressionStatement *exprStmt = dynamic_cast<astNs::expressionStatement *>(node);
    if (exprStmt != nullptr)
        return eval(exprStmt->expr);

    astNs::integerLiteral *integerLiteral = dynamic_cast<astNs::integerLiteral *>(node);
    if (integerLiteral != nullptr)
        return new Integer(integerLiteral->value);

    return nullptr;
}

object *evalStatements(vector<astNs::statement *> stmts) {
    object *obj;
    for (astNs::statement *stmt: stmts) {
        obj = eval(stmt);
    }
    return obj;
}