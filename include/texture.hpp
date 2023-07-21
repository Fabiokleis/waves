#ifndef TEXTURE_H
#define TEXTURE_H
#include "renderer.hpp"

class Texture
{
private:
    unsigned int m_RendererID;
    std::string m_FilePath;
    unsigned char* m_LocationBuffer;
    int m_Width, m_Height, m_BPP;
    
public:
    Texture(const std::string& path);
    ~Texture();

    void Bind(unsigned int slot = 0) const;
    void Unbind() const;

    inline int GetWidth() const { return m_Width; }
    inline int GetHeight() const { return m_Height; }
};


#endif /* end of include guard: TEXTURE_H */
