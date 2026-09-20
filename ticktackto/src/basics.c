#include "core.h"
#include "utils.h"
#include "shaders.h"
#include "render.h"

//#include <GL/gl.h>
#include <GLFW/glfw3.h>
#include <stdatomic.h>
#include <string.h>

#include "externel/stb_image.h"

#include "../include/tick-tack-to.h"
#define debugy(x) printf("%d\n",x);


typedef struct {float x,y;u32 c;} Vertex;

#define VERTFG_TRINGELS 0
#define VERTFG_CERCULS  1



typedef struct {
	char Practicul:4;
	bool Enbletextures:1;
	u32 textslot:6;
	int rsv:21;
}__attribute__((packed)) VertexFlags;


TickContext g_defaultContext;

atomic_bool g_LibraryHaveBeenInit=false;
atomic_uint  g_doubleClickeDelaye=300;//in ms

atomic_uint   g_pressedkey=0;//the last key been preased;
atomic_size_t g_pressed_window=0; //This is a pointer , a GLFWwindow* , for the window tha been preased by the last key 


void initDefautlFont();
void InitUI();

void keyboardPressReqaust(GLFWwindow* window, unsigned int codepoint);


TickContext TickInit(GLFWwindow* window){
	TickContext context;
	memset(&context,0,sizeof(TickContext));
	context.window=window;
	if(g_LibraryHaveBeenInit){
		context.Shader2D = g_defaultContext.Shader2D;
	}else {
		context.Shader2D= CreatShader(g_2DShape_vertexshader, g_2DShape_fragmentshader);
	}
	context.uniform2DMvp = GetUniform("u_MVP", context.Shader2D);		

	if(context.uniform2DMvp == -1){
		Eloge("SHADER ERORR");
	}
	InitlizeRendrer(&context.Shape2D);
	Genrate2DShapeAttribute(context.Shape2D.VAO, context.Shape2D.VertexBuffer, context.Shape2D.IndexBuffer);
	glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, (int*)&context.maxTexturesSlotsSepurted);//geting the maximum texture slots per texture
	if(context.maxTexturesSlotsSepurted > TICK_MAX_TEXTURE_SLOTS_SEPURTED){ 
		context.maxTexturesSlotsSepurted=TICK_MAX_TEXTURE_SLOTS_SEPURTED;
		Wloge("The maximum Textures slots sepurted ny your GPU is more than what TICK-TACK-TO can handel, so it will use %d slots",TICK_MAX_TEXTURE_SLOTS_SEPURTED);
	}

	
	context.samplerCount=0;
	context.samplerPtr=0;
	
	context.window_w=0;
	context.window_h=0;
	context.scaleX=1.0f;
	context.scaleY=1.0f;
	context.Z = TICK_TOP_Z;	
	context.lastClick=0.0;
	context.mousex=~0;
	context.mousey=~0;
	context.mousemensions=0;
	context.selectID=-1;
	glfwSetCharCallback(window, keyboardPressReqaust);


	if(!g_LibraryHaveBeenInit){
		g_defaultContext=context;
		initDefautlFont();
		InitUI();
		g_LibraryHaveBeenInit=true;
	}
	glUseProgram(context.Shader2D);

	char textureN[50];
	for(int i = 0 ; i < min(TICK_MAX_TEXTURE_SLOTS_SEPURTED , context.maxTexturesSlotsSepurted); i++){
		sprintf(textureN,"texture%d",i);
		u32 text =GetUniform((const char*)textureN, context.Shader2D);
		if(text!=-1){
			glUniform1i(text,i);
		}
	}
	
	

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
	return context;
}



void TickInitWindowFlags()
{
	glfwWindowHint(GLFW_DEPTH_BITS, 24);
	glfwWindowHint(GLFW_SAMPLES, 8);
	
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE,GLFW_OPENGL_CORE_PROFILE);
	return;
}



TickContext* GetDefaultContext(){return &g_defaultContext;}


void TickSetWindow(GLFWwindow* window){
	TickSetWindow_ctx(window, &g_defaultContext);
}


void TickSetWindow_ctx(GLFWwindow* window , TickContext* ctx){
	ctx->window=window;
	glfwSetCharCallback(window, keyboardPressReqaust);
}



/************* Mouse input ***********/

Vec2i GetMousePos(){
	Vec2i ret = {g_defaultContext.mousex,g_defaultContext.mousey};
	return ret;
}

Vec2i GetMousePos_ctx(TickContext* ctx){
	Vec2i ret = {ctx->mousex,ctx->mousey};
	return ret;
}

u32 GetMouseClickes(){
	return g_defaultContext.mousemensions;
}

u32 GetMouseClickes_ctx(TickContext* ctx){
	return ctx->mousemensions;
}


/***************** Keyboard inputr *******************/
char IsKeyPressed(u32 k){
	return IsKeyPressed_ctx(k, &g_defaultContext);
}

char IsKeyPressed_ctx(u32 k,TickContext* ctx){
	char ret = (glfwGetKey(ctx->window, k)==GLFW_PRESS);

	if(ret){
		ctx->lastKeyPress=k;
	}
	return ret;
}


u32 GetLastKey_ctx(TickContext* ctx){
	if(g_pressed_window == (size_t)ctx->window){
		return g_pressedkey;
	}
	return 0;
}
u32 GetLastKey(){
	return GetLastKey_ctx(&g_defaultContext);
}



char GetKeyPressed(u32 k){
	GetKeyPressed_ctx(k,&g_defaultContext);	

	return 0;
}

char GetKeyPressed_ctx(u32 k,TickContext* ctx)//is it ugly?will...
{
	if(glfwGetKey(ctx->window, k) == GLFW_RELEASE){
		return 0;
	}

	char ret = 0;
	
	
	if(k!=ctx->lastKeyPress){
		ctx->lastKeyPress=k;
		ctx->lastKeyPressTime=glfwGetTime();
		ret=true;
	}else {
		double delta = glfwGetTime()-ctx->lastKeyPressTime;
		ret=false;
		if(delta>0.5 ){
			ret=true;
			ctx->lastKeyPressTime+=delta-0.45;
		}
	}
	return ret;
}






void keyboardPressReqaust(GLFWwindow* window, unsigned int codepoint)
{
	g_pressedkey=codepoint;
	g_pressed_window=(size_t)window;
}



/************** Scale***************/



