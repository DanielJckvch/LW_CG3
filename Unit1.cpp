//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "Unit1.h"
#include "Unit2.h"
#include <math.h>
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TForm1 *Form1;
//Шестиугольник. Сделать вращение, масштабирование, перемещение, закраска
void printhexagon(MyPoint* hexagon, TImage* Image1);
void rotandscale(TImage* Image1, double par, int sw, char a, char b, MyPoint& V);
void hexagoninit(MyPoint* h);
MyPoint* hexagon=new MyPoint[7];
int x=200;
int y=200;
int rad=100;
double a_step=10;
int sc_step=2;
int mov_step=10;

//---------------------------------------------------------------------------
__fastcall TForm1::TForm1(TComponent* Owner)
        : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TForm1::Form1Create(TObject *Sender)
{
//Инициализация и вывод шестиугольнка
hexagoninit(hexagon);
printhexagon(hexagon, Image1);
}
//---------------------------------------------------------------------------
void printhexagon(MyPoint* hexagon, TImage* Image1)
{

int angle=60;
//Матрица поворота
double r[3][3];
 r[0][0]=cos(angle*3.14/180);
 r[0][1]=sin(angle*3.14/180);
 r[0][2]=0;
 r[1][0]=-sin(angle*3.14/180);
 r[1][1]=cos(angle*3.14/180);
 r[1][2]=0;
 r[2][0]=0;
 r[2][1]=0;
 r[2][2]=1;
bool out_f=true;
int k;
int i, j;
//Для проверки выхода точек в отрицательную область
while(out_f)
{
out_f=false;
int v1[3];
v1[0]=hexagon[1].get_x();
v1[1]=hexagon[1].get_y();
v1[2]=1;

int v2[3]={0, 0, 1};
//Вычисление координат точек относительно 0

for(k=2; k<8; k++)
{
for (i = 0;i < 3;i++)
{
        double sum = 0;
        for (j = 0;j < 3;j++)
        {
                sum= sum+ (r[j][i] * v1[j]);
        }
        v2[i] = sum;
}
 //Проверка на выход точки за холст
v1[0]=v2[0];
v1[1]=v2[1];
if(v2[0]+hexagon[0].get_x()<=0||v2[1]+hexagon[0].get_y()<=0)
{
hexagon[0].set_x();
hexagon[0].set_y();
hexagon[1].set_x();
hexagon[1].set_y();
out_f=true;
break;
 }
hexagon[k].set_x(v2[0]);
hexagon[k].set_y(v2[1]);
}

}
//Отрисовка контура
int off_x, off_y;
for(k=1; k<6; k++)
{
Image1->Canvas->MoveTo(hexagon[k].get_x()+hexagon[0].get_x(),hexagon[k].get_y()+hexagon[0].get_y());
Image1->Canvas->LineTo(hexagon[k+1].get_x()+hexagon[0].get_x(),hexagon[k+1].get_y()+hexagon[0].get_y());
}
Image1->Canvas->MoveTo(hexagon[k].get_x()+hexagon[0].get_x(),hexagon[k].get_y()+hexagon[0].get_y());
Image1->Canvas->LineTo(hexagon[1].get_x()+hexagon[0].get_x(),hexagon[1].get_y()+hexagon[0].get_y());
//Закраска
for(j=0; j<441; j++)
{
int f_pix=-1;
for(i=0; i<537; i++)
{
   if((Image1->Canvas->Pixels[i][j]==clBlack)&&(f_pix==-1))
   {
    f_pix=i;
   }
   else if((f_pix!=-1)&&(Image1->Canvas->Pixels[i][j]==clBlack)&&(Image1->Canvas->Pixels[i-1][j]!=clBlack))
   {
      for(k=i-1; k>f_pix;k--)
      {Image1->Canvas->Pixels[k][j]=clRed;}
   }
}
}
//Расставление букв точек
for(k=1; k<7; k++)
{
//Определение значения отступа для буквы
off_x=(hexagon[k].get_x()<0)?-6:4;
off_y=(hexagon[k].get_y()<0)?-12:2;

Image1->Canvas->MoveTo(hexagon[k].get_x()+hexagon[0].get_x(),hexagon[k].get_y()+hexagon[0].get_y());
Image1->Canvas->TextOutA(hexagon[k].get_x()+hexagon[0].get_x()+off_x,hexagon[k].get_y()+hexagon[0].get_y()+off_y,hexagon[k].get_let());
}

}

