#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> 

//Header files
#include "headers/getPromptComand.h" 
#include "headers/tftpRecvFile.h"

void printHelpMessage(){
	printf("\nSono disponibili i seguenti comadi:\n");
	printf("!help --> mostra l'elenco dei comandi disponibili\n");
	printf("!mode {txt|bin} --> imposta il modo di trasferimento dei file (testo o binario)\n");
	printf("!get filename nome_locale --> richiede al server il nome del file\n");
	printf("!quit --> termina il client\n\n");
	return;
}

int main (int argc, char* argv[]){
	int porta, sd, ret;	
	struct sockaddr_in sv_addr; // Struttura per il server
	
	enum ClientCommand cliCommand;
	
	struct promptReturn pRet;
	
	if(argc < 3){
		printf("Non hai inserito abbastanza argomenti\n");
		exit(0);
	}
	
	porta = atoi(argv[2]);
	
	// Creazione socket 
	sd = socket(AF_INET, SOCK_DGRAM, 0);
	if (sd == -1){
		perror("Errore durate la creazione del socket: ");
		exit(1);
	}
	
	// Creazione indirizzo del server 
	memset(&sv_addr, 0, sizeof(sv_addr)); // Pulizia
	sv_addr.sin_family = AF_INET;
	sv_addr.sin_port = htons(porta);
	inet_pton(AF_INET, argv[1], &sv_addr.sin_addr);
	
	//connetto il socket all'indirizzo del server
	ret = connect(sd, (struct sockaddr*)&sv_addr, sizeof(sv_addr));
	if (ret == -1){
		perror("Errore durate la connessione con l'indirizzo: ");
		exit(1);
	}
	
	//metto i risultati di defalult in pRet
	strcpy(pRet.serverFilename, "");
	strcpy(pRet.localFilename, "");
	strcpy(pRet.mode, "bin");
	
	printHelpMessage();
	
	while(1){
		
		cliCommand = getCommand(&pRet);
		
		switch(cliCommand){
			case HELP:
				printHelpMessage();
				break;
			
			case MODE:
				printf("\nla modalita' %s e' stata selezionata\n\n", pRet.mode);
				break;
				
			case MODE_UNKNOWN:
				printf("\nLa modalita' che hai selezionato non e' valida\n\n");
				break;
				
			case GET:
				//Richiedo al server il file, usando il protocollo TFTP
				tftpRecvFile(sd, pRet.serverFilename, pRet.localFilename, pRet.mode);
				break;
			
			case UNKNOWN:
				printf("\nIl commando che hai selezionato non e' valida\n\n");
				break;
			
			default:
				break;
		}
		
		if(cliCommand == QUIT){
			break;
		}
	}
	close(sd);
	
	return 0;
}



































