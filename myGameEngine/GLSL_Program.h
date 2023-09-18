#pragma once

#include <string>
#include <GL/glew.h>

namespace myGameEngine {
	class GLSL_Program
	{
	public:
		void compileShaders(const std::string& vertexShaderFilePath,
			const std::string& fragmentShaderFilepath);
		void linkShaders();
		void addAttribute(const std::string& attributeName);
		GLint getUniformLocation(const std::string& uniformName);
		void use();
		void unuse();

	private:

		// Position is 1 attribute, color is another, so if you have both you would have
		// 2 attributes
		int _numAttributes = 0;

		void compileShader(const std::string& filePath, GLuint id);

		GLuint _programID = 0;
		GLuint _vertexShaderID = 0;
		GLuint _fragmentShaderID = 0;
	};
}

