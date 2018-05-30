#ifndef DRAWFONTS_H
#define DRAWFONTS_H

#ifdef CreateFont
#undef CreateFont
#endif

#ifdef GetCharABCwide
#undef GetCharABCwide
#endif

struct fonts_api_t
{
	void	( *Init )				( void );		// Don't use it !
	void	( *Shutdown )			( void );		// Don't use it !
	void	( *DrawSetTextFont )	( int font );
	void	( *DrawSetTextColor )	( int r, int g, int b, int a );
	void	( *DrawSetTextPos )		( int x, int y );
	void	( *DrawGetTextPos )		( int *x, int *y );
	void	( *DrawPrintText )		( const wchar_t *text );
	void	( *DrawUnicodeChar )	( wchar_t wch );
	int		( *CreateFont )			( void );
	bool	( *AddGlyphSetToFont )	( int font, const char *windowsFontName, int tall, int weight, int blur, int scanlines, int flags, int lowRange, int highRange );
	bool	( *AddCustomFontFile )	( const char *fontFileName );
	int		( *GetFontTall )		( int font );
	void	( *GetCharABCwide )		( int font, int ch, int *a, int *b, int *c );
	int		( *GetCharacterWidth )	( int font, int ch );
	void	( *GetTextSize )		( int font, const wchar_t *text, int *wide, int *tall );
	int		( *GetFontAscent )		( int font, wchar_t wch );
	int		( *GetFontFlags )		( int font );
};

#endif