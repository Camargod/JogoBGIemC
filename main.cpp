#include<stdio.h>
#include<stdlib.h>
#include<iostream>
#include<graphics.h>
#include<time.h>
#include<math.h>
#include <windows.h>
#include "MMSystem.h"
#pragma comment(lib, "winmm.lib");
using namespace std;

#define ESC    	27


unsigned char *imagens[15];
unsigned char *mascaras[15];
int TamP[15];
//Aqui est� o esquema de camadas para trabalhar com UI
int layer = 0;
int blink = 0;

struct gameConfigurations
{
  bool isPaused;
  bool inGame;
  int money;
  int time;
  int round;
  int resolution[2];
  int frames;
  double starttime;
  bool first;
  float fps;
  float timePassed;
  int index;
  int projectileIndex;
  int enemiesIndex;
  int enemiesMoveIndex;
  int colisionsIndex;
  bool inCutscene;
  bool prep;
  int scene;
  bool menu;
  int select;
  int horda;
};

int projectileNumber = 0;

struct torre
{
  float PosX;
  float PosY;
  float PosX0;
  float PosY0;
  float PosX1;
  float PosY1;
  char src;
  bool isEnabled;
};

struct enemies
{
  float PosX;
  float PosY;
  
  char src;
  bool isEnabled;
  float vetX;
  float vetY;
  float speed = 10;
  const char * graphics;
  int life;
};

struct projectile
{
  int size;
  double speed;
  bool isLoaded;
  double x;
  double y;
  double initX;
  double initY;
  double finX;
  double finY;
  double Xvet;
  double Yvet;
};

struct colisionRoutes
{
  int x;
  int y;
  int vetX;
  int vetY;
};

struct gameConfigurations gameConfig;
struct torre torres[7];
struct enemies inimigos[20]; 
struct projectile projeteis[100];
struct colisionRoutes colisionRoutesBoxes[17];

void setRoutesConfig()
{
  colisionRoutesBoxes[0].x = 200;
  colisionRoutesBoxes[0].y = 280;
  colisionRoutesBoxes[0].vetX = -1;
  colisionRoutesBoxes[0].vetY = 0;

  colisionRoutesBoxes[1].x = 125;
  colisionRoutesBoxes[1].y = 280;
  colisionRoutesBoxes[1].vetX = 0;
  colisionRoutesBoxes[1].vetY = 1;

  colisionRoutesBoxes[2].x = 530;
  colisionRoutesBoxes[2].y = 30;
  colisionRoutesBoxes[2].vetX = 0;
  colisionRoutesBoxes[2].vetY = 1;
  
  colisionRoutesBoxes[3].x = 125;
  colisionRoutesBoxes[3].y = 380;
  colisionRoutesBoxes[3].vetX = -1;
  colisionRoutesBoxes[3].vetY = 0;

  colisionRoutesBoxes[4].x = 410;
  colisionRoutesBoxes[4].y = 280;
  colisionRoutesBoxes[4].vetX = -1;
  colisionRoutesBoxes[4].vetY = 0;

  colisionRoutesBoxes[5].x = 420;
  colisionRoutesBoxes[5].y = 340;
  colisionRoutesBoxes[5].vetX = 0;
  colisionRoutesBoxes[5].vetY = -1;

  colisionRoutesBoxes[6].x = 480;
  colisionRoutesBoxes[6].y = 340;
  colisionRoutesBoxes[6].vetX = 0;
  colisionRoutesBoxes[6].vetY = 1;

  colisionRoutesBoxes[6].x = 640;
  colisionRoutesBoxes[6].y = 340;
  colisionRoutesBoxes[6].vetX = -1;
  colisionRoutesBoxes[6].vetY = 0;

  colisionRoutesBoxes[6].x = 640;
  colisionRoutesBoxes[6].y = 170;
  colisionRoutesBoxes[6].vetX = -1;
  colisionRoutesBoxes[6].vetY = 0;

  colisionRoutesBoxes[7].x = 480;
  colisionRoutesBoxes[7].y = 340;
  colisionRoutesBoxes[7].vetX = -1;
  colisionRoutesBoxes[7].vetY = 0;

  colisionRoutesBoxes[8].x = 985;
  colisionRoutesBoxes[8].y = 340;
  colisionRoutesBoxes[8].vetX = -1;
  colisionRoutesBoxes[8].vetY = 0;
  
  colisionRoutesBoxes[9].x = 985;
  colisionRoutesBoxes[9].y = 570;
  colisionRoutesBoxes[9].vetX = 0;
  colisionRoutesBoxes[9].vetY = -1;

  colisionRoutesBoxes[10].x = 1130;
  colisionRoutesBoxes[10].y = 570;
  colisionRoutesBoxes[10].vetX = -1;
  colisionRoutesBoxes[10].vetY = 0;

  colisionRoutesBoxes[11].x = 630;
  colisionRoutesBoxes[11].y = 340;
  colisionRoutesBoxes[11].vetX = 0;
  colisionRoutesBoxes[11].vetY = 1;

  colisionRoutesBoxes[12].x = 810;
  colisionRoutesBoxes[12].y = 60;
  colisionRoutesBoxes[12].vetX = 0;
  colisionRoutesBoxes[12].vetY = 1;

  colisionRoutesBoxes[13].x = 810;
  colisionRoutesBoxes[13].y = 170;  
  colisionRoutesBoxes[13].vetX = 0;
  colisionRoutesBoxes[13].vetY = 1;

  colisionRoutesBoxes[14].x = 775;
  colisionRoutesBoxes[14].y = 340;
  colisionRoutesBoxes[14].vetX = -1; //aleatorio
  colisionRoutesBoxes[14].vetY = 0;

  colisionRoutesBoxes[15].x = 480;
  colisionRoutesBoxes[15].y = 465;
  colisionRoutesBoxes[15].vetX = -1;
  colisionRoutesBoxes[15].vetY = 0;

  colisionRoutesBoxes[16].x = 300;
  colisionRoutesBoxes[16].y = 465;
  colisionRoutesBoxes[16].vetX = -1;
  colisionRoutesBoxes[16].vetY = 0;
}


void setInitialEnemyConfig()
{
	int dist = 0;
  for(int index = 1; index < 7; index++)
  {
  	switch (index) {
  		case 1:
		  inimigos[index].PosX = 1320 + dist;
		  inimigos[index].PosY = 540;
		  inimigos[index].vetX = -1;
		  inimigos[index].vetY = 0;
		  break;
		case 2:
			inimigos[index].PosX = 770;
    		inimigos[index].PosY = 706 + dist;
    		inimigos[index].vetY = -1;
    		inimigos[index].vetX = 0;
    		break;
    	case 3:
    		inimigos[index].PosX = 500;
    		inimigos[index].PosY = -180 - dist;
    		inimigos[index].vetY = 1;
    		inimigos[index].vetX = 0;
    		break;
	  }
    inimigos[index].isEnabled = true;
    //inimigos[index].vetX = -1;
    //inimigos[index].vetY = 0;
    inimigos[index].speed = 20;
    dist += rand() % 80 + 120;
  }
  
  inimigos[0].PosX = 770;
  inimigos[0].PosY = 706;
  inimigos[0].isEnabled = true;
  inimigos[0].vetY = -1;
  inimigos[0].vetX = 0;
  inimigos[0].speed = 20;
}


