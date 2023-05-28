//
// Created by rahultumpala on 28/4/23.
//

#include "../object/object.h"
#include "../ast/ast.h"

#ifndef MONKEYINTERPRETER_EVALUATOR_H
#define MONKEYINTERPRETER_EVALUATOR_H


object *null = new Null();
object *True = new Boolean(true);
object *False = new Boolean(false);

object *eval(astNs::astNode *node, Environment *env);

object *evalProgram(astNs::program *program, Environment *env);

object *evalBlockStatment(astNs::blockStatement * blockStatement, Environment *env);

object *evalPrefixExpression(std::string prefixOperator, object *right);

object *evalInfixExpression(std::string infixOperator, object *left, object *right);

object *evalBangOperatorExpression(object *right);

object *evalMinusPrefixOperatorExpression(object *right);

object *evalIntegerInfixExpression(std::string infixOperator, Integer *left, Integer *right);

object *evalStringInfixExpression(std::string infixOperator, String *left, String *right);

object *nativeBoolToBooleanObject(bool val);

object *evalIfExpression(astNs::ifExpression *ifExpression, Environment *env);

vector<object*> evalExpressions(vector<astNs::expression*> args, Environment *env);

object *applyFunction(object *function, vector<object*> args);

Error *newError(string format, ...);

bool isErrorObj(object* obj);

object *evalIdentifier(astNs::identifier *id, Environment *env);

Environment *extendFunctionEnv( Function *fn, vector<object*> args);

object *unwrapReturnValue(object* obj);

object *evaluateArrayExpressions(astNs::arrayExpression *arrayExpr, Environment *env);

object *evaluateArrayAccessExpression(astNs::arrayAccessExpr *arrayAccessExpr, Environment *env);

#endif //MONKEYINTERPRETER_EVALUATOR_H
