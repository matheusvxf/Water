#include "Base.h"


Base::Base(const Vector4& u, const Vector4& v, const Vector4& w, const Vector4& origin)
{
	m_trans[0] = u;
	m_trans[1] = v;
	m_trans[2] = w;
	m_trans.setColumn(3, origin);
	m_trans[3][3] = 1;
	m_trans_inv[0] = u;
	m_trans_inv[1] = v;
	m_trans_inv[2] = w;
	m_trans_inv[0][3] = -(u.dot(origin));
	m_trans_inv[1][3] = -(v.dot(origin));
	m_trans_inv[2][3] = -(w.dot(origin));
	m_trans_inv[3][3] = 1;
}

Base::~Base()
{
}

Vector4 Base::transform(const Vector4& p) const
{
	return m_trans * p;
}

Vector4 Base::revert(const Vector4& p_prime) const{
	return m_trans_inv * p_prime;
}
