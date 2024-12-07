#ifndef __ENTITY_HPP__
#define __ENTITY_HPP__
#include "Vector.hpp"

class Texture;
struct ObjectSize;

class Entity {
public:
  virtual ~Entity() {}
  
  virtual Texture& GetTexture() = 0;
  virtual Vec2i& GetPosition() = 0;

  virtual void SetPosition(const Vec2i &pos) = 0;
  virtual const Vec2i& GetPosition() const = 0;

  virtual void SetSize(const ObjectSize &size) = 0;
  virtual const ObjectSize GetSize() const = 0;
  
private:
};
#endif //! __ENTITY_HPP__