//
// Created by rahultumpala on 26/4/23.
//

#ifndef MONKEYINTERPRETER_OBJECT_H
#define MONKEYINTERPRETER_OBJECT_H

#include <string>
#include <vector>
#include <map>
#include "../ast/ast.h"

enum objectType
{
    integer_obj,
    boolean_obj,
    null_obj,
    return_value_obj,
    error_obj,
    function_obj,
    string_obj
};

class object
{
public:
    virtual std::string inspect() = 0;

    virtual objectType getType() = 0;

    virtual std::string getTypeString() = 0;
};

class Integer : public object
{
public:
    int value;

    Integer(int value)
    {
        this->value = value;
    }

    std::string inspect() override
    {
        return std::to_string(value);
    }

    objectType getType() override
    {
        return integer_obj;
    }

    std::string getTypeString() override
    {
        return "Integer";
    }
};

class String : public object
{
public:
    string value;

    String(string value)
    {
        this->value = value;
    }

    std::string inspect() override
    {
        return value;
    }

    objectType getType() override
    {
        return string_obj;
    }

    std::string getTypeString() override
    {
        return "String";
    }
};

class Boolean : public object
{
public:
    bool value;

    Boolean(bool val)
    {
        this->value = val;
    }

    std::string inspect() override
    {
        return value ? "true" : "false";
    }

    objectType getType() override
    {
        return boolean_obj;
    }

    std::string getTypeString() override
    {
        return "Boolean";
    }
};

class Null : public object
{
public:
    std::string inspect() override
    {
        return "null";
    }

    objectType getType() override
    {
        return null_obj;
    }

    std::string getTypeString() override
    {
        return "Null";
    }
};

class ReturnValue : public object
{
public:
    object *value;

    ReturnValue(object *val)
    {
        this->value = val;
    }

    std::string inspect() override
    {
        return value->inspect();
    }

    objectType getType() override
    {
        return return_value_obj;
    }

    std::string getTypeString() override
    {
        return "Return";
    }
};

class Error : public object
{
public:
    std::string message;

    Error(std::string message)
    {
        this->message = message;
    }

    std::string inspect() override
    {
        return message;
    }

    objectType getType() override
    {
        return error_obj;
    }

    std::string getTypeString() override
    {
        return "Error";
    }
};

class Environment
{
public:
    std::map<std::string, object *> store;
    Environment *outer;

    Environment() {}

    object *get(std::string name)
    {
        if (this->store.find(name) != this->store.end())
        {
            return (*this).store.at(name);
        }
        else if (this->outer != nullptr && this->outer->store.find(name) != this->outer->store.end())
        {
            return (*this->outer).store.at(name);
        }
        return nullptr;
    }

    object *set(std::string name, object *val)
    {
        store[name] = val;
        return val;
    }
};

Environment *newEnclosedEnv(Environment *outer)
{
    Environment *env = new Environment();
    env->outer = outer;
    return env;
}

class Function : public object
{
public:
    vector<astNs::expression *> parameters;
    astNs::blockStatement *body;
    Environment *env;

    Function() {}

    std::string inspect() override
    {
        std::string str = "fn (";
        for (int i = 0; i < parameters.size(); i++)
        {
            astNs::expression *expr = parameters[i];
            str += expr->String();
            str += i == parameters.size() - 1 ? "" : ",";
        }
    }

    objectType getType() override
    {
        return function_obj;
    }

    std::string getTypeString() override
    {
        return "Function";
    }
};

#endif // MONKEYINTERPRETER_OBJECT_H
