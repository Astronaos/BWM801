#include <bwm801.h>
#define OGLEXT_DEF
#include <bwm801_glext.h>


void bwm801::glGenerateMipmap(GLenum i_eTarget)
{
	if (bwm801::g_lpf_glGenerateMipmap != nullptr)
	{
		bwm801::g_lpf_glGenerateMipmap(i_eTarget);
	}
}
void bwm801::glGenerateTextureMipmap(GLuint i_uiTexture)
{
	if (bwm801::g_lpf_glGenerateTextureMipmap != nullptr)
	{
		bwm801::g_lpf_glGenerateTextureMipmap(i_uiTexture);
	}
}
