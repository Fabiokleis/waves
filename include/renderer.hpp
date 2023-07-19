#ifndef RENDERER_H_
#define RENDERER_H_

#include <GL/glew.h>
#include "vertexarray.hpp"
#include "indexbuffer.hpp"
#include "shader.hpp"

class Renderer
{
private:
public:
    void Clear() const;
    void Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const;
};

#endif /* end of include guard: RENDERER_H_HWLOAG95 */
