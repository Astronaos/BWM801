#include <bwm801.h>
#include <bwm801_screenshot.h>
#include <bwm801_main_data.h>
#include <png.h>
#include <sstream>
#include <map>
#include <iostream>

#if !defined WIN32 && !defined __MINGW32__
#include <unistd.h>
namespace bwm801
{
	void Sleep(unsigned int i_uiLength)
	{
		usleep(i_uiLength * 1000);
	}
};
#endif

using namespace bwm801;


ogl_screenshot_info g_cOGL_Screenshot;

void ogl_screenshot_info::Initializer (void)
{
	m_bRequest = false;
	m_bReady = false;
	m_tWidth = 0;
	m_tHeight = 0;
	m_tColor_Depth_Bits = 0;
	m_lpvData = nullptr;
}

class main_screenshot_info
{
public:
	bool m_bRequest;
	bool m_bRequest_Ready;
	bool m_bPending;
	float m_fProgress;
	std::string m_szFilename;
	main_screenshot_info(void)
	{
		m_bRequest = false;
		m_bRequest_Ready = false;
		m_bPending = false;
		m_fProgress = 0.0;
		m_szFilename.clear();
	}

};

main_screenshot_info	g_cScreenshot;

void main::Request_Screenshot(const std::string & i_szFilename)
{
	if (!g_cScreenshot.m_bPending && !g_cScreenshot.m_bRequest)
	{
		g_cScreenshot.m_bRequest = true;
		if (!i_szFilename.empty())
			g_cScreenshot.m_szFilename = i_szFilename;
		else
		{
			std::vector<std::string> vszDir_Files = Get_Directory_File_List(((main_data*)m_lpvData)->m_szScreenshot_Default_Path);
			std::map<unsigned int, bool> mapExist_Files;
			bool bDone = false;
			for (auto iterI = vszDir_Files.begin(); !bDone && iterI != vszDir_Files.end(); iterI++)
			{
				if (iterI->size() > 4)
				{
					std::string szExt = iterI->substr(iterI->size() - 4,4);
					if (szExt == ".png" || szExt == ".PNG")
					{
						if (iterI->substr(0, ((main_data*)m_lpvData)->m_szScreenshot_Default_Filename.size()) == ((main_data*)m_lpvData)->m_szScreenshot_Default_Filename)
						{
							unsigned int uiCount = std::stoi(iterI->substr(((main_data*)m_lpvData)->m_szScreenshot_Default_Filename.size() + 1,8));
							mapExist_Files[uiCount] = 1;
						}
					}
				}
			}
			unsigned int uiI = 1;
			while (mapExist_Files.count(uiI) > 0)
				uiI++;
			
			std::ostringstream ossFilename;
			ossFilename << ((main_data*)m_lpvData)->m_szScreenshot_Default_Path;
			ossFilename << "/";
			ossFilename << ((main_data*)m_lpvData)->m_szScreenshot_Default_Filename;
			ossFilename << "-" << uiI << ".png";
			g_cScreenshot.m_szFilename = ossFilename.str();
		}
		g_cScreenshot.m_bRequest_Ready = true;
	}
}

void write_row_callback(png_structrp i_ppPng_Ptr, png_uint_32 i_uiRow, int i_iPass)
{
	if (g_cScreenshot.m_bPending && g_cOGL_Screenshot.m_bReady)
		g_cScreenshot.m_fProgress += (float)(1.0 / g_cOGL_Screenshot.m_tHeight); // or something
}

extern bwm801::main * g_lpMain;

void Screenshot_Loop(void)
{
	do
	{

		if (g_cScreenshot.m_bRequest && g_cScreenshot.m_bRequest_Ready)
		{
			g_cScreenshot.m_bPending = true;
			g_cOGL_Screenshot.m_bRequest = true;
			g_lpMain->Request_Refresh();
			std::string szFilename = g_cScreenshot.m_szFilename;
			g_cScreenshot.m_bRequest_Ready = false;
			g_cScreenshot.m_bRequest = false;
			g_cScreenshot.m_fProgress = 0.0;
			FILE *filePNG = fopen(szFilename.c_str(), "wb");
			if (filePNG != nullptr)
			{
				std::cout << "Writing screenshot to " << szFilename << std::endl;
				png_structp spngStruct = png_create_write_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
				if (spngStruct != nullptr)
				{
					png_set_write_status_fn(spngStruct, write_row_callback);
					png_infop pspngiInfo = png_create_info_struct(spngStruct);
					if (pspngiInfo != nullptr)
					{
						png_init_io(spngStruct, filePNG);

						png_set_write_status_fn(spngStruct, write_row_callback);

						while (!g_cOGL_Screenshot.m_bReady)
							Sleep(1); // wait on the image to be ready before procedding

						png_set_IHDR(spngStruct, pspngiInfo, g_cOGL_Screenshot.m_tWidth, g_cOGL_Screenshot.m_tHeight, g_cOGL_Screenshot.m_tColor_Depth_Bits / 3, PNG_COLOR_TYPE_RGB, PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_DEFAULT, PNG_FILTER_TYPE_DEFAULT);
						png_write_info(spngStruct, pspngiInfo);

						png_byte ** lpbRow_Pointers = new png_byte *[g_cOGL_Screenshot.m_tHeight];
						size_t tRow_Size = (g_cOGL_Screenshot.m_tColor_Depth_Bits >> 3) * g_cOGL_Screenshot.m_tWidth;
						lpbRow_Pointers[0] = (png_byte *)(g_cOGL_Screenshot.m_lpvData) + (g_cOGL_Screenshot.m_tHeight - 1) * tRow_Size;
						for (size_t tI = 1; tI < g_cOGL_Screenshot.m_tHeight; tI++)
						{
							lpbRow_Pointers[tI] = lpbRow_Pointers[tI - 1] - tRow_Size;
						}
						png_write_image(spngStruct, lpbRow_Pointers);
						png_write_end(spngStruct, pspngiInfo);

						png_destroy_write_struct(&spngStruct,&pspngiInfo);
						delete[] lpbRow_Pointers;
					}
					else
						png_destroy_write_struct(&spngStruct,nullptr);

				}
				fclose(filePNG);
			}
		}

		Sleep(1);
	}
	while (!g_lpMain->Pending_Quit());

return;
	
}
