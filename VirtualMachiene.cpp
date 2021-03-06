#include "pch.h"
#include <iostream>
#include <fstream>
#include <bitset>
using namespace std;

class VirtualMachine
{
	int skaitliukas;
	int decryptor[256];
	int counter;
	int R[16];

public:

	void Nuskaitymas()
	{
		unsigned char reader;
		counter = 0;
		ifstream duomenys("decryptor.bin", ios::binary);
		while (reader = duomenys.get(), duomenys.good()) {decryptor[counter] = (int)reader;  counter++; }
		duomenys.close();
	}

	void Machiene()
	{
		ifstream encr("q1_encr.txt");
		char symbol;
		int skaitliukas = 0;
		//unsigned char x, y;
		int x, y;
		bool pabaiga = false;

		bool flagEND = false;
		bool flagZERO = false;
		bool flagJUMP = false;

		while (!pabaiga)
		{
			x = 0x0f & decryptor[skaitliukas + 1];
			y = (0xf0 & decryptor[skaitliukas + 1]) >> 4;

			switch (decryptor[skaitliukas])
			{
			//INC
				case 0x01: R[x] += 1; break;
			//DEC
				case 0x02: R[x] -= 1; break;
			//MOV
				case 0x03: R[x] = R[y]; break;
			//MOVC "baito konstanta"
				case 0x04:R[0] = decryptor[skaitliukas + 1];
					break;
			//LSL Rx
				case 0x05: R[x] = R[x] << 1; break;
			//LSR Rx
				case 0x06: R[x] = R[x] >> 1; break;
			//JMP addr
				case 0x07: 
						skaitliukas += decryptor[skaitliukas + 1];
						while (skaitliukas >= counter) skaitliukas -= counter;
						flagJUMP = true;
						break;
			//JZ addr
				case 0x08: 
					if (flagZERO == true) 
					{
						skaitliukas += decryptor[skaitliukas + 1];
						while (skaitliukas >= counter) skaitliukas -= counter;
						flagJUMP = true;
					}
					break; 
			//JNZ addr
				case 0x09: 
					if (flagZERO == false)
						{
							skaitliukas += decryptor[skaitliukas + 1];
							while (skaitliukas >= counter) skaitliukas -= counter;
							flagJUMP = true;
						}
					break;
			//JFE
				case 0x0a: 
					if (flagEND == true) 
					{
						skaitliukas += decryptor[skaitliukas + 1];
						while (skaitliukas >= counter) skaitliukas -= counter;
						flagJUMP = true;
					}
					break;
			//RET
				case 0x0b: pabaiga = true; break;
			//ADD Rx, Ry
				case 0x0c: R[x] = R[x]+R[y]; break;
			//SUB Rx, Ry
				case 0x0d: R[x] = R[x]-R[y]; break;
			//XOR Rx, Ry
				case 0x0e: R[x] = R[x]^R[y]; break;
			//OR Reg1, Reg2
				case 0x0f: R[x] = R[x]|R[y]; break;
			//IN Rx
				case 0x10: 
					if (!encr.eof()) {
						encr >> symbol;
						R[x] = (int)symbol;
					}
					else flagEND = true; break;
			//OUT Rx
				case 0x11: cout << (char)R[x]; 
					break;
			}
			
			if (flagJUMP == false) 
			{
				skaitliukas += 2;
			}
			else flagJUMP = false;
			
		}
		encr.close();
	}
};

int main()
{
	VirtualMachine go;
	go.Nuskaitymas();
	go.Machiene();
}