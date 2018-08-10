#pragma once
namespace bwm801
{
	template <typename T> class EXPORT pair
	{
	public:
		T m_tX;
		T m_tY;

		pair(void)
		{
			m_tX = 0;
			m_tY = 0;
		}
		pair(T i_tX, T i_tY)
		{
			m_tX = i_tX;
			m_tY = i_tY;
		}
		pair operator +(const pair & i_cRHO) const
		{
			return pair(m_tX + i_cRHO.m_tX, m_tY + i_cRHO.m_tY);
		}
		pair & operator +=(const pair & i_cRHO)
		{
			m_tX += i_cRHO.m_tX;
			m_tY += i_cRHO.m_tY;
			return *this;
		}
		pair operator -(const pair & i_cRHO) const
		{
			return pair(m_tX - i_cRHO.m_tX, m_tY - i_cRHO.m_tY);
		}
		pair & operator -=(const pair & i_cRHO)
		{
			m_tX -= i_cRHO.m_tX;
			m_tY -= i_cRHO.m_tY;
			return *this;
		}
		bool operator < (const pair & i_cRHO) const
		{
			return (m_tX < i_cRHO.m_tX || (m_tX == i_cRHO.m_tX && m_tY < i_cRHO.m_tY));
		}
		bool operator <= (const pair & i_cRHO) const
		{
			return (m_tX < i_cRHO.m_tX || (m_tX == i_cRHO.m_tX && m_tY <= i_cRHO.m_tY));
		}
		bool operator > (const pair & i_cRHO) const
		{
			return (m_tX > i_cRHO.m_tX || (m_tX == i_cRHO.m_tX && m_tY > i_cRHO.m_tY));
		}
		bool operator >= (const pair & i_cRHO) const
		{
			return (m_tX > i_cRHO.m_tX || (m_tX == i_cRHO.m_tX && m_tY >= i_cRHO.m_tY));
		}
		bool operator == (const pair & i_cRHO) const
		{
			return (m_tX == i_cRHO.m_tX && m_tY == i_cRHO.m_tY);
		}
		bool operator != (const pair & i_cRHO) const
		{
			return (m_tX != i_cRHO.m_tX || m_tY != i_cRHO.m_tY);
		}

		template <typename S> pair & operator =(const pair<S> &i_cRHO)
		{
			m_tX = (T)i_cRHO.m_tX;
			m_tY = (T)i_cRHO.m_tY;
			return *this;
		}
		template <typename S> pair(const pair<S> &i_cRHO)
		{
			m_tX = (T)i_cRHO.m_tX;
			m_tY = (T)i_cRHO.m_tY;
		}
		void glVertex(void) const
		{
			glVertex3f((GLfloat)m_tX, (GLfloat)m_tY, 0.0);
		}
		void glTranslate(void) const
		{
			glTranslatef((GLfloat)m_tX, (GLfloat)m_tY, 0.0);
		}
		void glScale(void) const
		{
			glScalef((GLfloat)m_tX, (GLfloat)m_tY, 1.0);
		}

		pair <float> Scalepair(const pair <double> &i_tScalar) const
		{
			pair<float> tRet;
			tRet.m_tX = m_tX * i_tScalar.m_tX;
			tRet.m_tY = m_tY * i_tScalar.m_tY;
			return tRet;
		}
		pair <float> Scalepair(const pair <float> &i_tScalar) const
		{
			pair<float> tRet;
			tRet.m_tX = m_tX * i_tScalar.m_tX;
			tRet.m_tY = m_tY * i_tScalar.m_tY;
			return tRet;
		}
		pair operator *(const float & i_cRHO)
		{
			pair<T> cRet;
			cRet.m_tX *= i_cRHO;
			cRet.m_tY *= i_cRHO;
			return cRet;
		};
		pair& operator *=(const float & i_cRHO)
		{
			m_tX *= i_cRHO;
			m_tY *= i_cRHO;
			return *this;
		};
		pair operator /(const float & i_cRHO)
		{
			pair<T> cRet;
			cRet.m_tX /= i_cRHO;
			cRet.m_tY /= i_cRHO;
			return cRet;
		};
		pair& operator /=(const float & i_cRHO)
		{
			m_tX /= i_cRHO;
			m_tY /= i_cRHO;
			return *this;
		}
	};
//	pair<float> operator *(const pair<float> & i_cLHO, const float &i_dRHO);
//	pair<float> operator *=(pair<float> & io_cLHO, const float &i_dRHO);
//	pair<float> operator /(const pair<float> & i_cLHO, const float &i_dRHO);
//	pair<float> operator /=(pair<float> & io_cLHO, const float &i_dRHO);
//	pair<float> operator *(const float &i_dRHO, const pair<float> & i_cLHO );
//	pair<float> operator /(const float &i_dRHO, const pair<float> & i_cLHO);
}

