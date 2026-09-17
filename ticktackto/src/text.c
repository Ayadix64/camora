#include "utils.h"
#include "../include/tick-tack-to.h"
#include "render.h"
#define STB_TRUETYPE_IMPLEMENTATION  
#include "externel/stb_truetype.h"
#include <stdatomic.h>
#include <stdlib.h>
#include <stdio.h>


//#define ENDPOINTS_SEPURTED 5000 //never make it under 32; it will subtract by 32, yeah, you will get a bad time
#define DEFAULTXPADD (u32)1
#define DEFAULTYPADD (u32)3
extern TickContext g_defaultContext;
atomic_uint g_tabSpaces = 5;

unsigned char defultFontBM[95][13] = {//thanks random persone on stackoverflow
	{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},// space :32
	{0x00, 0x00, 0x18, 0x18, 0x00, 0x00, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18},// ! :33
	{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x36, 0x36, 0x36, 0x36},
	{0x00, 0x00, 0x00, 0x66, 0x66, 0xff, 0x66, 0x66, 0xff, 0x66, 0x66, 0x00, 0x00},
	{0x00, 0x00, 0x18, 0x7e, 0xff, 0x1b, 0x1f, 0x7e, 0xf8, 0xd8, 0xff, 0x7e, 0x18},
	{0x00, 0x00, 0x0e, 0x1b, 0xdb, 0x6e, 0x30, 0x18, 0x0c, 0x76, 0xdb, 0xd8, 0x70},
	{0x00, 0x00, 0x7f, 0xc6, 0xcf, 0xd8, 0x70, 0x70, 0xd8, 0xcc, 0xcc, 0x6c, 0x38},
	{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x1c, 0x0c, 0x0e},
	{0x00, 0x00, 0x0c, 0x18, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x18, 0x0c},
	{0x00, 0x00, 0x30, 0x18, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x18, 0x30},
	{0x00, 0x00, 0x00, 0x00, 0x99, 0x5a, 0x3c, 0xff, 0x3c, 0x5a, 0x99, 0x00, 0x00},
	{0x00, 0x00, 0x00, 0x18, 0x18, 0x18, 0xff, 0xff, 0x18, 0x18, 0x18, 0x00, 0x00},
	{0x00, 0x00, 0x30, 0x18, 0x1c, 0x1c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
	{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00},
	{0x00, 0x00, 0x00, 0x38, 0x38, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
	{0x00, 0x60, 0x60, 0x30, 0x30, 0x18, 0x18, 0x0c, 0x0c, 0x06, 0x06, 0x03, 0x03},
	{0x00, 0x00, 0x3c, 0x66, 0xc3, 0xe3, 0xf3, 0xdb, 0xcf, 0xc7, 0xc3, 0x66, 0x3c},
	{0x00, 0x00, 0x7e, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x78, 0x38, 0x18},
	{0x00, 0x00, 0xff, 0xc0, 0xc0, 0x60, 0x30, 0x18, 0x0c, 0x06, 0x03, 0xe7, 0x7e},
	{0x00, 0x00, 0x7e, 0xe7, 0x03, 0x03, 0x07, 0x7e, 0x07, 0x03, 0x03, 0xe7, 0x7e},
	{0x00, 0x00, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0xff, 0xcc, 0x6c, 0x3c, 0x1c, 0x0c},
	{0x00, 0x00, 0x7e, 0xe7, 0x03, 0x03, 0x07, 0xfe, 0xc0, 0xc0, 0xc0, 0xc0, 0xff},
	{0x00, 0x00, 0x7e, 0xe7, 0xc3, 0xc3, 0xc7, 0xfe, 0xc0, 0xc0, 0xc0, 0xe7, 0x7e},
	{0x00, 0x00, 0x30, 0x30, 0x30, 0x30, 0x18, 0x0c, 0x06, 0x03, 0x03, 0x03, 0xff},
	{0x00, 0x00, 0x7e, 0xe7, 0xc3, 0xc3, 0xe7, 0x7e, 0xe7, 0xc3, 0xc3, 0xe7, 0x7e},
	{0x00, 0x00, 0x7e, 0xe7, 0x03, 0x03, 0x03, 0x7f, 0xe7, 0xc3, 0xc3, 0xe7, 0x7e},
	{0x00, 0x00, 0x00, 0x38, 0x38, 0x00, 0x00, 0x38, 0x38, 0x00, 0x00, 0x00, 0x00},
	{0x00, 0x00, 0x30, 0x18, 0x1c, 0x1c, 0x00, 0x00, 0x1c, 0x1c, 0x00, 0x00, 0x00},
	{0x00, 0x00, 0x06, 0x0c, 0x18, 0x30, 0x60, 0xc0, 0x60, 0x30, 0x18, 0x0c, 0x06},
	{0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0x00, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00},
	{0x00, 0x00, 0x60, 0x30, 0x18, 0x0c, 0x06, 0x03, 0x06, 0x0c, 0x18, 0x30, 0x60},
	{0x00, 0x00, 0x18, 0x00, 0x00, 0x18, 0x18, 0x0c, 0x06, 0x03, 0xc3, 0xc3, 0x7e},
	{0x00, 0x00, 0x3f, 0x60, 0xcf, 0xdb, 0xd3, 0xdd, 0xc3, 0x7e, 0x00, 0x00, 0x00},
	{0x00, 0x00, 0xc3, 0xc3, 0xc3, 0xc3, 0xff, 0xc3, 0xc3, 0xc3, 0x66, 0x3c, 0x18},
	{0x00, 0x00, 0xfe, 0xc7, 0xc3, 0xc3, 0xc7, 0xfe, 0xc7, 0xc3, 0xc3, 0xc7, 0xfe},
	{0x00, 0x00, 0x7e, 0xe7, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xe7, 0x7e},
	{0x00, 0x00, 0xfc, 0xce, 0xc7, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xc7, 0xce, 0xfc},
	{0x00, 0x00, 0xff, 0xc0, 0xc0, 0xc0, 0xc0, 0xfc, 0xc0, 0xc0, 0xc0, 0xc0, 0xff},
	{0x00, 0x00, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xfc, 0xc0, 0xc0, 0xc0, 0xff},
	{0x00, 0x00, 0x7e, 0xe7, 0xc3, 0xc3, 0xcf, 0xc0, 0xc0, 0xc0, 0xc0, 0xe7, 0x7e},
	{0x00, 0x00, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xff, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3},
	{0x00, 0x00, 0x7e, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x7e},
	{0x00, 0x00, 0x7c, 0xee, 0xc6, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06},
	{0x00, 0x00, 0xc3, 0xc6, 0xcc, 0xd8, 0xf0, 0xe0, 0xf0, 0xd8, 0xcc, 0xc6, 0xc3},
	{0x00, 0x00, 0xff, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0},
	{0x00, 0x00, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xdb, 0xff, 0xff, 0xe7, 0xc3},
	{0x00, 0x00, 0xc7, 0xc7, 0xcf, 0xcf, 0xdf, 0xdb, 0xfb, 0xf3, 0xf3, 0xe3, 0xe3},
	{0x00, 0x00, 0x7e, 0xe7, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xe7, 0x7e},
	{0x00, 0x00, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xfe, 0xc7, 0xc3, 0xc3, 0xc7, 0xfe},
	{0x00, 0x00, 0x3f, 0x6e, 0xdf, 0xdb, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0x66, 0x3c},
	{0x00, 0x00, 0xc3, 0xc6, 0xcc, 0xd8, 0xf0, 0xfe, 0xc7, 0xc3, 0xc3, 0xc7, 0xfe},
	{0x00, 0x00, 0x7e, 0xe7, 0x03, 0x03, 0x07, 0x7e, 0xe0, 0xc0, 0xc0, 0xe7, 0x7e},
	{0x00, 0x00, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0xff},
	{0x00, 0x00, 0x7e, 0xe7, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3},
	{0x00, 0x00, 0x18, 0x3c, 0x3c, 0x66, 0x66, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3},
	{0x00, 0x00, 0xc3, 0xe7, 0xff, 0xff, 0xdb, 0xdb, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3},
	{0x00, 0x00, 0xc3, 0x66, 0x66, 0x3c, 0x3c, 0x18, 0x3c, 0x3c, 0x66, 0x66, 0xc3},
	{0x00, 0x00, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x3c, 0x3c, 0x66, 0x66, 0xc3},
	{0x00, 0x00, 0xff, 0xc0, 0xc0, 0x60, 0x30, 0x7e, 0x0c, 0x06, 0x03, 0x03, 0xff},
	{0x00, 0x00, 0x3c, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x3c},
	{0x00, 0x03, 0x03, 0x06, 0x06, 0x0c, 0x0c, 0x18, 0x18, 0x30, 0x30, 0x60, 0x60},
	{0x00, 0x00, 0x3c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x3c},
	{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xc3, 0x66, 0x3c, 0x18},
	{0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
	{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x38, 0x30, 0x70},
	{0x00, 0x00, 0x7f, 0xc3, 0xc3, 0x7f, 0x03, 0xc3, 0x7e, 0x00, 0x00, 0x00, 0x00},
	{0x00, 0x00, 0xfe, 0xc3, 0xc3, 0xc3, 0xc3, 0xfe, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0},
	{0x00, 0x00, 0x7e, 0xc3, 0xc0, 0xc0, 0xc0, 0xc3, 0x7e, 0x00, 0x00, 0x00, 0x00},
	{0x00, 0x00, 0x7f, 0xc3, 0xc3, 0xc3, 0xc3, 0x7f, 0x03, 0x03, 0x03, 0x03, 0x03},
	{0x00, 0x00, 0x7f, 0xc0, 0xc0, 0xfe, 0xc3, 0xc3, 0x7e, 0x00, 0x00, 0x00, 0x00},
	{0x00, 0x00, 0x30, 0x30, 0x30, 0x30, 0x30, 0xfc, 0x30, 0x30, 0x30, 0x33, 0x1e},
	{0x7e, 0xc3, 0x03, 0x03, 0x7f, 0xc3, 0xc3, 0xc3, 0x7e, 0x00, 0x00, 0x00, 0x00},
	{0x00, 0x00, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xfe, 0xc0, 0xc0, 0xc0, 0xc0},
	{0x00, 0x00, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x00, 0x00, 0x18, 0x00},
	{0x38, 0x6c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x00, 0x00, 0x0c, 0x00},
	{0x00, 0x00, 0xc6, 0xcc, 0xf8, 0xf0, 0xd8, 0xcc, 0xc6, 0xc0, 0xc0, 0xc0, 0xc0},
	{0x00, 0x00, 0x7e, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x78},
	{0x00, 0x00, 0xdb, 0xdb, 0xdb, 0xdb, 0xdb, 0xdb, 0xfe, 0x00, 0x00, 0x00, 0x00},
	{0x00, 0x00, 0xc6, 0xc6, 0xc6, 0xc6, 0xc6, 0xc6, 0xfc, 0x00, 0x00, 0x00, 0x00},
	{0x00, 0x00, 0x7c, 0xc6, 0xc6, 0xc6, 0xc6, 0xc6, 0x7c, 0x00, 0x00, 0x00, 0x00},
	{0xc0, 0xc0, 0xc0, 0xfe, 0xc3, 0xc3, 0xc3, 0xc3, 0xfe, 0x00, 0x00, 0x00, 0x00},
	{0x03, 0x03, 0x03, 0x7f, 0xc3, 0xc3, 0xc3, 0xc3, 0x7f, 0x00, 0x00, 0x00, 0x00},
	{0x00, 0x00, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xe0, 0xfe, 0x00, 0x00, 0x00, 0x00},
	{0x00, 0x00, 0xfe, 0x03, 0x03, 0x7e, 0xc0, 0xc0, 0x7f, 0x00, 0x00, 0x00, 0x00},
	{0x00, 0x00, 0x1c, 0x36, 0x30, 0x30, 0x30, 0x30, 0xfc, 0x30, 0x30, 0x30, 0x00},
	{0x00, 0x00, 0x7e, 0xc6, 0xc6, 0xc6, 0xc6, 0xc6, 0xc6, 0x00, 0x00, 0x00, 0x00},
	{0x00, 0x00, 0x18, 0x3c, 0x3c, 0x66, 0x66, 0xc3, 0xc3, 0x00, 0x00, 0x00, 0x00},
	{0x00, 0x00, 0xc3, 0xe7, 0xff, 0xdb, 0xc3, 0xc3, 0xc3, 0x00, 0x00, 0x00, 0x00},
	{0x00, 0x00, 0xc3, 0x66, 0x3c, 0x18, 0x3c, 0x66, 0xc3, 0x00, 0x00, 0x00, 0x00},
	{0xc0, 0x60, 0x60, 0x30, 0x18, 0x3c, 0x66, 0x66, 0xc3, 0x00, 0x00, 0x00, 0x00},
	{0x00, 0x00, 0xff, 0x60, 0x30, 0x18, 0x0c, 0x06, 0xff, 0x00, 0x00, 0x00, 0x00},
	{0x00, 0x00, 0x0f, 0x18, 0x18, 0x18, 0x38, 0xf0, 0x38, 0x18, 0x18, 0x18, 0x0f},
	{0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18},
	{0x00, 0x00, 0xf0, 0x18, 0x18, 0x18, 0x1c, 0x0f, 0x1c, 0x18, 0x18, 0x18, 0xf0},
	{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x06, 0x8f, 0xf1, 0x60, 0x00, 0x00, 0x00}};  // :126



TickFont g_defaultFont;
atomic_bool g_defaultFontSetToDefault;

void initDefautlFont();
void FontsInit(){
	initDefautlFont();
}


void initDefautlFont(){

	g_defaultFontSetToDefault=true;
	g_defaultFont.size=13;
	g_defaultFont.linegap=13;
	g_defaultFont.scalex=1.0;
	g_defaultFont.scaley=1.0;
	g_defaultFont.CharcturesArray = (typeof(g_defaultFont.CharcturesArray))malloc(sizeof(typeof (g_defaultFont.CharcturesArray[0]))*95);
	g_defaultFont.maxChar=126;
	g_tabSpaces=5;
	u32* texture = (u32*)malloc(8*13*95*sizeof(u32));
	for(int i = 0 ; i < 95; i++){
		for(int y = 0 ; y < 13 ; y++ ){
			for(int x = 0 ; x < 8 ; x++){
				if(defultFontBM[i][12-y]&(1<<(7-x))){
					texture[y*95*8+x+i*8]=0xffffffff;	
				}else {
					texture[y*95*8+x+i*8]=0;
				}
			}
		}
		
		g_defaultFont.CharcturesArray[i].w=8;
		g_defaultFont.CharcturesArray[i].h=13;
		g_defaultFont.CharcturesArray[i].tcx=i*8;
		g_defaultFont.CharcturesArray[i].tcy=0;
		g_defaultFont.CharcturesArray[i].yoffset=0;

	}
	g_defaultFont.texture=LoadTexture(texture, 95*8, 13, 4);
}


void DeleteFont(TickFont*font){
	DeleteFont_ctx(font, &g_defaultContext);
}


void DeleteFont_ctx(TickFont*font,TickContext*ctx){
	if(!font->CharcturesArray){
		Eloge("Invaliad Font");
		return;
	}
	RemoveTexture_ctx(&g_defaultFont.texture,ctx);
	
	free(font->CharcturesArray);
	font->CharcturesArray=NULL;
	return;
}


void SetDefaultFont(TickFont* font){
	if(memcmp(font,&g_defaultFont,sizeof(TickFont)) && g_defaultFontSetToDefault){
		DeleteFont(&g_defaultFont);
	}
	g_defaultFont=*font;
	g_defaultFontSetToDefault=false;
}


TickFont GetDefaultFont(){return g_defaultFont;}

/*************************** Font Load Functions ****************************************/

TickFont LoadFont(const char* filen,u32 scale){
	return LoadFont_ctx(filen, scale,&g_defaultContext);
}
TickFont LoadFont_ctx(const char* filen,u32 scale,TickContext* ctx){
	TickFont ret;
	u64 lng;
	u8* data = (u8*)readFile(filen,&lng);
	if(!data){
		fprintf(stderr, "[ERORR] cant load font \"%s\", file curepted or not exiset or not allawed to use it.\n",filen);
		return g_defaultFont;
	}
	ret = LoadMemFont_ctx(data, lng, scale,ctx);
	free(data);
	return ret;
}


TickFont LoadMemFont(void* fontData, u32 size, u32 scale ){
	return LoadMemFont_ctx(fontData,size,scale, &g_defaultContext);
}


TickFont LoadMemFont_ctx(void* data,u32 size, u32 scale , TickContext* ctx){
	TickFont ret;
	stbtt_fontinfo font;
	stbtt_InitFont(&font, (u8*)data, 0/*stbtt_GetFontOffsetForIndex(data,0)*/);
	

	ret.CharcturesArray = (typeof(ret.CharcturesArray))malloc(((font.numGlyphs-32))*sizeof(typeof(ret.CharcturesArray[0]))); //if somthing break, this may be it
	
	ret.size=scale;
	ret.scalex=1.0f;
	ret.scaley=1.0f;
	ret.maxChar = font.numGlyphs;
	const u32 c = 0xffffffff;
	
	
	
	float fntscale = stbtt_ScaleForPixelHeight(&font, (float)scale);
	u32 gap;
	stbtt_GetFontVMetrics(&font, (int*)&gap, 0, 0);
	ret.linegap=gap*fntscale;
	
	int maxdemensions=0;    //maximum texture demensions (i.e 13060x13060)
 	glGetIntegerv(GL_MAX_TEXTURE_SIZE, &maxdemensions);   //Returns 1 value

	printf("**** GLO %d ****\n", font.numGlyphs);
	u32 texturewidth=0, textureheigth=0;
	u32 xoffset=0 ,yoffset=0;
	int w =0, h=0 , y0=0,x0=0;

	for(int i = 0 ; i < font.numGlyphs-32  ; i++){
		if(!i){
			u32 advance,lsb;
			stbtt_GetCodepointHMetrics(&font,i+32,(int*)&advance,(int*)&lsb);
			
			ret.CharcturesArray[i].w=advance*fntscale;
			ret.CharcturesArray[i].h=0;
			ret.CharcturesArray[i].tcx=0;
			ret.CharcturesArray[i].tcy=0;
			ret.CharcturesArray[i].yoffset=0;
			continue;
		}
		u8* bitmap = stbtt_GetCodepointBitmap(&font, 0,stbtt_ScaleForPixelHeight(&font, scale), i+32, &w, &h,&x0 ,&y0);
		ret.CharcturesArray[i].yoffset=y0+gap*fntscale;
		if(bitmap){
			if(xoffset+w>maxdemensions){
				xoffset=0;
				yoffset+=textureheigth;
				textureheigth+=yoffset;
			}else {
				if(xoffset+w>texturewidth){
					texturewidth=xoffset+w;
				}
				
				if(h+yoffset>textureheigth){
					textureheigth=yoffset+h;
				}
			}
		}
		ret.CharcturesArray[i].tcx=xoffset;//if some how the encoding error out (i dont know how ttf works btw) w & h will be zero, so just ignord
		ret.CharcturesArray[i].tcy=yoffset;
		ret.CharcturesArray[i].w=w;
		ret.CharcturesArray[i].h=h;
		xoffset+=w;
		free(bitmap);

	}
	xoffset=yoffset=0;
	u32 * texture = (u32*)malloc(texturewidth*textureheigth*sizeof(u32));
	
	for(int i = 1 ; i < font.numGlyphs-32 ; i++){
		int w , h;
		u8* bitmap = stbtt_GetCodepointBitmap(&font, 0,stbtt_ScaleForPixelHeight(&font, scale), i+32, &w, &h, 0,0);

		if(bitmap && ~(size_t)bitmap){
			for(int y = 0 ; y < h ; y++){
				for(int x = 0 ; x < w ; x++){
					if(bitmap[y*w+x]){
						//c&=~(0xff<<24);
						//c|=(((bitmap[y*w+x]*cl.a)/255)&0xff)<<24;
						
						texture[(y+yoffset)*texturewidth+x+xoffset]=(c&~(0xff<<24) | (((bitmap[y*w+x]*255)/255)&0xff)<<24) ;
					}else{
						texture[(y+yoffset)*texturewidth+x+xoffset]=0;
					}
				}
			}
			if(xoffset+w>texturewidth){
				xoffset=0;
				yoffset=ret.CharcturesArray[i].tcy;
			}
			else{
				xoffset+=w;
			}
			free(bitmap);
		}else {
			fprintf(stderr,"[ERORR] cant finde chartcture codepoint %d\n",i);
		}
	}
	ret.texture = LoadTexture_ctx(texture, texturewidth, textureheigth, 4,ctx);
	free(texture);
	return ret;
}



void ResetDefaultFont(){
	if(!g_defaultFontSetToDefault){
		initDefautlFont();
	}
	return;
}


void SetTabSpaces(u32 tabs){
	g_tabSpaces=tabs;
}




/**************************************** Text magerment functions **********************************/






void GetTextDemensions(const char* text, u32* w, u32* h){
	GetFontTextDemensions(text, g_defaultFont, w, h);
}


void GetFontTextDemensions(const char* text, TickFont font,u32* w, u32* h){
	GetFontTextDemensionsExtended(text, font, DEFAULTXPADD, DEFAULTYPADD, w, h);
}



void GetTextDemensionsExtended(const char* text,u32 xppading, u32 ypadding, u32* w, u32* h){
	GetFontTextDemensionsExtended(text, g_defaultFont,xppading,ypadding, w, h);
}


void GetFontTextDemensionsExtended(const char* text, TickFont font, u32 xpadding, u32 ypadding ,u32* w, u32* h){
	GetFontTextDemensionsExtendedSize(text, strlen(text), font, xpadding, ypadding, w, h);	
	return;

}


void GetTextDemensionsSuze(const char* text, u32 textSize,u32* w, u32* h){
	GetFontTextDemensionsSize(text, textSize, g_defaultFont, w, h);
}


void GetFontTextDemensionsSize(const char* text,u32 textSize ,TickFont font,u32* w, u32* h){
	GetFontTextDemensionsExtendedSize(text,textSize ,font, DEFAULTXPADD, DEFAULTYPADD, w, h);
}



void GetTextDemensionsExtendedSize(const char* text,u32 textSize,u32 xppading, u32 ypadding, u32* w, u32* h){
	GetFontTextDemensionsExtendedSize(text, textSize, g_defaultFont,xppading,ypadding, w, h);
}


void GetFontTextDemensionsExtendedSize(const char* text , u32 textSize, TickFont font, u32 xpadding, u32 ypadding ,u32* w, u32* h){
	if(w)*w=0;
	if(h)*h=0;
	u32 xx = 0;
	u32 yy = font.linegap;
	u32 ww=0,hh=0;
	for(int i = 0 ; i<textSize ;i++){
		
		if(text[i]>=32){
			ww = font.CharcturesArray[text[i]-32].w;
			hh = font.CharcturesArray[text[i]-32].h;
		}else {
			ww=hh=0;
		}
		if(text[i]=='\n'){
			yy+=font.linegap+ypadding;
			xx=0;
			continue;
		}
		else if(text[i]=='\t'){
			xx+=font.CharcturesArray[0].w*g_tabSpaces;
			continue;
		}
	
		else if(text[i]>font.maxChar){
			ww = font.CharcturesArray['?'-32].w;
			hh = font.CharcturesArray['?'-32].h;
			
		}else if(text[i]<32){continue;}
		xx+=ww+xpadding;
		if(w&&xx>*w){
			*(u32*)w=(u32)xx;
		}
	}
	if(h){
		*h = yy;
	}
	return;

}

void GetCharDemensions(u32 c ,u32* w, u32* h){
	GetFontCharDemensions(c, g_defaultFont, w, h);
}

void GetFontCharDemensions(u32 c,  TickFont font ,u32* w, u32* h){
	if(w)*w=0;
	if(h)*h=0;
	if(c=='\n'){
		if(h)*h=font.linegap;
	}else if(c=='\t'){
		*w = 	font.CharcturesArray[0].w*g_tabSpaces;
	}
	else if(c<font.maxChar && c>=32){
		if(w)*w=font.CharcturesArray[c-32].w;
		if(h)*h=font.CharcturesArray[c-32].h;
	}else if(c>font.maxChar){
		if(w)*w=font.CharcturesArray['?'-32].w;
		if(h)*h=font.CharcturesArray['?'-32].h;

	}

	return;

}



/***************************************** Text Drawing functions ************************************************/





void DrawText(const char* text , int x,int y,Vec4c cl ){
	DrawTextFont_ctx(text, x, y, cl,g_defaultFont, &g_defaultContext);	
	return;
}


void DrawText_ctx(const char* text , int x,int y,Vec4c cl ,TickContext* ctx){
	DrawTextFont_ctx(text, x, y,cl, g_defaultFont, ctx);	
	return;
}



void DrawTextFont(const char* text , int x,int y,Vec4c cl ,TickFont font){
	DrawTextFont_ctx(text, x, y, cl,font,&g_defaultContext);
	return;
}
void DrawTextFont_ctx(const char* text , int x,int y,Vec4c cl ,TickFont font,TickContext* ctx){
	DrawTextFontExtended_ctx(text, x, y, DEFAULTXPADD, DEFAULTYPADD, cl,g_defaultFont, ctx);
	return;
}


void DrawTextExtended(const char* text , int x,int y,u32 xpaading ,u32 ypadding ,Vec4c cl ){
	DrawTextExtended_ctx(text,  x, y, xpaading, ypadding,cl, &g_defaultContext);
}


void DrawTextExtended_ctx(const char* text , int x,int y,u32 xpaading ,u32 ypadding ,Vec4c cl ,TickContext* ctx){
	 DrawTextFontExtended_ctx(text , x, y,xpaading ,ypadding ,cl,g_defaultFont, ctx);
}




void DrawTextFontExtended(const char* text , int x,int y,u32 xpaading ,u32 ypadding ,Vec4c cl ,TickFont font){
	DrawTextFontExtended_ctx(text, x, y, xpaading, ypadding,cl, font, &g_defaultContext);
}


void DrawTextFontExtended_ctx(const char* text , int x,int y,u32 xpaading ,u32 ypadding ,Vec4c cl ,TickFont font,TickContext* ctx){
	DrawTextFontExtendedSize_ctx(text, (u32)strlen(text), x, y, xpaading, ypadding, cl,font, ctx);
	return;
}




void DrawTextSegment(const char* text , int x,int y , u32 xx , u32 yy , u32 w , u32 h,Vec4c cl){
	DrawTextSegment_ctx(text, x, y, xx, yy, w, h, cl,&g_defaultContext);
}

void DrawTextSegment_ctx(const char* text , int x,int y , u32 xx , u32 yy , u32 w , u32 h,Vec4c cl ,TickContext* ctx){
	DrawTextSegmentExtendedFont_ctx(text, x, y, xx, yy, w, h, DEFAULTXPADD, DEFAULTYPADD,cl ,g_defaultFont, &g_defaultContext);

	return;
}



void DrawTextSegmentExtended(const char* text , int x,int y , u32 xx , u32 yy , u32 w , u32 h,u32 xpadd, u32 ypadd,Vec4c cl ){
	DrawTextSegmentExtendedFont_ctx(text, x, y, xx, yy, w, h, xpadd, ypadd, cl,g_defaultFont, &g_defaultContext);
	return;
}

void DrawTextSegmentExtended_ctx(const char* text , int x,int y , u32 xx , u32 yy , u32 w , u32 h , u32 xpadd, u32 ypadd,Vec4c cl ,TickContext* ctx){
	DrawTextSegmentExtendedFont_ctx(text, x, y, xx, yy, w, h, xpadd, ypadd,cl, g_defaultFont, ctx);
	return;
}




void DrawTextSegmentExtendedFont(const char* text , int x,int y , u32 xx , u32 yy , u32 w , u32 h, u32 xpadd , u32 ypadd ,Vec4c cl , TickFont font){
	DrawTextSegmentExtendedFont_ctx(text, x, y, xx, yy, w, h, xpadd, ypadd, cl,font, &g_defaultContext);
	return;
}



void DrawTextSegmentExtendedFont_ctx(const char* text , int x,int y ,
					u32 xx , u32 yy ,
					u32 w , u32 h,
					u32 xpadd, u32 ypadd ,Vec4c cl ,
					TickFont font,TickContext* ctx)
{

	DrawTextSegmentExtendedFontSize_ctx(text,strlen(text),x,y,xx,yy,w,h,xpadd,ypadd,cl,font,ctx);
} 





/************************** Draw Text Size (i dont have a better name btw) *****************************/




void DrawTextSize(const char* text , u32 size , int x,int y,Vec4c cl ){
	DrawTextFontSize_ctx(text,size, x, y, cl,g_defaultFont, &g_defaultContext);
	return;
}


void DrawTextSize_ctx(const char* text , u32 size , int x,int y,Vec4c cl ,TickContext* ctx){
	DrawTextFontSize_ctx(text,size, x, y,cl, g_defaultFont, ctx);	
	return;
}



void DrawTextFontSize(const char* text , u32 size , int x,int y,Vec4c cl ,TickFont font){
	DrawTextFontSize_ctx(text,size, x, y, cl,font, &g_defaultContext);
	return;
}
void DrawTextFontSize_ctx(const char* text , u32 size , int x,int y,Vec4c cl ,TickFont font,TickContext* ctx){
	DrawTextFontExtendedSize_ctx(text,size, x, y, DEFAULTXPADD, DEFAULTYPADD,cl ,g_defaultFont, ctx);
	return;
}


void DrawTextExtendedSize(const char* text , u32 size , int x,int y,u32 xpaading ,u32 ypadding,Vec4c cl  ){
	DrawTextExtendedSize_ctx(text,size,  x, y, xpaading, ypadding, cl,&g_defaultContext);
}


void DrawTextExtendedSize_ctx(const char* text , u32 size , int x,int y,u32 xpaading ,u32 ypadding,Vec4c cl ,TickContext* ctx){
	 DrawTextFontExtendedSize_ctx(text ,  size , x, y,xpaading ,ypadding ,cl,g_defaultFont, ctx);
}




void DrawTextFontExtendedSize(const char* text , u32 size , int x,int y,u32 xpaading ,u32 ypadding ,Vec4c cl ,TickFont font){
	DrawTextFontExtendedSize_ctx(text,size, x, y, xpaading, ypadding,cl, font, &g_defaultContext);
}




void DrawTextSegmentSize(const char* text , u32 size , int x,int y , u32 xx , u32 yy , u32 w , u32 h,Vec4c cl ){
	DrawTextSegmentSize_ctx(text,size, x, y, xx, yy, w, h,cl, &g_defaultContext);
}

void DrawTextSegmentSize_ctx(const char* text , u32 size , int x,int y , u32 xx , u32 yy , u32 w , u32 h,Vec4c cl ,TickContext* ctx){
	DrawTextSegmentExtendedFontSize_ctx(text,size, x, y, xx, yy, w, h, DEFAULTXPADD, DEFAULTYPADD,cl, g_defaultFont, &g_defaultContext);

	return;
}



void DrawTextSegmentExtendedSize(const char* text , u32 size , int x,int y , u32 xx , u32 yy , u32 w , u32 h,u32 xpadd, u32 ypadd,Vec4c cl ){
	DrawTextSegmentExtendedFontSize_ctx(text,size, x, y, xx, yy, w, h, xpadd, ypadd,cl, g_defaultFont, &g_defaultContext);
	return;
}

void DrawTextSegmentExtendedSize_ctx(const char* text , u32 size , int x,int y , u32 xx , u32 yy , u32 w , u32 h , u32 xpadd, u32 ypadd,Vec4c cl ,TickContext* ctx){
	DrawTextSegmentExtendedFontSize_ctx(text,size, x, y, xx, yy, w, h, xpadd, ypadd,cl, g_defaultFont, ctx);
	return;
}




void DrawTextSegmentExtendedFontSize(const char* text , u32 size , int x,int y , u32 xx , u32 yy , u32 w , u32 h, u32 xpadd , u32 ypadd,Vec4c cl  , TickFont font){
	DrawTextSegmentExtendedFontSize_ctx(text,size, x, y, xx, yy, w, h, xpadd, ypadd,cl, font, &g_defaultContext);
	return;
}






void DrawTextFontExtendedSize_ctx(const char* text , u32 size,int x,int y,u32 xpaading ,u32 ypadding,Vec4c cl  ,TickFont font,TickContext* ctx)
{
	int xx = x;
	u32 ww=0,hh=0,tcx=0,tcy=0;
	int yoff=0;
	u32 c=0;
	u32 utf8left=0;
	for(int i = 0 ; i< size; i++){

		if((((u8)text[i]) & 0x80 )== 0){
			c=text[i];
			utf8left=0;
		}
		else {

			u8 cc = text[i];
			bool addtoleft = !utf8left;
			if(!utf8left){
				c=0;
			}
			int ii=7;
			for ( ; ii >= 0 ; ii--){

				if( !(cc & (1<<ii))){
					break;
				}
				if(addtoleft){utf8left++;}
			} 
			c<<=ii;
			c|=cc & ~(0xff<<ii);

			utf8left--;
			if(utf8left){continue;}
		}
		if(c>=32 && c<font.maxChar){
			ww   = font.CharcturesArray[c-32].w      ;
			hh   = font.CharcturesArray[c-32].h      ;
			tcx  = font.CharcturesArray[c-32].tcx    ;
			tcy  = font.CharcturesArray[c-32].tcy    ;
			yoff = font.CharcturesArray[c-32].yoffset;
		}else {
			ww=hh=tcx=tcy=yoff=0;
		}
		if(c== '\n'){
			y+=font.linegap*font.scaley+ypadding;
			xx=x;
			continue;
		}
		else if(c=='\t'){
			xx+=font.CharcturesArray[0].w*g_tabSpaces;
			continue;
		}
		else if(c== ' '){
			xx+=ww*font.scalex;
			continue;
		}
		else if(c>font.maxChar){
			ww = font.CharcturesArray['?'-32].w;
			hh = font.CharcturesArray['?'-32].h;
			tcx= font.CharcturesArray['?'-32].tcx;

		}else if(c<32){continue;}
		else {
			DrawTextureSegmentMask_ctx(font.texture, xx, y+yoff, ww*font.scalex, hh*font.scaley, tcx, tcy, ww, hh,cl,ctx);
		}
		xx+=ww*font.scalex+xpaading;
	}
	return;
}




void DrawTextSegmentExtendedFontSize_ctx(const char* text ,u32 size, int x,int y ,
					u32 xx , u32 yy ,
					u32 w , u32 h,
					u32 xpadd, u32 ypadd,Vec4c cl ,
					TickFont font,TickContext* ctx) //bor is it a long name
{
	int xpos = x;
	int ypos = y;
	u32 fcxoff=0;//first charceture x offset
	u32 lcxoff=0;//last  ...
	u32 fcyoff=0;//first charceture y offset
	u32 lcyoff=0;//last  ...
	u32 ww=0,hh=0,tcx=0,tcy=0,yoff=0;
	u32 c = 0;
	u32 utf8left=0;
	
	for(int i = 0 ; i<size; i++,xpos+=ww*font.scalex+xpadd){
		/************** utf8 char decide ***************/
		if((((u8)text[i]) & 0x80 )== 0){
			c=text[i];
			utf8left=0;
		}
		else {

			u8 cc = text[i];
			bool addtoleft = !utf8left;
			if(!utf8left){
				c=0;
			}
			int ii=7;
			for ( ; ii >= 0 ; ii--){

				if( !(cc & (1<<ii))){
					break;
				}
				if(addtoleft){utf8left++;}
			} 
			c<<=ii;
			c|=cc & ~(0xff<<ii);

			utf8left--;
			if(utf8left){continue;}
		}
		/***********************************************/
		if(c>=32 && c<font.maxChar){
			ww   = font.CharcturesArray[c-32].w;
			hh   = font.CharcturesArray[c-32].h;
			tcx  = font.CharcturesArray[c-32].tcx;
			tcy  = font.CharcturesArray[c-32].tcy;
			yoff = font.CharcturesArray[c-32].yoffset;
		}else{
			ww=hh=tcx=tcy=yoff=0;
		}
		
		if(c== '\n'){
			ypos+=font.linegap*font.scaley+ypadd;
			xpos=x-(ww)*font.scalex-xpadd;//this is all will automaticly aded
			continue;
		}
		else if(c=='\t'){
			ww=font.CharcturesArray[0].w*g_tabSpaces; 
			continue;
		}
		else if(c>font.maxChar){
			ww = font.CharcturesArray['?'-32].w;
			hh = font.CharcturesArray['?'-32].h;
			tcx= font.CharcturesArray['?'-32].tcx;
		}
		else if(c<=32){continue;}
		if(xpos-x+ww<xx || xpos-x>xx+w || ypos+yoff-y+hh<yy || ypos+yoff-y>yy+h){continue;}
		
		fcxoff = xpos-x<xx?xx-(xpos-x):0;
		lcxoff = xpos-x+ww>xx+w?xpos-x+ww-(xx+w):0;
				
		fcyoff = ypos+yoff-y<yy?yy-(ypos+yoff-y):0;
		lcyoff = ypos+yoff-y+hh>yy+h?ypos+yoff-y+hh-(yy+h):0;

		DrawTextureSegmentMask_ctx( font.texture, xpos+fcxoff         , 
					ypos+yoff+fcyoff                  , 
					(ww-fcxoff-lcxoff)*font.scalex    ,
					(hh-fcyoff-lcyoff)*font.scaley    , 
					tcx+fcxoff, tcy+fcyoff            , 
					ww-fcxoff-lcxoff, hh-fcyoff-lcyoff,cl,
					ctx);
	}
	return;
}
