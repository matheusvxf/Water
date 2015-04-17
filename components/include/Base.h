#pragma once

#include "Vector4.h"
#include "Matrix4.h"

class Base
{
private:
	Matrix4 m_trans;
	Matrix4 m_trans_inv;
public:
	Base(const Vector4& u, const Vector4& v, const Vector4& w, const Vector4& origin);
	~Base();

	Vector4 transform(const Vector4& p) const;
	Vector4 revert(const Vector4& p_prime) const;
};

