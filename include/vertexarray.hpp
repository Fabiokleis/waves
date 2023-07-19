#ifndef VERTEXARRAY_H
#define VERTEXARRAY_H

#include "vertexbuffer.hpp"
#include "vertexbufferlayout.hpp"


class VertexArray
{
private:
    unsigned int m_RendererID;
public:
    VertexArray();
    ~VertexArray();
    void Bind() const;
    void Unbind() const;
    void AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout);
    void AddBufferVertex(const VertexBuffer &vb);
};


#endif /* end of include guard: VERTEXARRAY_H */