void startGameConfig()
{
  gameConfig.isPaused = false;
  gameConfig.inCutscene = false;
  gameConfig.inGame = false;
  gameConfig.resolution[0] = 1280;
  gameConfig.resolution[1] = 720;
  gameConfig.frames = 0;
  gameConfig.starttime = 0;
  gameConfig.first = true;
  gameConfig.fps = 0.0f;
  gameConfig.index = 0;
  gameConfig.enemiesIndex = 0;
  gameConfig.colisionsIndex = 0;
  gameConfig.enemiesMoveIndex = 0;
  gameConfig.money = 100;
  gameConfig.round = 1;
  gameConfig.prep = false;
  gameConfig.scene = 4;
  gameConfig.menu = true;
  gameConfig.select = 0;
  gameConfig.horda = 0;
}


void projectileConfig()
{
  for(int index = 0; index < 100; index -=-1)
  {
    projeteis[index].isLoaded = false;
    projeteis[index].x = 0;
    projeteis[index].y = 0;
  }
}


void setPlayerConfig()
{
  torres[0].PosX = 410;
  torres[0].PosY = 415;
  torres[0].PosX0 = torres[0].PosX - 50;  
  torres[0].PosY0 = torres[0].PosY - 50;
  torres[0].PosX1 = torres[0].PosX + 50;  
  torres[0].PosY1 = torres[0].PosY + 50;

  torres[1].PosX = 165;
  torres[1].PosY = 345;
  torres[1].PosX0 = torres[1].PosX - 50;  
  torres[1].PosY0 = torres[1].PosY - 50;
  torres[1].PosX1 = torres[1].PosX + 50;  
  torres[1].PosY1 = torres[1].PosY + 50;

  torres[2].PosX = 265;
  torres[2].PosY = 215;
  torres[2].PosX0 = torres[2].PosX - 50;  
  torres[2].PosY0 = torres[2].PosY - 50;
  torres[2].PosX1 = torres[2].PosX + 50;  
  torres[2].PosY1 = torres[2].PosY + 50;

  torres[3].PosX = 715;
  torres[3].PosY = 560;
  torres[3].PosX0 = torres[3].PosX - 50;  
  torres[3].PosY0 = torres[3].PosY - 50;
  torres[3].PosX1 = torres[3].PosX + 50;  
  torres[3].PosY1 = torres[3].PosY + 50;

  torres[4].PosX = 750;
  torres[4].PosY = 120;
  torres[4].PosX0 = torres[4].PosX - 50;  
  torres[4].PosY0 = torres[4].PosY - 50;
  torres[4].PosX1 = torres[4].PosX + 50;  
  torres[4].PosY1 = torres[4].PosY + 50;

  torres[5].PosX = 915;
  torres[5].PosY = 415;
  torres[5].PosX0 = torres[5].PosX - 50;  
  torres[5].PosY0 = torres[5].PosY - 50;
  torres[5].PosX1 = torres[5].PosX + 50;  
  torres[5].PosY1 = torres[5].PosY + 50;

  torres[6].PosX = 1180;
  torres[6].PosY = 200;
  torres[6].PosX0 = torres[6].PosX - 50;  
  torres[6].PosY0 = torres[6].PosY - 50;
  torres[6].PosX1 = torres[6].PosX + 50;  
  torres[6].PosY1 = torres[6].PosY + 50;

  torres[0].isEnabled = false;
  torres[1].isEnabled = false;
  torres[2].isEnabled = false;
  torres[3].isEnabled = false;
  torres[4].isEnabled = false;
  torres[5].isEnabled = false;
  torres[6].isEnabled = false;
}


void escolhendopos(int id, int preco) {
	POINT mouse2;
	int pos = GetCursorPos(&mouse2);
	int posx = mouse2.x;
	int posy = mouse2.y;
	putimage(posx, posy, mascaras[id], AND_PUT);
	putimage(posx, posy, imagens[id], OR_PUT);
	int cont = 0;
	
	// TORRE 1
	if (posx >= 170 && posx <= 225 && posy <= 395 && posy >= 350 && torres[1].isEnabled == false) {
		if ((GetKeyState(VK_LBUTTON) & 0x80) != 0) {
			if (gameConfig.money >= preco) {
    		torres[1].isEnabled = true;
    		gameConfig.money -= preco;
        	mciSendString("play src/Buy.wav", NULL, 0, NULL);
    		gameConfig.select = 0;
    		}
	    	else {
	    		outtextxy(posx,posy-20,"Sem dinheiro suficiente!");
			}
		} 
	}
	
	//TORRE 2
	if (posx >= 269 && posx <= 320 && posy <= 262 && posy >= 220 && torres[2].isEnabled == false) 
  {
		if ((GetKeyState(VK_LBUTTON) & 0x80) != 0) 
    {
			if (gameConfig.money >= preco) 
      {
        torres[2].isEnabled = true;
        gameConfig.money -= preco;
        mciSendString("play src/Buy.wav", NULL, 0, NULL);
        gameConfig.select = 0;
	    }
	    else 
      {
	    	outtextxy(posx,posy-20,"Sem dinheiro suficiente!");
			}
		} 
	}
	
	//TORRE 3
	if (posx >= 413 && posx <= 465 && posy <= 466 && posy >= 420 && torres[0].isEnabled == false) {
		if ((GetKeyState(VK_LBUTTON) & 0x80) != 0) {
			if (gameConfig.money >= preco) {
    		torres[0].isEnabled = true;
    		gameConfig.money -= preco;
    		mciSendString("play src/Buy.wav", NULL, 0, NULL);
    		gameConfig.select = 0;
    		}
	    	else {
	    		outtextxy(posx,posy-20,"Sem dinheiro suficiente!");
			}
		} 
	}
	
	//TORRE 4
	if (posx >= 715 && posx <= 768 && posy <= 612 && posy >= 565 && torres[3].isEnabled == false) {
		if ((GetKeyState(VK_LBUTTON) & 0x80) != 0) {
			if (gameConfig.money >= preco) {
    		torres[3].isEnabled = true;
    		gameConfig.money -= preco;
    		mciSendString("play src/Buy.wav", NULL, 0, NULL);
    		gameConfig.select = 0;
    		}
	    	else {
	    		outtextxy(posx,posy-20,"Sem dinheiro suficiente!");
			}
		} 
	}
	
	//TORRE 5
	if (posx >= 748 && posx <= 796 && posy <= 170 && posy >= 126 && torres[4].isEnabled == false) {
		if ((GetKeyState(VK_LBUTTON) & 0x80) != 0) {
			if (gameConfig.money >= preco) {
    		torres[4].isEnabled = true;
    		gameConfig.money -= preco;
    		mciSendString("play src/Buy.wav", NULL, 0, NULL);
    		gameConfig.select = 0;
    		}
	    	else {
	    		outtextxy(posx,posy-20,"Sem dinheiro suficiente!");
			}
		} 
	}
	
	//TORRE 6
	if (posx >= 919 && posx <= 970 && posy <= 467 && posy >= 422 && torres[5].isEnabled == false) {
		if ((GetKeyState(VK_LBUTTON) & 0x80) != 0) {
			if (gameConfig.money >= preco) {
    		torres[5].isEnabled = true;
    		gameConfig.money -= preco;
    		mciSendString("play src/Buy.wav", NULL, 0, NULL);
    		gameConfig.select = 0;
    		}
	    	else {
	    		outtextxy(posx,posy-20,"Sem dinheiro suficiente!");
			}
		} 
	}
	
	//TORRE 7
	if (posx >= 1181 && posx <= 1231 && posy <= 248 && posy >= 203 && torres[6].isEnabled == false) {
		if ((GetKeyState(VK_LBUTTON) & 0x80) != 0) {
			if (gameConfig.money >= preco) {
    		torres[6].isEnabled = true;
    		gameConfig.money -= preco;
    		mciSendString("play src/Buy.wav", NULL, 0, NULL);
    		gameConfig.select = 0;
    		}
	    	else {
	    		outtextxy(posx,posy-20,"Sem dinheiro suficiente!");
			}
		} 
	}
}

