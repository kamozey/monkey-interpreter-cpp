//
// Created by rahultumpala on 28/4/23.
//

#include "../object/object.h"
#include "evaluator.h"
#include <iostream>
#include <stdexcept>
#include <stdarg.h>

object *eval(astNs::astNode *node, Environment* env)
{
    astNs::program *program = dynamic_cast<astNs::program *>(node);
    if (program != nullptr)
        return evalProgram(program, env);

    astNs::expressionStatement *exprStmt = dynamic_cast<astNs::expressionStatement *>(node);
    if (exprStmt != nullptr)
        return eval(exprStmt->expr, env);

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
        object *right = eval(prefixExpr->right, env);
        if (isErrorObj(right))
            return right;
        return evalPrefixExpression(prefixExpr->prefixOperator, right);
    }

    astNs::infixExpression *infixExpr = dynamic_cast<astNs::infixExpression *>(node);
    if (infixExpr != nullptr)
    {
        object *left = eval(infixExpr->left, env);
        object *right = eval(infixExpr->right, env);
        if (isErrorObj(left))
            return left;
        if (isErrorObj(right))
            return right;
        return evalInfixExpression(infixExpr->infixOperator, left, right);
    }

    astNs::ifExpression *ifExpr = dynamic_cast<astNs::ifExpression *>(node);
    if (ifExpr != nullptr)
    {
        return evalIfExpression(ifExpr, env);
    }

    astNs::blockStatement *blockStmt = dynamic_cast<astNs::blockStatement *>(node);
    if (blockStmt != nullptr)
    {
        return evalBlockStatment(blockStmt, env);
    }

    astNs::returnStatement *returnStmt = dynamic_cast<astNs::returnStatement *>(node);
    if (returnStmt != nullptr)
    {
        object *value = eval(returnStmt->returnValue, env);
        if (isErrorObj(value))
            return value;
        return new ReturnValue(value);
    }

    astNs::letStatement *letStatement = dynamic_cast<astNs::letStatement *>(node);
    if (letStatement != nullptr)
    {
        object *value = eval(letStatement->value, env);
        if (isErrorObj(value))
            return value;
        env->set(letStatement->name->value, value);
    }

    astNs::identifier *identifier = dynamic_cast<astNs::identifier *>(node);
    if (identifier != nullptr)
    {
        return evalIdentifier(identifier, env);
    }

    astNs::functionLiteral *f_literal = dynamic_cast<astNs::functionLiteral *>(node);
    if (f_literal != nullptr)
    {
        Function *func = new Function();
        func->body = f_literal->body;
        func->parameters = f_literal->args;
        func->env = env;
        return func;
    }

    astNs::callExpression *call_expr = dynamic_cast<astNs::callExpression *>(node);
    if (call_expr != nullptr)
    {
        object *function = eval(call_expr->name, env);
        if (isErrorObj(function))
        {
            return function;
        }
        vector<object *> args = evalExpressions(call_expr->args, env);
        if (args.size() == 1 && isErrorObj(args[0]))
        {
            return args[0];
        }
        return applyFunction(function, args);
    }

    astNs::stringLiteral *stringLiteral = dynamic_cast<astNs::stringLiteral *>(node);
    if(stringLiteral != nullptr){
        return new String(stringLiteral->value);
    }

    astNs::arrayExpression *arrayExpr = dynamic_cast<astNs::arrayExpression *>(node);
    if(arrayExpr != nullptr){
        return evaluateArrayExpressions(arrayExpr, env);
    }

    astNs::arrayAccessExpr *arrayAccessExpr = dynamic_cast<astNs::arrayAccessExpr *>(node);
    if (arrayAccessExpr != nullptr){
        return evaluateArrayAccessExpression(arrayAccessExpr, env);
    }

    astNs::hashLiteral *hashLiteral = dynamic_cast<astNs::hashLiteral *>(node);
    if(hashLiteral != nullptr){
        return evaluateHashLiteral(hashLiteral, env);
    }

    return null;
}

