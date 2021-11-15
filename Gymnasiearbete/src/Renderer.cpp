#include "Renderer.h"

Renderer::Renderer()
{
    //Would be great to have screen drawing in here...
}
Renderer::~Renderer()
{

}

void Renderer::DrawElements(const VertexArray& va, const IndexBuffer& ib, const Shader& shader)
{
    va.Bind();
    ib.Bind();
    shader.Bind();

    glDrawElements(GL_TRIANGLES, ib.GetCount(), GL_UNSIGNED_INT, nullptr);
}

void Renderer::Clear()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}