void SetScaleX(float scale){
	SetScaleX_ctx(&g_defaultContext, scale);
}
void SetScaleY(float scale){
	SetScaleY_ctx(&g_defaultContext, scale);
}
void SetScaleXY( float scalex, float scaley){
	SetScaleXY_ctx(&g_defaultContext, scalex, scaley);
}
void SetScale(float scale){
	SetScale_ctx(&g_defaultContext, scale);
}



void SetScaleX_ctx(TickContext* ctx, float scale){
	ctx->scaleX=scale;
}

void SetScaleY_ctx(TickContext* ctx, float scale){
	ctx->scaleY=scale;
}

void SetScaleXY_ctx(TickContext* ctx, float scalex, float scaley){
	ctx->scaleX=scalex;
	ctx->scaleY=scaley;
}
void SetScale_ctx(TickContext* ctx, float scale){
	ctx->scaleX=scale;
	ctx->scaleY=scale;
}


u32 GetScaleX()
{
	return  g_defaultContext.scaleX;
}
u32 GetScaleY()
{
	return  g_defaultContext.scaleY;
}


u32 GetScaleX_ctx(TickContext* ctx)
{
	return ctx->scaleX;
}
u32 GetScaleY_ctx(TickContext* ctx)
{
	return ctx->scaleY;
}

u32 GetWindowW()
{
	return g_defaultContext.window_w;
}
u32 GetWindowH()
{
	return g_defaultContext.window_h;
}
u32 GetWindowW_ctx(TickContext* ctx)
{
	return ctx->window_w;
}
u32 GetWindowH_ctx(TickContext* ctx)
{
	return ctx->window_h;
}

/******************************** Shapes Drawing ***********************************/






void DrawTriangle(Vec2f v1 , Vec2f v2, Vec2f v3 ,Vec4c cl)
{
	DrawTriangle_ctx(v1,  v2, v3, cl,&g_defaultContext);
}



void DrawLine(Vec2f v1 , Vec2f v2 , float thicknis , Vec4c cl){

	DrawLine_ctx( v1,  v2,  thicknis,  cl, &g_defaultContext);
	return;	
}

void DrawQuadrilateral(Vec2f v1 , Vec2f v2, Vec2f v3 , Vec2f v4,Vec4c cl)// v1___v2
									 //  |   |
									 //  |   |
									 // v3"""v4
{
	
	DrawQuadrilateral_ctx(v1 , v2, v3 , v4,cl,&g_defaultContext);
		
}
void DrawRectangel(float x, float y , float w , float h,Vec4c cl){
	DrawQuadrilateral((Vec2f){x,y}, (Vec2f){x+w,y}, (Vec2f){x,y+h}, (Vec2f){x+w,y+h},  cl);
	return;
}


void Draw2DVerteces(Vec2f* verteces , u32 Vertecount , Vec4c cl){
	Draw2DVerteces_ctx(verteces,Vertecount,cl,&g_defaultContext);
	return;
}




void Draw2DVertecesExt(Vec2f* verteces , u32 Vertecount ,u32* indeces,u32 Indexcont, Vec4c cl){
	Draw2DVertecesExt_ctx(verteces,Vertecount,indeces,Indexcont,cl,&g_defaultContext);
	return;
}


void DrawCircle(float x , float y , float r, float segments , Vec4c cl){
	DrawCircle_ctx(x, y, r,  segments,  cl, &g_defaultContext);
	return;

}


void DrawCircleSector(float x , float y , float r, float segmetDegree , float segmentsDegreeStart, float segments , Vec4c cl)
/*	startDegree (0° on this case)
 *      __
 *      | ^,   the segmentDgree(90° in this case)
 *     r|   \
 *     y|___|
 * 	x
 * hope that this is straid forwared*/
{

	DrawCircleSector_ctx(x, y, r, segmetDegree, segmentsDegreeStart, segments, cl, &g_defaultContext);
}
void DrawRoundedRectangel(float x, float y , float w , float h,float r , float segments ,Vec4c cl){
	DrawRoundedRectangel_ctx(x, y, w, h, r, segments, cl, &g_defaultContext);

}

void DrawEmptyCircle(float x , float y , float r,float thicknis, float segments , Vec4c cl){
	DrawEmptyCircle_ctx(x, y, r, thicknis, segments, cl, &g_defaultContext);
}

void DrawEmptyCircleSector(float x , float y , float r,float thicknis, float segmetDegree, float segmentsDegreeStart ,float segments , Vec4c cl)
{
	DrawEmptyCircleSector_ctx(x, y, r, thicknis, segmetDegree, segmentsDegreeStart, segments, cl, &g_defaultContext);
}
void DrawEmptyRoundedRectangel(float x, float y , float w , float h,float r , float thickness, float segments ,Vec4c cl){
	DrawEmptyRoundedRectangel_ctx(x,y, w, h, r, thickness, segments, cl, &g_defaultContext);
}






/************************************* Context Functions **********************************/






void DrawTriangle_ctx(Vec2f v1 , Vec2f v2, Vec2f v3 ,Vec4c cl, TickContext* ctx)
{
	
	ctx->Z-=TICK_Z_OFSSET;
	VertexFlags flage={.Practicul=VERTFG_TRINGELS,.Enbletextures=false};

	u32 c = cl.r << 24 | cl.g<<16 | cl.b << 8 | cl.a;
	float verteces[]={
		v1.x,v1.y,ctx->Z,*(float*)&c,*(float*)&flage,0,0,0,
		v2.x,v2.y,ctx->Z,*(float*)&c,*(float*)&flage,0,0,0,
		v3.x,v3.y,ctx->Z,*(float*)&c,*(float*)&flage,0,0,0
	};
	u32 indece[3]={0,(u32)1,(u32)2};//i know, this is reducled, but i am too lazy to think about a new way to do it with out a index count

	//u32 verty , indexy;
	//g_2DShapesBatchRen	derer->Push(verteces,9,indece,3);
	BatcheRendrerAdd2DShape(verteces, sizeof(verteces)/sizeof(float), indece, 3,8, &ctx->Shape2D);
	
	
}




