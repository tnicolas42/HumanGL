#ifndef MESH_HPP
# define MESH_HPP

#include "commonInclude.hpp"
#include "Shader.hpp"
# include "Material.hpp"
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
	u_int32_t bonesID[NUM_BONES_PER_VERTEX];
	float bonesW[NUM_BONES_PER_VERTEX];

	VertexMat() :
		pos(mat::Vec3()),
		norm(mat::Vec3()),
		texCoords(mat::Vec2()),
		bonesID{0, 0, 0, 0},
		bonesW{0, 0, 0, 0} {
	}
};
struct Vertex {  // contain pointer on data on matrix object
	float posx, posy, posz;
	float normx, normy, normz;
	float texCoordsx, texCoordsy;
	u_int32_t bonesID[NUM_BONES_PER_VERTEX];
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
		std::vector<Texture> textures, Material material);
		Mesh(Mesh const &src);
		virtual ~Mesh();

		Mesh &operator=(Mesh const &rhs);

		u_int32_t	getVao() const;
		u_int32_t	getVbo() const;
		u_int32_t	getEbo() const;

		void		draw(Shader &sh) const;
		void		addBoneData(u_int32_t boneID, float weight, u_int32_t vertexID);
		void		setupMesh();

		std::vector<VertexMat>	vertices;
		std::vector<u_int32_t>	indices;
		std::vector<Texture>	textures;
		Material				material;
	private:

        u_int32_t	_vao;
        u_int32_t	_vbo;
        u_int32_t	_ebo;
};

#endif
