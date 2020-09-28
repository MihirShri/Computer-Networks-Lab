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

	//call socket() function with necessary parameters for UDP
	c_sock = socket(AF_INET, SOCK_DGRAM, 0);
	
	if (c_sock < 0)
		error("Error opening socket");
	
	struct sockaddr_in client;
	socklen_t client_len;
	
	memset(&client, 0, sizeof(client));
	
	client_len = sizeof(client);
	client.sin_family = AF_INET;
	client.sin_port = htons(9009);
	client.sin_addr.s_addr = INADDR_ANY;

	printf("Yo! This is Client.\n\n");

	char fname[100], extension[100], ext[10], buffer[20000], buf[1];

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
	sendto(c_sock, fname, sizeof(fname), 0, (struct sockaddr*)&client, sizeof(client));
	sendto(c_sock, ext, sizeof(ext), 0, (struct sockaddr*)&client, sizeof(client));

	while(!feof(fp))
	{
		//scan the file and send its contents to the server
		fscanf(fp, "%c", &buf[0]);
		sendto(c_sock, eof, sizeof(eof), 0, (struct sockaddr*)&client, sizeof(client));
		sendto(c_sock, buf, sizeof(buf), 0, (struct sockaddr*)&client, sizeof(client));
	}
	
	printf("\n\nFile Ended \n\n");
	
	//send a signal to the server indicating that the file has been sent
	eof[0] = '2';
	sendto(c_sock, eof, sizeof(eof), 0, (struct sockaddr*)&client, sizeof(client));

	printf("File sent.\n");

	//close client
	close(c_sock);

	return 0;
}


