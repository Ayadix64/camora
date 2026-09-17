#include "utils.h"
#include <GLFW/glfw3.h>

#include "../include/tick-tack-to.h"
extern  TickContext g_defaultContext;

extern  TickFont g_defaultFont;
#define DEF_XPADD 1
#define DEF_YPADD 4


Vec4c g_defaultBackgroundColor;
Vec4c g_defaultSlecetColor;
Vec4c g_defaultHoverColor;
Vec4c g_defaultFontColor;
u32 g_Roundness=30;





void ResetDefaultUIColors(){
	g_defaultBackgroundColor = UI_DEFAULT_BACKGROUND_COLOR;
	g_defaultHoverColor      = UI_DEFAULT_HOUVER_COLOR    ;
	g_defaultSlecetColor     = UI_DEFAULT_SELECT_COLOR    ;
	g_defaultFontColor       = UI_DEFAULT_FONT_COLOR      ;
};




void InitUI(){
	ResetDefaultUIColors();
}


void SetDefaultUIColors(Vec4c dbg, Vec4c hc, Vec4c sc , Vec4c fc){
	g_defaultBackgroundColor=dbg;
	g_defaultHoverColor=hc;
	g_defaultSlecetColor=sc;
	g_defaultFontColor=fc;

}


bool IsSlected(TickContext* ctx){
	return ctx->selectID==ctx->selectCount;
}

void UpdateSelect(bool selected, bool highlited,TickContext* ctx){
	if(selected){
		ctx->selectID=ctx->selectCount;
	}
	if(highlited){
		ctx->highlightedElement=ctx->samplerCount;
	}
	ctx->selectCount++;
}

void DiscardSelect(TickContext* ctx){
	ctx->selectID=-1;
}


bool Hover(int x , int y , int w , int h){
	return (int)GetMousePos().x>=x && (int)GetMousePos().x <=w+x &&
	       (int)GetMousePos().y>=y && (int)GetMousePos().y <= y+h && !(GetMouseClickes()&(1<<3));
}
bool Hover_ctx(int x , int y , int w , int h, TickContext* ctx){
	
	return (int)GetMousePos_ctx(ctx).x>=x && (int)GetMousePos_ctx(ctx).x <=w+x &&
	       (int)GetMousePos_ctx(ctx).y>=y && (int)GetMousePos_ctx(ctx).y <= y+h && !(GetMouseClickes_ctx(ctx)&(1<<3));
}


bool Clicked(int x , int y , int w , int h){
	return (int)GetMousePos().x>=x && (int)GetMousePos().x <=w+x &&
	       (int)GetMousePos().y>=y && (int)GetMousePos().y <= y+h && GetMouseClickes()&(1<<3);
}
bool Clicked_ctx(int x , int y , int w , int h, TickContext* ctx){
	
	return (int)GetMousePos_ctx(ctx).x>=x && (int)GetMousePos_ctx(ctx).x <=w+x &&
	       (int)GetMousePos_ctx(ctx).y>=y && (int)GetMousePos_ctx(ctx).y <= y+h && GetMouseClickes_ctx(ctx)&(1<<3);
}

bool ClickedAndReleased(int x , int y , int w , int h){
	return (int)GetMousePos().x>=x && (int)GetMousePos().x <=w+x &&
	       (int)GetMousePos().y>=y && (int)GetMousePos().y <= y+h && GetMouseClickes()&1;
}
bool ClickedAndReleased_ctx(int x , int y , int w , int h, TickContext* ctx){
	
	return (int)GetMousePos_ctx(ctx).x>=x && (int)GetMousePos_ctx(ctx).x <=w+x &&
	       (int)GetMousePos_ctx(ctx).y>=y && (int)GetMousePos_ctx(ctx).y <= y+h && GetMouseClickes_ctx(ctx)&1;
}

bool DoubleClicked(int x , int y , int w , int h){
	return (int)GetMousePos().x>=x && (int)GetMousePos().x <=w+x &&
	       (int)GetMousePos().y>=y && (int)GetMousePos().y <= y+h && GetMouseClickes()&(1<<3);
}
bool DoubleClicked_ctx(int x , int y , int w , int h, TickContext* ctx){
	return (int)GetMousePos_ctx(ctx).x>=x && (int)GetMousePos_ctx(ctx).x <=w+x &&
	       (int)GetMousePos_ctx(ctx).y>=y && (int)GetMousePos_ctx(ctx).y <= y+h && GetMouseClickes_ctx(ctx)&(1<<3);
}



