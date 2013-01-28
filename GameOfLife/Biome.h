/*
	CRIADO POR:
	
	Guilherme Silva de Souza
	guilherme.sdes@gmail.com / guilhermesouza@dcc.ufba.br
*/

#define CELL_SIZE 10

typedef struct
{
	float x, y;
	int alive;
	int CellsAround;
} CELL;

#ifndef DEFINED_CELLS
#define DEFINED_CELLS

CELL **cells;
int MATRIX_Y, MATRIX_X;

#endif

int CountCellsAround(int x, int y);
int InitBiomeFromFile(char *fileName);
void InitBiome(int mx, int my, int flag);
void UpdateCellLife(int y, int x, int value);
void callocCells(int width, int hight);
void PlayNextGeneration();

