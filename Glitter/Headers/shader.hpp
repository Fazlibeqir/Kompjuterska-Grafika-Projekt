#pragma once

using namespace std;

// Stdandard libraries
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
// GL libraries
#include <glm/glm.hpp>
#include <glad/glad.h>
class Shader
{
public:
    GLuint Program;

    Shader(const GLchar* vertexPath, const GLchar* fragmentPath)
    {

        string vertexCode;
        string fragmentCode;
        ifstream vShaderFile;
        ifstream fShaderFile;

        vShaderFile.exceptions (ifstream::failbit | ifstream::badbit);
        fShaderFile.exceptions (ifstream::failbit | ifstream::badbit);
        try
        {
            vShaderFile.open(vertexPath);
            fShaderFile.open(fragmentPath);
            stringstream vShaderStream, fShaderStream;
            vShaderStream << vShaderFile.rdbuf();
            fShaderStream << fShaderFile.rdbuf();
            vShaderFile.close();
            fShaderFile.close();
            vertexCode = vShaderStream.str();
            fragmentCode = fShaderStream.str();
        }
        catch (ifstream::failure e)
        {
            cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << endl;
        }

        const GLchar* vShaderCode = vertexCode.c_str();
        const GLchar * fShaderCode = fragmentCode.c_str();

        GLuint vertex, fragment;

        // Vertex Shader
        vertex = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertex, 1, &vShaderCode, NULL);
        glCompileShader(vertex);
        checkCompileErrors(vertex, "VERTEX");

        // Fragment Shader
        fragment = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragment, 1, &fShaderCode, NULL);
        glCompileShader(fragment);
        checkCompileErrors(fragment, "FRAGMENT");

        // Shader Program creation
        this->Program = glCreateProgram();
        glAttachShader(this->Program, vertex);
        glAttachShader(this->Program, fragment);
        glLinkProgram(this->Program);
        checkCompileErrors(this->Program, "PROGRAM");

        glDeleteShader(vertex);
        glDeleteShader(fragment);
    }

    void Use() { glUseProgram(this->Program); }

    void Delete() {    glDeleteProgram(this->Program); }

    void setBool(const std::string &name, bool value) const {
        glUniform1i(glGetUniformLocation(Program, name.c_str()), (int)value); }

    void setInt(const std::string &name, int value) const {
        glUniform1i(glGetUniformLocation(Program, name.c_str()), value); }

    void setFloat(const std::string &name, float value) const {
        glUniform1f(glGetUniformLocation(Program, name.c_str()), value); }

    void setVec2(const std::string &name, const glm::vec2 &value) const {
        glUniform2fv(glGetUniformLocation(Program, name.c_str()), 1, &value[0]); }

    void setVec2(const std::string &name, float x, float y) const {
        glUniform2f(glGetUniformLocation(Program, name.c_str()), x, y); }

    void setVec3(const std::string &name, const glm::vec3 &value) const {
        glUniform3fv(glGetUniformLocation(Program, name.c_str()), 1, &value[0]); }

    void setVec3(const std::string &name, float x, float y, float z) const {
        glUniform3f(glGetUniformLocation(Program, name.c_str()), x, y, z); }

    void setVec4(const std::string &name, const glm::vec4 &value) const {
        glUniform4fv(glGetUniformLocation(Program, name.c_str()), 1, &value[0]); }

    void setVec4(const std::string &name, float x, float y, float z, float w) const {
        glUniform4f(glGetUniformLocation(Program, name.c_str()), x, y, z, w); }

    void setMat2(const std::string &name, const glm::mat2 &mat) const {
        glUniformMatrix2fv(glGetUniformLocation(Program, name.c_str()), 1, GL_FALSE, &mat[0][0]); }

    void setMat3(const std::string &name, const glm::mat3 &mat) const {
        glUniformMatrix3fv(glGetUniformLocation(Program, name.c_str()), 1, GL_FALSE, &mat[0][0]); }

    void setMat4(const std::string &name, const glm::mat4 &mat) const {
        glUniformMatrix4fv(glGetUniformLocation(Program, name.c_str()), 1, GL_FALSE, &mat[0][0]); }

private:

    void checkCompileErrors(GLuint shader, string type)
    {
        GLint success;
        GLchar infoLog[1024];
        if(type != "PROGRAM")
        {
            glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
            if(!success)
            {
                glGetShaderInfoLog(shader, 1024, NULL, infoLog);
                cout << "| ERROR::::SHADER-COMPILATION-ERROR of type: " << type << "|\n" << infoLog << "\n| -- --------------------------------------------------- -- |" << endl;
            }
        }
        else
        {
            glGetProgramiv(shader, GL_LINK_STATUS, &success);
            if(!success)
            {
                glGetProgramInfoLog(shader, 1024, NULL, infoLog);
                cout << "| ERROR::::PROGRAM-LINKING-ERROR of type: " << type << "|\n" << infoLog << "\n| -- --------------------------------------------------- -- |" << endl;
            }
        }
    }
};