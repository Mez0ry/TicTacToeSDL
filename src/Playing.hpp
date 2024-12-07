#ifndef __PLAYING_HPP__
#define __PLAYING_HPP__
#include "Board.hpp"
#include "Player.hpp"

#include "GameScene.hpp"
#include "EventHandler.hpp"
#include "Text.hpp"
#include <cmath>
#include "FadeHelper.hpp"

class SceneManager;

class Playing : virtual public GameScene {
private:
  Core::Ref<Renderer> m_Renderer;
  SceneManager& m_SceneManager;
public:
  Playing(const Core::Ref<Renderer> renderer, const Core::Ref<Window> window,SceneManager& scene_manager);
  ~Playing();

  void OnResize(const Core::Ref<Window> window) override;
  void OnCreate() override;
  void OnDestroy() override;
  
  void HandleInput(const Core::Ref<EventHandler> event_handler) override;
  void Update(float dt) override;
  void Render(const Core::Ref<Renderer> renderer) override;

private:
  Board m_Board;
  Texture m_BackgroundTexture;
  std::vector<Player> m_Players;
  
  FadeHelper<double,std::milli> m_WinnerFadeHelper;
  Text m_WinnerText;
  SDL_Color m_WinnerColor = {171,142,186,255};
};

#endif //! __PLAYING_HPP__