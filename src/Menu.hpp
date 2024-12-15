#ifndef __MENU_HPP__
#define __MENU_HPP__
#include "GameScene.hpp"
#include "Renderer.hpp"
#include "Window.hpp"

#include <array>
#include "EventHandler.hpp"
#include "GUI/Button.hpp"
#include "Texture.hpp"
#include "Lerp.hpp"

#include "KeyFrame.hpp"

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
  void OnDestroy() override;
  void HandleInput(const Core::Ref<EventHandler> event_handler) override;
  void Update(float dt) override;
  void Render(const Core::Ref<Renderer> renderer) override;

private:
  Texture m_BackgroundTexture;
  Stellar::KeyFrame m_TitleKFIn,m_TitleKFOut;
  Texture m_TitlePanelTexture;
  Text m_TitleText;
  Vec2i m_TmpTitleTextPos, m_TmpPanelTexturePos;
  
  std::vector<Core::Ref<Button>> m_Buttons;
  const Color m_ButtonColor = {68,220,236,250};
  Stellar::KeyFrame m_ButtonKeyFrame;
  Vec2i m_TargetPlayButtonPos,m_TargetSettingsButtonPos, m_TargetExitButtonPos;
  int m_DefaultPlayButtonPosX,m_DefaultSettingsButtonPosX,m_DefaultExitButtonPosX;
};

#endif //! __MENU_HPP__