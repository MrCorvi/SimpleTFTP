#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

//Header files
#include"headers/requestPackage.h"
#include"headers/errorPackage.h"
#include"headers/dataPackage.h"

#include"headers/tftpSendFile.h"

#define BUF_LEN			30

extern struct rrqQueueStruct rrqQueue;

extern unsigned short getOpcode(char* buffer);

int main (int argc, char *argv[]) {
	int port, sd, svAddrLen, ret;
	socklen_t clAddrLen;
	
	struct sockaddr_in sv_addr; // Struttura per il server
	struct sockaddr_in cl_addr;
	char buffer[1024], dir[64], serverFilename[64];
	
	FILE* fPointer;
	
	struct request rrq;
	
	//E' stata inserita la porta da usare ?
	if(argc < 3){
		printf("Nessuna cartella selezionata: verra usata la cartella locale\n");
		strcpy(dir, "./");
	}else{
		strcpy(dir, argv[2]);
	}
	
	//E' stata inserita la porta da usare ?
	if(argc < 2){
		printf("Verra usata la porta di 69 default\n");
		port = 69;  
	}else{
		port = atoi(argv[1]);
		
		//Controllo che la porta sia valida
		if(port <= 0 || port >= 65535){
			printf("Porta scelta fuori dal range consentito\n");
			exit(0);
		}
	}
	
	//Creazione socket
	sd = socket(AF_INET, SOCK_DGRAM, 0);
	if (sd == -1){
		perror("Errore durate la creazione del socket: ");
		exit(1);
	}
	
	
	/* Creazione indirizzo del server */
	memset(&sv_addr, 0, sizeof(sv_addr)); // Pulizia
	sv_addr.sin_family = AF_INET;
	sv_addr.sin_port = htons(port);
	inet_pton(AF_INET, "localhost", &sv_addr.sin_addr);
	
	//Aspetto rischieste sulla porta sd
	svAddrLen = sizeof(sv_addr);
	ret = bind(sd, (struct sockaddr*)&sv_addr, svAddrLen);
	if (ret == -1){
		perror("Errore durate il binding all'indirizzo: ");
		exit(1);
	}
	
	//inizializzo la coda
	rrqQueue.index = -1;
	
	while(1){
		//Invio il messaggio
		clAddrLen = (socklen_t)sizeof(cl_addr);
		if(rrqQueue.index == -1){
			recvfrom(sd, buffer, BUF_LEN, 0, (struct sockaddr*)&cl_addr, &clAddrLen);
		}else{
			cl_addr = rrqQueue.addr[rrqQueue.index];
			clAddrLen = rrqQueue.len[rrqQueue.index];
			rrqQueue.index--;
		}
		
		printf("\n\033[1;31m" "Richiesta di connessione" "\033[0m; \n");
		
		rrq = decodeRequestPackage(buffer);
		printf("Client Address: %s \nPorta clinet: %u\n", 
				inet_ntoa(*(struct in_addr*)&cl_addr),
                ntohs(cl_addr.sin_port));
        
        //Controllo che il messaggio di richiesta sia effettovamente un pacchetto RRQ
        if(getOpcode(buffer) != 1){
        	sendErrorPacket(sd, 0, "illegal TFTP operation, codice operativo invalido", cl_addr, clAddrLen);
        	continue;
        }
        
        strcpy(serverFilename, dir);
        strcat(serverFilename, rrq.filename);
		
		//Apro il file richiesto dal client e ne invio il contenuto
		printf("Richiesto il File: %s\n", serverFilename);
		if(strcmp(rrq.mode,"netascii"))
			fPointer = fopen(serverFilename, "r");
		else
			//Invio in modalita Binaria
			fPointer = fopen(serverFilename, "rb");
		
        if(!fPointer){
        	printf("File non trovato: %s\n", serverFilename);
        	sendErrorPacket(sd, 0, "Il file richiesto non era presente.", cl_addr, clAddrLen);
        	continue;
        }
                
		tftpSendFile(sd, cl_addr, clAddrLen, fPointer, rrq.mode);
		fclose(fPointer);
	}
	
}
























