#ifndef __SIGN_HPP__
#define __SIGN_HPP__
#include "Entity.hpp"
#include "Texture.hpp"
#include <array>

enum class SignType{
  CROSS,ZERO,UNKNOWN
};

class Sign : public Entity {
public:
  Sign() = default;
  Sign(const Core::Ref<Renderer> renderer, const std::string& path,SignType type);
  ~Sign();

  Texture& GetTexture() override;
  Vec2i& GetPosition() override;

  void SetPosition(const Vec2i &pos) override;
  const Vec2i& GetPosition() const override;

  void SetSize(const ObjectSize &size) override;
  const ObjectSize GetSize() const override;

  SignType GetSignType() const {return m_SignType;}
  void SetSignType(SignType type) {m_SignType = type;}
private:
  Texture m_Texture;
  Vec2i m_Pos;
  SignType m_SignType;
};

#endif //!__SIGN_HPP__