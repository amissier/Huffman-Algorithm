#include<stdio.h>
#include<conio.h>
#include<alloc.h>
#include<string.h>
int min_bitsize = 8;
/*unsigned char compstr[100];*/
int count;
struct  list
{
unsigned char ch;
unsigned int count;
struct list *l;
struct list *r;
struct list *next;
};
struct table
{
unsigned char ch;
long int count;
int nbits;
unsigned int bvalue;
};
struct File_Info
{
int  Min_BitSize;
char *Fname;
long FSize;
}Fhead;

struct table tab[255];
typedef struct list node;
node* newnode(unsigned char,unsigned int);
node* insertlist(node*);
void displist(void);
void huffmantree(node*,int);
node* inserttree(void);
void disptable(void);
void compress();
void do_compress(char,int,int);
node *head=NULL;
FILE *Fp, *Fp1;
char Gstr;
void main()
{

	node *temp,*h;
	long int an[255];
	char ch,c1;
	int i;

	if((Fp=fopen("Dcomp.txt","r"))==NULL)
		{
			printf("Error in file open");
			exit(0);
		}
	clrscr();
	for(i = 0;i<255;i++)
		an[i] = 0;
/*	p=str;*/
	while ((ch=fgetc(Fp))!=EOF)
	{
	    count++;
	    an[ch]++;
	}
	fclose(Fp);
	for(i=0;i<255;i++)
	{
		if(an[i] != 0)
		{
			c1 =(char)i;
			printf("%c",c1);
			printf("->");
			printf("%d",an[i]);
			temp = newnode(c1,an[i]);
			h = insertlist(temp);
		}
	}

	printf("List contents\n");
	displist();
	printf("Tree constructioxn\n");
	while(head->next != NULL)
	{
		h = inserttree();
		head = head->next->next;
		h = insertlist(h);
		displist();
		printf("\n");
	}
	printf("\ndisplay from tree\n");
	huffmantree(head,0);
/*	while(head!=NULL)
	{
		head=head->next;
		free(head);
	}
  */	printf("\ndisplay from table\n");
	disptable();
	compress();
}
node *newnode(unsigned char data,unsigned int occurance)
{

	node *temp;
	temp = (node*)malloc(sizeof(node));
	temp->ch = data;
	temp->count = occurance;
	temp->l=NULL;
	temp->r=NULL;
	return(temp);
}

node *insertlist(node *temp)
{
	node *duphead;
	duphead = head;
	if(head == NULL || head->count> temp->count)
	{
		head = temp;
		head->next = duphead;
		return head;
	}
	else
	{
		while(duphead !=  NULL)
		{
			if(duphead->count <= temp->count && (duphead->next ==  NULL || duphead->next->count > temp->count))
			{
				temp->next = duphead->next;
				duphead->next = temp;
				return head;
			}
			duphead = duphead->next;
		}
		duphead->next = temp;
	}
	return head;
}

void displist()
{
	node *duphead;
	duphead = head;
	if(duphead==NULL)
	{
		printf("no elements");
	}
	else
	{
		while(duphead != NULL)
		{
			printf("\nch=%c\tcount=%d",duphead->ch,duphead->count);
			duphead = duphead->next;
		}
	}


}

node *inserttree()
{
	node *temp,*duphead;
	duphead = head;
	temp = (node*)malloc(sizeof(node));
	temp->ch = '~';
	temp->count = duphead->count + duphead->next->count;
	temp->l = (node*)malloc(sizeof(node));
	temp->l = duphead;
	temp->r = (node*)malloc(sizeof(node));
	temp->r = duphead->next;
	return temp;
}
void huffmantree (node *T,int Direction)
{
	static int nbits=1;				/* To count no of bits*/
	static int Val=0;
	int shiftVal=0;				/* To get the unique value*/
	if (T !=NULL)
	{
		Val |= Direction;
		if (T->l ==NULL && T->r ==NULL)    /* We need only leaf*/
		{
			shiftVal=Val<<(8-nbits);
			printf("\n%c\t%d\t%d\t%d\t%d\n",T->ch,T->count,nbits,Val,shiftVal);
			if(nbits < min_bitsize)
				min_bitsize=nbits;		/* To get Minimum bit size*/
			tab[T->ch].ch =  T->ch;
			tab[T->ch].count =T->count ;
			tab[T->ch].nbits =nbits;
			tab[T->ch].bvalue =shiftVal;
			/*printf("\t%c\t%ld\t%d\t%d\n",tab[T->ch].ch,tab[T->ch].count,tab[T->ch].nbits,tab[T->ch].bvalue);*/
		}
		nbits++;			/* Increase the no of bit count when traversing left side      */
		Val<<=1;
		huffmantree(T->l,0);
		nbits++;			/* Increase the no of bit count when traversing right side*/
		Val<<=1;
		huffmantree(T->r,1);
	}
	nbits--;				/* Decrease the no of bit count when back track*/
	Val>>=1;
}


void disptable()
{
int i=0;
for(i=0;i<255;i++)
if(tab[i].count > 0)
printf("\t%c\t%ld\t%d\t%d\n",tab[i].ch,tab[i].count,tab[i].nbits,tab[i].bvalue);
}

void compress()
{
char i,c;

if((Fp= fopen("Dcomp.txt","r"))==NULL)
	{
	printf("Error in file open");
	exit(0);
	}
if((Fp1=fopen("Write.txt","w"))==NULL)
	{
	printf("Error in file open");
	exit(0);
	}
Fhead.Min_BitSize=min_bitsize;
strcpy(Fhead.Fname,"File.txt");
Fhead.FSize=count;
/*fputc(min_bitsize,Fp1);*/
fwrite(&Fhead,sizeof(Fhead),1,Fp1);
fwrite(&tab,sizeof(tab),1,Fp1);

while((c=fgetc(Fp))!=EOF)
{
do_compress(c,tab[c].nbits,tab[c].bvalue);
}
if (c==EOF){Gstr=c; do_compress(c,1,0);}
fcloseall();

}

void do_compress(char str,int nbits,int shiftval)
{
	static int k;
	int compval=128;
	int j,flag;
	static unsigned char wchar='\0',wchar1='\0';
	for(;k<8;)
	{
		for(j=0;j<nbits;j++)
		{
			if(Gstr == EOF)
			{
			wchar=wchar1;
			while(k < 8)
				{
					wchar<<=1;
					compval>>=1;
					k++;
				}
/*				printf("%c",wchar);*/
				fputc(wchar,Fp1);
			}
			else
			{
			wchar |=(shiftval & compval)?1:0;
			compval>>=1;
			k++;
			if(k==8)
			{
  /*				printf("%c",wchar);*/
				fputc(wchar,Fp1);
				wchar = '\0';
				k=0;
			      /*compval = 128;*/
				wchar<<=1;
			}
			wchar1=wchar;
			wchar<<=1;
			}
		}
			break;
	}
}



