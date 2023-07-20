#pragma once

#include <glad/glad.h>
#include <glitter.hpp>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
//glShaderSource(vertexShader, 1, &vertexShaderCode, nullptr);
class Shader
{
public:
    unsigned int ID{};
    Shader()= default;
    Shader(const char* vertexPath, const char* fragmentPath)
    {
        std::string vertexCode;
        std::string fragmentCode;
        std::ifstream vShaderFile;
        std::ifstream fShaderFile;

        vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
        fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
        try
        {
            vShaderFile.open(vertexPath);
            fShaderFile.open(fragmentPath);
            std::stringstream vShaderStream, fShaderStream;

            vShaderStream << vShaderFile.rdbuf();
            fShaderStream << fShaderFile.rdbuf();

            vShaderFile.close();
            fShaderFile.close();

            vertexCode = vShaderStream.str();
            fragmentCode = fShaderStream.str();
        }
        catch (std::ifstream::failure& e)
        {
            std::cout << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ: " << e.what() << std::endl;
        }
        const char* vShaderCode = vertexCode.c_str();
        const char* fShaderCode = fragmentCode.c_str();

        unsigned int vertex, fragment;

        vertex = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertex, 1, &vShaderCode, nullptr);
        glCompileShader(vertex);
        checkCompileErrors(vertex, "VERTEX");

        fragment = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragment, 1, &fShaderCode, nullptr);
        glCompileShader(fragment);
        checkCompileErrors(fragment, "FRAGMENT");

        ID = glCreateProgram();
        glAttachShader(ID, vertex);
        glAttachShader(ID, fragment);
        glLinkProgram(ID);
        checkCompileErrors(ID, "PROGRAM");

        glDeleteShader(vertex);
        glDeleteShader(fragment);
    }
    Shader(const Shader& other)
    {
        // Copy the shader program ID
        ID = glCreateProgram();

        // Copy the shader source code from the original shader
        glUseProgram(other.ID);
        GLint shaderCount;
        glGetProgramiv(other.ID, GL_ATTACHED_SHADERS, &shaderCount);
        auto shaders = new GLuint[shaderCount];
        glGetAttachedShaders(other.ID, shaderCount, nullptr, shaders);

        for (int i = 0; i < shaderCount; ++i) {
            GLuint shader = shaders[i];
            GLint shaderType;
            glGetShaderiv(shader, GL_SHADER_TYPE, &shaderType);

            GLuint newShader = glCreateShader(shaderType);
            glAttachShader(ID, newShader);

            GLint sourceLength;
            glGetShaderiv(shader, GL_SHADER_SOURCE_LENGTH, &sourceLength);
            auto shaderSource = new GLchar[sourceLength];
            glGetShaderSource(shader, sourceLength, nullptr, shaderSource);
            const GLchar* shaderSourcePtr = shaderSource;
            glShaderSource(newShader, 1, &shaderSourcePtr, nullptr);

            glCompileShader(newShader);

            delete[] shaderSource;
        }

        glLinkProgram(ID);

        // Clean up the temporary shader array
        delete[] shaders;

        // Copy any other member variables if there are any
    }

    Shader& operator=(const Shader& other)
    {
        if (this != &other) {
            // Delete the existing shader program
            glDeleteProgram(ID);

            // Copy the shader program ID
            ID = glCreateProgram();

            // Copy the shader source code from the original shader
            glUseProgram(other.ID);
            GLint shaderCount;
            glGetProgramiv(other.ID, GL_ATTACHED_SHADERS, &shaderCount);
            auto shaders = new GLuint[shaderCount];
            glGetAttachedShaders(other.ID, shaderCount, nullptr, shaders);

            for (int i = 0; i < shaderCount; ++i) {
                GLuint shader = shaders[i];
                GLint shaderType;
                glGetShaderiv(shader, GL_SHADER_TYPE, &shaderType);

                GLuint newShader = glCreateShader(shaderType);
                glAttachShader(ID, newShader);

                GLint sourceLength;
                glGetShaderiv(shader, GL_SHADER_SOURCE_LENGTH, &sourceLength);
                auto shaderSource = new GLchar[sourceLength];
                glGetShaderSource(shader, sourceLength, nullptr, shaderSource);
                const GLchar* shaderSourcePtr = shaderSource;
                glShaderSource(newShader, 1, &shaderSourcePtr, nullptr);

                glCompileShader(newShader);

                delete[] shaderSource;
            }

            glLinkProgram(ID);

            // Clean up the temporary shader array
            delete[] shaders;

            // Copy any other member variables if there are any
        }

        return *this;
    }

    ~Shader()
    {
        glDeleteProgram(ID);
    }
    void use() const
    {
        glUseProgram(ID);
    }

    void setBool(const std::string& name, bool value) const
    {
        glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
    }

    void setInt(const std::string& name, int value) const
    {
        glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
    }

    void setFloat(const std::string& name, float value) const
    {
        glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
    }

    void setMat4(const std::string& name, glm::mat4 value) const
    {
        glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, glm::value_ptr(value));
    }
    void setVec2(const std::string& name, glm::vec2 vec2) const
    {
        glUniform2f(glGetUniformLocation(ID, name.c_str()), vec2.x, vec2.y);
    }

    void setVec3(const std::string& name, float f0, float f1, float f2) const
    {
        glUniform3f(glGetUniformLocation(ID, name.c_str()), f0, f1, f2);
    }
    void setVec3(const std::string& name, glm::vec3 vec3) const
    {
        glUniform3f(glGetUniformLocation(ID, name.c_str()), vec3.x, vec3.y, vec3.z);
    }

private:

    static void checkCompileErrors(unsigned int shader, const std::string& type)
    {
        int success;
        char infoLog[1024];
        if (type != "PROGRAM")
        {
            glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
            if (!success)
            {
                glGetShaderInfoLog(shader, 1024, nullptr, infoLog);
                std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
            }
        }
        else
        {
            glGetProgramiv(shader, GL_LINK_STATUS, &success);
            if (!success)
            {
                glGetProgramInfoLog(shader, 1024, nullptr, infoLog);
                std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
            }
        }
    }
};