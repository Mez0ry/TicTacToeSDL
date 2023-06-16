#ifndef __PLAYING_HPP__
#define __PLAYING_HPP__
#include "Board.hpp"
#include "Player.hpp"

#include "GameScene.hpp"
#include "EventHandler.hpp"
#include "Text.hpp"
#include "Timestamp.hpp"
#include <cmath>

class SceneManager;

class Playing : virtual public GameScene {
public:
  Playing(const Base::Ref<Renderer> renderer, const Base::Ref<Window> window,SceneManager* scene_manager);
  ~Playing();

  void OnResize() override;
  void OnCreate() override;
  void OnDestroy() override;
  
  void HandleInput(const Base::Ref<EventHandler> event_handler) override;
  void Update(float dt) override;
  void Render() override;

private:
  Board m_Board;
  Texture m_BackgroundTexture;
  std::vector<Player> m_Players;
  
  Timestamp m_WinnerTextShowTimer;
  bool m_StartTimerOnce = false;
  Text m_WinnerText;
  SDL_Color m_WinnerColor = {171,142,186,255};
private:
  SceneManager* m_SceneManager;
};

#endif //! __PLAYING_HPP__