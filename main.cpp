#include<stdio.h>
#include<stdlib.h>
#include<iostream>
#include<graphics.h>


using namespace std;

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
  bool isPaused;
  bool inGame;
};

struct personagem personagem1; 

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
  personagem1.isPaused = true;
  personagem1.inGame = false;
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
    if (Img[i]==B and Img[i+1]==G and Img[i+2]==R) 
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
    printf("esquerda");
  }
  if(GetKeyState(VK_RIGHT)&0x80)
  {
  	personagem1.direction='right';
    personagem1.PosX += personagem1.VetorAccX;
    printf("direita.");
  }
  if(GetKeyState(VK_UP)&0x80 && personagem1.can_jump)
  {
  	printf("%i",personagem1.actual_jump_stamina);
  	personagem1.state = 'jumping';
    personagem1.PosY += -10;
    if(personagem1.actual_jump_stamina > 0)
    {
      personagem1.actual_jump_stamina -= 4;
    }
    if(personagem1.actual_jump_stamina == 0)
    {
      personagem1.can_jump = false; 
      printf("no puedes mas dar pulitos.");
    }
	}
  else
  {
    if(personagem1.actual_jump_stamina != personagem1.jump_stamina)
    {
      personagem1.actual_jump_stamina +=4;
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
  personagem1.PosY +=5;
  if(personagem1.PosY >= personagem1.ground)
  {
  	personagem1.PosY = personagem1.ground;
  	personagem1.can_jump = true;
  }
}
void PauseListener()
{

}

int main()  
{ 
  int pg = 1;
  unsigned char *P, *M;
  int TamP;
  initwindow(800, 600);	
  setPlayerConfig();
  TamP = imagesize(0, 0, 31, 31);
  P = (unsigned char *)malloc(TamP);
  M = (unsigned char *)malloc(TamP);
  readimagefile("Sans.bmp",0 , 0 , 31, 31); // carrega a imagem
  getimage(0, 0, 31, 31, P); // captura para o ponteiro P
  getimage(0, 0, 31, 31, M); // captura para a m�scara M (a mesma imagem de P, que depois ser� manipulada na rotina PreparaImg)
  ImageConfig(TamP,P,M);
  backgroundConfig();
  while(personagem1.inGame)
  {
	if (pg == 1) pg = 2; else pg = 1;
    setactivepage(pg);
    cleardevice();    
    putimage(personagem1.PosX, personagem1.PosY, M, AND_PUT);
    putimage(personagem1.PosX, personagem1.PosY, P, OR_PUT);
    Move();
    PauseListener();
    StateCheck(personagem1.direction);
    Gravity();
	  setvisualpage(pg);
    

    delay(10);
  }
  readimagefile("pauseScreen.bmp",0 , 0 , 302, 165); // carrega a imagem
  TamP = imagesize(0, 0, 302, 165);
  P = (unsigned char *)malloc(TamP);
  M = (unsigned char *)malloc(TamP);
  getimage(0, 0, 302, 165, P); // captura para o ponteiro P
  getimage(0, 0, 302, 165, M); // captura para a m�scara M (a mesma imagem de P, que depois ser� manipulada na rotina PreparaImg)
  ImageConfig(TamP,P,M);
  while(personagem1.isPaused)
  {
    if (pg == 1) pg = 2; else pg = 1;
    setactivepage(pg);
    cleardevice(); 
    putimage(0, 0, M, AND_PUT);
    putimage(0, 0, P, OR_PUT);
    setvisualpage(pg);
  }
  
  //while(!kbhit());	
  //closegraph();	
  return 0; 
}

