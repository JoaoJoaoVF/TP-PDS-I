#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <string.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>

#define NAVEGACAO 0
#define BATALHA 1

#define TAM_HEROI 20
#define PASSO 30

#define TAM_OBJETIVO 40
#define MAX_DIST 20

#define TAM 20

const float FPS = 10;

const int SCREEN_W = 1280;
const int SCREEN_H = 720;

int danoheroi = 10;
int danomonstro = 0;

int X_OPCOES;
int Y_OPCOES;

ALLEGRO_FONT *FONTE;

typedef struct Heroi
{
	int x, y;
	ALLEGRO_COLOR cor;
	int vida;
	int pontos;
	int posicao;
	//BATALHA
	int acao;
	int executar;
	int x_old, y_old;
	int ataque;
} Heroi;

typedef struct Monstro
{
	int x, y;
	ALLEGRO_COLOR cor;
	int vida;
	int ataque;
	int tipo;
} Monstro;

//menu de batalha
typedef struct Menu
{
	int opcao;
	int mudou;
	int confirma;
} Menu;

//inicia as variaveis globais
void iniciaGlobais()
{
	X_OPCOES = 3 * SCREEN_W / 4;
	Y_OPCOES = 3 * SCREEN_H / 4;

	int tam_fonte = 23;
	if (SCREEN_H < 300)
	{
		tam_fonte = TAM;
	}
	//carrega o arquivo arial.ttf da fonte Arial e define que sera usado o tamanho 32 (segundo parametro)
	FONTE = al_load_font("arial.ttf", tam_fonte, 1);
	if (FONTE == NULL)
	{
		fprintf(stderr, "font file does not exist or cannot be accessed!\n");
	}
}
//==================== INICIALIZAÇÃO ===============================================
//inicializou o heroi
void initHeroi(Heroi *h)
{
	h->x = 1;
	h->y = SCREEN_H - (2 * TAM_HEROI);
	h->cor = al_map_rgb(255, 1, 1);
	h->vida = 450 + (rand() % 100);
	h->acao = 0;
	h->executar = 0;
	h->x_old = h->x;
	h->y_old = h->y;
	h->pontos = 0;
	h->posicao = 1;
}

//inicializou o monstro
void initMonstro(Monstro *m, int a)
{
	do
	{
		m->x = (rand() % SCREEN_W) + 30;
	} while (m->x > 1220 || m->x < 20 && m->x == 0);
	do
	{

		m->y = (rand() % SCREEN_H) + 30;
	} while (m->y > 700 || m->y < 20 && m->y == 0);
	printf("%d:(%d, %d)\n", a, m->x, m->y);
	m->cor = al_map_rgb(0, 0, 0);
	m->vida = 100 + (rand() % 100);
	m->tipo = 0;
}

//==================== NAVEGACAO ===============================================

//desenhou o heroi na navegacao
void desenhaHeroiNavegacao(Heroi *h)
{
	if (h->posicao != 0)
	{
		if (h->posicao == 1)
		{
			ALLEGRO_BITMAP *image = al_load_bitmap("heroi1.png");
			al_draw_bitmap(image, h->x, h->y, 5);
			al_destroy_bitmap(image);
		}
		else if (h->posicao == 2)
		{
			ALLEGRO_BITMAP *image = al_load_bitmap("heroi2.png");
			al_draw_bitmap(image, h->x, h->y, 0);
			al_destroy_bitmap(image);
		}
		else if (h->posicao == 3)
		{
			ALLEGRO_BITMAP *image = al_load_bitmap("heroi3.png");
			al_draw_bitmap(image, h->x, h->y, 0);
			al_destroy_bitmap(image);
		}
		else if (h->posicao == 4)
		{
			ALLEGRO_BITMAP *image = al_load_bitmap("heroi4.png");
			al_draw_bitmap(image, h->x, h->y, 0);
			al_destroy_bitmap(image);
		}
	}
}

//desenhou o cenario de navegacao
void desenhaCenarioNavegacao(Heroi *h)
{

	ALLEGRO_BITMAP *image = al_load_bitmap("navegacao.png");
	al_draw_bitmap(image, 0, 0, 0);
	char my_text3[TAM];
	sprintf(my_text3, "%d", h->pontos);
	al_draw_text(FONTE, al_map_rgb(255, 0, 30), 10, 10, 0, my_text3);
	al_destroy_bitmap(image);
}

//processa o heroi andando
void processaTeclaNavegacao(Heroi *h, int tecla)
{

	h->x_old = h->x;
	h->y_old = h->y;
	int pont3 = 0;
	char linha[TAM];
	FILE *arq4 = NULL;

	switch (tecla)
	{
	case ALLEGRO_KEY_UP:
		if (h->y >= -20)
		{
			h->posicao = 1;
			h->y -= PASSO;

			if (h->y + PASSO <= -5)
			{
				h->y = SCREEN_H - TAM_HEROI;
			}
		}
		break;

	case ALLEGRO_KEY_DOWN:
		if (h->y + TAM_HEROI + PASSO <= SCREEN_H + 60)
		{
			h->posicao = 2;
			h->y += PASSO;

			if (h->y >= SCREEN_H + 10)
			{
				h->y = -5;
			}
		}
		break;

	case ALLEGRO_KEY_LEFT:
		if (h->x - PASSO >= -40)
		{
			h->posicao = 4;
			h->x -= PASSO;

			if (h->x <= -25)
			{
				h->x = SCREEN_W - 10;
			}
		}
		break;

	case ALLEGRO_KEY_RIGHT:
		if (h->x + TAM_HEROI + PASSO <= SCREEN_W + TAM_HEROI + PASSO)
		{
			h->posicao = 3;
			h->x += PASSO;

			if (h->x >= SCREEN_W + 10)
			{
				h->x = -10;
			}
		}
		break;

	case ALLEGRO_KEY_R:
		arq4 = fopen("Maxpontos.txt", "r");
		while (!feof(arq4))
		{
			fgets(linha, TAM, arq4);
			pont3 = atoi(strtok(linha, "\0"));
			//printf("\nlinha 894\n%d\n", pont3);
			break;
		}
		h->pontos = pont3;
		fclose(arq4);
		break;
	}
}

