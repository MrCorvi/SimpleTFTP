
//file con strutture e funzioni che gestiscono il pacchetto RRQ per la richiesta di un file

struct request{
	char filename[64];
	char mode[14];
};

struct request decodeRequestPackage(char* buffer);

void makeRequestPackage(char* filename, char* buffer, char* mode);
