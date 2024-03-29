#pragma once

#include <string>
#include <vector>
#include "Shader.hpp"

/*
name:
SKYBOX_START SKYBOX_NAME_RIGHT SKYBOX_EXT
ex:
	#define SKYBOX_START "skybox/"
	#define SKYBOX_EXT ".jpg"
	#define SKYBOX_NAME_RIGHT "right"
	-> skybox/right.jpg
ex:
	#define SKYBOX_START "skybox/cc_"
	#define SKYBOX_EXT ".jpg"
	#define SKYBOX_NAME_RIGHT "rt"
	-> skybox/cc_rt.jpg
*/
#define SKYBOX_USING 1
#if SKYBOX_USING == 0
	#define SKYBOX_START	"skybox/skybox0/"
	#define SKYBOX_EXT		".jpg"
#else
	#define SKYBOX_START	"skybox/skybox1/miramar_"
	#define SKYBOX_EXT		".tga"
#endif

#define SKYBOX_NAME_TYPE SKYBOX_USING
#if SKYBOX_NAME_TYPE == 0
	#define SKYBOX_NAME_RIGHT	"right"
	#define SKYBOX_NAME_LEFT	"left"
	#define SKYBOX_NAME_TOP		"top"
	#define SKYBOX_NAME_BOTTOM	"bottom"
	#define SKYBOX_NAME_FRONT	"front"
	#define SKYBOX_NAME_BACK	"back"
#else  // http://www.custommapmakers.org/skyboxes.php#
	#define SKYBOX_NAME_RIGHT	"ft"
	#define SKYBOX_NAME_LEFT	"bk"
	#define SKYBOX_NAME_TOP		"up"
	#define SKYBOX_NAME_BOTTOM	"dn"
	#define SKYBOX_NAME_FRONT	"rt"
	#define SKYBOX_NAME_BACK	"lf"
#endif

class Skybox {
	public:
		explicit Skybox(Shader &sh);
		Skybox(Skybox const &src);
		virtual ~Skybox();

		Skybox &operator=(Skybox const &rhs);

		void load(std::vector<std::string> &faces);
		void draw();

		Shader		&getShader();
		Shader		&getShader() const;
		uint32_t	getTextureID() const;
		uint32_t	getVao() const;
		uint32_t	getVbo() const;
	protected:
	private:
		Shader		&_shader;
		uint32_t	_textureID;
		uint32_t	_vao;
		uint32_t	_vbo;

		static const float _vertices[];
};