//processa se chegou no destino
int chegouObjetivo(Heroi h)
{
	if (h.x >= SCREEN_W - TAM_OBJETIVO && h.y + TAM_HEROI <= TAM_OBJETIVO)
	{
		return 1;
	}
	return 0;
}

//determina a distancia entre heroi e monstro4
float dist(int x1, int y1, int x2, int y2)
{
	return sqrt(pow(x1 - x2, 2) + pow(y1 - y2, 2));
}

//determina se heroi e monstro colidiram
int detectouMonstro(Heroi h, Monstro m)
{
	if (dist(h.x, h.y, m.x, m.y) <= MAX_DIST)
	{
		return 1;
	}
	return 0;
}
//==================== BATALHA ===============================================

//desenhou o monstro na batallha
void desenhaBatalha1(Menu *menu, Heroi h, Monstro *m)
{
	m->tipo = 1;
	ALLEGRO_BITMAP *image2 = al_load_bitmap("batalha.png");
	al_draw_bitmap(image2, 0, 0, 0);
	char my_text3[TAM];
	sprintf(my_text3, "%d", h.pontos);
	al_draw_text(FONTE, al_map_rgb(255, 0, 0), 10, 10, 0, my_text3);
	al_draw_filled_rectangle(960, 135, 1020, 170, al_map_rgb(255, 255, 255));
	al_draw_rectangle(961, 136, 1021, 171, al_map_rgb(0, 0, 0), 1);
	al_draw_filled_rectangle(305, 135, 365, 170, al_map_rgb(255, 255, 255));
	al_draw_rectangle(306, 136, 366, 171, al_map_rgb(0, 0, 0), 1);
	al_draw_filled_rectangle(X_OPCOES, Y_OPCOES + 40, SCREEN_W, SCREEN_H, al_map_rgb(100, 10, 100));
	al_draw_rectangle(X_OPCOES + 1, Y_OPCOES + 41, SCREEN_W + 1, SCREEN_H, al_map_rgb(255, 255, 255), 5);
	al_draw_text(FONTE, al_map_rgb(255, 255, 255), X_OPCOES + 10, Y_OPCOES + 50, 0, "ATACAR");
	al_draw_text(FONTE, al_map_rgb(255, 255, 255), X_OPCOES + 10, Y_OPCOES + 95, 0, "ESPECIAL");
	al_draw_text(FONTE, al_map_rgb(255, 255, 255), X_OPCOES + 10, Y_OPCOES + 140, 0, "FUGIR");
	al_draw_filled_circle(X_OPCOES + 200, Y_OPCOES + 60 + menu->mudou, 5, al_map_rgb(255, 0, 0));
	char my_text4[TAM];
	sprintf(my_text4, "%d", h.vida);
	al_draw_text(FONTE, h.cor, 970, 140, 0, my_text4);
	char my_text5[TAM];
	sprintf(my_text5, "%d", m->vida);
	al_draw_text(FONTE, m->cor, 315, 140, 0, my_text5);
	al_destroy_bitmap(image2);
}

void desenhaBatalha2(Menu *menu, Heroi *h, Monstro *m)
{
	m->tipo = 2;

	ALLEGRO_BITMAP *image2 = al_load_bitmap("batalha2.png");
	al_draw_bitmap(image2, 0, 0, 0);
	char my_text3[TAM];
	sprintf(my_text3, "%d", h->pontos);
	al_draw_text(FONTE, al_map_rgb(255, 0, 0), 10, 10, 0, my_text3);
	al_draw_filled_rectangle(960, 135, 1020, 170, al_map_rgb(255, 255, 255));
	al_draw_rectangle(961, 136, 1021, 171, al_map_rgb(0, 0, 0), 1);
	al_draw_filled_rectangle(305, 135, 365, 170, al_map_rgb(255, 255, 255));
	al_draw_rectangle(306, 136, 366, 171, al_map_rgb(0, 0, 0), 1);
	al_draw_filled_rectangle(X_OPCOES, Y_OPCOES + 40, SCREEN_W, SCREEN_H, al_map_rgb(100, 10, 100));
	al_draw_rectangle(X_OPCOES + 1, Y_OPCOES + 41, SCREEN_W + 1, SCREEN_H, al_map_rgb(255, 255, 255), 5);
	al_draw_text(FONTE, al_map_rgb(255, 255, 255), X_OPCOES + 10, Y_OPCOES + 50, 0, "ATACAR");
	al_draw_text(FONTE, al_map_rgb(255, 255, 255), X_OPCOES + 10, Y_OPCOES + 95, 0, "ESPECIAL");
	al_draw_text(FONTE, al_map_rgb(255, 255, 255), X_OPCOES + 10, Y_OPCOES + 140, 0, "FUGIR");
	al_draw_filled_circle(X_OPCOES + 200, Y_OPCOES + 60 + menu->mudou, 5, al_map_rgb(255, 0, 0));
	char my_text4[TAM];
	sprintf(my_text4, "%d", h->vida);
	al_draw_text(FONTE, h->cor, 970, 140, 0, my_text4);
	char my_text5[TAM];
	sprintf(my_text5, "%d", m->vida);
	al_draw_text(FONTE, m->cor, 315, 140, 0, my_text5);
	al_destroy_bitmap(image2);
}

