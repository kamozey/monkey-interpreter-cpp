//
// Created by rahultumpala on 26/4/23.
//

#include<string>

#ifndef MONKEYINTERPRETER_OBJECT_H
#define MONKEYINTERPRETER_OBJECT_H

enum objectType {
    integer_obj,
    boolean_obj,
    null_obj
};

class object {
public:
    virtual std::string inspect() = 0;

    virtual objectType getType() = 0;
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

};

class Null : public object {
public:
    std::string inspect() override {
        return "null";
    }

    objectType getType() override {
        return null_obj;
    }

};

#endif //MONKEYINTERPRETER_OBJECT_H
