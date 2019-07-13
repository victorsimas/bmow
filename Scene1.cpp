#include <iostream>
#include <graphics.h>
//BIBLIOTECAS QUE USEI PARA INSERIR SOM//
#include <windows.h>
// PARA O USO EFETIVO, É NECESSÁRIO O LINKER "-lwinmm"
#include <time.h>
using namespace std;

#define BMOSprites 8
#define BMOPoses 3
#define LUZSprites 4

struct BMO{
	int x, y;
};
struct BALL{
	int x, y;
	bool fire;
};
struct ZOMBIE{
	int x, y;
	int vivo;
	int PoseZ;
};
struct LUZ{
	int x, y;
};
struct FASE1{
	int x, x2;
};

//===============================================================STRUCTS E BIBLIOTECAS====================================================//

//=================================================================VARIAVEIS GLOBAIS=======================================================//
//TELA, CONTADORES E TECLA
int ScreenX = 0;
int ScreenY = 0;
	
int l, k, j, i, pg, Spr = 0, Sprtime = 10, LUZSpr = 0;
int BarG;
int MAINtxt, CODEtxt, OPTtxt;

bool LoadScreen = true, MainScreen = false, GameFase1 = false;
	
FASE1 *Fundo1;
	
//BMO	
BMO *BMOCHAR;
int BMOSIZE;
void *imgBMO[BMOSprites][BMOPoses];
int Pose = 0;
int Velocidade = 10;
	
//BALL
BALL *BALLCHAR;
int BALLSIZE;
void *imgBALL[LUZSprites];
int VelocidadeBALL = 20;
	
//LUZ
LUZ *LUZCHAR;
int QtdeLUZ = 16;
	
//ZOMBIE
ZOMBIE *ZOMBIECHAR;
int ZOMBIESIZE;
void *imgZOMBIE[BMOSprites][BMOPoses];
int QtdeZOMBIE = 40;
int VelocidadeZ = 10;
	
//CORES
int BGColor = RGB(20,20,20);
int TextColor = RGB(160,160,160);
int LoadbarColor = RGB (15, 155, 15);
int MainbarColor = RGB (100, 20, 20);

//VALIDADORES
bool New = false, NewConf = false;
int Venceu = 0;
bool Perdeu = false;
bool Fim = false;
bool Stop = false;
bool Sucess = false;

//=========================================================FIM VARIAVEIS GLOBAIS===========================================================//

//===============================================================FUNÇÕES===================================================================//

void Resolucao(int& ScreenX, int& ScreenY);

void FimdoJogo(int Venceu);

void Audio();

void Load();

void Menu();

void FASE_1();

//===============================================================FIM FUNÇÕES================================================================//

///////////////////////////////////////////////////////////////////MAIN/////////////////////////////////////////////////////////////////////////
int main(){
	//SEED-------------
	srand(time(NULL));
	
	ScreenY = ScreenY - 50;
	
	MAINtxt = 40;
	CODEtxt = 80;
	OPTtxt = 60; 
	
	BarG = 0;
	
	Resolucao(ScreenX, ScreenY);
	
	//INICIANDO GRAFICOS
	initwindow(ScreenX,ScreenY, "BMO_W", - 5, 0,false);
	
	BMOSIZE = imagesize(0,0, 340*2 -1, 105*2 - 1);
	ZOMBIESIZE = imagesize(0,0, 340*2 -1, 105*2 - 1);
	BALLSIZE = imagesize(0,0, 200 -1, 100 - 1);
	
	Fundo1 = (FASE1 *)malloc(sizeof(FASE1) * 1);
	
	BMOCHAR = (BMO *)malloc(sizeof(BMO) * 1);
	ZOMBIECHAR = (ZOMBIE *)malloc(sizeof(ZOMBIE) * QtdeZOMBIE);
	LUZCHAR = (LUZ *)malloc(sizeof(LUZ) * QtdeLUZ);
	BALLCHAR = (BALL *) malloc (sizeof(BALL) * 1);
	
	//DEFININDO POSIÕES BMO, LUZ, CENÁRIO E ZOMBIE
	BMOCHAR[0].x = 100;
	BMOCHAR[0].y = ScreenY/1.6;
	
	BALLCHAR[0].x = BMOCHAR[0].x + 150;
	BALLCHAR[0].y = BMOCHAR[0].y + 35;
	for(i=0; i < QtdeLUZ - 1; i++){
    	LUZCHAR[i].x = ScreenX/15 + 400*i;
    	LUZCHAR[i].y = ScreenY/6;
  	}
  	
  	for(i=0; i < QtdeZOMBIE; i++){
    	ZOMBIECHAR[i].x = rand() % ScreenX*4 + 1000*(i + 5)/2;
    	ZOMBIECHAR[i].y = ScreenY/1.6;
    	ZOMBIECHAR[i].vivo = 1;
		ZOMBIECHAR[i].PoseZ = 2;           
  	}
  	
  	//DEFININDO A POSIÇÃO DA TELA
  	Fundo1[0].x = 0;
  	Fundo1[0].x2 = ScreenX *3;

	Audio();
	
	Load();

	Menu();
	
	mciSendString("close System wait", NULL, 0, 0);
	mciSendString("close Main", NULL, 0, 0);
	mciSendString("play Scene repeat", NULL, 0, 0);
	
	cleardevice();
	
	FASE_1();

	
}	

