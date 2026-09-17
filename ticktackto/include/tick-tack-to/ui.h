#ifndef TICK_UI
#define TICK_UI
#include "utils.h"
#include "basics.h"
#include "text.h"

#define UI_DEFAULT_BACKGROUND_COLOR (Vec4c){0x9,0x16,0x56,0xff} //#091656
#define UI_DEFAULT_HOUVER_COLOR     (Vec4c){0x05,0x14,0xb8 ,255} //#0514B8
#define UI_DEFAULT_SELECT_COLOR     (Vec4c){0x20,0x41,0xe8,255} //#2041e8
#define UI_DEFAULT_FONT_COLOR       (Vec4c){0xff,0xff,0xff,0xff} 


typedef struct {
	char EnableNumbers:1;
	char EnbleCharctures:1;
	char Password:1;
} TickTextBoxFlags;

typedef struct {
	u32 size;
	u32 maxsize;
	u32 usedsize;
	
	u32 pos;
	u32 xoffset;

	TickTextBoxFlags flags;
	char* data;
}TextBoxData;

void ResetDefaultUIColors();
void SetDefaultUIColors(Vec4c dbg, Vec4c hc, Vec4c sc , Vec4c fc);



char Button              (const char* text,int x , int y , int w /*0 or -1 for default*/ , int h /*0 or -1 for default*/); //bit1: preased, bit2: hover
char Button_ctx          (const char* text,int x , int y ,int w , int h ,TickContext* ctx); 
char ButtonColor         (const char* text,int x , int y ,int w , int h ,Vec4c dbg, Vec4c hbg, Vec4c sbg,Vec4c tc); 
char ButtonColor_ctx     (const char* text,int x , int y ,int w , int h ,Vec4c dbg, Vec4c hbg, Vec4c sbg,Vec4c tc,TickContext*ctx); 
char ButtonFont          (const char* text,int x , int y ,int w , int h ,TickFont font ,u32 xpadd, u32 ypadd);
char ButtonFont_ctx      (const char* text,int x , int y ,int w , int h ,TickFont font ,u32 xpadd, u32 ypadd, TickContext*ctx); 
char ButtonExtended      (const char* text,int x , int y ,int w , int h ,Vec4c dbg, Vec4c hbg, Vec4c sbg,Vec4c tc, TickFont font ,u32 xpadd ,u32 ypadd); 
char ButtonExtended_ctx  (const char* text,int x , int y ,int w , int h ,Vec4c dbg, Vec4c hbg, Vec4c sbg,Vec4c tc, TickFont font ,u32 xpadd ,u32 ypadd,TickContext*ctx);




void InitTextBoxData(TextBoxData* tbd,u32 maxsize);

char TextBox            (int x , int y , int w,int h/*0 or-1 for default*/,   TextBoxData* tbd);
char TextBox_ctx        (int x , int y , int w , int h/*0 or-1 for default*/, TextBoxData* tbd,TickContext*ctx);
char TextBoxColor       (int x , int y , int w , int h, TextBoxData* tbd, Vec4c bg, Vec4c hbg , Vec4c sbg, Vec4c cursurCl,Vec4c tc);
char TextBoxColor_ctx   (int x , int y , int w , int h, TextBoxData* tbd, Vec4c bg, Vec4c hbg , Vec4c sbg, Vec4c cursurCl,Vec4c tc,TickContext*ctx);
char TextBoxFont        (int x , int y , int w , int h, TextBoxData* tbd, TickFont font ,u32 xpadd ,u32 ypadd);
char TextBoxFont_ctx    (int x , int y , int w , int h, TextBoxData* tbd, TickFont font ,u32 xpadd ,u32 ypadd, TickContext*ctx);
char TextBoxExtended    (int x , int y , int w , int h, TextBoxData* tbd, Vec4c bg, Vec4c hbg , Vec4c sbg, Vec4c cursurCl,Vec4c tc,TickFont font , u32 xpadd, u32 ypadd);
char TextBoxExtended_ctx(int x , int y , int w , int h, TextBoxData* tbd, Vec4c bg, Vec4c hbg , Vec4c sbg, Vec4c cursurCl,Vec4c tc,TickFont font , u32 xpadd, u32 ypadd,TickContext*ctx);




void CheckBox            (const char* bx ,int x , int y , char* b);
void CheckBox_ctx        (const char* bx ,int x , int y , char* b, TickContext* ctx);
void CheckBoxColor       (const char* bx ,int x , int y , char* b, Vec4c dbg, Vec4c hbg, Vec4c sbg,Vec4c tc);
void CheckBoxColor_ctx   (const char* bx ,int x , int y , char* b, Vec4c dbg, Vec4c hbg, Vec4c sbg,Vec4c tc,TickContext* ctx);
void CheckBoxFont        (const char* bx ,int x , int y , char* b, TickFont font ,u32 xpadd, u32 ypadd);
void CheckBoxFont_ctx    (const char* bx ,int x , int y , char* b, TickFont font ,u32 xpadd, u32 ypadd,TickContext* ctx);
void CheckBoxExtended    (const char* bx ,int x , int y , char* b, Vec4c dbg, Vec4c hbg, Vec4c sbg, Vec4c tc, TickFont font ,u32 xpadd, u32 ypadd);
void CheckBoxExtended_ctx(const char* bx ,int x , int y , char* b, Vec4c dbg, Vec4c hbg, Vec4c sbg, Vec4c tc,TickFont font ,u32 xpadd, u32 ypadd, TickContext* ctx);



char Slider          (int x , int y , int w,  float* s);
char Slider_ctx      (int x , int y , int w,  float* s ,TickContext* ctx);
char SliderColor     (int x , int y , int w,  float* s ,Vec4c dbg, Vec4c hbg, Vec4c sbg);
char SliderColor_ctx (int x , int y , int w,  float* s ,Vec4c dbg, Vec4c hbg, Vec4c sbg , TickContext* ctx);

#endif
