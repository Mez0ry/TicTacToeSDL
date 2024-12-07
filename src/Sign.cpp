#include "Sign.hpp"

Sign::Sign(const Core::Ref<Renderer> renderer, const std::string& path,SignType type){
  m_Texture.LoadTexture(renderer,path);
  SetSignType(type);
}

Sign::~Sign(){
    
}

Texture& Sign::GetTexture(){
    return m_Texture;
}

Vec2i& Sign::GetPosition(){
 return m_Pos;
}

void Sign::SetPosition(const Vec2i &pos){
    m_Pos = pos;
}

const Vec2i& Sign::GetPosition() const{
    return m_Pos;
}

void Sign::SetSize(const ObjectSize &size){
    m_Texture.SetSize(size);
}

const ObjectSize Sign::GetSize() const{
    return m_Texture.GetSize();
}