/////////////////////////////////////////////////////////////////////////FIM MAIN////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////ORQUETRAÇÃO DAS FUNÇÕES--------------------------------------------------------

void FASE_1(){
	while (GameFase1 == true){
		if (pg == 1) pg = 2;
		else pg = 1;
		
		setactivepage(pg);
		cleardevice();
		
		if (Fundo1[0].x2 <= ScreenX){
			Fundo1[0].x = 0;
			Fundo1[0].x2 = ScreenX *3;
			
			for(i=0; i < QtdeLUZ; i++){
    			LUZCHAR[i].x = ScreenX/15 + 400*i;
    			LUZCHAR[i].y = ScreenY/6;
  			}
		}
		
		//FUNDO
		setfillstyle(2, RGB(40,40,40));
		bar(Fundo1[0].x,0, Fundo1[0].x2, ScreenY);
		
		setfillstyle(1, RGB(30,60,30));
		if (Perdeu == true){
	    		setfillstyle(1, RGB(60, 30, 30));
			}
		bar(Fundo1[0].x, ScreenY/2, Fundo1[0].x2, ScreenY/1.8);
		
		setfillstyle(1, RGB(40,70,40));
		if (Perdeu == true){
	    		setfillstyle(1, RGB(70, 40, 40));
			}
		bar(Fundo1[0].x, ScreenY/1.9, Fundo1[0].x2, ScreenY/1.85);
		
		// CHAO
		setfillstyle(1, RGB(15,15,15));
		bar(Fundo1[0].x,ScreenY/1.2, Fundo1[0].x2, ScreenY);
		
		setfillstyle(1, RGB(10,10,10));
		bar(Fundo1[0].x,ScreenY/1.1, Fundo1[0].x2, ScreenY);
		
		//BMO CHAR
		putimage(BMOCHAR[0].x, BMOCHAR[0].y, imgBMO[k + 4][Pose], AND_PUT);
	    putimage(BMOCHAR[0].x, BMOCHAR[0].y, imgBMO[k][Pose], OR_PUT);
	   
	    //ZOMBIE CHAR
	    for (i=0; i < QtdeZOMBIE; i++){
			if (ZOMBIECHAR[i].vivo == 1 && ZOMBIECHAR[i].x <= ScreenX){
	    		putimage(ZOMBIECHAR[i].x, ZOMBIECHAR[i].y, imgZOMBIE[k + 4][ZOMBIECHAR[i].PoseZ], AND_PUT);
	    		putimage(ZOMBIECHAR[i].x, ZOMBIECHAR[i].y, imgZOMBIE[k][ZOMBIECHAR[i].PoseZ], OR_PUT);
			}
			
	    	if (ZOMBIECHAR[i].x < BMOCHAR[0].x + 10 && ZOMBIECHAR[i].vivo == 1){
				ZOMBIECHAR[i].PoseZ = 1;
				sndPlaySound(".\\Audio\\Bite.wav", SND_ASYNC);
				MainScreen = true; 
				Perdeu = true;
			}	
		}
	    
	    //LUZ CHAR
	    for (l=0; l < QtdeLUZ - 1;l++){
	    	setcolor(RGB(0,40,0));
	    	setfillstyle(1, RGB(0,40,0));
	    	if (Perdeu == true){
	    		setcolor(RGB(40,0,0));
	    		setfillstyle(1, RGB(40,0,0));
			}
			if (Sucess == true){
				setcolor(RGB(0,0, 40));
	    		setfillstyle(1, RGB(0,0, 40));
			}
	    	fillellipse(LUZCHAR[l].x, LUZCHAR[l].y, 100 - LUZSpr, 100 - LUZSpr);
	    	setcolor(RGB(0,70,0));
	    	setfillstyle(1, RGB(0,70,0));
	    	if (Perdeu == true){
	    		setcolor(RGB(70,0,0));
	    		setfillstyle(1, RGB(70,0,0));
			}
			if (Sucess == true){
				setcolor(RGB(0,0, 70));
	    		setfillstyle(1, RGB(0,0, 70));
			}
	    	fillellipse(LUZCHAR[l].x, LUZCHAR[l].y, 70 - LUZSpr, 70 - LUZSpr);
	    	setcolor(RGB(0,100,0));
	    	setfillstyle(1, RGB(0,100,0));
	    	if (Perdeu == true){
	    		setcolor(RGB(100,0,0));
	    		setfillstyle(1, RGB(100,0,0));
			}
			if (Sucess == true){
				setcolor(RGB(0,0, 100));
	    		setfillstyle(1, RGB(0,0, 100));
			}
	    	fillellipse(LUZCHAR[l].x, LUZCHAR[l].y, 50 - LUZSpr, 50 - LUZSpr);
	    	setcolor(RGB(0,130,0));
	    	setfillstyle(1, RGB(0,130,0));
	    	if (Perdeu == true){
	    		setcolor(RGB(130,0,0));
	    		setfillstyle(1, RGB(130,0,0));
			}
			if (Sucess == true){
				setcolor(RGB(0,0, 130));
	    		setfillstyle(1, RGB(0,0, 130));
			}
	    	fillellipse(LUZCHAR[l].x, LUZCHAR[l].y, 40 , 40 );
	    	setcolor(RGB(0,160,0));
	    	setfillstyle(1, RGB(0,160,0));
	    	if (Perdeu == true){
	    		setcolor(RGB(160,0,0));
	    		setfillstyle(1, RGB(160,0,0));
			}
			if (Sucess == true){
				setcolor(RGB(0,0, 160));
	    		setfillstyle(1, RGB(0,0, 160));
			}
	    	fillellipse(LUZCHAR[l].x, LUZCHAR[l].y, 30, 30);
	    	setcolor(RGB(0,190,0));
	    	setfillstyle(1, RGB(0,190,0));
	    	if (Perdeu == true){
	    		setcolor(RGB(190,0,0));
	    		setfillstyle(1, RGB(190,0,0));
			}
			if (Sucess == true){
				setcolor(RGB(0,0, 190));
	    		setfillstyle(1, RGB(0,0, 190));
			}
	    	fillellipse(LUZCHAR[l].x, LUZCHAR[l].y, 20, 20);
		}
	 	
		//BALL CHAR
		if (BALLCHAR[0].fire == true){
			setcolor(RGB(0,100,0));
	    	setfillstyle(1, RGB(0, 80,0));
	    	fillellipse(BALLCHAR[0].x, BALLCHAR[0].y, 20 - LUZSpr +10, 20 - LUZSpr + 10);
	    	setcolor(RGB(0,130,0));
	    	setfillstyle(1, RGB(0,130,0));
	    	fillellipse(BALLCHAR[0].x, BALLCHAR[0].y, 15 - LUZSpr +10, 15 - LUZSpr +10);
	    	setcolor(RGB(0,160,0));
	    	setfillstyle(1, RGB(0,180,0));
	    	fillellipse(BALLCHAR[0].x, BALLCHAR[0].y, 10, 10);
	    	
	    	BALLCHAR[0].x = BALLCHAR[0].x + VelocidadeBALL;
	    	
	    	for (i=0; i < QtdeZOMBIE; i++){
	    		if (BALLCHAR[0].x > ScreenX){
					BALLCHAR[0].fire = false;
	    			BALLCHAR[0].x = BMOCHAR[0].x + 150;
	    			
				}
				if (BALLCHAR[0].x > ZOMBIECHAR[i].x + 100 && ZOMBIECHAR[i].vivo == 1){
					sndPlaySound(".\\Audio\\Explosion2.wav", SND_ASYNC);
					BALLCHAR[0].fire = false;
	    			BALLCHAR[0].x = BMOCHAR[0].x + 150;
					ZOMBIECHAR[i].vivo = 0;
				}
			}
	    	
		}     
		
		if (Spr > Sprtime && Stop == false){
			Spr = 0;
		    k++;
		    j++;
		    LUZSpr++;
		    if (LUZSpr > 20){
	    		for (i=0; i < 20; i++){
	    			LUZSpr--;
				}
			}
		    if (k > 3){
			   k = 0;
		    }
		    if (j = 1){
			    j = 0;
		    }
	    }
	    Spr++;
	    
	    FimdoJogo(Venceu);
					
		if (Stop == false && GetKeyState(VK_SPACE)&0x80){
			Pose = 1;
			if(Pose == 1 && k == 2){
				BALLCHAR[0].fire = true;
			}
		} 
		else if (Stop == false && GetKeyState(VK_RIGHT)&0x80){
			Pose = 2; 
			VelocidadeZ = 20;
			
			for (i=0; i < QtdeLUZ; i++){
				LUZCHAR[i].x = LUZCHAR[i].x - Velocidade;
			}
			Fundo1[0].x = Fundo1[0].x - Velocidade;
			Fundo1[0].x2 = Fundo1[0].x2 - Velocidade;
		} 	
		else Pose = 0; VelocidadeZ = 10; 
		
		if (Stop == false){
	    	for (i=0; i < QtdeZOMBIE; i++){
	    		ZOMBIECHAR[i].x = ZOMBIECHAR[i].x - VelocidadeZ;
			}
		}
		
		setvisualpage(pg);		
		delay(10);
	}
}

