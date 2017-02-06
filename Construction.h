#pragma once
#include <iostream>
#include <fstream>
#include <time.h>
#include <windows.h>
using namespace std;

//Enumerations for basic reigons and enemies.
enum REGION {
	A_REG,
	B_REG,
	C_REG,
	D_REG
};

enum Etype {
	PVR,
	FITR,
	SHLD_FITR,
};

//Definations of constants
#define CmdWidth	    150
#define CmdHeight	    50
#define CastleWidth	    30
#define CastleLength	20
#define CastleXStrt	    (CmdWidth/2-(CastleWidth/2))
#define CastleYStrt	    (CmdHeight/2-(CastleLength/2))
#define TowerWidth      7
#define TowerLength     3
#define EnemyShape  	219  //ASCII code of enemy char shape 

struct enemy
{
	int ID;			  //Each enemy has a unique ID (sequence number)
	REGION Region;	  //Region of this enemy
	float Distance;	  //Distance to the castle
	float Health;	  //Enemy health
	Etype Type;		  //PVR, FITR, SHLD_FITR
	float OH;          //The original health.
	int TS;           //Enemy time step
	int FSH;          //Time of first shot.
	int  KT;          //killed time.
	float FP;           //fire power 
	int RP;           //Enemy reload period
	float prior;      //Enemy Priority
	float speed;      //handling enemies with different speeds.
};

struct Node
{
	enemy Enemy;
	Node *next;
};

struct queue
{
	Node *front;
	Node *rear;
};

struct queuearr {
	int front;
	int rear;
	enemy*arr[100];
};

struct LinkList
{
	Node *head;
};

struct Tower
{
	int TW;                                   //Tower width
	int TL;                                   //Tower Height
	int Health;                               //Tower Health
	float FP;                                   //Tower firepower
	int N;                                    //No. of enemies tower can hit at any time
	int LKT;
	LinkList* ACT;
	LinkList* HI;
	LinkList* Kill;
};

struct castle
{
	                                    
	int Xstrt;                                //starting x,y coordination
	int Ystrt;
	int W;	                                  //Width
	int L;                                    //Height
	Tower towers[4];	                      //Castle has 4 towers
	float c1;
	float c2;
	float c3;
};

struct output
{
	int FD;
	int KD;
	int FT;
	int KTS;
};



castle*Initiallizations(queue*&InactivE);


/*A function to set the position of cursor on the screen*/
void gotoxy(int x, int y);

/*A function to set the command window lenght and height for the game specification*/
void SetWindow();

/*A function to color the cmd text*/
void color(int thecolor);

/*A function to partition the castle into regions (A,B,C,D)*/
void DrawRegions(const castle & C);

/*A function to draw the castle and the towers*/
void DrawCastle(const castle & C, int SimulationTime);

/*A function to draw a single enemy using its distance from the castle*/
void DrawEnemy(const enemy& E, int Ypos = 0);

/*A function to draw a list of enemies exist in all regions and ensure there is no overflow in the drawing*/
void DrawEnemies(enemy* enemies[], int size);

/*Student use this function to print his/her message*/
void PrintMsg(char*msg);

//*****************************************************************************************************************************************************//
// convert function
void convert(enemy*&en, char reg, int type);
//*****************************************************************************************************************************************************//
//revert function
void revert(enemy*en, char &reg);
//*****************************************************************************************************************************************************//
void DestroyQueue(queuearr*&all_enemies, int size);
//*****************************************************************************************************************************************************//
//File Loading Function
int Read(queue*&inactivE, castle*&ct , float&c1, float&c2, float&c3);
//*****************************************************************************************************************************************************//
//Enqueue function
bool enqueue(queue*&en, enemy*newE);
//*****************************************************************************************************************************************************//
//Dequeue function
bool dequeue(queue*&en, enemy*&E);
//*****************************************************************************************************************************************************//
//IsQueueEmpty function
bool IsQueueEmpty(queue*en);
//*****************************************************************************************************************************************************//
//Queue Initializing function
void InitQueue(queue*&q);
//*****************************************************************************************************************************************************//
//InsertFront function
bool InsertFront(LinkList*&list, enemy*Enem);
//*****************************************************************************************************************************************************//
//IsLinkEmpty function
bool IsLinkEmpty(LinkList*list);
//*****************************************************************************************************************************************************//
//Check if queue is empty.
int size_queue(queue*q);
//*****************************************************************************************************************************************************//
//Initialising function for linked list
void InitLink(LinkList*&list);
//*****************************************************************************************************************************************************//
//Inactiv_activ function, to transfer enemies from inactive list to active list
void inactiv_to_activ(queue*&Inactiv, LinkList*&activ, int Tick);
//*****************************************************************************************************************************************************//
// Transfer function, to transfer all enemies of the same arrival time from inactive list to active list
void transfer(queue*&InactivE, LinkList*&RegA_H, LinkList*&RegB_H, LinkList*&RegC_H, LinkList*&RegD_H,
	LinkList*&RegA_N, LinkList*&RegB_N, LinkList*&RegC_N, LinkList*&RegD_N, int Tick);
//*****************************************************************************************************************************************************//
//Display current Active Enemies function
void DispInf(LinkList*list_H, LinkList*list_N, LinkList*Killed_list, int*KillCount);
//*****************************************************************************************************************************************************//
// Remove Killed Enemies function
void Remove_Killed(LinkList*&list);
//*****************************************************************************************************************************************************//
//Simulator function
void simulateprogram(queue*&InactivE, castle*&ct, float&c1, float&c2, float&c3);

//*****************************************************************************************************************************************************//
//return size of killed enemies
int return_killed_size(LinkList*list);

//return size
int return_size(LinkList*list);
//*****************************************************************************************************************************************************//
void removeKilled(castle*&ct, int Tick);
//*****************************************************************************************************************************************************//
void MoveEnemies(castle*ct, int Tick, int*Paved);
//*****************************************************************************************************************************************************//
void Paving(castle*ct, int*Paved);
//*****************************************************************************************************************************************************//
void EnemiesFire(castle*&ct, int Tick, int*Paved);
//*****************************************************************************************************************************************************//
void PriorTowersFire(castle*ct,  int Tick);
//*****************************************************************************************************************************************************//
void TowersFire_N(castle*&ct, int*rem, int Tick);
//*****************************************************************************************************************************************************//
int*TowersFire_H(castle*&ct, int Tick);
//*****************************************************************************************************************************************************//
void toArray(queuearr*&q, LinkList*list);
//*****************************************************************************************************************************************************//
int preDraw(queuearr*&all_enemies, castle*ct);
//*****************************************************************************************************************************************************//
bool insertNode(enemy* thenew, LinkList*&list);
//*****************************************************************************************************************************************************//
void PriorCalc(castle*&ct, int Tick);
//*****************************************************************************************************************************************************//
void sorting(castle*&ct);
//*****************************************************************************************************************************************************//
void EnemiesRotation(castle*&ct, int i, int*Paved);
//*****************************************************************************************************************************************************//
int KilledSizes(castle*ct, queue*InactivE);
//*****************************************************************************************************************************************************//
void sortingoutputKTS(output*&arr, int size);
//*****************************************************************************************************************************************************//
void sortingoutputFD(output*&arr, int size);
//*****************************************************************************************************************************************************//
void Outputfile(int m, queuearr*q, castle*c, int*paved, int x, int nom_enemies);
//*****************************************************************************************************************************************************//
void DispStepInf(castle*ct, int*Paved);