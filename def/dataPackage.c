#include <netinet/in.h>
#include <string.h>
#include <stdio.h>

#include"../headers/dataPackage.h"

//Mi alloca in una struttura apposita DataPack il contenuto del campo di tipo DATA
struct DataPack decodeDataPackage(char* buffer){
	struct DataPack dataP;
	
	//estraggo dai campi del pacchetto	
	dataP.blockNum = *(unsigned short*)(buffer + 2);
	
	strcpy(dataP.data, buffer + 4);
	
	return dataP;
}

// formatta un buffer in modo tale che contenga un pacchetto di tipo DATA  
void makeDataPackage(char* buffer, unsigned short seq, char* data, int nData){
	unsigned short opcode = 3;
	
	//Imposto i campi del package DATA
	buffer[0] = (char) opcode & 0x00FF;
	buffer[1] = (char) opcode >> 8;
	buffer[2] = (char) seq & 0x00FF;
	buffer[3] = (char) seq >> 8;
	strncpy(buffer + 4, data, nData);
	buffer[4 + nData] = '\0';
}