/**************************************** BUTTONS ********************************************/





char Button              (const char* text,int x , int y , int w /*0 or -1 for default*/ , int h /*0 or -1 for default*/)//bit1: preased, bit2: hover

{
	return ButtonExtended_ctx(text, x,y, w, h, g_defaultBackgroundColor, g_defaultHoverColor, g_defaultSlecetColor,g_defaultFontColor,
			g_defaultFont, DEF_XPADD, DEF_YPADD, &g_defaultContext);
} 
char Button_ctx          (const char* text,int x , int y ,int w , int h ,TickContext* ctx)
{
	return ButtonExtended_ctx(text, x,y, w, h, g_defaultBackgroundColor, g_defaultHoverColor, g_defaultSlecetColor,g_defaultFontColor,
			g_defaultFont, DEF_XPADD, DEF_YPADD, ctx);

} 
char ButtonColor        (const char* text,int x , int y ,int w , int h ,Vec4c dbg, Vec4c hbg, Vec4c sbg,Vec4c tc)
{
	return ButtonExtended_ctx(text, x,y, w, h, dbg, hbg, sbg,tc,
			g_defaultFont, DEF_XPADD, DEF_YPADD, &g_defaultContext);

}
char ButtonColor_ctx    (const char* text,int x , int y ,int w , int h ,Vec4c dbg, Vec4c hbg, Vec4c sbg,Vec4c tc,TickContext*ctx)
{
	return ButtonExtended_ctx(text, x,y, w, h, dbg, hbg, sbg,tc,
			g_defaultFont, DEF_XPADD, DEF_YPADD, ctx);


} 
char ButtonFont          (const char* text,int x , int y ,int w , int h ,TickFont font ,u32 xpadd, u32 ypadd)
{
	return ButtonExtended_ctx(text, x,y, w, h, g_defaultBackgroundColor, g_defaultHoverColor, g_defaultSlecetColor,g_defaultFontColor,
			font, xpadd, ypadd, &g_defaultContext);

}
char ButtonFont_ctx      (const char* text,int x , int y ,int w , int h ,TickFont font ,u32 xpadd, u32 ypadd, TickContext*ctx)
{
	return ButtonExtended_ctx(text, x,y, w, h, g_defaultBackgroundColor, g_defaultHoverColor, g_defaultSlecetColor,g_defaultFontColor,
			font, xpadd, ypadd, ctx);

} 
char ButtonExtended      (const char* text,int x , int y ,int w , int h ,Vec4c dbg, Vec4c hbg, Vec4c sbg,Vec4c tc, TickFont font ,u32 xpadd ,u32 ypadd)
{
	return ButtonExtended_ctx  (text,x , y , w , h ,dbg, hbg, sbg,tc, font , xpadd, ypadd,&g_defaultContext);

} 


char ButtonExtended_ctx  (const char* text,int x , int y , int w , int h ,Vec4c dbg, Vec4c hbg, Vec4c sbg,Vec4c tc, TickFont font , u32 xpadd, u32 ypadd,TickContext*ctx)
{
	char preased = false;
	bool highlited=false;
	bool slected = IsSlected(ctx);
	if(slected){highlited=true;};
	

	u32* ww = (w&&w!=-1)?0:(u32*)&w;
	u32 th = 0;//h?(h!=-1?0:(u32*)&h):(u32*)&h;
	GetTextDemensionsExtended(text,xpadd,ypadd,ww, &th);
	ww?w+=30:0;
	(h==-1||!h)?h=th+30:0;
	if(slected){
		DrawEmptyRoundedRectangel_ctx(x-1, y-1, w + 2, h + 2, 10,2 ,90,  hbg,ctx);

	}

	if(Hover_ctx(x, y, w, h,ctx)){
		DrawRoundedRectangel_ctx(x, y, w , h, 10, 90, hbg,ctx);
		preased|=2;
	}
 	else if(Clicked_ctx(x, y, w, h,ctx) || (slected && IsKeyPressed_ctx(GLFW_KEY_ENTER,ctx))){
		DrawRoundedRectangel_ctx(x, y, w , h , 10, 90, sbg,ctx);
		preased|=2;
		
	}
	if(ClickedAndReleased_ctx( x, y, w, h,ctx) || (slected && IsKeyPressed_ctx(GLFW_KEY_ENTER,ctx))){
		DrawRoundedRectangel_ctx(x, y, w , h , 10, 90, sbg,ctx);
		preased|=1;
	}
	if(!preased) {
		DrawRoundedRectangel_ctx(x, y, w , h , 10, 90, dbg,ctx);

	}
	
	if(!Clicked_ctx( x, y, w, h,ctx)&& GetMouseClickes_ctx(ctx)&1 && slected){
		DiscardSelect(ctx);
		slected=false;
	}
	if(preased&1){slected=1;}	
	DrawTextFontExtended_ctx(text, x+15,  y+(h-th)/2,xpadd,ypadd,tc,font,ctx);
	
	UpdateSelect(slected&1, highlited, ctx);
	return preased;
}








