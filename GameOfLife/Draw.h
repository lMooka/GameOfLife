/*
	CRIADO POR:
	
	Guilherme Silva de Souza
	guilherme.sdes@gmail.com / guilhermesouza@dcc.ufba.br
*/

#include <gl/glut.h>

typedef struct {
	float r;
	float g;
	float b;
} Color;

#ifndef DEFINED_MATRIX
#define DEFINED_MATRIX
int MATRIX_Y; // Altura da Matriz
int MATRIX_X; // Largura da Matriz
#endif

void InitProgram();
void InitDrawing();
void DrawSquare(int left, int right, int bottom, int top, Color *c);
void NewWindow(char *title, int hight, int width);
void Draw(void);
void ResizeWindow(GLsizei w, GLsizei h);
void DrawGrid();