void backgroundConfig() 
{
	setbkcolor(RGB(70, 170, 70));
}

void ImageConfig(int Tam, unsigned char *Img, unsigned char *Msk) 
{
// Tam ? o tamanho dos ponteiros da imagem e da m?scara
// Img ? o ponteiro que aponta para o vetor que cont?m a imagem capturada com getimage(..)
// Msk ? o ponteiro que ser? transformado em m?scara
  int i;
  unsigned char B, G, R;
  B = Img[24];
  G = Img[25];
  R = Img[26];
  for(i=24; i < Tam; i+=4) 
  {
    if (Img[i]==B && Img[i+1]==G && Img[i+2]==R) 
    {
      Img[i] = 0;
      Img[i+1] = 0;
      Img[i+2] = 0;
      Msk[i] = 255;
      Msk[i+1] = 255;
      Msk[i+2] = 255;
    }
    else 
    {
      Msk[i] = 0;
      Msk[i+1] = 0;
      Msk[i+2] = 0;
    }
  }
}

#pragma region Mec�ncia de cordenada de tiro
void shoot(int posInit[2], int posFinal[2], int structureSpeed)
{
  if(!projeteis[gameConfig.index].isLoaded)
  {
    projeteis[gameConfig.index].x = posInit[0] + 15;
    projeteis[gameConfig.index].y = posInit[1] + 15;
    projeteis[gameConfig.index].speed = structureSpeed;
    projeteis[gameConfig.index].size = 4;
    projeteis[gameConfig.index].initX = posInit[0];
    projeteis[gameConfig.index].initY = posInit[1];
    projeteis[gameConfig.index].finX = posFinal[0];
    projeteis[gameConfig.index].finY = posFinal[1];

    projeteis[gameConfig.index].Xvet = (posInit[0] - posFinal[0]) / 80;
    projeteis[gameConfig.index].Yvet = (posInit[1] - posFinal[1]) / 80;
    projeteis[gameConfig.index].isLoaded = true;
  //   double a;
  //   double b;
  //   if(projeteis[gameConfig.index].initY - projeteis[gameConfig.index].finY > 0)
  //   {
  //     a = pow((projeteis[gameConfig.index].initY - projeteis[gameConfig.index].finY),2);
  //   }
  //   if(projeteis[gameConfig.index].initY - projeteis[gameConfig.index].finY == 0)
  //   {
  //     a = 0;
  //   }
  //   if (projeteis[gameConfig.index].initY - projeteis[gameConfig.index].finY < 0)
  //   {
  //     a = pow((projeteis[gameConfig.index].finY - projeteis[gameConfig.index].initY),2);
  //   }

  //   if(projeteis[gameConfig.index].initX - projeteis[gameConfig.index].finX > 0)
  //   {
  //     b = pow((projeteis[gameConfig.index].initX - projeteis[gameConfig.index].finX),2);
  //   }
  //   if(projeteis[gameConfig.index].initX - projeteis[gameConfig.index].finX == 0)
  //   {
  //     b = 0;
  //   }
  //   if (projeteis[gameConfig.index].initX - projeteis[gameConfig.index].finX < 0)
  //   {
  //     b = pow((projeteis[gameConfig.index].finX - projeteis[gameConfig.index].initX),2);
  //   }
    
  //   b = pow((projeteis[gameConfig.index].finX - projeteis[gameConfig.index].initX),2);
  //   double hip = sqrt(a + b);
  //   // printf("Cateto Adjacente do projetil %i : %lf\n",gameConfig.index,a);
  //   // printf("Cateto Oposto do projetil %i : %lf\n",gameConfig.index,b);
  //   // printf("Hipotenusa do projetil %i : %lf\n",gameConfig.index,hip);
  //   if(b !=0)
  //   {
  //     projeteis[gameConfig.index].Xvet =  cos(b /hip);
  //   }
  //   else
  //   {
  //     projeteis[gameConfig.index].Xvet = 0;
  //   }
  //   if(a !=0)
  //   {
  //     projeteis[gameConfig.index].Yvet =  sin(a /hip);
  //   }
  //   else
  //   {
  //     projeteis[gameConfig.index].Yvet = 0;
  //   }
  //   // printf("Seno do projetil (Yvet) %i : %lf\n",gameConfig.index,projeteis[gameConfig.index].Yvet);
  //   // printf("Coseno do projetil (Xvet) %i : %lf\n",gameConfig.index,projeteis[gameConfig.index].Xvet);
  //   projeteis[gameConfig.index].isLoaded = true;
  }
  else
  {
    if(gameConfig.index >= 25)
    {
      gameConfig.index = 0;
    }
    gameConfig.index++;
  }
}



#pragma endregion
void Move()
{
  if(GetKeyState(VK_UP)&0x80)
  {
    int posIni[2];
    posIni[0] = torres[0].PosX;
    posIni[1] = torres[0].PosY;
    int posFinal[2];
    posFinal[0] = inimigos[0].PosX;
    posFinal[1] = inimigos[0].PosY;

    shoot(posIni,posFinal,25);
	}
}



