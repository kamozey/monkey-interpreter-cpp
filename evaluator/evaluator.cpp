//
// Created by rahultumpala on 28/4/23.
//

#include "../object/object.h"
#include "evaluator.h"

object *eval(astNs::astNode *node)
{
    astNs::program *program = dynamic_cast<astNs::program *>(node);
    if (program != nullptr)
        return evalStatements(program->statements);

    astNs::expressionStatement *exprStmt = dynamic_cast<astNs::expressionStatement *>(node);
    if (exprStmt != nullptr)
        return eval(exprStmt->expr);

    astNs::integerLiteral *integerLiteral = dynamic_cast<astNs::integerLiteral *>(node);
    if (integerLiteral != nullptr)
        return new Integer(integerLiteral->value);

    astNs::booleanLiteral *booleanLiteral = dynamic_cast<astNs::booleanLiteral*>(node);
    if(booleanLiteral != nullptr){
        if(booleanLiteral->value) 
            return True;
        return False;
    }

    astNs::prefixExpression *prefixExpr = dynamic_cast<astNs::prefixExpression *>(node);
    if (prefixExpr != nullptr)
    {
        object *right = eval(prefixExpr->right);
        return evalPrefixExpression(prefixExpr->prefixOperator, right);
    }

    return null;
}

object *evalStatements(vector<astNs::statement *> stmts)
{
    object *obj;
    for (astNs::statement *stmt : stmts)
    {
        obj = eval(stmt);
    }
    return obj;
}

object *evalPrefixExpression(std::string prefixOperator, object *right)
{
    if (prefixOperator == "bang")
    {
        return evalBangOperatorExpression(right);
    }
    else if (prefixOperator == "minus")
    {
        return evalMinusPrefixOperatorExpression(right);
    }
    return null;
}

object *evalBangOperatorExpression(object *right)
{
    Boolean *obj = dynamic_cast<Boolean *>(right);
    if(obj != nullptr && obj->value) return False;
    if(obj != nullptr && !obj->value) return True;
    return False;
}

object *evalMinusPrefixOperatorExpression(object *right)
{
    Integer *obj = dynamic_cast<Integer *>(right);
    if(obj == nullptr) return null;
    int val = obj->value;
    return new Integer(-val);
}