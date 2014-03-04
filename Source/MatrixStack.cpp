#include "MatrixStack.h"

#include <GLMatrixStack.h>

MatrixStack::MatrixStack( )
{
	matrixStack = new GLMatrixStack;
}


MatrixStack::~MatrixStack( )
{
	delete matrixStack;
}

void MatrixStack::PushMatrix( )
{
	matrixStack->PushMatrix();
}

void MatrixStack::PopMatrix( )
{
	matrixStack->PopMatrix();
}

void MatrixStack::MultMatrix( const Matrix44& matrix )
{
	matrixStack->MultMatrix(matrix.elm);
}

Matrix44 MatrixStack::GetMatrix( ) const
{
	return matrixStack->GetMatrix();
}
