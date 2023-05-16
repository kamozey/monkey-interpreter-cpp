//
// Created by rahultumpala on 28/4/23.
//

#include "../object/object.h"
#include "evaluator.h"
#include <iostream>
#include <stdexcept>
#include <stdarg.h>

object *eval(astNs::astNode *node)
{
    astNs::program *program = dynamic_cast<astNs::program *>(node);
    if (program != nullptr)
        return evalProgram(program);

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
        if (isErrorObj(right))
            return right;
        return evalPrefixExpression(prefixExpr->prefixOperator, right);
    }

    astNs::infixExpression *infixExpr = dynamic_cast<astNs::infixExpression *>(node);
    if (infixExpr != nullptr)
    {
        object *left = eval(infixExpr->left);
        object *right = eval(infixExpr->right);
        if (isErrorObj(left))
            return left;
        if (isErrorObj(right))
            return right;
        return evalInfixExpression(infixExpr->infixOperator, left, right);
    }

    astNs::ifExpression *ifExpr = dynamic_cast<astNs::ifExpression *>(node);
    if (ifExpr != nullptr)
    {
        return evalIfExpression(ifExpr);
    }

    astNs::blockStatement *blockStmt = dynamic_cast<astNs::blockStatement *>(node);
    if (blockStmt != nullptr)
    {
        return evalBlockStatment(blockStmt);
    }

    astNs::returnStatement *returnStmt = dynamic_cast<astNs::returnStatement *>(node);
    if (returnStmt != nullptr)
    {
        object *value = eval(returnStmt->returnValue);
        if (isErrorObj(value))
            return value;
        return new ReturnValue(value);
    }

    return null;
}

object *evalProgram(astNs::program *program)
{
    object *result;
    for (astNs::statement *stmt : program->statements)
    {
        result = eval(stmt);

        // checking to see if we encountered a return statement
        if (result != nullptr && result->getType() == objectType::return_value_obj)
        {
            ReturnValue *returnObject = dynamic_cast<ReturnValue *>(result);
            return returnObject->value;
        }

        if (result != nullptr && result->getType() == objectType::error_obj)
        {
            Error *returnObject = dynamic_cast<Error *>(result);
            return result;
        }
    }
    return result;
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
    return newError("unknown operator: %s %s", prefixOperator, right->getTypeString());
}

object *evalBangOperatorExpression(object *right)
{
    Boolean *obj = dynamic_cast<Boolean *>(right);
    if (obj != nullptr && obj->value)
        return False;
    if (obj != nullptr && !obj->value)
        return True;
    Null *nullobj = dynamic_cast<Null *>(right); // evals to !null which is true
    if (nullobj != nullptr)
        return True;
    return False;
}

object *evalMinusPrefixOperatorExpression(object *right)
{
    Integer *obj = dynamic_cast<Integer *>(right);
    if (obj == nullptr)
        return newError("unknown operator: -%s", right->getTypeString());
    int val = obj->value;
    return new Integer(-val);
}

object *evalInfixExpression(std::string infixOperator, object *left, object *right)
{
    if (left->getType() != right->getType())
    {
        return newError("type mismatch: %s %s %s", left->getTypeString(), infixOperator, right->getTypeString());
    }
    if (left->getType() == integer_obj && right->getType() == integer_obj)
    {
        Integer *l_int = dynamic_cast<Integer *>(left);
        Integer *r_int = dynamic_cast<Integer *>(right);
        return evalIntegerInfixExpression(infixOperator, l_int, r_int);
    }
    return newError("unknown operator: %s %s %s", left->getTypeString(), infixOperator, right->getTypeString());
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
    return newError("unknown operator: %s %s %s", left->getTypeString(), infixOperator, right->getTypeString());
}

object *nativeBoolToBooleanObject(bool val)
{
    if (val)
        return True;
    return False;
}

object *evalIfExpression(astNs::ifExpression *ifExpr)
{
    object *condition = eval(ifExpr->condition);
    if (isErrorObj(condition))
        return condition;
    // condition is truthy if it evalautes to any object other than False, null objects
    if (condition != False && condition != null)
    {
        return eval(ifExpr->evalTrue);
    }
    return eval(ifExpr->evalFalse);
}

object *evalBlockStatment(astNs::blockStatement *blockStatement)
{
    object *result;
    for (astNs::statement *stmt : blockStatement->stmts)
    {
        result = eval(stmt);
        if (result != nullptr && (result->getType() == objectType::return_value_obj || result->getType() == objectType::error_obj))
        {
            return result;
        }
    }
    return result;
}

Error *newError(string format, string args...)
{
    cout << format << endl;
    va_list argptr;
    va_start(argptr, args);
    char sval[1024*64];
    vsprintf(sval, format.c_str(), argptr);
    va_end(argptr);
    string msg(sval);
    cout << msg << endl;
    Error *errorObj = new Error(msg);
    return errorObj;
}

bool isErrorObj(object *obj)
{
    if (obj != nullptr)
        return obj->getType() == objectType::error_obj;
    return false;
}