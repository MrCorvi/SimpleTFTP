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
#include"../headers/tftpRecvFile.h"

#define TOT_LEN 		1024 
#define BUF_LEN			69

unsigned short getOpcode(char* buffer){
	return *(unsigned short*)buffer;
}

//Riceve i pacchetti DATA e gestisce
//		1) la creazione del file
//		2) eventuali errori di trasmissione
//		3) l'invio dei ACK
void RecvFile(int sd, char* destFile, char* mode){
	FILE* fPointer;
	char buffer[1024];
	struct DataPack dataP;
	int ret;
	
	dataP.blockNum = 0;
	
	//cancello il contenuto del file, e se non esiste lo creo
	fPointer = fopen(destFile, "w");
	fclose(fPointer);
	
	//A seconda della modalita richiesta, appendo o in modalita Testo o in modalita Binaria
	if(!strcmp(mode, "netascii"))
		fPointer = fopen(destFile, "a");
	else
		//Invio in modalita Binaria
		fPointer = fopen(destFile, "ab");
	
	printf("\ninizio download del file dal server\n");
	
	do{
		printf("\tAttesa pacchetto %d", dataP.blockNum);
		//Aspetto che arrivi il pacchetto DATA, estraggo le informazioni e le inserisco del mio file 
		recv(sd, buffer, 517, 0);
		if (ret == -1){
			perror("Errore durante la ricezione del pacchetto dal server: ");
			exit(1);
		}
		
		//Verifico se ci sono stati errori nella trasmissione	
		if(getOpcode(buffer) == 5){
			printError(buffer);
			break;
		}	
		
		dataP = decodeDataPackage(buffer);
		
		if(!strcmp(mode, "netascii"))
			fprintf(fPointer, "%s", dataP.data);
		else
			fwrite(dataP.data, strlen(dataP.data), 1, fPointer);
		
		//invio l'ACK del pacchetto
		makeAckPackage(buffer, dataP.blockNum);
		printf("\r\tRicevuto pacchetto %u di %d byte\n", dataP.blockNum, (int)strlen(dataP.data));
		ret = send(sd, buffer, 4, 0);
		if (ret == -1){
			perror("Errore nel invio del ACK: ");
			exit(1);
		}
	}while(strlen(dataP.data) >= 512);
	
	printf("Trasferiti con successo %u pacchetti\n\n",  dataP.blockNum + 1);
	fclose(fPointer);
}

void tftpRecvFile(int sd, char* requestFilename, char* destFilename, char* mode){
	char buffer[TOT_LEN];

	makeRequestPackage(requestFilename, buffer, mode);
	send(sd, buffer, BUF_LEN, 0);
	RecvFile(sd, destFilename, mode);
}

















