#include <stdio.h>
#ifndef LZW_STRUCT_H
#define LZW_STRUCT_H
#define BITS 12
#define HASGING_SHIFT BITS-8                      // HASH ����λ�� 
#define MAX_VALUE (1<<BITS)-1 
#define MAX_CODE MAX_VALUE-1                      // ����� 
#define TABLE_SIZE 4099                           // ɢ�б�ĳ��� 
typedef struct
{
	int *code;                                    // ��ʶ 
	unsigned int *prefix;                         // ǰ׺ 
	unsigned char *suffix;                        // ��׺ 
}LZW_DATA;
//unsigned char decode_stack[TABLE_SIZE];           // ���ڱ����ѹ��������� 
//LZW_DATA lzw1,*lzw;

void compress(FILE *input, FILE *output);         // ѹ������
unsigned int hashsearch(int prefix,unsigned int suffix);    // ɢ�б���������
void outcode(FILE *output,unsigned int code);     // ���ѹ������ֽ�����


void expand(FILE *input,FILE *output);             // ��ѹ������ 
unsigned int incode(FILE *input);                  // ��ѹ���ļ��ж�ȡ����
unsigned char *decode(unsigned char *buffer,unsigned int code);     // ���뺯�� 
#endif