#pragma once
#if __has_include("amp_win.h")
#include "amp_win.h"
#else
#define RESTRICT
#include <cmath>
namespace amp_win_math = std;
#endif

//ver 4.0.0 make use template
//2017/11/20

//please use C++ 11 conpatible compiler
namespace akechi_akihide {
	template<int d, typename T>
	class CDVector;

	template<int d, typename T>
	class CDMatrix;

	template<typename T>
	class CQuaternion;

	template<bool is, typename Tc, int d>
	class ValidPack;


	template< typename Tc, int d>
	class ValidPack<true, Tc, d> {
	private:
		template< typename first, typename... REST>
		static void fconst(CDVector<d, Tc>& vi, int cd, first f, REST... rest) RESTRICT
		{
			vi[cd] = f;
			fconst(vi, cd + 1, rest...);
		}

		static void fconst(CDVector<d, Tc>& vi, int cd) RESTRICT
		{

		}
	public:
		template<typename... VIN>
		static void pack(CDVector<d, Tc>& vi, VIN... vin) RESTRICT
		{
			fconst(vi, 0, vin...);
		}
	};


	template<typename Tc>
	inline Tc sqrtR(Tc fi)  RESTRICT
	{
		return amp_win_math::sqrt(fi);
	}
	template<typename Tc>
	inline Tc sinR(Tc fi) RESTRICT
	{
		return amp_win_math::sin(fi);
	}

	template<typename Tc>
	inline Tc cosR(Tc fi) RESTRICT
	{
		return amp_win_math::cos(fi);
	}

	template<typename Tc>
	inline Tc absR(Tc fi) RESTRICT
	{
		return amp_win_math::fabs(fi);
	}

	template<typename Tc>
	inline Tc acosR(Tc fi) RESTRICT
	{
		return amp_win_math::acos(fi);
	}
	template<>
	inline double sqrtR<double>(double fi)  RESTRICT
	{
		return amp_win_math::sqrt(fi);
	}

	template<>
	inline double sinR<double>(double fi) RESTRICT
	{
		return amp_win_math::sin(fi);
	}

	template<>
	inline double cosR<double>(double fi) RESTRICT
	{
		return amp_win_math::cos(fi);
	}

	template<>
	inline double acosR<double>(double fi) RESTRICT
	{
		return amp_win_math::acos(fi);
	}


	template<>
	inline double absR<double>(double fi) RESTRICT
	{
		return amp_win_math::fabs(fi);
	}


	template<typename T>
	class CQuaternion
	{
	public:
		T m_v[4];
		CQuaternion(T r, T ix, T jy, T kz) RESTRICT
		{
			m_v[0] = r;
			m_v[1] = 0;
			m_v[2] = 0;
			m_v[3] = 0;
		}
		CQuaternion() RESTRICT {}
		template<typename Tc2>
		CQuaternion(CQuaternion<Tc2> qi) RESTRICT
		{
			for (int i = 0; i<4; i++)
				m_v[i] = qi.m_v[i];
		}
		template<typename Tc2>
		CQuaternion(CDVector<4, Tc2> qi) RESTRICT
		{
			for (int i = 0; i<4; i++)
				m_v[i] = qi.m_v[i];
		}
		~CQuaternion() RESTRICT {};
		T & operator [](int i) RESTRICT;
		void  setRot(T Theta, CDVector<3, T> vec) RESTRICT;
		CQuaternion<T>  operator + (CQuaternion<T> qi) RESTRICT;
		CQuaternion<T>  operator - (CQuaternion<T> qi) RESTRICT;

		CQuaternion<T>& operator  =(CQuaternion<T> qi)   RESTRICT
		{
			for (int i = 0; i<4; i++)
				m_v[i] = qi.m_v[i];

			return *this;
		}
		CQuaternion<T>& operator  =(CDVector<4, T> qi)  RESTRICT
		{
			for (int i = 0; i<4; i++)
				m_v[i] = qi.m_v[i];
			return *this;
		}

		CQuaternion<T>& operator +=(CQuaternion<T> qi) RESTRICT;
		CQuaternion<T>& operator -=(CQuaternion<T> qi) RESTRICT;
		CQuaternion<T>  operator * (CQuaternion<T> qi) RESTRICT;
		CQuaternion<T>& operator *=(CQuaternion<T> qi) RESTRICT;
		CQuaternion<T>  operator / (CQuaternion<T> qi) RESTRICT;
		CQuaternion<T>& operator /=(CQuaternion<T> qi) RESTRICT;
		CQuaternion<T>  operator ~(void) RESTRICT;
		CQuaternion<T>  operator* (T is) RESTRICT;
		CQuaternion<T>& operator*=(T is) RESTRICT;
		CQuaternion<T>  operator/ (T is) RESTRICT;
		CQuaternion<T>& operator/=(T is) RESTRICT;
		T len() RESTRICT;
		CQuaternion<T> norm() RESTRICT;
		void selfnorm() RESTRICT;
		CQuaternion<T> slerpto(CQuaternion<T> qi, T t) RESTRICT;//q*t + qi*(1-t)

	};

	template<typename Tc>
	CQuaternion<Tc> operator-(CQuaternion<Tc>& qi) RESTRICT;//flip the sign

	template<typename T>
	CQuaternion<T> operator*(T t, CQuaternion<T> &qi) RESTRICT;//multiply scalar from left

	template<int d, typename Tc>
	class CDVector
	{
	public:
		Tc m_v[d];

		template<typename... Args>
		CDVector(Args... args) RESTRICT
		{
			ValidPack<(d == sizeof...(Args)), Tc, d>::pack(*this, args...);
		}
		CDVector() RESTRICT
		{}

		template<typename... Args>
		void set(Args... args) RESTRICT
		{
			ValidPack<(d == sizeof...(Args)), Tc, d>::pack(*this, args...);
		}

		Tc& operator [](int i) RESTRICT
		{
			return m_v[i];
		}

		const Tc& operator [](int i) const  RESTRICT
		{
			return m_v[i];
		}

		CDVector<d, Tc>& operator=(const CDVector<d, Tc>& vec) RESTRICT
		{
			for (int i = 0; i < d; i++)
			{
				m_v[i] = vec.m_v[i];
			}

			return *this;
		}

		template<typename Tc2>
		CDVector(const CDVector<d, Tc2>& vec) RESTRICT
		{
			for (int i = 0; i < d; i++)
			{
				m_v[i] = vec.m_v[i];
			}
		}

