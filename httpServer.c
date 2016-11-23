
/**
 * \file
 *
 * Este arquivo (jutamente com o seu respectivo httpd.h) foi derivado do HTTP Server
 * (tinyhttpd: \ref http://tinyhttpd.sourceforge.net/) e deve ser utilizado apenas para
 * propositos academicos, uma vez que nao eh um programa de producao.
 *
 * @author Rafael B. Januzi (rjanuzi@gmail.com)
 * @date 07/11/2016
 *
 */

#include <httpServer.h>

#define TEST_JSON	"{\"alt\":1.0,\"lat\":-23.210240,\"lon\": -45.875479,\"vel_x\":0.0,\"vel_y\":0.0,\"vel_z\":0.0,\"pitch\":0.0,\"roll\":0.0,\"yaw\":-37.0,\"temp\":25.0,\"bat\":73.0,\"press\":1.0}"

volatile static bool httpServer_ok = false;
volatile static pthread_t serverThreadHandle;

#define ISspace(x) isspace((int)(x))

#define SERVER_STRING "Server: arDroneMiddleware/1.0-mocked\r\n"

void accept_request(int);
void bad_request(int);
void cat(int, FILE *);
void cannot_execute(int);
void error_die(const char *);
void execute_cgi(int, const char *, const char *, const char *);
int get_line(int, char *, int);
void headers(int, const char *);
void not_found(int);
void serve_file(int, const char *);
int startup(u_short *);
void unimplemented(int);

void* httpServer_serverThread(void *arg)
{
	int server_sock = -1;
	u_short port = HTTP_SERVER_PORT;
	int client_sock = -1;
	struct sockaddr_in client_name;
	int client_name_len = sizeof(client_name);
	pthread_t newthread;

	server_sock = startup(&port);

	if(server_sock < 0)
	{
		printf("\n[ERROR]: Fail to initiate the HTTP Server on port %d.", port);
		goto errInit;
	}
	else
	{
		printf("\n[LOG]: HTTP Server running on port %d\n", port);
		httpServer_ok = true;
	}

	while (1)
	{
		client_sock = accept(server_sock,(struct sockaddr *)&client_name, &client_name_len);
		if (client_sock == -1)
		{
			perror("accept");
			goto errStops;
		}

		if (pthread_create(&newthread ,(volatile const pthread_attr_t* ) NULL, accept_request, client_sock) != 0)
			perror("pthread_create");
	}

	errStops:
	close(server_sock);

	errInit:
	httpServer_ok = false;

	return NULL;
}

void accept_request(int client)
{
	char buf[1024];
	int numchars;
	char method[255];
	char url[255];
	char path[512];
	char cmdVal[50];
	size_t i, j;
	struct stat st;
	int cgi = 0;      /* becomes true if server decides this is a CGI
	 * program */
	char *query_string = NULL;

	numchars = get_line(client, buf, sizeof(buf));
	i = 0; j = 0;

	while (!ISspace(buf[j]) && (i < sizeof(method) - 1))
	{
		method[i] = buf[j];
		i++; j++;
	}
	method[i] = '\0';

	/* Verifica se foi um GET */
	if(strcasecmp(method, "GET") == 0)
	{
		i = 0;
		while (ISspace(buf[j]) && (j < sizeof(buf)))
		{
			j++;
		}
		while (!ISspace(buf[j]) && (i < sizeof(url) - 1) && (j < sizeof(buf)))
		{
			url[i] = buf[j];
			i++; j++;
		}
		url[i] = '\0';

		//Para essa aplicacao especifica apenas checa-se para os casos especificos a url e responde de acordo.
		if(strcmp(url, HTTP_SERVER_TM_URL) == 0)
		{
			//Solicitacao de TMs
			httpServer_sendTms(client);
		}
		else
		{
			if(strcmp(url, HTTP_SERVER_PHOTO_URL) == 0)
			{
				//Solicitacao de Foto
				httpServer_sendPhotoData(client);
			}
			else
			{
				if(strcmp(url, HTTP_SERVER_CMD_URL) == 0)
				{
					//Solicitacao da pagina de teste de comandos
					httpServer_sendPage(client, "cmds_test.html");
				}
				else
				{
					printf("\n[ERROR]: URL invalido: GET %s\n", url);
					not_found(client);
				}
			}
		}
	}
	else
	{
		if(strcasecmp(method, "POST") == 0)
		{
			printf("\n[LOG]: Recebido POST.\n");
			i = 0;
			while (ISspace(buf[j]) && (j < sizeof(buf)))
			{
				j++;
			}
			while (!ISspace(buf[j]) && (i < sizeof(url) - 1) && (j < sizeof(buf)))
			{
				url[i] = buf[j];
				i++; j++;
			}
			url[i] = '\0';

			//Para essa aplicacao especifica apenas checa-se para os casos especificos a url e responde de acordo.
			if(strcmp(url, HTTP_SERVER_CMD_URL) == 0)
			{
				//Solicitacao de Comando
				httpServer_sendPage(client, "cmds_test.html");
				//				httpServer_sendPage(client, "very_simple_page.html");

				/* Procura o parametro HTTP_SERVER_CMD_PARAM no request recebido */
				do
				{
					numchars = get_line(client, buf, sizeof(buf));
				}while(numchars > 0 && !httpServer_strStartsWith(HTTP_SERVER_CMD_PARAM, buf));

				httpServer_getCmdParamVal(cmdVal, buf);

				printf("\nComando recebido: %s\n", cmdVal);

				httpServer_execDroneCmd(cmdVal);
			}
			else
			{
				printf("\n[ERROR]: URL invalido: POST %s\n", url);
				not_found(client);
			}
		}
		else
		{
			unimplemented(client);
			return;
		}
	}

	sleep(5);

	close(client);
}