void imagesRenderer()
{
  TamP[0] = imagesize(0, 0, 60, 60);
  TamP[1] = imagesize(0, 0, 649, 479);
  TamP[2] = imagesize(0,0,1279,719);
  TamP[3] = imagesize(0,0,80,80);
  TamP[4] = imagesize(0,0,1279,719);
  TamP[5] = imagesize(0,0,1279,719);
  TamP[6] = imagesize(0,0,1279,719);
  TamP[7] = imagesize(0,0,100,100);
  TamP[8] = imagesize(0,0,1279,719);
  TamP[9] = imagesize(0,0,1279,719);
  TamP[10] = imagesize(0,0,1279,719);
  TamP[11] = imagesize(0,0,100,100);
  TamP[12] = imagesize(0,0,100,100);
  
  readimagefile("Estilingue.bmp",0 , 0 , 100, 100); // carrega a imagem
  imagens[7] = (unsigned char *)malloc(TamP[7]);
  mascaras[7] = (unsigned char *)malloc(TamP[7]);
  getimage(0, 0, 100, 100, imagens[7]); // captura para o ponteiro P
  getimage(0, 0, 100, 100, mascaras[7]); // captura para a m�scara M (a mesma imagem de P, que depois ser� manipulada na rotina PreparaImg)
  ImageConfig(TamP[7],imagens[7],mascaras[7]);
  
  readimagefile("fundo.bmp",0 , 0 , 1279, 719); // carrega a imagem
  imagens[4] = (unsigned char *)malloc(TamP[4]);
  mascaras[4] = (unsigned char *)malloc(TamP[4]);
  getimage(0, 0, 1279, 719, imagens[4]); // captura para o ponteiro P
  getimage(0, 0, 1279, 719, mascaras[4]); // captura para a m�scara M (a mesma imagem de P, que depois ser� manipulada na rotina PreparaImg)
  ImageConfig(TamP[4],imagens[4],mascaras[4]);
  
  readimagefile("menu.bmp",0 , 0 , 1279, 719); // carrega a imagem
  imagens[8] = (unsigned char *)malloc(TamP[8]);
  mascaras[8] = (unsigned char *)malloc(TamP[8]);
  getimage(0, 0, 1279, 719, imagens[8]); // captura para o ponteiro P
  getimage(0, 0, 1279, 719, mascaras[8]); // captura para a m�scara M (a mesma imagem de P, que depois ser� manipulada na rotina PreparaImg)
  ImageConfig(TamP[8],imagens[8],mascaras[8]);
  
  readimagefile("menu2.bmp",0 , 0 , 1279, 719); // carrega a imagem
  imagens[9] = (unsigned char *)malloc(TamP[9]);
  mascaras[9] = (unsigned char *)malloc(TamP[9]);
  getimage(0, 0, 1279, 719, imagens[9]); // captura para o ponteiro P
  getimage(0, 0, 1279, 719, mascaras[9]); // captura para a m�scara M (a mesma imagem de P, que depois ser� manipulada na rotina PreparaImg)
  ImageConfig(TamP[9],imagens[9],mascaras[9]);

  readimagefile("Cenario_MP3.bmp",0 , 0 , 1279, 719); // carrega a imagem
  imagens[10] = (unsigned char *)malloc(TamP[10]);
  mascaras[10] = (unsigned char *)malloc(TamP[10]);
  getimage(0, 0, 1279, 719, imagens[10]); // captura para o ponteiro P
  getimage(0, 0, 1279, 719, mascaras[10]); // captura para a m�scara M (a mesma imagem de P, que depois ser� manipulada na rotina PreparaImg)
  ImageConfig(TamP[10],imagens[10],mascaras[10]);
  
  readimagefile("Cenario_MP2.bmp",0 , 0 , 1279, 719); // carrega a imagem
  imagens[6] = (unsigned char *)malloc(TamP[6]);
  mascaras[6] = (unsigned char *)malloc(TamP[6]);
  getimage(0, 0, 1279, 719, imagens[6]); // captura para o ponteiro P
  getimage(0, 0, 1279, 719, mascaras[6]); // captura para a m�scara M (a mesma imagem de P, que depois ser� manipulada na rotina PreparaImg)
  ImageConfig(TamP[6],imagens[6],mascaras[6]);
  
  readimagefile("Cenario_MP.bmp",0 , 0 , 1279, 719); // carrega a imagem
  imagens[5] = (unsigned char *)malloc(TamP[5]);
  mascaras[5] = (unsigned char *)malloc(TamP[5]);
  getimage(0, 0, 1279, 719, imagens[5]); // captura para o ponteiro P
  getimage(0, 0, 1279, 719, mascaras[5]); // captura para a m�scara M (a mesma imagem de P, que depois ser� manipulada na rotina PreparaImg)
  ImageConfig(TamP[5],imagens[5],mascaras[5]);
  
  readimagefile("Cenario_SP.bmp",0 , 0 , 1279, 719); // carrega a imagem
  imagens[2] = (unsigned char *)malloc(TamP[2]);
  mascaras[2] = (unsigned char *)malloc(TamP[2]);
  getimage(0, 0, 1279, 719, imagens[2]); // captura para o ponteiro P
  getimage(0, 0, 1279, 719, mascaras[2]); // captura para a m�scara M (a mesma imagem de P, que depois ser� manipulada na rotina PreparaImg)
  ImageConfig(TamP[2],imagens[2],mascaras[2]);

  readimagefile("Estilingue2.bmp",0 , 0 , 60, 60); // carrega a imagem
  imagens[0] = (unsigned char *)malloc(TamP[0]);
  mascaras[0] = (unsigned char *)malloc(TamP[0]);
  getimage(0, 0, 60, 60, imagens[0]); // captura para o ponteiro P
  getimage(0, 0, 60, 60, mascaras[0]); // captura para a m�scara M (a mesma imagem de P, que depois ser� manipulada na rotina PreparaImg)
  ImageConfig(TamP[0],imagens[0],mascaras[0]);

  readimagefile("pause.BMP",0 , 0 , 639, 479); // carrega a imagem
  imagens[1] = (unsigned char *)malloc(TamP[1]);
  mascaras[1] = (unsigned char *)malloc(TamP[1]);
  getimage(0, 0, 639, 479, imagens[1]); // captura para o ponteiro P
  getimage(0, 0, 639, 479, mascaras[1]); // captura para a m�scara M (a mesma imagem de P, que depois ser� manipulada na rotina PreparaImg)
  ImageConfig(TamP[1],imagens[1],mascaras[1]);

  readimagefile("SaciT0.bmp", 0, 0, 80, 80); // carrega a imagem
  imagens[3] = (unsigned char *)malloc(TamP[3]);
  mascaras[3] = (unsigned char *)malloc(TamP[3]);
  getimage(0, 0, 80, 80, imagens[3]); // captura para o ponteiro P
  getimage(0, 0, 80, 80, mascaras[3]); // captura para a m�scara M (a mesma imagem de P, que depois ser� manipulada na rotina PreparaImg)
  ImageConfig(TamP[3],imagens[3],mascaras[3]);
  
  readimagefile("a.bmp", 0, 0, 100, 100); // carrega a imagem
  imagens[11] = (unsigned char *)malloc(TamP[11]);
  mascaras[11] = (unsigned char *)malloc(TamP[11]);
  getimage(0, 0, 100, 100, imagens[11]); // captura para o ponteiro P
  getimage(0, 0, 100, 100, mascaras[11]); // captura para a m�scara M (a mesma imagem de P, que depois ser� manipulada na rotina PreparaImg)
  ImageConfig(TamP[11],imagens[11],mascaras[11]);

  readimagefile("BoitataT.bmp", 0, 0, 100, 100); // carrega a imagem
  imagens[12] = (unsigned char *)malloc(TamP[12]);
  mascaras[12] = (unsigned char *)malloc(TamP[12]);
  getimage(0, 0, 100, 100, imagens[12]); // captura para o ponteiro P
  getimage(0, 0, 100, 100, mascaras[12]); // captura para a m�scara M (a mesma imagem de P, que depois ser� manipulada na rotina PreparaImg)
  ImageConfig(TamP[12],imagens[12],mascaras[12]);
  
  
  cleardevice();// limpa a tela
  
  
}


void globalKeyListener()
{
  fflush(stdin);
  
  if (kbhit())
  {
    int key = getch(); 
    if(key == VK_ESCAPE)
    {
      gameConfig.inGame = !gameConfig.inGame;
      gameConfig.isPaused = !gameConfig.isPaused;
      if(gameConfig.inGame)
      {
        mciSendString("open \"./src/sounds/tema.mp3\" type mpegvideo alias tema", NULL, 0, NULL);
        // mciSendString("play mp3 notify repeat", NULL, 0, 0);
      }
    }
  }
}

