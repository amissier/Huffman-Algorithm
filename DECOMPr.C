#include<stdio.h>
#include<conio.h>
#include<alloc.h>
#include<string.h>

struct File_Info
{
int Min_BitSize;
char *FName;
long FSize;
}Fhead;

int Min_BitSize;
struct table
{
unsigned char ch;
long int count;
int nbits;
unsigned int bvalue;
} Thead[255];

FILE *Fp,*Fp1;
void Decompress(unsigned char);

void main()
{
	unsigned char c;
	clrscr();
	if((Fp=fopen("Write.txt","r"))==NULL)
		{
			printf("Error in file open");
			exit(0);
		}

	if((Fp1=fopen("File1.txt","w"))==NULL)
		{
			printf("Error in file open");
			exit(0);
		}
	fread(&Fhead,sizeof(Fhead),1,Fp);
	fread(&Thead,sizeof(Thead),1,Fp);
	while((c=fgetc(Fp))!=EOF)
	      Decompress(c);
	fcloseall();
}

void Decompress(unsigned char Dstr)
{
	int Compval,i,k;
	static unsigned char Rchar='\0',Rchar1='\0';
	int flag;
	static long count=0;
	static int j;
	Compval=128;

	for(k=0;k<8;k++)
	{
		if (k==0) Rchar=Rchar1<<1;
		Rchar |=(Dstr & Compval) ? 1:0;
		Compval >>=1;
		j++;
		if (j>=Min_BitSize)
		 {
			Rchar1=Rchar;
			Rchar <<=(8-j);
			for(i=0;i<255;i++)
				{
					if(j==Thead[i].nbits && Rchar== Thead[i].bvalue)
						{
							if (count >= Fhead.FSize) exit(0);
							printf("%c", Thead[i].ch);
							fputc(Thead[i].ch,Fp1);
							Rchar='\0'; flag=1;
							count++;
							j=0;
							break;
						}
				}
		       if(flag==1)
			       Rchar >>=(8-j);
		       else
			       Rchar=Rchar1;
		   }
		  Rchar<<=1;
	}
	Rchar1=Rchar>>1;
}