/**********************************************************************/
/* Inform the client that a request it has made has a problem.
 * Parameters: client socket */
/**********************************************************************/
void bad_request(int client)
{
	char buf[1024];

	sprintf(buf, "HTTP/1.0 400 BAD REQUEST\r\n");
	send(client, buf, sizeof(buf), 0);
	sprintf(buf, "Content-type: text/html\r\n");
	send(client, buf, sizeof(buf), 0);
	sprintf(buf, "\r\n");
	send(client, buf, sizeof(buf), 0);
	sprintf(buf, "<P>Your browser sent a bad request, ");
	send(client, buf, sizeof(buf), 0);
	sprintf(buf, "such as a POST without a Content-Length.\r\n");
	send(client, buf, sizeof(buf), 0);
}

/**********************************************************************/
/* Put the entire contents of a file out on a socket.  This function
 * is named after the UNIX "cat" command, because it might have been
 * easier just to do something like pipe, fork, and exec("cat").
 * Parameters: the client socket descriptor
 *             FILE pointer for the file to cat */
/**********************************************************************/
void cat(int client, FILE *resource)
{
	char buf[1024];

	fgets(buf, sizeof(buf), resource);
	while (!feof(resource))
	{
		send(client, buf, strlen(buf), 0);
		fgets(buf, sizeof(buf), resource);
	}
}

void sendString(int client, const char* strToSend)
{
	char buf[2048];

	strcpy(buf, strToSend);
	send(client, buf, strlen(buf), 0);
}

/**********************************************************************/
/* Inform the client that a CGI script could not be executed.
 * Parameter: the client socket descriptor. */
/**********************************************************************/
void cannot_execute(int client)
{
	char buf[1024];

	sprintf(buf, "HTTP/1.0 500 Internal Server Error\r\n");
	send(client, buf, strlen(buf), 0);
	sprintf(buf, "Content-type: text/html\r\n");
	send(client, buf, strlen(buf), 0);
	sprintf(buf, "\r\n");
	send(client, buf, strlen(buf), 0);
	sprintf(buf, "<P>Error prohibited CGI execution.\r\n");
	send(client, buf, strlen(buf), 0);
}

/**********************************************************************/
/* Print out an error message with perror() (for system errors; based
 * on value of errno, which indicates system call errors) and exit the
 * program indicating an error. */
/**********************************************************************/
void error_die(const char *sc)
{
	perror(sc);
	exit(1);
}

/**********************************************************************/
/* Execute a CGI script.  Will need to set environment variables as
 * appropriate.
 * Parameters: client socket descriptor
 *             path to the CGI script */