void DrawLine_ctx(Vec2f v1 , Vec2f v2 , float thicknis , Vec4c cl,TickContext* ctx){
	float offsetFromeTheSenter = thicknis/2.0f;//we ant the center line to be align with the line that the user want
	if(v1.y == v2.y && v1.x == v2.x){
		return;//wont draw any way lol
	}else if(v1.y==v2.y){
		DrawQuadrilateral_ctx((Vec2f){v1.x,v1.y-offsetFromeTheSenter}, (Vec2f){v2.x,v2.y-offsetFromeTheSenter}, (Vec2f){v1.x,v1.y+offsetFromeTheSenter} , (Vec2f){v2.x,v2.y+offsetFromeTheSenter},  cl,ctx);
	}else if(v1.x==v2.x){
		DrawQuadrilateral_ctx((Vec2f){v1.x-offsetFromeTheSenter,v1.y}, (Vec2f){v2.x-offsetFromeTheSenter,v2.y}, (Vec2f){v1.x+offsetFromeTheSenter,v1.y} , (Vec2f){v2.x+offsetFromeTheSenter,v2.y},  cl,ctx);
	}//we wont lose profourmence becuse the user want a renamed rectangel are we?
	else {
		float a = (v1.x-v2.x)/(v2.y-v1.y);//basicly we the  perpendicular line of the line to make a line with in the line
		
		float x_ = sqrtf((offsetFromeTheSenter*offsetFromeTheSenter)/(a*a+1.0)) ;//with some maths, and some deep think, l plk sio ak opopjr ,jdn :kdll k [Segmentation Fult (core dump)]
		float y_ = x_*a ; //the y is bascily f(x);
		DrawQuadrilateral_ctx((Vec2f){v1.x-x_,v1.y-y_}, (Vec2f){v2.x-x_,v2.y-y_}, (Vec2f){v1.x+x_,v1.y+y_} , (Vec2f){v2.x+x_,v2.y+y_},  cl,ctx);
	}
	
	return;	
}





void DrawQuadrilateral_ctx(Vec2f v1 , Vec2f v2, Vec2f v3 , Vec2f v4,Vec4c cl, TickContext* ctx)
{
	VertexFlags flage={.Practicul=VERTFG_TRINGELS,.Enbletextures=false};
	
	ctx->Z-=TICK_Z_OFSSET;
	u32 indeces[6]={
		0,1,2,
		2,3,1
	};
	u32 c = (cl.r&0xff) << 24 | (cl.g&0xff) <<16 | (cl.b&0xff) << 8 | cl.a&0xff;	
	float verteces[]={ 
		v1.x,v1.y,ctx->Z,*(float*)&c,*(float*)&flage,0,0,0,
		v2.x,v2.y,ctx->Z,*(float*)&c,*(float*)&flage,0,0,0,
		v3.x,v3.y,ctx->Z,*(float*)&c,*(float*)&flage,0,0,0,
		v4.x,v4.y,ctx->Z,*(float*)&c,*(float*)&flage,0,0,0,
	};

	BatcheRendrerAdd2DShape(verteces, sizeof(verteces)/sizeof(float), indeces, sizeof(indeces)/sizeof(u32),8, &ctx->Shape2D);
	
}
void DrawRectangel_ctx(float x, float y , float w , float h,Vec4c cl,TickContext* ctx){
	DrawQuadrilateral_ctx((Vec2f){x,y}, (Vec2f){x+w,y}, (Vec2f){x,y+h}, (Vec2f){x+w,y+h},  cl,ctx);
	return;
}



void Draw2DVerteces_ctx(Vec2f* verteces , u32 Vertecount , Vec4c cl,TickContext* ctx){
	VertexFlags flage={.Practicul=VERTFG_TRINGELS,.Enbletextures=false};
	
	ctx->Z-=TICK_Z_OFSSET;

	float* Vertex = (float*)malloc((Vertecount*8)*sizeof(float));
	u32* indeces = (u32*)malloc(Vertecount*3*sizeof(u32));
	
	u32 c = cl.r << 24 | cl.g<<16 | cl.b << 8 | cl.a;
	for(u32 i = 0 ; i < Vertecount; i++){
		Vertex[i*8]=verteces[i].x;
		Vertex[i*8+1]=verteces[i].y;
		Vertex[i*8+2]=ctx->Z;
		Vertex[i*8+3]=*(float*)&c;
		Vertex[i*8+4]=*(float*)&flage;
		Vertex[i*8+5]=0;
		Vertex[i*8+6]=0;
		Vertex[i*8+7]=0;
	}
	for(u32 i = 0 ; i < Vertecount ; i++){
		indeces[i*3] = i;
		if(Vertecount-i == 2){
			indeces[i*3+1] = i+1;
			indeces[i*3+2] = 0;

		}
		else if(Vertecount-i == 1){
			indeces[i*3+1] = 0;
			indeces[i*3+2] = 1;

		}else{ 
			indeces[i*3+1] = i+1;
			indeces[i*3+2] = i+2;
		}
	}
	//g_2DShapesBatchRenderer->Push(Vertex,Vertecount*3,indeces,Vertecount*3);
	BatcheRendrerAdd2DShape(Vertex, Vertecount*8, indeces, Vertecount*3,8, &ctx->Shape2D);
	
	free(Vertex);
	free(indeces);
	return;
}




void Draw2DVertecesExt_ctx(Vec2f* verteces , u32 Vertecount ,u32* indeces,u32 Indexcont, Vec4c cl,TickContext* ctx){
	VertexFlags flage={.Practicul=VERTFG_TRINGELS,.Enbletextures=false};
	
	ctx->Z-=TICK_Z_OFSSET;

	float* Vertex = (float*)malloc((Vertecount*8)*sizeof(float));
	
	u32 c = cl.r << 24 | cl.g<<16 | cl.b << 8 | cl.a;
	for(u32 i = 0 ; i < Vertecount; i++){
		Vertex[i*8]=verteces[i].x;
		Vertex[i*8+1]=verteces[i].y;
		Vertex[i*8+2]=ctx->Z;
		Vertex[i*8+3]=*(float*)&c;
		Vertex[i*8+4]=*(float*)&flage;
		Vertex[i*8+5]=0;
		Vertex[i*8+6]=0;
		Vertex[i*8+7]=0;
	}

	BatcheRendrerAdd2DShape(Vertex, Vertecount*8, indeces, Indexcont, 8,&ctx->Shape2D);
	free(Vertex);

	
	return;
}


