#include "utils.h"
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
/*void loge(std::string lg , std::string ms ){
	std::cout << "[" << lg << "] " << ms << ".\n";
	return;
}
void Eloge(std::string ms){
	 std::cerr << "[ERORR] " << ms << ".\n";
}
void Wloge(std::string ms){
	 std::cerr << "[WARNING] " << ms << ".\n";
}

void readFile(std::string path, std::string& data){
	std::ifstream fl(path);
	if(!fl.is_open()){
		Eloge("At opening " + path + " file disnt exiset or curupted");
		return;
	}
	fl.seekg(0,fl.end);
	
	unsigned int u_fileSize = fl.tellg();
	data.resize(u_fileSize);
	
	fl.seekg(0);
	fl.read(data.data(), u_fileSize);
	fl.close();
		
	return;
}
*/

void loge (const char* lg ,const char* ms ,...)
{
	printf("[%s] ",lg);
	va_list args;
	va_start(args, ms);
	vprintf(ms, args);
	va_end(args);
	printf(".\n");

}
void Eloge(const char* ms , ...)
{
	fprintf(stderr,"[ERORR] ");
	va_list args;
	va_start(args, ms);
	vfprintf(stderr,ms, args);
	va_end(args);
	fprintf(stderr,".\n");
}
void Wloge(const char* ms , ...){
	fprintf(stderr,"[WARNING] ");
	va_list args;
	va_start(args, ms);
	vfprintf(stderr,ms, args);
	va_end(args);
	fprintf(stderr,".\n");

}

unsigned long GetFileSize(FILE* fl){
	if(fl==NULL){
		printf("err in file\n");
		return -1;
	}
	long prev=ftell(fl);
	if(fseek(fl, 0L, SEEK_END)==-1){
		printf("files to fseek");
		return -1;
	}
	unsigned long fileSize = ftell(fl);
	fseek(fl, prev, SEEK_SET);
	return fileSize;

}


void* readFile(const char* fileName , unsigned long * sizeOUT){
	FILE* file = fopen(fileName, "r");
	if(file==NULL){
		printf("err in opening file\n");
		return NULL;
	}
	unsigned long fileSz = GetFileSize(file);
	*sizeOUT=fileSz;
	
	if(fileSz!=-1){
		char* data = (char*)malloc(fileSz);
		fread(data, fileSz, 1, file);
		
		
		fclose(file);
		return data;
	}

	fclose(file);
	return NULL;
}






void tRotate(float* x,float* y,float xx,float yy,float theta){
	float raduis=(theta*3.14159)/(180.0);
	float X = *x-xx;
	float Y = *y-yy;
	*x=xx+X*cos(raduis)-Y*sin(raduis);
	*y=yy+X*sin(raduis)+Y*cos(raduis);
	return ;
}

char ReadBitmap(void* bm,u32 num){
	return ((u8*)bm)[num/8]&(1<<(num%8));
}
void SetBitmap(void* bm,u32 num,char val){
	((u8*)bm)[num/8]&=~(1<<(num%8));
	((u8*)bm)[num/8]|=((val&1)<<(num%8));
	return;
}



u32 GetUnicode8Size(char* text,u32 size){
	u32 ret=0;
	for(u32 i = 0 ; i<=size;i++){
		if((((u8)text[i])&(3<<5)) == 1<<6){//10xxxxxxx, a continue
			continue;
		}
		ret++;
	}
	return ret;
}

u32 GetUnicode8(char *text, u32 number,char val){
	u32 unicode=0;
	for(u32 i = 0, uniN=0  ; uniN<=number; i++){
		if(!text[i])return 32;
		if(!(text[i]&(1<<7))){
			unicode=text[i];
			uniN++;
		}else if((text[i]&(3<<6))==1<<7){
			unicode<<=6;
			unicode|=text[i]&0x3f;;	
		}else if ((text[i]&(3<<6))==3<<6 ){
			unicode=0;
			
			uniN++ ;
			int leftshift=6;
			for(int ii = 6 ; ii  >= 0 ;  ii--,leftshift--){
				if(!(text[i]&(1<<ii)))break;
			}
			unicode|=text[i]&~(0xff<<leftshift);
		}
	}
	return unicode;
}


u64 max(u64 v1 , u64 v2){
	return v1>v2?v1:v2;
}


u64 min(u64 v1 , u64 v2){
	return v1>v2?v2:v1;
}



size_t minarg(u32 count ,...){
	va_list args;

	va_start(args,count);
	size_t ret = -1;
	
	size_t arg__;
	for(u32 i = 0 ; i < count ; i++){
		arg__=va_arg(args, size_t);
		ret=arg__<ret?arg__:ret;
	}

	va_end(args);	
	return ret;
}




