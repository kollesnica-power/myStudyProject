#include <stdio.h>
#include <iostream>
#include <windows.h>
#include <conio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

using namespace std;

typedef struct bin
{
	int x, l, r;
}cell;
typedef int int1;
int n,m,zap=0,zay=0;

//Vanek Deboshir's part
void firstplan(int bj[10], int ai[10], cell ab[10][10])
{
	int Bj[10], Ai[10];
	for (int i = 0; i <= n-1; i++)
	{
		Ai[i] = ai[i];
	}
	for(int j=0;j<m;j++)
    {
        Bj[j] = bj[j];
    }
	int i = 0, j = 0;
	while (Ai[n-1] != 0 && Bj[m-1] != 0)
	{
		if (Ai[i] < Bj[j])
		{
			ab[i][j].x = Ai[i];
			Bj[j] = Bj[j] - Ai[i];
			Ai[i] = 0;
			i++;
		}
		else
		{
			ab[i][j].x = Bj[j];
			Ai[i] = Ai[i] - Bj[j];
			Bj[j] = 0;
			j++;
		}
	}
}
void output(cell AB[10][10], int Ai[10], int Bj[10], int Al[10], int Be[10])
{
	printf("+---------+");
	for (int j = 0; j <= m - 1; j++)
	{
		printf("--------+");
	}
	printf("--------+---------+\n");
	printf("|   \\     |");
	for (int j = 0; j <= m - 1; j++)
	{
		printf("        |");
	}
	printf("        |         |\n");
	printf("|    \\ Pn |");
	for (int j = 0; j <= m - 1; j++)
	{
		printf("   B%d   |", j + 1);
	}
	printf("   Ai   |  Alfai  |\n");
	printf("|  Po \\   |");
	for (int j = 0; j <= m - 1; j++)
	{
		printf("        |");
	}
	printf("        |         |\n");
	printf("+---------+");
	for (int j = 0; j <= m - 1; j++)
	{
		printf("--------+");
	}
	printf("--------+---------+\n");

	for (int i = 0; i <= n - 1; i++)
	{
		printf("|  A%d     ", i + 1);
		for (int j = 0; j <= m - 1; j++)
		{
			if (AB[i][j].x >= 0)
			{
				printf("|     %3d", AB[i][j].r);
			}
			else
			{
				printf("|%3d  %3d", AB[i][j].l, AB[i][j].r);
			}
		}
		printf("|        |         |\n");
		printf("|         ");
		for (int j = 0; j <= m - 1; j++)
		{
			if (AB[i][j].x == -1)
			{
				if (AB[i][j].l > AB[i][j].r)
				{
					printf("|    X   ");
				}
				else
				{
					printf("|        ");
				}
			}
			else
			{
				printf("|  %3d   ", AB[i][j].x);
			}
		}
		printf("|  %3d   |   %3d   |\n", Ai[i], Al[i]);
		printf("+---------+");
		for (int j = 0; j <= m - 1; j++)
		{
			printf("--------+");
		}
		printf("--------+---------+\n");
	}
	printf("|  Bj     ");
	int SumBjOrAi = 0;
	for (int j = 0; j <= m - 1; j++)
	{
		SumBjOrAi = SumBjOrAi + Bj[j];
		printf("|  %3d   ", Bj[j]);
	}

	printf("|  %3d   |         |\n", SumBjOrAi);
	printf("+---------+");
	for (int j = 0; j <= m - 1; j++)
	{
		printf("--------+");
	}
	printf("--------+---------+\n");
	printf("|  Betaj  ");

	for (int j = 0; j <= m - 1; j++)
	{
		printf("|  %3d   ", Be[j]);
	}
	printf("|        |         |\n");
	printf("+---------+");
	for (int j = 0; j <= m - 1; j++)
	{
		printf("--------+");
	}
	printf("--------+---------+\n");
}