void DrawCircle_ctx(float x , float y , float r, float segments , Vec4c cl, TickContext* ctx){
	VertexFlags flage={.Practicul=VERTFG_CERCULS,.Enbletextures=false};
	
	ctx->Z-=TICK_Z_OFSSET;
	u32 c = cl.r << 24 | cl.g<<16 | cl.b << 8 | cl.a;	
	
	float verteces[24]={x,y  ,ctx->Z,*(float*)&c,*(float*)&flage,x,y,0.0f,
		 	   x,y-r,ctx->Z,*(float*)&c,*(float*)&flage,x,y,0.0f,
			   x,y-r,ctx->Z,*(float*)&c,*(float*)&flage,x,y,360.0f/segments};
	u32 indeces[3]={0,1,2};
	BatcheRendrerAdd2DShape(verteces, 24, indeces, 3,8, &ctx->Shape2D);
	
	for(int i = 1; i < segments ; i++){
		verteces[0] = x;
		verteces[1] = y-r;
		verteces[2] = ctx->Z;
		verteces[3] = *(float*)&c;
		verteces[4] = *(float*)&flage;
		verteces[5] = x;
		verteces[6] = y;
		float offset = ((float)(i+1))*(360.0f/(segments));
		verteces[7] =  offset>360.0f?360.0f:offset;
		indeces[0]=-i-2;
		indeces[1]=-1;
		indeces[2]=0;
		BatcheRendrerAdd2DShape(verteces, 8, indeces, 3,8,&ctx->Shape2D);
	}
	
	// now this is kinde good
	return;

}


void DrawCircleSector_ctx(float x , float y , float r, float segmetDegree , float segmentsDegreeStart, float segments , Vec4c cl, TickContext* ctx){
/*	startDegree (0° on this case)
 *      __
 *      | ^,   the segmentDgree(90° in this case)
 *     r|   \
 *     y|___|
 * 	x
 * hope that this is straid forwared*/

	VertexFlags flage={.Practicul=VERTFG_CERCULS,.Enbletextures=false};
	
	ctx->Z-=TICK_Z_OFSSET;
	u32 c = cl.r << 24 | cl.g<<16 | cl.b << 8 | cl.a;	
	
	float verteces[24]={x,y  ,ctx->Z,*(float*)&c,*(float*)&flage,x,y,0.0f,
		 	   x,y-r,ctx->Z,*(float*)&c,*(float*)&flage,x,y,segmentsDegreeStart,
			   x,y-r,ctx->Z,*(float*)&c,*(float*)&flage,x,y,segmetDegree/segments+segmentsDegreeStart};
	u32 indeces[3]={0,1,2};
	BatcheRendrerAdd2DShape(verteces, 24, indeces, 3,8, &ctx->Shape2D);
	
	for(int i = 1; i < segments ; i++){
		verteces[0] = x;
		verteces[1] = y-r;
		verteces[2] = ctx->Z;
		verteces[3] = *(float*)&c;
		verteces[4] = *(float*)&flage;
		verteces[5] = x;
		verteces[6] = y;
		float offset = ((float)(i+1))*(segmetDegree/(segments));
		verteces[7] =  (offset>segmetDegree?segmetDegree:offset)+segmentsDegreeStart;
		indeces[0]=-i-2;
		indeces[1]=-1;
		indeces[2]=0;
		BatcheRendrerAdd2DShape(verteces, 8, indeces, 3,8,&ctx->Shape2D);
	}
	
	// now this is kinde good
	return;

}


void DrawEmptyCircle_ctx(float x , float y , float r,float thicknis, float segments, Vec4c cl, TickContext* ctx){ //if you have a butter name let me know
	VertexFlags flage={.Practicul=VERTFG_CERCULS,.Enbletextures=false};
	
	ctx->Z-=TICK_Z_OFSSET;
	u32 c = cl.r << 24 | cl.g<<16 | cl.b << 8 | cl.a;	
	
	float verteces[16]={x,y-r           ,ctx->Z,*(float*)&c,*(float*)&flage,x,y,0.0f,
		 	   x,y-r+thicknis  ,ctx->Z,*(float*)&c,*(float*)&flage,x,y,0.0f};
	u32 indeces[6]={0,1,2};
	BatcheRendrerAdd2DShape(verteces, 16, indeces, 0,8, &ctx->Shape2D);
	
	for(int i = 0; i < segments ; i++){
		verteces[0] = x;
		verteces[1] = y-r;
		verteces[2] = ctx->Z;
		verteces[3] = *(float*)&c;
		verteces[4] = *(float*)&flage;
		
		verteces[5] = x;
		verteces[6] = y;
		float offset = ((float)(i+1))*(360.0f/(segments));
		verteces[7] =  offset>360.0f?360.0f:offset;
		
		verteces[8] = x;
		verteces[9] = y-r+thicknis;
		verteces[10] = ctx->Z;
		verteces[11] = *(float*)&c;
		verteces[12] = *(float*)&flage;
		
		verteces[13] = x;
		verteces[14] = y;
		verteces[15] = offset>360.0f?360.0f:offset;

		indeces[0]=-1;
		indeces[1]=-2;
		indeces[2]=0;
		
		indeces[3]=-1;
		indeces[4]=0;
		indeces[5]=1;
		
		BatcheRendrerAdd2DShape(verteces, 16, indeces, 6,8,&ctx->Shape2D);
	}
	
	// now this is kinde good
	return;

}




void DrawEmptyCircleSector_ctx(float x , float y , float r,float thicknis, 
		float segmetDegree, float segmentsDegreeStart ,float segments , Vec4c cl, TickContext* ctx)
{
	VertexFlags flage={.Practicul=VERTFG_CERCULS,.Enbletextures=false};
	
	ctx->Z-=TICK_Z_OFSSET;
	u32 c = cl.r << 24 | cl.g<<16 | cl.b << 8 | cl.a;	
	
	float verteces[16]={x,y-r           ,ctx->Z,*(float*)&c,*(float*)&flage,x,y,segmentsDegreeStart,
		 	   x,y-r+thicknis  ,ctx->Z,*(float*)&c,*(float*)&flage,x,y,segmentsDegreeStart};
	u32 indeces[6]={0,1,2};
	BatcheRendrerAdd2DShape(verteces, 16, indeces, 0,8, &ctx->Shape2D);
	
	for(int i = 0; i < segments ; i++){
		verteces[0] = x;
		verteces[1] = y-r;
		verteces[2] = ctx->Z;
		verteces[3] = *(float*)&c;
		verteces[4] = *(float*)&flage;
		
		verteces[5] = x;
		verteces[6] = y;
		float offset = ((float)(i+1))*(segmetDegree/(segments)) + segmentsDegreeStart;
		verteces[7] =  offset>(segmetDegree+segmentsDegreeStart)?(segmetDegree+segmentsDegreeStart):offset;
		
		verteces[8] = x;
		verteces[9] = y-r+thicknis;
		verteces[10] = ctx->Z;
		verteces[11] = *(float*)&c;
		verteces[12] = *(float*)&flage;
		
		verteces[13] = x;
		verteces[14] = y;
		verteces[15] = offset>(segmetDegree+segmentsDegreeStart)?(segmetDegree+segmentsDegreeStart):offset;

		indeces[0]=-1;
		indeces[1]=-2;
		indeces[2]=0;
		
		indeces[3]=-1;
		indeces[4]=0;
		indeces[5]=1;
		
		BatcheRendrerAdd2DShape(verteces, 16, indeces, 6,8,&ctx->Shape2D);
	}
	
	// now this is kinde good
	return;

}






