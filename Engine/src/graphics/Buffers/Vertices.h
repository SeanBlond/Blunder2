#ifndef VERTICES
#pragma once

#include <smath/smath.h>
#include "../../Color/Color.h"

// * ------------------ *
// |  Vertices Structs  |
// * ------------------ *

// Position and Color Vertex
struct PosColorVert
{
	smath::vec3 position;
	Color color;
};

// Position, Color, and TexCoord Vertex
struct PosColorTexVert
{
	smath::vec3 position;
	Color color;
	smath::vec2 texCoord;
};

// Position and TexCoord Vertex
struct PosTexVert
{
    smath::vec3 position;
    smath::vec2 texCoord;
};

// * -------------- *
// |  Mesh Structs  |
// * -------------- *

// PosColor Mesh
struct PosColorMesh
{
	std::vector<PosColorVert> vertices;
	std::vector<uint32_t> indices;
};

// PosColorTex Mesh
struct PosColorTexMesh
{
	std::vector<PosColorTexVert> vertices;
	std::vector<int> indices;
};

// PosTex Mesh
struct PosTexMesh
{
	std::vector<PosTexVert> vertices;
	std::vector<int> indices;
};

// * ---------------------- *
// |  Other shared structs  |
// * ---------------------- *

// Struct for containing the texcoord position and sizes of the texture atlas
struct TextureAtlasCoord
{
	smath::vec2 origin;
	smath::vec2 size;
};


#endif // !VERTICES
