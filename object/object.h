//
// Created by rahultumpala on 26/4/23.
//

#include<string>

#ifndef MONKEYINTERPRETER_OBJECT_H
#define MONKEYINTERPRETER_OBJECT_H

enum objectType {
    integer_obj,
    boolean_obj,
    null_obj,
    return_value_obj,
    error_obj
};

class object {
public:
    virtual std::string inspect() = 0;

    virtual objectType getType() = 0;

    virtual std::string getTypeString() = 0;
};

class Integer : public object {
public:
    int value;

    Integer(int value) {
        this->value = value;
    }

    std::string inspect() override {
        return std::to_string(value);
    }

    objectType getType() override {
        return integer_obj;
    }

    std::string getTypeString() override {
        return "Integer";
    }

};

class Boolean : public object {
public:
    bool value;

    Boolean(bool val) {
        this->value = val;
    }

    std::string inspect() override {
        return value ? "true" : "false";
    }

    objectType getType() override {
        return boolean_obj;
    }

    std::string getTypeString() override {
        return "Boolean";
    }

};

class Null : public object {
public:
    std::string inspect() override {
        return "null";
    }

    objectType getType() override {
        return null_obj;
    }

    std::string getTypeString() override {
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

    std::string getTypeString() override {
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

    std::string getTypeString() override {
        return "Error";
    }
};

#endif //MONKEYINTERPRETER_OBJECT_H