object *evalProgram(astNs::program *program, Environment *env)
{
    object *result;
    for (astNs::statement *stmt : program->statements)
    {
        result = eval(stmt, env);

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
    if(left->getType() == string_obj && right->getType() == string_obj){
        String *l_str = dynamic_cast<String *>(left);
        String *r_str = dynamic_cast<String *>(right);
        return evalStringInfixExpression(infixOperator, l_str, r_str);
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

object *evalStringInfixExpression(std::string infixOperator, String *left, String *right){
    if(infixOperator == "plus"){
        return new String(left->value + right->value);
    }
    return newError("unsupported operator: %s %s %s", left->getTypeString(), infixOperator, right->getTypeString());
}

object *nativeBoolToBooleanObject(bool val)
{
    if (val)
        return True;
    return False;
}

object *evalIfExpression(astNs::ifExpression *ifExpr, Environment *env)
{
    object *condition = eval(ifExpr->condition, env);
    if (isErrorObj(condition))
        return condition;
    // condition is truthy if it evalautes to any object other than False, null objects
    if (condition != False && condition != null)
    {
        return eval(ifExpr->evalTrue, env);
    }
    return eval(ifExpr->evalFalse, env);
}

object *evalBlockStatment(astNs::blockStatement *blockStatement, Environment *env)
{
    object *result;
    for (astNs::statement *stmt : blockStatement->stmts)
    {
        result = eval(stmt, env);
        if (result != nullptr && (result->getType() == objectType::return_value_obj || result->getType() == objectType::error_obj))
        {
            return result;
        }
    }
    return result;
}

Error *newError(string format, ...)
{
    va_list argptr;
    va_start(argptr, format);
    char sval[1024 * 64];
    vsprintf(sval, format.c_str(), argptr);
    va_end(argptr);
    string msg(sval);
    Error *errorObj = new Error(msg);
    return errorObj;
}

bool isErrorObj(object *obj)
{
    if (obj != nullptr)
        return obj->getType() == objectType::error_obj;
    return false;
}

object *evalIdentifier(astNs::identifier *id, Environment *env)
{
    object *value = env->get(id->value);
    if (value == nullptr)
    {
        return newError("identifier not found: " + id->value);
    }
    return value;
}

vector<object *> evalExpressions(vector<astNs::expression *> args, Environment *env){
    vector<object*> result;
    for(astNs::expression *expr : args){
        object *val = eval(expr, env);
        if(isErrorObj(val)){
            return vector<object*> {val};
        }
        result.push_back(val);
    }
    return result;
}

object *applyFunction(object *function, vector<object*> args){
    if(function->getType() != objectType::function_obj){
        return newError("not a function: %s", function->getTypeString());
    }
    Function *fn = dynamic_cast<Function*>(function);
    Environment *extendedEnv = extendFunctionEnv(fn, args);
    object *evaluated = eval(fn->body, extendedEnv);
    return unwrapReturnValue(evaluated);
}

Environment *extendFunctionEnv( Function *fn, vector<object*> args){
    Environment *env = newEnclosedEnv(fn->env);
    for(int i=0; i< args.size() ; i++){
        env->set(fn->parameters[i]->String(), args[i]);
    }
    return env;
}

object *unwrapReturnValue(object* obj){
    if(obj->getType() == objectType::return_value_obj){
        ReturnValue *returnObj = dynamic_cast<ReturnValue *>(obj);
        return returnObj->value;
    }
    return obj;
}

object *evaluateArrayExpressions(astNs::arrayExpression *arrayExpr, Environment *env){
    vector<object*> items;
    for(astNs::expression *expr : arrayExpr->items){
        object *obj = eval(expr, env);
        items.push_back(obj);
    }
    return new Array(items);
}

object *evaluateArrayAccessExpression(astNs::arrayAccessExpr *arrayAccessExpr,Environment *env){
    object *obj = eval(arrayAccessExpr->arrayExpr, env);
    if(obj->getType() != array_obj){
        return newError("invalid array access operation. expected Array but got %s", obj->getTypeString());
    }
    Array *array = dynamic_cast<Array *>(obj);
    obj = eval(arrayAccessExpr->itemIndex, env);
    if(obj->getType() != integer_obj){
        return newError("invalid expression inside []. expected Integer but got %s", obj->getTypeString());
    }
    Integer *idx = dynamic_cast<Integer *>(obj);
    if(idx->value >= array->items.size()){
        return newError("array index out of bounds. max size = %d, received %d", array->items.size(), idx->value);
    }
    return array->items[idx->value];
}

object *evaluateHashLiteral(astNs::hashLiteral *hashLiteral, Environment *env){
    Hash *hash = new Hash();
    map<uint64_t, HashPair> map;
    std::map<astNs::expression*, astNs::expression*>::iterator it;
    for(it = hashLiteral->items.begin(); it!= hashLiteral->items.end(); it++){
        object *key = eval(it->first, env);
        object *val = eval(it->second, env);
        HashKey hkey = hashKey(key);
        HashPair hpair;
        hpair.key = key;
        hpair.value = val;
        map[hkey.value] = hpair;
    }
    hash->pairs = map;
    return hash;
}