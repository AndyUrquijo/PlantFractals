#include "GLObjectSystem.h"

#include "GLRenderer.h"



GLObjectSystem::~GLObjectSystem()
{
	if (root)
		delete root;
}



void GLObjectSystem::Draw()
{
	if (root) 
		root->Draw();
}

// -------------
// Node
// -------------

GLObjectSystem::Node::~Node()
{
	for (auto it = children.begin(); it != children.end(); ++it)
		delete *it;
}

void GLObjectSystem::Node::Draw()
{
	GLRenderer& renderer = GLRenderer::GetInstance();
	MatrixStack& stack = renderer.GetMatrixStack();

	stack.PushMatrix();
		
	object.Draw();

	for (auto it = children.begin(); it != children.end(); ++it)
		(*it)->Draw();

	stack.PopMatrix();
}

void GLObjectSystem::Node::AddChild(Node* child)
{
	children.push_back(child);
}
