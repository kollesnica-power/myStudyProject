#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <math.h>
#include <locale.h>
#include <windows.h>

int main(void)
{
  setlocale(LC_ALL,"Russian");
  system("color F0");
  int n;
  do{
        printf("Введите количество вершин [1;10] ");
        scanf("%d",&n);
    }while(n<1 || n>10);
  int a[n][n];
    for(int i=0; i<n;i++)
    {
        for(int j=0;j<n;j++)
        {
          a[i][j]=0;
        }
    }
  char ans;
  int i,j;
  do{
    do{
        printf("Введите номера вершин, между которыми есть связь ");
        scanf("%d%d",&i,&j);
        if(i<1 || i>n || j<1 || j>n)printf("Вы ввели неверные данные\n");
      }while(i<1 || i>n || j<1 || j>n);
    a[i-1][j-1]=1;
    printf("Продолжить ввод? (y/n)");
    ans=getch();
    printf("\n");
  }while((ans=='y') || (ans==13));
  //system("cls");
  /*a[0][1]=1;
  a[0][2]=1;
  a[1][4]=1;
  a[2][3]=1;
  a[2][5]=1;
  a[3][4]=1;
  a[3][5]=1;
  a[3][6]=1;
  a[4][6]=1;
  a[4][7]=1;
  a[5][6]=1;
  a[5][8]=1;
  a[6][7]=1;
  a[6][8]=1;
  a[6][9]=1;
  a[7][9]=1;
  a[8][9]=1;*/
  printf("Матрица смежности:\n     ");
  for(int i=0;i<n;i++)
  {
      printf("x%d ",i+1);
  }
  printf("\n");
  for(int i=0; i<n-1;i++)
  {
    printf("x%d ",i+1);
    for(int j=0;j<n;j++)
    {
        printf("%3d",a[i][j]);
    }
    printf("\n");
  }
  printf("x%d",n);
  for(int i=0;i<n; i++)
  {
    printf("%3d",a[n-1][i]);
  }
  int SS[n][n];
  int BS[n][n];//Прямое соответствие/обратное соответствие
  for(int i=0; i<n;i++)
  {
    for(int j=0;j<n;j++)
    {
      SS[i][j]=0;
      BS[i][j]=0;
    }
  }
  int k;
  for(int i=0;i<n;i++)
  {
    k=0;
    for(int j=0;j<n;j++)
    {
        if(a[i][j])
        {
            SS[i][k]=j+1;
            k++;
        }

    }
  }
printf("\nПрямое соответствие:\n");
  for (int i=0;i<n;i++)
  {
      printf("D(x%d)=",i+1);
      if(i==n-1)printf("%d",0);
      for(int j=0;j<n;j++)
      {
          if(SS[i][j])
          {
              printf("%d ",SS[i][j]);
          }
      }
      printf("\n");
  }
  for(int i=0;i<n;i++)
  {
    k=0;
    for(int j=0;j<n;j++)
    {
        if(a[j][i])
        {
            BS[i][k]=j+1;
            k++;
        }
    }
  }
  printf("\nОбратное соответствие:\n");
  for (int i=0;i<n;i++)
  {
      printf("D(x%d)=",i+1);
      if(i==0)printf("%d",0);
      for(int j=0;j<n;j++)
      {
          if(BS[i][j])
          {
              printf("%d ",BS[i][j]);
          }
      }
      printf("\n");
  }
    printf("\nПолустепени захода:\n");
  //Полустепени захода и исхода
  int hi[n],ho[n];
  for(int i=0;i<n;i++)
  {
      hi[i]=0;
      for(int j=0;j<n;j++)
      {
          if(a[j][i]) hi[i]++;
      }
  }
  for(int i=0;i<n;i++)
  {
      ho[i]=0;
      for(int j=0;j<n;j++)
      {
          if(a[i][j]) ho[i]++;
      }
  }
  for(int i=0;i<n;i++)
  {
      printf("d0(X%d)=%d\n",i+1,hi[i]);
  }
  printf("\nПолустепени исхода:\n");
  for(int i=0;i<n;i++)
  {
      printf("dt(X%d)=%d\n",i+1,ho[i]);
  }
  return 0;
}
