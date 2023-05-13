//
// Created by rahultumpala on 28/4/23.
//

#include "../object/object.h"
#include "evaluator.h"
#include <iostream>
#include <stdexcept>

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

    astNs::booleanLiteral *booleanLiteral = dynamic_cast<astNs::booleanLiteral *>(node);
    if (booleanLiteral != nullptr)
    {
        if (booleanLiteral->value)
            return True;
        return False;
    }

    astNs::prefixExpression *prefixExpr = dynamic_cast<astNs::prefixExpression *>(node);
    if (prefixExpr != nullptr)
    {
        object *right = eval(prefixExpr->right);
        return evalPrefixExpression(prefixExpr->prefixOperator, right);
    }

    astNs::infixExpression *infixExpr = dynamic_cast<astNs::infixExpression *>(node);
    if (infixExpr != nullptr)
    {
        object *left = eval(infixExpr->left);
        object *right = eval(infixExpr->right);
        return evalInfixExpression(infixExpr->infixOperator, left, right);
    }

    astNs::ifExpression *ifExpr = dynamic_cast<astNs::ifExpression *>(node);
    if(ifExpr != nullptr){
        return evalIfExpression(ifExpr);
    }

    astNs::blockStatement *blockStmt = dynamic_cast<astNs::blockStatement *>(node);
    if(blockStmt != nullptr){
        return evalStatements(blockStmt->stmts);
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
    if (obj != nullptr && obj->value)
        return False;
    if (obj != nullptr && !obj->value)
        return True;
    Null *nullobj = dynamic_cast<Null *>(right); // evals to !null which is true
    if(nullobj != nullptr) return True;
    return False;
}

object *evalMinusPrefixOperatorExpression(object *right)
{
    Integer *obj = dynamic_cast<Integer *>(right);
    if (obj == nullptr)
        return null;
    int val = obj->value;
    return new Integer(-val);
}

object *evalInfixExpression(std::string infixOperator, object *left, object *right)
{
    if (left->getType() == integer_obj && right->getType() == integer_obj)
    {
        Integer *l_int = dynamic_cast<Integer *>(left);
        Integer *r_int = dynamic_cast<Integer *>(right);
        return evalIntegerInfixExpression(infixOperator, l_int, r_int);
    }
}

object *evalIntegerInfixExpression(std::string infixOperator, Integer *left, Integer *right)
{
    int lval = left->value;
    int rval = right->value;
    if (infixOperator == "plus")
    {
        return new Integer(lval + rval);
    }
    else if (infixOperator == "minus")
    {
        return new Integer(lval - rval);
    }
    else if (infixOperator == "asterisk")
    {
        return new Integer(lval * rval);
    }
    else if (infixOperator == "division")
    {
        if (rval == 0)
            throw std::runtime_error("cannot divide by zero. check expression");
        return new Integer(lval / rval);
    }
    else if (infixOperator == "gte")
    {
        return nativeBoolToBooleanObject(lval >= rval);
    }
    else if (infixOperator == "lte")
    {
        return nativeBoolToBooleanObject(lval <= rval);
    }
    else if (infixOperator == "gt")
    {
        return nativeBoolToBooleanObject(lval > rval);
    }
    else if (infixOperator == "lt")
    {
        return nativeBoolToBooleanObject(lval < rval);
    }
    else if (infixOperator == "eq")
    {
        return nativeBoolToBooleanObject(lval == rval);
    }
    else if (infixOperator == "neq")
    {
        return nativeBoolToBooleanObject(lval != rval);
    }
    return null;
}

object *nativeBoolToBooleanObject(bool val){
    if(val) return True;
    return False;
}

object *evalIfExpression(astNs::ifExpression *ifExpr){
    object *condition = eval(ifExpr->condition);
    // condition is truthy if it evalautes to any object other than False, null objects
    if(condition != False && condition != null){
        return eval(ifExpr->evalTrue);
    }
    return eval(ifExpr->evalFalse);
}