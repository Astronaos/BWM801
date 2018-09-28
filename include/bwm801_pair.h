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
		inline pair operator +(const pair & i_cRHO) const
		{
			return pair(m_tX + i_cRHO.m_tX, m_tY + i_cRHO.m_tY);
		}
		inline pair & operator +=(const pair & i_cRHO)
		{
			m_tX += i_cRHO.m_tX;
			m_tY += i_cRHO.m_tY;
			return *this;
		}
		inline pair operator -(const pair & i_cRHO) const
		{
			return pair(m_tX - i_cRHO.m_tX, m_tY - i_cRHO.m_tY);
		}
		inline pair & operator -=(const pair & i_cRHO)
		{
			m_tX -= i_cRHO.m_tX;
			m_tY -= i_cRHO.m_tY;
			return *this;
		}
		inline bool operator < (const pair & i_cRHO) const
		{
			return (m_tX < i_cRHO.m_tX || (m_tX == i_cRHO.m_tX && m_tY < i_cRHO.m_tY));
		}
		inline bool operator <= (const pair & i_cRHO) const
		{
			return (m_tX < i_cRHO.m_tX || (m_tX == i_cRHO.m_tX && m_tY <= i_cRHO.m_tY));
		}
		inline bool operator > (const pair & i_cRHO) const
		{
			return (m_tX > i_cRHO.m_tX || (m_tX == i_cRHO.m_tX && m_tY > i_cRHO.m_tY));
		}
		inline bool operator >= (const pair & i_cRHO) const
		{
			return (m_tX > i_cRHO.m_tX || (m_tX == i_cRHO.m_tX && m_tY >= i_cRHO.m_tY));
		}
		inline bool operator == (const pair & i_cRHO) const
		{
			return (m_tX == i_cRHO.m_tX && m_tY == i_cRHO.m_tY);
		}
		inline bool operator != (const pair & i_cRHO) const
		{
			return (m_tX != i_cRHO.m_tX || m_tY != i_cRHO.m_tY);
		}

		template <typename S> inline pair & operator =(const pair<S> &i_cRHO)
		{
			m_tX = (T)i_cRHO.m_tX;
			m_tY = (T)i_cRHO.m_tY;
			return *this;
		}
		template <typename S> inline pair(const pair<S> &i_cRHO)
		{
			m_tX = (T)i_cRHO.m_tX;
			m_tY = (T)i_cRHO.m_tY;
		}

		inline void glVertex(void) const
		{
			glVertex3f((GLfloat)m_tX, (GLfloat)m_tY, 0.0);
		}
		inline void glTranslate(void) const
		{
			glTranslatef((GLfloat)m_tX, (GLfloat)m_tY, 0.0);
		}
		inline void glScale(void) const
		{
			glScalef((GLfloat)m_tX, (GLfloat)m_tY, 1.0);
		}

		inline pair <float> Scalepair(const pair <double> &i_tScalar) const
		{
			pair<float> tRet;
			tRet.m_tX = m_tX * i_tScalar.m_tX;
			tRet.m_tY = m_tY * i_tScalar.m_tY;
			return tRet;
		}
		inline pair <float> Scalepair(const pair <float> &i_tScalar) const
		{
			pair<float> tRet;
			tRet.m_tX = m_tX * i_tScalar.m_tX;
			tRet.m_tY = m_tY * i_tScalar.m_tY;
			return tRet;
		}
		inline pair operator *(const float & i_cRHO) const
		{
			pair<T> cRet;
			cRet.m_tX *= i_cRHO;
			cRet.m_tY *= i_cRHO;
			return cRet;
		};
		inline pair& operator *=(const float & i_cRHO)
		{
			m_tX *= i_cRHO;
			m_tY *= i_cRHO;
			return *this;
		};
		inline pair operator /(const float & i_cRHO) const
		{
			pair<T> cRet;
			cRet.m_tX /= i_cRHO;
			cRet.m_tY /= i_cRHO;
			return cRet;
		};
		inline pair& operator /=(const float & i_cRHO)
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