void* PushBuffer(void* val , u32 sizeofstr ,size_t pos,  size_t* dataSize , size_t* usedData , void* data){
	
	if(((*usedData)+sizeofstr) >= *dataSize){
		size_t newDataSize = *usedData+sizeofstr+0x1000;
		data=realloc(data,newDataSize);
		*dataSize=newDataSize;
	}
	if(pos>=*dataSize){
		size_t newDataSize = pos+sizeofstr+0x1000;
		data=realloc(data, newDataSize);
		*dataSize=newDataSize;
	}
	for(u32 i = *usedData+sizeofstr; i>pos+sizeofstr; i--){
		((u8*)data)[i-1] = ((u8*)data)[i-sizeofstr-1];
	}
	
	for(u32 i = 0 ; i < sizeofstr ; i++){
		((u8*)data)[i+pos]=((u8*)val)[i];
	}
	*usedData+=sizeofstr;
	return data;
}
void* PushChar   (char val  ,size_t pos,  size_t* dataSize , size_t* usedData , void* data){
	void *ret= PushBuffer(&val , (u32)sizeof(typeof(val)) , pos*sizeof(typeof(val)),  dataSize , usedData , data);
	return ret;
}
void* PushShort  (short val  ,size_t pos,  size_t* dataSize , size_t* usedData , void* data){
	return  PushBuffer(&val , (u32)sizeof(typeof(val)) , pos*sizeof(typeof(val)),  dataSize , usedData , data); //intristing stuff
}
void* PushInteger(int val  ,size_t pos,  size_t* dataSize , size_t* usedData , void* data){
	return  PushBuffer(&val , (u32)sizeof(typeof(val)) , pos*sizeof(typeof(val)),  dataSize , usedData , data);
}
void* PushFloat  (float val  ,size_t pos,  size_t* dataSize , size_t* usedData , void* data){
	return  PushBuffer(&val , (u32)sizeof(typeof(val)) , pos*sizeof(typeof(val)),  dataSize , usedData , data);
}
void* PushLong   (long val  ,size_t pos,  size_t* dataSize , size_t* usedData , void* data){
	return  PushBuffer(&val , (u32)sizeof(typeof(val)) , pos*sizeof(typeof(val)),  dataSize , usedData , data);
}
void* PushDouble (double val  ,size_t pos,  size_t* dataSize , size_t* usedData , void* data){
	return  PushBuffer(&val , (u32)sizeof(typeof(val)) , pos*sizeof(typeof(val)),  dataSize , usedData , data);
}


void PopBuffer(size_t pos, u32 size,  size_t* dataSize ,void* data){
	if(size>*dataSize ){
		printf("[**ERORR**] poped size biger then the buffer size");
		return;
	}
	if(pos+size>*dataSize ){
		printf("[**ERORR**] poped pos over buffer size");
		return;
	}
	for(size_t i = pos ; i+size<*dataSize ; i++){
		*(u8*)((size_t)data+i) = *(u8*)((size_t)data+i+size);
	}
	for(u32 i = 0 ; i < size ; i++){
		((u8*)data)[*dataSize-i]=0;
	}
	*dataSize-=size;
	return ;
}


char PopChar   (size_t pos,  size_t* dataSize ,  void* data){
	char ret = *(char*)((size_t)data+pos*sizeof(char));
	PopBuffer(pos , (u32)sizeof(char) ,  dataSize ,  data);
	return ret;
}
short PopShort  (size_t pos,  size_t* dataSize ,  void* data){
	short ret = *(short*)((size_t)data+pos*sizeof(short));
	PopBuffer(pos , (u32)sizeof(short) ,   dataSize ,  data); //intristing stuff
	return ret;
}
int PopInteger(size_t pos,  size_t* dataSize ,  void* data){
	int ret = *(int*)((size_t)data+pos*sizeof(int));
	PopBuffer(pos , (u32)sizeof(int) ,  dataSize ,  data);
	return ret;
}
float PopFloat  (size_t pos,  size_t* dataSize ,  void* data){
	float ret = *(float*)((size_t)data+pos*sizeof(float));
	PopBuffer(pos , (u32)sizeof(float) ,   dataSize ,  data);
	return ret;
}
long PopLong   (size_t pos,  size_t* dataSize ,  void* data){
	long ret = *(long*)((size_t)data+pos*sizeof(long));
	PopBuffer(pos , (u32)sizeof(long) ,   dataSize ,  data);
	return ret;
}
double PopDouble (size_t pos,  size_t* dataSize ,  void* data){
	double ret = *(double*)((size_t)data+pos*sizeof(double));
	PopBuffer(pos , (u32)sizeof(double) , dataSize ,  data);
	return ret;
}