void nothing()
{

}

void putImages(int id)
{
  putimage(0,0,mascaras[id],AND_PUT);
  putimage(0,0,imagens[id], OR_PUT);

  setfillstyle(1,RGB(0,0,0));
  projeteis[0].isLoaded ? fillellipse(projeteis[0].x, projeteis[0].y,projeteis[0].size,projeteis[0].size) : nothing();
  projeteis[1].isLoaded ? fillellipse(projeteis[1].x, projeteis[1].y,projeteis[1].size,projeteis[1].size) : nothing();
  projeteis[2].isLoaded ? fillellipse(projeteis[2].x, projeteis[2].y,projeteis[2].size,projeteis[2].size) : nothing();
  projeteis[3].isLoaded ? fillellipse(projeteis[3].x, projeteis[3].y,projeteis[3].size,projeteis[3].size) : nothing();
  projeteis[4].isLoaded ? fillellipse(projeteis[4].x, projeteis[4].y,projeteis[4].size,projeteis[4].size) : nothing();
  projeteis[5].isLoaded ? fillellipse(projeteis[5].x, projeteis[5].y,projeteis[5].size,projeteis[5].size) : nothing();
  projeteis[6].isLoaded ? fillellipse(projeteis[6].x, projeteis[6].y,projeteis[6].size,projeteis[6].size) : nothing();
  projeteis[7].isLoaded ? fillellipse(projeteis[7].x, projeteis[7].y,projeteis[7].size,projeteis[7].size) : nothing();
  projeteis[8].isLoaded ? fillellipse(projeteis[8].x, projeteis[8].y,projeteis[8].size,projeteis[8].size) : nothing();
  projeteis[9].isLoaded ? fillellipse(projeteis[9].x, projeteis[9].y,projeteis[9].size,projeteis[9].size) : nothing();
  projeteis[10].isLoaded ? fillellipse(projeteis[10].x, projeteis[10].y,projeteis[10].size,projeteis[10].size) : nothing();
  projeteis[11].isLoaded ? fillellipse(projeteis[11].x, projeteis[11].y,projeteis[11].size,projeteis[11].size) : nothing();
  projeteis[12].isLoaded ? fillellipse(projeteis[12].x, projeteis[12].y,projeteis[12].size,projeteis[12].size) : nothing();
  projeteis[13].isLoaded ? fillellipse(projeteis[13].x, projeteis[13].y,projeteis[13].size,projeteis[13].size) : nothing();
  projeteis[14].isLoaded ? fillellipse(projeteis[14].x, projeteis[14].y,projeteis[14].size,projeteis[14].size) : nothing();
  projeteis[15].isLoaded ? fillellipse(projeteis[15].x, projeteis[15].y,projeteis[15].size,projeteis[15].size) : nothing();
  projeteis[16].isLoaded ? fillellipse(projeteis[16].x, projeteis[16].y,projeteis[16].size,projeteis[16].size) : nothing();
  projeteis[17].isLoaded ? fillellipse(projeteis[17].x, projeteis[17].y,projeteis[17].size,projeteis[17].size) : nothing();
  projeteis[18].isLoaded ? fillellipse(projeteis[18].x, projeteis[18].y,projeteis[18].size,projeteis[18].size) : nothing();
  projeteis[19].isLoaded ? fillellipse(projeteis[19].x, projeteis[19].y,projeteis[19].size,projeteis[19].size) : nothing();
  projeteis[20].isLoaded ? fillellipse(projeteis[20].x, projeteis[20].y,projeteis[20].size,projeteis[20].size) : nothing();
  projeteis[21].isLoaded ? fillellipse(projeteis[21].x, projeteis[21].y,projeteis[21].size,projeteis[21].size) : nothing();
  projeteis[22].isLoaded ? fillellipse(projeteis[22].x, projeteis[22].y,projeteis[22].size,projeteis[22].size) : nothing();
  projeteis[23].isLoaded ? fillellipse(projeteis[23].x, projeteis[23].y,projeteis[23].size,projeteis[23].size) : nothing();
  projeteis[24].isLoaded ? fillellipse(projeteis[24].x, projeteis[24].y,projeteis[24].size,projeteis[24].size) : nothing();
  // projeteis[25].isLoaded ? fillellipse(projeteis[25].x, projeteis[25].y,projeteis[25].size,projeteis[25].size) : nothing();
  // projeteis[26].isLoaded ? fillellipse(projeteis[26].x, projeteis[26].y,projeteis[26].size,projeteis[26].size) : nothing();
  // projeteis[27].isLoaded ? fillellipse(projeteis[27].x, projeteis[27].y,projeteis[27].size,projeteis[27].size) : nothing();
  // projeteis[28].isLoaded ? fillellipse(projeteis[28].x, projeteis[28].y,projeteis[28].size,projeteis[28].size) : nothing();
  // projeteis[29].isLoaded ? fillellipse(projeteis[29].x, projeteis[29].y,projeteis[29].size,projeteis[29].size) : nothing();
  // projeteis[30].isLoaded ? fillellipse(projeteis[30].x, projeteis[30].y,projeteis[30].size,projeteis[30].size) : nothing();
  // projeteis[31].isLoaded ? fillellipse(projeteis[31].x, projeteis[31].y,projeteis[31].size,projeteis[31].size) : nothing();
  // projeteis[32].isLoaded ? fillellipse(projeteis[32].x, projeteis[32].y,projeteis[32].size,projeteis[32].size) : nothing();
  // projeteis[33].isLoaded ? fillellipse(projeteis[33].x, projeteis[33].y,projeteis[33].size,projeteis[33].size) : nothing();
  // projeteis[34].isLoaded ? fillellipse(projeteis[34].x, projeteis[34].y,projeteis[34].size,projeteis[34].size) : nothing();
  // projeteis[35].isLoaded ? fillellipse(projeteis[35].x, projeteis[35].y,projeteis[35].size,projeteis[35].size) : nothing();
  // projeteis[36].isLoaded ? fillellipse(projeteis[36].x, projeteis[36].y,projeteis[36].size,projeteis[36].size) : nothing();
  // projeteis[37].isLoaded ? fillellipse(projeteis[37].x, projeteis[37].y,projeteis[37].size,projeteis[37].size) : nothing();
  // projeteis[38].isLoaded ? fillellipse(projeteis[38].x, projeteis[38].y,projeteis[38].size,projeteis[38].size) : nothing();
  // projeteis[39].isLoaded ? fillellipse(projeteis[39].x, projeteis[39].y,projeteis[39].size,projeteis[39].size) : nothing();
  // projeteis[40].isLoaded ? fillellipse(projeteis[40].x, projeteis[40].y,projeteis[40].size,projeteis[40].size) : nothing();
  // projeteis[41].isLoaded ? fillellipse(projeteis[41].x, projeteis[41].y,projeteis[41].size,projeteis[41].size) : nothing();
  // projeteis[42].isLoaded ? fillellipse(projeteis[42].x, projeteis[42].y,projeteis[42].size,projeteis[42].size) : nothing();
  // projeteis[43].isLoaded ? fillellipse(projeteis[43].x, projeteis[43].y,projeteis[43].size,projeteis[43].size) : nothing();
  // projeteis[44].isLoaded ? fillellipse(projeteis[44].x, projeteis[44].y,projeteis[44].size,projeteis[44].size) : nothing();
  // projeteis[45].isLoaded ? fillellipse(projeteis[45].x, projeteis[45].y,projeteis[45].size,projeteis[45].size) : nothing();
  // projeteis[46].isLoaded ? fillellipse(projeteis[46].x, projeteis[46].y,projeteis[46].size,projeteis[46].size) : nothing();
  // projeteis[47].isLoaded ? fillellipse(projeteis[47].x, projeteis[47].y,projeteis[47].size,projeteis[47].size) : nothing();
  // projeteis[48].isLoaded ? fillellipse(projeteis[48].x, projeteis[48].y,projeteis[48].size,projeteis[48].size) : nothing();
  // projeteis[49].isLoaded ? fillellipse(projeteis[49].x, projeteis[49].y,projeteis[49].size,projeteis[49].size) : nothing();
  torres[0].isEnabled ? putimage(torres[0].PosX, torres[0].PosY, mascaras[0],AND_PUT) : nothing();
  torres[0].isEnabled ? putimage(torres[0].PosX, torres[0].PosY, imagens[0], OR_PUT) : nothing();
  torres[1].isEnabled ? putimage(torres[1].PosX, torres[1].PosY, mascaras[0],AND_PUT) : nothing();
  torres[1].isEnabled ? putimage(torres[1].PosX, torres[1].PosY, imagens[0], OR_PUT) : nothing();
  torres[2].isEnabled ? putimage(torres[2].PosX, torres[2].PosY, mascaras[0],AND_PUT) : nothing();
  torres[2].isEnabled ? putimage(torres[2].PosX, torres[2].PosY, imagens[0], OR_PUT) : nothing();
  torres[3].isEnabled ? putimage(torres[3].PosX, torres[3].PosY, mascaras[0],AND_PUT) : nothing();
  torres[3].isEnabled ? putimage(torres[3].PosX, torres[3].PosY, imagens[0], OR_PUT) : nothing();
  torres[4].isEnabled ? putimage(torres[4].PosX, torres[4].PosY, mascaras[0],AND_PUT) : nothing();
  torres[4].isEnabled ? putimage(torres[4].PosX, torres[4].PosY, imagens[0], OR_PUT) : nothing();
  torres[5].isEnabled ? putimage(torres[5].PosX, torres[5].PosY, mascaras[0],AND_PUT) : nothing();
  torres[5].isEnabled ? putimage(torres[5].PosX, torres[5].PosY, imagens[0], OR_PUT) : nothing();
  torres[6].isEnabled ? putimage(torres[6].PosX, torres[6].PosY, mascaras[0],AND_PUT) : nothing();
  torres[6].isEnabled ? putimage(torres[6].PosX, torres[6].PosY, imagens[0], OR_PUT) : nothing();
  inimigos[0].isEnabled ? putimage(inimigos[0].PosX, inimigos[0].PosY, mascaras[3],AND_PUT) : nothing();
  inimigos[0].isEnabled ? putimage(inimigos[0].PosX, inimigos[0].PosY, imagens[3], OR_PUT) : nothing();
  inimigos[1].isEnabled ? putimage(inimigos[1].PosX, inimigos[1].PosY, mascaras[3],AND_PUT) : nothing();
  inimigos[1].isEnabled ? putimage(inimigos[1].PosX, inimigos[1].PosY, imagens[3], OR_PUT) : nothing();
  inimigos[2].isEnabled ? putimage(inimigos[2].PosX, inimigos[2].PosY, mascaras[3],AND_PUT) : nothing();
  inimigos[2].isEnabled ? putimage(inimigos[2].PosX, inimigos[2].PosY, imagens[3], OR_PUT) : nothing();
  inimigos[3].isEnabled ? putimage(inimigos[3].PosX, inimigos[3].PosY, mascaras[12],AND_PUT) : nothing();
  inimigos[3].isEnabled ? putimage(inimigos[3].PosX, inimigos[3].PosY, imagens[12], OR_PUT) : nothing();
  
  /*putimage(colisionRoutesBoxes[0].x, colisionRoutesBoxes[0].y, mascaras[12],AND_PUT);
  putimage(colisionRoutesBoxes[0].x, colisionRoutesBoxes[0].y, imagens[12], OR_PUT);
  putimage(colisionRoutesBoxes[1].x, colisionRoutesBoxes[1].y, mascaras[12],AND_PUT);
  putimage(colisionRoutesBoxes[1].x, colisionRoutesBoxes[1].y, imagens[12] , OR_PUT);
  putimage(colisionRoutesBoxes[2].x, colisionRoutesBoxes[2].y, mascaras[12],AND_PUT);
  putimage(colisionRoutesBoxes[2].x, colisionRoutesBoxes[2].y, imagens[12] , OR_PUT);
  putimage(colisionRoutesBoxes[3].x, colisionRoutesBoxes[3].y, mascaras[12],AND_PUT);
  putimage(colisionRoutesBoxes[3].x, colisionRoutesBoxes[3].y, imagens[12] , OR_PUT);
  putimage(colisionRoutesBoxes[4].x, colisionRoutesBoxes[4].y, mascaras[12],AND_PUT);
  putimage(colisionRoutesBoxes[4].x, colisionRoutesBoxes[4].y, imagens[12] , OR_PUT);
  putimage(colisionRoutesBoxes[5].x, colisionRoutesBoxes[5].y, mascaras[12],AND_PUT);
  putimage(colisionRoutesBoxes[5].x, colisionRoutesBoxes[5].y, imagens[12] , OR_PUT);
  putimage(colisionRoutesBoxes[6].x, colisionRoutesBoxes[6].y, mascaras[12],AND_PUT);
  putimage(colisionRoutesBoxes[6].x, colisionRoutesBoxes[6].y, imagens[12] , OR_PUT);
  putimage(colisionRoutesBoxes[7].x, colisionRoutesBoxes[7].y, mascaras[12],AND_PUT);
  putimage(colisionRoutesBoxes[7].x, colisionRoutesBoxes[7].y, imagens[12] , OR_PUT);
  putimage(colisionRoutesBoxes[8].x, colisionRoutesBoxes[8].y, mascaras[12],AND_PUT);
  putimage(colisionRoutesBoxes[8].x, colisionRoutesBoxes[8].y, imagens[12] , OR_PUT);
  putimage(colisionRoutesBoxes[9].x, colisionRoutesBoxes[9].y, mascaras[12],AND_PUT);
  putimage(colisionRoutesBoxes[9].x, colisionRoutesBoxes[9].y, imagens[12] , OR_PUT);
  putimage(colisionRoutesBoxes[10].x, colisionRoutesBoxes[10].y, mascaras[12],AND_PUT);
  putimage(colisionRoutesBoxes[10].x, colisionRoutesBoxes[10].y, imagens[12] , OR_PUT);
  putimage(colisionRoutesBoxes[11].x, colisionRoutesBoxes[11].y, mascaras[12],AND_PUT);
  putimage(colisionRoutesBoxes[11].x, colisionRoutesBoxes[11].y, imagens[12] , OR_PUT);
  putimage(colisionRoutesBoxes[12].x, colisionRoutesBoxes[12].y, mascaras[12],AND_PUT);
  putimage(colisionRoutesBoxes[12].x, colisionRoutesBoxes[12].y, imagens[12] , OR_PUT);
  putimage(colisionRoutesBoxes[13].x, colisionRoutesBoxes[13].y, mascaras[12],AND_PUT);
  putimage(colisionRoutesBoxes[13].x, colisionRoutesBoxes[13].y, imagens[12] , OR_PUT);
  putimage(colisionRoutesBoxes[14].x, colisionRoutesBoxes[14].y, mascaras[12],AND_PUT);
  putimage(colisionRoutesBoxes[14].x, colisionRoutesBoxes[14].y, imagens[12] , OR_PUT);
  putimage(colisionRoutesBoxes[15].x, colisionRoutesBoxes[15].y, mascaras[12],AND_PUT);
  putimage(colisionRoutesBoxes[15].x, colisionRoutesBoxes[15].y, imagens[12] , OR_PUT);
  putimage(colisionRoutesBoxes[16].x, colisionRoutesBoxes[16].y, mascaras[12],AND_PUT);
  putimage(colisionRoutesBoxes[16].x, colisionRoutesBoxes[16].y, imagens[12] , OR_PUT);*/
  
  
  setfillstyle(1,BLACK);
  rectangle(10,10,10,10);
}
void TowerColision()
{
  if(torres[gameConfig.colisionsIndex])
}

