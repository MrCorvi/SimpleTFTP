#include <netinet/in.h>
#include <string.h>
#include <stdio.h>


#include"../headers/errorPackage.h"

//Stampa l'errore che e' stato mandato tramite al interno del pacchetto ERROR
void printError(char* buffer){
	unsigned short errorCode;
	char errorMsg[60];
	
	errorCode = *(unsigned short*)(buffer + 2);
	strcpy(errorMsg, buffer + 4);
	
	printf("Errore di trasmissione %u: %s\n\n", errorCode, errorMsg);
}

// formatta un buffer in modo tale che contenga un pacchetto di tipo ERROR  
// Codici errore: 
//		0) file non trovato
//		1) illigal TFTP operation, messaggio di richiesta errato
void  sendErrorPacket(int sd, unsigned short errorCode, char* errorMsg, struct sockaddr_in cl_addr, int clAddrLen){
	char buffer[1024];
	unsigned short opcode = 5;

	buffer[0] = (char) opcode & 0x00FF;
	buffer[1] = (char) opcode >> 8;
	buffer[2] = (char) errorCode & 0x00FF;
	buffer[3] = (char) errorCode >> 8;
	
	strcpy(buffer + 4, errorMsg);
	
	sendto(sd, buffer, 4 + strlen(errorMsg), 0, (struct sockaddr*)&cl_addr, clAddrLen);
	
	printf("Errore di trasmissione %u: %s \n\n", errorCode, errorMsg);
}

