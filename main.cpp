#include<stdio.h>
#include<stdlib.h>
#include<iostream>
#include<graphics.h>


using namespace std;

#define ESC    	27


unsigned char *imagens[2];
unsigned char *mascaras[2];
int TamP[2];
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
};

struct personagem
{
  int PosX;
  int PosY;
  int VetorAccX;
  int VetorAccY;
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
  int width;
  int size;
  int speed;
};

struct projectile projeteis[];
struct personagem personagem1; 
struct gameConfiguration gameConfig;

void startGameConfig()
{
  gameConfig.isPaused = true;
  gameConfig.inGame = false;
}

void setPlayerConfig()
{
  personagem1.PosX = 400;
  personagem1.PosY = 400;
  personagem1.maxJump = -220;
  personagem1.ground = 400;
  personagem1.can_jump = true;
  personagem1.VetorAccX = 5;
  personagem1.VetorAccY = 5;
  personagem1.actual_jump_stamina = 0;
  personagem1.jump_stamina = 120;
  personagem1.direction = 'none';
  personagem1.state = 'running';
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
  }
  if(GetKeyState(VK_UP)&0x80 && personagem1.can_jump)
  {
  	// printf("%i",personagem1.actual_jump_stamina);
  	// personagem1.state = 'jumping';
    // personagem1.PosY += -10;
    // if(personagem1.actual_jump_stamina > 0)
    // {
    //   personagem1.actual_jump_stamina -= 4;
    // }
    // if(personagem1.actual_jump_stamina == 0)
    // {
    //   personagem1.can_jump = false; 
    // }
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
      personagem1.inGame = !personagem1.inGame;
      personagem1.isPaused = !personagem1.isPaused;
    }
  }
}

int main()  
{ 
  int pg = 1;
  
  initwindow(800, 600);	
  setPlayerConfig();
  imagesRenderer();
  backgroundConfig();
  while(1)
  {
    globalKeyListener();
    if(personagem1.inGame)
    {
      if (pg == 1) pg = 2; else pg = 1;
      setactivepage(pg);
      cleardevice();    
      putimage(personagem1.PosX, personagem1.PosY, mascaras[0],AND_PUT);
      putimage(personagem1.PosX, personagem1.PosY, imagens[0], OR_PUT);
      Move(); 
      StateCheck(personagem1.direction);
      Gravity();
      setvisualpage(pg);
      

      delay(10);
    }
    if(personagem1.isPaused)
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
  }
  
  
  //while(!kbhit());	
  //closegraph();	
  return 0; 
}