void MoveTroupes()
{
		//rota 1
	if (inimigos[gameConfig.enemiesIndex].PosX >= 760 && 
      inimigos[gameConfig.enemiesIndex].PosX <= 860 && 
      inimigos[gameConfig.enemiesIndex].PosY >= 220 && 
      inimigos[gameConfig.enemiesIndex].PosY <= 320) 
  {
		inimigos[gameConfig.enemiesIndex].vetX = -1; // esq
		inimigos[gameConfig.enemiesIndex].vetY = 0;
	} 
	
	if (inimigos[gameConfig.enemiesIndex].PosX >= 310 && 
      inimigos[gameConfig.enemiesIndex].PosX <= 410 && 
      inimigos[gameConfig.enemiesIndex].PosY >= 256 && 
      inimigos[gameConfig.enemiesIndex].PosY <= 356) 
  {
		inimigos[gameConfig.enemiesIndex].vetX = 0;
		inimigos[gameConfig.enemiesIndex].vetY = -1; // cima
	} 
	
	if (inimigos[gameConfig.enemiesIndex].PosX >= 310 && 
      inimigos[gameConfig.enemiesIndex].PosX <= 410 && 
      inimigos[gameConfig.enemiesIndex].PosY >= 155 && 
      inimigos[gameConfig.enemiesIndex].PosY <= 255) 
  {
		inimigos[gameConfig.enemiesIndex].vetX = -1;
		inimigos[gameConfig.enemiesIndex].vetY = 0; 
	} 
	
	if (inimigos[gameConfig.enemiesIndex].PosX >= 20  && 
      inimigos[gameConfig.enemiesIndex].PosX <= 120 && 
      inimigos[gameConfig.enemiesIndex].PosY >= 150 && 
      inimigos[gameConfig.enemiesIndex].PosY <= 250) 
  {
		inimigos[gameConfig.enemiesIndex].vetX = 0;
		inimigos[gameConfig.enemiesIndex].vetY = 1; 
	} 
	
	if (inimigos[gameConfig.enemiesIndex].PosX >= 50  &&
      inimigos[gameConfig.enemiesIndex].PosX <= 150 && 
      inimigos[gameConfig.enemiesIndex].PosY >= 380 && 
      inimigos[gameConfig.enemiesIndex].PosY <= 480) 
  {
		inimigos[gameConfig.enemiesIndex].vetX = -1;
		inimigos[gameConfig.enemiesIndex].vetY = 0; 
	} 
	
	// rota 2
	if (inimigos[gameConfig.enemiesIndex].PosX >= 880 &&
      inimigos[gameConfig.enemiesIndex].PosX <= 980 && 
      inimigos[gameConfig.enemiesIndex].PosY >= 530 && 
      inimigos[gameConfig.enemiesIndex].PosY <= 670) 
  {
		inimigos[gameConfig.enemiesIndex].vetX = 0;
		inimigos[gameConfig.enemiesIndex].vetY = -1; 
	} 
	
	if (inimigos[gameConfig.enemiesIndex].PosX >= 880 && 
      inimigos[gameConfig.enemiesIndex].PosX <= 980 && 
      inimigos[gameConfig.enemiesIndex].PosY >= 220 && 
      inimigos[gameConfig.enemiesIndex].PosY <= 320) 
  {
		inimigos[gameConfig.enemiesIndex].vetX = -1;
		inimigos[gameConfig.enemiesIndex].vetY = 0; 
	} 
	
	
	// rota 3
	if (inimigos[gameConfig.enemiesIndex].PosX >= 500 && 
      inimigos[gameConfig.enemiesIndex].PosX <= 600 && 
      inimigos[gameConfig.enemiesIndex].PosY >= 50  && 
      inimigos[gameConfig.enemiesIndex].PosY <= 150) 
  {
		inimigos[gameConfig.enemiesIndex].vetX = 1;
		inimigos[gameConfig.enemiesIndex].vetY = 0; 
	} 
	
	if (inimigos[gameConfig.enemiesIndex].PosX >= 765 && 
      inimigos[gameConfig.enemiesIndex].PosX <= 865 && 
      inimigos[gameConfig.enemiesIndex].PosY >= 50  && 
      inimigos[gameConfig.enemiesIndex].PosY <= 150) 
  {
		inimigos[gameConfig.enemiesIndex].vetX = 0;
		inimigos[gameConfig.enemiesIndex].vetY = 1; 
	} 
	
	if (inimigos[gameConfig.enemiesIndex].PosX >= 755 && 
      inimigos[gameConfig.enemiesIndex].PosX <= 855 && 
      inimigos[gameConfig.enemiesIndex].PosY >= 140 && 
      inimigos[gameConfig.enemiesIndex].PosY <= 240) 
  {
		inimigos[gameConfig.enemiesIndex].vetX = -1;
		inimigos[gameConfig.enemiesIndex].vetY = 0; 
	} 
	
	if (inimigos[gameConfig.enemiesIndex].PosX >= 648 && 
      inimigos[gameConfig.enemiesIndex].PosX <= 548 && 
      inimigos[gameConfig.enemiesIndex].PosY >= 140 && 
      inimigos[gameConfig.enemiesIndex].PosY <= 240) 
  {
		inimigos[gameConfig.enemiesIndex].vetX = 0;
		inimigos[gameConfig.enemiesIndex].vetY = 1; 
	} 
	
  inimigos[gameConfig.enemiesIndex].PosX += inimigos[gameConfig.enemiesIndex].vetX * inimigos[gameConfig.enemiesIndex].speed;
  inimigos[gameConfig.enemiesIndex].PosY += inimigos[gameConfig.enemiesIndex].vetY * inimigos[gameConfig.enemiesIndex].speed;
}