/***************************************  TEXTBOX ***********************************************/

void InitTextBoxData(TextBoxData* tbd,u32 maxsize){
	tbd->size=1024;
	tbd->pos=0;
	tbd->maxsize=maxsize?maxsize:-1;
	tbd->flags=(TickTextBoxFlags){.EnableNumbers=1,.EnbleCharctures=1};
	tbd->xoffset=0;
	tbd->data=(char*)malloc(tbd->size);
	tbd->usedsize=0;
	memset(tbd->data, 0, tbd->size);
}

char TextBox            (int x , int y , int w,int h/*0 or-1 for default*/,   TextBoxData* tbd)
{
	return TextBoxExtended_ctx(x, y, w, h, tbd, g_defaultBackgroundColor, g_defaultHoverColor, g_defaultSlecetColor, (Vec4c){255,255,255,255},g_defaultFontColor,
			g_defaultFont, DEF_XPADD, DEF_YPADD, &g_defaultContext);
}
char TextBox_ctx        (int x , int y , int w , int h/*0 or-1 for default*/, TextBoxData* tbd,TickContext*ctx)
{
	return TextBoxExtended_ctx(x, y, w, h, tbd, g_defaultBackgroundColor, g_defaultHoverColor, g_defaultSlecetColor, (Vec4c){255,255,255,255},g_defaultFontColor,
			g_defaultFont, DEF_XPADD, DEF_YPADD, ctx);

}
char TextBoxColor      (int x , int y , int w , int h, TextBoxData* tbd, Vec4c bg, Vec4c hbg , Vec4c sbg, Vec4c cursurCl,Vec4c tc)
{
	return TextBoxExtended_ctx(x, y, w, h, tbd, bg, hbg, sbg, cursurCl,tc,
			g_defaultFont, DEF_XPADD, DEF_YPADD, &g_defaultContext);

}
char TextBoxColor_ctx  (int x , int y , int w , int h, TextBoxData* tbd, Vec4c bg, Vec4c hbg , Vec4c sbg, Vec4c cursurCl,Vec4c tc,TickContext*ctx)
{
	return TextBoxExtended_ctx(x, y, w, h, tbd, bg, hbg, sbg, cursurCl,tc,
			g_defaultFont, DEF_XPADD, DEF_YPADD, ctx);

}
char TextBoxFont        (int x , int y , int w , int h, TextBoxData* tbd, TickFont font, u32 xpadd, u32 ypadd)
{
	return TextBoxExtended_ctx(x, y, w, h, tbd, g_defaultBackgroundColor, g_defaultHoverColor, g_defaultSlecetColor, (Vec4c){255,255,255,255},g_defaultFontColor,
			font, xpadd, ypadd, &g_defaultContext);

}
char TextBoxFont_ctx    (int x , int y , int w , int h, TextBoxData* tbd, TickFont font, u32 xpadd, u32 ypadd, TickContext*ctx){
	return TextBoxExtended_ctx(x, y, w, h, tbd, g_defaultBackgroundColor, g_defaultHoverColor, g_defaultSlecetColor, (Vec4c){255,255,255,255},g_defaultFontColor,
			font, xpadd, ypadd, ctx);

}
char TextBoxExtended    (int x , int y , int w , int h, TextBoxData* tbd, Vec4c bg, Vec4c hbg , Vec4c sbg,Vec4c tc, Vec4c cursurCl,TickFont font, u32 xpadd, u32 ypadd)
{
	return TextBoxExtended_ctx(x, y, w, h, tbd, bg, hbg, sbg, cursurCl,g_defaultFontColor,
			font, xpadd, ypadd, &g_defaultContext);

}


