#pragma once
#include "wx/wxprec.h"
#include <memory>
#include <glm/gtc/type_ptr.hpp>
#include "GLShader.h"
#include "wx/glcanvas.h"

class GLShaderProgram;

class ChaosPanel :
    public wxGLCanvas
{
public:
    ChaosPanel(wxWindow* parent, wxWindowID id, const int* attribList,
        const wxSize& size = { 800, 600 });
    ChaosPanel(const ChaosPanel&) = delete;
    ChaosPanel(ChaosPanel&&) = delete;
    virtual ~ChaosPanel();
    ChaosPanel& operator=(const ChaosPanel&) = delete;
    ChaosPanel& operator=(ChaosPanel&&) = delete;
    void InitializeGLEW();
    void SetContext() const noexcept { SetCurrent(*m_context); }


protected:
    virtual void BuildVertexShader() = 0;
    virtual void BuildFragmentShader() = 0;
    virtual void BuildShaderProgram() = 0;
    std::unique_ptr<GLShader> m_vertexShader;
    std::unique_ptr<GLShader> m_fragmentShader;
    std::unique_ptr<GLShaderProgram> m_program;
    void SetupTriangles(std::vector<glm::vec4>& vert, GLint prog);

private:
    virtual void OnPaint(wxPaintEvent& event) = 0;
    std::unique_ptr<wxGLContext> m_context;
    GLuint m_vbo;
    GLuint m_vao;
};