void DrawRoundedRectangel_ctx(float x, float y , float w , float h,float r , float segments ,Vec4c cl,TickContext* ctx){
	if(!r){
		DrawQuadrilateral_ctx((Vec2f){x,y}, (Vec2f){x+w,y}, (Vec2f){x,y+h}, (Vec2f){x+w,y+h},  cl,ctx);
		return;
	}
	DrawRectangel_ctx(x, y+r, r, h-r*2, cl,ctx);
	DrawRectangel_ctx(x+w-r, y+r, r, h-r*2, cl,ctx);
	DrawRectangel_ctx(x+r, y, w-r*2, h, cl,ctx);
	
	DrawCircleSector_ctx(x+r  , y+r  , r, 90.0f, 270.0f, segments, cl, ctx);
	DrawCircleSector_ctx(x+w-r, y+r  , r, 90.0f, 0.0f  , segments, cl, ctx);
	DrawCircleSector_ctx(x+r  , y+h-r, r, 90.0f, 180.0f, segments, cl, ctx);
	DrawCircleSector_ctx(x+w-r, y+h-r, r, 90.0f, 90.0f , segments, cl, ctx);
	return;
}


void DrawEmptyRoundedRectangel_ctx(float x, float y , float w , float h,float r , float thickness, float segments ,Vec4c cl,TickContext* ctx)
{
	if(!r){
		DrawQuadrilateral_ctx((Vec2f){x,y}, (Vec2f){x+w,y}, (Vec2f){x,y+h}, (Vec2f){x+w,y+h},  cl,ctx);
		return;
	}
	
	DrawRectangel_ctx(x, y+r, thickness, h-r*2, cl,ctx);
	DrawRectangel_ctx(x+w-thickness, y+r, thickness, h-r*2, cl,ctx);
	
	DrawRectangel_ctx(x+r, y, w-r*2, thickness, cl,ctx);
	DrawRectangel_ctx(x+r, y+h-thickness, w-r*2, thickness, cl,ctx);
	
	
	DrawEmptyCircleSector_ctx(x+r  , y+r  , r,thickness, 90.0f, 270.0f, segments, cl, ctx);
	DrawEmptyCircleSector_ctx(x+w-r, y+r  , r,thickness, 90.0f, 0.0f  , segments, cl, ctx);
	DrawEmptyCircleSector_ctx(x+r  , y+h-r, r,thickness, 90.0f, 180.0f, segments, cl, ctx);
	DrawEmptyCircleSector_ctx(x+w-r, y+h-r, r,thickness, 90.0f, 90.0f , segments, cl, ctx);
	return;
}






/******************************************* Textures *********************************************/



TickTexture2D LoadTexture(void* bitmap,float w, float h, u32 bpp)
{
	return LoadTexture_ctx(bitmap,w, h, bpp, &g_defaultContext);
}
TickTexture2D LoadTextureFromeFile(const char * fileName)
{
	return LoadTextureFromeFile_ctx(fileName, &g_defaultContext);
}


void DrawTexture(TickTexture2D texture,float x , float y , float w,  float h )
{
	DrawTexture_ctx(texture, x, y, w,  h, &g_defaultContext);
	return;
}

void DrawTextureMask(TickTexture2D texture,float x , float y , float w , float h ,Vec4c mask)
{
	return DrawTextureMask_ctx(texture, x, y, w, h, mask, &g_defaultContext);
}

void DrawTextureExtended(TickTexture2D texture,Vec2f v1 , Vec2f v2 , Vec2f v3,  Vec2f v4,Vec4c mask)
{
	return DrawTextureExtended_ctx(texture, v1, v2, v3, v4, mask, &g_defaultContext);
}

void DrawTextureSegment(TickTexture2D texture,float x , float y  , float w, float h ,float xx , float yy ,  float ww,  float hh )
{
	return DrawTextureSegment_ctx(texture, x, y,  w,  h, xx, yy,  ww,  hh, &g_defaultContext);//heh, did you know that you can do that?
}

void DrawTextureSegmentMask             (TickTexture2D texture,float x , float y , float w , float h , float xx , float yy , float ww , float hh ,Vec4c mask)
{
	return DrawTextureSegmentMask_ctx(texture, x, y, w, h, xx, yy, ww, hh, mask, &g_defaultContext);
}
void DrawTextureSegmentExtended(TickTexture2D texture,Vec2f v1 ,Vec2f v2 ,Vec2f v3 ,Vec2f v4 ,Vec2f tc1 , Vec2f tc2 ,Vec2f tc3 ,Vec2f tc4,Vec4c mask )
{
	return DrawTextureSegmentExtended_ctx(texture,v1, v2, v3, v4,tc1,tc2,tc3,tc4,mask, &g_defaultContext);
}

void DrawVertcesTexture(TickTexture2D texture, Vec2f* verteces ,  Vec2f* texturepos,u32 vetexCount, u32* indeces, u32 indexCount, Vec4c mask)
{
	return; DrawVertcesTexture_ctx(texture, verteces, texturepos, vetexCount, indeces, indexCount, mask, &g_defaultContext);
}




void RemoveTexture(TickTexture2D* texture){
	RemoveTexture_ctx(texture, &g_defaultContext);
	return;
}

void ReloadTextureFromeFile(TickTexture2D* texture, const char* fileName){
	ReloadTextureFromeFile_ctx(texture, fileName, &g_defaultContext);
	return;
}

void ReloadTexture(TickTexture2D* texture, void* data,u32 w , u32 h , u32 bpp ){
	ReloadTexture_ctx(texture, data, w, h, bpp, &g_defaultContext);
	return;
}