char TextBoxExtended_ctx(int x , int y , int w , int h, TextBoxData* tbd, Vec4c bg, Vec4c hbg , Vec4c sbg, Vec4c cursurCl,Vec4c tc,TickFont font, u32 xpadd, u32 ypadd,TickContext*ctx)
{
	char preased = false;
	bool highlited=false;
	bool slected = IsSlected(ctx);
	if(slected){highlited=true;};
	if(!h || h==-1){	
		h=font.linegap+30;
	}
	if(slected){
		DrawRoundedRectangel_ctx(x-1, y-1, w+2, h +2, 10, 90,  hbg,ctx);

	}

	if(Hover(x, y, w, h)){
		DrawRoundedRectangel_ctx(x, y, w , h , 10, 90, hbg,ctx);
		preased|=2;
	}
	else if(Clicked(x, y, w, h) || (slected && IsKeyPressed_ctx(GLFW_KEY_ENTER,ctx))){
		DrawRoundedRectangel_ctx(x, y, w , h , 10, 90, sbg,ctx);	
		preased|=2;
	}
	if(ClickedAndReleased( x, y, w+30, h+30) || (slected && IsKeyPressed_ctx(GLFW_KEY_ENTER,ctx))){
		DrawRoundedRectangel_ctx(x, y, w , h , 10, 90, sbg,ctx);
		preased|=1;
	}
	if(!preased){
		DrawRoundedRectangel_ctx(x, y, w , h , 10, 90, bg,ctx);
	}
	
	if(preased&1){slected=1;}	
	if(!preased&& GetMouseClickes_ctx(ctx)&(1<<3) && slected){
		DiscardSelect(ctx);
		slected=false;
	}
	UpdateSelect(slected, highlited, ctx);
	

	
	u32 curserpos =0;
	GetFontTextDemensionsExtendedSize(tbd->data, tbd->pos, font,xpadd, ypadd, &curserpos, 0);
	



	curserpos-=tbd->xoffset;
	

	
	if(slected){
		if((u64)(glfwGetTime()*4.0)%2){ //aka; flicker evry half a secend
			DrawRectangel_ctx(x+15+curserpos, y+ (h-font.linegap)/2 - 2, xpadd, font.linegap+4, cursurCl,ctx);
		}	
	}else {
		DrawRectangel_ctx(x+15+curserpos, y+ (h-font.linegap)/2 - 2, xpadd, font.linegap+4, cursurCl,ctx);
	}
	if(!tbd->flags.Password){
		DrawTextSegmentExtendedFontSize_ctx(tbd->data, tbd->usedsize, x+15-tbd->xoffset, y+ (h-font.linegap)/2, tbd->xoffset, 0, w-30, h,xpadd,xpadd,tc,font,ctx);
	}else {
		//TODO
	}
	
	u32 keypreased = GetLastKey_ctx(ctx);

	if((keypreased>=32 || keypreased=='\t') && tbd->size < tbd->maxsize && slected){
		size_t sz=tbd->size,usz=tbd->usedsize;
		tbd->data=(char*)PushChar(keypreased, tbd->pos, &sz, (size_t*)&usz, tbd->data);
		tbd->size=sz;
		tbd->usedsize=usz;
		tbd->pos++;
		u32 cw;
		GetFontCharDemensions(keypreased,font, &cw, 0);
		if(curserpos+cw+30> w){

			tbd->xoffset+=cw+xpadd;
		}
	}else if(GetKeyPressed_ctx(GLFW_KEY_BACKSPACE,ctx) && tbd->pos&& slected){
		tbd->pos--;
		u32 cw;
		GetFontCharDemensions(tbd->data[tbd->pos],font, &cw, 0);
		
		if((int)(curserpos-(cw+xpadd))<=5){ // to show if the user delet all or still
			tbd->xoffset-= min((cw+xpadd),tbd->xoffset);
		}

		size_t usz=tbd->usedsize;
		PopChar(tbd->pos, &usz, tbd->data);
		tbd->usedsize=usz;
	}else if(GetKeyPressed_ctx(GLFW_KEY_LEFT,ctx) && tbd->pos && slected){
		
		tbd->pos--;
		u32 cw;
		
		GetCharDemensions(tbd->data[tbd->pos], &cw, 0);
		
		if((int)(curserpos-(cw+xpadd))<=0){
			tbd->xoffset-= min((cw+xpadd),tbd->xoffset); // so we are not sub zero
		}

		
	}else if(GetKeyPressed_ctx(GLFW_KEY_RIGHT,ctx) && tbd->pos < tbd->usedsize && slected){
		u32 cw;
		GetCharDemensions(tbd->data[tbd->pos], &cw, 0);
		
		tbd->pos++;
		if((int)(curserpos)>=(int)(w-cw-xpadd-30)){
			tbd->xoffset+=cw+xpadd;// so we are not sub zero
		}

	}
	


	
	return preased;
}






