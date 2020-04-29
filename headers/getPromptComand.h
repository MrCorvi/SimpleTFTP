
enum ClientCommand{
	HELP,
	MODE,
	MODE_UNKNOWN,
	GET,
	QUIT,
	UNKNOWN
};

struct promptReturn{
	char serverFilename[64];
	char localFilename[64];
	char mode[6];
};

enum ClientCommand getCommand(struct promptReturn* pRet);
