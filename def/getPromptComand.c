
#include <string.h>
#include <stdio.h>


#include"../headers/getPromptComand.h"

enum ClientCommand getCommand(struct promptReturn* pRet){
	char input[30], tempMode[6];
	int ret;
	
	printf("\x1b[32m" "tftp>> " "\x1b[0m");
	fgets(input, 200, stdin);
	
	if(strlen(input) < 4){
		return UNKNOWN;
	}
	
	if(!strncmp(input, "!get", 4)){
		ret = sscanf(input, "!get %s %s", pRet->serverFilename, pRet->localFilename);
		if(ret < 0){
			strcpy(pRet->serverFilename, "source.txt");
			strcpy(pRet->localFilename, "./dest.txt");
		}
		return GET;
	}
	
	if(!strncmp(input, "!mode", 5)){
		ret = sscanf(input, "!mode %s", tempMode);
		if(strcmp(tempMode, "txt") && strcmp(tempMode, "bin"))
			return MODE_UNKNOWN;

		strcpy(pRet->mode, tempMode);
		return MODE;
	}
	
	if(!strncmp(input, "!help", 5)){
		//printf("hufauwfhauwf %s\n", input);
		return HELP;
	}
	
	if(!strncmp(input, "!quit", 5)){
		return QUIT;
	}
	
	return UNKNOWN;
}
