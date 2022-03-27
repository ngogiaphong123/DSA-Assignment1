#ifndef SYMBOLTABLE_H
#define SYMBOLTABLE_H
#include "main.h"
class SymbolTable
{
public:
    SymbolTable() {}
    void run(string filename);
};
struct data {
    data(){
        this->name="";
        this->type="";
        this->value="";
        this->level=-1;
        this->seen=false;
    }
    data(string name, string type, string value, int level, bool seen) {
        this->name = name;
        this->type = type;
        this->value = value;
        this->level = level;
        this->seen = seen;
    }
    string name;
    string type;
    string value;
    int level;
    bool seen;
};
struct arrOfIndex {
    int* begin;
    int sizeI;
    int capacityI;
    arrOfIndex() {
        this->capacityI=10;
        this->sizeI=0;
        this->begin=new int[this->capacityI];
    }
    void ensure(int capacity);
    void push(int index);
    int top();
    void pop();
    ~arrOfIndex() {
        delete[] this->begin;
        sizeI=0;
        capacityI=0;
    }
};
struct arrOfSymbolTable {
    data* arr;
    int size;
    int capacity;
    arrOfSymbolTable() {
        this->capacity = 10;
        this->arr = new data[this->capacity];
        this->size =0;
    }
    ~arrOfSymbolTable() {
        delete[] arr;
        size=0;
        capacity=0;
    }
    void ensureCapacity(int capacity);
    void changeState();
    bool add(string name , string type,int currLevel);
    bool search(string name,int level);
    bool assignNumber(string value,int index,string inputType);
    bool assignString(string value,int index, string inputType);
    bool assignIdentifier(string identifier, int index,int currLevel);
    int searchToAssign(string name,int currLevel);
    void changeFalseIfInsert(data input);
    void changeTrueIfEnd(data input,int currLevel,int index);
    bool checkIndex(int index);
};
#endif