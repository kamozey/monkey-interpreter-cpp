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

object *eval(astNs::astNode *node);

object *evalProgram(astNs::program *program);

object *evalBlockStatment(astNs::blockStatement * blockStatement);

object *evalPrefixExpression(std::string prefixOperator, object *right);

object *evalInfixExpression(std::string infixOperator, object *left, object *right);

object *evalBangOperatorExpression(object *right);

object *evalMinusPrefixOperatorExpression(object *right);

object *evalIntegerInfixExpression(std::string infixOperator, Integer *left, Integer *right);

object *nativeBoolToBooleanObject(bool val);

object *evalIfExpression(astNs::ifExpression *ifExpression);

#endif //MONKEYINTERPRETER_EVALUATOR_H
