#ifndef MESH_HPP
# define MESH_HPP

#include "commonInclude.hpp"
#include "Shader.hpp"
#include <vector>
#include <map>

enum class TextureT {
	difuse,
	specular,
};

static const std::string	gTextType[] =
{
	"diffuse",
	"specular"
};
struct VertexMat {  // contain matrix objects
	mat::Vec3	pos;
	mat::Vec3	norm;
	mat::Vec2	texCoords;
	uint bonesID[NUM_BONES_PER_VERTEX];
	float bonesW[NUM_BONES_PER_VERTEX];
};
struct Vertex {  // contain pointer on data on matrix object
	float posx, posy, posz;
	float normx, normy, normz;
	float texCoordsx, texCoordsy;
	uint bonesID[NUM_BONES_PER_VERTEX];
	float bonesW[NUM_BONES_PER_VERTEX];
};

struct Texture {
	u_int32_t	id;
	TextureT	type;
	std::string	path;
};

class Mesh {
	public:
		Mesh(std::vector<VertexMat> vertices, std::vector<u_int32_t> indices, \
		std::vector<Texture> textures);
		Mesh(Mesh const &src);
		virtual ~Mesh();

		Mesh &operator=(Mesh const &rhs);

		u_int32_t	getVao() const;
		u_int32_t	getVbo() const;
		u_int32_t	getEbo() const;

		void		draw(Shader &shader) const;
		void		addBoneData(uint boneID, float weight, GLuint vertexID);

		std::vector<VertexMat>		vertices;
		std::vector<u_int32_t>	indices;
		std::vector<Texture>	textures;
	private:
        void	_setupMesh();

        u_int32_t	_vao;
        u_int32_t	_vbo;
        u_int32_t	_ebo;
};

#endif
