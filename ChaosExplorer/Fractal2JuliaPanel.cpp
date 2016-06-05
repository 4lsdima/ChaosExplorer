#include "wx/notebook.h"
#include <stdexcept>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "GL/glew.h"
//#include "GLFractal2JuliaShaderProgram.h"
#include "GLSquareShaderProgram.h"
#include "Fractal2JuliaPanel.h"
#include "ChaosExplorerWindow.h"

extern std::vector<glm::vec4> colors;

Fractal2JuliaPanel::Fractal2JuliaPanel(wxWindow* parent, wxWindowID id, const int* attribList,
    const wxSize& size,
    std::complex<float> power,
    std::complex<float> c,
    std::complex<float> ul,
    std::complex<float> lr)
    : PlottingZ0Panel<GLFractal2JuliaShaderProgram>(parent, id, attribList, size, power, c, ul, lr)
{
    if (power.real() < 1.0f) {
        throw std::invalid_argument(
            "The real portion of the power argument for Julia Set is less than 1.0");
    }

    // create popup menu
    CreatePopupMenu();

    // set up GL stuff
    CreateShaderProgram();
    SetupTriangles();
    SetupSquareArrays();
    glUseProgram(GetShaderProgram()->GetProgramHandle());
    GLFractal2JuliaShaderProgram* prog = dynamic_cast<GLFractal2JuliaShaderProgram*>(GetShaderProgram());
    glUniform2f(prog->GetUniformHandle("c"), c.real(), c.imag());
    glUniform2f(prog->GetUniformHandle("p"), power.real(), power.imag());
    glUniform2f(prog->GetUniformHandle("viewDimensions"), size.x, size.y);
    glUniform4fv(prog->GetUniformHandle("color[0]"), colors.size() * 4, &colors[0].x);
}


Fractal2JuliaPanel::~Fractal2JuliaPanel()
{
}

void Fractal2JuliaPanel::CreatePopupMenu()
{
    wxMenu* popup = new wxMenu;
    SetPopupMenu(popup);
    popup->Append(ID_DRAWFROMSELECTION, L"Draw From Selection");
    popup->Append(ID_DELETESELECTION, L"Deselect Selection");
    popup->AppendSeparator();
    popup->Append(ID_PRECLOSETAB, L"Close Tab");
    Bind(wxEVT_COMMAND_MENU_SELECTED, &Fractal2JuliaPanel::OnDrawFromSelection,
        this, ID_DRAWFROMSELECTION);
    Bind(wxEVT_COMMAND_MENU_SELECTED, &Fractal2JuliaPanel::OnDeleteSelection,
        this, ID_DELETESELECTION);
    Bind(wxEVT_COMMAND_MENU_SELECTED, [this](wxCommandEvent& event) {
        // Closing a tab must be done after popup menu has closed.
        // Otherwise, assert fails in wxMenuBase::SetInvokingWindow()
        CallAfter(&ChaosPanel::OnCloseTab); },
        ID_PRECLOSETAB);
    Bind(wxEVT_MENU_OPEN, &Fractal2JuliaPanel::OnMenuOpen, this);
}

void Fractal2JuliaPanel::OnDrawFromSelection(wxCommandEvent& event)
{
    std::complex<float> ul;
    std::complex<float> lr;
    CalculateUpperLeftAndLowerRight(ul, lr);
    // create and display a new MultibrotPanel for the display
    wxNotebook* nBook = dynamic_cast<wxNotebook*>(GetParent());
    if (nBook == nullptr) {
        throw std::logic_error("Could not retrieve the Notebook for the new MandelJuliaPanel.");
    }
    Fractal2JuliaPanel* mPanel = new Fractal2JuliaPanel(nBook, wxID_ANY, nullptr, GetSize(), GetPower(), GetC(), ul, lr);
    nBook->AddPage(mPanel, L"Mandelbrot-Julia", true);
}

void Fractal2JuliaPanel::OnDeleteSelection(wxCommandEvent& event)
{
    // to delete the selection, just set leftDown and leftUp positions to the same value
    SetLeftDown({ 0, 0 });
    SetLeftUp({ 0, 0 });
    Refresh();
}

void Fractal2JuliaPanel::OnMenuOpen(wxMenuEvent& event)
{
    // enable/disable the various popup menu items
    wxPoint leftDown = GetLeftDown();
    wxPoint leftUp = GetLeftUp();
    wxMenu* popup = GetPopupMenu();
    popup->Enable(ID_DRAWFROMSELECTION, leftDown != leftUp);
    popup->Enable(ID_DELETESELECTION, leftDown != leftUp);
    wxNotebook* noteBook = dynamic_cast<wxNotebook*>(GetParent());
    int tabCount = noteBook->GetPageCount();
    popup->Enable(ID_PRECLOSETAB, tabCount > 1);
}
