#include <bwm801.h>
#include <string>
#include <png.h>
#include <cstring>
#include <iostream>

// for now, assume pages are 4096 bytes each. @@TODO implement per machine page sizing
#define ASSUMED_PAGE_SIZE 4096
using namespace bwm801;


class tex_buffer
{
public:
	char * m_chTexture_Buffer;
	size_t m_tTexture_Buffer_Size;
	tex_buffer(void)
	{
		m_chTexture_Buffer = nullptr;
		m_tTexture_Buffer_Size = 0;
	}
	tex_buffer(const tex_buffer &i_cRHO)
	{
		m_chTexture_Buffer = new char[i_cRHO.m_tTexture_Buffer_Size];
		m_tTexture_Buffer_Size = i_cRHO.m_tTexture_Buffer_Size;
		memcpy(m_chTexture_Buffer, i_cRHO.m_chTexture_Buffer, m_tTexture_Buffer_Size);
	}
	void realloc(size_t i_tDesired_Size)
	{
		if (i_tDesired_Size > m_tTexture_Buffer_Size)
		{
			if (m_chTexture_Buffer != nullptr)
				delete[] m_chTexture_Buffer;
			m_tTexture_Buffer_Size = (i_tDesired_Size / ASSUMED_PAGE_SIZE);
			if ((i_tDesired_Size % ASSUMED_PAGE_SIZE) != 0)
				m_tTexture_Buffer_Size++;
			m_tTexture_Buffer_Size *= ASSUMED_PAGE_SIZE;
			m_chTexture_Buffer = new char[m_tTexture_Buffer_Size];
		}
	}
	~tex_buffer(void)
	{
		if (m_chTexture_Buffer != nullptr)
		{
			delete[] m_chTexture_Buffer;
		}
		m_chTexture_Buffer = nullptr;
		m_tTexture_Buffer_Size = 0;
	}

};
tex_buffer g_cCore_Texture_Buffer;

//	GLuint m_uiTexture_ID;
bool Read_PNG(const std::string & i_sFile_Path, size_t & o_tWidth, size_t & o_tHeight, size_t & o_iNum_Colors)
{
	bool bRet; // defined for each if ... else becase libpng insists on requiring setjmp
	if (!i_sFile_Path.empty())
	{
		FILE *fileImage = fopen(i_sFile_Path.c_str(), "rb");
		if (fileImage != nullptr)
		{
			char lpchHeader[8];
				fread(lpchHeader, 1, 8, fileImage);
			int tiIs_PNG = !png_sig_cmp((png_const_bytep)lpchHeader, 0, 8);
			if (tiIs_PNG != 0)
			{
				fseek(fileImage, 0, SEEK_SET); // rewind to start of file
				png_structp pngsRead_Struct = png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
				if (pngsRead_Struct != nullptr)
				{
					png_infop pngsInfo_Struct = png_create_info_struct(pngsRead_Struct);
					png_infop pngsEnd_Info_Struct = nullptr;
					if (pngsInfo_Struct != nullptr)
					{
						pngsEnd_Info_Struct = png_create_info_struct(pngsRead_Struct);
						if (pngsEnd_Info_Struct != nullptr)
						{
							if (setjmp(png_jmpbuf(pngsRead_Struct)) == 0)
							{
								bRet = false;
								png_init_io(pngsRead_Struct, fileImage);
								png_read_png(pngsRead_Struct, pngsInfo_Struct, PNG_TRANSFORM_STRIP_16 | PNG_TRANSFORM_PACKING | PNG_TRANSFORM_EXPAND, nullptr);
								png_bytepp lpRow_Pointers = png_get_rows(pngsRead_Struct, pngsInfo_Struct);
								if (lpRow_Pointers != nullptr)
								{
									png_uint_32 uiWidth, uiHeight;
									int iBit_Depth, iColor_Type, iInterlace_Type;
									png_get_IHDR(pngsRead_Struct, pngsInfo_Struct, &uiWidth, &uiHeight, &iBit_Depth, &iColor_Type, &iInterlace_Type, nullptr, nullptr);
									// we now have all of the data that we need for creating the texture
									size_t tData_Size;
									switch (iColor_Type)
									{
									case PNG_COLOR_TYPE_GRAY:
										o_iNum_Colors = 1;
										break;
									case PNG_COLOR_TYPE_GRAY_ALPHA:
										o_iNum_Colors = 2;
										break;
									default:
									case PNG_COLOR_TYPE_RGB:
										o_iNum_Colors = 3;
										break;
									case PNG_COLOR_TYPE_RGB_ALPHA:
										o_iNum_Colors = 4;
										break;
									}
									size_t tRow_Len = o_iNum_Colors * uiWidth;
									tData_Size = o_iNum_Colors * uiWidth * uiHeight;
									g_cCore_Texture_Buffer.realloc(tData_Size);
									size_t tIdx = 0;
									for (size_t tI = uiHeight - 1; tI < uiHeight; tI--)
									{
										memcpy(g_cCore_Texture_Buffer.m_chTexture_Buffer + tIdx, lpRow_Pointers[tI], tRow_Len);
										tIdx += tRow_Len;
									}
									o_tWidth = uiWidth;
									o_tHeight = uiHeight;
									bRet = true; // success
								}
								else
									bRet = false;
							}
							else
								bRet = false;
						}
						else
							bRet = false;
					}
					else
						bRet = false;
					png_destroy_read_struct(&pngsRead_Struct, &pngsInfo_Struct, &pngsEnd_Info_Struct);
				}
				else
					bRet = false;
			}
			else
				bRet = false;
			fclose(fileImage);
			fileImage = nullptr;
		}
		else
			bRet = false;
	}
	else
		bRet = false;
	return bRet;
}

