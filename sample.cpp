#include <iostream>
#include <unistd.h>
#include <fcntl.h>
#include <iomanip>
using namespace std;

int main()
{
        char buffer[4];
        int i;
        char * iPtr;
        iPtr = (char*)(void*) &i;

        int FD = open("test1.bin", O_RDONLY);

        int amt = 4;
        while( amt != 0 )
        {
                amt = read(FD, buffer, 4);
                if( amt == 4)
                {
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
                        cout << "Valid bit: " << to_string(validBit) <<endl;
                        cout << "Opcode: " << to_string(opcode) << endl;
                }
        }







}
