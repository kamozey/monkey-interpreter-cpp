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
    string_obj,
    array_obj,
    hash_obj
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

class Array : public object
{
public:
    std::vector<object*> items;

    Array(std::vector<object*> &items)
    {
        this->items = items;
    }

    std::string inspect() override
    {
        std::string val = "[ ";
        int len = items.size();
        for(int i =0; i<len; ++i){
            val += items[i]->inspect();
            val += i == items.size() - 1 ? "" : " , ";
        }
        val += " ]";
        return val;
    }

    objectType getType() override
    {
        return array_obj;
    }

    std::string getTypeString() override
    {
        return "Array";
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
        str += "){\n\t";
        str += body->String();
        str += "}";
        return str;
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

struct HashKey {
    objectType type;
    uint64_t value;

    // taken from https://github.com/SRombauts/cpp-algorithms/blob/master/src/algo/hash.cpp
    static uint64_t hash(const char* apStr) {
        uint64_t hash = 14695981039346656037ULL; // 64 bit offset_basis = 14695981039346656037

        for (uint32_t idx = 0; apStr[idx] != 0; ++idx) {
            // 64 bit FNV_prime = 240 + 28 + 0xb3 = 1099511628211
            hash = 1099511628211ULL * (hash ^ static_cast<unsigned char>(apStr[idx]));
        }
        return hash;
    }
};

HashKey hashKey(object *obj){
    HashKey h;
    h.type = obj->getType();
    switch (obj->getType())
    {
        case boolean_obj:{
            Boolean *b = dynamic_cast<Boolean*>(obj);
            h.value = b->value ? 1 : 0;
            break;
        }

        case integer_obj:{
            Integer *i = dynamic_cast<Integer*>(obj);
            h.value = i->value;
            break;
        }

        case string_obj:{
            String *s = dynamic_cast<String*>(obj);
            h.value = HashKey::hash(s->value.c_str());
            break;
        }

        default:
            break;
    }
    return h;
}

struct HashPair{
    object *key;
    object *value;
};

class Hash : public object
{
public:
    std::map<uint64_t, HashPair> pairs;

    Hash(){}

    std::string inspect() override
    {
        string s;
        s += "{";
        int cnt = pairs.size();
        std::map<uint64_t, HashPair>::iterator it;
        for (it=pairs.begin(); it != pairs.end(); it++) {
            cnt--;
            s += it->second.key->inspect() + " : " + it->second.value->inspect();
            s += cnt == 0? "" : ",";
        }
        s += "}";
        return s;
    }

    objectType getType() override
    {
        return hash_obj;
    }

    std::string getTypeString() override
    {
        return "Hash";
    }
};

#endif // MONKEYINTERPRETER_OBJECT_H
