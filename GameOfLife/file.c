#include <stdlib.h>
#include "file.h"

/*
	* Retorna as informações do arquivo lido em vetor.
*/
int ReadGameFile(char *fname)
{
	FILE *file;
	char *buf, ch; // buffer
	int i = 0, j = 0; // variáveis de loop
	int width = 0, hight = 0; // largura, altura
	
	if((file = fopen(fname, "r")) == NULL)
	{
		printf("Arquivo não encontrado.");
		return -1;
	}

	// Ler toda a informação das células (0's e 1's).
	buf = (char*) calloc(hight * width, sizeof(char));
	do
	{
		ch = getc(file);
		if(ch != '\n')
		{
			buf[j + i * hight] = ch;
			i++;
			continue;
		}
		i++;
	} while(buf[j + i * hight] != '\0');

	// Passa a informação lida para a estrutura do programa.
	cells = (CELL**) calloc(hight, sizeof(CELL*));
	for(i = 0; i < hight; i++)
	{
		cells[i] = (CELL*) calloc(width, sizeof(CELL));
		for(j = 0; j < width; j++)
		{
			cells[i][j].y = (float) (i * CELL_SIZE);
			cells[i][j].x = (float) (j * CELL_SIZE);
			cells[i][j].alive = buf[j + i * hight];
		}
	}

	free(buf);
	return 0;
}

/*
	* Ler um inteiro dentro de um arquivo.
*/
int ReadInt(FILE *arch)
{
	char *buf;
	int i = 0, value = 0;

	buf = (char*) calloc(12, sizeof(char));
	do
	{
		buf[i] = getc(arch);
		i++;
	} while(buf[i] >= '0' && buf[i] <= '9');

	buf[i+1] = '\0';
	value = atoi(buf);
	free(buf);

	return value;
}












