#pragma once
typedef void (WINAPI * oglve) (GLenum  eEnum);
typedef void (WINAPI * oglvui) (GLuint   uiUint);

#ifdef OGLEXT_DEF
# define EXTERN
#else
# define EXTERN extern
#endif

EXTERN oglve g_lpf_glGenerateMipmap;
EXTERN oglvui g_lpf_glGenerateTextureMipmap;
