#include "utils.h"
#include "core.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define STB_IMAGE_IMPLEMENTATION
#include "externel/stb_image.h"



unsigned int CompileShader(unsigned int type , const char* src){
	unsigned int sh = glCreateShader(type);
	glShaderSource(sh,1,&src,NULL);
	glCompileShader(sh);
	//debuging
	int err;
	glGetShaderiv(sh,GL_COMPILE_STATUS , &err);
	if(!err){
		int len;
		glGetShaderiv(sh,GL_INFO_LOG_LENGTH,&len);
		char* errmsg = (char*)malloc(len);
		glGetShaderInfoLog(sh,len,&len,errmsg);
		//loge("*** "+std::string(type==GL_VERTEX_SHADER?"VERTAX":type==GL_FRAGMENT_SHADER?"FRAGMENT":"UKNOWN")+" SHADER ERORR ***");
		fprintf(stderr,"*** %s SHADER ERORR",type==GL_VERTEX_SHADER?"VERTAX":type==GL_FRAGMENT_SHADER?"FRAGMENT":"UKNOWN");
		fprintf(stderr, "===========================\n"
				"===========================\n%s\n"
				"===========================\n"
				"===========================",errmsg);
		//std::cout<<"===>>\n"<<errmsg<<"===>>END\n";
		return 0;
	}
	return sh;
}


unsigned int CreatShader(const char* vert, const char* frag){
	unsigned int prog = glCreateProgram();
	
	unsigned int vs = CompileShader(GL_VERTEX_SHADER,vert);
	unsigned int fs = CompileShader(GL_FRAGMENT_SHADER,frag);
	CHECK_GL_ERORR(glAttachShader(prog,vs));
	CHECK_GL_ERORR(glAttachShader(prog,fs));
	
	CHECK_GL_ERORR(glLinkProgram(prog));
	CHECK_GL_ERORR(glValidateProgram(prog));

	CHECK_GL_ERORR(glDeleteShader(vs));
	CHECK_GL_ERORR(glDeleteShader(fs));
	return prog;
}



int GetUniform(const char* uniform, u32 shader){
	int uniformLocation=glGetUniformLocation(shader,uniform);
	if(uniformLocation==-1){
		Wloge("uniform \"%s\" never found",uniform);
	}
	return uniformLocation;
}





const unsigned int SizeOfType(unsigned int type){
	switch (type) {
		case GL_FLOAT:
			return sizeof(float);
		case GL_UNSIGNED_INT:
			return sizeof(unsigned int);
		case GL_UNSIGNED_SHORT:
			return sizeof(unsigned short);
		case GL_UNSIGNED_BYTE:
			return sizeof(unsigned char);
		case GL_INT:
			return sizeof(int);
		case GL_SHORT:
			return sizeof(short);
		case GL_BYTE:
			return sizeof(char);
		default:
			return 0;

	}
	return 0;

}
u32 GenVertexArray(){
	u32 vao;
	CHECK_GL_ERORR(glGenVertexArrays(1,&vao));
	CHECK_GL_ERORR(glBindVertexArray(vao));
	return vao;
}

void DeletVertexArray(u32* vao){
	CHECK_GL_ERORR(glDeleteVertexArrays(1,vao));
}

void RegenrateVetexArray(u32* vao){
	DeletVertexArray(vao);
	*vao = GenVertexArray();
	return;
}

void GenVAAP(u32 index, u32 type, u32 norm,u32 strid , u32 count , void* offset){
	CHECK_GL_ERORR(glEnableVertexAttribArray(index));
	CHECK_GL_ERORR(glVertexAttribPointer(index,count,type, norm , strid*SizeOfType(type), (void*)offset));

}

u32 GenVertexBuffer(void* buff, u32 size){
	u32 vb;
	CHECK_GL_ERORR(glGenBuffers(1,&vb));
	CHECK_GL_ERORR(glBindBuffer(GL_ARRAY_BUFFER,vb));
	CHECK_GL_ERORR(glBufferData(GL_ARRAY_BUFFER,size,buff,GL_DYNAMIC_DRAW));
	return vb;
}
void FullVertexBuffer(void* buff, u32 size){
	CHECK_GL_ERORR(glBufferSubData(GL_ARRAY_BUFFER,0,size,buff));
	return;
}

void SetVertexBuff(void* data, u32 size, u32 pos){
	CHECK_GL_ERORR(glBufferSubData(GL_ARRAY_BUFFER,pos,size,data));

}
void DeletVertexBuffer(u32* vbID){
	if(*vbID){
		CHECK_GL_ERORR(glDeleteBuffers(1,vbID));
	}else {
		fprintf(stderr,"[ERORR] No vertex buffer to delete\n");
	}
	return;

}

void RegenrateVertexBuffer(u32* vbID, void* buff, u32 size){
	DeletVertexBuffer(vbID);
	*vbID = GenVertexBuffer(buff, size);
	return;
}




