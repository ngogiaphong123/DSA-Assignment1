#include "SymbolTable.h"
int numberOfWords(string input) {
    int count = 0;
    int n = input.length();
    for(int i = 0 ; i < n ; i++) {
        if(input[i] ==' ') count++;
    }
    return count+1;
}
string* tokenize(string input, string del = " ") {
    int numberOfWord = numberOfWords(input);
    int i=0;
    string *cmd = new string[numberOfWord];
    int start = 0;
    int end = (int)input.find(del);
    while (end != -1) {
        cmd[i] = input.substr(start, end - start);
        start = (int)end + (int)del.size();
        end = (int)input.find(del, start);
        i++;
    }
    cmd[i] = input.substr(start, end - start);
    if(numberOfWord > 3) {
        for(int i = 3; i < numberOfWord; i++) {
            cmd[2] = cmd[2]+" "+cmd[i];
        }
        numberOfWord=3;
    }
    return cmd;
}
// -----------------INSERT----------------------------
bool arrOfSymbolTable :: checkIndex(int index) {
    if(index >= 0 && index < this->size) return true;
    return false;
}
bool checkCmd2(string value) {
    if(value == "number" || value == "string") return true;
    return false;
}
void arrOfSymbolTable :: ensureCapacity(int capacity) {
    if (capacity > this->capacity) {
        int newCapacity = this->capacity*3/2;
        data *newData = new data[newCapacity];
        for(int i = 0 ; i < this->capacity ; i++) {
            newData[i] = this->arr[i];
        }
        this->capacity = newCapacity;
        delete[] this->arr;
        this->arr = newData;
    }
}
bool arrOfSymbolTable :: search(string name,int level) {
    for(int i = 0 ; i < this->size ; i++) {
        if(arr[i].name == name && arr[i].level == level) {
            return true;
        }
    }
    return false;
}
// change seen state of the one who have a same name with input but diffrent level.
void arrOfSymbolTable :: changeFalseIfInsert(data input) {
    for(int i = 0 ; i < this->size ; i++) {
        if(arr[i].name == input.name && arr[i].level < input.level && arr[i].level != -1) {
            arr[i].seen=false;
        }
    }
}
void arrOfSymbolTable :: changeTrueIfEnd(data input,int currLevel,int index) {
    bool first = true;
    for(int i = index ; i >= 0 ; i--) {
        if(first == true) {
            if(arr[i].name == input.name && arr[i].level <= currLevel && arr[i].level != -1) {
                first=false;
                arr[i].seen=true;
            }
        }
    }
}
bool arrOfSymbolTable :: add(string name, string type,int currLevel) {
    data newNode(name,type,"",currLevel,true);
    if(search(newNode.name,newNode.level) == true) 
        return false;
    this->ensureCapacity(this->size+1);
    this->size++;
    this->arr[this->size-1] = newNode;
    changeFalseIfInsert(newNode);
    return true;
}
// -----------------ASSIGN---------------------------
// Search name and visibility
int arrOfSymbolTable ::searchToAssign(string name,int currLevel) {
    for(int i = 0 ; i < this->size ; i++) {
        if(arr[i].name == name && arr[i].level <= currLevel && arr[i].seen==true && arr[i].level != -1) {
            return i;
        }
    }
    return -1;
}
// -----------------CheckConditionToAssign------------------
bool checkIdentifierName(string name) {
    if(!(name[0] >= 'a' && name[0] <= 'z')) return false;
    int n = name.length();
    for(int i = 1 ; i < n ; i++) {
        if(!((name[i] >= 'A' && name[i] <= 'Z') || (name[i]>='a' && name[i]<= 'z') || (name[i]=='_') || (name[i] >= '0' && name[i] <= '9'))) 
            return false;
    }
    return true;
}
bool checkValueString(string value) {
    int n = value.length();
    if(value[0] != 39) return false;
    if(value[n-1] != 39) return false;
    for(int i = 1 ; i < n-1 ; i++) {
        if (!((value[i] >= 'a' && value[i] <= 'z') || (value[i] >= 'A' && value[i] <= 'Z') || (value[i] >= '0' && value[i] <= '9') || value[i] == 32))
            return false;
    }
    return true;
}
bool checkValueNumber(string value) {
    int n = value.length();
    for(int i = 0 ; i < n ; i++) {
        if(!(value[i] >= '0' && value[i] <= '9')) return false;
    }
    return true;
}
bool checkValueIdentifier(string value) {
    int n = value.length();
    if(value[0]!=39 && value[n-1]!=39 && (numberOfWords(value) == 1)) return true;
    return false;
}
bool arrOfSymbolTable :: assignNumber(string value,int index,string inputType) {
    if(arr[index].type != inputType) return false;
    arr[index].value = value;
    return true;
}
bool arrOfSymbolTable :: assignString(string value,int index,string inputType) {
    if(arr[index].type != inputType) return false;
    arr[index].value = value;
    return true;
}
bool arrOfSymbolTable :: assignIdentifier(string identifier, int index,int currLevel) {
    string value = "";
    int i = searchToAssign(identifier,currLevel);
    if(checkIndex(i) == false) return false;
    if(arr[i].type != arr[index].type) return false;
    arr[index].value = arr[i].value;
    return true;
}
//---------------------BEGIN/END----------------------------------
void arrOfIndex :: push(int index) {
    this->ensure(this->sizeI+1);
    this->sizeI++;
    begin[this->sizeI-1] = index;
}
int arrOfIndex :: top() {
    return begin[this->sizeI-1];
}
void arrOfIndex :: pop() {
    this->sizeI--;
}
void arrOfIndex ::ensure(int capacity) {
    if(capacity > this->capacityI) {
        int newCapacity = this->capacityI *3/2;
        int* data= new int[newCapacity];
        for(int i = 0 ; i < this->capacityI; i++) {
            data[i]= this->begin[i];
        }
        this->capacityI = newCapacity;
        delete [] this->begin;
        this->begin = data;
    }
}
void SymbolTable::run(string filename)
{
    int currLevel=0;
    arrOfSymbolTable arr;
    arrOfIndex begin;
    string tmp="";
    ifstream file(filename);
    int endIndex = 0;
    while(!file.eof()) {
        getline(file, tmp);
        // int n = numberOfWords(tmp);
        // string* cmd = new string[n];
        // cmd = tokenize(tmp);
        if(tmp == "") {
            throw InvalidInstruction("");
        }
        string* cmd = tokenize(tmp);
        if(cmd[0] == "INSERT") {
            if(numberOfWords(tmp) != 3) {
                delete [] cmd;
                throw InvalidInstruction(tmp);
            }
            if(checkIdentifierName(cmd[1])==false) {
                delete [] cmd;
                throw InvalidInstruction(tmp);
            }
            if(checkCmd2(cmd[2])==false) {
                delete [] cmd;
                throw InvalidInstruction(tmp);
            }
            if(arr.add(cmd[1],cmd[2],currLevel)==true) cout << "success" << endl;
            else {
                delete [] cmd;
                throw Redeclared(tmp);
            }
        }
        else if(cmd[0] == "ASSIGN") {
            if(numberOfWords(tmp) < 3) {
                delete [] cmd;
                throw InvalidInstruction(tmp);
            }
            int index = arr.searchToAssign(cmd[1],currLevel);
            if(checkIdentifierName(cmd[1])==false) {
                delete [] cmd;
                throw InvalidInstruction(tmp);
            }
            if (arr.checkIndex(index)==false) {
                delete [] cmd;
                throw Undeclared(tmp);
            }
            if(checkValueNumber(cmd[2])==true) {
                string inputType ="number";
                if(arr.assignNumber(cmd[2],index,inputType)==true) cout << "success" << endl;
                else {
                    delete [] cmd;
                    throw TypeMismatch(tmp);
                }
            }
            else if(checkValueString(cmd[2])==true) {
                string inputType ="string";
                if(arr.assignNumber(cmd[2],index,inputType)==true) cout << "success" << endl;
                else {
                    delete [] cmd;
                    throw TypeMismatch(tmp);
                }
            }
            // ASSIGN identifier
            else {
                if(checkValueIdentifier(cmd[2]) == true) {
                    if(checkIdentifierName(cmd[2])==false)  {
                        delete [] cmd;
                        throw InvalidInstruction(tmp);
                    }
                    int i = arr.searchToAssign(cmd[2],currLevel);
                    if(arr.checkIndex(i)==false) {
                        delete [] cmd;
                        throw Undeclared(tmp);
                    }
                    if(arr.assignIdentifier(cmd[2],index,currLevel)==true) cout << "success" << endl;
                    else {
                        delete [] cmd;
                        throw TypeMismatch(tmp);
                    }
                }
                else {
                    delete [] cmd;
                    throw InvalidInstruction(tmp);
                }
            }
        }
        else if(cmd[0] == "BEGIN") {
            if(numberOfWords(tmp) != 1) {
                delete [] cmd;
                throw InvalidInstruction(tmp);
            }
            begin.push(arr.size);
            currLevel=currLevel+1;
        }
        else if(cmd[0] == "END") {
            if(numberOfWords(tmp) != 1) {
                delete [] cmd;
                throw InvalidInstruction(tmp);
            }
            endIndex =arr.size-1;
            currLevel=currLevel-1;
            if(currLevel < 0) {
                delete [] cmd;
                throw UnknownBlock();
            }
            int index = begin.top();
            for(int i = begin.top() ; i <= endIndex ; i++) {
                arr.changeTrueIfEnd(arr.arr[i],currLevel,index);
                arr.arr[i].seen=false;
                arr.arr[i].level=-1;
            }
            begin.pop();
        }
        else if(cmd[0] == "LOOKUP") {
            if(numberOfWords(tmp) != 2) {
                delete [] cmd;
                throw InvalidInstruction(tmp);
            }
            if(checkIdentifierName(cmd[1])==false) {
                delete [] cmd;
                throw InvalidInstruction(tmp);
            }
            int index = arr.searchToAssign(cmd[1],currLevel);
            if(arr.checkIndex(index)==false) {
                delete [] cmd;
                throw Undeclared(tmp);
            }
            cout << arr.arr[index].level;
            cout << endl;
        }
        else if(cmd[0] == "PRINT") {
            if(numberOfWords(tmp) != 1) {
                delete [] cmd;
                throw InvalidInstruction(tmp);
            }
            if(arr.size==0) cout <<"";
            else {
                bool flag = true;
                int count = 0;
                for(int i = 0 ; i < arr.size ; i++) {
                    if (arr.arr[i].seen==true&&arr.arr[i].level != -1) {
                        if(flag==true) {
                            count ++;
                            cout << arr.arr[i].name << "//" << arr.arr[i].level;
                            flag = false;
                        }
                        else cout << " " << arr.arr[i].name << "//" << arr.arr[i].level;
                    }
                }
                if(count == 1) cout << endl;
            }
        }
        else if(cmd[0] == "RPRINT") {
            if(numberOfWords(tmp) != 1) {
                delete [] cmd;
                throw InvalidInstruction(tmp);
            }
            if(arr.size==0) cout <<"";
            else {
                bool flag = true;
                int count = 0;
                for(int i = arr.size-1 ; i >= 0 ; i--) {
                    if (arr.arr[i].seen==true&&arr.arr[i].level != -1) {
                        if(flag==true) {
                            count++;
                            cout << arr.arr[i].name << "//" << arr.arr[i].level;
                            flag = false;
                        }
                        else cout << " " << arr.arr[i].name << "//" << arr.arr[i].level;
                    }
                }
                if(count == 1) cout << endl;
            }
        }
        else {
            delete[] cmd;
            throw InvalidInstruction(tmp);
        }
        delete[] cmd;
    }
    if(currLevel > 0) throw UnclosedBlock(currLevel);
}