#ifndef __SETTINGS_HPP__
#define __SETTINGS_HPP__
#include "GameScene.hpp"

#include "Text.hpp"

#include "KeyFrame.hpp"
class Settings : virtual public GameScene {
private:
  Core::Ref<Renderer> m_Renderer;
public:
  Settings(const Core::Ref<Renderer> renderer, const Core::Ref<Window> window,SceneManager& scene_manager);
  ~Settings();

  void OnResize(const Core::Ref<Window> window) override;
  void OnCreate() override;
  
  void HandleInput(const Core::Ref<EventHandler> event_handler) override;
  void Update(float dt) override;
  void Render(const Core::Ref<Renderer> renderer) override;
private:
    Texture m_BackgroundTexture;
};

#endif // __SETTINGS_HPP__