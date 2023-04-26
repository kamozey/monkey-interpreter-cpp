//
// Created by rahultumpala on 26/4/23.
//

#include<string>

#ifndef MONKEYINTERPRETER_OBJECT_H
#define MONKEYINTERPRETER_OBJECT_H

enum objectType {
    integer_obj
};

class object {
public:
    virtual std::string inspect() = 0;

    virtual objectType getType() = 0;
};

class integer : public object {
public:
    int value;

    std::string inspect() override {
        return std::to_string(value);
    }

    objectType getType() override {
        return integer_obj;
    }

};

#endif //MONKEYINTERPRETER_OBJECT_H
