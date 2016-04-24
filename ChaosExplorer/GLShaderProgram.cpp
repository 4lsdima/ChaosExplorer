#define _CRT_SECURE_NO_WARNINGS
#include "GLShaderProgram.h"

GLShaderProgram::GLShaderProgram(ChaosPanel& canvas, std::vector<GLuint>& shaders)
    : m_canvas(&canvas)
{
    m_canvas->SetContext();
    m_program = glCreateProgram();
    for (GLuint shader : shaders) {
        glAttachShader(m_program, shader);
    }
    Link();
}


GLShaderProgram::~GLShaderProgram()
{
    glDeleteProgram(m_program);
}

void GLShaderProgram::BindFragmentDataLocations(const std::vector<std::string>& varNames)
{
    m_canvas->SetContext();
    for (const std::string& name : varNames) {
        glBindFragDataLocation(m_program, 0, name.c_str());
    }
    Link();
}

void GLShaderProgram::Link()
{
    glLinkProgram(m_program);
    // check shader compile status, and throw exception if compile failed
    GLint status = GL_TRUE;
    glGetShaderiv(m_program, GL_LINK_STATUS, &status);
    if (status != GL_TRUE) {
        // Program Info Log is empty
        // so don't try to read it
        throw std::logic_error("Linking of shader program failed.");
    }

}
