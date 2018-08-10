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
	virtual void Initializer (void);


	ogl_screenshot_info(void){Initializer();}
};

extern ogl_screenshot_info g_cOGL_Screenshot;
extern void Screenshot_Loop(void);