void texture::Initializer(void)
{
	m_uiTexture_ID = (unsigned int)-1;
	m_tWidth = (size_t)-1;
	m_tHeight = (size_t)-1;

	m_iParam_Wrap_S = GL_CLAMP_TO_EDGE;
	m_iParam_Wrap_T = GL_CLAMP_TO_EDGE;
	m_iParam_Min_Filter = GL_LINEAR;
	m_iParam_Mag_Filter = GL_LINEAR;
	m_lpfParam_Border_Color[0] = 0.0;
	m_lpfParam_Border_Color[1] = 0.0;
	m_lpfParam_Border_Color[2] = 0.0;
	m_lpfParam_Border_Color[3] = 0.0;

}

void texture::Destructor(void)
{
	//Delete(); // don't delete! Because the texture is a class the texture may be copied through multiple instances. We don't want to delete the texture when a copy is destroyed
}

void texture::Load_Image(const std::string & i_sFile_Path, int i_iMipmap_Level)
{
	if (i_sFile_Path.size() > 4) // it at least needs to be called '.png'
	{
		bool bSuccess = false;
		size_t tWidth, tHeight, tColors;
		if (i_sFile_Path.substr(i_sFile_Path.size() - 4, 4) == ".png" ||
			i_sFile_Path.substr(i_sFile_Path.size() - 4, 4) == ".PNG")
		{
			bSuccess = Read_PNG(i_sFile_Path, tWidth, tHeight, tColors);
		}
		else if (i_sFile_Path.substr(i_sFile_Path.size() - 4, 4) == ".jpg" ||
			i_sFile_Path.substr(i_sFile_Path.size() - 4, 4) == ".JPG" ||
			i_sFile_Path.substr(i_sFile_Path.size() - 5, 5) == ".jpeg" ||
			i_sFile_Path.substr(i_sFile_Path.size() - 5, 5) == ".JPEG")
		{
			std::cerr << ".jpg file format not currently supported (" << i_sFile_Path << ")" << std::endl;
		}
		
		if (bSuccess)
		{
			m_tWidth = tWidth;
			m_tHeight = tHeight;

			if (m_uiTexture_ID == (unsigned int)-1)
				glGenTextures(1, &m_uiTexture_ID);
			glBindTexture(GL_TEXTURE_2D, m_uiTexture_ID);
			// set the texture wrapping/filtering options (on the currently bound texture object)
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, m_iParam_Wrap_S);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, m_iParam_Wrap_T);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, m_iParam_Min_Filter);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, m_iParam_Mag_Filter);
			if (m_iParam_Wrap_S == GL_CLAMP_TO_BORDER || m_iParam_Wrap_T == GL_CLAMP_TO_BORDER)
				glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, m_lpfParam_Border_Color);

			GLenum eColor;
			switch (tColors)
			{
			case 1:
				eColor = GL_RED;
				break;
			case 3:
			default:
				eColor = GL_RGB;
				break;
			case 4:
				eColor = GL_RGBA;
				break;
			}

			// load and generate the texture
			if (g_cCore_Texture_Buffer.m_chTexture_Buffer != nullptr)
			{
				if (i_iMipmap_Level == -1)
				{
					glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, tWidth, tHeight, 0, eColor, GL_UNSIGNED_BYTE, g_cCore_Texture_Buffer.m_chTexture_Buffer);
					glGenerateMipmap(GL_TEXTURE_2D);
				}
				else
				{
					glTexImage2D(GL_TEXTURE_2D, i_iMipmap_Level, GL_RGBA, tWidth, tHeight, 0, eColor, GL_UNSIGNED_BYTE, g_cCore_Texture_Buffer.m_chTexture_Buffer);
				}
			}
		}
		else
		{
			std::cerr << "Failed to load texture " << i_sFile_Path << std::endl;
		}
	}
}
void texture::Apply(void) const
{
	if (m_uiTexture_ID != (unsigned int)-1)
	{
		glBindTexture(GL_TEXTURE_2D, m_uiTexture_ID);
		// set the texture wrapping/filtering options (on the currently bound texture object)
//		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, m_iParam_Wrap_S);
//		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, m_iParam_Wrap_T);
//		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, m_iParam_Min_Filter);
//		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, m_iParam_Mag_Filter);
//		if (m_iParam_Wrap_S == GL_CLAMP_TO_BORDER || m_iParam_Wrap_T == GL_CLAMP_TO_BORDER)
//			glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, m_lpfParam_Border_Color);
	}
}
void texture::Delete(void)
{
	if (m_uiTexture_ID != (unsigned int)-1)
	{
		glDeleteTextures(1, &m_uiTexture_ID);
		m_uiTexture_ID = (unsigned int)-1;
	}
}

void texture::Set_Border_Color(const GLfloat & i_fRed, const GLfloat & i_fGreen, const GLfloat & i_fBlue, const GLfloat & i_fAlpha)
{
	m_lpfParam_Border_Color[0] = i_fRed;
	m_lpfParam_Border_Color[1] = i_fGreen;
	m_lpfParam_Border_Color[2] = i_fBlue;
	m_lpfParam_Border_Color[3] = i_fAlpha;

}
void texture::Set_Parameter(GLenum i_eParam, GLint i_iValue)
{
	switch (i_eParam)
	{
	default: break;
	case GL_TEXTURE_WRAP_S:
		m_iParam_Wrap_S = i_iValue;
		break;
	case GL_TEXTURE_WRAP_T:
		m_iParam_Wrap_T = i_iValue;
		break;
	case GL_TEXTURE_MIN_FILTER:
		m_iParam_Min_Filter = i_iValue;
		break;
	case GL_TEXTURE_MAG_FILTER:
		m_iParam_Mag_Filter = i_iValue;
		break;
	}
}

size_t texture::Get_Width(void) const
{
	return m_tWidth;
}
size_t texture::Get_Height(void) const
{
	return m_tHeight;
}