/***************************************** CheckBox *****************************************/




void CheckBox(const char* bx ,int x , int y , char* b){
	CheckBoxExtended_ctx(bx, x, y, b, g_defaultBackgroundColor,
			g_defaultHoverColor, g_defaultSlecetColor,g_defaultFontColor,g_defaultFont , DEF_XPADD, DEF_YPADD, &g_defaultContext);	
	
	return;
}


void CheckBox_ctx(const char* bx ,int x , int y , char* b,TickContext* ctx){
		
	CheckBoxExtended_ctx(bx, x, y, b, g_defaultBackgroundColor,
			g_defaultHoverColor, g_defaultSlecetColor,g_defaultFontColor,g_defaultFont , DEF_XPADD, DEF_YPADD, ctx);
	
	return;
}


void CheckBoxColor(const char* bx ,int x , int y , char* b,Vec4c dbg, Vec4c hbg, Vec4c sbg,Vec4c tc){
	CheckBoxExtended_ctx(bx, x, y, b, dbg,
			hbg, sbg,tc,g_defaultFont , DEF_XPADD, DEF_YPADD, &g_defaultContext);

	return;
}


void CheckBoxColor_ctx(const char* bx ,int x , int y , char* b,Vec4c dbg, Vec4c hbg, Vec4c sbg,Vec4c tc,TickContext* ctx){
	CheckBoxExtended_ctx(bx, x, y, b, dbg,tc,
			 hbg, sbg,g_defaultFont , DEF_XPADD, DEF_YPADD, ctx);

	return;
}


void CheckBoxFont(const char* bx ,int x , int y , char* b,TickFont font, u32 xpadd, u32 ypadd){
	CheckBoxExtended_ctx(bx, x, y, b, g_defaultBackgroundColor
			, g_defaultHoverColor, g_defaultSlecetColor,g_defaultFontColor,font , xpadd, ypadd, &g_defaultContext);

	return;
}


void CheckBoxFont_ctx(const char* bx ,int x , int y , char* b,TickFont font, u32 xpadd, u32 ypadd,TickContext* ctx){
	CheckBoxExtended_ctx(bx, x, y, b, g_defaultBackgroundColor
			, g_defaultHoverColor, g_defaultSlecetColor,g_defaultFontColor,font , xpadd, ypadd, ctx);

	return;
}


void CheckBoxExtended(const char* bx ,int x , int y , char* b,Vec4c dbg, Vec4c hbg, Vec4c sbg,Vec4c tc, TickFont font , u32 xpadd, u32 ypadd){
	CheckBoxExtended_ctx(bx ,x , y , b,dbg, hbg, sbg,tc, font , xpadd, ypadd, &g_defaultContext);
}


