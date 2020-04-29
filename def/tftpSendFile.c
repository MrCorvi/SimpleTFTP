#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> 

#include"../headers/requestPackage.h"
#include"../headers/errorPackage.h"
#include"../headers/dataPackage.h"
#include"../headers/ackPackage.h"
#include"../headers/tftpSendFile.h"

#define BUF_LEN			30
#define DATA_SIZE		512

unsigned short getOpcode(char* buffer){
	return *(unsigned short*)buffer;
}

void tftpSendFile(int sd, struct sockaddr_in cl_addr, int clAddrLen, FILE* fPointer, char* mode){
	char buffer[1024], data[513];
	unsigned short i, j, nPack, rest, size;
	int ret;
	
	struct sockaddr_in tempClAddr;
	socklen_t tempClAddrLen;
	
	//Cerco la lunghezza del file
	fseek(fPointer, 0L, SEEK_END);
	size = ftell(fPointer);
	rewind(fPointer);
	nPack = size / DATA_SIZE;
	
	printf("\nDimensioni totale file: %u\nNumero pacchetti da inviare: %u \n", size, nPack + 1);

			
	for(i=0; i <= nPack; i++){
		if(i < nPack){
			if(!strcmp(mode, "netascii")){
				for(j=0; j<DATA_SIZE; j++){
					data[j] = fgetc(fPointer);
					if(data[j] == '\0')
						break;
				}
			}else{
				fread(data, DATA_SIZE, 1, fPointer);
			}
			data[DATA_SIZE] = '\0';
			makeDataPackage(buffer, i, data, 513);
		
			ret = sendto(sd, buffer, 4 + 513, 0, (struct sockaddr*)&cl_addr, clAddrLen);
			if (ret == -1){
				perror("Errore durate l'invio del packet DATA: ");
				exit(1);
			}
		}else{
			rest = size % DATA_SIZE;
			if(!strcmp(mode, "netascii")){
				for(j=0; j<rest; j++){
					data[j] = fgetc(fPointer);
					if(data[j] == '\0')
						break;
				}
			}else{
				fread(data, rest, 1, fPointer);
			}
			data[rest] = '\0';
			makeDataPackage(buffer, i, data, rest);
			sendto(sd, buffer, 517, 0, (struct sockaddr*)&cl_addr, clAddrLen);
			//sei il numero di byte e' un multiplo di 512, allora mando un pachetto aggiuntivo con solo il carattere \0
			if(rest == 0){
				strcpy(data, "\0");
				makeDataPackage(buffer, i, data, 1);
				sendto(sd, buffer, 517, 0, (struct sockaddr*)&cl_addr, clAddrLen);
			}
		}
		printf("Invio pacchetto %d\n\tdimensioni: %d\n", i, (int)strlen(data));
		
		//aspetto che arrivi l'ACK del paccketto mandato
		printf("\tAspetto l'ACK del client");
		do{
			tempClAddr = cl_addr;
			tempClAddrLen = (socklen_t)clAddrLen;
			ret = recvfrom(sd, buffer, BUF_LEN, 0, (struct sockaddr*)&tempClAddr, &tempClAddrLen);
			if (ret == -1){
				perror("Errore mentre aspettavo l'ack dal client: ");
				exit(1);
			}
			
			if(getOpcode(buffer) == 1){
				rrqQueue.index++;
				rrqQueue.addr[rrqQueue.index] = tempClAddr;
				rrqQueue.len[rrqQueue.index] = clAddrLen;
			}
		}while(getOpcode(buffer) != 4);
		printf("\r\tRicevuto l'ACK del pacchetto %d\n", i);
		printf("\tPacchetti trasferiti: %d su %d \n", i + 1, nPack + 1);
	}
	printf("Termine invio pachetti\n");
}











