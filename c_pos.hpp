/*
Author: Shriraj P. Sawant
Contact: sr.official@hotmail.com
*/

//Simple API for basically manipulating cursor positions and string colors
//Note:- This API is abridged version of my original, having capabilities requiring only for AVL. 
#ifndef _CPOS
#define _CPOS
#include<windows.h>

#define BF FOREGROUND_BLUE 
#define GF FOREGROUND_GREEN
#define RF FOREGROUND_RED 
#define IF FOREGROUND_INTENSITY 
#define BB BACKGROUND_BLUE 
#define GB BACKGROUND_GREEN 
#define RB BACKGROUND_RED 
#define IB BACKGROUND_INTENSITY
#define PF BF|RF
#define YF GF|RF
#define AF BF|GF
#define PB BB|RB
#define YB GB|RB
#define AB BB|GB

#define BD BB|BF
#define GD GB|GF
#define RD RB|RF
#define PD BB|RB|BF|RF
#define YD GB|RB|GF|RF
#define AD BB|GB|BF|GF
#define ID IB|IF

#define WF BF|GF|RF
#define WB BB|GB|RB
#define WD WF|WB 

using namespace std;

class setcurs //manip of gotoxy
{
	COORD cursor;
	public:		
	setcurs(COORD t) {cursor=t;}	
	setcurs(short x, short y){cursor.X=x; cursor.Y=y;}
	friend ostream& operator<<(ostream& out, setcurs cur);	
};
class setcolor //manip to change colorof text and bckgrnd
{
	WORD color;	
	void setcol()
	{
		HANDLE OutputH;
		OutputH = GetStdHandle(STD_OUTPUT_HANDLE);
		SetConsoleTextAttribute(OutputH, this->color);
	}
	public:
	setcolor(WORD t) {color=t;}
	friend ostream& operator<<(ostream& out, setcolor col);
	friend ostream& resetcolor(ostream& out);	
};
void gotoxy(short x, short y)
{
	COORD pos={x,y};
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE),pos);
}
void gotoxy(COORD z)
{
	COORD pos=z;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE),pos);
}
COORD wherexy() //returns present pos of cursor
{
	COORD scpos;
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	if(!GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE),&csbi)) scpos.X=scpos.Y=-1;
	else{
		scpos=csbi.dwCursorPosition;
	}
	return scpos;
}
SHORT cX() {return wherexy().X;}
SHORT cY() {return wherexy().Y;}

ostream& operator<<(ostream& out, setcurs cur)
{
	gotoxy(cur.cursor);
	return out;
}
ostream& operator<<(ostream& out, setcolor col)
{
	col.setcol();
	return out;
}
ostream& resetcolor(ostream& out)
{
	setcolor col(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
	col.setcol();
	return out;
}
#endif
