#ifndef UTILS_TICKTACKTO
#define UTILS_TICKTACKTO

#ifndef UTILS_TICKTACKTO_CORE
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
#endif

#endif