void input(int Bj[10], int Ai[10], cell AB[10][10], int AL[10], int BE[10])
{
	/*Bj[0] = 40; Ai[0] = 60;
	Bj[1] = 44; Ai[1] = 48;
	Bj[2] = 28; Ai[2] = 12;
	Bj[3] = 32; Ai[3] = 36;
	Bj[4] = 36; Ai[4] = 24;
	AB[0][0].r = 5; AB[0][1].r = 1; AB[0][2].r = 2; AB[0][3].r = 3; AB[0][4].r = 4;
	AB[1][0].r = 4; AB[1][1].r = 5; AB[1][2].r = 1; AB[1][3].r = 2; AB[1][4].r = 3;
	AB[2][0].r = 3; AB[2][1].r = 4; AB[2][2].r = 5; AB[2][3].r = 1; AB[2][4].r = 2;
	AB[3][0].r = 2; AB[3][1].r = 3; AB[3][2].r = 4; AB[3][3].r = 5; AB[3][4].r = 1;
	AB[4][0].r = 1; AB[4][1].r = 2; AB[4][2].r = 3; AB[4][3].r = 4; AB[4][4].r = 5;*/
    int er;

	for (int i = 0; i <= m-1; i++)
	{
	    er=0;
		do
        {
        if (er==1) cout<<"'B' cannot be negative or bigger than 500! Please retry ";
		cout << "Enter B[" << i+1 << "]: ";
		cin >> Bj[i];
		er=1;
		} while (Bj[i]<0 || Bj[i]>500);
		zay = zay + Bj[i];

	}
	for (int i = 0; i <= n-1; i++)
	{
	    er=0;
		do
        {
        if (er==1) cout<<"'A' cannot be negative or bigger than 500! Please retry ";
		cout << "Enter A[" << i+1 << "]: ";
		cin >> Ai[i];
		er=1;
		} while (Ai[i]<0 || Ai[i]>500);
		zap = zap + Ai[i];
	}
	for (int i = 0; i <= n-1; i++)
	{
		for (int j = 0; j <= m-1; j++)
		{
			er=0;
            do
            {
            if (er==1) cout<<"'c' cannot be less than 1 or bigger than 99! Please retry ";
			cout << "Enter c[" << i+1 << "," << j+1 << "]: ";
			cin >> AB[i][j].r;
			er=1;
            } while (AB[i][j].r<1 || AB[i][j].r>99);
		}
	}
	for (int i = 0; i <= n-1; i++)
	{
		AL[i] = 0;
		for (int j = 0; j <= m-1; j++)
		{
			AB[i][j].x = -1;
			AB[i][j].l = 0;

		}
	}
	for (int j = 0; j <= m - 1; j++)
	{
		BE[j] = 0;
	}
}
//Vader's part
void potencia(cell ab[10][10], int al[10], int be[10])
{
	for (int i = 0/*row*/; i <= n - 1; i++)
	{
		for (int j = 0/*column*/; j <= m - 1; j++)
		{
			if (ab[i][j].x == (-1))
			{
				ab[i][j].l = al[i] + be[j];
			}
		}
	}
}
int check(int al[10],int be[10])
{
    for(int i=0;i<n;i++)
    {
        if((al[i]=='z')) return 1;
    }
    for (int j=0;j<m;j++)
    {
        if(be[j]=='z') return 1;
    }
    return 0;
}
void prosto(cell ab[10][10], int al[10], int be[10])
{
    for(int i=0;i<m;i++)
    {
        be[i]='z';
    }
    for (int i = 0; i <= n-1; i++)
    {
        al[i] = 'z';
        for (int j = 0; j <= m-1; j++)
        {
                ab[i][j].l = 0;
        }
    }
    be[0]=0;
    do{
        for (int i = 0/*row*/; i <= n - 1; i++)
        {
            for (int j = 0/*column*/; j <= m - 1; j++)
            {
                if ((ab[i][j].x>(-1)) && (al[i] == 'z')&&(be[j])!='z')
                {
                    al[i] = ab[i][j].r - be[j];
                }
                if ((ab[i][j].x>(-1))&&(al[i] != 'z'))
                {
                    be[j] = ab[i][j].r - al[i];
                }
            }
        }
    }while(check(al,be));
}
//Nafanailol's part
void circle(cell ab[10][10],int way[2][20],int &w)
{
 cell DA[10][10];
 for (int i=0;i<=n-1;i++)
 {
        for (int j=0;j<=m-1;j++)
        {
            DA[i][j].x=ab[i][j].x;
            DA[i][j].l=ab[i][j].l;
            DA[i][j].r=ab[i][j].r;
        }
 }
    int maximum=0;
    int maximumi;
    int maximumj;
    int temp;
    for (int i=0;i<=n-1;i++)
    {
        for (int j=0;j<=m-1;j++)
        { temp=DA[i][j].l-DA[i][j].r;
            if (temp>maximum)
            {
            maximum=DA[i][j].l-DA[i][j].r; maximumi=i; maximumj=j;
            }
        }
    }

    int h=0;
    int Pr=0;
    int ti,tj;
    ti=maximumi;
    tj=maximumj;
    int pn;
    short int tpj,tpi;
    w=0;
    way[0][w]=ti;
    way[1][w]=tj;
    w++;
    while (h==0)
    {tpi=ti;
     tpj=tj;

        if (Pr==0)
        {   pn=Pr;
            for (int i=0;i<=n-1;i++)
            {
                if (i!=tpi && DA[i][tj].x>-1) { ti=i;  Pr=1; }
            }
        }
        else
        {
            pn=Pr;
            for (int j=0;j<=m-1;j++)
            {
                if (j!=tpj && DA[ti][j].x>-1) { tj=j; Pr=0; }
            }
        }
            if (pn==Pr)
            {
                DA[ti][tj].x=-1;
                ti=maximumi;
                tj=maximumj;
                Pr=0;
                w=1;
            }
            else
            {
                way[0][w]=ti;
                way[1][w]=tj;
                w++;
            }
     if (ti==maximumi && w>2) h=1;
    }
    return;
}
//kollesnica power's part
int minArray(int a[20],int l)//Возвращает минимум массива
{
    int min=a[0];
    for (int i=1;i<l/2;i++)
    {
       if (min>a[i])min=a[i];
    }
    return min;
}
void outZ(cell AB[10][10], int &z)
{
    for(int i=0;i<n;i++)
    {
        for(int j=0;j<m;j++)
        {
        if (AB[i][j].x>0) z+=AB[i][j].x*AB[i][j].r;
        }
    }
    cout<<"Z="<<z<<endl;
}
void newPlan(int w[2][20],int l,cell AB [10][10])//zExp - Ожидаемый план
{
    int kMin,gamma,z=0,zExp=0;
    int k[20];
    int j=0;
    for(int i=1;i<l;i+=2)
    {
            k[j]=AB[w[0][i]][w[1][i]].x;
            j++;
    }
    kMin=minArray(k,l);
    gamma=AB[w[0][0]][w[1][0]].r-AB[w[0][0]][w[1][0]].l;
    cout<<"k="<<kMin<<endl;
    cout<<"Gamma="<<gamma<<endl;
    outZ(AB,z);
    zExp=z+kMin*gamma;
    cout<<"Zexp="<<zExp<<endl;
    int zn=1;//Меняет знак
    for(int i=0;i<l;i++)
    {
        AB[w[0][i]][w[1][i]].x+=zn*kMin;
        zn*=-1;
    }
    AB[w[0][0]][w[1][0]].x++;
	int zMin[2];
	zMin[0]=n;
	zMin[1]=m;
	for(int i=0;i<l;i++)
	{
        if(AB[w[0][i]][w[1][i]].x==0)
        {
            if((w[0][i]<zMin[0])&&(w[1][i]<zMin[1]))
            {
                if ((w[0][i]!=w[0][0])||(w[1][i]!=w[1][0]))
                {
                    zMin[0]=w[0][i];
                    zMin[1]=w[1][i];
                }
            }
        }
    }
    AB[zMin[0]][zMin[1]].x--;

}
int con(cell AB [10][10])
{
    for(int i=0;i<n;i++)
    {
        for(int j=0;j<m;j++)
        {
           if (AB[i][j].l>AB[i][j].r) return 1;
        }
    }
    return 0;
}
//Gaben1488's part
void disbalance(int Bj[10], int Ai[10], cell AB[10][10], int AL[10], int BE[10])
{
	int a;
	if (zap > zay)
	{
		m++;
		a = zap - zay;
		int j = m - 1;
		Bj[j] = a;
		for (int i = 0; i <= n - 1; i++)
		{
			AB[i][j].r = 0;
			AB[i][j].l = 0;
			AB[i][j].x = -1;
		}
	}
	else
	{
		n++;
		a = zay - zap;
		int i = n - 1;
		Ai[i] = a;
		for (int j = 0; j <= m - 1; j++)
		{
			AB[i][j].r = 0;
			AB[i][j].l = 0;
			AB[i][j].x = -1;
		}
	}
}
int main()
{
	system("color F0");
	int Bj[10], Ai[10], AL[10], BE[10];
	cell AB[10][10];
	int1 WAY[2][20];
    int er=0,kol,z=0;
    do
    {
        if (er==1) cout<<"ERROR: n and m must be in - [1;10], retry please"<<endl;
        cout << "Enter n: ";
        cin >> n;
        cout << "Enter m: ";
        cin >> m;
        er=1;
    } while (n<0 || n>10 || m<0 || m>10);
	input(Bj,Ai,AB,AL,BE);
	if (zap!=zay) disbalance(Bj, Ai, AB, AL, BE);
	firstplan(Bj, Ai, AB);
	prosto(AB, AL, BE);
	potencia(AB, AL, BE);
	output(AB, Ai, Bj, AL, BE);
    while(con(AB))
    {
        circle(AB,WAY,kol);
        newPlan(WAY,kol,AB);
        prosto(AB, AL, BE);
        potencia(AB, AL, BE);
        output(AB, Ai, Bj, AL, BE);
      }
    outZ(AB,z);
    cout<<"Plan is optimal";
    return 0;
}
