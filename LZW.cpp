#include <stdio.h>
#include <stdlib.h>
#include "LZW.h"

LZW_DATA lzw1,*lzw;
unsigned char decode_stack[TABLE_SIZE];           // ���ڱ����ѹ��������� 
/////////////      ѹ������         ///////////////// 
void compress(FILE *input,FILE *output)
{
	unsigned int curr_code;
	unsigned int suffix;                 // ��׺�ַ�
	unsigned int prefix;                 // ǰ׺�ַ�
	unsigned int index;
	int i;
	
	if(!(lzw1.code=(int*)malloc(TABLE_SIZE*sizeof(unsigned int))))
	{
		printf("�ڴ����ʧ�ܣ�\n");
		exit(0);
	}
	if(!(lzw1.prefix=(unsigned int*)malloc(TABLE_SIZE*sizeof(unsigned int))))
	{
		printf("�ڴ����ʧ�ܣ�\n");
		exit(0);
	}
	if(!(lzw1.suffix=(unsigned char*)malloc(TABLE_SIZE*sizeof(unsigned char))))
	{
		printf("�ڴ����ʧ�ܣ�\n");
		exit(0);
	}
	
	lzw=&lzw1;
	curr_code=258;                      // ������е��ַ�����258��ʼ��
	for(i=0;i<TABLE_SIZE;i++)           // ��ʼ��������� 
	    lzw->code[i]=-1; 
    i=0;
    printf("\n��ʼѹ��.");
    prefix=getc(input);                 // ���ļ��ж�ȡһ���ֽ�
	while((suffix=getc(input)) != (unsigned)EOF)
	{
		if(++i==1000)
		{
			i=0;
			printf(".");
		}
		index = hashsearch(prefix,suffix);   // ��ɢ�б��в��ң�������������
		if(lzw->code[index]!=-1)             // ���ñ�Ŵ��� 
		    prefix = lzw->code[index];       // ʹ�øñ����Ϊǰ׺ 
		else                                 // �������� 
		{
			if(curr_code<=(MAX_CODE))        // ���δ��������� 
			{
				lzw->code[index]=curr_code++; // ����һ����� 
				lzw->prefix[index]=prefix;    // ����ǰ׺ 
				lzw->suffix[index]=suffix;    // �����׺ 
			}
			outcode(output,prefix);           // ���ǰ׺�ַ����� 
			prefix=suffix;                    // ����׺��Ϊǰ׺��׼���´�ѭ�� 
		}
	}
	outcode(output,prefix);                   // ���ǰ׺ 
	outcode(output,(MAX_VALUE));              // ���������־ 
	outcode(output,0);
	
	free(lzw->code);
	free(lzw->prefix);
	free(lzw->suffix);
}


/////////////      ɢ�б���������         ///////////////// 
unsigned int hashsearch(int prefix,unsigned int suffix)
{
	int index;
	int offset;
	index=(suffix << (HASGING_SHIFT))^prefix;        // ����HASH��ַ
	if(index==0)
	    offset=-1;
	else
	    offset=TABLE_SIZE-index;
	while(1)
	{
		if(lzw->code[index]==-1)return (index);    // �ҵ�һ���ձ��� 
		if(lzw->prefix[index]==prefix && lzw->suffix[index]==suffix)  // �ҵ�Ŀ���ַ 
		    return (index);
        index-=offset;                            //  �����ͻ,����hash��ַ 
        if(index<0)
            index+=TABLE_SIZE;                     // ����HASH��ַ 
	}
}

/////////////      ���ѹ�����ַ�����         ///////////////// 
void outcode(FILE *output,unsigned int code)
{
	static int ob=0;                          // ��̬������������������ݵĶ�����λ�� 
	static unsigned long obb=0L;              // ��̬������������������ݵĶ�����
	obb |=(unsigned long)code <<(32-BITS-ob);    // ������λ�ϲ�
	ob+=BITS;
	while(ob>=8)                              // �ﵽһ���ֽ�(8λ)������� 
	{
		putc(obb>>24,output);                 // ����24λ��ʹ��8λΪ������� 
		obb<<=8;                              // ����8λ��ȥ���������һ���ֽ����� 
		ob-=8;                                // �������8λ������ʣ���δ�����λ�� 
	}
	return;
}


/////////////      ��ѹ������         ///////////////// 
void expand(FILE*input,FILE*output)
{
	unsigned int curr_code;
	unsigned int suffix;
	unsigned int prefix;
	int ch;
	int i;
	unsigned char *ps;
	unsigned char *decode(unsigned char *buffer ,unsigned int code);
	
	if(!(lzw1.code=(int*)malloc(TABLE_SIZE*sizeof(unsigned int))))
	{
		printf("�ڴ����ʧ�ܣ�\n");
		exit(0);
	}
	if(!(lzw1.prefix=(unsigned int*)malloc(TABLE_SIZE*sizeof(unsigned int))))
	{
		printf("�ڴ����ʧ�ܣ�\n");
		exit(0);
	}
	if(!(lzw1.suffix=(unsigned char*)malloc(TABLE_SIZE*sizeof(unsigned char))))
	{
		printf("�ڴ����ʧ�ܣ�\n");
		exit(0);
	}
	lzw=&lzw1;
	curr_code=258;
	i=0;
	printf("\n��ѹ��.");
	prefix = incode(input);
	ch=prefix;
	putc(prefix,output);
	while((suffix=incode(input))!=(MAX_VALUE))
	{
		if(++i==1000)
		{
		    i=0;
			printf(".");
		}
		if(suffix>=curr_code)
		{
			*decode_stack=ch;
			ps=decode(decode_stack+1,prefix);
		}
		else
		    ps=decode(decode_stack,suffix);
        ch=*ps;
        while(ps>=decode_stack)
            putc(*ps--,output);
        if(curr_code<=MAX_CODE)
        {
        	lzw->prefix[curr_code]=prefix;
        	lzw->suffix[curr_code]=ch;
        	curr_code++;
        }
        prefix=suffix;
	}
	free(lzw->code);
	free(lzw->prefix);
	free(lzw->suffix);
}


/////////////      ��ѹ���ļ��ж�ȡ����         ///////////////// 
unsigned int incode(FILE*input)
{
	unsigned int ret;
	static int ib=0;
	static unsigned long ibb=0L;
	while(ib<=24)
	{
		ibb|=(unsigned long)getc(input)<<(24-ib);
		ib+=8;
	}
	ret=ibb>>(32-BITS);
	ibb<<=BITS;
	ib-=BITS;
	return (ret);
}

/////////////       ���뺯��         ///////////////// 
unsigned char *decode(unsigned char*buffer,unsigned int code)
{
	int i=0;
	while(code>257)
	{
		*buffer++ =lzw->suffix[code];
		code=lzw->prefix[code];
		if(i++>=TABLE_SIZE)
		{
			printf("�ڴ����\n");
			exit(1);
		}
	}
	*buffer=code;
	return (buffer);
}