/**********************************************************************/
void execute_cgi(int client, const char *path,
		const char *method, const char *query_string)
{
	char buf[1024];
	int cgi_output[2];
	int cgi_input[2];
	pid_t pid;
	int status;
	int i;
	char c;
	int numchars = 1;
	int content_length = -1;

	buf[0] = 'A'; buf[1] = '\0';
	if (strcasecmp(method, "GET") == 0)
		while ((numchars > 0) && strcmp("\n", buf))  /* read & discard headers */
			numchars = get_line(client, buf, sizeof(buf));
	else    /* POST */
	{
		numchars = get_line(client, buf, sizeof(buf));
		while ((numchars > 0) && strcmp("\n", buf))
		{
			buf[15] = '\0';
			if (strcasecmp(buf, "Content-Length:") == 0)
				content_length = atoi(&(buf[16]));
			numchars = get_line(client, buf, sizeof(buf));
		}
		if (content_length == -1) {
			bad_request(client);
			return;
		}
	}

	sprintf(buf, "HTTP/1.0 200 OK\r\n");
	send(client, buf, strlen(buf), 0);

	if (pipe(cgi_output) < 0) {
		cannot_execute(client);
		return;
	}
	if (pipe(cgi_input) < 0) {
		cannot_execute(client);
		return;
	}

	if ( (pid = fork()) < 0 ) {
		cannot_execute(client);
		return;
	}
	if (pid == 0)  /* child: CGI script */
	{
		char meth_env[255];
		char query_env[255];
		char length_env[255];

		dup2(cgi_output[1], 1);
		dup2(cgi_input[0], 0);
		close(cgi_output[0]);
		close(cgi_input[1]);
		sprintf(meth_env, "REQUEST_METHOD=%s", method);
		putenv(meth_env);
		if (strcasecmp(method, "GET") == 0) {
			sprintf(query_env, "QUERY_STRING=%s", query_string);
			putenv(query_env);
		}
		else {   /* POST */
			sprintf(length_env, "CONTENT_LENGTH=%d", content_length);
			putenv(length_env);
		}
		execl(path, path, NULL);
		exit(0);
	} else {    /* parent */
		close(cgi_output[1]);
		close(cgi_input[0]);
		if (strcasecmp(method, "POST") == 0)
			for (i = 0; i < content_length; i++) {
				recv(client, &c, 1, 0);
				write(cgi_input[1], &c, 1);
			}
		while (read(cgi_output[0], &c, 1) > 0)
			send(client, &c, 1, 0);

		close(cgi_output[0]);
		close(cgi_input[1]);
		waitpid(pid, &status, 0);
	}
}

/**********************************************************************/
/* Get a line from a socket, whether the line ends in a newline,
 * carriage return, or a CRLF combination.  Terminates the string read
 * with a null character.  If no newline indicator is found before the
 * end of the buffer, the string is terminated with a null.  If any of
 * the above three line terminators is read, the last character of the
 * string will be a linefeed and the string will be terminated with a
 * null character.
 * Parameters: the socket descriptor
 *             the buffer to save the data in
 *             the size of the buffer
 * Returns: the number of bytes stored (excluding null) */
/**********************************************************************/
int get_line(int sock, char *buf, int size)
{
	int i = 0;
	char c = '\0';
	int n;

	while ((i < size - 1) && (c != '\n'))
	{
		n = recv(sock, &c, 1, 0);
		/* DEBUG printf("%02X\n", c); */
		if (n > 0)
		{
			if (c == '\r')
			{
				n = recv(sock, &c, 1, MSG_PEEK);
				/* DEBUG printf("%02X\n", c); */
				if ((n > 0) && (c == '\n'))
					recv(sock, &c, 1, 0);
				else
					c = '\n';
			}
			buf[i] = c;
			i++;
		}
		else
			c = '\n';
	}
	buf[i] = '\0';

	return(i);
}

/**********************************************************************/
/* Return the informational HTTP headers about a file. */
/* Parameters: the socket to print the headers on
 *             the name of the file */
/**********************************************************************/
void headers(int client, const char *filename)
{
	char buf[1024];
	(void)filename;  /* could use filename to determine file type */

	strcpy(buf, "HTTP/1.0 200 OK\r\n");
	send(client, buf, strlen(buf), 0);
	strcpy(buf, SERVER_STRING);
	send(client, buf, strlen(buf), 0);
	sprintf(buf, "Content-Type: application/json\r\n");
	send(client, buf, strlen(buf), 0);
	strcpy(buf, "\r\n");
	send(client, buf, strlen(buf), 0);
}

/**********************************************************************/
/* Give a client a 404 not found status message. */
/**********************************************************************/
void not_found(int client)
{
	char buf[1024];

	sprintf(buf, "HTTP/1.0 404 NOT FOUND\r\n");
	send(client, buf, strlen(buf), 0);
	sprintf(buf, SERVER_STRING);
	send(client, buf, strlen(buf), 0);
	sprintf(buf, "Content-Type: text/html\r\n");
	send(client, buf, strlen(buf), 0);
	sprintf(buf, "\r\n");
	send(client, buf, strlen(buf), 0);
	sprintf(buf, "<HTML><TITLE>Not Found</TITLE>\r\n");
	send(client, buf, strlen(buf), 0);
	sprintf(buf, "<BODY><P>The server could not fulfill\r\n");
	send(client, buf, strlen(buf), 0);
	sprintf(buf, "your request because the resource specified\r\n");
	send(client, buf, strlen(buf), 0);
	sprintf(buf, "is unavailable or nonexistent.\r\n");
	send(client, buf, strlen(buf), 0);
	sprintf(buf, "</BODY></HTML>\r\n");
	send(client, buf, strlen(buf), 0);
}

