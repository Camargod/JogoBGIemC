#include<stdio.h>
#include<stdlib.h>
#include<iostream>
#include<graphics.h>
#include<time.h>

#include <windows.h>
#include "MMSystem.h"
#pragma comment(lib, "winmm.lib");
using namespace std;

#define ESC    	27


unsigned char *imagens[3];
unsigned char *mascaras[3];
int TamP[3];
//Aqui está o esquema de camadas para trabalhar com UI
int layer = 0;
int blink = 0;

struct gameConfigurations
{
  bool isPaused;
  bool inGame;
  bool money;
  bool time;
  bool round;
  int resolution[2];
  int frames;
  double starttime;
  bool first;
  float fps;
  float timePassed;
  int index;
  int projectileIndex;
};

int projectileNumber = 0;
struct personagem
{
  float PosX;
  float PosY;
  float VetorAccX;
  float VetorAccY;
  char state;
  char src;
  int maxJump;
  int ground;
  bool can_jump;
  int jump_stamina;
  int actual_jump_stamina;
  char direction;

};

struct projectile
{
  int size;
  int speed;
  bool isLoaded;
  int x;
  int y;
  int initX;
  int initY;
  int finX;
  int finY;
  int Xvet;
  int Yvet;
};

struct gameConfigurations gameConfig;
struct personagem torres[7];
struct personagem inimigos[20]; 
struct projectile projeteis[100];


void startGameConfig()
{
  gameConfig.isPaused = true;
  gameConfig.inGame = false;
  gameConfig.resolution[0] = 1280;
  gameConfig.resolution[1] = 720;
  gameConfig.frames = 0;
  gameConfig.starttime = 0;
  gameConfig.first = TRUE;
  gameConfig.fps = 0.0f;
  gameConfig.index = 0;
}
void projectileConfig()
{
  for(int index = 0; index < 100; index -=-1)
  {
    projeteis[index].isLoaded = false;
    projeteis[index].x = 5000;
  }
}
void setPlayerConfig()
{
  torres[0].PosX = 420;
  torres[0].PosY = 420;

  inimigos[0].PosX = 730;
  inimigos[0].PosY = 580;
  inimigos[0].maxJump = -220;
  inimigos[0].ground = 600;
  inimigos[0].can_jump = true;
  inimigos[0].VetorAccX = 5;
  inimigos[0].VetorAccY = 5;
  inimigos[0].actual_jump_stamina = 0;
  inimigos[0].jump_stamina = 120;
  //personagem1.src = './src/Dude_Monster.png';
  //personagem1.state = 'idle';
}

void backgroundConfig() 
{
	setbkcolor(RGB(70, 170, 70));
}

