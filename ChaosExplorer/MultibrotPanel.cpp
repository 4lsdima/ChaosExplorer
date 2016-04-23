#define _CRT_SECURE_NO_WARNINGS
#include <stdexcept>
#include "GL/glew.h"
#include "MultibrotPanel.h"


MultibrotPanel::MultibrotPanel(wxWindow* parent, wxWindowID id, const int* attribList, 
    const wxSize& size,
    std::complex<double> power,
    std::complex<double> ul,
    std::complex<double> lr)
    : ChaosPanel(parent, id, attribList, size),
    m_power(power)
{
    if (power.real() < 1.0) {
        throw std::invalid_argument(
            "The real portion of the power argument for Multibrot is less than 1.0");
    }
    m_upperLeft = std::complex<double>(std::min(ul.real(), lr.real()), std::max(ul.imag(), lr.imag()));
    m_lowerRight = std::complex<double>(std::max(ul.real(), lr.real()), std::min(ul.imag(), lr.imag()));
    if (ul.real() == lr.real()) {
        throw std::invalid_argument(
            "The real portions of the upper-left and lower-right corners of the Multibrot display are equal.");
    }
    if (ul.imag() == lr.imag()) {
        throw std::invalid_argument(
            "The imag portions of the upper-left and lower-right corners of the Multibrot display are equal.");
    }

    Bind(wxEVT_PAINT, &MultibrotPanel::OnPaint, this);
}


MultibrotPanel::~MultibrotPanel()
{
}

void MultibrotPanel::OnPaint(wxPaintEvent& event)
{
    SetContext();
    // set background to black
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glFlush();
    SwapBuffers();
}

