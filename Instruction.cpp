#include "Instruction.h"
#include <string>
#include <iostream>

using namespace std;

    //static arrays
    int Instruction::dataArray[24] = {0};
    int Instruction::registers[32] = {0};

    //default
    Instruction::Instruction(){
         this->printText = "NOP";
        this->address = 0;
        this->name = "NOP";
        this->rs = 0;
        this->rt = 0;
        this->rd = 0;
        this->imm = 0;
    }

    //for Break instruction
    Instruction::Instruction(string text){
        this->printText = "BREAK";
        this->address = 0;
        this->name = "BREAK";
        this->rs = 0;
        this->rt = 0;
        this->rd = 0;
        this->imm = 0;
    }

    //R type and I type
    Instruction::Instruction(string printText, int address, string name, int rs, int rt, int rd, int imm){
        this->printText = printText;
        this->address = address;
        this->name = name;
        this->rs = rs;
        this->rt = rt;
        this->rd = rd;
        this->imm = imm;
    }

    //for SLL
    Instruction::Instruction(string printText, int address, string name, int rs, int rt, int sa){
        this->printText = printText;
        this->address = address;
        this->name = name;
        this->rs = rs;
        this->rt = rt;
        this->rd = rd;
        this->imm = imm;
    }

    //J type
    Instruction::Instruction(string printText, int address, string name, int jumpValue){
        this->printText = printText;
        this->address = address;
        this->name = name;
        this->jumpValue = jumpValue;
    }

    //static
    void Instruction::fillDataArray(int array[]){
        for (int i = 0; i < 24; i++)
        {
            dataArray[i] = array[i];
        }
        
    }

    string Instruction::getName(){
        return name;
    }

    int Instruction::execute(){
        if(name == "J"){
            return jumpValue;
        }
        else if(name == "JR"){
            return registers[rs];
        }
        else if(name == "BEQ"){
            if(registers[rs] == registers[rt]){
                return address + imm;
            }
            else{
                return 0;
            }
        }
        else if(name == "BLTZ"){
            if(registers[rs] < 0){
                return address + imm;
            }
            else{
                return 0;
            }
        }
        else if(name == "ADD"){
            registers[rd] = registers[rs] + registers[rt];
            return 0;
        }
        else if(name == "ADDI"){
            registers[rt] = registers[rs] + imm;
            return 0;
        }
        else if(name == "SUB"){
            registers[rd] = registers[rs] - registers[rt];
            return 0;
        }
        else if(name == "SW"){
            dataArray[rs + imm] = registers[rt];
            return 0;
        }
        else if(name == "LW"){
            registers[rt] = dataArray[rs + imm];
            return 0;
        }
        else if(name == "SLL"){
            registers[rd] = registers[rt] * 2^imm;
            return 0;
        }
        else if(name == "SRL"){
            registers[rd] = registers[rt] / 2^imm;
            return 0;
        }
        else if(name == "MUL"){
            registers[rd] = registers[rs] * registers[rt];
            return 0;
        }
        else if(name == "AND"){
            if(registers[rs] == 1 && registers[rt] == 1){
                registers[rd] = 1;
            }
            else{
                registers[rd] = 0;
            }
            return 0;
        }
        else if(name == "OR"){
             if(registers[rs] == 1 || registers[rt] == 1){
                registers[rd] = 1;
            }
            else{
                registers[rd] = 0;
            }
            return 0;
        }
        else if(name == "MOVZ"){
            if(registers[rt] == 0){
                registers[rd] = registers[rs];
            }
            return 0;
        }
        else if(name == "NOP"){
            return 0;
        }
    }

    void Instruction::print(int cycle, int a){
        cout << "====================" << endl;
        cout << "cycle:" + to_string(cycle) + "\t" << to_string(a) + "\t" + printText << endl << endl;

        //print registers
        cout << "registers:" << endl;
        cout << "r00:\t";
        for(int i = 0; i < 32; i++){
           if(i == 8 || i == 16 || i == 24){
               cout << endl;
               if(i+1 < 10){
                   cout << "r0" + to_string(i) + ":\t";
               }
               else{
                   cout << "r" + to_string(i) + ":\t";
               }
           }
           cout << to_string(registers[i]) + "\t";
        }

        //print data values
        int value = 172;
        cout << endl << endl << "data:" << endl << to_string(value) + ":\t";
        for(int i = 0; i < 24; i++){
            if((i % 8) == 0 && i != 0){
                value += 32;
                cout << endl << to_string(value) + ":\t";
            }
            cout << to_string(dataArray[i]) + "\t";
        }
        cout << endl;
    }
    

    
