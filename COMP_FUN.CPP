

   	



/* Decompress Function */

void FileCompress::Do_Decompress(unsigned char *DStr)
{
	int j=0;
	int Compval;
	int i,k,flag;
	static unsigned char Rchar='\0';
	while(*DStr !='\0')
	{
		Compval=128;
		for(k=0;k<8;k++)
		{
			Rchar|=(*DStr & Compval) ? 1:0;	// Get the decompressed character from the compressed file
			Compval>>=1;
			j++;
			flag=1;
			if (j >=Min_BitSize)	// Check the bit size with Minimum bit size
			{
				Rchar <<=(8-j);
				for(i=0;i<255;i++)
				{
					if(j==Thead[i].Nbits && Rchar==Thead[i].Bvalue)		// Check the bit size and shift value with table datas
					{
						cout << Thead[i].Data ;
						Rchar ='\0';
						j=0; flag=0;
						break;
					}
				}
				if(flag==1)
					Rchar >>=(8-j);
			}
			Rchar <<=1;
		}
		*DStr++;
	}
}
