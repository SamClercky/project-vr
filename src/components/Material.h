#ifndef OPENGL_MATERIAL_H
#define OPENGL_MATERIAL_H

#include "engine/Texture2D.h"

namespace components {

	struct Material {
        engine::Texture2D diffuseMap;
        engine::Texture2D specularMap;
        float shininess;
	};

}


#endif// !OPENGL_MATERIAL_H