		template<typename Tc2>
		CDVector(CDVector<d, Tc2>&& vec) RESTRICT
		{
			for (int i = 0; i < d; i++)
			{
				m_v[i] = vec.m_v[i];
			}
		}
		CDVector<d, Tc>  operator+ (CDVector<d, Tc> vec) RESTRICT;
		CDVector<d, Tc>  operator- (CDVector<d, Tc> vec) RESTRICT;
		CDVector<d, Tc>& operator+=(CDVector<d, Tc> vec) RESTRICT;
		CDVector<d, Tc>& operator-=(CDVector<d, Tc> vec) RESTRICT;
		Tc operator  *(CDVector<d, Tc> vec) RESTRICT;      //inner product
		CDVector<d, Tc>  operator& (CDVector<d, Tc> vec) RESTRICT;
		CDVector<d, Tc>& operator&=(CDVector<d, Tc> vec) RESTRICT;
		CDVector<d, Tc>  operator/ (CDVector<d, Tc> vec) RESTRICT;
		CDVector<d, Tc>& operator/=(CDVector<d, Tc> vec) RESTRICT;
		CDVector<d, Tc>  operator* (Tc is) RESTRICT;
		CDVector<d, Tc>& operator*=(Tc is) RESTRICT;
		CDVector<d, Tc>  operator/ (Tc is) RESTRICT;
		CDVector<d, Tc>& operator/=(Tc is) RESTRICT;
		Tc len() RESTRICT;
		CDVector<d, Tc> norm() RESTRICT;
		void selfnorm() RESTRICT;
	};
	template<int d, typename Tc>
	inline CDVector<d, Tc> operator-(CDVector<d, Tc> vi) RESTRICT //flip the sign
	{
		CDVector<d, Tc> vr;
		for (int i = 0; i < d; i++)
		{
			vr[i] = -vi[i];
		}
		return vr;
	}
	template<int d, typename Tc>
	inline CDVector<d, Tc> operator*(Tc ii, CDVector<d, Tc> vi) RESTRICT//multipy a scalar from left
	{
		CDVector<d, Tc> vr;
		for (int i = 0; i < d; i++)
		{
			vr[i] = ii*vi[i];
		}
		return vr;
	}
	template<int d, typename Tc>
	inline CDVector<d, Tc> operator+(CDVector<d, Tc> vi1, CDVector<d, Tc> vi2) RESTRICT
	{
		CDVector<d, Tc> vr;
		for (int i = 0; i < d; i++)
		{
			vr[i] = vi1[i] + vi2[i];
		}
		return vr;
	}
	template<int d, typename Tc>
	inline CDVector<d, Tc> operator-(CDVector<d, Tc> vi1, CDVector<d, Tc> vi2) RESTRICT
	{
		CDVector<d, Tc> vr;
		for (int i = 0; i < d; i++)
		{
			vr[i] = vi1[i] - vi2[i];
		}
		return vr;
	}


	template<int d, typename Tc>
	class CDMatrix
	{
		template<int d2, typename Tsc>
		friend class Inverse;
		template<int d2, typename Tsc>
		friend class CDVector;
	protected:
	public:
		CDMatrix() RESTRICT
		{}
		Tc m[d][d];

		template<typename Tc2>
		CDMatrix(const CDMatrix<d, Tc2>& mi) RESTRICT
		{
			for (int i = 0; i < d; i++)
			{
				for (int j = 0; j < d; j++)
				{
					m[i][j] = mi[i][j];
				}
			}
		}

		template<typename Tc2>
		CDMatrix(CDMatrix<d, Tc2>&& mi) RESTRICT
		{
			for (int i = 0; i < d; i++)
			{
				for (int j = 0; j < d; j++)
				{
					m[i][j] = mi[i][j];
				}
			}
		}
		template<typename Tc2>
		CDMatrix& operator=(const CDMatrix<d, Tc2>& mi) RESTRICT
		{
			for (int i = 0; i < d; i++)
			{
				for (int j = 0; j < d; j++)
				{
					m[i][j] = mi[i][j];
				}
			}
			return *this;
		}

		template<typename Tc2>
		CDMatrix& operator=(CDMatrix<d, Tc2>&& mi) RESTRICT
		{
			for (int i = 0; i < d; i++)
			{
				for (int j = 0; j < d; j++)
				{
					m[i][j] = mi[i][j];
				}
			}
			return *this;
		}

		Tc* operator[](unsigned int u) RESTRICT
		{
			return m[u];
		}
		const Tc* operator[](unsigned int u) const RESTRICT
		{
			return m[u];
		}
		CDMatrix<d, Tc>  operator+ (CDMatrix<d, Tc> mi) RESTRICT;
		CDMatrix<d, Tc>  operator- (CDMatrix<d, Tc> mi) RESTRICT;
		CDMatrix<d, Tc>& operator+=(CDMatrix<d, Tc> mi) RESTRICT;
		CDMatrix<d, Tc>& operator-=(CDMatrix<d, Tc> mi) RESTRICT;
		CDMatrix<d, Tc>  operator* (const CDMatrix<d, Tc>& mi) const RESTRICT;
		CDMatrix<d, Tc>& operator*=(CDMatrix<d, Tc> mi) RESTRICT;
		CDMatrix<d, Tc>& operator =(CDMatrix<d, Tc> mi) RESTRICT;
		CDMatrix<d, Tc>  operator* (Tc is) RESTRICT;
		CDMatrix<d, Tc>  operator+ (Tc is) RESTRICT;
		CDMatrix<d, Tc>  operator- (Tc is) RESTRICT;
		CDMatrix<d, Tc>& operator+=(Tc is) RESTRICT;
		CDMatrix<d, Tc>& operator-=(Tc is) RESTRICT;
		CDMatrix<d, Tc>& operator =(Tc is) RESTRICT;
		CDMatrix<d, Tc>& operator*=(Tc is) RESTRICT;

		template<typename Tsc>
		CDVector<d - 1, Tsc>  operator* (const CDVector<d-1, Tsc>& vi)const RESTRICT
		{
			CDVector<d - 1, Tsc>  vr;

			for (int i = 0; i < d - 1; i++)
			{
				vr[i] = m[i][d - 1];
				for (int j = 0; j < d - 1; j++)
				{
					vr[i] += m[i][j] * vi[j];
				}
			}
			return vr;
		}

		template<typename Tsc>
		CDVector<d, Tsc>  operator* (const  CDVector<d, Tsc>&  vi)const RESTRICT
		{
			CDVector<d, Tsc>  vr;

			for (int i = 0; i < d; i++)
			{
				vr[i] = 0;
				for (int j = 0; j < d; j++)
				{
					vr[i] += m[i][j] * vi[j];
				}
			}
			return vr;
		}
		bool  Power_Self(int in) RESTRICT;

