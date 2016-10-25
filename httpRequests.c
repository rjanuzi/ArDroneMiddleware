#include <stdio.h> /* printf, sprintf */
#include <stdlib.h> /* exit */
#include <unistd.h> /* read, write, close */
#include <string.h> /* memcpy, memset */
#include <sys/socket.h> /* socket, connect */
#include <netinet/in.h> /* struct sockaddr_in, struct sockaddr */
#include <netdb.h> /* struct hostent, gethostbyname */

#define HTTP_MESSAGE_MAX_LEN	1000
#define HTTP_RESPONSE_MAX_LEN	10000

void error(const char *msg) { perror(msg); exit(0); }

void makeGet(int portno, char* host, char* application, char* response)
{
	char message_template[HTTP_MESSAGE_MAX_LEN] = "GET %s HTTP/1.0\r\n\r\n";
	char message[HTTP_MESSAGE_MAX_LEN];
	struct hostent *server;
	struct sockaddr_in serv_addr;
	int sockfd, bytes, sent, received, total;

	sprintf(message, message_template, application);

	/* create the socket */
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if(sockfd < 0)
		error("ERROR opening socket");

	/* lookup the ip address */
	server = gethostbyname(host);
	if(server == NULL)
		error("ERROR, no such host");

	/* fill in the structure */
	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(portno);
	memcpy(&serv_addr.sin_addr.s_addr, server->h_addr, server->h_length);

	/* connect the socket */
	if( connect(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) <0)
		error("ERROR connecting");

	/* send the request */
	total = strlen(message);
	sent = 0;
	do {
		bytes = write(sockfd, message+sent, total-sent);
		if( bytes < 0 )
			error("ERROR writing message to socket");
		if( bytes == 0 )
			break;
		sent += bytes;
	} while( sent < total );

	/* receive the response */
	memset(response, 0, sizeof(response));
	total = HTTP_RESPONSE_MAX_LEN-1;
	received = 0;

	do {
		bytes = read(sockfd, response+received, total-received);
		if( bytes < 0 )
			error("ERROR reading response from socket");
		if( bytes == 0 )
			break;
		received += bytes;
	} while (received < total);

	if(received == total)
		error("ERROR storing complete response from socket");

	/* close the socket */
	close(sockfd);
}

void makePOST(int portno, char* host, char* application, char* body, char* response)
{
	char message_template[HTTP_MESSAGE_MAX_LEN] = "POST %s HTTP/1.0\r\nContent-Type: text/html\r\nContent-Length: %d\r\n\r\n%s";
	char message[HTTP_MESSAGE_MAX_LEN];
	struct hostent *server;
	struct sockaddr_in serv_addr;
	int sockfd, bytes, sent, received, total;

	sprintf(message, message_template, application, strlen(body), body);

	/* create the socket */
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if(sockfd < 0)
		error("ERROR opening socket");

	/* lookup the ip address */
	server = gethostbyname(host);
	if(server == NULL)
		error("ERROR, no such host");

	/* fill in the structure */
	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(portno);
	memcpy(&serv_addr.sin_addr.s_addr, server->h_addr, server->h_length);

	/* connect the socket */
	if( connect(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) <0)
		error("ERROR connecting");

	/* send the request */
	total = strlen(message);
	sent = 0;
	do {
		bytes = write(sockfd, message+sent, total-sent);
		if( bytes < 0 )
			error("ERROR writing message to socket");
		if( bytes == 0 )
			break;
		sent += bytes;
	} while( sent < total );

	/* receive the response */
	memset(response, 0, sizeof(response));
	total = HTTP_RESPONSE_MAX_LEN-1;
	received = 0;

	do {
		bytes = read(sockfd, response+received, total-received);
		if( bytes < 0 )
			error("ERROR reading response from socket");
		if( bytes == 0 )
			break;
		received += bytes;
	} while (received < total);

	if(received == total)
		error("ERROR storing complete response from socket");

	/* close the socket */
	close(sockfd);
}

int main ( void )
{
	char response[HTTP_RESPONSE_MAX_LEN];

	printf("\n\nFazendo GET: ");
	makeGet(8080, "localhost", "/HttpRequestsTestServer/HttpRequests", response);
	printf("Response: \n%s\n", response);

	printf("\n\nFazendo POST: ");
	makePOST(8080, "localhost", "/HttpRequestsTestServer/HttpRequests", "nome=Rafael&user=rjanuzi", response);
	printf("Response: \n%s\n", response);

	return 0;
}

