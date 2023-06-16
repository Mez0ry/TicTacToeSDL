#include "Player.hpp"

void Player::Setup(const Base::Ref<Renderer> renderer,const std::string& sign_path,SignType type, bool is_bot) {
  m_renderer = renderer;
  m_Signs.reserve(m_SignsSize);
  
  auto texture = Texture().LoadTexture(renderer,sign_path);
  
  for (uint32_t i = 0;i < m_SignsSize;i++) {
    m_Signs.push_back(Base::CreateRef<Sign>(renderer,sign_path,type));
    m_Signs[i]->GetTexture().ShareSDLTexture(texture);
    m_Signs[i]->SetPosition({-1,-1});
  }
  m_SignType = type;
  m_bIsBot = is_bot;
}

void Player::OnDestroy(){
  m_ReadPos = 0;
  for(auto& sign : m_Signs){
    sign->SetPosition({-1,-1});
    sign->GetTexture().SetPosition({-1,-1});
  }
}