		void SetFromRow(CDVector<d, Tc> v1[d]) RESTRICT
		{
			for (int i = 0; i < d; i++)
			{
				for (int j = 0; j < d; j++)
				{
					m[i][j] = v1[j][i];
				}
			}
		}


	};

	//these are for 3D graphics.
	template<typename Tc> CDMatrix<4, Tc> setOrthoLH(Tc w, Tc h, Tc zn, Tc zf) RESTRICT;
	template<typename Tc> CDMatrix<4, Tc> setPerspectiveLH(Tc w, Tc h, Tc zn, Tc zf) RESTRICT;

	template<typename Tc> CDMatrix<4, Tc> setPerspectiveFovLH(Tc fovy, Tc aspect, Tc zn, Tc zf) RESTRICT;
	template<typename Tc> CDMatrix<4, Tc> setLookAt(CDVector<3, Tc> vEye, CDVector<3, Tc> vAt) RESTRICT;
	//generate a camera
	template<typename Tc> CDMatrix<4, Tc> setLookAt(CDVector<3, Tc> vEye, CDVector<3, Tc> vAt, CDVector<3, Tc> vUp) RESTRICT;
	//transe linear
	template<typename Tc> CDMatrix<4, Tc> setTrans(CDVector<3, Tc> vTrans) RESTRICT;

	//-----------------------------------//
	//-------)---------------//
	//-----------------------------------//


	template<typename Tc>
	Tc& CQuaternion<Tc>::operator [](int i)    RESTRICT
	{
		return m_v[i];
	}

	template<typename T>
	void CQuaternion<T>::setRot(T Theta, CDVector<3, T> vec)   RESTRICT
	{
		m_v[0] = sinR<T>(Theta / 2);
		m_v[1] = vec.m_v[0] * sinR<T>(Theta / 2);
		m_v[2] = vec.m_v[1] * sinR<T>(Theta / 2);
		m_v[3] = vec.m_v[2] * sinR<T>(Theta / 2);
	}
	template<typename Tc>
	CQuaternion<Tc>  CQuaternion<Tc>::operator + (CQuaternion<Tc> qi)    RESTRICT
	{
		CQuaternion<Tc> qr;

		qr.m_v[0] = m_v[0] + qi.m_v[0];
		qr.m_v[1] = m_v[1] + qi.m_v[1];
		qr.m_v[2] = m_v[2] + qi.m_v[2];
		qr.m_v[3] = m_v[3] + qi.m_v[3];

		return qr;
	}
	template<typename Tc>
	CQuaternion<Tc>  CQuaternion<Tc>::operator - (CQuaternion<Tc> qi)     RESTRICT
	{
		CQuaternion<Tc> qr;

		qr.m_v[0] = m_v[0] - qi.m_v[0];
		qr.m_v[1] = m_v[1] - qi.m_v[1];
		qr.m_v[2] = m_v[2] - qi.m_v[2];
		qr.m_v[3] = m_v[3] - qi.m_v[3];

		return qr;
	}

