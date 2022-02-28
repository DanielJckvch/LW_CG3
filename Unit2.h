//---------------------------------------------------------------------------

#ifndef Unit2H
#define Unit2H
class MyPoint
{
private:
char let;

int x;
int y;

int x0;
int y0;

public:

int get_x();
int get_y();
char get_let();
void set_x(int a);
void set_y(int a);
void set_x();
void set_y();
~MyPoint();
MyPoint(char l, int i, int j);
 MyPoint();
} ;
//---------------------------------------------------------------------------
#endif
 