 
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
	int s_sock;
	
	//call socket() function with necessary parameters for UDP
	s_sock = socket(AF_INET, SOCK_DGRAM, 0);
	
	if (s_sock < 0)
		error("Error opening socket");

	struct sockaddr_in server, client;
	socklen_t client_len;
	
	memset(&server, 0, sizeof(server));
	memset(&client, 0, sizeof(client));

	server.sin_family = AF_INET;
	server.sin_port = htons(9009);
	server.sin_addr.s_addr = INADDR_ANY;

	client_len = sizeof(client);
	
	if(bind(s_sock, (struct sockaddr*)&server, sizeof(server)) != -1)
		printf("Yo! This is SERVER.\n\n");
		
	else
	{
		error("\nError in Bind.\n");
		return 1;
	} 

	char fname[100], extension[100] = "", ext[10], buffer[2000], buf[1];

	//receive the fname and extension from the server
	recvfrom(s_sock, fname, sizeof(fname), 0, (struct sockaddr*)&client, &client_len);
	recvfrom(s_sock, ext, sizeof(ext), 0, (struct sockaddr*)&client, &client_len);

	//create a new file to which the contents of the file received will be dumped
	strcat(extension, fname);
	strcat(extension, "_recv.");
	strcat(extension, ext);

	//open the file in write mode
	FILE *fp;
	fp = fopen(extension, "w");

	char eof[1] = "0";

	int flag = 0;
	while(flag == 0)
	{
		//receive the end of file signal from the client
		recvfrom(s_sock, eof, sizeof(eof), 0, (struct sockaddr*)&client, &client_len);
		
		if(eof[0] == '2')
			flag = 1;
			
		else
		{
			//continue receiving data until end of file signal is received from the client
			recvfrom(s_sock, buf, sizeof(buf), 0, (struct sockaddr*)&client, &client_len);
			fprintf(fp, "%c", buf[0]);
		}
	}
	
	printf("\n\nDone receiving file\n\n");

	//close the file
	fclose(fp);
		
	printf("File %s Received Successfully.\n", extension);

	//close server
	close(s_sock);

	return 0;
}