	template<typename Tc>
	CQuaternion<Tc> CQuaternion<Tc>::operator ~(void) RESTRICT
	{
		CQuaternion<Tc> qr;

		qr.m_v[0] = m_v[0];
		qr.m_v[1] = -m_v[1];
		qr.m_v[2] = -m_v[2];
		qr.m_v[3] = -m_v[3];

		return qr;
	}
	template<typename Tc>
	CQuaternion<Tc>& CQuaternion<Tc>::operator+=(CQuaternion<Tc> qi)     RESTRICT
	{
		m_v[0] = m_v[0] + qi.m_v[0];
		m_v[1] = m_v[1] + qi.m_v[1];
		m_v[2] = m_v[2] + qi.m_v[2];
		m_v[3] = m_v[3] + qi.m_v[3];
		return *this;
	}
	template<typename Tc>
	CQuaternion<Tc>& CQuaternion<Tc>::operator-=(CQuaternion<Tc> qi)     RESTRICT
	{
		m_v[0] = m_v[0] - qi.m_v[0];
		m_v[1] = m_v[1] - qi.m_v[1];
		m_v[2] = m_v[2] - qi.m_v[2];
		m_v[3] = m_v[3] - qi.m_v[3];
		return *this;
	}
	template<typename Tc>
	CQuaternion<Tc>  CQuaternion<Tc>::operator* (CQuaternion<Tc> qi)    RESTRICT
	{
		CQuaternion<Tc> qr;
		qr.m_v[0] = m_v[0] * qi.m_v[0] - m_v[1] * qi.m_v[1] - m_v[2] * qi.m_v[2] - m_v[3] * qi.m_v[3];
		qr.m_v[1] = m_v[0] * qi.m_v[1] + m_v[1] * qi.m_v[0] + m_v[2] * qi.m_v[3] - m_v[3] * qi.m_v[2];
		qr.m_v[2] = m_v[0] * qi.m_v[2] - m_v[1] * qi.m_v[3] + m_v[2] * qi.m_v[0] + m_v[3] * qi.m_v[1];
		qr.m_v[3] = m_v[0] * qi.m_v[3] + m_v[1] * qi.m_v[2] - m_v[2] * qi.m_v[1] + m_v[3] * qi.m_v[0];
		return qr;
	}
	template<typename Tc>
	CQuaternion<Tc>& CQuaternion<Tc>::operator*=(CQuaternion<Tc> qi)    RESTRICT
	{
		Tc qr[4];

		qr[0] = m_v[0] * qi.m_v[0] - m_v[1] * qi.m_v[1] - m_v[2] * qi.m_v[2] - m_v[3] * qi.m_v[3];
		qr[1] = m_v[0] * qi.m_v[1] + m_v[1] * qi.m_v[0] + m_v[2] * qi.m_v[3] - m_v[3] * qi.m_v[2];
		qr[2] = m_v[0] * qi.m_v[2] - m_v[1] * qi.m_v[3] + m_v[2] * qi.m_v[0] + m_v[3] * qi.m_v[1];
		qr[3] = m_v[0] * qi.m_v[3] + m_v[1] * qi.m_v[2] - m_v[2] * qi.m_v[1] + m_v[3] * qi.m_v[0];

		for (int i = 0; i < 4; i++)
		{
			m_v[i] = qr[i];
		}

		return *this;
	}
	template<typename Tc>
	CQuaternion<Tc> CQuaternion<Tc>::operator/ (CQuaternion<Tc> qi)    RESTRICT
	{
		CQuaternion<Tc> qr;
		Tc qt[4];
		Tc len;

		len = qi.m_v[0] * qi.m_v[0] + qi.m_v[1] * qi.m_v[1] + qi.m_v[2] * qi.m_v[2] + qi.m_v[3] * qi.m_v[3];
		len = sqrtR<Tc>(len) + 1e-40;

		qt[0] = qi.m_v[0] / len;
		qt[1] = -qi.m_v[1] / len;
		qt[2] = -qi.m_v[2] / len;
		qt[3] = -qi.m_v[3] / len;

		qr.m_v[0] = m_v[0] * qt[0] - m_v[1] * qt[1] - m_v[2] * qt[2] - m_v[3] * qt[3];
		qr.m_v[1] = m_v[0] * qt[1] + m_v[1] * qt[0] + m_v[2] * qt[3] - m_v[3] * qt[2];
		qr.m_v[2] = m_v[0] * qt[2] - m_v[1] * qt[3] + m_v[2] * qt[0] + m_v[3] * qt[1];
		qr.m_v[3] = m_v[0] * qt[3] + m_v[1] * qt[2] - m_v[2] * qt[1] + m_v[3] * qt[0];

		return qr;
	}
	template<typename Tc>
	CQuaternion<Tc>& CQuaternion<Tc>::operator/=(CQuaternion<Tc> qi)   RESTRICT
	{
		CQuaternion<Tc> qr;
		Tc qt[4];
		Tc len2;

		len2 = qi.m_v[0] * qi.m_v[0] + qi.m_v[1] * qi.m_v[1] + qi.m_v[2] * qi.m_v[2] + qi.m_v[3] * qi.m_v[3] + 1e-40;

		qt[0] = qi.m_v[0] / len2;
		qt[1] = -qi.m_v[1] / len2;
		qt[2] = -qi.m_v[2] / len2;
		qt[3] = -qi.m_v[3] / len2;

		qr.m_v[0] = m_v[0] * qt[0] - m_v[1] * qt[1] - m_v[2] * qt[2] - m_v[3] * qt[3];
		qr.m_v[1] = m_v[0] * qt[1] + m_v[1] * qt[0] + m_v[2] * qt[3] - m_v[3] * qt[2];
		qr.m_v[2] = m_v[0] * qt[2] - m_v[1] * qt[3] + m_v[2] * qt[0] + m_v[3] * qt[1];
		qr.m_v[3] = m_v[0] * qt[3] + m_v[1] * qt[2] - m_v[2] * qt[1] + m_v[3] * qt[0];

		for (int i = 0; i < 4; i++)
		{
			m_v[i] = qr.m_v[i];
		}
		return *this;
	}
	template<typename T>
	inline  CQuaternion<T>  CQuaternion<T>::operator* (T is)          RESTRICT
	{
		CQuaternion<T> qr;
		qr.m_v[0] = m_v[0] * is;
		qr.m_v[1] = m_v[1] * is;
		qr.m_v[2] = m_v[2] * is;
		qr.m_v[3] = m_v[3] * is;

		return qr;
	}
	template<typename T>
	CQuaternion<T>& CQuaternion<T>::operator*=(T is)            RESTRICT
	{
		m_v[0] *= is;
		m_v[1] *= is;
		m_v[2] *= is;
		m_v[3] *= is;

		return *this;
	}
	template<typename T>
	CQuaternion<T>  CQuaternion<T>::operator/ (T is)            RESTRICT
	{
		CQuaternion<T> qr;
		qr.m_v[0] = m_v[0] / is;
		qr.m_v[1] = m_v[1] / is;
		qr.m_v[2] = m_v[2] / is;
		qr.m_v[3] = m_v[3] / is;

		return qr;
	}
	template<typename T>
	CQuaternion<T>& CQuaternion<T>::operator/=(T is)            RESTRICT
	{
		m_v[0] /= is;
		m_v[1] /= is;
		m_v[2] /= is;
		m_v[3] /= is;

		return *this;
	}
	template<typename T>
	T CQuaternion<T>::len()              RESTRICT
	{
		return sqrtR<T>(m_v[0] * m_v[0] + m_v[1] * m_v[1] + m_v[2] * m_v[2] + m_v[3] * m_v[3]);
	}

	template<typename T>
	CQuaternion<T> CQuaternion<T>::norm()     RESTRICT
	{
		CQuaternion<T> qr;
		T len;
		len = m_v[0] * m_v[0] + m_v[1] * m_v[1] + m_v[2] * m_v[2] + m_v[3] * m_v[3];
		len = sqrtR<T>(len) + 1e-40;
		qr.m_v[0] = m_v[0] / len;
		qr.m_v[1] = m_v[1] / len;
		qr.m_v[2] = m_v[2] / len;
		qr.m_v[3] = m_v[3] / len;

		return qr;
	}
	template<typename T>
	void CQuaternion<T>::selfnorm()       RESTRICT
	{
		T len;
		len = m_v[0] * m_v[0] + m_v[1] * m_v[1] + m_v[2] * m_v[2] + m_v[3] * m_v[3];

		len = sqrtR<T>(len) + 1e-40;
		m_v[0] = m_v[0] / len;
		m_v[1] = m_v[1] / len;
		m_v[2] = m_v[2] / len;
		m_v[3] = m_v[3] / len;
	}

	template<typename T>
	CQuaternion<T> operator-(CQuaternion<T>& qi) RESTRICT
	{
		CQuaternion<T> qr;
		qr.m_v[0] = -qi.m_v[0];
		qr.m_v[1] = -qi.m_v[1];
		qr.m_v[2] = -qi.m_v[2];
		qr.m_v[3] = -qi.m_v[3];

		return qr;
	}
	template<typename T>
	CQuaternion<T> operator*(T ii, CQuaternion<T> &qi) RESTRICT
	{
		CQuaternion<T> qr;
		qr.m_v[0] = ii*qi.m_v[0];
		qr.m_v[1] = ii*qi.m_v[1];
		qr.m_v[2] = ii*qi.m_v[2];
		qr.m_v[3] = ii*qi.m_v[3];

		return qr;
	}