void Menu(){
	while (MainScreen == true){
		
		if (pg == 1) pg = 2;
		else pg = 1;
		
		setactivepage(pg);
		cleardevice();
			
		setfillstyle(6, LoadbarColor);
		bar(0, ScreenY/8, ScreenX, ScreenY/4);
			
		setfillstyle(8, MainbarColor);
		bar(ScreenX/1.4, 0, ScreenX, ScreenY);
		bar(0, 0, ScreenX/4, ScreenY);
			
		setfillstyle(3, RGB(10, 80, 10));
		bar(ScreenX/1.4 + ScreenY/10, 0 + ScreenY/10, ScreenX - 60, ScreenY - 60);
		bar(0 + 60, 0 + 60, ScreenX/4 - 60, ScreenY - 60);
		
		setcolor(RGB(200,200,200));
		settextstyle(2, 0, MAINtxt);
		
		outtextxy(ScreenX/2 - ScreenX/5, ScreenY/8, "BMO_W 1.0");
		settextstyle(2, 0, CODEtxt);
		outtextxy(ScreenX/2 - ScreenX/2.2, ScreenY/8, "Missão:");
		outtextxy(ScreenX/2 - ScreenX/2.2, ScreenY/8 + 30, "Destruir todos os");
		outtextxy(ScreenX/2 - ScreenX/2.2, ScreenY/8 + 50, "processos Zumbis");
		outtextxy(ScreenX/2 - ScreenX/2.2, ScreenY/8 + 80, "Para Andar (->)");
		outtextxy(ScreenX/2 - ScreenX/2.2, ScreenY/8 + 100, "Atacar (Espaço)");
		
		outtextxy(ScreenX/2 - ScreenX/2.2, ScreenY/8 + 200, "Relatório:");
		outtextxy(ScreenX/2 - ScreenX/2.2, ScreenY/8 + 230, "Dispositivo não");
		outtextxy(ScreenX/2 - ScreenX/2.2, ScreenY/8 + 250, "identificado");
		outtextxy(ScreenX/2 - ScreenX/2.2, ScreenY/8 + 270, "adentrou, iniciando");
		outtextxy(ScreenX/2 - ScreenX/2.2, ScreenY/8 + 290, "operações vazias");
		
		outtextxy(ScreenX/2 - ScreenX/5, ScreenY - 100, "Feito e Criado por:");
		outtextxy(ScreenX/2 - ScreenX/5, ScreenY - 80, "Victor Santos Simas Almeida");
			
		setfillstyle(2, RGB(10, 80, 10));			
		bar(ScreenX/2 - ScreenX/7 - 40, ScreenY/3, ScreenX/1.6, ScreenY/3 + ScreenY/10);
		bar(ScreenX/2 - ScreenX/7 - 40, ScreenY/3 + ScreenY/5, ScreenX/1.6, ScreenY/3 + ScreenY/3.334);
			
		settextstyle(2, 0, MAINtxt);
		setcolor(RGB(200,200,200));
		if (New == true) setcolor(LoadbarColor);
		outtextxy(ScreenX/2 - ScreenX/7, ScreenY/3, " NOVO ");
		setcolor(RGB(200,200,200));
		if (New == false) setcolor(LoadbarColor);
		outtextxy(ScreenX/2 - ScreenX/7, ScreenY/3 + ScreenY/5, " SAIR ");
		
		if (GetKeyState(VK_UP)&0x80){
  			New = true;
		}
  		
		if (GetKeyState(VK_DOWN)&0x80){
			New = false;
		}  
		
		if (GetKeyState(VK_RETURN)&0x80){
			NewConf = true;
			if (NewConf == true && New == true){
				GameFase1 = true; 
				MainScreen = false; 
			} 
			
			if (NewConf == true && New == false) {
				MainScreen = false; 
			}
			
		}
		setvisualpage(pg);
		delay(10);
		
	}
}

