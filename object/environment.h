#ifndef MONKEY_INTERPRETER_ENVIRONMENT_H
#define MONKEY_INTERPRETER_ENVIRONMENT_H

#include <map>
#include <string>
#include "object.h"


class Environment
{
public:
    std::map<std::string, object *> store;

    Environment() {}

    object* get(std::string name){
        if(this->store.find(name) != this->store.end()){
            return (*this).store.at(name);
        };
        return nullptr;
    }

    object* set(std::string name, object* val){
        store[name] = val;
        return val;
    }    
};

#endif // MONKEY_INTERPRETER_ENVIRONMENT_H