//////////////context texture function




void DrawTexture_ctx(TickTexture2D texture,float x , float y , float w,  float h , TickContext* ctx){
	return DrawTextureExtended_ctx(texture, (Vec2f){x,y}, (Vec2f){x+w,y}, (Vec2f){x,y+h}, (Vec2f){x+w,y+h}, (Vec4c){255,255,255,255}, ctx);
}

void DrawTextureMask_ctx(TickTexture2D texture,float x , float y , float w , float h , Vec4c mask , TickContext* ctx)

{
	return DrawTextureExtended_ctx(texture, (Vec2f){x,y}, (Vec2f){x+w,y}, (Vec2f){x,y+h}, (Vec2f){x+w,y+h},mask, ctx);
}


void DrawTextureExtended_ctx(TickTexture2D texture,Vec2f v1 , Vec2f v2 , Vec2f v3,  Vec2f v4,Vec4c mask, TickContext* ctx){
	if(texture.index==-1){
		Eloge("Texture is not valide");
		return;
	}


	u32 c = (mask.r&0xff)<<24 | (mask.g&0xff)<<16| (mask.b&0xff)<<8 | mask.a&0xff;
	ctx->Z-=TICK_Z_OFSSET;
	int sampler = texture.index/ctx->maxTexturesSlotsSepurted;
	int slot = texture.index%ctx->maxTexturesSlotsSepurted;
	if(texture.index==-1 || 
	  sampler >= ctx->samplerPtr ||
	  !(ctx->samplers[sampler].texture[slot])){
		Eloge("Unvaliad Texture");
		return;
	}
	VertexFlags flage={.Practicul=VERTFG_TRINGELS,.Enbletextures=true,.textslot=slot};
	
	u32 indeces[6]={
		0,1,2,
		2,3,1
	};
	float verteces[]={ 
		v1.x, v1.y ,ctx->Z ,*(float*)&c,*(float*)&flage,0.0f,0.0f, 
		v2.x, v2.y ,ctx->Z ,*(float*)&c,*(float*)&flage,1.0f,0.0f, 
		v3.x, v3.y ,ctx->Z ,*(float*)&c,*(float*)&flage,0.0f,1.0f,
		v4.x, v4.y ,ctx->Z ,*(float*)&c,*(float*)&flage,1.0f,1.0f 
	};
	BatcheRendrerAdd2DShape(verteces, sizeof(verteces)/sizeof(float), indeces, sizeof(indeces)/sizeof(u32),7,&ctx->samplers[sampler].rendrer);
	
}



void DrawTextureSegment_ctx(TickTexture2D texture,float x , float y  , float w, float h ,float xx , float yy ,  float ww,  float hh , TickContext* ctx){
	DrawTextureSegmentExtended_ctx(texture, (Vec2f){x,y}, (Vec2f){x+w,y},(Vec2f){x,y+h}, (Vec2f){x+w,y+h}, (Vec2f){xx,yy}, (Vec2f){xx+ww,yy}, (Vec2f){xx,yy+hh}, (Vec2f){xx+ww,yy+hh} , (Vec4c){255,255,255	,255}, ctx);
}

void DrawTextureSegmentMask_ctx(TickTexture2D texture,float x , float y , float w , float h ,
			float xx  , float yy ,  float ww,  float hh ,Vec4c mask , TickContext* ctx)

{
	return DrawTextureSegmentExtended_ctx(texture, (Vec2f){x,y}, (Vec2f){x+w,y}, (Vec2f){x,y+h}, (Vec2f){x+w,y+h}, (Vec2f){xx,yy}, (Vec2f){xx+ww,yy}, (Vec2f){xx,yy+hh}, (Vec2f){xx+ww,yy+hh} ,mask, ctx);
}


void DrawTextureSegmentExtended_ctx(TickTexture2D texture,Vec2f v1 , Vec2f v2  , Vec2f v3, Vec2f v4 , 
		                    Vec2f tc1 , Vec2f tc2 , Vec2f tc3 , Vec2f tc4 ,Vec4c mask, TickContext* ctx)

{
	if(texture.index==-1){
		Eloge("Texture is not valide");
		return;
	}

	ctx->Z-=TICK_Z_OFSSET;
	int sampler = texture.index/ctx->maxTexturesSlotsSepurted;
	int slot = texture.index%ctx->maxTexturesSlotsSepurted;
	u32 c = (mask.r&0xff)<<24 | (mask.g&0xff)<<16| (mask.b&0xff)<<8 | mask.a&0xff;
	if(texture.index==-1 || 
	  sampler >= ctx->samplerPtr ||
	  !(ctx->samplers[sampler].texture[slot])){
		Eloge("Unvaliad Texture");
		return;
	}
	VertexFlags flage={.Practicul=VERTFG_TRINGELS,.Enbletextures=true,.textslot=slot&0x1f};
	u32 indeces[6]={
		0,1,2,
		2,3,1
	};
	const Vec2f segmentveteces[4]= {{tc1.x/(float)texture.w,tc1.y/(float)texture.h},
				  	{tc2.x/(float)texture.w,tc2.y/(float)texture.h},
				  	{tc3.x/(float)texture.w,tc3.y/(float)texture.h},
				  	{tc4.x/(float)texture.w,tc4.y/(float)texture.h}};
	float verteces[]={ 
		v1.x,v1.y,ctx->Z ,*(float*)&c,*(float*)&flage,segmentveteces[0].x,segmentveteces[0].y, 
		v2.x,v2.y,ctx->Z ,*(float*)&c,*(float*)&flage,segmentveteces[1].x,segmentveteces[1].y, 
		v3.x,v3.y,ctx->Z ,*(float*)&c,*(float*)&flage,segmentveteces[2].x,segmentveteces[2].y,
		v4.x,v4.y,ctx->Z ,*(float*)&c,*(float*)&flage,segmentveteces[3].x,segmentveteces[3].y, 
	};//textures are weard, they are truely are
	BatcheRendrerAdd2DShape(verteces, sizeof(verteces)/sizeof(float),indeces, sizeof(indeces)/sizeof(u32),7,&ctx->samplers[sampler].rendrer);
	
	
}