void Load(){
//==============================================================INICIO TELA DE LOADING===================================================//	
		setactivepage(1);
		setfillstyle(1, BGColor);
		bar(0,0, ScreenX, ScreenY);
		setbkcolor(BGColor);
		settextstyle(2, 0, MAINtxt);
		outtextxy(ScreenX/2 - ScreenX/3, ScreenY/8, "BMO_W");
		rectangle(ScreenX/2 - ScreenX/3, ScreenY/4, ScreenX/2, ScreenY/3);
		setvisualpage(1);
		
		for(k=0; k < BMOPoses; k++){
			for (i=0; i < BMOSprites; i++){
				imgBMO[i][k] = malloc(BMOSIZE);	
			} 
		}
		
		for(k=0; k < BMOPoses; k++){
			for (i=0; i < BMOSprites; i++){
				imgZOMBIE[i][k] = malloc(ZOMBIESIZE);	
			} 
		}
		
		setactivepage(2);
		//BMO IMAGENS SENDO CARREGADAS
		readimagefile("BMO_W.bmp", 0, 0, 340*2 - 1, 105*2 -1);
		getimage(0,0, 85*2 - 1, 105*2 - 1, imgBMO[0][0]);
		getimage(85*2,0, 170*2 - 1, 105*2 - 1, imgBMO[1][0]);
		getimage(170*2, 0, 255*2 - 1, 105*2 - 1, imgBMO[2][0]);
		getimage(255*2, 0, 340*2 - 1, 105*2 - 1, imgBMO[3][0]);
			
		readimagefile("BMO_WBG.bmp", 0, 0, 340*2 - 1, 105*2 -1);
		getimage(0,0, 85*2 - 1, 105*2 - 1, imgBMO[4][0]);
		getimage(85*2,0, 170*2 - 1, 105*2 - 1, imgBMO[5][0]);
		getimage(170*2, 0, 255*2 - 1, 105*2 - 1, imgBMO[6][0]);
		getimage(255*2, 0, 340*2 - 1, 105*2 - 1, imgBMO[7][0]);
		
		readimagefile("BMO_W_AT.bmp", 0, 0, 340*2 - 1, 105*2 -1);
		getimage(0,0, 85*2 - 1, 105*2 - 1, imgBMO[0][1]);
		getimage(85*2,0, 170*2 - 1, 105*2 - 1, imgBMO[1][1]);
		getimage(170*2, 0, 255*2 - 1, 105*2 - 1, imgBMO[2][1]);
		getimage(255*2, 0, 340*2 - 1, 105*2 - 1, imgBMO[3][1]);
			
		readimagefile("BMO_W_ATBG.bmp", 0, 0, 340*2 - 1, 105*2 -1);
		getimage(0,0, 85*2 - 1, 105*2 - 1, imgBMO[4][1]);
		getimage(85*2,0, 170*2 - 1, 105*2 - 1, imgBMO[5][1]);
		getimage(170*2, 0, 255*2 - 1, 105*2 - 1, imgBMO[6][1]);
		getimage(255*2, 0, 340*2 - 1, 105*2 - 1, imgBMO[7][1]);	
		
		readimagefile("BMO_W_AN.bmp", 0, 0, 340*2 - 1, 105*2 -1);
		getimage(0,0, 85*2 - 1, 105*2 - 1, imgBMO[0][2]);
		getimage(85*2,0, 170*2 - 1, 105*2 - 1, imgBMO[1][2]);
		getimage(170*2, 0, 255*2 - 1, 105*2 - 1, imgBMO[2][2]);
		getimage(255*2, 0, 340*2 - 1, 105*2 - 1, imgBMO[3][2]);
			
		readimagefile("BMO_W_ANBG.bmp", 0, 0, 340*2 - 1, 105*2 -1);
		getimage(0,0, 85*2 - 1, 105*2 - 1, imgBMO[4][2]);
		getimage(85*2,0, 170*2 - 1, 105*2 - 1, imgBMO[5][2]);
		getimage(170*2, 0, 255*2 - 1, 105*2 - 1, imgBMO[6][2]);
		getimage(255*2, 0, 340*2 - 1, 105*2 - 1, imgBMO[7][2]);
		
		//ZOMBIE IMAGENS SENDO CARREGADAS
		readimagefile("ZOMBIE_STOP.bmp", 0, 0, 340*2 - 1, 105*2 -1);
		getimage(0,0, 85*2 - 1, 105*2 - 1, imgZOMBIE[0][0]);
		getimage(85*2,0, 170*2 - 1, 105*2 - 1, imgZOMBIE[1][0]);
		getimage(170*2, 0, 255*2 - 1, 105*2 - 1, imgZOMBIE[2][0]);
		getimage(255*2, 0, 340*2 - 1, 105*2 - 1, imgZOMBIE[3][0]);
			
		readimagefile("ZOMBIE_STOPBG.bmp", 0, 0, 340*2 - 1, 105*2 -1);
		getimage(0,0, 85*2 - 1, 105*2 - 1, imgZOMBIE[4][0]);
		getimage(85*2,0, 170*2 - 1, 105*2 - 1, imgZOMBIE[5][0]);
		getimage(170*2, 0, 255*2 - 1, 105*2 - 1, imgZOMBIE[6][0]);
		getimage(255*2, 0, 340*2 - 1, 105*2 - 1, imgZOMBIE[7][0]);
		
		readimagefile("ZOMBIE_ATTACK.bmp", 0, 0, 340*2 - 1, 105*2 -1);
		getimage(0,0, 85*2 - 1, 105*2 - 1, imgZOMBIE[0][1]);
		getimage(85*2,0, 170*2 - 1, 105*2 - 1, imgZOMBIE[1][1]);
		getimage(170*2, 0, 255*2 - 1, 105*2 - 1, imgZOMBIE[2][1]);
		getimage(255*2, 0, 340*2 - 1, 105*2 - 1, imgZOMBIE[3][1]);
			
		readimagefile("ZOMBIE_ATTACKBG.bmp", 0, 0, 340*2 - 1, 105*2 -1);
		getimage(0,0, 85*2 - 1, 105*2 - 1, imgZOMBIE[4][1]);
		getimage(85*2,0, 170*2 - 1, 105*2 - 1, imgZOMBIE[5][1]);
		getimage(170*2, 0, 255*2 - 1, 105*2 - 1, imgZOMBIE[6][1]);
		getimage(255*2, 0, 340*2 - 1, 105*2 - 1, imgZOMBIE[7][1]);	
		
		readimagefile("ZOMBIE_WALK.bmp", 0, 0, 340*2 - 1, 105*2 -1);
		getimage(0,0, 85*2 - 1, 105*2 - 1, imgZOMBIE[0][2]);
		getimage(85*2,0, 170*2 - 1, 105*2 - 1, imgZOMBIE[1][2]);
		getimage(170*2, 0, 255*2 - 1, 105*2 - 1, imgZOMBIE[2][2]);
		getimage(255*2, 0, 340*2 - 1, 105*2 - 1, imgZOMBIE[3][2]);
			
		readimagefile("ZOMBIE_WALKBG.bmp", 0, 0, 340*2 - 1, 105*2 -1);
		getimage(0,0, 85*2 - 1, 105*2 - 1, imgZOMBIE[4][2]);
		getimage(85*2,0, 170*2 - 1, 105*2 - 1, imgZOMBIE[5][2]);
		getimage(170*2, 0, 255*2 - 1, 105*2 - 1, imgZOMBIE[6][2]);
		getimage(255*2, 0, 340*2 - 1, 105*2 - 1, imgZOMBIE[7][2]);

		//BARRA DO LOADING
		for (i=0; i < ScreenX;i++){
			setactivepage(1);
			if (BarG < ScreenX/2 - 10 ){
				BarG = ScreenX/2 - ScreenX/3 + 10 + i/2;
			}
			else{
				BarG = ScreenX/2 - 10;
			}	
			setfillstyle(3, LoadbarColor);
			bar(ScreenX/2 - ScreenX/3 + 10, ScreenY/4 + 10, BarG, ScreenY/3 - 10);
			setvisualpage(1); 
		} 
		
		mciSendString("play System wait", NULL, 0, 0);

        
        setcolor(RGB(220,220,220));
		setfillstyle(1, BGColor);
		bar(0,0, ScreenX, ScreenY);
		settextstyle(2, 0, CODEtxt);
		outtextxy(ScreenX/10 - 20, ScreenY/10, "Inicializando S.O. ...");
		mciSendString("play Pointers wait", NULL, 0, 0);
		outtextxy(ScreenX/10 - 20, ScreenY/10 + 40, "Coletando DATA da Memória ...");
		mciSendString("play Pointers2 wait", NULL, 0, 0);
		outtextxy(ScreenX/10 - 20, ScreenY/10 + 80, "Iniciando DQO ...");
		mciSendString("play Pointers3 wait", NULL, 0, 0);
		outtextxy(ScreenX/10 - 20, ScreenY/10 + 120, "Checando Dispositivos ...");
		mciSendString("play Confirm wait", NULL, 0, 0);
		
		LoadScreen = false; 
		MainScreen = true;
		
		mciSendString("play Main repeat", NULL, 0, 0);
		BALLCHAR[0].fire = false;
//=============================================================TELA DE LOADING FIM==========================================================//
}

