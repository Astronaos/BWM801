#include <bwm801.h>

using namespace bwm801;

bool calllist::Is_Valid(void) const
{
	return (m_uiList_ID != (unsigned int)-1);
}
void calllist::Compile(bool i_bCompile_and_Exec)
{
	if (m_uiList_ID == (unsigned int)-1)
		m_uiList_ID = glGenLists(1);
#ifdef _DEBUG
	_RPT1(_CRT_WARN, "Call list created: %i\n", m_uiList_ID);
#endif
	glNewList(m_uiList_ID, i_bCompile_and_Exec ? GL_COMPILE_AND_EXECUTE : GL_COMPILE);
}
void calllist::End_Compile(void)
{
	glEndList();
}
void calllist::Draw(void)
{
	if (m_uiList_ID != (unsigned int)-1)
		glCallList(m_uiList_ID);
}
void	calllist::Delete(void)
{
	if (m_uiList_ID != (unsigned int)-1)
		glDeleteLists(m_uiList_ID, 1);
	m_uiList_ID = (unsigned int)-1;
}

void calllist::Initializer(void)
{
	m_uiList_ID = (unsigned int)-1;
}
void calllist::Destructor(void)
{ 
	Delete();
}

