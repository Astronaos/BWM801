#pragma once
#ifndef WIN32
#define WINAPI
#endif


#ifdef OGLEXT_DEF
# define EXTERN
#else
# define EXTERN extern
#endif
namespace bwm801
{
	typedef void (WINAPI * oglve) (GLenum  eEnum);
	typedef void (WINAPI * oglvui) (GLuint   uiUint);

	EXTERN oglve g_lpf_glGenerateMipmap;
	EXTERN oglvui g_lpf_glGenerateTextureMipmap;
};
#undef EXTERN
#ifndef WIN32
#undef WINAPI
#endif