void DrawVertcesTexture_ctx(TickTexture2D texture, Vec2f* verteces ,  Vec2f* texturepos,u32 vetexCount, u32* indeces, u32 indexCount, Vec4c mask, TickContext* ctx) //nevr found a better name
{
	if(!indexCount ||!vetexCount || !texture.index || texture.index==-1){
		Eloge("Texture is not valide");
		return;
	}
	
	ctx->Z-=TICK_Z_OFSSET;
	int sampler = texture.index/ctx->maxTexturesSlotsSepurted;
	int slot = texture.index%ctx->maxTexturesSlotsSepurted;
	u32 c = (mask.r&0xff)<<24 | (mask.g&0xff)<<16| (mask.b&0xff)<<8 | mask.a&0xff;
	if(texture.index==-1 || 
			sampler >= ctx->samplerPtr ||
			!(ctx->samplers[sampler].texture[slot])){
		Eloge("Unvaliad Texture");
		return;
	}
	VertexFlags flage={.Practicul=VERTFG_TRINGELS,.Enbletextures=true,.textslot=slot&0x1f};

	float* vertexdata = malloc(vetexCount*7*sizeof(float));

	for(u32 i = 0 ; i < vetexCount ; i++){
		vertexdata[i*7]=verteces[i].x;
		vertexdata[i*7+1]=verteces[i].y;
		vertexdata[i*7+2]=ctx->Z;
		vertexdata[i*7+3]=*(float*)&c;
		vertexdata[i*7+4]=*(float*)&flage;
		vertexdata[i*7+5]=texturepos[i].x/(float)texture.w;
		vertexdata[i*7+6]=texturepos[i].y/(float)texture.h;
	}
	BatcheRendrerAdd2DShape(vertexdata,vetexCount*7,indeces, indexCount,7,&ctx->samplers[sampler].rendrer);

	free(vertexdata);
	return;
}







TickTexture2D LoadTexture_ctx(void* bitmap,float w, float h, u32 bpp, TickContext* ctx){
	TickTexture2D ret;
	u32 samplerNumber = -1;
	u32 textureSlot=-1;
	u32 notempty = false;

	for(int i = 0 ; i < ctx->samplerPtr ; i++){
		notempty=false;
		for(int ii = 0 ; ii < ctx->maxTexturesSlotsSepurted ; ii++){
			notempty|=ctx->samplers[i].texture[ii];
			if(!ctx->samplers[i].texture[ii] && samplerNumber==-1 && textureSlot==-1){
				textureSlot=ii;
				samplerNumber=i;
				
			}

		}
		if(samplerNumber!=-1){break;}
	}
		

	if(samplerNumber==-1){
		if(ctx->samplerCount<=ctx->samplerPtr+1){
			ctx->samplerCount+=50;
			ctx->samplers=(TickTextureRendrerStruct*)realloc(ctx->samplers, ctx->samplerCount*sizeof(TickTextureRendrerStruct));
			
		}
		samplerNumber=ctx->samplerPtr;
		textureSlot=0;
		memset(ctx->samplers[samplerNumber].texture,0,sizeof(ctx->samplers[samplerNumber].texture));
		InitlizeRendrer(&ctx->samplers[samplerNumber].rendrer);
		ctx->samplerPtr++;
		notempty=true;
	}

	ctx->samplers[samplerNumber].texture[textureSlot] = GenTexture();
	CHECK_GL_ERORR(glBindTexture(GL_TEXTURE_2D, ctx->samplers[samplerNumber].texture[textureSlot]));
	SetTextureData((u8*)bitmap, w, h, bpp);
	
	if(!notempty){
		InitlizeRendrer(&ctx->samplers[samplerNumber].rendrer);
	}
	
	ret=(TickTexture2D){.index=samplerNumber*ctx->maxTexturesSlotsSepurted + textureSlot,
	     .w=(u32)w,.h=(u32)h,.bpp=bpp};
	return ret;//samplerNumber*ctx->maxTexturesSlotsSepurted + textureSlot;
}




TickTexture2D LoadTextureFromeFile_ctx(const char * fileName, TickContext *ctx){
	int w,h,bpp;
	TickTexture2D ret={.index=(u32)-1};
	u8* pb = stbi_load((const char*)fileName, (int*)&w, (int*)&h, (int*)&bpp, (int)4);
	if(!pb){
		//Eloge("Cant Load "+ std::string(fileName)+" , "+std::string(stbi_failure_reason()));
		printf("[ERORR] Cant Load Texture \"%s\" , %s .\n",fileName,stbi_failure_reason());
		return ret;
	}	

	ret = LoadTexture_ctx(pb, w,h,  4, ctx);	
	stbi_image_free(pb);
	
	return ret;
}








void ReloadTexture_ctx(TickTexture2D* texture, void* data,u32 w , u32 h , u32 bpp , TickContext* ctx){
	int sampler = texture->index/ctx->maxTexturesSlotsSepurted;
	int slot = texture->index%ctx->maxTexturesSlotsSepurted;
	if(texture->index==-1 || 
	   sampler >= ctx->samplerPtr ||
	  !(ctx->samplers[sampler].texture[slot])){
		Eloge("Unvaliad Texture");
		return;
	}
	
	CHECK_GL_ERORR(glBindTexture(GL_TEXTURE_2D, ctx->samplers[sampler].texture[slot]));
	SetTextureData((u8*)data, w, h, bpp);
	texture->w=w;
	texture->h=h;
	texture->bpp=bpp;
	return;
}



void ReloadTextureFromeFile_ctx(TickTexture2D* texture, const char* fileName, TickContext* ctx){
	int sampler = texture->index/ctx->maxTexturesSlotsSepurted;
	
	u32 textureSlot = texture->index%ctx->maxTexturesSlotsSepurted;
	if(texture->index==-1 || 
	  sampler >= ctx->samplerPtr ||
	  !ctx->samplers[sampler].texture[textureSlot]){
		Eloge("Unvaliad Texture");
		return;
	}

	int w,h,bpp;
	u8* pb = stbi_load((const char*)fileName, (int*)&w, (int*)&h, (int*)&bpp, (int)4);
	if(!pb){
		Eloge("Cant Load \"%s\" , %s",fileName,stbi_failure_reason());
		return;
	}
	ReloadTexture_ctx(texture, pb,  w, h, 4, ctx);
	return;
}