void EnemySpawn()
{
  inimigos[0].isEnabled = true;
  inimigos[0].PosX = 100;
  inimigos[0].PosY = 300;
}

void ProjectileTrigger()
{
if(projeteis[gameConfig.projectileIndex].x > 1400 || 
     projeteis[gameConfig.projectileIndex].x < -100 || 
     projeteis[gameConfig.projectileIndex].y > 1000 || 
     projeteis[gameConfig.projectileIndex].y < -100)
  {
    projeteis[gameConfig.projectileIndex].isLoaded = false;
  }
  if(projeteis[gameConfig.projectileIndex].isLoaded)
  {
    projeteis[gameConfig.projectileIndex].x -= projeteis[gameConfig.projectileIndex].speed * projeteis[gameConfig.projectileIndex].Xvet;
    projeteis[gameConfig.projectileIndex].y -= projeteis[gameConfig.projectileIndex].speed * projeteis[gameConfig.projectileIndex].Yvet;
  }
}

void GlobalIndexMover()
{
  if(gameConfig.inGame && !gameConfig.inCutscene)
  {
  gameConfig.enemiesMoveIndex++;
  gameConfig.projectileIndex++;
  gameConfig.enemiesIndex++;
  gameConfig.colisionsIndex++;
  if(gameConfig.enemiesIndex == 7)
  {
    gameConfig.enemiesIndex = 0;
  }  
  if(gameConfig.enemiesMoveIndex == 7)
  {
    gameConfig.enemiesMoveIndex = 0;
  }
  if(gameConfig.projectileIndex == 25)
  {
    gameConfig.projectileIndex = 0;
  }
  if(gameConfig.colisionsIndex == 7)
  {
    gameConfig.colisionsIndex = 0;
  }
  ProjectileTrigger();
  }
  
}

