//Import necessary libraries
#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<string.h>
#include<unistd.h>


void error(const char* msg)
{
	perror(msg);
	exit(1);
}


int main()
{
	int c_sock;
	
	//call socket() function with necessary parameters for TCP
	c_sock = socket(AF_INET, SOCK_STREAM, 0);
	
	if (c_sock < 0)
		error("Error opening socket");
	
	struct sockaddr_in client;
	
	memset(&client, 0, sizeof(client));
	
	client.sin_family = AF_INET;
	client.sin_port = htons(9009);
	client.sin_addr.s_addr = INADDR_ANY;
	
	//establish connection
	if(connect(c_sock, (struct sockaddr *)&client, sizeof(client)) == -1)
	{
		error("Connection failed\n");
		return 1;
	}

	printf("Yo! This is Client.\n\n");

	char fname[100], extension[100], ext[10], buffer[2000], buf[1];

	printf("Enter File name without extension: ");
	scanf("%s", fname);

	printf("Enter extension: ");
	scanf("%s", ext);

	//concat the extension to the filname with a dot in between
	strcpy(extension, fname);
	strcat(extension, ".");
	strcat(extension, ext);

	printf("\nFile Name: %s.\n\n", extension);

	//open the file in read mode
	FILE *fp;
	fp = fopen(extension, "r");

	printf("Opened %s.\n", extension);

	char eof[1] = "0";

	//send the name of the file and its extension to the server
	send(c_sock, fname, sizeof(fname), 0);
	send(c_sock, ext, sizeof(ext), 0);

	while(!feof(fp))
	{ 
		//scan the file and send its contents to the server
		fscanf(fp, "%c", &buf[0]);
		send(c_sock, eof, sizeof(eof), 0);
		send(c_sock, buf, sizeof(buf), 0);
	}
	
	printf("\n\nDone reading file.\n\n");
	
	//send a signal to the server indicating that the file has been sent
	eof[0] = '1';
	send(c_sock, eof, sizeof(eof), 0);
	
	printf("File Sent.\n");

	//close client
	close(c_sock);

	return 0;
}