void rotandscale(TImage* Image1, double par, int sw, MyPoint& V)
{
double r[3][3];
// Выбор способа преобразования
 switch(sw)
 {
 case 0:
 r[0][0]=cos(par*3.14/180);
 r[0][1]=sin(par*3.14/180);
 r[0][2]=0;
 r[1][0]=-sin(par*3.14/180);
 r[1][1]=cos(par*3.14/180);
 r[1][2]=0;
 r[2][0]=0;
 r[2][1]=0;
 r[2][2]=1;
 break;
 case 1:
 r[0][0]=par;
 r[0][1]=0;
 r[0][2]=0;
 r[1][0]=0;
 r[1][1]=par;
 r[1][2]=0;
 r[2][0]=0;
 r[2][1]=0;
 r[2][2]=1;
 break;
 }
// Преобразование точки V
int v1[3]={V.get_x(),V.get_y(),1};
int v2[3]={0, 0, 1};
int i, j;
for (i = 0;i < 3;i++)
{
        double sum = 0;
        for (j = 0;j < 3;j++)
        {
                sum= sum+ (r[j][i] * v1[j]);
        }

        v2[i] = sum;
        if(v2[i]-sum>=0.5)
        {
        v2[i]++;
        }
}
V.set_x(v2[0]);
V.set_y(v2[1]);
}

void __fastcall TForm1::Button1Click(TObject *Sender)
{
//Поворот против часовой стрелки
rotandscale(Image1, -a_step, 0, (hexagon[1]));
//Очистка холста
TRect rct;
rct = Rect(0,0,Image1->Width,Image1->Height);
Image1->Canvas->Brush->Style=bsSolid;
Image1->Canvas->FillRect(rct);
 //Отрисовка
printhexagon(hexagon, Image1);
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Button2Click(TObject *Sender)
{
//Поворот по часовой стрелке
rotandscale(Image1, a_step, 0, (hexagon[1]));
//Очистка холста
TRect rct;
rct = Rect(0,0,Image1->Width,Image1->Height);
Image1->Canvas->Brush->Style=bsSolid;
Image1->Canvas->FillRect(rct);
 //Отрисовка
printhexagon(hexagon, Image1);
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Button3Click(TObject *Sender)
{
//?Масштаб -
rotandscale(Image1, (double)1/sc_step, 1, (hexagon[1]));
//Очистка холста
TRect rct;
rct = Rect(0,0,Image1->Width,Image1->Height);
Image1->Canvas->Brush->Style=bsSolid;
Image1->Canvas->FillRect(rct);
 //Отрисовка
printhexagon(hexagon, Image1);
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Button4Click(TObject *Sender)
{
//Масштаб +
rotandscale(Image1, sc_step, 1, (hexagon[1]));
//Очистка холста
TRect rct;
rct = Rect(0,0,Image1->Width,Image1->Height);
Image1->Canvas->Brush->Style=bsSolid;
Image1->Canvas->FillRect(rct);
 //Отрисовка
printhexagon(hexagon, Image1);
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Button5Click(TObject *Sender)
{
//Сдвиг влево
hexagon[0].set_x(hexagon[0].get_x()-mov_step);
//Очистка холста
TRect rct;
rct = Rect(0,0,Image1->Width,Image1->Height);
Image1->Canvas->Brush->Style=bsSolid;
Image1->Canvas->FillRect(rct);
//Отрисовка
printhexagon(hexagon, Image1);
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Button6Click(TObject *Sender)
{
//Сдвиг вправо
hexagon[0].set_x(hexagon[0].get_x()+mov_step);
//Очистка холста
TRect rct;
rct = Rect(0,0,Image1->Width,Image1->Height);
Image1->Canvas->Brush->Style=bsSolid;
Image1->Canvas->FillRect(rct);
//Отрисовка
printhexagon(hexagon, Image1);
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Button7Click(TObject *Sender)
{
//Сдвиг вниз
hexagon[0].set_y(hexagon[0].get_y()+mov_step);
//Очистка холста
TRect rct;
rct = Rect(0,0,Image1->Width,Image1->Height);
Image1->Canvas->Brush->Style=bsSolid;
Image1->Canvas->FillRect(rct);
 //Отрисовка
printhexagon(hexagon, Image1);
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Button8Click(TObject *Sender)
{
//Сдвиг вверх
hexagon[0].set_y(hexagon[0].get_y()-mov_step);
//Очистка холста
TRect rct;
rct = Rect(0,0,Image1->Width,Image1->Height);
Image1->Canvas->Brush->Style=bsSolid;
Image1->Canvas->FillRect(rct);
//Отрисовка
printhexagon(hexagon, Image1);
}
//---------------------------------------------------------------------------
void hexagoninit(MyPoint* h)
{ int i=0;
 h[0]=MyPoint(0,x, y);
 h[1]=MyPoint('A',0, rad);
 for(i=2; i<7; i++)
 {
 h[i]=MyPoint('A'+i-1,0,0);
 }
 }
