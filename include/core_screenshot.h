#pragma once


class ogl_screenshot_info
{
public:
	bool m_bRequest;
	bool m_bReady;
	size_t m_tWidth;
	size_t m_tHeight;
	size_t m_tColor_Depth_Bits;
	void * m_lpvData;
	ogl_screenshot_info (void)
	{
		m_bRequest = false;
		m_bReady = false;
		m_tWidth = 0;
		m_tHeight = 0;
		m_tColor_Depth_Bits = 0;
		m_lpvData = nullptr;
	}
};

extern ogl_screenshot_info g_cOGL_Screenshot;
extern void Screenshot_Loop(void);


