#ifndef _VERTEX_H_
#define _VERTEX_H_
#include <glm/glm.hpp>


struct Vertex {
    glm::vec2 position;
    glm::vec4 color;
    glm::vec2 tex_coords;
    float tex_idx;
};

#endif // _VERTEX_H_ vertex header