u32 GenIndexBuff(u32 *data, u32 size){
	u32 indexbuff;

	CHECK_GL_ERORR(glGenBuffers(1,&indexbuff));
	CHECK_GL_ERORR(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,indexbuff));
	CHECK_GL_ERORR(glBufferData(GL_ELEMENT_ARRAY_BUFFER,size,data,GL_DYNAMIC_DRAW));
	return indexbuff;
}

void DeletIndexBuff(u32 * indexbuff){
	if(*indexbuff){
		CHECK_GL_ERORR(glDeleteBuffers(1,indexbuff));
	}else {
		fprintf(stderr,"No index buffer to delete");
	}
}
void FullIndexBuff(void* buff, u32 size){
	CHECK_GL_ERORR(glBufferSubData(GL_ELEMENT_ARRAY_BUFFER,0,size,buff));
	return;
}
void SetIndexBuff(void* data, u32 size, u32 pos){
	CHECK_GL_ERORR(glBufferSubData(GL_ELEMENT_ARRAY_BUFFER,pos,size,data));
	return;
}


void RegenrateIndexBuffer(u32* indexbuff, u32* data, u32 size){
	DeletIndexBuff(indexbuff);
	*indexbuff = GenIndexBuff(data, size);
	return;
}





u32 GenTexture(){
	u32 texture;
	CHECK_GL_ERORR(glGenTextures(1, &texture));
	CHECK_GL_ERORR(glBindTexture(GL_TEXTURE_2D, texture));
	CHECK_GL_ERORR(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
	CHECK_GL_ERORR(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
	CHECK_GL_ERORR(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
	CHECK_GL_ERORR(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
	//glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);



	return texture;
}

void SetTextureData(u8 *data, u32 w, u32 h, u32 bpp){
	if(bpp == 4){
		glTexImage2D(GL_TEXTURE_2D, 0,GL_RGBA8, (int)w, (int)h, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	}else if(bpp == 3){
		glTexImage2D(GL_TEXTURE_2D, 0,GL_RGB8, (int)w, (int)h, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	
	}else if(bpp==1){
		glTexImage2D(GL_TEXTURE_2D, 0,GL_R3_G3_B2, (int)w, (int)h, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	}else {
		fprintf(stderr,"Texture formate not sepurted!");
	}
	return;
}
void SetTexturDataFromeFile(const char* fileName){
	stbi_set_flip_vertically_on_load(1);
	int w,h,bpp;
	u8* pb = stbi_load((const char*)fileName, (int*)&w, (int*)&h, (int*)&bpp, (int)4);
	if(!pb){
		Eloge("Cant Load %s , %s" ,fileName , stbi_failure_reason());
		return;
	}	

	glTexImage2D(GL_TEXTURE_2D, 0,GL_RGBA8, (int)w, (int)h, 0, GL_RGBA, GL_UNSIGNED_BYTE, pb);

	stbi_image_free(pb);
	return;
}

void BindTexture(u32 texture, u8 slot){
	CHECK_GL_ERORR(glActiveTexture(GL_TEXTURE0+(slot&0x1f)));
	CHECK_GL_ERORR(glBindTexture(GL_TEXTURE_2D, texture));
}



void DeletTexture(u32* texture){
	CHECK_GL_ERORR(glDeleteTextures(1, texture));
	return;
}



void BatchRendringAddVertex(float** buffer , u32 * bufferSize, u32* pointer, char* isItChanged,float* data, u32 count){
	
	if(!count)return;
	if((*pointer+count)*sizeof(float) > *bufferSize){
		u32 newMemorySize = *bufferSize+count*sizeof(float)+0x1000;
		if(*buffer){
			*buffer=(float*)realloc(*buffer,newMemorySize);

		}else {
			*buffer=(float*)malloc(newMemorySize);
		}
		*bufferSize=newMemorySize;
	}
	for(u32 i = 0 ; i < count ; i++ ){
		if(memcmp(
			&((*buffer)[*pointer+i])
			,&data[i],sizeof(float)
		))
		
		{
			(*buffer)[pointer[0]+i] =data[i];
			*isItChanged=true;
		}
	}
	*pointer+=count;
	return;
}



void BatchRendringAddIndex(u32** buffer , u32 * bufferSize, u32* pointer, char* isItChanged,u32* data, u32 count, u32 vertexPtr, u32 strid){
	if(!count)return;
	if((*pointer+count)*sizeof(float) >= *bufferSize){
		u32 newMemorySize = (*pointer+count)*sizeof(u32)+0x1000;
		if(*buffer){
			*buffer=(u32*)realloc(*buffer,newMemorySize);
		}else {
			*buffer=(u32*)malloc(newMemorySize);
		}
		*bufferSize=newMemorySize;
	}
	for(u32 i = 0 ; i < count ; i++ ){
		u32 data_ = data[i] + (vertexPtr/strid);
		if((*buffer)[*pointer+i]!=data_){
			(*buffer)[*pointer+i]=data_;
			*isItChanged=true;
		}
	}
	*pointer+=count;
	return;
}
