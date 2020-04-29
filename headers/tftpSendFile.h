
struct rrqQueueStruct{
	struct sockaddr_in addr[50];
	int len[50];
	int index;
} rrqQueue;

void tftpSendFile(int sd, struct sockaddr_in cl_addr, int clAddrLen, FILE* fPointer, char* mode);
