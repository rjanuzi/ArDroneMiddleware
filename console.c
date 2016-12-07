/**
 * \file
 *
 * Este arquivo, juntamente com o seu .h relacionado, reune funcionalidades simples para 
 * recepcao de comandos para teste utilizando o console.
 *
 * @author Rafael B. Januzi (rjanuzi@gmail.com)
 * @date 16/11/2016
 *
 */

#include <droneTMs.h>

static volatile pthread_t console_inputReceiver;

void* console_inputReceiverThread (void *arg)
{
	char input;

	while( true )
	{
		scanf("%c", &input);

		switch(input)
		{
		case 't':
		case 'T':
			printf("\nTaking off...");
			break;
		case 'l':
		case 'L':
			printf("\nLanding....");
			break;

		default:
			break;
		}
	}

	return NULL;
}

bool console_init()
{
	int err;

	/* Input Listener */
	err = pthread_create(&console_inputReceiver, NULL, &console_inputReceiverThread, NULL);
	if(err != 0)
	{
		printf("\n[ERROR]: console_init - Can't create console_inputReceiverThread: [%s]", strerror(err));
		return false;
	}
	else
	{
		printf("\n[LOG]: console_init - console_inputReceiverThread created sucessfully\n");
	}

	return true;
}

pthread_t console_getInputReceiverThread()
{
	return console_inputReceiver;
}

