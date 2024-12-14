#include "Game.hpp"
#include "Timer.hpp"
#include "Engine.hpp"
#include "EventHandler.hpp"

#include "Menu.hpp"
#include "Playing.hpp"
#include "Settings.hpp"

#include "Exit.hpp"

Game::Game() : m_bIsRunning(true) {
  if(!Engine::Initialize(SDL_INIT_EVERYTHING,IMG_INIT_PNG | IMG_INIT_JPG,true)){
    STELLAR_CRITICAL_THROW(std::runtime_error,"","Failed to initialize engine");
  }

  Engine::RegisterModule<Window>("Tic-Tac-Toe",SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,800,700, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
  Engine::RegisterModule<Renderer>(Engine::GetModule<Window>(),-1,SDL_RENDERER_ACCELERATED);
  Engine::RegisterModule<EventHandler>();

  m_SceneManager.AddScene<Menu>(Engine::GetModule<Renderer>(),Engine::GetModule<Window>(),m_SceneManager);
  m_SceneManager.AddChildScene<Menu,Playing>(Engine::GetModule<Renderer>(),Engine::GetModule<Window>(),m_SceneManager);
  m_SceneManager.AddChildScene<Menu,Exit>(Engine::GetModule<Renderer>(),Engine::GetModule<Window>(),m_bIsRunning);
  m_SceneManager.AddChildScene<Menu,Settings>(Engine::GetModule<Renderer>(),Engine::GetModule<Window>(),m_SceneManager);
}

Game::~Game() {}

bool Game::Run() {
  Timer cap_timer;
  
  Timer step_timer;
  
  while (m_bIsRunning)
  {
    cap_timer.Start();
    Engine::GetModule<Renderer>()->RenderClear();

    HandleInput();

    double dt = step_timer.GetTicks() / 1000.f;
    
    Update(dt);

    step_timer.Start();

    Render();
    
    Engine::GetModule<Renderer>()->RenderPresent();

    auto elapsed = cap_timer.GetTicks();
    if(elapsed <= 10)
    {
      SDL_Delay(10 - elapsed);
    }

  }

  return true;
}

void Game::HandleInput() {
 auto event_handler = Engine::GetModule<EventHandler>();
 event_handler->PollEvents();

 if(event_handler->IsQuit() || KeyboardInput::IsPressed(SDLK_ESCAPE)){
  m_bIsRunning = false;
 }
 
 if(WindowInput::OnEvent(SDL_WINDOWEVENT_RESIZED)){
  m_SceneManager.OnResize(Engine::GetModule<Window>());
 }

 m_SceneManager.HandleInput(event_handler);
}

void Game::Update(float dt) {   
  m_SceneManager.Update(dt);
}

void Game::Render() {
 m_SceneManager.Render(Engine::GetModule<Renderer>());
}
