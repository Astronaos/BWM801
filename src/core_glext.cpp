#include <core.hpp>
#define OGLEXT_DEF
#include <bwm801_glext.h>

void glGenerateMipmap(GLenum i_eTarget)
{
	if (g_lpf_glGenerateMipmap != nullptr)
	{
		g_lpf_glGenerateMipmap(i_eTarget);
	}
}
void glGenerateTextureMipmap(GLuint i_uiTexture)
{
	if (g_lpf_glGenerateMipmap != nullptr)
	{
		glGenerateTextureMipmap(i_uiTexture);
	}
}