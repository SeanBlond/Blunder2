#ifndef TRASNFORM
#pragma once

#include "vectors.h"
#include "mat4.h"

namespace smath
{
	class transform
	{
	public:
		// Constructors
		transform(smath::vec3 position = smath::vec3(0), smath::vec3 rotation = smath::vec3(0), smath::vec3 scale = smath::vec3(1)) : position(position), rotation(rotation), scale(scale) {}

		// Modifiers
		// Position
		void addPosition(smath::vec3 position) { this->position += position; }
		void addPosition(float x, float y, float z) { this->position += smath::vec3(x, y, z); }
		void addPosition(float position) { this->position += smath::vec3(position); }
		void addPositionX(float position) { this->position.x += position; }
		void addPositionY(float position) { this->position.y += position; }
		void addPositionZ(float position) { this->position.z += position; }

		// Rotation
		void addRotation(smath::vec3 rotation) { this->rotation += rotation; }
		void addRotation(float x, float y, float z) { this->rotation += smath::vec3(x, y, z); }
		void addRotation(float rotation) { this->rotation += smath::vec3(rotation); }
		void addRotationX(float rotation) { this->rotation.x += rotation; }
		void addRotationY(float rotation) { this->rotation.y += rotation; }
		void addRotationZ(float rotation) { this->rotation.z += rotation; }

		// Scale
		void addScale(smath::vec3 scale) { this->scale += scale; }
		void addScale(float x, float y, float z) { this->scale += smath::vec3(x, y, z); }
		void addScale(float scale) { this->scale += smath::vec3(scale); }
		void addScaleX(float scale) { this->scale.x += scale; }
		void addScaleY(float scale) { this->scale.y += scale; }
		void addScaleZ(float scale) { this->scale.z += scale; }

		// Functions
		transform& operator=(const transform& transform)
		{
			this->position = transform.position;
			this->rotation = transform.rotation;
			this->scale = transform.scale;

			return *this;
		}
		smath::mat4 gettransformMatrix()
		{
			return (smath::mat4() * smath::translate(position)/* * smath::rotate(rotation)*/ * smath::scale(scale));
		}

		smath::vec3 position;
		smath::vec3 rotation;
		smath::vec3 scale;
	};
}

#endif // !TRASNFORM