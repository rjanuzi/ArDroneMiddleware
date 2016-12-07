/**
 * \file
 *
 * Este arquivo, juntamente com o seu .h relacionado, representam o ponto de entrada
 * do programa (funcao main).
 *
 * @author Rafael B. Januzi (rjanuzi@gmail.com)
 * @date 14/11/2016
 *
 */

#include <main.h>

int main(int argc, char** argv)
{
	httpServer_init();
	//droneTms_init();
	//console_init();

	while(true)
	{
		sleep(60);

		if(!httpServer_isServerOk())
		{
			printf("\n[ERROR]: Server is not ok. Killing the ArDroneMiddleware.");
			break;
		}

		//TODO - Adicionar outras possiveis verificacoes.

		printf("\nI'am Alive!");
	}

	return 0;
}
