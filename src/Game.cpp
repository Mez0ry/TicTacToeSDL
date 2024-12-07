#include "Game.hpp"

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
}

Game::~Game() {}

bool Game::Run() {

  //The frames per second timer
  Timer fps_timer;

  //The frames per second cap timer
  Timer cap_timer;
  
  Timer step_timer;
  
  int counted_frames = 0;
  fps_timer.Start();
  
  while (m_bIsRunning)
  {
    cap_timer.Start();
    Engine::GetModule<Renderer>()->RenderClear();

    HandleInput();
    float dt = step_timer.GetTicks() / 1000.0f;
    Update(dt * ((m_fps / 2) - 0.1f));
    step_timer.Start();
    Render();
    
    Engine::GetModule<Renderer>()->RenderPresent();

    ++counted_frames;

    int frame_ticks = cap_timer.GetTicks();

    if(m_frameDelay > frame_ticks)
    {
      SDL_Delay(m_frameDelay - frame_ticks);
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