/**********************************************************************/
/* Send a regular file to the client.  Use headers, and report
 * errors to client if they occur.
 * Parameters: a pointer to a file structure produced from the socket
 *              file descriptor
 *             the name of the file to serve */
/**********************************************************************/
void serve_file(int client, const char *filename)
{
	FILE *resource = NULL;
	int numchars = 1;
	char buf[1024];

	buf[0] = 'A'; buf[1] = '\0';
	while ((numchars > 0) && strcmp("\n", buf))  /* read & discard headers */
		numchars = get_line(client, buf, sizeof(buf));

	resource = fopen(filename, "r");
	if (resource == NULL)
		not_found(client);
	else
	{
		headers(client, filename);
		cat(client, resource);
	}
	fclose(resource);
}

void httpServer_sendTms(int client)
{
	int numchars = 1;
	char buf[1024];
	char resultJson[1024];

	buf[0] = 'A'; buf[1] = '\0';
	while ((numchars > 0) && strcmp("\n", buf))  /* read & discard headers */
		numchars = get_line(client, buf, sizeof(buf));

	HTTP_SERVER_TM_JSON_CREATE(droneTms_getTmData(), resultJson);

	headers(client, NULL);
	sendString(client, resultJson);
}

void httpServer_sendPhotoData(int client)
{
	uint32_t numBytes = -1;
	uint8_t imgBinData[DRONE_PHOTO_MAX_LEN];

	numBytes = droneTms_getPhoto(imgBinData);

	if(numBytes < 0)
	{
		printf("\n[ERROR]: httpServer_sendPhotoData - Fail reading the image bytes.\n");
		not_found(client);
		return;
	}

	httpServer_headersJPG(client, numBytes);
	send(client, (void*) imgBinData, (size_t) numBytes, 0);
}

void httpServer_headersJPG(int client, uint32_t imgLenBytes)
{
	char buf[1024];

	strcpy(buf, "HTTP/1.0 200 OK\r\n");
	send(client, buf, strlen(buf), 0);
	strcpy(buf, SERVER_STRING);
	send(client, buf, strlen(buf), 0);
	sprintf(buf, "Content-Length: %d\r\nContent-Type: image/jpeg\r\n", imgLenBytes);
	send(client, buf, strlen(buf), 0);
	strcpy(buf, "\r\n");
	send(client, buf, strlen(buf), 0);
}

void httpServer_sendPage(int client, const char* pageFile)
{
	FILE* page;
	uint16_t numBytes = -1;
	uint8_t pageBinData[HTTP_PAGE_FILE_MAX_LEN];

	page = fopen(pageFile, "r");

	if(page == NULL)
	{
		printf("[ERROR]: httpServer_sendPage - Fail Opening the page bytes.");
		not_found(client);
		return;
	}

	numBytes = fread((void*) pageBinData, 1, HTTP_PAGE_FILE_MAX_LEN, page);

	fclose(page);

	if(numBytes <= 0)
	{
		printf("[ERROR]: httpServer_sendPage - Fail reading the page bytes.");
		not_found(client);
		return;
	}

	httpServer_headersHtml(client, numBytes);
	send(client, (void*) pageBinData, (size_t) numBytes, 0);
}

void httpServer_headersHtml(int client, uint16_t size)
{
	char buf[1024];

	strcpy(buf, "HTTP/1.0 200 OK\r\n");
	send(client, buf, strlen(buf), 0);
	strcpy(buf, SERVER_STRING);
	send(client, buf, strlen(buf), 0);
	sprintf(buf, "Content-Length: %d\r\nContent-Type: text/html\r\n", size);
	send(client, buf, strlen(buf), 0);
	strcpy(buf, "\r\n");
	send(client, buf, strlen(buf), 0);
}


/**********************************************************************/
/* This function starts the process of listening for web connections
 * on a specified port.  If the port is 0, then dynamically allocate a
 * port and modify the original port variable to reflect the actual
 * port.
 * Parameters: pointer to variable containing the port to connect on
 * Returns: the socket */
