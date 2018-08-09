#pragma once

namespace bwm801
{
	template <typename T> class triplet
	{
	public:
		T m_tX;
		T m_tY;
		T m_tZ;

		triplet(void)
		{
			m_tX = 0;
			m_tY = 0;
			m_tZ = 0;
		}
		triplet(T i_tX, T i_tY, T i_tZ)
		{
			m_tX = i_tX;
			m_tY = i_tY;
			m_tZ = i_tZ;
		}
		triplet operator +(const triplet & i_cRHO) const
		{
			return triplet(m_tX + i_cRHO.m_tX,m_tY + i_cRHO.m_tY,m_tZ + i_cRHO.m_tZ);
		}
		triplet & operator +=(const triplet & i_cRHO)
		{
			m_tX += i_cRHO.m_tX;
			m_tY += i_cRHO.m_tY;
			m_tZ += i_cRHO.m_tZ;
			return *this;
		}
		triplet operator -(const triplet & i_cRHO) const
		{
			return triplet(m_tX - i_cRHO.m_tX,m_tY - i_cRHO.m_tY,m_tZ - i_cRHO.m_tZ);
		}
		triplet & operator -=(const triplet & i_cRHO)
		{
			m_tX -= i_cRHO.m_tX;
			m_tY -= i_cRHO.m_tY;
			m_tZ -= i_cRHO.m_tZ;
			return *this;
		}
		bool operator < (const triplet & i_cRHO) const
		{
			return (m_tX < i_cRHO.m_tX || (m_tX == i_cRHO.m_tX && m_tY < i_cRHO.m_tY) || (m_tX == i_cRHO.m_tX && m_tY == i_cRHO.m_tY && m_tZ < i_cRHO.m_tZ));
		}
		bool operator <= (const triplet & i_cRHO) const
		{
			return (m_tX < i_cRHO.m_tX || (m_tX == i_cRHO.m_tX && m_tY <= i_cRHO.m_tY) || (m_tX == i_cRHO.m_tX && m_tY == i_cRHO.m_tY && m_tZ <= i_cRHO.m_tZ));
		}
		bool operator > (const triplet & i_cRHO) const
		{
			return (m_tX > i_cRHO.m_tX || (m_tX == i_cRHO.m_tX && m_tY > i_cRHO.m_tY) || (m_tX == i_cRHO.m_tX && m_tY == i_cRHO.m_tY && m_tZ > i_cRHO.m_tZ));
		}
		bool operator >= (const triplet & i_cRHO) const
		{
			return (m_tX > i_cRHO.m_tX || (m_tX == i_cRHO.m_tX && m_tY >= i_cRHO.m_tY) || (m_tX == i_cRHO.m_tX && m_tY == i_cRHO.m_tY && m_tZ == i_cRHO.m_tZ));
		}
		bool operator == (const triplet & i_cRHO) const
		{
			return (m_tX == i_cRHO.m_tX && m_tY == i_cRHO.m_tY && m_tZ == i_cRHO.m_tZ);
		}
		bool operator != (const triplet & i_cRHO) const
		{
			return (m_tX != i_cRHO.m_tX || m_tY != i_cRHO.m_tY || m_tZ != i_cRHO.m_tZ);
		}

		template <typename S> triplet & operator =(const triplet<S> &i_cRHO)
		{
			m_tX = (T) i_cRHO.m_tX;
			m_tY = (T) i_cRHO.m_tY;
			m_tZ = (T) i_cRHO.m_tZ;
		}
		template <typename S> triplet (const triplet<S> &i_cRHO)
		{
			m_tX = (T) i_cRHO.m_tX;
			m_tY = (T) i_cRHO.m_tY;
			m_tZ = (T) i_cRHO.m_tZ;
		}
		void glVertex(void) const
		{
			glVertex3f((GLfloat)m_tX,(GLfloat)m_tY,(GLfloat)m_tZ);
		}
		void glTranslate(void) const
		{
			glTranslatef((GLfloat)m_tX,(GLfloat)m_tY,(GLfloat)m_tZ);
		}
		void glScale(void) const
		{
			glScalef((GLfloat)m_tX,(GLfloat)m_tY,(GLfloat)m_tZ);
		}

		triplet <float> ScaleTriplet(const triplet <double> &i_tScalar) const
		{
			triplet<float> tRet;
			tRet.m_tX = m_tX * i_tScalar.m_tX;
			tRet.m_tY = m_tY * i_tScalar.m_tY;
			tRet.m_tZ = m_tZ * i_tScalar.m_tZ;
			return tRet;
		}
		void glColor(const double & i_dAlpha = 1.0) const
		{
			glColor4f((GLfloat) m_tX, (GLfloat) m_tY, (GLfloat) m_tZ, (GLfloat)i_dAlpha);
		}
	};
	triplet<float> operator *(const triplet<float> & i_cLHO, const double &i_dRHO);
	triplet<float> operator *=(triplet<float> & io_cLHO, const double &i_dRHO);
	triplet<float> operator /(const triplet<float> & i_cLHO, const double &i_dRHO);
	triplet<float> operator /=(triplet<float> & io_cLHO, const double &i_dRHO);
	triplet<float> operator *(const double &i_dRHO, const triplet<float> & i_cLHO );
};