	template<typename T>
	CQuaternion<T> CQuaternion<T>::slerpto(CQuaternion<T> qi, T t) RESTRICT
	{
		CQuaternion<T> qr;
		T cosv;
		T Theta;
		T sinTheta;
		T sinThetat;
		T sinTheta1_t;

		T s1, s2;

		cosv = m_v[0] * qi.m_v[0] + m_v[1] * qi.m_v[1] + m_v[2] * qi.m_v[2] + m_v[3] * qi.m_v[3];
		if (cosv<1)
		{
			Theta = acosR(cosv);
			sinTheta = sinR<T>(Theta);
			sinThetat = sinR<T>(Theta*t);
			sinTheta1_t = sinR<T>(Theta*(1.0f - t));
		}
		else
		{
			sinTheta = 1;
			sinThetat = t;
			sinTheta1_t = 1.0f - t;
		}

		if (cosv<0.0f)
		{
			sinThetat *= -1;
		}
		s1 = sinTheta1_t / sinTheta;
		s2 = sinThetat / sinTheta;

		qr.m_v[0] = s1*m_v[0] + s2*qi.m_v[0];
		qr.m_v[1] = s1*m_v[1] + s2*qi.m_v[1];
		qr.m_v[2] = s1*m_v[2] + s2*qi.m_v[2];
		qr.m_v[3] = s1*m_v[3] + s2*qi.m_v[3];
		return qr;
	}

	//-----------------------------------------//
	//-----------------------------------------//
	//----------CDVector--------------------//
	//-----------------------------------------//
	//-----------------------------------------//

	template<int d, typename Tc >
	CDVector<d, Tc>  CDVector<d, Tc>::operator+ (CDVector<d, Tc> vec) RESTRICT
	{
		CDVector<d, Tc> cret;
		for (int i = 0; i < d; i++)
			cret[i] = vec.m_v[i] + m_v[i];
		return cret;
	}
	template<int d, typename Tc >
	CDVector<d, Tc>  CDVector<d, Tc>::operator- (CDVector<d, Tc> vec) RESTRICT
	{
		CDVector<d, Tc> cret;
		for (int i = 0; i < d; i++)
			cret.m_v[i] = -vec.m_v[i] + m_v[i];
		return cret;
	}
	template<int d, typename Tc >
	CDVector<d, Tc>& CDVector<d, Tc>::operator+=(CDVector<d, Tc> vec) RESTRICT
	{
		for (int i = 0; i < d; i++)
			m_v[i] = vec.m_v[i] + m_v[i];
		return *this;
	}
	template<int d, typename Tc >
	CDVector<d, Tc>& CDVector<d, Tc>::operator-=(CDVector<d, Tc> vec) RESTRICT
	{
		for (int i = 0; i < d; i++)
			m_v[i] = m_v[i] - vec.m_v[i];
		return *this;
	}
	template<int d, typename Tc >
	Tc CDVector<d, Tc>::operator *(CDVector<d, Tc> vec) RESTRICT
	{
		Tc r0 = 0;
		for (int i = 0; i < d; i++)
			r0 += vec.m_v[i] * m_v[i];
		return r0;
	}
	template<int d, typename Tc >
	CDVector<d, Tc> CDVector<d, Tc>::operator &(CDVector<d, Tc> vec) RESTRICT
	{
		CDVector<d, Tc> r;
		for (int i = 0; i<d; i++)
			r[i] = vec.m_v[i] * m_v[i];
		return r;
	}
	template<int d, typename Tc >
	CDVector<d, Tc>& CDVector<d, Tc>::operator&=(CDVector<d, Tc> vec) RESTRICT
	{
		for (int i = 0; i<d; i++)
			m_v[i] *= vec.m_v[i];
		return *this;
	}
	template<int d, typename Tc >
	CDVector<d, Tc>  CDVector<d, Tc>::operator/ (CDVector<d, Tc> vec) RESTRICT
	{
		CDVector<d, Tc> vr;
		for (int i = 0; i<d; i++)
			vr.m_v[i] = m_v[i] / vec.m_v[i];
		return vr;
	}
	template<int d, typename Tc >
	CDVector<d, Tc>& CDVector<d, Tc>::operator/=(CDVector<d, Tc> vec) RESTRICT
	{
		for (int i = 0; i<d; i++)
			m_v[i] /= vec.m_v[i];
		return *this;
	}
	template<typename Tc>
	CDVector<3, Tc> CrossV(CDVector<3, Tc> v1, CDVector<3, Tc> v2) RESTRICT
	{
		CDVector<3, Tc> vr;
		vr.m_v[0] = v1.m_v[1] * v2.m_v[2] - v1.m_v[2] * v2.m_v[1];
		vr.m_v[1] = v1.m_v[2] * v2.m_v[0] - v1.m_v[0] * v2.m_v[2];
		vr.m_v[2] = v1.m_v[0] * v2.m_v[1] - v1.m_v[1] * v2.m_v[0];
		return vr;
	}

	template<typename Tc>
	Tc CrossV(CDVector<2, Tc> v1, CDVector<2, Tc> v2) RESTRICT
	{
		Tc v;
		v = v1.m_v[0] * v2.m_v[1] - v1.m_v[1] * v2.m_v[0];
		return v;
	}

	template<int d, typename Tc >
	CDVector<d, Tc> CDVector<d, Tc>::operator *(Tc is) RESTRICT
	{
		CDVector<d, Tc> cret;
		for (int i = 0; i<d; i++)
			cret.m_v[i] = m_v[i] * is;
		return cret;
	}
	template<int d, typename Tc >
	CDVector<d, Tc>& CDVector<d, Tc>::operator*=(Tc is) RESTRICT
	{
		for (int i = 0; i<d; i++)
			m_v[i] *= is;

		return *this;
	}
	template<int d, typename Tc >
	CDVector<d, Tc>  CDVector<d, Tc>::operator/(Tc is) RESTRICT
	{
		CDVector<d, Tc> cret;
		for (int i = 0; i<d; i++)
			cret.m_v[i] = m_v[i] / is;

		return cret;
	}
	template<int d, typename Tc >
	CDVector<d, Tc>& CDVector<d, Tc>::operator/=(Tc is) RESTRICT
	{
		for (int i = 0; i<d; i++)
			m_v[i] /= is;

		return *this;
	}



	template<int d, typename Tc>
	Tc CDVector<d, Tc>::len()  RESTRICT
	{
		Tc r = 0;
		for (int i = 0; i < d; i++)
		{
			r += m_v[i] * m_v[i];
		}
		return sqrtR<Tc>(r);
	}