void RemoveTexture_ctx(TickTexture2D* texture, TickContext* ctx){
	int sampler = texture->index/ctx->maxTexturesSlotsSepurted;
	
	u32 slot = texture->index%ctx->maxTexturesSlotsSepurted;
	if(texture->index==-1 || 
	  sampler >= ctx->samplerPtr ||
	  !ctx->samplers[sampler].texture[slot]){
		Eloge("Unvaliad Texture");
		return;
	}
	

	CHECK_GL_ERORR(glDeleteTextures(1, &ctx->samplers[sampler].texture[slot]));
	ctx->samplers[sampler].texture[slot]=0;

	u32 notempty = false;
	for(int i = 0 ; i < sizeof(ctx->samplers[sampler].texture) / sizeof(u32);++i){
		notempty|=ctx->samplers[sampler].texture[i]; 
		if(notempty)break;
	};
	

	if(!notempty){
		DeletRendrer(&ctx->samplers[sampler].rendrer);
	}
	*texture=(TickTexture2D){(u32)-1,(u32)-1,(u32)-1,(u32)-1};
	//we cant reorginaze them becuse we have to reindex all of them, at the same time we cant 
	//return a texture object becuse we want to use all the avliable slots for max profourmence
	//it is a traide of betwen gpu profourmence and tiny system memory
	//TODO: my be beter layout?
	return;
}







/************************************** Rendrer  ****************************************/







void TickRendre(){
	TickRendre_ctx(&g_defaultContext);

	return;
}


void TickNewFrame(){
	TickNewFrame_ctx(&g_defaultContext);
	return;

}


void TickNewFrame_ctx(TickContext* context){
	ResetRendrer(&context->Shape2D);
	ResetRendrer(&context->ShapeCir2D);
	for(int i = 0 ; i < context->samplerPtr ; i++){
		ResetRendrer(&context->samplers[i].rendrer);
	}
	
	context->Z=TICK_TOP_Z;
	context->selectCount=0;
	
	return;
}


void TickRendre_ctx(TickContext* ctx){
	if(ctx->Z<=TICK_BUTTOM_Z){
		Eloge("Z <=  %d you draw too much and part of that will not been rendred!",TICK_BUTTOM_Z);
	}
	
	if(!g_LibraryHaveBeenInit){
		Eloge("Rendring without a Context ===> did you call TickInit() ?");
		return;
	}
	
	int usedShader ;
	glGetIntegerv(GL_CURRENT_PROGRAM,&usedShader);
	if(usedShader!=ctx->Shader2D){
		CHECK_GL_ERORR(glUseProgram(ctx->Shader2D));
	}
	
	//if the window changed, update the mvp
	int real_w=0, real_h=0;
	glfwGetFramebufferSize(ctx->window, &real_w, &real_h);
	int window_w= (int)(((float)real_w)/ctx->scaleX);
	int window_h= (int)(((float)real_h)/ctx->scaleY);
	static float iw = 0.0;
	if(window_w!=ctx->window_w || window_h!=ctx->window_h){
		float proj[4][4] ;
		memset(&proj[0][0],0,sizeof(proj));
		proj[0][0] = 2.0f / (float)window_w;
		proj[1][1] =-2.0f / (float)window_h;
		proj[2][2] = 1.0f;
		proj[3][0] =-1.0f;
		proj[3][1] = 1.0f;
		proj[3][3] = 1.0f;

		glViewport(0,0,real_w,real_h);
		glUniformMatrix4fv(ctx->uniform2DMvp,1,GL_FALSE,&proj[0][0]);
		ctx->window_w=window_w;
		ctx->window_h=window_h;
	}
	Render(&ctx->Shape2D);
	for(int i = 0 ; i < ctx->samplerPtr ; i++){
		RenderTexture(&ctx->samplers[i]);
	}
	
	double xmouse,ymouse;
	
	glfwGetCursorPos(ctx->window, &xmouse, &ymouse);	
	
	ctx->mousex=xmouse/ctx->scaleX;
	ctx->mousey=ymouse/ctx->scaleY;
	if(glfwGetMouseButton(ctx->window, GLFW_MOUSE_BUTTON_LEFT)==GLFW_RELEASE){
		ctx->mousemensions&=~1;
	} 
	
	if(glfwGetMouseButton(ctx->window, GLFW_MOUSE_BUTTON_RIGHT)!=GLFW_RELEASE){
		ctx->mousemensions&=~2;
	}
	
	
	char mousebefaure = ctx->mousemensions;
	ctx->mousemensions=0;
	
	ctx->mousemensions |= (glfwGetMouseButton(ctx->window, GLFW_MOUSE_BUTTON_LEFT)!=GLFW_PRESS) && (mousebefaure&(1<<3));
	ctx->mousemensions |= (glfwGetMouseButton(ctx->window, GLFW_MOUSE_BUTTON_RIGHT)!=GLFW_PRESS && (mousebefaure&(1<<4)))<<1;
		
	ctx->mousemensions |= (glfwGetMouseButton(ctx->window, GLFW_MOUSE_BUTTON_LEFT)==GLFW_PRESS  ) <<3;
	ctx->mousemensions |= (glfwGetMouseButton(ctx->window, GLFW_MOUSE_BUTTON_RIGHT)==GLFW_PRESS ) <<4;
	

	
	u64 deley = (u64)((glfwGetTime() - ctx->lastClick)*1000.0);
	
	ctx->mousemensions|= (((deley <= g_doubleClickeDelaye)&&(ctx->mousemensions&1) && !(mousebefaure&1))&1)<<2;
	
	if(ctx->mousemensions&1){
		
		ctx->lastClick=glfwGetTime();
	}
	
	if(!glfwGetKey(ctx->window, ctx->lastKeyPress)){
		ctx->lastKeyPress=0; //if it is not presed; why wild we repret it?
	}

	if((size_t)ctx->window==g_pressed_window){
		g_pressedkey=0;
	}
	

	return;
}





void TickClose(){
	if(!g_LibraryHaveBeenInit){
		Eloge("Tick never init to close");
		return;
	}
	g_LibraryHaveBeenInit=false;
	TickClose_ctx(&g_defaultContext);
	
}


void TickClose_ctx(TickContext* context){
	DeletRendrer(&context->Shape2D);
	DeletRendrer(&context->ShapeCir2D);
	for(int i =  0 ; i  < context->samplerCount ; i++){
		DeletRendrer(&context->samplers[i].rendrer);
		for(int ii = 0 ; ii < context->maxTexturesSlotsSepurted ; ii++){
			if(context->samplers[i].texture[ii]){
				DeletTexture(&context->samplers[i].texture[ii]);
			}
		}
	}
}