void desenhaBatalha3(Menu *menu, Heroi h, Monstro *m)
{
	m->tipo = 3;
	ALLEGRO_BITMAP *image2 = al_load_bitmap("batalha3.png");
	al_draw_bitmap(image2, 0, 0, 0);
	char my_text3[TAM];
	sprintf(my_text3, "%d", h.pontos);
	al_draw_text(FONTE, al_map_rgb(255, 0, 0), 10, 10, 0, my_text3);
	al_draw_filled_rectangle(960, 135, 1020, 170, al_map_rgb(255, 255, 255));
	al_draw_rectangle(961, 136, 1021, 171, al_map_rgb(0, 0, 0), 1);
	al_draw_filled_rectangle(305, 135, 365, 170, al_map_rgb(255, 255, 255));
	al_draw_rectangle(306, 136, 366, 171, al_map_rgb(0, 0, 0), 1);
	al_draw_filled_rectangle(X_OPCOES, Y_OPCOES + 40, SCREEN_W, SCREEN_H, al_map_rgb(100, 10, 100));
	al_draw_rectangle(X_OPCOES + 1, Y_OPCOES + 41, SCREEN_W + 1, SCREEN_H, al_map_rgb(255, 255, 255), 5);
	al_draw_text(FONTE, al_map_rgb(255, 255, 255), X_OPCOES + 10, Y_OPCOES + 50, 0, "ATACAR");
	al_draw_text(FONTE, al_map_rgb(255, 255, 255), X_OPCOES + 10, Y_OPCOES + 95, 0, "ESPECIAL");
	al_draw_text(FONTE, al_map_rgb(255, 255, 255), X_OPCOES + 10, Y_OPCOES + 140, 0, "FUGIR");
	al_draw_filled_circle(X_OPCOES + 200, Y_OPCOES + 60 + menu->mudou, 5, al_map_rgb(255, 0, 0));
	char my_text4[TAM];
	sprintf(my_text4, "%d", h.vida);
	al_draw_text(FONTE, h.cor, 970, 140, 0, my_text4);
	char my_text5[TAM];
	sprintf(my_text5, "%d", m->vida);
	al_draw_text(FONTE, m->cor, 315, 140, 0, my_text5);
	al_destroy_bitmap(image2);
}

void desenhaBatalha4(Menu *menu, Heroi *h, Monstro *m)
{
	m->tipo = 4;
	ALLEGRO_BITMAP *image2 = al_load_bitmap("batalha4.png");
	al_draw_bitmap(image2, 0, 0, 0);
	char my_text3[TAM];
	sprintf(my_text3, "%d", h->pontos);
	al_draw_text(FONTE, al_map_rgb(255, 0, 0), 10, 10, 0, my_text3);
	al_draw_filled_rectangle(960, 135, 1020, 170, al_map_rgb(255, 255, 255));
	al_draw_rectangle(961, 136, 1021, 171, al_map_rgb(0, 0, 0), 1);
	al_draw_filled_rectangle(305, 135, 365, 170, al_map_rgb(255, 255, 255));
	al_draw_rectangle(306, 136, 366, 171, al_map_rgb(0, 0, 0), 1);
	al_draw_filled_rectangle(X_OPCOES, Y_OPCOES + 40, SCREEN_W, SCREEN_H, al_map_rgb(100, 10, 100));
	al_draw_rectangle(X_OPCOES + 1, Y_OPCOES + 41, SCREEN_W + 1, SCREEN_H, al_map_rgb(255, 255, 255), 5);
	al_draw_text(FONTE, al_map_rgb(255, 255, 255), X_OPCOES + 10, Y_OPCOES + 50, 0, "ATACAR");
	al_draw_text(FONTE, al_map_rgb(255, 255, 255), X_OPCOES + 10, Y_OPCOES + 95, 0, "ESPECIAL");
	al_draw_text(FONTE, al_map_rgb(255, 255, 255), X_OPCOES + 10, Y_OPCOES + 140, 0, "FUGIR");
	al_draw_filled_circle(X_OPCOES + 200, Y_OPCOES + 60 + menu->mudou, 5, al_map_rgb(255, 0, 0));
	char my_text4[TAM];
	sprintf(my_text4, "%d", h->vida);
	al_draw_text(FONTE, h->cor, 970, 140, 0, my_text4);
	char my_text5[TAM];
	sprintf(my_text5, "%d", m->vida);
	al_draw_text(FONTE, m->cor, 315, 140, 0, my_text5);
	al_destroy_bitmap(image2);
}

void desenhaBatalha5(Menu *menu, Heroi *h, Monstro *m)
{
	m->tipo = 5;
	ALLEGRO_BITMAP *image2 = al_load_bitmap("batalha5.png");
	al_draw_bitmap(image2, 0, 0, 0);
	char my_text3[TAM];
	sprintf(my_text3, "%d", h->pontos);
	al_draw_text(FONTE, al_map_rgb(255, 0, 0), 10, 10, 0, my_text3);
	al_draw_filled_rectangle(960, 135, 1020, 170, al_map_rgb(255, 255, 255));
	al_draw_rectangle(961, 136, 1021, 171, al_map_rgb(0, 0, 0), 1);
	al_draw_filled_rectangle(305, 135, 365, 170, al_map_rgb(255, 255, 255));
	al_draw_rectangle(306, 136, 366, 171, al_map_rgb(0, 0, 0), 1);
	al_draw_filled_rectangle(X_OPCOES, Y_OPCOES + 40, SCREEN_W, SCREEN_H, al_map_rgb(100, 10, 100));
	al_draw_rectangle(X_OPCOES + 1, Y_OPCOES + 41, SCREEN_W + 1, SCREEN_H, al_map_rgb(255, 255, 255), 5);
	al_draw_text(FONTE, al_map_rgb(255, 255, 255), X_OPCOES + 10, Y_OPCOES + 50, 0, "ATACAR");
	al_draw_text(FONTE, al_map_rgb(255, 255, 255), X_OPCOES + 10, Y_OPCOES + 95, 0, "ESPECIAL");
	al_draw_text(FONTE, al_map_rgb(255, 255, 255), X_OPCOES + 10, Y_OPCOES + 140, 0, "FUGIR");
	al_draw_filled_circle(X_OPCOES + 200, Y_OPCOES + 60 + menu->mudou, 5, al_map_rgb(255, 0, 0));
	char my_text4[TAM];
	sprintf(my_text4, "%d", h->vida);
	al_draw_text(FONTE, h->cor, 970, 140, 0, my_text4);
	char my_text5[TAM];
	sprintf(my_text5, "%d", m->vida);
	al_draw_text(FONTE, m->cor, 315, 140, 0, my_text5);
	al_destroy_bitmap(image2);
}

