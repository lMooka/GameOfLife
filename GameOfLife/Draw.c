/*
	CRIADO POR:
	
	Guilherme Silva de Souza
	guilherme.sdes@gmail.com / guilhermesouza@dcc.ufba.br
*/

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
#include "Draw.h"
#include "Biome.h"



GLsizei W_HIGHT;
GLsizei W_WIDTH;

GLsizei GRID_WIDTH;
GLsizei GRID_HIGHT;

int isPlaying = 0;
int Ticks = 0, speed = 2;

/**************************** FUNÇÕES DE CALLBACK ********************************/

void Draw(void)
{
	int i, j;
	Color *color = (Color*) malloc(sizeof(Color));

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glClear(GL_COLOR_BUFFER_BIT); // Limpa a tela
	//glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

	for(i = 0; i < MATRIX_Y; i++) 
	{
		for(j = 0; j < MATRIX_X; j++)
		{
			if(cells[i][j].alive) 
			{
				color->r = 0.0f;
				color->g = 0.0f;
				color->b = 0.9f;
			}
			else
			{
				color->r = 1.0f;
				color->g = 1.0f;
				color->b = 1.0f;
			}
			DrawSquare(cells[i][j].y, cells[i][j].y + CELL_SIZE, cells[i][j].x, cells[i][j].x + CELL_SIZE, color);
		}
	}
	DrawGrid();

	// Executa comandos OpenGL
	glutSwapBuffers();
}

/* 
* Função de callback responsável por ajustar a janela e recalcular o tamanho do grid.
*/
void ResizeWindow(GLsizei w, GLsizei h)
{
	float wf, hf;

	W_HIGHT = h;
	W_WIDTH = w;

	// Evita a divisao por zero
	if(h == 0) h = 1;
	if(w == 0) w = 1;

	hf = h;
	wf = w;

	// Inicializa o sistema de coordenadas
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	//gluOrtho2D(GLdouble left, GLdouble right, GLdouble bottom, GLdouble top);
	if(w >= h)
	{
		glViewport(1, 1, (int) ((float) w * (wf/hf)) - 3, h - 3);
		gluOrtho2D((double) 0, (double) ((float) w * (wf/hf)), (double)  0, (double)  h);
	}
	else
	{
		glViewport(1, 1, (int) ((float) w * (hf/wf)) - 3, h - 3);
		gluOrtho2D((double) 0, (double) ((float) w * (hf/wf)), (double)  0, (double)  h);
	}
	glutPostRedisplay();
}


/*
* Função de callback responsável pelos comandos do mouse.
*/
void MouseEvent(int button, int state, int x, int y)
{
	// Cálculo para corrigir as diferentes coordenadas da matriz e as passadas pelo click do mouse.
	int lx = (x / CELL_SIZE);
	int ly = (W_HIGHT/CELL_SIZE) - (y / CELL_SIZE) - 1;

	// Evita duplo click
	if(state == GLUT_UP) return;

	// Inverte o status de vida da célula na posição clicada
	if(button == GLUT_LEFT_BUTTON)
	{
		if(lx >= 0 && lx < MATRIX_X && ly >= 0 && ly < MATRIX_Y)
		{
			//printf("[%i,%i] { ", ly, lx);
			//CountCellsAround(ly, lx);
			//printf("\n};\n\n");
			if(cells[ly][lx].alive) UpdateCellLife(ly, lx, 0);
			else UpdateCellLife(ly, lx, 1);
			//printf("[%i, %i]%i:%i\t", ly, lx, cells[ly][lx].CellsAround, cells[ly][lx].alive);
		}
	}

	// Pausa ou despausa o jogo
	if(button == GLUT_RIGHT_BUTTON)
	{
		if(isPlaying) isPlaying = 0; else isPlaying = 1;
	}
}


/*
* Função de callback responsável pelos comandos do teclado.
*/
void KeyboardEvent(unsigned char key, int x, int y)
{
	int i, j, flag;

	switch(key)
	{
	case '0': // Zera todos elementos alive de cells[][].alive
		for(i = 0; i < MATRIX_Y; i++)
		{
			for(j = 0; j < MATRIX_X; j++)
			{
				cells[i][j].alive = 0;
			}
		}
		break;

	case '1': // Define 1 em todos elementos cells[][].alive
		for(i = 0; i < MATRIX_Y; i++)
		{
			for(j = 0; j < MATRIX_X; j++)
			{
				cells[i][j].alive = 1;
			}
		}
		break;

	default: // Randomiza todos elementos de cells[][].alive
		flag = key - 48;
		if(flag < '2' && flag > '9') return;

		srand((unsigned int) time(NULL));
		for(i = 0; i < MATRIX_Y; i++)
		{
			for(j = 0; j < MATRIX_X; j++)
			{
				cells[i][j].alive = rand() % flag * rand() % flag * rand() % flag;
			}
		}
		break;

	case ' ': // Realiza uma geração
		PlayNextGeneration();
		break;

	case '+': // Aumente velocidade em que as gerações acontecem
		speed++;
		Ticks = 1000 / speed;
		break;

	case '-': // Diminui velocidade em que as gerações acontecem
		if(--speed < 1) speed = 1;
		Ticks = 1000 / speed;
		break;
	}

}

/*
* Função de callback responsável pela animação.
*/
void Timer(int value)
{
	if(isPlaying) PlayNextGeneration();
	glutPostRedisplay();
	glutTimerFunc(Ticks, Timer, 1);
}

/**************************** FIM FUNÇÕES DE CALLBACK ********************************/


