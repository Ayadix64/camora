#ifndef TICK_CORE
#define TICK_CORE
#include "utils.h"

unsigned int CreatShader(const char* vert, const char* frag);
unsigned int CompileShader(unsigned int type , const char* src);
int GetUniform(const char* uniform, u32 shader);


u32 GenVertexArray();
void DeletVertexArray(u32* vao);
void RegenrateVetexArray(u32* vao);
void GenVAAP(u32 index, u32 type, u32 norm,u32 strid , u32 count , void* offset);
const unsigned int SizeOfType(unsigned int type);


u32 GenVertexBuffer(void* buff, u32 size);
void FullVertexBuffer(void* buff, u32 size);
void DeletVertexBuffer(u32* vbID);
void SetVertexBuff(void* data, u32 size, u32 pos);
void RegenrateVertexBuffer(u32* vbID, void* buff, u32 size);


u32 GenIndexBuff(u32* data, u32 size);
void DeletIndexBuff(u32 * indexbuff);
void FullIndexBuff(void* buff, u32 size);
void SetIndexBuff(void* data, u32 size, u32 pos);
void RegenrateIndexBuffer(u32* indexbuff, u32* data, u32 size);


u32 GenTexture();
void SetTextureData(u8* data, u32 w,u32 h, u32 bpp);
void SetTexturDataFromeFile(const char* fileName);
void BindTexture(u32 texture, u8 slot);
void DeletTexture(u32* texture);


void BatchRendringAddVertex(float** buffer , u32 * bufferSize, u32* pointer, char* isItChanged,float* data, u32 count);
void BatchRendringAddIndex(u32** buffer , u32 * bufferSize, u32* pointer, char* isItChanged,u32* data, u32 count , u32 vertexptr, u32 vertexstrid);



#endif