//inicia as opcoes do menu de batalha
void initMenu(Menu *menu)
{
	menu->opcao = 0;
	menu->mudou = 0;
	menu->confirma = 0;
}

//processa as teclas na batalha
void processaTeclaBatalha(Heroi *h, int tecla, Menu *menu)
{
	int pont4 = 0;
	char linha[TAM];
	FILE *arq5 = NULL;

	switch (tecla)
	{

	//comando de ataque
	case ALLEGRO_KEY_J:
		menu->opcao = 1;
		menu->mudou = 0;
		break;

		//comando de especial
	case ALLEGRO_KEY_K:
		menu->opcao = 2;
		menu->mudou = 46;
		break;

		//comando de fugir
	case ALLEGRO_KEY_L:
		menu->opcao = 3;
		menu->mudou = 90;
		break;

	case ALLEGRO_KEY_R:
		arq5 = fopen("Maxpontos.txt", "r");
		while (!feof(arq5))
		{
			fgets(linha, TAM, arq5);
			pont4 = atoi(strtok(linha, "\0"));
			//printf("\nlinha 894\n%d\n", pont3);
			break;
		}
		h->pontos = pont4;
		fclose(arq5);
		break;

	case ALLEGRO_KEY_ENTER:
		h->executar = 1;
		break;
	}
	if (h->executar == 1)
	{
		if (menu->opcao == 1)
		{
			h->acao = 10;
		}
		else if (menu->opcao == 2)
		{
			h->acao = 20;
		}
		else if (menu->opcao == 3)
		{
			h->acao = 30;
		}
		h->executar = 0;
	}
}

//define a quantidade de dano que o heroi causou
int ataqueHeroi(Heroi *h)
{

	danoheroi = ((rand() % 5) * 12) + 5;
	h->pontos += danoheroi;
	return danoheroi;
}

//define a quantidade de dano que o monstro causou
int ataqueMonstro(Monstro *m)
{
	danomonstro = ((rand() % 5) * 10) + 3;

	return danomonstro;
}

//define a quantidade de dano especial que o heroi causou
int especialHeroi(Heroi *h)
{

	danoheroi = ((rand() % 5) * 10 + (rand() % 5) * 10) + 20;
	h->pontos += danoheroi;
	return danoheroi;
}

int fuga()
{
	return rand() % 100;
}

