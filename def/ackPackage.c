#include <string.h>
#include <stdio.h>

#include"../headers/ackPackage.h"

void makeAckPackage(char* buffer, unsigned short blockNum){
	unsigned short opcode = 4;
	
	//Imposto i campi del package ACK
	buffer[0] = (char) opcode & 0x00FF;
	buffer[1] = (char) opcode >> 8;
	buffer[2] = (char) blockNum & 0x00FF;
	buffer[3] = (char) blockNum >> 8;
}