/*
* Desenha um quadrado na janela no local e cor passados.
*/
void DrawSquare(int left, int right, int bottom, int top, Color *c)
{
	// Desenha um quadrado preenchido com a cor corrente
	glBegin(GL_QUADS);
	glColor3f(c->r, c->g, c->b);
	glVertex2i(top, left);
	glVertex2i(top, right);
	glVertex2i(bottom, right);
	glVertex2i(bottom, left);
	glEnd();
}

/*
* Desenha uma grade na tela (Grid) com base no tamanho inicial da janela.
*/
void DrawGrid()
{
	int i, j;

	//Linhas vertical
	for(j = 0; j < MATRIX_X; j++)
	{
		glBegin(GL_LINES);
		glColor3f(0.5f, 0.5f, 0.5f);
		glVertex2i(j * CELL_SIZE, 0);
		glVertex2i(j * CELL_SIZE, GRID_HIGHT);
		glEnd();
	}

	//Linhas horizontal
	for(i = 0; i < MATRIX_Y; i++)
	{
		glBegin(GL_LINES);
		glColor3f(0.5f, 0.5f, 0.5f);
		glVertex2i(0 , i * CELL_SIZE);
		glVertex2i(GRID_WIDTH, i * CELL_SIZE);
		glEnd();
	}

	glBegin(GL_LINE_LOOP);
	glColor3f(0.0f, 0.0f, 0.9f);
	glVertex2i(0,0);
	glVertex2i(GRID_WIDTH, 1);
	glVertex2i(GRID_WIDTH, GRID_HIGHT);
	glVertex2i(1, GRID_HIGHT);
	glEnd();
}

/*
* Função que inicializa tudo que é necessário para desenhar.
*/
void InitDrawing()
{
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
}


/*
* Cria a Janela princial de desenho.
* Define nome da janela, seu tamanho e sua posição inicial.
* Define pela primeira vez o W_HIGHT e W_WIDTH para uso posterior na função Draw.
*/
void NewWindow(char *title, int width, int hight)
{
	printf("Inicializando a janela...\n");
	// Passa os valores para uma variável global
	GRID_WIDTH = width;
	GRID_HIGHT = hight;
	W_WIDTH = width;
	W_HIGHT = hight;

	glutInitDisplayMode(GLUT_DOUBLE| GLUT_RGB);
	glutInitWindowSize(width, hight); // matrix_y e matrix_x inicial
	glutInitWindowPosition(1000, 270); // Posição inicial
	glutCreateWindow(title); // Nome da janela
	glutDisplayFunc(Draw); // Difine função callback de desenho
	glutMouseFunc(MouseEvent); // Define função de callback para o mouse
	glutKeyboardFunc(KeyboardEvent);
	glutTimerFunc(Ticks, Timer, 1); // Define funcção de callback para animação
	glutReshapeFunc(ResizeWindow); // Define função callback de redimensionamento
}

void InitProgram()
{

	int width = 0, hight = 0, flag = 0;
	char command[10];
	char console_input[100];

	printf("[Jogo da Vida]\n");
	printf("Bem vindo ao Jogo da Vida!\n\n");
	printf("[Criado por: Guilherme Silva de Souza, Sistemas de Informacao, UFBA.]\n");
	printf("[guilherme.sdes@gmail.com, guilhermesouza@dcc.ufba.br]\n\n");
	printf("(digite 'help' para receber ajuda)\n\n");
	while(1)
	{
		printf("@");
		gets(console_input);
		sscanf(console_input, "%s", &command);

		if(!strcmp(command, "loadfile"))
		{
			char fileName[20];
			sscanf(console_input, "%s %s", &command, &fileName);
			printf("/%s ", command, fileName);
			speed = 2;
			Ticks = 1000 / speed;
			if(!InitBiomeFromFile(fileName)) continue;
			NewWindow("Bioma", MATRIX_X * CELL_SIZE, MATRIX_Y * CELL_SIZE);
			break;
		} 
		else if(!strcmp(command, "start"))
		{
			sscanf(console_input, "%s %i %i %i %i", &command, &width, &hight, &flag, &speed);
			//Evita divisão por 0 e estabelece velocidade mínima.
			if(speed < 1) speed = 1;
			//Define o tempo em que a função Timer é chamada (milisegundos).
			Ticks = 1000 / speed;
			// Inicia as estruturas de dados das células
			InitBiome(width, hight, flag);
			// Cria uma janela com nome e dimensões passadas
			NewWindow("Bioma", width * CELL_SIZE, hight * CELL_SIZE);
			break;
		} 
		else if(!strcmp(command, "help"))
		{
			printf("---- COMANDOS ----\n");
			printf("@loadfile [NomeDoArquivo]\n\n");
			printf("@start [Largura] [Altura] [RandomFlag] [CiclosPorSegundo]\n");
			printf("\n[RandomFlag]\t0 : Matriz vazia (Todas as celulas mortas); \n\t\t1 : Matriz cheia (Completamente viva); \n\t\t2+: Matriz gerada aleatoriamente;");
			printf("\n\n\n--- HOTKEYS ---\n");
			printf("[0~9]: Recriar game com [RandomFlag] equivalente ao numero pressionado.\n");
			printf("[+/-]: Acelera ou reduz a velocidade em que as geracoes acontecem.\n");
			printf("[LMB]: Adicionar ou remover celula do game. (clicar na grade do jogo)\n");
			printf("[RMB]: Pausar/despausar game.\n");
			printf("[Space]: Calcula a proxima geracao. (game pausado)\n");
			printf("\n\n");
			continue;
		}
		else
		{
			printf("Commando desconhecido.\n");
			continue;
		}
	}

	// Define configurações necessárias antes de começar a desenhar
	InitDrawing();
	// Inicia o Loop de desenho.
	// Permenece nesta função até a finalização do programa,
	// retornando somente nas funções de callback.
	glutMainLoop(); 
}

