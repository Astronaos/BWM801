#include <bwm801.h>
#define OGLEXT_DEF
#include <bwm801_glext.h>

using namespace bwm801;

void glGenerateMipmap(GLenum i_eTarget)
{
	if (g_lpf_glGenerateMipmap != nullptr)
	{
		g_lpf_glGenerateMipmap(i_eTarget);
	}
}
void glGenerateTextureMipmap(GLuint i_uiTexture)
{
	if (g_lpf_glGenerateTextureMipmap != nullptr)
	{
		g_lpf_glGenerateTextureMipmap(i_uiTexture);
	}
}
