#ifndef TRASNFORM_2D
#pragma once

#include "vectors.h"
#include "mat4.h"

namespace smath
{
	class transform2D
	{
	public:
		// Constructors
		transform2D(smath::vec2 position = smath::vec2(0), uint8_t layer = 0, float rotation = float(0), smath::vec2 scale = smath::vec2(1))
			: position(position), rotation(rotation), scale(scale), layer(layer) {}

		// Modifiers
		// Position
		void addPosition(smath::vec2 position) { this->position += position; }
		void addPosition(float x, float y) { this->position += smath::vec2(x, y); }
		void addPosition(float position) { this->position += smath::vec2(position); }
		void addPositionX(float position) { this->position.x += position; }
		void addPositionY(float position) { this->position.y += position; }

		// Rotation
		void addRotation(float rotation) { this->rotation += rotation; }

		// Scale
		void addScale(smath::vec2 scale) { this->scale += scale; }
		void addScale(float x, float y) { this->scale += smath::vec2(x, y); }
		void addScale(float scale) { this->scale += smath::vec2(scale); }
		void addScaleX(float scale) { this->scale.x += scale; }
		void addScaleY(float scale) { this->scale.y += scale; }

		// Functions
		transform2D& operator=(const transform2D& transform)
		{
			this->position = transform.position;
			this->layer = transform.layer;
			this->rotation = transform.rotation;
			this->scale = transform.scale;

			return *this;
		}
		smath::mat4 getTransformMatrix()
		{
			// Applying transformations
			smath::mat4 transformMatrix = 
				smath::mat4() * 
				smath::translate(smath::vec3(position, (float)layer / 255.0f)) * 
				smath::rotateZ(smath::DEG2RAD * rotation) * 
				smath::scale(smath::vec3(scale, 1.0f));

			// Applying parent transformation (if applicable)
			if (parent)
				transformMatrix *= parent->getTransformMatrix();

			return transformMatrix;
		}

		smath::vec2 position;
		int layer;
		smath::vec2 scale;
		float rotation;
		transform2D* parent;
	};
}

#endif // !TRASNFORM_2D