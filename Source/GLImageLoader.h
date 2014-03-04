#pragma once

#include "GLSharedTypes.h"

enum GLImageFormat{ IMAGE_FORMAT_BMP };

#undef LoadImage

namespace GLImageLoader
{
	GLint LoadImage( const char* file, GLImageFormat format);
};

