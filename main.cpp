#include "LZW.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
int main(int argc,char *argv[])
{
	FILE *fp1,*fp2;
	int flag=0;
	char *op;
	if(argc!=4)flag=1;
	else
	{
		op=argv[1];
		if((strcmp(op,"-z")!=0) && strcmp(op,"-e")!=0)flag=1;
	}
	if(flag)
	{
		printf("ʹ�÷�����command -z/-e source dest\n");
		exit(1);
	}
	if((fp1=fopen(argv[2],"rb"))==NULL)
	{
		printf("���ܴ�Դ�ļ�������\n");
		exit(1);
	}
	if((fp2=fopen(argv[3],"wb"))==NULL)
	{
		printf("���ܴ���Ŀ���ļ�������\n");
		exit(1);
	}
	if(strcmp(op,"-z")==0)
		compress(fp1,fp2);     // ����ѹ������ 
	else
		expand(fp1,fp2);       // ��ѹ������
		
    fclose(fp1);
    fclose(fp2);
    return 0;
}