/*
	CRIADO POR:
	
	Guilherme Silva de Souza
	guilherme.sdes@gmail.com / guilhermesouza@dcc.ufba.br
*/

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "Biome.h"

/*
	* Inicializa o bioma com largura e altura passadas.
*/
void InitBiome(int width, int hight, int flag)
{
	int i, j;

	printf("inicializando Jogo da Vida...");

	MATRIX_Y = hight;
	MATRIX_X = width;
	
	callocCells(MATRIX_X, MATRIX_Y);
	// Inicializa��o das CELLs
	srand((unsigned int) time(NULL)); // necess�rio para o random
	
	for(i = 0; i < MATRIX_Y; i++)
	{
		for(j = 0; j < MATRIX_X; j++)
		{
			switch(flag)
			{
			case 0: // Celulas em branco
				cells[i][j].alive = 0;
				break;

			case 1:
				 // Celulas totalmente preenchidas
				cells[i][j].alive = 1;
				break;

			default:
				//Random de 0 a 1, com a % variando em cima do valor passado na flag.
				//(2 = 12,5% de chance)
				cells[i][j].alive = rand() % flag * rand() % flag * rand() % flag; 
				break;
			}
			cells[i][j].y = (float) (i * CELL_SIZE);
			cells[i][j].x = (float) (j * CELL_SIZE);
		}
	}
}


/*
	*Inicia o Game a partir de um arquivo.
*/
int InitBiomeFromFile(char *fname)
{
	FILE *file;
	char *buf, ch; // buffer
	int i = 0, j = 0; // vari�veis de loop
	
	if((file = fopen(fname, "r")) == NULL)
	{
		printf("\nArquivo n�o encontrado.\n");
		return 0;
	}

	printf("Carregando dados...");
	// Ler os 2 inteiros no inicio do arquivo

	buf = (char*) calloc(12, sizeof(char));

	for(i = 0; (buf[i] = getc(file)) != '\n'; i++);

	sscanf(buf, "%i %i", &MATRIX_X, &MATRIX_Y);
	printf(" %ix%i Elementos...\n", MATRIX_X, MATRIX_Y);

	// Ler toda a informa��o das c�lulas (0's e 1's).
	buf = (char*) calloc(MATRIX_Y * MATRIX_X, sizeof(char));

	for(i = 0; (ch = getc(file)) != EOF; i++)
	{
		if(ch == '\n')
		{
			i--; //Evita que o \n deixe uma posi��o vazia no buf
			continue;
		}
		
		buf[i] = ch;
		printf("%c", buf[i]);
	}

	fclose(file);

	printf("\n");

	//aloca mem�ria necess�ria
	callocCells(MATRIX_X, MATRIX_Y);
	
	// Passa a informa��o lida para a estrutura do programa.
	for(i = 0; i < MATRIX_Y; i++)
	{
		for(j = 0; j < MATRIX_X; j++)
		{
			//if(j == MATRIX_X) continue;
			cells[i][j].alive = (int) (buf[j + i * MATRIX_Y] - 48);
			cells[i][j].y = (float) (i * CELL_SIZE);
			cells[i][j].x = (float) (j * CELL_SIZE);
		}
	}

	return 1;
}


/*
	* Realiza todas as a��es e c�lculos necess�rios para gerar a pr�xima gera��o.

i) Qualquer c�lula viva com menos de dois vizinhos vivos morre de solid�o (torna-se vazia);
ii) Qualquer c�lula viva com mais de tr�s vizinhos vivos morre por superpopula��o;
iii) Qualquer c�lula vazia com exatamente tr�s vizinhos vivos se torna uma c�lula viva;
iv) Qualquer c�lula viva com dois ou tr�s vizinhos vivos continua no mesmo estado para a pr�xima gera��o.

*/
void PlayNextGeneration()
{
	int i, j;

	for(i = 0; i < MATRIX_Y; i++)
	{
		for(j = 0; j < MATRIX_X; j++)
			cells[i][j].CellsAround = CountCellsAround(i, j);
	}

	for(i = 0; i < MATRIX_Y; i++)
	{
		for(j = 0; j < MATRIX_X; j++)
		{
			if((cells[i][j].CellsAround < 2 || cells[i][j].CellsAround > 3) && cells[i][j].alive) UpdateCellLife(i, j, 0);
			if(cells[i][j].CellsAround == 3 && !cells[i][j].alive) UpdateCellLife(i, j, 1);
		}
	}
}


/*
	* CountCellAround(posi��o_altura, posi��o_largura);
	* Retorna a quantidade de c�lulas vizinhas.
*/
int CountCellsAround(int y, int x)
{
	int i, j, tx = 0, ty = 0, count = 0;

	for(i = 0, ty = (y - 1); i < 3; i++, ty++)
	{
		for(j = 0, tx = (x - 1); j < 3; j++, tx++)
		{
			if(tx < 0 || ty < 0 || tx >= MATRIX_X || ty >= MATRIX_Y || ty == y && tx == x) continue;
			if(cells[ty][tx].alive) count++;
		}
	}

	return count;
}

/*
	* Define o valor de uma c�lula.
*/
void UpdateCellLife(int y, int x, int value)
{
	cells[y][x].alive = value;
}



//*********************** MANIPULA��O DE MEM�RIA ***********************/

/*
	* Aloca mem�ria necess�ria para **cells.
*/
void callocCells(int width, int hight)
{
	int i;

	cells = (CELL**) calloc(hight, sizeof(CELL*));

	for(i = 0; i < MATRIX_Y; i++)
		cells[i] = (CELL*) calloc(width, sizeof(CELL));
}