void animacaoAtaqueHeroi(Heroi *h, Monstro *m)
{
	int a = 0, b = 0;
	a = 1;
	bool redraw = true;
	ALLEGRO_BITMAP *ataqueh = NULL;
	ALLEGRO_BITMAP *image2 = NULL;
	ataqueh = al_load_bitmap("ataqueHeroi.png");
	int largura_ataqueh = al_get_bitmap_width(ataqueh);
	int altura_ataqueh = al_get_bitmap_height(ataqueh);
	//posicao x e y inicial do passaro na tela
	int ataqueh_x = 1000;
	int ataqueh_y = 400;

	int ataqueh_dx = 4, ataqueh_dy = 0;
	while (ataqueh_x != 300)
	{
		if (a == 1)
		{
			//incrementa as posicoes x e y do passaro com o seu deslocamento dx e dy
			ataqueh_x -= ataqueh_dx;
			ataqueh_y += ataqueh_dy;
			//como eu movi o passaro, preciso redesenhar ele (remova essa linha e veja o que acontece)
			redraw = true;
			b = 1;
		}
		if (b == 1)
		{
			if (m->tipo == 1)
			{
				image2 = al_load_bitmap("batalha.png");
				al_draw_bitmap(image2, 0, 0, 0);
			}
			else if (m->tipo == 2)
			{
				image2 = al_load_bitmap("batalha2.png");
				al_draw_bitmap(image2, 0, 0, 0);
			}
			else if (m->tipo == 3)
			{
				image2 = al_load_bitmap("batalha3.png");
				al_draw_bitmap(image2, 0, 0, 0);
			}
			else if (m->tipo == 4)
			{
				image2 = al_load_bitmap("batalha4.png");
				al_draw_bitmap(image2, 0, 0, 0);
			}
			else if (m->tipo == 5)
			{
				image2 = al_load_bitmap("batalha5.png");
				al_draw_bitmap(image2, 0, 0, 0);
			}
			al_draw_filled_rectangle(960, 135, 1020, 170, al_map_rgb(255, 255, 255));
			al_draw_rectangle(961, 136, 1021, 171, al_map_rgb(0, 0, 0), 1);
			al_draw_filled_rectangle(305, 135, 365, 170, al_map_rgb(255, 255, 255));
			al_draw_rectangle(306, 136, 366, 171, al_map_rgb(0, 0, 0), 1);

			char my_text4[TAM];
			sprintf(my_text4, "%d", h->vida);
			al_draw_text(FONTE, h->cor, 970, 140, 0, my_text4);
			char my_text5[TAM];
			sprintf(my_text5, "%d", m->vida);
			al_draw_text(FONTE, m->cor, 315, 140, 0, my_text5);
			al_draw_bitmap(ataqueh, ataqueh_x, ataqueh_y, 0);
			//dou um refresh na tela
			al_flip_display();
			al_destroy_bitmap(image2);
		}
		if (ataqueh_x == 300)
		{

			break;
		}
	}
	al_destroy_bitmap(ataqueh);
}
void animacaoAtaqueMonstro(Heroi *h, Monstro *m)
{
	int a = 0, b = 0;
	a = 1;
	bool redraw = true;
	ALLEGRO_BITMAP *ataquem = NULL;
	ALLEGRO_BITMAP *image2 = NULL;
	int ataquem_x = 300;
	int ataquem_y = 400;

	int ataquem_dx = 4, ataquem_dy = 0;
	while (ataquem_x != 1000)
	{
		if (a == 1)
		{
			//incrementa as posicoes x e y do passaro com o seu deslocamento dx e dy
			ataquem_x += ataquem_dx;
			ataquem_y += ataquem_dy;
			//como eu movi o passaro, preciso redesenhar ele (remova essa linha e veja o que acontece)
			redraw = true;
			b = 1;
		}
		if (b == 1)
		{
			if (m->tipo == 1)
			{
				image2 = al_load_bitmap("batalha.png");
				al_draw_bitmap(image2, 0, 0, 0);
				ataquem = al_load_bitmap("ataqueMonstro.png");
				int largura_ataquem = al_get_bitmap_width(ataquem);
				int altura_ataquem = al_get_bitmap_height(ataquem);
			}
			else if (m->tipo == 2)
			{
				image2 = al_load_bitmap("batalha2.png");
				al_draw_bitmap(image2, 0, 0, 0);
				ataquem = al_load_bitmap("ataqueMonstro.png");
				int largura_ataquem = al_get_bitmap_width(ataquem);
				int altura_ataquem = al_get_bitmap_height(ataquem);
			}
			else if (m->tipo == 3)
			{
				image2 = al_load_bitmap("batalha3.png");
				al_draw_bitmap(image2, 0, 0, 0);
				ataquem = al_load_bitmap("ataqueMonstro.png");
				int largura_ataquem = al_get_bitmap_width(ataquem);
				int altura_ataquem = al_get_bitmap_height(ataquem);
			}
			else if (m->tipo == 4)
			{
				image2 = al_load_bitmap("batalha4.png");
				al_draw_bitmap(image2, 0, 0, 0);
				ataquem = al_load_bitmap("ataqueMonstro.png");
				int largura_ataquem = al_get_bitmap_width(ataquem);
				int altura_ataquem = al_get_bitmap_height(ataquem);
			}
			else if (m->tipo == 5)
			{
				image2 = al_load_bitmap("batalha5.png");
				al_draw_bitmap(image2, 0, 0, 0);
				ataquem = al_load_bitmap("ataqueMonstro5.png");
				int largura_ataquem = al_get_bitmap_width(ataquem);
				int altura_ataquem = al_get_bitmap_height(ataquem);
			}
			al_draw_filled_rectangle(960, 135, 1020, 170, al_map_rgb(255, 255, 255));
			al_draw_rectangle(961, 136, 1021, 171, al_map_rgb(0, 0, 0), 1);
			al_draw_filled_rectangle(305, 135, 365, 170, al_map_rgb(255, 255, 255));
			al_draw_rectangle(306, 136, 366, 171, al_map_rgb(0, 0, 0), 1);

			char my_text4[TAM];
			sprintf(my_text4, "%d", h->vida);
			al_draw_text(FONTE, h->cor, 970, 140, 0, my_text4);
			char my_text5[TAM];
			sprintf(my_text5, "%d", m->vida);
			al_draw_text(FONTE, m->cor, 315, 140, 0, my_text5);
			al_draw_bitmap(ataquem, ataquem_x, ataquem_y, 0);
			//dou um refresh na tela
			al_flip_display();
			al_destroy_bitmap(image2);
		}
		if (ataquem_x == 1000)
		{
			break;
		}
	}
	al_destroy_bitmap(ataquem);
}
void animacaoEspecialHeroi(Heroi *h, Monstro *m)
{
	int a = 0, b = 0;
	a = 1;
	bool redraw = true;
	ALLEGRO_BITMAP *especialh = NULL;
	ALLEGRO_BITMAP *image2 = NULL;
	especialh = al_load_bitmap("ataqueEspecial.png");
	int largura_especialh = al_get_bitmap_width(especialh);
	int altura_especialh = al_get_bitmap_height(especialh);
	//posicao x e y inicial do passaro na tela
	int especialh_x = 1000;
	int especialh_y = 400;

	int especialh_dx = 4, especialh_dy = 0;
	while (especialh_x != 300)
	{
		if (a == 1)
		{
			//incrementa as posicoes x e y do passaro com o seu deslocamento dx e dy
			especialh_x -= especialh_dx;
			especialh_y += especialh_dy;
			//como eu movi o passaro, preciso redesenhar ele (remova essa linha e veja o que acontece)
			redraw = true;
			b = 1;
		}
		if (b == 1)
		{
			if (m->tipo == 1)
			{
				image2 = al_load_bitmap("batalha.png");
				al_draw_bitmap(image2, 0, 0, 0);
			}
			else if (m->tipo == 2)
			{
				image2 = al_load_bitmap("batalha2.png");
				al_draw_bitmap(image2, 0, 0, 0);
			}
			else if (m->tipo == 3)
			{
				image2 = al_load_bitmap("batalha3.png");
				al_draw_bitmap(image2, 0, 0, 0);
			}
			else if (m->tipo == 4)
			{
				image2 = al_load_bitmap("batalha4.png");
				al_draw_bitmap(image2, 0, 0, 0);
			}
			else if (m->tipo == 5)
			{
				image2 = al_load_bitmap("batalha5.png");
				al_draw_bitmap(image2, 0, 0, 0);
			}
			al_draw_filled_rectangle(960, 135, 1020, 170, al_map_rgb(255, 255, 255));
			al_draw_rectangle(961, 136, 1021, 171, al_map_rgb(0, 0, 0), 1);
			al_draw_filled_rectangle(305, 135, 365, 170, al_map_rgb(255, 255, 255));
			al_draw_rectangle(306, 136, 366, 171, al_map_rgb(0, 0, 0), 1);
			char my_text4[TAM];
			sprintf(my_text4, "%d", h->vida);
			al_draw_text(FONTE, h->cor, 970, 140, 0, my_text4);
			char my_text5[TAM];
			sprintf(my_text5, "%d", m->vida);
			al_draw_text(FONTE, m->cor, 315, 140, 0, my_text5);
			al_draw_bitmap(especialh, especialh_x, especialh_y, 0);
			//dou um refresh na tela
			al_flip_display();
			al_destroy_bitmap(image2);
		}
		if (especialh_x == 300)
		{
			break;
		}
	}
	//al_destroy_bitmap(image2);
	al_destroy_bitmap(especialh);
}

