#ifndef ITEXTURERENDER_H
#define ITEXTURERENDER_H

#ifdef _WIN32
#pragma once
#endif

#include <interface.h>

#include "DrawTexture.h"
#include "DrawFonts.h"

class ITextureRender : public IBaseInterface
{
public:
	virtual texture_api_t *GetTextureAPI(void);
	virtual fonts_api_t *GetFontsAPI(void);
};

#define METAHOOK_TEXTURERENDER_VERSION "METAHOOK_TEXTURERENDER_VERSION001"

#endif