#include "Construction.h"

void gotoxy(int x, int y)
{
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

	_COORD pos;
	pos.X = x;
	pos.Y = y;

	SetConsoleCursorPosition(hConsole, pos);
}
///////////////////////////////////////////////////////////////////////////////////////

/*A function to set the command window lenght and height for the game specification*/
void SetWindow()
{
	system("mode 150,50");   //Set mode to ensure window does not exceed buffer size
	SMALL_RECT WinRect = { 0, 0, CmdWidth, CmdHeight };   //New dimensions for window in 8x12 pixel chars
	SMALL_RECT* WinSize = &WinRect;
	SetConsoleWindowInfo(GetStdHandle(STD_OUTPUT_HANDLE), true, WinSize);   //Set new size for window
}
///////////////////////////////////////////////////////////////////////////////////////

/*A function to color the cmd text*/
void color(int thecolor)
{
	HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hStdout,
		thecolor | FOREGROUND_INTENSITY);
}
///////////////////////////////////////////////////////////////////////////////////////

/*A function to partition the castle into regions (A,B,C,D)*/
void DrawRegions(const castle & C)
{
	color(FOREGROUND_RED | FOREGROUND_BLUE);
	gotoxy(0, C.Ystrt + C.L / 2);
	for (int i = 0; i<C.W + 2 * C.Xstrt; i++)
		cout << "-";
	for (int j = 0; j<C.L + 2 * C.Ystrt; j++)
	{
		gotoxy(C.Xstrt + C.W / 2, j);
		cout << "|";
	}
	color(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
}
///////////////////////////////////////////////////////////////////////////////////////

/*A function to draw the castle and the towers*/
void DrawCastle(const castle & C, int SimulationTime)
{
	system("CLS");

	//Draw the castle
	cout << endl;
	gotoxy(C.Xstrt, C.Ystrt);
	for (int i = 0; i<C.W; i++)
		cout << "=";
	for (int j = 1; j<C.L; j++)
	{
		gotoxy(C.Xstrt, C.Ystrt + j);
		cout << "|";
		gotoxy(C.Xstrt + C.W - 1, C.Ystrt + j);
		cout << "|";
	}
	gotoxy(C.Xstrt, C.Ystrt + C.L - 1);
	for (int i = 0; i<C.W; i++)
		cout << "=";

	//Draw towers
	int T1X, T2X, T3X, T4X, T1Y, T2Y, T3Y, T4Y;
	for (int i = 0; i<4; i++)
	{

		if (i == 0)
		{
			T1X = C.Xstrt + 1;
			T1Y = C.Ystrt + TowerLength;
			gotoxy(T1X, T1Y);
			for (int i = 0; i<TowerWidth; i++)
				cout << "=";
			gotoxy(T1X + 1, T1Y - 1);
			cout << "T1" << "(" << C.towers[0].Health << ")";

			gotoxy(C.Xstrt + C.W / 4, C.Ystrt + C.L / 4);
			cout << "(A)";
		}
		else if (i == 1)
		{
			T2X = C.Xstrt + C.W - 2 - TowerWidth;
			T2Y = T1Y;
			gotoxy(T2X, T2Y);
			for (int i = 0; i<TowerWidth + 1; i++)
				cout << "=";
			gotoxy(T2X + 1, T2Y - 1);
			cout << "T2" << "(" << C.towers[1].Health << ")";
			gotoxy(C.Xstrt + (3 * C.W / 4), C.Ystrt + C.L / 4);
			cout << "(B)";
		}
		else if (i == 2)
		{
			T3X = T2X;
			T3Y = C.Ystrt + C.L - TowerLength - 1;;
			gotoxy(T3X, T3Y);
			for (int i = 0; i<TowerWidth + 1; i++)
				cout << "=";
			gotoxy(T3X + 1, T3Y + 1);
			cout << "T3" << "(" << C.towers[2].Health << ")";
			gotoxy(C.Xstrt + (3 * C.W / 4), C.Ystrt + (3 * C.L / 4) - 1);
			cout << "(C)";

		}
		else
		{
			T4X = T1X;
			T4Y = C.Ystrt + C.L - TowerLength - 1;
			gotoxy(T4X, T4Y);
			for (int i = 0; i<TowerWidth; i++)
				cout << "=";
			gotoxy(T4X + 1, T4Y + 1);
			cout << "T4" << "(" << C.towers[3].Health << ")";
			gotoxy(C.Xstrt + (C.W / 4), C.Ystrt + (3 * C.L / 4) - 1);
			cout << "(D)";

		}
	}


	DrawRegions(C);

	//****************************
	gotoxy(CmdWidth / 2 - 10, CmdHeight - 1);
	cout << "Simulation Time : " << SimulationTime << endl;
}

///////////////////////////////////////////////////////////////////////////////////////

/*A function to draw a single enemy using its distance from the castle*/
void DrawEnemy(const enemy& E, int Ypos)
{
	int x, y;
	//First calcuale x,y position of the enemy on the output screen
	//It depends on the region and the enemy distance
	REGION EnemyRegion = E.Region;
	switch (EnemyRegion)
	{
	case A_REG:
		x = CastleXStrt - E.Distance + 1;
		y = (CmdHeight / 2) - (CastleLength / 4) - Ypos;
		break;
	case B_REG:
		x = CastleXStrt + CastleWidth + E.Distance - 1;
		y = (CmdHeight / 2) - (CastleLength / 4) - Ypos;
		break;
	case C_REG:
		x = CastleXStrt + CastleWidth + E.Distance - 1;
		y = (CmdHeight / 2) + (CastleLength / 4) + Ypos;
		break;
	default:
		x = CastleXStrt - E.Distance + 1;
		y = (CmdHeight / 2) + (CastleLength / 4) + Ypos;

	}

	gotoxy(x, y);

	//set enemy color according to it type
	Etype EnemyType = E.Type;
	switch (EnemyType)
	{
	case PVR:
		color(FOREGROUND_GREEN);
		break;
	case FITR:
		color(FOREGROUND_RED | FOREGROUND_GREEN);//Yellow
		break;
	default:
		color(FOREGROUND_RED);
	}


	cout << (char)EnemyShape; //Draw the enemy

	color(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
}
///////////////////////////////////////////////////////////////////////////////////////

/*A function to draw a list of enemies and ensure there is no overflow in the drawing*/
//To use this function, you must prepare its input parameters as specified
//Input Parameters:
// enemies [ ] : array of enemy pointers (ALL enemies from all regions in one array)
// size : the size of the array (total no. of enemies)
void DrawEnemies(enemy* enemies[], int size)
{
	//draw enemies at each region 
	for (int region = A_REG; region <= D_REG; region++)
	{
		int CountEnemies = 0;	//count Enemies at the same distance to draw them vertically if they are <= 15 enemy else print number of enemy in the region
		bool draw = true;
		for (int distance = ((CmdWidth / 2) - (CastleWidth / 2)); distance>1; distance--)
		{
			CountEnemies = 0;

			for (int i = 0; i<size; i++)
			{
				if (enemies[i]->Distance == distance && enemies[i]->Region == region)
				{
					CountEnemies++;
				}
			}
			if (CountEnemies>15)
			{
				draw = false;
				break;
			}

		}
		if (draw)
		{
			for (int distance = ((CmdWidth / 2) - (CastleWidth / 2)); distance>1; distance--)
			{
				CountEnemies = 0;

				for (int i = 0; i<size; i++)
				{
					if (enemies[i]->Distance == distance && enemies[i]->Region == region)
					{
						DrawEnemy(*(enemies[i]), CountEnemies);
						CountEnemies++;
					}
				}

			}


		}
		else // print message maximum reached in this region
		{
			int x; int y;
			if (region == A_REG)
			{
				x = CastleXStrt - 30;
				y = (CmdHeight / 2) - (CastleLength / 4);
			}
			else if (region == B_REG)
			{
				x = CastleXStrt + CastleWidth + 30;
				y = (CmdHeight / 2) - (CastleLength / 4);


			}
			else if (region == C_REG)
			{
				x = CastleXStrt + CastleWidth + 30;
				y = (CmdHeight / 2) + (CastleLength / 4);

			}
			else
			{
				x = CastleXStrt - 30;
				y = (CmdHeight / 2) + (CastleLength / 4);

			}
			gotoxy(x, y);
			cout << "Maximum limit";
		}

	}
	gotoxy(0, CmdHeight - 1);
}
///////////////////////////////////////////////////////////////////////////////////////

/*Use this function to print a message, statistics to the user*/
void PrintMsg(char*msg)
{
	cout << msg;
}
//*****************************************************************************************************************************************************//
void convert(enemy*&en, char reg, int type)
{
	if (reg == 'A') en->Region = A_REG;
	else if (reg == 'B') en->Region = B_REG;
	else if (reg == 'C') en->Region = C_REG;
	else en->Region = D_REG;

	if (type == 0) en->Type = PVR;
	else if (type == 1) en->Type = FITR;
	else en->Type = SHLD_FITR;
}
//*****************************************************************************************************************************************************//
void revert(enemy*en, char &reg)
{
	if (en->Region == A_REG) reg = 'A';
	else if (en->Region == B_REG) reg = 'B';
	else if (en->Region == C_REG) reg = 'C';
	else reg = 'D';
}
//*****************************************************************************************************************************************************//
void InitLink(LinkList*&list)
{
	list->head = NULL;
}
//*****************************************************************************************************************************************************//
queuearr* initQueue() {
	queuearr* q = new queuearr;
	q->front = -1;
	q->rear = -1;
	return q;
}
bool isEmptyQ(queuearr* q) {
	return (q->front == -1 && q->rear == -1);
}
bool isFullQ(queuearr* q, int size) {
	return ((q->rear + 1) % size == q->front);
}
bool enqueueA(queuearr* &q, int size, enemy*E)
{
	if (isFullQ(q, size))
		return false;
	else if (isEmptyQ(q))
		q->front = q->rear = 0;
	else
		q->rear = (q->rear + 1) % size;

	q->arr[q->rear] = E;
	return true;
}
bool dequeueA(queuearr* &q, int size, enemy*&E)
{
	if (isEmptyQ(q))
		return false;
	else if (q->front == q->rear) {
		E = q->arr[q->front];
		q->rear = q->front = -1;
	}
	else {
		E = q->arr[q->front];
		q->front = (q->front + 1) % size;
	}
	return true;
}
void DestroyQueue(queuearr*&all_enemies, int size)
{
	enemy*e;
	while (!isEmptyQ(all_enemies))
	{
		dequeueA(all_enemies, size, e);
	}
}
//*****************************************************************************************************************************************************//
bool enqueue(queue*&en, enemy*newE)
{
	Node *n_Node = new Node;
	n_Node->Enemy = *newE;
	if (en->front == NULL)
	{
		en->front = en->rear = n_Node;
		n_Node->next = NULL;
		return true;
	}
	n_Node->next = NULL;
	en->rear->next = n_Node;
	en->rear = n_Node;
	return true;
}
//*****************************************************************************************************************************************************//
bool dequeue(queue*&en, enemy*&Enem)
{
	if (IsQueueEmpty(en)) return false;
	Node *temp = new Node;
	temp = en->front;
	*Enem = temp->Enemy;
	en->front = en->front->next;
	return true;
}
//*****************************************************************************************************************************************************//
bool InsertFront(LinkList*&list, enemy*E)
{
	Node *newNode = new Node;
	newNode->Enemy = *E;
	newNode->next = list->head;
	list->head = newNode;
	return true;
}
//*****************************************************************************************************************************************************//
bool insertNode(enemy* thenew, LinkList*&list) {
	Node* ptr = new Node; ptr->Enemy = *thenew;
	if (list->head == NULL) {
		ptr->next = NULL;
		list->head = ptr;
		return true;
	}
	Node* temp = list->head;
	while (temp != NULL) {
		if (temp->next == NULL) {
			ptr->next = NULL;
			temp->next = ptr;
			return true;
		}
		temp = temp->next;
	}
	return true;
}
//*****************************************************************************************************************************************************//
bool IsLinkEmpty(LinkList*list)
{
	if (list->head == NULL) return true;
	return false;
}
//*****************************************************************************************************************************************************//
void InitQueue(queue*&q)
{
	q->front = NULL;
	q->rear = NULL;
}
//*****************************************************************************************************************************************************//
bool IsQueueEmpty(queue*en)
{
	if (en->front == NULL) return true;
	return false;
}

//*****************************************************************************************************************************************************//
// File Loading Function
int Read(queue*&inactivE, castle*&ct, float&c1, float&c2, float&c3)
{
	int c=0;
	ifstream inp("Enemies.txt");
	inp >> ct->towers[0].Health >> ct->towers[0].N >> ct->towers[0].FP;
	ct->towers[1].Health = ct->towers[0].Health;     ct->towers[1].N = ct->towers[0].N;     ct->towers[1].FP = ct->towers[0].FP;
	ct->towers[2].Health = ct->towers[0].Health;     ct->towers[2].N = ct->towers[0].N;     ct->towers[2].FP = ct->towers[0].FP;
	ct->towers[3].Health = ct->towers[0].Health;     ct->towers[3].N = ct->towers[0].N;     ct->towers[3].FP = ct->towers[0].FP;
	

	inp >> c1 >> c2 >> c3;

	enemy*E = new enemy;
	E->ID = 0;
	while (!inp.eof())
	{
		int type;      char reg;
		inp >> E->ID >> type >> E->TS >> E->Health >> E->FP >> E->RP >> reg >> E->speed;
		if (E->ID != -1)
		{
			E->OH = E->Health;
			convert(E, reg, type);
			c++;
			bool x = enqueue(inactivE, E);
		}
	}
	inp.close();
	return c;
}
//*****************************************************************************************************************************************************//
void inactiv_to_activ(queue*&Inactiv, LinkList*&activ, int Tick)
{
	enemy*E = new enemy;
	bool x = dequeue(Inactiv, E);
	E->Distance = 60;                  //seting default distance.
	E->TS = Tick;                      //seting initial time step.
	insertNode(E, activ);
}
//*****************************************************************************************************************************************************//
void transfer(queue*&InactivE, LinkList*&RegA_H, LinkList*&RegB_H, LinkList*&RegC_H, LinkList*&RegD_H, 
	                           LinkList*&RegA_N, LinkList*&RegB_N, LinkList*&RegC_N, LinkList*&RegD_N, int Tick)
{
	while (InactivE->front->Enemy.TS == Tick)
	{
		if (InactivE->front->Enemy.Region == A_REG &&  InactivE->front->Enemy.Type == 2)        inactiv_to_activ(InactivE, RegA_H, Tick);
		else if (InactivE->front->Enemy.Region == A_REG)                                       inactiv_to_activ(InactivE, RegA_N, Tick);
		else if (InactivE->front->Enemy.Region == B_REG &&  InactivE->front->Enemy.Type == 2)   inactiv_to_activ(InactivE, RegB_H, Tick);
		else if (InactivE->front->Enemy.Region == B_REG)                                       inactiv_to_activ(InactivE, RegB_N, Tick);
		else if (InactivE->front->Enemy.Region == C_REG &&  InactivE->front->Enemy.Type == 2)   inactiv_to_activ(InactivE, RegC_H, Tick);
		else if (InactivE->front->Enemy.Region == C_REG)                                       inactiv_to_activ(InactivE, RegC_N, Tick);
		else if (InactivE->front->Enemy.Type == 2)                                             inactiv_to_activ(InactivE, RegD_H, Tick);
		else                                                                                  inactiv_to_activ(InactivE, RegD_N, Tick);
		if (InactivE->front == NULL) return;
	}
}
//*****************************************************************************************************************************************************//
int*random(int n)
{
	int*z = new int [2];
	while (z[0] >= z[1])
	{
		srand(static_cast<unsigned int>(time(0)));
		for (int i = 0; i < 2; i++)
		{
			z[i] = rand() % n + 1;
		}
	}
	return z;
}
//*****************************************************************************************************************************************************//
int return_size(LinkList*list)
{
	if (IsLinkEmpty(list)) return 0;
	int c = 0;
	Node *ptr = list->head;
	while (ptr != NULL)
	{
		c++;
		ptr = ptr->next;
	}
	return c;
}
//*****************************************************************************************************************************************************//
int return_killed_size(LinkList*list)
{
	Node*ptr = list->head;
	int c = 0;
	while (ptr != NULL)
	{
		if (ptr->Enemy.Health == 0)
			c++;
		ptr = ptr->next;
	}
	return c;
}
//*****************************************************************************************************************************************************//
//*****************************************************************************************************************************************************//
int size_queue(queue*q)
{
	int c = 0;
	enemy*x;
	while (!IsQueueEmpty(q))
	{
		c++;
		dequeue(q, x);
	}
	return c;
}
//*****************************************************************************************************************************************************//
void Remove_Killed(LinkList*&list)
{
	if (IsLinkEmpty(list)) return;

	if (list->head->Enemy.Health == 0 && list->head->next == NULL)
	{
		delete list->head;
		list->head = NULL;
		return;
	}

	Node *pre = list->head;
	Node *ptr = pre->next;
	while (pre->Enemy.Health == 0)
	{
		if (ptr == NULL)
		{
			delete list->head;
			list->head = NULL;
			return;
		}
		pre = ptr;
		ptr = ptr->next;
	}
	while (ptr != NULL)
	{
		if (ptr->Enemy.Health == 0)
		{
			pre = ptr->next;
		}
		pre = ptr;
		ptr = ptr->next;
	}
	return;
}
//*****************************************************************************************************************************************************//
void deleteNode(Node*&head, Node* deleted) {
	Node* temp = head;

	if (head->Enemy.ID == deleted->Enemy.ID) {
		if (head->next == NULL) {
			head = NULL;
			return;
		}
		head = head->next;
		delete temp;
		return;
	}
	Node* pre = NULL;
	while (temp != NULL) {
		if (temp->Enemy.ID == deleted->Enemy.ID) {
			pre->next = temp->next;
			return;
		}
		pre = temp;
		temp = temp->next;
	}
}
//*****************************************************************************************************************************************************//
bool DelLinkedList(LinkList*L, int loc) {
	int count = -1;
	if (L->head == NULL)
		return false;

	if (loc == 0) {
		Node*temp = L->head;
		L->head = temp->next;
		delete temp;
		return true;
	}
	Node* temp = L->head;
	Node* pre = NULL;
	while (temp != NULL&&count != (loc - 1)) {
		count++;
		pre = temp;
		temp = temp->next;
	}
	pre->next = temp->next;
	delete temp;
	return true;
}
//*****************************************************************************************************************************************************//
castle*Initiallizations(queue*&InactivE)
{
	castle*ct = new castle;
	ct->Xstrt = CastleXStrt;
	ct->Ystrt = CastleYStrt;
	ct->W = CastleWidth;
	ct->L = CastleLength;


	LinkList *RegA_H = new LinkList; LinkList *RegB_H = new LinkList; LinkList *RegC_H = new LinkList; LinkList *RegD_H = new LinkList; 
	LinkList *RegA_N = new LinkList; LinkList *RegB_N = new LinkList; LinkList *RegC_N = new LinkList; LinkList *RegD_N = new LinkList; 
	LinkList*KillA = new LinkList; LinkList*KillB = new LinkList; LinkList*KillC = new LinkList; LinkList*KillD = new LinkList;

	InitLink(RegA_H);
	InitLink(RegA_N);
	InitLink(RegB_N);
	InitLink(RegB_H);
	InitLink(RegC_H);
	InitLink(RegC_N);
	InitLink(RegD_N);
	InitLink(RegD_H);
	InitLink(KillA);
	InitLink(KillB);
	InitLink(KillC);
	InitLink(KillD);


	ct->towers[0].HI = RegA_H; ct->towers[1].HI = RegB_H; ct->towers[2].HI = RegC_H; ct->towers[3].HI = RegD_H;
	ct->towers[0].ACT = RegA_N; ct->towers[1].ACT = RegB_N; ct->towers[2].ACT = RegC_N; ct->towers[3].ACT = RegD_N;
	ct->towers[0].Kill = KillA; ct->towers[1].Kill = KillB; ct->towers[2].Kill = KillC; ct->towers[3].Kill = KillD;
	return ct;
}
//*****************************************************************************************************************************************************//
void DispInf(LinkList*list_H, LinkList*list_N, LinkList*Killed_list, int*KillCount)
{
	if (IsLinkEmpty(list_H) && IsLinkEmpty(list_N)) cout << "This region has no active enemies now.\n";
	else
	{
		Node *ptr = list_H->head;
		int c = return_size(list_H) + return_size(list_N) - return_killed_size(list_H) - return_killed_size(list_N);
		cout << "Total number of active enemies =  " << c << endl;
			while (ptr != NULL)
			{
				if (ptr->Enemy.Health != 0)
				{
					char reg;
					revert(&ptr->Enemy, reg);
					cout << "ID: " << ptr->Enemy.ID << "\t" << "Time Step: " << ptr->Enemy.TS << "\t" << "Health: " << ptr->Enemy.Health << "\t" << "Firepower: " << ptr->Enemy.FP << "\t" << "Region: " << reg << "\t" << "Type: " << ptr->Enemy.Type << "\t" << "Reload Period: " << ptr->Enemy.RP << endl;
				}
				ptr = ptr->next;
			}
		ptr = list_N->head;
		while (ptr != NULL)
		{
			if (ptr->Enemy.Health != 0)
			{
				char reg;
				revert(&ptr->Enemy, reg);
				cout << "ID: " << ptr->Enemy.ID << "\t" << "Time Step: " << ptr->Enemy.TS << "\t" << "Health: " << ptr->Enemy.Health << "\t" << "Firepower: " << ptr->Enemy.FP << "\t" << "Region: " << reg << "\t" << "Type: " << ptr->Enemy.Type << "\t" << "Reload Period: " << ptr->Enemy.RP << endl;
			}
			ptr = ptr->next;
		}

	}

	if (IsLinkEmpty(Killed_list)) cout << "This region has no killed enemies now.\n";
	else
	{
		cout << "Number of enemies killed in this region=  " << return_size(Killed_list) << endl << endl;
		Node*ptr = Killed_list->head;
		while (ptr != NULL)
		{
			char reg;
			revert(&ptr->Enemy, reg);
			cout << "ID: " << ptr->Enemy.ID << "\t" << "Time Step: " << ptr->Enemy.TS << "\t" << "Health: " << ptr->Enemy.Health << "\t" << "Firepower: " << ptr->Enemy.FP << "\t" << "Region: " << reg << "\t" << "Type: " << ptr->Enemy.Type << "\t" << "Reload Period: " << ptr->Enemy.RP << endl;
			ptr = ptr->next;
		}
	}
}
//*****************************************************************************************************************************************************//
void EnemiesRotation(castle*&ct, int i, int*Paved)
{
	Node*ptr = ct->towers[i].ACT->head;
	while (ptr != NULL)
	{
		if (i == 0)
		{
			Node*temp = ptr->next;
			ptr->Enemy.Region = B_REG;
			if (ptr->Enemy.Distance< Paved[(i + 1) % 4])
			ptr->Enemy.Distance = Paved[(i + 1) % 4];
			insertNode(&ptr->Enemy, ct->towers[(i+1) % 4].ACT);
			deleteNode(ct->towers[i].ACT->head, ptr);
			ptr = temp;
		}
		if (i == 1)
		{
			Node*temp = ptr->next;
			ptr->Enemy.Region = C_REG;
			if (ptr->Enemy.Distance< Paved[(i + 1) % 4])
				ptr->Enemy.Distance = Paved[(i + 1) % 4];
			insertNode(&ptr->Enemy, ct->towers[(i + 1) % 4].ACT);
			deleteNode(ct->towers[i].ACT->head, ptr);
			ptr = temp;
		}
		if (i == 2)
		{
			Node*temp = ptr->next;
			ptr->Enemy.Region = D_REG;
			if (ptr->Enemy.Distance< Paved[(i + 1) % 4])
				ptr->Enemy.Distance = Paved[(i + 1) % 4];
			insertNode(&ptr->Enemy, ct->towers[(i + 1) % 4].ACT);
			deleteNode(ct->towers[i].ACT->head, ptr);
			ptr = temp;

		}
		if (i == 3)
		{
			Node*temp = ptr->next;
			ptr->Enemy.Region = A_REG;
			if (ptr->Enemy.Distance< Paved[(i + 1) % 4])
				ptr->Enemy.Distance = Paved[(i + 1) % 4];
			insertNode(&ptr->Enemy, ct->towers[(i + 1) % 4].ACT);
			deleteNode(ct->towers[i].ACT->head, ptr);
			ptr = temp;
		}
	}

	ptr = ct->towers[i].HI->head;
	while (ptr != NULL)
	{
		if (i == 0)
		{
			Node*temp = ptr->next;
			ptr->Enemy.Region = B_REG;
			if (ptr->Enemy.Distance< Paved[(i + 1) % 4])
				ptr->Enemy.Distance = Paved[(i + 1) % 4];
			insertNode(&ptr->Enemy, ct->towers[(i + 1) % 4].HI);
			deleteNode(ct->towers[i].HI->head, ptr);
			ptr = temp;
		}
		if (i == 1)
		{
			Node*temp = ptr->next;
			ptr->Enemy.Region = C_REG;
			if (ptr->Enemy.Distance< Paved[(i + 1) % 4])
				ptr->Enemy.Distance = Paved[(i + 1) % 4];
			insertNode(&ptr->Enemy, ct->towers[(i + 1) % 4].HI);
			deleteNode(ct->towers[i].HI->head, ptr);
			ptr = temp;
		}
		if (i == 2)
		{
			Node*temp = ptr->next;
			ptr->Enemy.Region = D_REG;
			if (ptr->Enemy.Distance< Paved[(i + 1) % 4])
				ptr->Enemy.Distance = Paved[(i + 1) % 4];
			insertNode(&ptr->Enemy, ct->towers[(i + 1) % 4].HI);
			deleteNode(ct->towers[i].HI->head, ptr);
			ptr = temp;
		}
		if (i == 3)
		{
			Node*temp = ptr->next;
			ptr->Enemy.Region = A_REG;
			if (ptr->Enemy.Distance< Paved[(i + 1) % 4])
				ptr->Enemy.Distance = Paved[(i + 1) % 4];
			insertNode(&ptr->Enemy, ct->towers[(i + 1) % 4].HI);
			deleteNode(ct->towers[i].HI->head, ptr);
			ptr = temp;
		}
	}
}
//*****************************************************************************************************************************************************//
void MoveEnemies(castle*ct, int Tick, int*Paved)
{
	for (int i = 0; i<4; i++) 
	{
		Node*temp = ct->towers[i].ACT->head;
			while (temp != NULL)
			{
				if (temp->Enemy.Type != PVR) 
				{
					if ((temp->Enemy.Distance - temp->Enemy.speed) >= Paved[i])
					{
							temp->Enemy.Distance -= temp->Enemy.speed;
					}
					else
					{
						temp->Enemy.Distance = Paved[i];
					}
				}
				if (temp->Enemy.Type == PVR)
				{
					if ((temp->Enemy.Distance - temp->Enemy.FP) < 2)
						temp->Enemy.Distance = 2;
					else
						temp->Enemy.Distance -= temp->Enemy.FP;
				}
				temp = temp->next;
			}
			temp = ct->towers[i].HI->head;
			while (temp != NULL)
			{
				if ((temp->Enemy.Distance - temp->Enemy.speed) >= Paved[i])
					temp->Enemy.Distance -= temp->Enemy.speed;
				else
					temp->Enemy.Distance = Paved[i];
				
				temp = temp->next;
			}
			if (Paved[i] <= 0) Paved[i] = 2;
		
	}
}
//*****************************************************************************************************************************************************//
void Paving(castle*ct, int*Paved)
{
	for (int i = 0; i < 4; i++)
	{
		Node*ptr = ct->towers[i].ACT->head;
		if (Paved[i] > 2)
		{
			while (ptr != NULL)
			{
				if (ptr->Enemy.Type == PVR)
				{
					if (((ptr->Enemy.Distance) - (ptr->Enemy.FP)) < Paved[i])
					{
						if ((ptr->Enemy.Distance - ptr->Enemy.FP) < 2) 
							Paved[i] = 2;
						else 
							Paved[i] -= (ptr->Enemy.FP) - (Paved[i] - ptr->Enemy.Distance);
					}
				}
				ptr = ptr->next;
			}
		}
	}
}
//*****************************************************************************************************************************************************//

void EnemiesFire(castle*&ct, int Tick, int*Paved)
{

	for (int i = 0; i < 4; i++)
	{
		Node*ptr = ct->towers[i].ACT->head;
		while (ptr != NULL)
		{
				if (ct->towers[i].Health != 0)
				{
					if ((Tick - ptr->Enemy.TS)%ptr->Enemy.RP == 0)
					{
						if (ptr->Enemy.Type == FITR)
							ct->towers[i].Health -= (1 / (ptr->Enemy.Distance))*(ptr->Enemy.FP);
						if (ct->towers[i].Health < 0)
							ct->towers[i].Health = 0;
					}
				}
			
			ptr = ptr->next;
		}
		ptr = ct->towers[i].HI->head;
		while (ptr != NULL)
		{
			if (ct->towers[i].Health != 0)
			{
				if ((Tick - ptr->Enemy.TS)%ptr->Enemy.RP == 0)
				{
					ct->towers[i].Health -= (2 / (ptr->Enemy.Distance))*(ptr->Enemy.FP);
					if (ct->towers[i].Health < 0)
						ct->towers[i].Health = 0;
				}
			}

			ptr = ptr->next;
		}

		if (ct->towers[i].Health == 0)
			EnemiesRotation(ct, i, Paved);
	}
}
//*****************************************************************************************************************************************************//
int*TowersFire_H(castle*&ct, int Tick)
{
	int n = 0;
	int*rem=new int;
	Node*ptr;
	for (int i = 0; i < 4; i++)
	{
		ptr = ct->towers[i].HI->head;
		while (ptr != NULL)
		{
			if (n == ct->towers[i].N)
				break;
			if (ptr->Enemy.Health == ptr->Enemy.OH) ptr->Enemy.FSH = Tick;
			ptr->Enemy.Health = ptr->Enemy.Health - (1 / ptr->Enemy.Distance)*(ct->towers[i].FP);
			if (ptr->Enemy.Health < 0) ptr->Enemy.Health = 0;
			n++;
			ptr = ptr->next;
		}
		rem[i] = ct->towers[i].N - n;
	}
	return rem;
}
//*****************************************************************************************************************************************************//
void TowersFire_N(castle*&ct, int*rem, int Tick)
{
	int n = 0;
	Node*ptr;
	for (int i = 0; i < 4; i++)
	{
		ptr = ct->towers[i].ACT->head;
			while (ptr != NULL)
			{
				if (n == rem[i])
					break;
				if (ptr->Enemy.Health == ptr->Enemy.OH) ptr->Enemy.FSH = Tick;
					ptr->Enemy.Health -= (1 / ptr->Enemy.Distance)*((ct->towers[i].FP)*0.5);
					if (ptr->Enemy.Health < 0) ptr->Enemy.Health = 0;
					n++;
				ptr = ptr->next;
			}
	}
}
//*****************************************************************************************************************************************************//
void sorting(castle*&ct)
{
	for (int j = 0; j < 4; j++)
	{
		int size = return_size(ct->towers[j].HI);
		LinkList*L = ct->towers[j].HI;
		if (L->head != NULL)
		{
			Node*k = L->head;
			LinkList*b = new LinkList;
			InitLink(b);
			float*arr = new float[size];
			int i = 0;
			while (k != NULL)
			{
				arr[i] = k->Enemy.prior;
				i++;
				k = k->next;
			}
			k = L->head;
			int x; int temp; int current = 0; int walker = 0; float smallest = 0.0;
			for (current; current < i; current++)
			{
				smallest = arr[current];
				x = 0;
				walker = current + 1;
				for (walker; walker < i; walker++)
				{
					if (arr[walker] < smallest)
					{
						smallest = arr[walker]; temp = walker; x++;
					}
				}
				if (x > 0)
				{
					arr[temp] = arr[current]; arr[current] = smallest;
				}
			}
			for (int t = 0; t < size; t++)
			{
				k = L->head;
				while (k != NULL)
				{
					Node* temp = k->next;
					if (arr[t] == k->Enemy.prior)
					{
						InsertFront(b, &k->Enemy); deleteNode(L->head, k);
					}
					k=temp;
				}
			}
			ct->towers[j].HI = b;
		}
	}
}
//*****************************************************************************************************************************************************//
void PriorTowersFire(castle*ct, int Tick)
{
	PriorCalc(ct, Tick);
	sorting(ct);
	int*rem = TowersFire_H(ct, Tick);
	TowersFire_N(ct, rem, Tick);
}
//*****************************************************************************************************************************************************//
void toArray(queuearr*&q, LinkList*list) 
{

	Node*temp = list->head;
	while (temp != NULL) {
		enqueueA(q, 100, &temp->Enemy);
		temp = temp->next;
	}

}

//*****************************************************************************************************************************************************//
int preDraw(queuearr*&all_enemies, castle*ct)
{
	for (int i = 0; i<4; i++)
	{
		toArray(all_enemies, ct->towers[i].ACT);
		toArray(all_enemies, ct->towers[i].HI);
	}
	if (isEmptyQ(all_enemies))
		return 0;
	return ((all_enemies->rear) - (all_enemies->front))+1;
}
//*****************************************************************************************************************************************************//
void removeKilled(castle*&ct, int Tick) 
{
	for (int i = 0; i<4; i++) {
		Node*temp2 = ct->towers[i].HI->head;
		Node*temp = ct->towers[i].ACT->head;
		while (temp != NULL) {
			Node* temp3 = temp->next;
			if (temp->Enemy.Health == 0) {
				temp->Enemy.KT = Tick;
				insertNode(&temp->Enemy, ct->towers[i].Kill);
				deleteNode(ct->towers[i].ACT->head, temp);
				ct->towers[i].LKT++;
			}
			temp = temp3;
		}
		while (temp2 != NULL) {
			Node* temp3 = temp2->next;
			if (temp2->Enemy.Health == 0) {
				temp2->Enemy.KT = Tick;
				insertNode(&temp2->Enemy, ct->towers[i].Kill);
				deleteNode(ct->towers[i].HI->head, temp2);
			}
			temp2=temp3;
		}
	}
}

//*****************************************************************************************************************************************************//
void PriorCalc(castle*&ct, int Tick)
{
	for (int i = 0; i < 4; i++)
	{
		Node*ptr = ct->towers[i].HI->head;
		while (ptr != NULL)
		{
			ptr->Enemy.prior = (((ptr->Enemy.FP)*ct->c1 / (ptr->Enemy.Distance)) + (ct->c2 / ((Tick% ptr->Enemy.RP + ptr->Enemy.TS% ptr->Enemy.RP) + 1) + (ct->c3*ptr->Enemy.Health)));
			ptr = ptr->next;
		}
	}
}
//*****************************************************************************************************************************************************//

int KilledSizes(castle*ct, queue*InactivE)
{
	int k=0;
	for (int i = 0; i < 4; i++)
	{
		k += return_size(ct->towers[i].Kill);
	}
	return k;
}
//*****************************************************************************************************************************************************//
queuearr*List_to_queue(castle*ct)
{
	queuearr*Killed = initQueue();
	for (int i = 0; i < 4; i++)
	{
		Node*ptr = ct->towers[i].Kill->head;
		while (ptr != NULL)
		{
			enqueueA(Killed, 100, &ptr->Enemy);
			ptr = ptr->next;
		}
		
	}
	return Killed;
}
//*****************************************************************************************************************************************************//
void sortingoutputKTS(output*&arr, int size)
{
	int x;
	int temp = 0;
	int walker = 0;
	int smallest = 0;
	output smallest1;
	output tempo;
	for (int current = 0; current < size; current++)
	{
		walker = current + 1;
		smallest = arr[current].KTS;
		x = 0;
		for (walker; walker < size; walker++)
		{
			if (arr[walker].KTS < smallest)
			{
				smallest1 = arr[walker];
				smallest = arr[walker].KTS;
				temp = walker; x++;
			}
		}
		if (x > 0)
		{
			arr[temp] = arr[current];
			arr[current] = smallest1;
		}
	}
}
//*****************************************************************************************************************************************************//
void sortingoutputFD(output*&arr, int size)
{
	int x;
	int temp = 0;
	int walker = 0;
	int smallest = 0;
	output smallest1;
	output tempo;
	for (int current = 0; current < size; current++)
	{
		walker = current + 1;
		smallest = arr[current].FD;
		x = 0;
		for (walker; walker < size; walker++)
		{
			if (arr[walker].FD < smallest)
			{
				smallest1 = arr[walker];
				smallest = arr[walker].FD;
				temp = walker; x++;
			}
		}
		if (x > 0)
		{
			arr[temp] = arr[current];
			arr[current] = smallest1;
		}
	}
}
//*****************************************************************************************************************************************************//
void Outputfile(int m, queuearr*q, castle*c, int*paved, int x, int nom_enemies)
{

	output*out = new output[m];
	ofstream Out("Outputfile.txt", ios::out);
	Out << "KTS\tS\tFD\tKD\tFT\n";
	for (int i = 0; i<m; i++)
	{
		out[i].FD = (q->arr[i]->FSH - q->arr[i]->TS);
		out[i].KD = (q->arr[i]->KT - q->arr[i]->FSH);
		out[i].FT = (q->arr[i]->KT - q->arr[i]->TS);
		out[i].KTS = q->arr[i]->KT;
	}
	sortingoutputFD(out, m);
	sortingoutputKTS(out, m);
	
	for (int j = 0; j<m; j++)
	{
		Out << out[j].KTS << "\t" << (j + 1) << "\t" << out[j].FD << "\t" << out[j].KD << "\t" << out[j].FT << "\t\n";
	}
	Out << "T1_total_damage\t T2_total_damage\tT3_total_damage\t T4_total_damage\n";
	Out << (200 - c->towers[0].Health)<<"\t\t\t\t\t"<< (200 - c->towers[1].Health)<<"\t\t\t\t\t"<<(200 - c->towers[2].Health)<<"\t\t\t\t\t"<<(200 - c->towers[3].Health)<<"\t\t\t\t\n";
	Out << "R1-Distance\t R2-Distance\t R3-Distance\t R4-Distance\t\n";
	Out << paved[0] << "\t\t\t\t" << paved[1] << "\t\t\t\t" << paved[2] << "\t\t\t\t" << paved[3] << "\t\t\t\t\n";
	float avgFD;
	float sum = 0.0;
	float sum1 = 0.0;
	float avgKD;
	for (int i = 0; i<m; i++)
	{
		sum = out[i].FD + sum; sum1 = out[i].KD + sum1;
	}
	avgFD = sum / m;
	avgKD = sum1 / m;
	if (x == 1)
	{
		Out << "GAME IS WIN\n";
		Out << "Total Enimies=\t" << nom_enemies << "\n";

		Out << "Average Fight Delay=\t" << avgFD << "\n";
		Out << "Average kill Delay=\t" << avgKD << "\n";
	}
	if (x == 0)
	{
		Out<<"GAME IS LOSE\n";
		Out << "Number of killed enemies=\t" << m << "\n";
		Out << "Nmber of alive enemies=\t" << (nom_enemies - m) << "\n";
		Out << "Average Fight Delay=\t" << avgFD << "\n";
		Out << "Average kill Delay=\t" << avgKD << "\n";
	}
}
//*****************************************************************************************************************************************************//
int ModeSelector()
{
	int c = 0;
	cout << "Welcome to Castle Simulation Game" << endl;

	while (!(c == 1 || c == 2 || c == 3))
	{
		cout << "Please enter the type of mode..\n 1)Interactive Mode. \n 2)Silent Mode. \n 3)Step Mode." << endl;
		cin >> c;
		if (!(c == 1 || c == 2 || c == 3))
			cout << "Error please enter applicable number" << endl;
	}
	return c;
}

void DispStepInf(castle*ct, int*Paved)
{
	cout << "\n";
	cout << "Reigon: " << "\t\t\t\t\t\t";
		cout <<"A \t" << "B \t"<< "C \t"<< "D \t";
		cout << "\n";
		PrintMsg("Total Number of current active enemies = "); cout << "\t\t" << (return_size(ct->towers[0].ACT) + return_size(ct->towers[0].HI)) << "\t" << (return_size(ct->towers[1].ACT) + return_size(ct->towers[1].HI)) << "\t" << (return_size(ct->towers[2].ACT) + return_size(ct->towers[2].HI)) << "\t" << (return_size(ct->towers[3].ACT) + return_size(ct->towers[3].HI)) << endl;
		PrintMsg("Total Number of killed enemies at the last TS = "); cout << "\t" <<ct->towers[0].LKT << "\t" << ct->towers[1].LKT << "\t" << ct->towers[2].LKT << "\t" << ct->towers[3].LKT << endl;
		PrintMsg("Total Number of killed enemies from the begining = "); cout<< "\t" << return_size(ct->towers[0].Kill) << "\t" << return_size(ct->towers[1].Kill) << "\t" << return_size(ct->towers[2].Kill) << "\t" << return_size(ct->towers[3].Kill) << endl;
		PrintMsg("Unpaved distance to castle = "); cout << "\t\t\t\t" << Paved[0]  << "\t" << Paved[1] << "\t" << Paved[2] << "\t" << Paved[3] << endl;

	
}

/*Simulating Function*/
void simulateprogram(queue*&InactivE, castle *&ct, float &c1, float &c2, float &c3)
{
	int c=Read(InactivE, ct, c1, c2, c3);
	int win= 1;
	int Tick = 1;
	int*Paved = new int[3];
	Paved[0] = Paved[1] = Paved[2] = Paved[3] = 30;
	ct->c1 = c1; ct->c2=c2; ct->c3=c3;
	int mode = ModeSelector();
	
	
	while (InactivE->front != NULL || ct->towers[0].ACT->head != NULL || ct->towers[0].HI->head != NULL || ct->towers[1].ACT->head != NULL || ct->towers[1].HI->head != NULL || ct->towers[2].ACT->head != NULL || ct->towers[2].HI->head != NULL || ct->towers[3].ACT->head != NULL || ct->towers[3].HI->head != NULL)
	{
		ct->towers[0].LKT = 0;
		ct->towers[1].LKT = 0;
		ct->towers[2].LKT = 0;
		ct->towers[3].LKT = 0;
		if (!IsQueueEmpty(InactivE))
			transfer(InactivE, ct->towers[0].HI, ct->towers[1].HI, ct->towers[2].HI, ct->towers[3].HI, ct->towers[0].ACT, ct->towers[1].ACT, ct->towers[2].ACT, ct->towers[3].ACT, Tick);
	
		MoveEnemies(ct, Tick, Paved);
		Paving(ct, Paved);
		EnemiesFire(ct, Tick, Paved);
		PriorTowersFire(ct, Tick);
		removeKilled(ct, Tick);

		queuearr*all_enemies;
		all_enemies = initQueue();
		int enemyCount = preDraw(all_enemies, ct);
		if (mode != 2)
		{
			DrawCastle(*ct, Tick);
			DrawEnemies(all_enemies->arr, enemyCount);
		}
			DestroyQueue(all_enemies, enemyCount);
		if (ct->towers[0].Health == 0 && ct->towers[1].Health == 0 && ct->towers[2].Health == 0 && ct->towers[3].Health == 0) {
			win = 0;
			break;
		}
		
		
		DispStepInf(ct, Paved);
		if (mode == 3)
		{
			cin.get();
		}
		if (mode!=2)
		Sleep(500);
		Tick++;	
	}
	int k=KilledSizes(ct, InactivE);
	queuearr*Killed_Queue = List_to_queue(ct);
	Outputfile(k, Killed_Queue, ct, Paved, win,c);
	if (mode == 2)
		cout << "A file has been saved.." << endl;
}
							
//*****************************************************************************************************************************************************//
