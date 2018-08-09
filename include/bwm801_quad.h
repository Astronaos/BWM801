#pragma once

//@@TODO: create volume class from triplets
namespace bwm801
{
	template <typename T> class quad
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
		quad operator +(const pair<T> & i_cRHO) const
		{
			return quad(m_tBL + i_cRHO,m_tTR + i_cRHO);
		}
		quad & operator +=(const pair<T> & i_cRHO)
		{
			m_tBL += i_cRHO;
			m_tTR += i_cRHO;
			return *this;
		}
		quad operator -(const pair<T> & i_cRHO) const
		{
			return quad(m_tBL - i_cRHO,m_tTR - i_cRHO);
		}
		quad & operator -=(const pair<T> & i_cRHO)
		{
			m_tBL -= i_cRHO;
			m_tTR -= i_cRHO;
			return *this;
		}
		bool operator ==(const quad<T> & i_cRHO) const
		{
			return (i_cRHO.m_tBL == i_cRHO.m_tBL && i_cRHO.m_tTR == i_cRHO.m_tTR);
		}
		bool operator !=(const quad<T> & i_cRHO) const
		{
			return (i_cRHO.m_tBL != i_cRHO.m_tBL || i_cRHO.m_tTR != i_cRHO.m_tTR);
		}
		pair<T> Get_Size(void) const
		{
			return (m_tTR - m_tBL);
		}

		template <typename S> quad & operator =(const quad<S> &i_cRHO)
		{
			m_tBL = (T) i_cRHO.m_tBL;
			m_tTR = (T) i_cRHO.m_tTR;
		}
		template <typename S> quad (const quad<S> &i_cRHO)
		{
			m_tBL = (T) i_cRHO.m_tBL;
			m_tTR = (T) i_cRHO.m_tTR;
		}
		void glDrawPolygon(void)
		{
			glBegin(GL_QUADS);
				glVertex2f((GLfloat)m_tBL.m_tX,(GLfloat)m_tBL.m_tY);
				glVertex2f((GLfloat)m_tBL.m_tX,(GLfloat)m_tTR.m_tY);
				glVertex2f((GLfloat)m_tTR.m_tX,(GLfloat)m_tTR.m_tY);
				glVertex2f((GLfloat)m_tTR.m_tX,(GLfloat)m_tBL.m_tY);
			glEnd();
		}
		void glDrawOutline(void)
		{
			glBegin(GL_LINE_LOOP);
				glVertex2f((GLfloat)m_tBL.m_tX,(GLfloat)m_tBL.m_tY);
				glVertex2f((GLfloat)m_tBL.m_tX,(GLfloat)m_tTR.m_tY);
				glVertex2f((GLfloat)m_tTR.m_tX,(GLfloat)m_tTR.m_tY);
				glVertex2f((GLfloat)m_tTR.m_tX,(GLfloat)m_tBL.m_tY);
			glEnd();
		}
	};
	quad<double> operator *(const quad<double> & i_cLHO, const double &i_dRHO);
	quad<double> operator *=(quad<double> & io_cLHO, const double &i_dRHO);
	quad<double> operator /(const quad<double> & i_cLHO, const double &i_dRHO);
	quad<double> operator /=(quad<double> & io_cLHO, const double &i_dRHO);
	quad<double> operator *(const double &i_dRHO, const quad<double> & i_cLHO );

};
