#ifndef __EXIT_HPP__
#define __EXIT_HPP__
#include "GameScene.hpp"
#include "Text.hpp"
#include "FadeHelper.hpp"
#include <mutex>

class Exit : virtual public GameScene {
private:
  Core::Ref<Renderer> m_Renderer;
public:
  Exit(const Core::Ref<Renderer> renderer,const Core::Ref<Window> window,bool& is_running);
  ~Exit();

  void OnResize(const Core::Ref<Window> window) override;
  void OnCreate() override;
  
  void HandleInput(const Core::Ref<EventHandler> event_handler) override;
  void Update(float dt) override;
  void Render(const Core::Ref<Renderer> renderer) override;

private:
  bool& m_IsRunning;
  Text m_Text;
  FadeHelper<double,std::milli> m_TextFadeHelper;
  Color m_BlackColor = {0,0,0,255};
};

#endif //! __EXIT_HPP__