int processaAcaoHeroi(Heroi *h, Monstro *m)
{
	int a = 0, b = 0, c = 0, d = 0, e = 0, f = 0;

	if (h->acao == 30)
	{
		a = fuga();
		if (a > 0 && a <= 100)
		{
			if (a >= 35)
			{
				h->x = h->x_old;
				h->y = h->y_old;
				h->acao = 0;
				return NAVEGACAO;
			}
			else
			{
				b = ataqueMonstro(m);
				animacaoAtaqueMonstro(h, m);
				h->vida -= b;
				h->acao = 0;
				return BATALHA;
			}
		}
		a = 0;
		b = 0;
		h->acao = 0;
	}

	else if (h->acao == 10)
	{

		for (int i = 0; i < 1; i++)
		{
			c = ataqueHeroi(h);
			animacaoAtaqueHeroi(h, m);
			m->vida -= c;
			if (m->vida <= 0)
			{
				m->x = 2000;
				m->y = 2000;
				h->x = h->x_old;
				h->y = h->y_old;
				h->acao = 0;
				return NAVEGACAO;
			}
			d = ataqueMonstro(m);
			animacaoAtaqueMonstro(h, m);
			h->vida -= d;
		}
		c = 0;
		d = 0;
		h->acao = 0;
		return BATALHA;
	}

	if (h->acao == 20)
	{
		for (int i = 0; i < 1; i++)
		{
			e = especialHeroi(h);
			animacaoEspecialHeroi(h, m);
			//animacaoAtaqueHeroi(h, m);
			m->vida -= e;
			if (m->vida <= 0)
			{
				m->x = 2000;
				m->y = 2000;
				h->x = h->x_old;
				h->y = h->y_old;
				return NAVEGACAO;
			}
			f = ataqueMonstro(m);
			animacaoAtaqueMonstro(h, m);
			h->vida -= f;
		}
		e = 0;
		f = 0;
		h->acao = 0;
		return BATALHA;
	}
	return BATALHA;
}

