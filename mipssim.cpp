#include <unistd.h>
#include <fcntl.h>
#include <iomanip>
#include <vector>
#include <string>
#include <bitset>
#include <fstream>
#include <iostream>
#include "Instruction.cpp"

using namespace std;


int getIntValue(bitset<5> bits){
    return (int)bits.to_ulong();
}

int main(int argc, char** argv)
{
    char buffer[4];
    
        int i;
        char * iPtr;
        iPtr = (char*)(void*) &i;
        bitset<1> validBit;
        bitset<5> opcodeBits;
        bitset<5> rsBits;
        bitset<5> rtBits;
        bitset<5> rdBits;
        bitset<5> zeroBits;
        bitset<6> functionBits;
        int pc = 96;
        string text;
        string lines[200];
        int lineCount = 0;
        int dataArray[24];
        Instruction instructionArray[1000];
        string fname;
        
        if(argc > 1){
            string filename(argv[1]);
            fname = filename.substr(0, 4);

            int FD = open(argv[1], O_RDONLY);
            bool b = false;
            int amt = 4;
            while( amt != 0 )
            {
                    amt = read(FD, buffer, 4);
                    if( amt == 4)
                    {
                            Instruction instr;
                            iPtr[0] = buffer[3];
                            iPtr[1] = buffer[2];
                            iPtr[2] = buffer[1];
                            iPtr[3] = buffer[0];
                            bitset<32> bin = bitset<32>((unsigned int)i);
                            

                            for(int i = 0; i < 32; i++){
                                    if(i <= 5)
                                            functionBits[i] = bin[i];
                                    else if(i <= 10)
                                            zeroBits[i - 6] = bin[i]; 
                                    else if(i <= 15)
                                            rdBits[i - 11] = bin[i];
                                    else if(i <= 20)
                                            rtBits[i - 16] = bin[i];
                                    else if(i <= 25)
                                            rsBits[i - 21] = bin[i];
                                    else if(i <= 30)
                                            opcodeBits[i - 26] = bin[i];
                                    else if(i == 31)
                                            validBit[0] = bin[i];
                            }

                            int functionDec = (int)functionBits.to_ulong();
                            int opcodeDec = (int)opcodeBits.to_ulong() + 32;

                            if((int)validBit.to_ulong() == 1){
                                if(functionDec == 13 && opcodeDec == 32){
                                    b = true;
                                    text = "BREAK";
                                    instr = Instruction(text);
                                }
                                else{
                                    int rs, rt, rd, sa, imm = 0;
                                    int jumpValue = 0;
                                    rs = getIntValue(rsBits);
                                    rt = getIntValue(rtBits);
                                    rd = getIntValue(rdBits);
                                    sa = getIntValue(zeroBits);
                                    bitset<16> immBits;
                                    bitset<26> jumpBits;
                                    for(int i = 0; i < 16; i++){
                                        immBits[i] = bin[i];
                                    }
                                    imm = (signed short)immBits.to_ulong();

                                    for(int i = 0; i < 26; i++){
                                        jumpBits[i] = bin[i];
                                    }
                                    jumpValue = (int)jumpBits.to_ulong() * 4;
                                    
                                switch (opcodeDec)
                                {
                                    case 60:
                                    text = "MUL\tR" + to_string(rd) + ", R" + to_string(rs) + ", R" + to_string(rt);
                                    instr = Instruction(text, pc, "MUL", rs, rt, rd, 0);
                                    case 40:
                                    text = "ADDI\tR" + to_string(rt) + ", R" + to_string(rs) + ", #" + to_string(imm); 
                                    instr = Instruction(text, pc, "ADDI", rs, rt, rd, imm);
                                    break;

                                    case 43: 
                                    text = "SW\tR" + to_string(rt) + ", " + to_string(imm) + "(R" + to_string(rs) + ")"; 
                                    instr = Instruction(text, pc, "SW", rs, rt, rd, imm);
                                        break;

                                    case 35:
                                    text = "LW\tR" + to_string(rt) + ", " + to_string(imm) + "(R" + to_string(rs) + ")"; 
                                    instr = Instruction(text, pc, "LW", rs, rt, rd, imm);
                                        break;

                                    case 33: 
                                    text = "BLTZ\tR" + to_string(rs) + ", #" + to_string(imm * 4); 
                                    instr = Instruction(text, pc, "BLTZ", rs, rt, rd, imm * 4);
                                        break;

                                    case 32:
                                        if(functionDec == 32){
                                            text = "ADD\tR" + to_string(rd) + ", R" + to_string(rs) + ", R" + to_string(rt);
                                            instr = Instruction(text, pc, "ADD", rs, rt, rd, 0);
                                        }
                                        else if(functionDec == 34){
                                            text = "SUB\tR" + to_string(rd) + ", R" + to_string(rs) + ", R" + to_string(rt);
                                            instr = Instruction(text, pc, "SUB", rs, rt, rd, 0);
                                        }
                                        else if(functionDec == 0){
                                            text = "SLL\tR" + to_string(rd) + ", R" + to_string(rt) + ", #" + to_string(sa);
                                            instr = Instruction(text, pc, "SLL", rs, rt, sa);
                                        }
                                        else if(functionDec == 8)
                                        {
                                            text = "JR\tR" + to_string(rs);
                                            instr = Instruction(text, pc, "JR", rs, rt, rd, 0);
                                        }
                                        else if(functionDec == 2){
                                            text = "SRL\tR" + to_string(rd) + ", R" + to_string(rt) + ", #" + to_string(sa);
                                            instr = Instruction(text, pc, "SRL", rs, rt, sa);
                                        }
                                        else if(functionDec == 12){
                                            text = "MOVZ\tR" + to_string(rd) + ", R" + to_string(rs) + ", R" + to_string(rt);
                                            instr = Instruction(text, pc, "MOVZ", rs, rt, rd, 0);
                                        }
                                        break;

                                    case 34:
                                        text = "J\t#" + to_string(jumpValue);
                                        instr = Instruction(text, pc, "J", jumpValue);
                                        break;
                                
                                default:
                                    break;
                                }
                                instructionArray[pc] = instr;
                                }
                            } 
                            else {
                                text = "Invalid Instruction";
                            }
                            
                            lines[lineCount++] = validBit.to_string() + " " + opcodeBits.to_string() + " " + rsBits.to_string() + " " + rtBits.to_string() + " " + rdBits.to_string() + " " + zeroBits.to_string() + " " + functionBits.to_string() + "\t" + to_string(pc) + "\t\t" + text;
                            pc += 4;
                            if(b == true){
                                int value = 0;
                                int valueCount = 0;
                                while(amt != 0){
                                    amt = read(FD, buffer, 4);
                                    if(amt == 4){
                                        iPtr[0] = buffer[3];
                                        iPtr[1] = buffer[2];
                                        iPtr[2] = buffer[1];
                                        iPtr[3] = buffer[0];
                                        bin = bitset<32>((unsigned int)i);
                                        value = (int)bin.to_ulong();
                                        dataArray[valueCount++] = value;
                                        lines[lineCount++] = bin.to_string() + "\t\t" + to_string(pc) + "\t\t" + to_string(value);
                                        pc += 4;
                                    }
                                }
                                break;
                            }         
                    }
            }//end while

                    Instruction::fillDataArray(dataArray);
        
        
                    int address = 0;
                    int count = 1;
                        for (int i = 100; i < 300; i += 4)
                        {
                            try
                            {
                                if(instructionArray[i].getName() == "BREAK"){
                                    instructionArray[i].print(count, i);
                                    break;
                                }
                                if(instructionArray[i].getName() != "NOP"){
                                    address = instructionArray[i].execute();
                                    instructionArray[i].print(count, i);
                                    count++;
                                    if(address != 0){
                                        i = address;
                                        address = 0;
                                    }
                                }
                            }
                            catch(const std::exception& e)
                            {
                                std::cerr << e.what() << '\n';
                            }                       
                        }  
                    ofstream outputFile;
                    fname += "_dis.txt";
                    const char* openFile = fname.c_str();
                    outputFile.open(openFile);
                    for(string line : lines){
                        outputFile << line << endl;
                    }
                    outputFile.close();
        }
        else{
            cout << "You must pass in a binary file as an argument" << endl;
        }      
}//end main
