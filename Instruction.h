#ifndef _INSTRUCTION_
#define _INSTRUCTION_
#include <string>
#include <iostream>

using namespace std;

class Instruction{

private:
    int address;
    string name;
    string printText;
    int rs;
    int rt;
    int rd;
    int imm;
    int sa;
    int jumpValue;
    friend std::ostream& operator<<(std::ostream&, const Instruction&);

    static int registers[32];
    static int dataArray[24];


public:
    //default
    Instruction();

    //for break instruction
    Instruction(string text);

    //R type and I type
    Instruction(string printText, int address, string name, int rs, int rt, int rd, int imm);

    //for SLL
    Instruction(string printText, int address, string name, int rs, int rt, int sa);

    //J type
    Instruction(string printText, int address, string name, int jumpValue);

    static void fillDataArray(int array[]);

    int execute();

    void print(int cycle, int address);

    string getName();

};

ostream& operator<<(ostream &strm, const Instruction &a){
    return strm << a.name << endl;
}



#endif