//=========================== MAIN =================================================
int main(int argc, char **argv)
{

	ALLEGRO_DISPLAY *display = NULL;
	ALLEGRO_EVENT_QUEUE *event_queue = NULL;
	ALLEGRO_TIMER *timer = NULL;
	srand(time(NULL));
	//----------------------- rotinas de inicializacao ---------------------------------------

	//inicializa o Allegro
	if (!al_init())
	{
		fprintf(stderr, "failed to initialize allegro!\n");
		return -1;
	}

	//inicializa o módulo de primitivas do Allegro
	if (!al_init_primitives_addon())
	{
		fprintf(stderr, "failed to initialize primitives!\n");
		return -1;
	}

	//inicializa o modulo que permite carregar imagens no jogo
	if (!al_init_image_addon())
	{
		fprintf(stderr, "failed to initialize image module!\n");
		return -1;
	}

	//cria um temporizador que incrementa uma unidade a cada 1.0/FPS segundos
	timer = al_create_timer(1.0 / FPS);
	if (!timer)
	{
		fprintf(stderr, "failed to create timer!\n");
		return -1;
	}

	//cria uma tela com dimensoes de SCREEN_W, SCREEN_H pixels
	display = al_create_display(SCREEN_W, SCREEN_H);
	if (!display)
	{
		fprintf(stderr, "failed to create display!\n");
		al_destroy_timer(timer);
		return -1;
	}

	//instala o teclado
	if (!al_install_keyboard())
	{
		fprintf(stderr, "failed to install keyboard!\n");
		return -1;
	}

	//inicializa o modulo allegro que carrega as fontes
	al_init_font_addon();

	//inicializa o modulo allegro que entende arquivos tff de fontes
	if (!al_init_ttf_addon())
	{
		fprintf(stderr, "failed to load tff font module!\n");
		return -1;
	}

	//cria a fila de eventos
	event_queue = al_create_event_queue();
	if (!event_queue)
	{
		fprintf(stderr, "failed to create event_queue!\n");
		al_destroy_display(display);
		return -1;
	}

	//registra na fila os eventos de tela (ex: clicar no X na janela)
	al_register_event_source(event_queue, al_get_display_event_source(display));
	//registra na fila os eventos de tempo: quando o tempo altera de t para t+1
	al_register_event_source(event_queue, al_get_timer_event_source(timer));
	//registra na fila os eventos de teclado (ex: pressionar uma tecla)
	al_register_event_source(event_queue, al_get_keyboard_event_source());

	//inicia o temporizador
	al_start_timer(timer);

	//----------------------- inicio do jogo ---------------------------------------
	int playing = 1;
	int modo_jogo = NAVEGACAO;

	int luta = 0;
	iniciaGlobais();

	char my_text[TAM];
	char my_text2[TAM];
	char my_text5[TAM];
	int mau = 0;
	char linha[TAM];

	Heroi heroi;
	Monstro monstro;
	Monstro monstro2;
	Monstro monstro3;
	Monstro monstro4;
	Monstro monstro5;
	Monstro monstro6;
	Monstro monstro7;
	Monstro monstro8;
	Monstro monstro9;
	Monstro monstro10;
	Monstro monstro11;
	Monstro monstro12;
	Monstro monstro13;
	Monstro monstro14;
	Monstro monstro15;
	Monstro monstro16;
	Monstro monstro17;
	Monstro monstro18;
	Monstro monstro19;
	Monstro monstro20;
	Monstro monstro21;
	Menu menu;

	initHeroi(&heroi);
	initMonstro(&monstro, 1);
	initMonstro(&monstro2, 2);
	initMonstro(&monstro3, 3);
	initMonstro(&monstro4, 4);
	initMonstro(&monstro5, 5);
	initMonstro(&monstro6, 6);
	initMonstro(&monstro7, 7);
	initMonstro(&monstro8, 8);
	initMonstro(&monstro9, 9);
	initMonstro(&monstro10, 10);
	initMonstro(&monstro11, 11);
	initMonstro(&monstro12, 12);
	initMonstro(&monstro13, 13);
	initMonstro(&monstro14, 14);
	initMonstro(&monstro15, 15);
	initMonstro(&monstro16, 16);
	initMonstro(&monstro17, 17);
	initMonstro(&monstro18, 18);
	initMonstro(&monstro19, 19);
	initMonstro(&monstro20, 20);
	initMonstro(&monstro21, 21);
	initMenu(&menu);

	while (playing)
	{
		ALLEGRO_EVENT ev;
		//espera por um evento e o armazena na variavel de evento ev
		al_wait_for_event(event_queue, &ev);

		//se o tipo de evento for um evento do temporizador, ou seja, se o tempo passou de t para t+1
		if (ev.type == ALLEGRO_EVENT_TIMER)
		{

			if (modo_jogo == NAVEGACAO)
			{
				desenhaCenarioNavegacao(&heroi);
				desenhaHeroiNavegacao(&heroi);

				if (detectouMonstro(heroi, monstro))
				{
					luta = 1;
					modo_jogo = BATALHA;
				}
				else if (detectouMonstro(heroi, monstro2))
				{
					luta = 2;
					modo_jogo = BATALHA;
				}
				else if (detectouMonstro(heroi, monstro3))
				{
					luta = 3;
					modo_jogo = BATALHA;
				}
				else if (detectouMonstro(heroi, monstro4))
				{
					luta = 4;
					modo_jogo = BATALHA;
				}
				else if (detectouMonstro(heroi, monstro5))
				{
					luta = 5;
					modo_jogo = BATALHA;
				}
				else if (detectouMonstro(heroi, monstro6))
				{
					luta = 6;
					modo_jogo = BATALHA;
				}
				else if (detectouMonstro(heroi, monstro7))
				{
					luta = 7;
					modo_jogo = BATALHA;
				}
				else if (detectouMonstro(heroi, monstro8))
				{
					luta = 8;
					modo_jogo = BATALHA;
				}
				else if (detectouMonstro(heroi, monstro9))
				{
					luta = 9;
					modo_jogo = BATALHA;
				}
				else if (detectouMonstro(heroi, monstro10))
				{
					luta = 10;
					modo_jogo = BATALHA;
				}
				else if (detectouMonstro(heroi, monstro11))
				{
					luta = 11;
					modo_jogo = BATALHA;
				}
				else if (detectouMonstro(heroi, monstro12))
				{
					luta = 12;
					modo_jogo = BATALHA;
				}
				else if (detectouMonstro(heroi, monstro13))
				{
					luta = 13;
					modo_jogo = BATALHA;
				}
				else if (detectouMonstro(heroi, monstro14))
				{
					luta = 14;
					modo_jogo = BATALHA;
				}
				else if (detectouMonstro(heroi, monstro15))
				{
					luta = 15;
					modo_jogo = BATALHA;
				}
				else if (detectouMonstro(heroi, monstro16))
				{
					luta = 16;
					modo_jogo = BATALHA;
				}
				else if (detectouMonstro(heroi, monstro17))
				{
					luta = 17;
					modo_jogo = BATALHA;
				}
				else if (detectouMonstro(heroi, monstro18))
				{
					luta = 18;
					modo_jogo = BATALHA;
				}
				else if (detectouMonstro(heroi, monstro19))
				{
					luta = 19;
					modo_jogo = BATALHA;
				}
				else if (detectouMonstro(heroi, monstro20))
				{
					luta = 20;
					modo_jogo = BATALHA;
				}
				else if (detectouMonstro(heroi, monstro21))
				{
					luta = 21;
					modo_jogo = BATALHA;
				}

				if (chegouObjetivo(heroi))
				{
					int pont = 0, pont2 = 0;
					FILE *arq = fopen("Maxpontos.txt", "r");
					while (1)
					{
						fgets(linha, TAM, arq);
						pont = atoi(strtok(linha, "\0"));
						printf("linha 861\n%d\n", pont);

						if (pont < heroi.pontos)
						{
							printf("aaaaaa\n");
							pont = heroi.pontos;
							printf("linha 867\n%d\n", pont);
							mau = 1;
							fclose(arq);
						}

						break;
					}
					printf("pontuacao feita: %d", heroi.pontos);
					playing = 0;
					al_clear_to_color(al_map_rgb(0, 0, 0));
					sprintf(my_text, "Pontos: %d", heroi.pontos);
					al_draw_text(FONTE, al_map_rgb(15, 200, 30), SCREEN_W / 3, SCREEN_H / 2, 0, my_text);
					sprintf(my_text2, "Você Venceu:");
					al_draw_text(FONTE, al_map_rgb(250, 200, 30), SCREEN_W / 3, SCREEN_H / 3, 0, my_text2);
					FILE *arq3 = fopen("Maxpontos.txt", "r");
					while (!feof(arq3))
					{
						fgets(linha, TAM, arq3);
						pont2 = atoi(strtok(linha, "\0"));
						printf("\nlinha 894\n%d\n", pont2);
						if (mau == 1)
						{
							printf("897\n");
							FILE *arq2 = fopen("Maxpontos.txt", "w");
							fprintf(arq2, "%d\n", heroi.pontos);
							fclose(arq2);
						}
						break;
					}
					sprintf(my_text5, "Recorde anterior: %d", pont2);
					al_draw_text(FONTE, al_map_rgb(250, 0, 255), (SCREEN_W / 3), (SCREEN_H / 1.5), 0, my_text5);
					fclose(arq3);
					al_flip_display();
					al_rest(3);
				}
			}
			else
			{
				if (luta == 1)
				{
					desenhaBatalha2(&menu, &heroi, &monstro);
					modo_jogo = processaAcaoHeroi(&heroi, &monstro);
				}
				else if (luta == 2)
				{
					desenhaBatalha5(&menu, &heroi, &monstro2);
					modo_jogo = processaAcaoHeroi(&heroi, &monstro2);
				}
				else if (luta == 3)
				{
					desenhaBatalha2(&menu, &heroi, &monstro3);
					modo_jogo = processaAcaoHeroi(&heroi, &monstro3);
				}
				else if (luta == 4)
				{
					desenhaBatalha3(&menu, heroi, &monstro4);
					modo_jogo = processaAcaoHeroi(&heroi, &monstro4);
				}
				else if (luta == 5)
				{
					desenhaBatalha1(&menu, heroi, &monstro5);
					modo_jogo = processaAcaoHeroi(&heroi, &monstro5);
				}
				else if (luta == 6)
				{
					desenhaBatalha3(&menu, heroi, &monstro6);
					modo_jogo = processaAcaoHeroi(&heroi, &monstro6);
				}
				else if (luta == 7)
				{
					desenhaBatalha1(&menu, heroi, &monstro7);
					modo_jogo = processaAcaoHeroi(&heroi, &monstro7);
				}
				else if (luta == 8)
				{
					desenhaBatalha1(&menu, heroi, &monstro8);
					modo_jogo = processaAcaoHeroi(&heroi, &monstro8);
				}
				else if (luta == 9)
				{
					desenhaBatalha1(&menu, heroi, &monstro9);
					modo_jogo = processaAcaoHeroi(&heroi, &monstro9);
				}
				else if (luta == 10)
				{
					desenhaBatalha2(&menu, &heroi, &monstro10);
					modo_jogo = processaAcaoHeroi(&heroi, &monstro10);
				}
				else if (luta == 11)
				{
					desenhaBatalha3(&menu, heroi, &monstro11);
					modo_jogo = processaAcaoHeroi(&heroi, &monstro11);
				}
				else if (luta == 12)
				{
					desenhaBatalha4(&menu, &heroi, &monstro12);
					modo_jogo = processaAcaoHeroi(&heroi, &monstro12);
				}
				else if (luta == 13)
				{
					desenhaBatalha4(&menu, &heroi, &monstro13);
					modo_jogo = processaAcaoHeroi(&heroi, &monstro13);
				}
				else if (luta == 14)
				{
					desenhaBatalha2(&menu, &heroi, &monstro14);
					modo_jogo = processaAcaoHeroi(&heroi, &monstro14);
				}
				else if (luta == 15)
				{
					desenhaBatalha3(&menu, heroi, &monstro15);
					modo_jogo = processaAcaoHeroi(&heroi, &monstro15);
				}
				else if (luta == 16)
				{
					desenhaBatalha4(&menu, &heroi, &monstro16);
					modo_jogo = processaAcaoHeroi(&heroi, &monstro16);
				}
				else if (luta == 17)
				{
					desenhaBatalha2(&menu, &heroi, &monstro17);
					modo_jogo = processaAcaoHeroi(&heroi, &monstro17);
				}
				else if (luta == 18)
				{
					desenhaBatalha3(&menu, heroi, &monstro18);
					modo_jogo = processaAcaoHeroi(&heroi, &monstro18);
				}
				else if (luta == 19)
				{
					desenhaBatalha1(&menu, heroi, &monstro19);
					modo_jogo = processaAcaoHeroi(&heroi, &monstro19);
				}
				else if (luta == 20)
				{
					desenhaBatalha4(&menu, &heroi, &monstro20);
					modo_jogo = processaAcaoHeroi(&heroi, &monstro20);
				}
				else if (luta == 21)
				{
					desenhaBatalha4(&menu, &heroi, &monstro21);
					modo_jogo = processaAcaoHeroi(&heroi, &monstro21);
				}

				if (heroi.vida <= 0)
				{
					int pont2 = 0;
					printf("pontuacao feita: %d", heroi.pontos);
					playing = 0;
					al_clear_to_color(al_map_rgb(0, 0, 0));
					sprintf(my_text, "Pontos: %d", heroi.pontos);
					al_draw_text(FONTE, al_map_rgb(128, 200, 30), SCREEN_W / 3, SCREEN_H / 2, 0, my_text);
					sprintf(my_text2, "GAME OVER! :(");
					al_draw_text(FONTE, al_map_rgb(250, 200, 30), SCREEN_W / 3, SCREEN_H / 3, 0, my_text2);
					FILE *arq3 = fopen("Maxpontos.txt", "r");
					while (!feof(arq3))
					{
						fgets(linha, TAM, arq3);
						pont2 = atoi(strtok(linha, "\0"));
						printf("\nlinha 894\n%d\n", pont2);
						break;
					}
					sprintf(my_text5, "Recorde anterior: %d", pont2);
					al_draw_text(FONTE, al_map_rgb(250, 0, 255), (SCREEN_W / 3), (SCREEN_H / 1.5), 0, my_text5);
					fclose(arq3);
					al_flip_display();
					al_rest(3);
				}
			}
			//atualiza a tela (quando houver algo para mostrar)
			al_flip_display();
		}
		//se o tipo de evento for o fechamento da tela (clique no x da janela)
		else if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
		{
			playing = 0;
		}
		//se o tipo de evento for um pressionar de uma tecla
		else if (ev.type == ALLEGRO_EVENT_KEY_DOWN)
		{
			if (modo_jogo == NAVEGACAO)
			{
				processaTeclaNavegacao(&heroi, ev.keyboard.keycode);
			}
			else
			{
				processaTeclaBatalha(&heroi, ev.keyboard.keycode, &menu);
			}
			//printf("\ncodigo tecla: %d", ev.keyboard.keycode);
		}

	} //fim do while

	//procedimentos de fim de jogo (fecha a tela, limpa a memoria, etc)

	al_destroy_timer(timer);
	al_destroy_display(display);
	al_destroy_event_queue(event_queue);
	al_destroy_display(display);

	return 0;
}