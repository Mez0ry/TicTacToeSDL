#ifndef __PLAYER_HPP__
#define __PLAYER_HPP__
#include "Sign.hpp"
#include <vector>
#include "Base.hpp"

class PlayerMemento;

class Player {
private:
friend PlayerMemento;
public:
  Player() = default;
  virtual ~Player() {}

  void Setup(const Base::Ref<Renderer> renderer,const std::string& sign_path,SignType type, bool is_bot = false);

  std::vector<Base::Ref<Sign>>& GetSigns() {return m_Signs;}
  uint32_t GetReadPos() const {return m_ReadPos;}
  
  Base::Ref<Sign> GetReadSign() {
    return (m_ReadPos < m_Signs.size()) ? m_Signs[m_ReadPos++] : nullptr;
  }
  void DecreaseReadPos() {--m_ReadPos;}

  SignType GetSignType() const{
   return m_Signs.front()->GetSignType();
  }
  bool IsBot() const {return m_bIsBot;}

  void OnDestroy();
private:
  std::vector<Base::Ref<Sign>> m_Signs;
  uint32_t m_ReadPos = 0;
  uint32_t m_SignsSize = 5;
  SignType m_SignType;
  Base::Ref<Renderer> m_renderer;
  bool m_bIsBot;
};
#endif //! __PLAYER_HPP__