void ImageConfig(int Tam, unsigned char *Img, unsigned char *Msk) 
{
// Tam � o tamanho dos ponteiros da imagem e da m�scara
// Img � o ponteiro que aponta para o vetor que cont�m a imagem capturada com getimage(..)
// Msk � o ponteiro que ser� transformado em m�scara
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

#pragma region Mecância de cordenada de tiro
void shoot(int posInit[2], int posFinal[2], int structureSpeed)
{
  if(!projeteis[gameConfig.index].isLoaded)
  {
    // malloc(int * sizeof(int *))
    projeteis[gameConfig.index].isLoaded = true;
    projeteis[gameConfig.index].x = posInit[0];
    projeteis[gameConfig.index].y = posInit[1];
    projeteis[gameConfig.index].speed = structureSpeed;
    projeteis[gameConfig.index].size = 4;
    if(projeteis[gameConfig.index].finX - projeteis[gameConfig.index].initX < 0)
    {
      projeteis[gameConfig.index].Xvet = -1;
    }
    else
    {
      projeteis[gameConfig.index].Xvet = 1;
    }
    if(projeteis[gameConfig.index].finY - projeteis[gameConfig.index].initY < 0)
    {
      projeteis[gameConfig.index].Yvet = -1;
    }
    else
    {
      projeteis[gameConfig.index].Yvet = 1;
    }
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
  TamP[0] = imagesize(0, 0, 31, 31);
  TamP[1] = imagesize(0, 0, 649, 479);
  TamP[2] = imagesize(0,0,1279,719);

  readimagefile("mainScene.bmp",0 , 0 , 1279, 719); // carrega a imagem
  imagens[2] = (unsigned char *)malloc(TamP[2]);
  mascaras[2] = (unsigned char *)malloc(TamP[2]);
  getimage(0, 0, 1279, 719, imagens[2]); // captura para o ponteiro P
  getimage(0, 0, 1279, 719, mascaras[2]); // captura para a m�scara M (a mesma imagem de P, que depois ser� manipulada na rotina PreparaImg)
  ImageConfig(TamP[2],imagens[2],mascaras[2]);

  readimagefile("Sans.bmp",0 , 0 , 31, 31); // carrega a imagem
  imagens[0] = (unsigned char *)malloc(TamP[0]);
  mascaras[0] = (unsigned char *)malloc(TamP[0]);
  getimage(0, 0, 31, 31, imagens[0]); // captura para o ponteiro P
  getimage(0, 0, 31, 31, mascaras[0]); // captura para a m�scara M (a mesma imagem de P, que depois ser� manipulada na rotina PreparaImg)
  ImageConfig(TamP[0],imagens[0],mascaras[0]);

  readimagefile("pause.BMP",0 , 0 , 639, 479); // carrega a imagem
  imagens[1] = (unsigned char *)malloc(TamP[1]);
  mascaras[1] = (unsigned char *)malloc(TamP[1]);
  getimage(0, 0, 649, 479, imagens[1]); // captura para o ponteiro P
  getimage(0, 0, 649, 479, mascaras[1]); // captura para a m�scara M (a mesma imagem de P, que depois ser� manipulada na rotina PreparaImg)
  ImageConfig(TamP[1],imagens[1],mascaras[1]);

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
        mciSendString("open \"./src/sounds/tema.mp3\" type mpegvideo alias mp3", NULL, 0, NULL);
        mciSendString("play mp3 notify repeat", NULL, 0, 0);
      }
    }
  }
}
void nothing()
{

}
void putImages()
{
  putimage(0,0,mascaras[2],AND_PUT);
  putimage(0,0,imagens[2], OR_PUT);
  putimage(torres[0].PosX, torres[0].PosY, mascaras[0],AND_PUT);
  putimage(torres[0].PosX, torres[0].PosY, imagens[0], OR_PUT);
  putimage(inimigos[0].PosX, inimigos[0].PosY, mascaras[0],AND_PUT);
  putimage(inimigos[0].PosX, inimigos[0].PosY, imagens[0], OR_PUT);

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
  rectangle(10,10,10,10);
  if(projeteis[gameConfig.projectileIndex].x - projeteis[gameConfig.projectileIndex].initX  > 2000 || projeteis[gameConfig.projectileIndex].x - projeteis[gameConfig.projectileIndex].initX < -200 || projeteis[gameConfig.projectileIndex].y > 1000 || projeteis[gameConfig.projectileIndex].y < -100 )
  {
    projeteis[gameConfig.projectileIndex].isLoaded = false;
    projeteis[gameConfig.projectileIndex].x = 5000;
  }
  if(projeteis[gameConfig.projectileIndex].isLoaded)
  {
    projeteis[gameConfig.projectileIndex].x += projeteis[gameConfig.projectileIndex].speed * projeteis[gameConfig.projectileIndex].Xvet;
    projeteis[gameConfig.projectileIndex].y += projeteis[gameConfig.projectileIndex].speed * projeteis[gameConfig.projectileIndex].Yvet;
  }
  if(gameConfig.projectileIndex == 25)
  {
    gameConfig.projectileIndex = 0;
  }
  gameConfig.projectileIndex++;

}

void Colision()
{

}

int main()  
{ 
  int pg = 1;
  startGameConfig();
  initwindow(gameConfig.resolution[0], gameConfig.resolution[1]);	
  setPlayerConfig();
  imagesRenderer();
  backgroundConfig();
  projectileConfig();
  while(1)
  {

    if (gameConfig.first)
    {
        gameConfig.frames = 0;
        gameConfig.starttime = gameConfig.timePassed;
        gameConfig.first = FALSE;
    }
    //Clock inicial para controle de frame
    globalKeyListener();
    if(gameConfig.inGame)
    {
      if (pg == 1) pg = 2; else pg = 1;
      setactivepage(pg);
      cleardevice();    
      putImages();
      Move();
      putImages(); 
      Colision();
      putImages();
      // MoveTroupes();
      // StateCheck(personagem1.direction);
      putImages();
      setvisualpage(pg);
    }
    if(gameConfig.isPaused)
    {
      if (pg == 1) pg = 2; else pg = 1;
      setactivepage(pg);
      cleardevice(); 
      putimage(0, 0, mascaras[1], AND_PUT);
      putimage(0, 0, imagens[1], OR_PUT);
      if(blink >= 160)
      {
        outtextxy(300,200,"Tela de pause mesmo rapaz");
        if(blink == 320)
        {
          blink = 0;
        }
      }
      if(blink <= 320)
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

