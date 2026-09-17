#include "utils.h"
#include "core.h"
#include "externel/stb_image.h"
#include "../include/tick-tack-to/basics.h"
#include <stdio.h>



void  Genrate2DShapeAttribute(u32 vao, u32 vb, u32 ib){
	
	CHECK_GL_ERORR(glBindVertexArray(vao));
	CHECK_GL_ERORR(glBindBuffer(GL_ARRAY_BUFFER,vb));
	CHECK_GL_ERORR(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,ib));
	
	CHECK_GL_ERORR(glEnableVertexAttribArray(0));
	CHECK_GL_ERORR(glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,8*sizeof(float),0));//pos
	CHECK_GL_ERORR(glEnableVertexAttribArray(1));
	CHECK_GL_ERORR(glVertexAttribPointer(1,1,GL_FLOAT,GL_FALSE,8*sizeof(float),(void*)12));//cl
	CHECK_GL_ERORR(glEnableVertexAttribArray(2));
	CHECK_GL_ERORR(glVertexAttribPointer(2,1,GL_FLOAT,GL_FALSE,8*sizeof(float),(void*)16));	//flags
	CHECK_GL_ERORR(glEnableVertexAttribArray(3));
	CHECK_GL_ERORR(glVertexAttribPointer(3,3,GL_FLOAT,GL_FALSE,8*sizeof(float),(void*)20));	// center + offset
	
}

void  GenrateTextureAttribute(u32 vao, u32 vb, u32 ib){ // yeah, circuls are a defrunt kinde of shape, how about that?
	CHECK_GL_ERORR(glBindVertexArray(vao));
	CHECK_GL_ERORR(glBindBuffer(GL_ARRAY_BUFFER,vb));
	CHECK_GL_ERORR(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,ib));
	
	CHECK_GL_ERORR(glEnableVertexAttribArray(0));
	CHECK_GL_ERORR(glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,7*sizeof(float),0));//pos
	CHECK_GL_ERORR(glEnableVertexAttribArray(1));
	CHECK_GL_ERORR(glVertexAttribPointer(1,1,GL_FLOAT,GL_FALSE,7*sizeof(float),(void*)12));//cl
	CHECK_GL_ERORR(glEnableVertexAttribArray(2));
	CHECK_GL_ERORR(glVertexAttribPointer(2,1,GL_FLOAT,GL_FALSE,7*sizeof(float),(void*)16));	//flags
	CHECK_GL_ERORR(glEnableVertexAttribArray(3));
	CHECK_GL_ERORR(glVertexAttribPointer(3,2,GL_FLOAT,GL_FALSE,7*sizeof(float),(void*)20));	// center + offset
	
}


void InitlizeRendrer(TickRendrerStruct* rendrer){
	rendrer->VAO = GenVertexArray();
	rendrer->VertexBuffer=GenVertexBuffer(NULL, 0);
	rendrer->IndexBuffer =GenIndexBuff(NULL, 0 );
	rendrer->VertexBufferSize=0;
	rendrer->IndexBufferSize=0;

	rendrer->vertexbatchSize=0x1000;
	rendrer->vertexbatchPtr=0;
	rendrer->vertexbatchr=(float*)malloc(0x1000);
	
	rendrer->indexbatchSize=0x1000;
	rendrer->indexbatchPtr=0;
	rendrer->indexbatchr=(u32*)malloc(0x1000);
	return;
}

void ResetRendrer(TickRendrerStruct* rendrer){
	rendrer->indexbatchPtr=0;
	rendrer->vertexbatchPtr=0;
	rendrer->isVertexChanged=false;
	rendrer->isIndexChanged=false;
	return;

}


void DeletRendrer(TickRendrerStruct* rendrer){
	DeletVertexArray(&rendrer->VAO);
	DeletVertexBuffer(&rendrer->VertexBuffer);
	DeletIndexBuff(&rendrer->IndexBuffer);
	rendrer->VAO=0;
	rendrer->VertexBuffer=0;
	rendrer->IndexBuffer=0;
	
	CHECK_GL_ERORR(glBindVertexArray(0));
	CHECK_GL_ERORR(glBindBuffer(GL_ARRAY_BUFFER,0));
	CHECK_GL_ERORR(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0));
	if(rendrer->vertexbatchr){
		free(rendrer->vertexbatchr);
		rendrer->vertexbatchr=NULL;
	}
	if(rendrer->indexbatchr){
		free(rendrer->indexbatchr);
		rendrer->indexbatchr=NULL;
	}
	rendrer->indexbatchPtr=0;
	rendrer->vertexbatchPtr=0;
	rendrer->indexbatchSize=0;
	rendrer->vertexbatchSize=0;
	rendrer->isVertexChanged=false;
	rendrer->isIndexChanged=false;
	return;
}



