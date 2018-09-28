#pragma once

//@@TODO: create volume class from triplets
namespace bwm801
{
	template <typename T> class EXPORT quad
	{
	public:
		pair<T> m_tBL; // bottom left
		pair<T> m_tTR; // top right

		quad(void) : m_tBL(), m_tTR()
		{
		}
		quad(pair<T> i_tBL, pair<T> i_tTR)
		{
			m_tBL = i_tBL;
			m_tTR = i_tTR;
		}
		inline quad operator +(const pair<T> & i_cRHO) const
		{
			return quad(m_tBL + i_cRHO,m_tTR + i_cRHO);
		}
		inline quad & operator +=(const pair<T> & i_cRHO)
		{
			m_tBL += i_cRHO;
			m_tTR += i_cRHO;
			return *this;
		}
		inline quad operator -(const pair<T> & i_cRHO) const
		{
			return quad(m_tBL - i_cRHO,m_tTR - i_cRHO);
		}
		inline quad & operator -=(const pair<T> & i_cRHO)
		{
			m_tBL -= i_cRHO;
			m_tTR -= i_cRHO;
			return *this;
		}
		inline bool operator ==(const quad<T> & i_cRHO) const
		{
			return (i_cRHO.m_tBL == i_cRHO.m_tBL && i_cRHO.m_tTR == i_cRHO.m_tTR);
		}
		inline bool operator !=(const quad<T> & i_cRHO) const
		{
			return (i_cRHO.m_tBL != i_cRHO.m_tBL || i_cRHO.m_tTR != i_cRHO.m_tTR);
		}
		inline pair<T> Get_Size(void) const
		{
			return (m_tTR - m_tBL);
		}

		template <typename S> inline quad & operator =(const quad<S> &i_cRHO) 
		{
			m_tBL = (T) i_cRHO.m_tBL;
			m_tTR = (T) i_cRHO.m_tTR;
			return *this;
		}
		template <typename S> inline quad (const quad<S> &i_cRHO)
		{
			m_tBL = (T) i_cRHO.m_tBL;
			m_tTR = (T) i_cRHO.m_tTR;
		}
		inline void glDrawPolygon(void)
		{
			glBegin(GL_QUADS);
				glVertex2f((GLfloat)m_tBL.m_tX,(GLfloat)m_tBL.m_tY);
				glVertex2f((GLfloat)m_tBL.m_tX,(GLfloat)m_tTR.m_tY);
				glVertex2f((GLfloat)m_tTR.m_tX,(GLfloat)m_tTR.m_tY);
				glVertex2f((GLfloat)m_tTR.m_tX,(GLfloat)m_tBL.m_tY);
			glEnd();
		}
		inline void glDrawOutline(void)
		{
			glBegin(GL_LINE_LOOP);
				glVertex2f((GLfloat)m_tBL.m_tX,(GLfloat)m_tBL.m_tY);
				glVertex2f((GLfloat)m_tBL.m_tX,(GLfloat)m_tTR.m_tY);
				glVertex2f((GLfloat)m_tTR.m_tX,(GLfloat)m_tTR.m_tY);
				glVertex2f((GLfloat)m_tTR.m_tX,(GLfloat)m_tBL.m_tY);
			glEnd();
		}
	};
	quad<float> operator *(const quad<float> & i_cLHO, const float &i_dRHO);
	quad<float> operator *=(quad<float> & io_cLHO, const float &i_dRHO);
	quad<float> operator /(const quad<float> & i_cLHO, const float &i_dRHO);
	quad<float> operator /=(quad<float> & io_cLHO, const float &i_dRHO);
	quad<float> operator *(const float &i_dRHO, const quad<float> & i_cLHO );

}