	template<int d, typename Tc>
	CDVector<d, Tc> CDVector<d, Tc> ::norm()  RESTRICT
	{
		CDVector<d, Tc> qr;
		Tc len = 0;
		for (int i = 0; i < d; i++)
		{
			len += m_v[i] * m_v[i];
		}
		len = sqrtR<Tc>(len);

		for (int i = 0; i < d; i++)
		{
			qr.m_v[i] = m_v[i] / (len + 1e-40);
		}
		return qr;
	}
	template<int d, typename Tc>
	void CDVector<d, Tc>::selfnorm()     RESTRICT
	{
		Tc len = 0;

		for (int i = 0; i < d; i++)
		{
			len += m_v[i] * m_v[i];
		}
		len = sqrtR<Tc>(len);

		for (int i = 0; i < d; i++)
		{
			m_v[i] = m_v[i] / (len + 1e-40);
		}
	}


	//-------------------------------------------------------//
	//-------------------------------------------------------//
	//-------------------------------------------------------//

	template<int d, typename Tc> class Inverse {
	public:
		static bool f(CDMatrix<d, Tc>& mi) RESTRICT
		{
			CDMatrix<d, Tc> mA, invA;

			Tc tmp;
			int i, j, k;
			mA = mi;
			invA = 1;
			int cmax;
			//Gauss-Jordan
			for (k = 0; k<d; k++)
			{
				//select the maximum pibot.
				cmax = k;
				for (j = k + 1; j<d; j++)
				{
					if (absR<Tc>(mA[j][k]) > absR<Tc>(mA[cmax][k]))
					{
						cmax = j;
					}
				}
				if (cmax != k)
				{
					for (i = 0; i<d; i++)
					{
						tmp = mA[cmax][i];
						mA[cmax][i] = mA[k][i];
						mA[k][i] = tmp;

						tmp = invA[cmax][i];
						invA[cmax][i] = invA[k][i];
						invA[k][i] = tmp;
					}
				}
				tmp = mA[k][k];
				for (i = 0; i<d; i++)
				{
					mA[k][i] /= tmp;
					invA[k][i] /= tmp;
				}
				for (j = 0; j<d; j++)
				{
					if (j != k)
					{
						tmp = mA[j][k] / mA[k][k];
						for (i = 0; i<d; i++)
						{
							mA[j][i] = mA[j][i] - mA[k][i] * tmp;
							invA[j][i] = invA[j][i] - invA[k][i] * tmp;
						}
					}
				}

			}
			mi = invA;
			return true;
		};
	};


	template<typename Tc> class Inverse<3, Tc> {
	public:
		static bool f(CDMatrix<3, Tc>& mi) RESTRICT
		{
			CDMatrix<3, Tc> rm;
			Tc det;
			det = mi.m[0][0] * mi.m[1][1] * mi.m[2][2];
			det += mi.m[1][0] * mi.m[2][1] * mi.m[0][2];
			det += mi.m[2][0] * mi.m[0][1] * mi.m[1][2];
			det -= mi.m[0][2] * mi.m[1][1] * mi.m[2][0];
			det -= mi.m[0][0] * mi.m[1][2] * mi.m[2][1];
			det -= mi.m[1][0] * mi.m[0][1] * mi.m[2][2];
			rm[0][0] = mi.m[1][1] * mi.m[2][2] - mi.m[1][2] * mi.m[2][1];
			rm[0][1] = -mi.m[0][1] * mi.m[2][2] + mi.m[2][1] * mi.m[0][2];
			rm[0][2] = mi.m[0][1] * mi.m[1][2] - mi.m[0][2] * mi.m[1][1];

			rm[1][0] = -mi.m[1][0] * mi.m[2][2] + mi.m[2][0] * mi.m[1][2];
			rm[1][1] = mi.m[0][0] * mi.m[2][2] - mi.m[2][0] * mi.m[0][2];
			rm[1][2] = -mi.m[0][0] * mi.m[1][2] + mi.m[1][0] * mi.m[0][2];

			rm[2][0] = mi.m[1][0] * mi.m[2][1] - mi.m[2][0] * mi.m[1][1];
			rm[2][1] = -mi.m[0][0] * mi.m[2][1] + mi.m[2][0] * mi.m[0][1];
			rm[2][2] = mi.m[0][0] * mi.m[1][1] - mi.m[1][0] * mi.m[0][1];

			for (int i = 0; i<3; i++)
			{
				rm[i][0] /= det;
				rm[i][1] /= det;
				rm[i][2] /= det;
			}

			mi = rm;
			if (det < 0)
				det *= -1;
			if (det < 1e-50)
				return false;

			return true;
		};
	};


	template<typename Tc> class Inverse<2, Tc> {
	public:
		static bool f(CDMatrix<2, Tc>& mi) RESTRICT
		{
			CDMatrix<2, Tc> rm;
			Tc det;
			det = mi.m[0][0] * mi.m[1][1];
			det += mi.m[1][0] * mi.m[0][1];
			rm[0][0] = mi.m[1][1];
			rm[0][1] = -mi.m[0][1];

			rm[1][0] = -mi.m[1][0];
			rm[1][1] = mi.m[0][0];

			for (int i = 0; i<2; i++)
			{
				rm[i][0] /= det;
				rm[i][1] /= det;
			}

			mi = rm;
			if (det < 0)
				det *= -1;
			if (det < 1e-50)
				return false;
			return true;
		};
	};

	template<typename Tc> class Inverse<1, Tc> {
	public:
		static bool f(CDMatrix<1, Tc> mi) RESTRICT
		{

			CDMatrix<1, Tc> rm;
			Tc det = mi.m[0][0];
			rm[0][0] = 1 / det;

			mi = rm;
			if (det < 0)
				det *= -1;
			if (det < 1e-50)
				return false;

			return true;
		};
	};

