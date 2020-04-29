#include <string.h>
#include <stdio.h>

#include"../headers/requestPackage.h"

struct request decodeRequestPackage(char* buffer){
	int endFileName;
	struct request rrq;
	
	//estraggo dai campi del pacchetto		
	endFileName = strlen(buffer + 2);
	strcpy(rrq.filename, buffer + 2);
	
	strcpy(rrq.mode, buffer + 2 + endFileName + 1);
	
	printf("Arrivata richiesta RRQ : %s - %s\n", rrq.filename, rrq.mode);
	
	return rrq;
}

//mode: netascii => modalita Text, octet => modalita Binaria 
void makeRequestPackage(char* filename, char* buffer, char* mode){
	int len;
	unsigned short opcode = 1;
	
	//Imposto i campi del package RRQ
	buffer[0] = (char) opcode & 0x00FF;
	buffer[1] = (char) opcode >> 8;
	strcpy(buffer + 2, filename);
	len = strlen(filename);
	if(!strcmp(mode, "txt"))
		strcpy(buffer + 2 + len + 1, "netascii");
	else
		strcpy(buffer + 2 + len + 1, "octet");
		
	//printf("Messaggio %d :%s\n", strlen(buffer), buffer + 2);
}
