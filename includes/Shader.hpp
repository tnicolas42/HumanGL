#ifndef SHADER_HPP
# define SHADER_HPP

# include "commonInclude.hpp"
# include <fstream>
# include <sstream>

/*
	Shader class used to manage shader compilation
	It also adds some tools to set uniform and activate shader easier

	Warning! before instantiating a Shader object you need to create the opengl contex
	with glfwCreateWindow
*/
class Shader {
	public:
		Shader(const char *vsPath, const char *fsPath, const char *gsPath = nullptr);
		Shader(Shader const &src);
		virtual ~Shader();

		Shader &operator=(Shader const &rhs);

		void	use();
		void	setBool(const std::string &name, bool value) const;
		void	setInt(const std::string &name, int value) const;
		void	setFloat(const std::string &name, float value) const;
		void	setVec2(const std::string &name, float x, float y) const;
		void	setVec3(const std::string &name, float x, float y, float z) const;
		void	setVec4(const std::string &name, float x, float y, float z, float w);
		// need to add matrix uniform functions when created
		void	setMat2(const std::string &name, const glm::mat2 &mat) const;
		void	setMat3(const std::string &name, const glm::mat3 &mat) const;
		void	setMat4(const std::string &name, const glm::mat4 &mat) const;

		u_int32_t	id;

		class ShaderCompileException : public std::exception {
			public:
				virtual const char* what() const throw();
		};
		class ShaderLinkingException : public std::exception {
			public:
				virtual const char* what() const throw();
		};
	private:
		void	checkCompileErrors(GLuint shader, std::string type);
};

#endif
