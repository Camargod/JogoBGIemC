#include<stdio.h>
#include<stdlib.h>
#include<iostream>
#include<graphics.h>
#include<time.h>

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
struct personagem personagem1;
struct personagem personagem2; 
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
  }
}
void setPlayerConfig()
{
  personagem1.PosX = 400;
  personagem1.PosY = 400;
  personagem1.maxJump = -220;
  personagem1.ground = 400;
  personagem1.can_jump = true;
  personagem1.VetorAccX = 10;
  personagem1.VetorAccY = 10;
  personagem1.actual_jump_stamina = 0;
  personagem1.jump_stamina = 120;
  personagem1.direction = 'none';
  personagem1.state = 'running';

  personagem2.PosX = 600;
  personagem2.PosY = 600;
  personagem2.maxJump = -220;
  personagem2.ground = 600;
  personagem2.can_jump = true;
  personagem2.VetorAccX = 5;
  personagem2.VetorAccY = 5;
  personagem2.actual_jump_stamina = 0;
  personagem2.jump_stamina = 120;
  personagem2.direction = 'none';
  personagem2.state = 'running';
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
void StateCheck(char direction)
{
	if(personagem1.state == 'running' && direction == 'right')
	{
		personagem1.VetorAccX = 5;
	}
	if(personagem1.state == 'running' && direction == 'left')
	{
		personagem1.VetorAccX = 5;
	}
	if(personagem1.state == 'jumping')
	{
		personagem1.VetorAccY = -10;
	}
	if(personagem1.state == 'jumping' && direction == 'left')
	{
		personagem1.VetorAccY = -10;
		personagem1.VetorAccX = -36;
	}
	if(personagem1.state == 'jumping' && direction == 'right')
	{
		personagem1.VetorAccY = -10;
		personagem1.VetorAccX = 36;
	}
  if(personagem1.state == 'jumping' && direction == 'none')
	{
		personagem1.VetorAccY = -10;
		personagem1.VetorAccX = 0;
	}
}
#pragma region Mecância de cordenada de tiro
void shoot(int posInit[2], int posFinal[2], int structureSpeed)
{
  if(!projeteis[gameConfig.index].isLoaded)
  {
    projeteis[gameConfig.index].isLoaded = true;
    projeteis[gameConfig.index].x = posInit[0];
    projeteis[gameConfig.index].y = posInit[1];
    projeteis[gameConfig.index].speed = structureSpeed;
    projeteis[gameConfig.index].size = 10;
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
    printf("Index de Projeteis: %i\n",gameConfig.index);
    gameConfig.index++;
  }

}
#pragma endregion
void Move()
{
	
  if(GetKeyState(VK_LEFT)&0x80)
  {
  	personagem1.direction='left';
    personagem1.PosX -= personagem1.VetorAccX;
  }
  if(GetKeyState(VK_RIGHT)&0x80)
  {
  	personagem1.direction='right';
    personagem1.PosX += personagem1.VetorAccX;
    // moveto(500,300);
  }
  if(GetKeyState(VK_UP)&0x80 && personagem1.can_jump)
  {
  	// printf("%i",personagem1.actual_jump_stamina);
  	personagem1.state = 'jumping';
    personagem1.PosY += -10;
    if(personagem1.actual_jump_stamina > 0)
    {
      personagem1.actual_jump_stamina -= 4;
    }
    if(personagem1.actual_jump_stamina == 0)
    {
      personagem1.can_jump = false; 
    }
    int posIni[2];
    posIni[0] = personagem1.PosX;
    posIni[1] = personagem1.PosY;
    int posFinal[2];
    posFinal[0] = personagem2.PosX;
    posFinal[1] = personagem2.PosY;
    shoot(posIni,posFinal,10);
	}
  else
  {
    if(personagem1.actual_jump_stamina != personagem1.jump_stamina)
    {
      personagem1.actual_jump_stamina += 4;
      printf("%i",personagem1.actual_jump_stamina);
    }
    else
    {
      personagem1.can_jump = true;
    }
  }
}
  /*if(GetKeyState(VK_DOWN)&0x80)
  {
    personagem1.PosY +=10;
  }*/

void Gravity()
{
  personagem1.PosY += 5;
  if(personagem1.PosY >= personagem1.ground)
  {
  	personagem1.PosY = personagem1.ground;
  	personagem1.can_jump = true;
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
    }
  }
}

void putImages()
{
  putimage(0,0,mascaras[2],AND_PUT);
  putimage(0,0,imagens[2], OR_PUT);
  putimage(personagem1.PosX, personagem1.PosY, mascaras[0],AND_PUT);
  putimage(personagem1.PosX, personagem1.PosY, imagens[0], OR_PUT);
  putimage(personagem2.PosX, personagem2.PosY, mascaras[0],AND_PUT);
  putimage(personagem2.PosX, personagem2.PosY, imagens[0], OR_PUT);
  if(projeteis[gameConfig.projectileIndex].isLoaded)
  {
    setfillstyle(1,RGB(0,0,0));
    fillellipse(projeteis[gameConfig.projectileIndex].x, projeteis[gameConfig.projectileIndex].y,projeteis[gameConfig.projectileIndex].size,projeteis[gameConfig.projectileIndex].size);
    projeteis[gameConfig.projectileIndex].x += projeteis[gameConfig.projectileIndex].speed * projeteis[gameConfig.projectileIndex].Xvet;
    projeteis[gameConfig.projectileIndex].y += projeteis[gameConfig.projectileIndex].speed * projeteis[gameConfig.projectileIndex].Yvet;
  }
  gameConfig.projectileIndex -=- 1;
  if(gameConfig.projectileIndex == 100)
  {
    gameConfig.projectileIndex = 0;
  }
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
      StateCheck(personagem1.direction);
      Gravity();
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

