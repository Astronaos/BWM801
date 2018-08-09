#pragma once
#include <string>

#ifdef WIN32
#define EXPORT __declspec(dllexport)
#else
#define EXPORT
#endif

#ifndef GL_MIRRORED_REPEAT
#define GL_MIRRORED_REPEAT                0x8370
#define GL_CLAMP_TO_EDGE                  0x812F
#define GL_CLAMP_TO_BORDER                0x812D
#define GL_MIRROR_CLAMP_TO_EDGE           0x8743
#endif
namespace bwm801
{
	class EXPORT texture
	{
	private:
		GLuint m_uiTexture_ID;

		size_t m_tWidth;
		size_t m_tHeight;

		GLint m_iParam_Wrap_S;
		GLint m_iParam_Wrap_T;
		GLint m_iParam_Min_Filter;
		GLint m_iParam_Mag_Filter;
		GLfloat m_lpfParam_Border_Color[4];

	public:
		virtual void Initializer(void);
		virtual void Destructor(void);

		texture(void) {Initializer();}
		~texture(void) {Destructor();}
		virtual size_t Get_Width(void) const;
		virtual size_t Get_Height(void) const;

		virtual void Load_Image(const std::string & i_sFile_Path, int i_iMipmap_Level);
		virtual void Apply(void)  const;
		virtual void Delete(void);

		virtual void Set_Border_Color(const GLfloat & i_fRed, const GLfloat & i_fGreen, const GLfloat & i_fBlue, const GLfloat & i_fAlpha);
		virtual void Set_Parameter(GLenum i_eParam, GLint i_iValue);
	};
};

#undef EXPORT
