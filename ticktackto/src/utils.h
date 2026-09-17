
#ifndef UTILS_TICKTACKTO_CORE
#define UTILS_TICKTACKTO_CORE

#ifndef GLAD_GL_IMPLEMENTATION 
	#define GLAD_GL_IMPLEMENTATION
	#include "externel/glad.h"
#endif



#include <stdlib.h>
#include <math.h>
#include <string.h>
//#include <deque>
//#include <iostream>
//#include <fstream>
#include <GLFW/glfw3.h>
#ifndef __cplusplus
#if __STDC_VERSION__ <= 201710L
	typedef char bool;
	#define false 0
	#define true  1
#endif
#endif
//#define DEBUG_MODE //debug mode, note that is a memory consuming

typedef unsigned char  u8 ;
typedef unsigned short u16;
typedef unsigned int   u32;
typedef unsigned long  u64;

typedef struct{
	float x,y;
} Vec2f;
typedef struct{
	u32 x,y;
}Vec2i;
typedef struct{
	float x,y,z;
} Vec3f;
typedef struct{
	u32 x,y,z;
}Vec3i;
typedef struct{
	float x,y,z,w;
} Vec4f;
typedef struct{
	u32 x,y,z,w;
}Vec4i;
typedef struct{
	u8 r,g,b,a;
}Vec4c;


typedef struct {float x , y; u32 c;} VertexShape;
//bool operator!=(VertexShape v1,VertexShape v){return true;};





#ifdef DEBUG_MODE
#define CHECK_GL_ERORR(x)\
	while(glGetError());\
	(x);\
	while(GLenum err = glGetError()){\
		fprintf(stderr,"[OpenGL ERORR] %s => %x @ \"%s\" ,line %d\n",#x,err,__FILE__,__LINE__);\
	}
#endif

#ifndef DEBUG_MODE
#define CHECK_GL_ERORR(x) (x)
#endif


//void loge(std::string lg , std::string ms = "");
//void Eloge(std::string ms);
//void Wloge(std::string ms);
//void readFile(std::string path, std::string& data);

void loge (const char* lg ,const char* ms ,...);
void Eloge(const char* ms , ...);
void Wloge(const char* ms , ...);


void* readFile(const char* fileName , unsigned long * sizeOUT);
void tRotate(float* x,float* y,float xx,float yy,float theta);

char ReadBitmap(void* bm,u32 num);
void SetBitmap(void* bm,u32 num, char val);

u32 GetUnicode8Size(char* text,u32 size);//retutns the number of utf-8 charctures in the string (ASCII + utf8)
u32 GetUnicode8(char* text, u32 number, char val);//not safe! it dosnt try to be safe!

size_t minarg(u32 count ,...);
u64 max(u64 v1 , u64 v2);
u64 min(u64 v1 , u64 v2);


void* PushBuffer(void* val , u32 Vsize ,size_t pos,  size_t* dataSize , size_t* usedData , void* data);

void* PushChar   (char   val ,size_t pos,  size_t* dataSize , size_t* usedData , void* data);
void* PushShort  (short  val ,size_t pos,  size_t* dataSize , size_t* usedData , void* data);
void* PushInteger(int    val ,size_t pos,  size_t* dataSize , size_t* usedData , void* data);
void* PushFloat  (float  val ,size_t pos,  size_t* dataSize , size_t* usedData , void* data);
void* PushLong   (long   val ,size_t pos,  size_t* dataSize , size_t* usedData , void* data);
void* PushDouble (double val ,size_t pos,  size_t* dataSize , size_t* usedData , void* data);


void PopBuffer(size_t pos, u32 size,  size_t* dataSize ,void* data);

char   PopChar   (size_t pos,  size_t* dataSize ,  void* data);
short  PopShort  (size_t pos,  size_t* dataSize ,  void* data);
int    PopInteger(size_t pos,  size_t* dataSize ,  void* data);
float  PopFloat  (size_t pos,  size_t* dataSize ,  void* data);
long   PopLong   (size_t pos,  size_t* dataSize ,  void* data);
double PopDouble (size_t pos,  size_t* dataSize ,  void* data);
#endif 
