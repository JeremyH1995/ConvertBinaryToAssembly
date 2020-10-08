#include <unistd.h>
#include <fcntl.h>
#include <iomanip>
#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include "Instruction.cpp"

using namespace std;

int main(int argc, char** argv)
{
    char buffer[4];
    
        int i;
        char * iPtr;
        iPtr = (char*)(void*) &i;
        int pc = 96;
        string text;
        string lines[200];
        int lineCount = 0;
        int dataArray[24];
        Instruction instructionArray[1000];
        string fname;
        
        if(argc > 0){
            //string filename(argv[1]);
            //fname = filename.substr(0, 4);

            int FD = open("test1.bin", O_RDONLY);
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
                        string binary = to_string(i);
                        int validBit = (((unsigned int)i)>>31);
                        int opcode = (((unsigned int)i)>>26);
                        int rs =  ((((unsigned int)i)<<6)>>27);
                        int rt = ((((unsigned int)i)<<11)>>27);
                        int rd = ((((unsigned int)i)<<16)>>27);
                        int sa = ((((unsigned int)i)<<21)>>27);
                        int functioncode = ((((unsigned int)i)<<26)>>26);
                        int jumpVal = ((((unsigned int)i)<<6)>>26) * 4;
                        int imm = ((((unsigned int)i)<<16)>>16);
                        cout << to_string(validBit) <<endl;
                        cout << to_string(opcode) << endl;

                            if(validBit == 1){
                                if(functioncode == 13 && opcode == 32){
                                    b = true;
                                    text = "BREAK";
                                    instr = Instruction(text);
                                }
                                else{
                                    
                                switch (opcode)
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
                                        if(functioncode == 32){
                                            text = "ADD\tR" + to_string(rd) + ", R" + to_string(rs) + ", R" + to_string(rt);
                                            instr = Instruction(text, pc, "ADD", rs, rt, rd, 0);
                                        }
                                        else if(functioncode == 34){
                                            text = "SUB\tR" + to_string(rd) + ", R" + to_string(rs) + ", R" + to_string(rt);
                                            instr = Instruction(text, pc, "SUB", rs, rt, rd, 0);
                                        }
                                        else if(functioncode == 0){
                                            text = "SLL\tR" + to_string(rd) + ", R" + to_string(rt) + ", #" + to_string(sa);
                                            instr = Instruction(text, pc, "SLL", rs, rt, sa);
                                        }
                                        else if(functioncode == 8)
                                        {
                                            text = "JR\tR" + to_string(rs);
                                            instr = Instruction(text, pc, "JR", rs, rt, rd, 0);
                                        }
                                        else if(functioncode == 2){
                                            text = "SRL\tR" + to_string(rd) + ", R" + to_string(rt) + ", #" + to_string(sa);
                                            instr = Instruction(text, pc, "SRL", rs, rt, sa);
                                        }
                                        else if(functioncode == 12){
                                            text = "MOVZ\tR" + to_string(rd) + ", R" + to_string(rs) + ", R" + to_string(rt);
                                            instr = Instruction(text, pc, "MOVZ", rs, rt, rd, 0);
                                        }
                                        break;

                                    case 34:
                                        text = "J\t#" + to_string(jumpVal);
                                        instr = Instruction(text, pc, "J", jumpVal);
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
                            
                            lines[lineCount++] = binary + "\t" + to_string(pc) + "\t" + text;
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
                                        value = (int)i;
                                        dataArray[valueCount++] = value;
                                        lines[lineCount++] = binary + "\t" + to_string(pc) + "\t" + to_string((signed int)i);
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
                    //fname += "_dis.txt";
                    //const char* openFile = fname.c_str();
                    outputFile.open("test1_dis.txt");
                    for(string line : lines){
                        outputFile << line << endl;
                    }
                    outputFile.close();
        }
        else{
            cout << "You must pass in a binary file as an argument" << endl;
        }      
}//end main