/**********************************************************************/
int startup(u_short *port)
{
	int httpd = 0;
	struct sockaddr_in name;

	httpd = socket(PF_INET, SOCK_STREAM, 0);
	if (httpd == -1)
		error_die("socket");
	memset(&name, 0, sizeof(name));
	name.sin_family = AF_INET;
	name.sin_port = htons(*port);
	name.sin_addr.s_addr = htonl(INADDR_ANY);

	if (bind(httpd, (struct sockaddr *)&name, sizeof(name)) < 0)
	{
		error_die("bind");
		goto err;
	}

	if (*port == 0)  /* if dynamically allocating a port */
	{
		int namelen = sizeof(name);
		if (getsockname(httpd, (struct sockaddr *)&name, &namelen) == -1)
		{
			perror("getsockname");
			goto err;
		}
		*port = ntohs(name.sin_port);
	}

	if (listen(httpd, 5) < 0)
	{
		perror("listen");
		goto err;
	}

	return(httpd);

	err:
	return -1;
}

/**********************************************************************/
/* Inform the client that the requested web method has not been
 * implemented.
 * Parameter: the client socket */
/**********************************************************************/
void unimplemented(int client)
{
	char buf[1024];

	sprintf(buf, "HTTP/1.0 501 Method Not Implemented\r\n");
	send(client, buf, strlen(buf), 0);
	sprintf(buf, SERVER_STRING);
	send(client, buf, strlen(buf), 0);
	sprintf(buf, "Content-Type: text/html\r\n");
	send(client, buf, strlen(buf), 0);
	sprintf(buf, "\r\n");
	send(client, buf, strlen(buf), 0);
	sprintf(buf, "<HTML><HEAD><TITLE>Method Not Implemented\r\n");
	send(client, buf, strlen(buf), 0);
	sprintf(buf, "</TITLE></HEAD>\r\n");
	send(client, buf, strlen(buf), 0);
	sprintf(buf, "<BODY><P>HTTP request method not supported.\r\n");
	send(client, buf, strlen(buf), 0);
	sprintf(buf, "</BODY></HTML>\r\n");
	send(client, buf, strlen(buf), 0);
}

bool httpServer_init(void)
{
	int err;

	err = pthread_create(&serverThreadHandle, NULL, &httpServer_serverThread, NULL);

	if(err != 0)
	{
		printf("\n[ERROR]: httpServer.httpServer_init - Can't create httpServer_serverThread: [%s]", strerror(err));
		return false;
	}
	else
	{
		printf("\n[LOG]: httpServer.httpServer_init - httpServer_serverThread created sucessfully\n");
		return true;
	}
}

bool httpServer_strStartsWith(const char* pre, const char* str)
{
	return strncmp(pre, str, strlen(pre)) == 0;
}

void httpServer_getCmdParamVal(char* valOut, const char* cmdParam)
{
	int i;

	valOut[0] = '\0';

	for(i = 0; i < strlen(cmdParam); i++)
	{
		//Find the equals('=') char
		if(cmdParam[i] == '=')
		{
			i++;
			break;
		}
	}

	strcpy(valOut, &cmdParam[i]); /* Copia o resto da String para a saida. "cmd=<VALUE>" */
}

void httpServer_execDroneCmd(const char* cmd)
{
	char atCmd[50];
	int i;

	if(strcmp(cmd, HTTP_SERVER_CMD_TAKEOFF) == 0)
	{
		printf("\n[LOG]: httpServer_execDroneCmd - Taking off...\n");
		for(i = 0; i < 10; i++)
		{
			ATCMD_CREATE_AT_REF_CMD(atCmd, droneTcs_getNextSeqNmbr(), ATCMD_AT_REF_FIXED_BITS | ATCMD_AT_REF_TAKE_OF_FLAG);
			droneTcs_sendAtCmd(atCmd);
		}
	}
	else
	{
		if(strcmp(cmd, HTTP_SERVER_CMD_LAND) == 0)
		{
			printf("\n[LOG]: httpServer_execDroneCmd - Landing...\n");
			for(i = 0; i < 10; i++)
			{
				ATCMD_CREATE_AT_REF_CMD(atCmd, droneTcs_getNextSeqNmbr(), ATCMD_AT_REF_FIXED_BITS | ATCMD_AT_REF_LAND_FLAG);
				droneTcs_sendAtCmd(atCmd);
			}
		}
		else
		{
			//if()...
			printf("\n[ERROR]: httpServer_execDroneCmd - Drone Command UNKNOW\n");
		}
	}
}

bool httpServer_isServerOk()
{
	return httpServer_ok;
}