	template<int d, typename Tc >
	CDMatrix<d, Tc> CDMatrix<d, Tc>::operator+ (CDMatrix<d, Tc> mi) RESTRICT
	{
		CDMatrix<d, Tc> mr;
		for (int i = 0; i<d; i++)
		{
			for (int j = 0; j < d; j++)
			{
				mr[i][j] = m[i][j] + mi[i][j];
			}
		}
		return mr;
	}
	template<int d, typename Tc >
	CDMatrix<d, Tc> CDMatrix<d, Tc>::operator+ (Tc is) RESTRICT
	{
		CDMatrix<d, Tc> mr;

		mr = *this;
		for (int i = 0; i < d; i++)
		{
			mr[i][i] = m[i][i] + is;
		}
		return mr;
	}
	template<int d, typename Tc >
	CDMatrix<d, Tc> CDMatrix<d, Tc>::operator- (CDMatrix<d, Tc> mi) RESTRICT
	{
		CDMatrix<d, Tc> mr;
		for (int i = 0; i<d; i++)
		{
			for (int j = 0; j < d; j++)
				mr[i][j] = m[i][j] - mi[i][j];
		}
		return mr;
	}
	template<int d, typename Tc >
	CDMatrix<d, Tc> CDMatrix<d, Tc>::operator- (Tc is) RESTRICT
	{
		CDMatrix<d, Tc> mr;
		mr = *this;
		for (int i = 0; i < d; i++)
			mr[i][i] = m[i][i] - is;
		return mr;
	}
	template<int d, typename Tc >
	CDMatrix<d, Tc>& CDMatrix<d, Tc>::operator+=(CDMatrix<d, Tc> mi) RESTRICT
	{
		for (int i = 0; i<d; i++)
		{
			for (int j = 0; j < d; j++)
				m[i][j] = m[i][j] + mi[i][j];
		}
		return *this;
	}
	template<int d, typename Tc >
	CDMatrix<d, Tc>& CDMatrix<d, Tc>::operator+=(Tc is) RESTRICT
	{
		for (int i = 0; i < d; i++)
			m[i][i] = m[i][i] + is;
		return *this;
	}
	template<int d, typename Tc >
	CDMatrix<d, Tc>& CDMatrix<d, Tc>::operator-=(CDMatrix<d, Tc> mi) RESTRICT
	{
		for (int i = 0; i<d; i++)
		{
			for (int j = 0; j < d; j++)
				m[i][j] = m[i][j] - mi[i][j];
		}
		return *this;
	}
	template<int d, typename Tc >
	CDMatrix<d, Tc>& CDMatrix<d, Tc>::operator-=(Tc is) RESTRICT
	{
		for (int i = 0; i < d; i++)
			m[i][i] = m[i][i] - is;
		return *this;
	}

	template<int d, typename Tc >
	CDMatrix<d, Tc>  CDMatrix<d, Tc>::operator* (const CDMatrix<d, Tc>& mi) const RESTRICT
	{
		CDMatrix<d, Tc> mr;
		for (int i = 0; i<d; i++)
		{
			for (int j = 0; j < d; j++)
			{
				mr.m[i][j] = 0;
				for (int k = 0; k < d; k++)
					mr.m[i][j] += m[i][k] * mi.m[k][j];
			}
		}
		return mr;
	}
	template<int d, typename Tc >
	CDMatrix<d, Tc>& CDMatrix<d, Tc>::operator*=(CDMatrix<d, Tc> mi) RESTRICT
	{
		Tc f;
		CDMatrix<d, Tc> mr;
		for (int i = 0; i<d; i++)
		{
			for (int j = 0; j < d; j++)
			{
				f = 0;
				for (int k = 0; k < d; k++)
					f += m[i][k] * mi.m[k][j];

				mr[i][j] = f;
			}
		}
		for (int i = 0; i<d; i++)
		{
			for (int j = 0; j < d; j++)
			{
				m[i][j] = mr[i][j];
			}
		}
		return *this;
	}
	template<int d, typename Tc >
	CDMatrix<d, Tc>  CDMatrix<d, Tc>::operator* (Tc is) RESTRICT
	{
		CDMatrix<d, Tc> mr;
		for (int i = 0; i<d; i++)
		{
			for (int j = 0; j < d; j++)
				mr[i][j] = m[i][j] * is;
		}
		return mr;
	}
	template<int d, typename Tc >
	CDMatrix<d, Tc>& CDMatrix<d, Tc>::operator =(CDMatrix<d, Tc> mi) RESTRICT
	{
		for (int i = 0; i < d; i++)
		{
			for (int j = 0; j < d; j++)
			{
				m[i][j] = mi.m[i][j];
			}
		}
		return *this;
	}
	template<int d, typename Tc >
	CDMatrix<d, Tc>& CDMatrix<d, Tc>::operator =(Tc is) RESTRICT
	{
		for (int i = 0; i<d; i++)
		{
			for (int j = 0; j < d; j++)
			{
				if (i == j)
					m[i][j] = is;
				else
					m[i][j] = 0;
			}
		}

		return *this;
	}
	template<int d, typename Tc >
	CDMatrix<d, Tc>& CDMatrix<d, Tc>::operator*=(Tc is) RESTRICT
	{
		for (int i = 0; i<d; i++)
		{
			for (int j = 0; j < d; j++)
			{
				m[i][j] = m[i][j] * is;
			}
		}
		return *this;
	}

	template<int d, typename Tc >
	bool  CDMatrix<d, Tc>::Power_Self(int in) RESTRICT
	{
		CDMatrix<d, Tc> rm, rm2;

		bool is = true;
		if (in == 0)
		{
			rm = 1;
			return true;
		}

		if (in < 0)
		{
			is = Inverse<d, Tc>::f(*this);
			rm2 = rm;
			in *= -1;
		}
		else
		{
			rm = *this;
			rm2 = rm;
		}

		for (int i = 0; i<in - 1; i++)
		{
			rm *= rm2;
		}
		return is;
	}

	template<typename Tc>
	CDMatrix<4, Tc> setLookAt(CDVector<3, Tc> vEye, CDVector<3, Tc> vAt) RESTRICT
	{
		CDMatrix<4, Tc> mi;
		CDVector<3, Tc> vUp(0, 1, 0), vZaxis, vXaxis, vYaxis;

		vZaxis = vAt - vEye;
		vZaxis = vZaxis.norm();


		vXaxis = CrossV(vUp, vZaxis);
		vXaxis = vXaxis.norm();

		vYaxis = CrossV(vZaxis, vXaxis);
		vYaxis = vYaxis.norm();

		mi[0][0] = vXaxis[0];
		mi[0][1] = vXaxis[1];
		mi[0][2] = vXaxis[2];
		mi[0][3] = -vXaxis * vEye;

		mi[1][0] = vYaxis[0];
		mi[1][1] = vYaxis[1];
		mi[1][2] = vYaxis[2];
		mi[1][3] = -vYaxis * vEye;

		mi[2][0] = vZaxis[0];
		mi[2][1] = vZaxis[1];
		mi[2][2] = vZaxis[2];
		mi[2][3] = -vZaxis * vEye;

		mi[3][0] = 0;
		mi[3][1] = 0;
		mi[3][2] = 0;
		mi[3][3] = 1;

		return mi;
	}
	template<typename Tc>
	CDMatrix<4, Tc> setLookAt(CDVector<3, Tc> vEye, CDVector<3, Tc> vAt, CDVector<3, Tc> vUp) RESTRICT
	{
		CDMatrix<4, Tc> mi;
		CDVector<3, Tc> vZaxis, vXaxis, vYaxis;

		vZaxis = vAt - vEye;
		vZaxis = vZaxis.norm();


		vXaxis = CrossV(vUp, vZaxis);
		vXaxis = vXaxis.norm();

		vYaxis = CrossV(vZaxis, vXaxis);
		vYaxis = vYaxis.norm();

		mi[0][0] = vXaxis[0];
		mi[0][1] = vXaxis[1];
		mi[0][2] = vXaxis[2];
		mi[0][3] = -vXaxis * vEye;

		mi[1][0] = vYaxis[0];
		mi[1][1] = vYaxis[1];
		mi[1][2] = vYaxis[2];
		mi[1][3] = -vYaxis * vEye;

		mi[2][0] = vZaxis[0];
		mi[2][1] = vZaxis[1];
		mi[2][2] = vZaxis[2];
		mi[2][3] = -vZaxis * vEye;

		mi[3][0] = 0;
		mi[3][1] = 0;
		mi[3][2] = 0;
		mi[3][3] = 1;

		return mi;
	}