void Audio(){
//================================================================TRILHA SONORA===========================================================//

    
	mciSendString("open .\\Audio\\MainTitle.mp3 type MPEGVideo alias Main", NULL, 0, 0); 
	mciSendString("open .\\Audio\\Confirm.mp3 type MPEGVideo alias Confirm", NULL, 0, 0); 
	mciSendString("open .\\Audio\\Scene1.mp3 type MPEGVideo alias Scene", NULL, 0, 0); 
	mciSendString("open .\\Audio\\System.mp3 type MPEGVideo alias System", NULL, 0, 0); 
	mciSendString("open .\\Audio\\System.mp3 type MPEGVideo alias System2", NULL, 0, 0); 
	mciSendString("open .\\Audio\\Pointers.mp3 type MPEGVideo alias Pointers", NULL, 0, 0); 
	mciSendString("open .\\Audio\\Pointers.mp3 type MPEGVideo alias Pointers2", NULL, 0, 0); 
	mciSendString("open .\\Audio\\Pointers.mp3 type MPEGVideo alias Pointers3", NULL, 0, 0); 
    

//=================================================================FIM DA TRILHA SONORA===================================================//
}

void FimdoJogo(int Venceu){
	
		for (i=0; i < QtdeZOMBIE; i++){
			if (ZOMBIECHAR[i].vivo == 0) Venceu++;
		}
		
		if (Venceu == QtdeZOMBIE && Perdeu != true){
				setbkcolor (RGB(40,180,40));
				setcolor (RGB(200,200,200));
				settextstyle(2, 0, MAINtxt);
				outtextxy(ScreenX/2 - ScreenX/3, ScreenY/10, "YOU WIN!");
				
				mciSendString("close Scene", NULL, 0, 0);
				mciSendString("play System2", NULL, 0, 0);
				
				Sucess = true;
				Stop = true;
				if (GetKeyState(VK_ESCAPE)&0X80){
					GameFase1 = false;
				}	
					
		}
		else if(Perdeu == true){
				setbkcolor (RGB(60,10,10));
				setcolor (RGB(200,200,200));
				settextstyle(2, 0, MAINtxt);
				outtextxy(ScreenX/2 - ScreenX/3, ScreenY/10, "GAME OVER");
				
				Stop = true;
				mciSendString("close Scene", NULL, 0, 0);
				mciSendString("play System2", NULL, 0, 0);
				
				if (GetKeyState(VK_ESCAPE)&0X80){
					GameFase1 = false;
				}
				
		}
}	

void Resolucao(int& ScreenX, int& ScreenY){
   RECT desktop;

   const HWND hDesktop = GetDesktopWindow();

   GetWindowRect(hDesktop, &desktop);

   ScreenX = desktop.right;
   ScreenY = desktop.bottom;
}		
