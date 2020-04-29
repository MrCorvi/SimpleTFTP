
struct DataPack{
	unsigned short blockNum;
	char data[512];
};

struct DataPack decodeDataPackage(char* buffer);
void makeDataPackage(char* buffer, unsigned short seq, char* data, int nData);