void BatcheRendrerAdd2DShape(float* vetex , u32 vcount , u32* index, u32 icount ,u32 steps, TickRendrerStruct* render){
	BatchRendringAddIndex(&render->indexbatchr, 
			&render->indexbatchSize,
			&render->indexbatchPtr,
			&render->isIndexChanged,
		 	index, icount, render->vertexbatchPtr, steps);
	BatchRendringAddVertex(&render->vertexbatchr,&render->vertexbatchSize , &render->vertexbatchPtr, &render->isVertexChanged 
			, vetex, vcount);
	return;
}




bool regenRendrerData(TickRendrerStruct* rendr){
	bool isitChanged = true;
	if(rendr->isVertexChanged){
		if(rendr->vertexbatchPtr*sizeof(float)>rendr->VertexBufferSize){
			RegenrateVertexBuffer(&rendr->VertexBuffer,rendr->vertexbatchr,rendr->vertexbatchPtr*sizeof(float));
			rendr->VertexBufferSize=rendr->vertexbatchPtr*sizeof(float);
			
			//std::cout<<"\nRegnarate Vertex Buffer to " << context.VertexBuffer2DSize;
			isitChanged=true;
		}else {
			glBindBuffer(GL_ARRAY_BUFFER,rendr->VertexBuffer);
			glBufferSubData(GL_ARRAY_BUFFER,0,rendr->vertexbatchPtr*4,rendr->vertexbatchr);
		}
	}
	if(rendr->isIndexChanged){
		if(rendr->indexbatchPtr*sizeof(u32)>rendr->IndexBufferSize){
			RegenrateIndexBuffer(&rendr->IndexBuffer,rendr->indexbatchr,rendr->indexbatchPtr*sizeof(u32));

			rendr->IndexBufferSize=rendr->indexbatchPtr*sizeof(u32);
			//std::cout<<"\nRegnarate Index Buffer to " << context.IndexBuffer2DSize;
			isitChanged=true;
		}else {
			CHECK_GL_ERORR(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,rendr->IndexBuffer));
			glBufferSubData(GL_ELEMENT_ARRAY_BUFFER,0,rendr->indexbatchPtr*sizeof(u32),rendr->indexbatchr);
			
		}
	}
	return isitChanged;

}





void Render(TickRendrerStruct* rendrer){
	bool rndChanged = regenRendrerData(rendrer);
	if(rndChanged){
		RegenrateVetexArray(&rendrer->VAO);
		Genrate2DShapeAttribute(rendrer->VAO, rendrer->VertexBuffer, rendrer->IndexBuffer);
	}

	CHECK_GL_ERORR(glBindVertexArray(rendrer->VAO));
	CHECK_GL_ERORR(glBindBuffer(GL_ARRAY_BUFFER,rendrer->VertexBuffer));
	CHECK_GL_ERORR(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,rendrer->IndexBuffer));
	
	CHECK_GL_ERORR(glDrawElements(GL_TRIANGLES, rendrer->indexbatchPtr, GL_UNSIGNED_INT, NULL));
}


void RenderExtended(TickRendrerStruct* rendrer){
	bool rndChanged = regenRendrerData(rendrer);
	if(rndChanged){
		RegenrateVetexArray(&rendrer->VAO);
		
		Genrate2DShapeAttribute(rendrer->VAO, rendrer->VertexBuffer, rendrer->IndexBuffer);
	}

	CHECK_GL_ERORR(glBindVertexArray(rendrer->VAO));
	CHECK_GL_ERORR(glBindBuffer(GL_ARRAY_BUFFER,rendrer->VertexBuffer));
	CHECK_GL_ERORR(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,rendrer->IndexBuffer));
	
	CHECK_GL_ERORR(glDrawElements(GL_TRIANGLES, rendrer->indexbatchPtr, GL_UNSIGNED_INT, NULL));
}


void RenderTexture(TickTextureRendrerStruct*texture){
	int skip = 0;
	for(int i = 0 ; i < sizeof(texture->texture)/sizeof(u32) ; i++){
		skip|=texture->texture[i];
		if(skip)break;
	}
	if(!skip){return;}
	bool rndChanged = regenRendrerData(&texture->rendrer);
	if(rndChanged){
		RegenrateVetexArray(&texture->rendrer.VAO);
		GenrateTextureAttribute(texture->rendrer.VAO, texture->rendrer.VertexBuffer, texture->rendrer.IndexBuffer);
	}	
	for(int i = 0 ; i <  TICK_MAX_TEXTURE_SLOTS_SEPURTED; ++i){
		if(texture->texture[i]){
			glActiveTexture(GL_TEXTURE0+i);
			CHECK_GL_ERORR(glBindTexture(GL_TEXTURE_2D,texture->texture[i]));
			//printf("\nRendring slots# %d",i);
		}
	}
		
	CHECK_GL_ERORR(glBindVertexArray(texture->rendrer.VAO));
	CHECK_GL_ERORR(glBindBuffer(GL_ARRAY_BUFFER,texture->rendrer.VertexBuffer));
	CHECK_GL_ERORR(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,texture->rendrer.IndexBuffer));
	
	CHECK_GL_ERORR(glDrawElements(GL_TRIANGLES, texture->rendrer.indexbatchPtr, GL_UNSIGNED_INT, NULL));
}