	template<typename Tc>
	CDMatrix<4, Tc> SetQuaternion4D(CQuaternion<Tc> vQ) RESTRICT
	{
		CDMatrix<4, Tc> mi;
		Tc r = vQ.m_v[0];
		Tc x = vQ.m_v[1];
		Tc y = vQ.m_v[2];
		Tc z = vQ.m_v[3];

		mi[0][0] = 1 - 2 * y*y - 2 * z*z;
		mi[0][1] = 2 * x*y - 2 * r*z;
		mi[0][2] = 2 * x*z + 2 * r*y;
		mi[0][3] = 0;

		mi[1][0] = 2 * x*y + 2 * r*z;
		mi[1][1] = 1 - 2 * x*x - 2 * z*z;
		mi[1][2] = 2 * y*z - 2 * r*x;
		mi[1][3] = 0;

		mi[2][0] = 2 * x*z - 2 * r*y;
		mi[2][1] = 2 * y*z + 2 * r*x;
		mi[2][2] = 1 - 2 * x*x - 2 * y*y;
		mi[2][3] = 0;

		mi[3][0] = 0;
		mi[3][1] = 0;
		mi[3][2] = 0;
		mi[3][3] = 1;
		return mi;
	}

	template<typename Tc>
	CDMatrix<3, Tc> SetQuaternion3D(CQuaternion<Tc> vQ) RESTRICT
	{
		CDMatrix<3, Tc> mi;
		Tc r = vQ.m_v[0];
		Tc x = vQ.m_v[1];
		Tc y = vQ.m_v[2];
		Tc z = vQ.m_v[3];

		mi[0][0] = 1 - 2 * y*y - 2 * z*z;
		mi[0][1] = 2 * x*y - 2 * r*z;
		mi[0][2] = 2 * x*z + 2 * r*y;

		mi[1][0] = 2 * x*y + 2 * r*z;
		mi[1][1] = 1 - 2 * x*x - 2 * z*z;
		mi[1][2] = 2 * y*z - 2 * r*x;

		mi[2][0] = 2 * x*z - 2 * r*y;
		mi[2][1] = 2 * y*z + 2 * r*x;
		mi[2][2] = 1 - 2 * x*x - 2 * y*y;

		return mi;
	}

	template<typename Tc>
	CDMatrix<4, Tc> setOrthoLH(Tc w, Tc h, Tc zn, Tc zf) RESTRICT
	{
		CDMatrix<4, Tc> mi;
		mi[0][0] = 2 / w;
		mi[0][1] = 0;
		mi[0][2] = 0;
		mi[0][3] = 0;

		mi[1][0] = 0;
		mi[1][1] = 2 / h;
		mi[1][2] = 0;
		mi[1][3] = 0;

		mi[2][0] = 0;
		mi[2][1] = 0;
		mi[2][2] = 1 / (zf - zn);
		mi[2][3] = zn / (zn - zf);

		mi[3][0] = 0;
		mi[3][1] = 0;
		mi[3][2] = 0;
		mi[3][3] = 1;
		return mi;
	}
	template<typename Tc>
	CDMatrix<4, Tc> setPerspectiveLH(Tc w, Tc h, Tc zn, Tc zf) RESTRICT
	{
		CDMatrix<4, Tc> mi;
		mi[0][0] = 2 * zn / w;
		mi[0][1] = 0;
		mi[0][2] = 0;
		mi[0][3] = 0;

		mi[1][0] = 0;
		mi[1][1] = 2 * zn / h;
		mi[1][2] = 0;
		mi[1][3] = 0;

		mi[2][0] = 0;
		mi[2][1] = 0;
		mi[2][2] = zf / (zn - zf);
		mi[2][3] = zn*zf / (zn - zf);

		mi[3][0] = 0;
		mi[3][1] = 0;
		mi[3][2] = 1;
		mi[3][3] = 0;

		return mi;
	}

	template<typename Tc>
	CDMatrix<4, Tc> setPerspectiveFovLH(Tc fovy, Tc aspect, Tc zn, Tc zf) RESTRICT
	{
		CDMatrix<4, Tc> mi;
		Tc yScale, xScale;

		yScale = 1 / amp_win_math::tan(fovy / 2);
		xScale = yScale / aspect;

		mi[0][0] = xScale;
		mi[0][1] = 0;
		mi[0][2] = 0;
		mi[0][3] = 0;

		mi[1][0] = 0;
		mi[1][1] = yScale;
		mi[1][2] = 0;
		mi[1][3] = 0;

		mi[2][0] = 0;
		mi[2][1] = 0;
		mi[2][2] = zf / (zf - zn);
		mi[2][3] = -zn*zf / (zf - zn);

		mi[3][0] = 0;
		mi[3][1] = 0;
		mi[3][2] = 1;
		mi[3][3] = 0;
		return mi;
	}

	template<typename Tc>
	CDMatrix<4, Tc> setTrans(CDVector<3, Tc> vTrans) RESTRICT
	{
		CDMatrix<4, Tc> mi;
		mi[0][0] = 1;
		mi[0][1] = 0;
		mi[0][2] = 0;
		mi[0][3] = vTrans[0];

		mi[1][0] = 0;
		mi[1][1] = 1;
		mi[1][2] = 0;
		mi[1][3] = vTrans[1];

		mi[2][0] = 0;
		mi[2][1] = 0;
		mi[2][2] = 1;
		mi[2][3] = vTrans[2];

		mi[3][0] = 0;
		mi[3][1] = 0;
		mi[3][2] = 0;
		mi[3][3] = 1;

		return mi;
	}



}