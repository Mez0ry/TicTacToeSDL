#ifndef __MENU_HPP__
#define __MENU_HPP__
#include "GameScene.hpp"
#include "Renderer.hpp"
#include "Window.hpp"

#include <array>
#include "EventHandler.hpp"
#include "GUI/Button.hpp"
#include "Texture.hpp"

class SceneManager;

class Menu : virtual public GameScene {
private:
  Core::Ref<Renderer> m_Renderer;
  SceneManager& m_SceneManager;
public:
  Menu(const Core::Ref<Renderer> renderer, const Core::Ref<Window> window, SceneManager& scene_manager);
  ~Menu();
  
  void OnResize(const Core::Ref<Window> window) override;
  void OnCreate() override;
  void HandleInput(const Core::Ref<EventHandler> event_handler) override;
  void Update(float dt) override;
  void Render(const Core::Ref<Renderer> renderer) override;

private:
  Texture m_RedSwitchPanel,m_GreenSwitchPanel;
  Texture m_BackgroundTexture;
  
  Texture m_TitlePanelTexture;
  Text m_TitleText;
  std::vector<Core::Ref<Button>> m_Buttons;
};

#endif //! __MENU_HPP__