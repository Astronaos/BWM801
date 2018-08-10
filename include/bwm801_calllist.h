#pragma once

#include <GL/gl.h>

namespace bwm801
{
	class EXPORT calllist
	{
	private:
		GLuint	m_uiList_ID;
	public:
		virtual bool Is_Valid(void) const;
		virtual void Compile(bool i_bCompile_and_Exec = false);
		virtual void End_Compile(void);
		virtual void Draw(void);
		virtual void Delete(void);

		virtual void Initializer(void);
		virtual void Destructor(void);
		calllist(void){Initializer();}
		~calllist(void){Destructor();}

	};
};

