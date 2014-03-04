#pragma once

#include <GLMatrixStack.h>

#include <list>

#include "GLObject.h"

/*
Structure that contains and relates GLShapes in a tree hiearchy.
*/
class GLObjectSystem
{

	public:
	struct Node
	{
		GLObject object;
		std::list<Node*> children;

		void Draw( );
		~Node( );

		void AddChild( Node* child );
	};

	private:

	// N-ary tree structure
	Node* root;

	public:
	~GLObjectSystem( );

	void SetRoot( Node* root ) { this->root = root; }
	Node* GetRoot( ) { return root; }

	void Draw( );
};

