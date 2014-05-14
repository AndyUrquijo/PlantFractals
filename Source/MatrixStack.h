#pragma once

#include "Math.h"
using namespace Math;

class GLMatrixStack;

#include <stack>
using std::stack;
//Wrapper class to Richard Wright's Matrix Stack. To be replaced with own code.
class MatrixStack
{
	
	stack<Matrix44> matrices;
	Matrix44 current;
	public:
	MatrixStack();
	~MatrixStack();

	void PushMatrix();
	void PopMatrix();
	Matrix44 GetMatrix() const;
	void MultMatrix(const Matrix44& matrix);
};

