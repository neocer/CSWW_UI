#ifndef DRAWTEXTURE_H
#define DRAWTEXTURE_H

#include <triangleapi.h>

struct texcache_t
{
	char name[64];
	int wide;
	int tall;
	int texid;
	texcache_t *next;
};

enum
{
	RenderNormal = kRenderTransTexture,
	RenderAdditive	= kRenderTransAdd,
};

struct texture_api_t
{
	texcache_t*	( *LoadTGA )			( const char *name );			//╪сть
	texcache_t*	( *LoadBMP )			( const char *name );			//╪сть
	void		( *DrawSetColor )		( int r, int g, int b, int a );
	texcache_t*	( *ForName )			( const char *name );
	void		( *DrawSetTexture )		( texcache_t *tex );
	void		( *DrawTexturedRect )	( int x, int y, int wide, int tall );
	void		( *DrawTexturedRect4 )	( const float *v1, const float *v2, const float *v3, const float *v4 );
	void		( *TexCoords )			( float c1, float c2, float c3, float c4 );
	void		( *RenderMode )			( int mode );
	void		( *EnableScissor )		( int x, int y, int wide, int tall );
	void		( *DisableScissor )		( void );
};

#endif