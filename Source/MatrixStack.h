#pragma once

#include "Math.h"
using namespace Math;

class GLMatrixStack;


//Wrapper class to Richard Wright's Matrix Stack. To be replaced with own code.
class MatrixStack
{
	GLMatrixStack* matrixStack;
	
	public:
	MatrixStack();
	~MatrixStack();

	void PushMatrix();
	void PopMatrix();
	Matrix44 GetMatrix() const;
	void MultMatrix(const Matrix44& matrix);
};

