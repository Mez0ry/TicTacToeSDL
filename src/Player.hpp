#ifndef __PLAYER_HPP__
#define __PLAYER_HPP__
#include "Sign.hpp"
#include <vector>
#include "Core.hpp"

class Player {
public:
  Player() = default;
  virtual ~Player() {}

  void Setup(const Core::Ref<Renderer> renderer,const std::string& sign_path,SignType type, bool is_bot = false);

  std::vector<Core::Ref<Sign>>& GetSigns() {return m_Signs;}
  uint32_t GetReadPos() const {return m_ReadPos;}
  
  Core::Ref<Sign> GetReadSign() {
    return (m_ReadPos < m_Signs.size()) ? m_Signs[m_ReadPos++] : nullptr;
  }

  void DecreaseReadPos() {--m_ReadPos;}

  SignType GetSignType() const{
   return m_Signs.front()->GetSignType();
  }

  bool IsBot() const {return m_bIsBot;}

  void OnDestroy();
private:
  std::vector<Core::Ref<Sign>> m_Signs;
  uint32_t m_ReadPos = 0;
  uint32_t m_SignsSize = 5;
  SignType m_SignType;
  bool m_bIsBot;
};
#endif //! __PLAYER_HPP__