

void printError(char* buffer);
void  sendErrorPacket(int sd, unsigned short errorCode, char* errorMsg, struct sockaddr_in cl_addr, int clAddrLen);