int main()  
{
	mciSendString("open \"./src/titulo.mp3\" type mpegvideo alias menu", NULL, 0, NULL);
	int pg = 1;
	POINT mouse;
	int intro = 1;
	char output[256];
	GetCursorPos(&mouse);
	float mousex = mouse.x;
	float mousey = mouse.y;
	int posXt = 50;
	int vel = 0;	
	startGameConfig();
	initwindow(gameConfig.resolution[0], gameConfig.resolution[1]);	
	imagesRenderer();
	setPlayerConfig();
	setRoutesConfig();
	backgroundConfig();
	projectileConfig();
  
  while(1)
  {
    settextstyle(9,HORIZ_DIR,13);
    GetCursorPos(&mouse);
    mousex = mouse.x;
    mousey = mouse.y;
    if (pg == 1) pg = 2; else pg = 1;
    setactivepage(pg);
    cleardevice();    
    Move();
    GlobalIndexMover();
    putImages(gameConfig.scene); 
    setvisualpage(pg);
    if (gameConfig.first)
    {
      gameConfig.frames = 0;
      gameConfig.starttime = gameConfig.timePassed;
      gameConfig.first = false;
    }
    //Clock inicial para controle de frame
    globalKeyListener();

    // cutscene
    if (gameConfig.inCutscene) 
    {
    	// cutscene (?)
    	mciSendString("stop menu", NULL, 0, NULL);
    	gameConfig.scene = 4;
    	if (intro == 1) mciSendString("play src/Intro.wav", NULL, 0, NULL);
    	outtextxy(posXt,200,"MONTE SEU EX�RCITO...");
    	if (posXt <= 150) {
    		intro = 0;
    		vel += 6;
    		posXt += vel;
		} else {
			if (vel >= 2) {
				vel -= 2;
			} else if (vel < 2) {
				vel = 2;
			}
		}
		posXt += vel;
		if (posXt >= 400) {
			gameConfig.inGame = true;
			gameConfig.inCutscene = false;
		}
	}
    
    
    //in game
    if(gameConfig.inGame && gameConfig.inCutscene == false) 
    {
      sprintf(output,"$ %d", gameConfig.money);
      if (intro == 0) 
      {
        mciSendString("open \"./sound/src/tema.mp3\" type mpegvideo alias mp3", NULL, 0, NULL);
        mciSendString("play mp3 repeat", NULL, 0, NULL);
        intro = 2;
      }
      posXt = 2000;
      gameConfig.scene = 5;
      
      if (gameConfig.horda == 0) 
      {
        outtextxy(1185,615,output);
        // checa se o mouse ta sobre o estilingue
        if ( mousex >= 1061 && mousex <= 1116 && mousey >= 673 && mousey <= 723 && gameConfig.select == 0 ) 
        {
          gameConfig.scene = 6;
          outtextxy(mousex,mousey-10,"estilingue $50");
          if ((GetKeyState(VK_LBUTTON) & 0x80) != 0) 
          {
            mciSendString("play src/Select.wav", NULL, 0, NULL);
            gameConfig.select = 1; //seleciona estilingue 
          }
			  }
			
			//checa se o estilingue ta selecionado
			if ( gameConfig.select == 1 ) {
				gameConfig.scene = 6;
				escolhendopos(7,50);
				
				//caso o player desista da sele��o
				if ((GetKeyState(VK_RBUTTON) & 0x80) != 0) {
					mciSendString("play src/Cancel.wav", NULL, 0, NULL);
					gameConfig.select = 0;
				}
			}
			
			//checa se o mouse t� sobre a seta
			if ( gameConfig.select == 0 && mousex >= 1165 && mousex <= 1270 && mousey >= 11 && mousey <= 102 ) {
				gameConfig.scene = 10;
				if ((GetKeyState(VK_LBUTTON) & 0x80) != 0) { // checa se o player clicou na seta
					mciSendString("play src/Start.wav", NULL, 0, NULL);
					Sleep(1500);
					gameConfig.scene = 5; 
					setInitialEnemyConfig();
					gameConfig.horda = 1; // come�a o ataque
				}
			}
		}
	
	
	
		if(gameConfig.horda == 1) 
    {
			gameConfig.scene = 2; 
			MoveTroupes();
			Colision();
      Move();
		}
	}
	


	//game menu
    if(gameConfig.menu) {
	mciSendString("play menu repeat", NULL, 0, NULL);
      gameConfig.scene = 8;
	  if ( mousex >= 1086 && mousex <= 1266 && mousey <= 562 && mousey >= 405 ) {
        	gameConfig.scene = 9;
        	if ((GetKeyState(VK_LBUTTON) & 0x80) != 0) {
        		gameConfig.menu = false;
        		gameConfig.inCutscene = true;
			}
		}
      
    } 
    
    //game paused
    if(gameConfig.isPaused && gameConfig.inCutscene == false) {
      if (pg == 1) pg = 2; else pg = 1;
      setactivepage(pg);
      cleardevice(); 
      putimage(0, 0, mascaras[4], AND_PUT);
      putimage(0, 0, imagens[4], OR_PUT);
      if(blink >= 100)
      {
        outtextxy(300,200,"aperta o esc ai");
        if(blink == 200)
        {
          blink = 0;
        }
      }
      if(blink <= 200)
      {
        blink++;
      }
      setvisualpage(pg);
    }
    gameConfig.frames++;
    
    if (gameConfig.timePassed - gameConfig.starttime > 0.25 && gameConfig.frames > 10)
    {
      gameConfig.fps = (double) gameConfig.frames / (gameConfig.timePassed - gameConfig.starttime);
      gameConfig.starttime = gameConfig.timePassed;
      gameConfig.frames = 0;
    }
    delay(gameConfig.fps);
  }
  
  
  //while(!kbhit());	
  //closegraph();	
  return 0; 
}