void CheckBoxExtended_ctx(const char* bx ,int x , int y , char* b,Vec4c dbg, Vec4c hbg, Vec4c sbg,Vec4c tc, TickFont font , u32 xpadd, u32 ypadd, TickContext* ctx){
	char preased = false;
	bool highlited=false;
	
	bool slected = IsSlected(ctx);
	if(slected){highlited=true;};
	
	u32 th=font.linegap;
	
	if(slected){
		DrawRoundedRectangel_ctx(x-1, y-1, th+2, th+2, 5, 90,  dbg,ctx);
	}

	if(Hover_ctx(x, y, th, th,ctx) && !*b){
		DrawRoundedRectangel_ctx(x, y, th , th, 5, 90, hbg,ctx);
		preased|=2;
	}
	else if(Clicked_ctx(x, y, th, th,ctx)){
		DrawRoundedRectangel_ctx(x, y, th ,th, 5, 90, sbg,ctx);
		preased|=2;
		
	}
	if(ClickedAndReleased_ctx( x, y, th, th,ctx) || (slected && GetKeyPressed_ctx(GLFW_KEY_ENTER,ctx))){
		DrawRoundedRectangel_ctx(x, y, th ,th, 5, 90, sbg,ctx);
		preased|=1;
	}
	if(!preased){
		if(*b){
			DrawRoundedRectangel_ctx(x, y, th , th , 5, 90, sbg,ctx);
		}else {
			DrawRoundedRectangel_ctx(x, y, th , th , 5, 90, dbg,ctx);
		}
	}
	
	if(preased&1){slected=1;}	

	if(!preased&& GetMouseClickes_ctx(ctx)&(1<<3) && slected){
		DiscardSelect(ctx);
		slected=false;
	}
	UpdateSelect(slected, highlited, ctx);
	

	//DrawTextExtended(bx, x+10+th, y, DEF_XPADD, DEF_XPADD);
	DrawTextFontExtended(bx, x+th+10, y, xpadd, ypadd, tc,font);
	if(preased&1){
		*b=!*b;
	}

	
	return;	
}




/****************************************** Slider ************************************************/




char Slider(int x , int y , int w,  float* s)
{
	return SliderColor_ctx(x, y, w, s, g_defaultBackgroundColor, g_defaultHoverColor, g_defaultSlecetColor, &g_defaultContext);
}
char Slider_ctx(int x , int y , int w,  float* s, TickContext* ctx)
{
	return SliderColor_ctx(x, y, w, s, g_defaultBackgroundColor, g_defaultHoverColor, g_defaultSlecetColor, ctx);
}

char SliderColor(int x , int y , int w,  float* s ,Vec4c dbg, Vec4c hbg, Vec4c sbg){
		return SliderColor_ctx(x, y, w, s, dbg, hbg, sbg, &g_defaultContext);
}


char SliderColor_ctx(int x , int y , int w,  float* s ,Vec4c dbg, Vec4c hbg, Vec4c sbg , TickContext* ctx)
{
	char preased = false;
	bool highlited=false;
		
	bool slected = IsSlected(ctx);
	if(slected){highlited=true;};
	
	float _s = *s>=0.0f?*s<=1.0f?*s:1.0f:0.0f;//very indrstundabel , cheks if 0.0 <= *s <= 1.0, if it isnt, set to eather 1.0 or 0.0
	DrawRoundedRectangel_ctx(x+10, y+2, w-20, 6, 3, 30, (Vec4c){50,50,60,255},ctx); //the slider
	if(slected){
		DrawEmptyCircle_ctx(x+_s*(w-20)+10, y+5, 10,1, 30,  hbg,ctx);
	}

	if(Hover_ctx(x, y, w, 20,ctx)){
		DrawCircle_ctx(x+_s*(w-20)+10, y+5, 10, 30,  hbg,ctx);
		preased|=2;
	}else 	
	if(Clicked_ctx(x, y, w, 20,ctx) || (slected && GetMouseClickes_ctx(ctx)&(1<<3)) /*aka, not realeased*/){
		DrawCircle_ctx(x+_s*(w-20)+10, y+5, 10, 30,  sbg,ctx);
		int offset = (int)GetMousePos_ctx(ctx).x - (int)(x+10);
		offset=offset>(int)(w-20)?(int)(w-20): offset<=0?0:offset;
		*s = ((float)(offset)/ (float)(w-20));
		preased|=1;
		
	}else {
		DrawCircle_ctx(x+_s*(w-20)+10, y+5, 10, 30,  dbg,ctx);
		if(slected){
			DiscardSelect(ctx);
			slected=false;
		}
	}
	
		
	if(preased&1){slected=1;}	

	if(!preased&& GetMouseClickes()&(1<<3) && slected){
	}
	UpdateSelect(slected, highlited, ctx);
	

	return preased;
}
