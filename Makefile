CC=gcc
CFLAGS = -Wall

objects = requestPackage.o errorPackage.o dataPackage.o ackPackage.o

server: server.o $(objects) tftpSendFile.o
	$(CC) $(CFLAGS) $^ -o server 
	rm *.o

server.o: server.c
	$(CC) $(CFLAGS) -c server.c -o $@



client: client.o $(objects) getPromptComand.o tftpRecvFile.o
	$(CC) $(CFLAGS) $^ -o client 
	rm *.o
	
client.o: client.c
	$(CC) $(CFLAGS) -c client.c -o client.o

#$(objects): $(definitions)
#	gcc -Wall -c $^ -o $@

%.o : def/%.c
	gcc -c $< -o $@


clear: 
	rm *.o
