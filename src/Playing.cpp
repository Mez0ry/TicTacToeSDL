#include "Playing.hpp"
#include "SceneManager.hpp"
#include "Menu.hpp"

Playing::Playing(const Base::Ref<Renderer> renderer, const Base::Ref<Window> window,SceneManager* scene_manager) : m_Board(renderer),m_SceneManager(scene_manager){
  m_Renderer = renderer;
  m_Window = window;
  
  m_Players.resize(2);

  m_Players[0].Setup(m_Renderer,"resources/cross.jpg",SignType::CROSS);

  m_Players[1].Setup(m_Renderer,"resources/zero.jpg",SignType::ZERO,true);

  for(auto& player : m_Players){
    auto& signs = player.GetSigns();
    for(auto& sign : signs){
      sign->SetSize(ObjectSize(50,50));
    }
  }

  m_BackgroundTexture.LoadTexture(m_Renderer,"resources/board.png");
  m_BackgroundTexture.SetRect({0,0},ObjectSize(378,666));
  
  OnResize();
}

Playing::~Playing(){

}


void Playing::OnResize(){
 m_Board.OnResize(m_Window);
 auto half_cell_size = m_Board.GetCellSize() / 2;

 auto[win_w,win_h] = m_Window->GetWindowSize();

 m_BackgroundTexture.SetSize(ObjectSize(win_w * 0.473,win_h * 0.952));
 m_BackgroundTexture.SetPosition({(win_w / 2) - (m_BackgroundTexture.GetSize().GetWidth() / 2),(win_h / 2) - (m_BackgroundTexture.GetSize().GetHeight() / 2)});
 
 auto& grid_texture = m_Board.GetGridTexture();
 auto grid_pos = grid_texture.GetPosition();
  
 grid_pos.x = (m_BackgroundTexture.GetPosition().x + (m_BackgroundTexture.GetSize().GetWidth() / 2)) - (grid_texture.GetSize().GetWidth() / 2);
 grid_pos.y = (m_BackgroundTexture.GetPosition().y + (m_BackgroundTexture.GetSize().GetHeight() / 2)) - (grid_texture.GetSize().GetHeight() / 2);
 grid_texture.SetPosition(grid_pos);

 for(auto& player : m_Players){
    auto& signs = player.GetSigns();
    for(auto& sign : signs){
      sign->SetSize(half_cell_size);

      Vec2 new_pos;
      new_pos.x = (grid_texture.TopLeft().x + half_cell_size.GetWidth() - (sign->GetSize().GetWidth() / 2))   + (sign->GetPosition().x * (m_Board.GetCellSize().GetWidth() + 15));
      new_pos.y = (grid_texture.TopLeft().y + half_cell_size.GetHeight() - (sign->GetSize().GetHeight() / 2)) + (sign->GetPosition().y * (m_Board.GetCellSize().GetHeight() + 15));

      sign->GetTexture().SetPosition(new_pos);
    }
 }

 m_WinnerText.LoadFont("resources/fonts/Wombyland/OpenType-TT/WombyLand.ttf",70);
 m_WinnerText->SetSize(ObjectSize(325,250));
 m_WinnerText->SetPosition({(win_w / 2) - (m_WinnerText->GetSize().GetWidth() / 2),(win_h / 2) - ((m_WinnerText->GetSize().GetHeight() / 2))});
}

void Playing::OnCreate(){
  m_Renderer->SetRenderDrawColor({50,40,90,220});
}

void Playing::OnDestroy(){
  m_StartTimerOnce = false;
  m_WinnerColor = {171,142,186,255};
  m_Board.OnDestroy();

 for(auto& player : m_Players){
   player.OnDestroy();
 }
}

void Playing::HandleInput(const Base::Ref<EventHandler> event_handler){
  auto& keyboard_input = event_handler->GetKeyboardInput();
  auto& mouse_input = event_handler->GetMouseInput();
  
  if(mouse_input.IsPressed(SDL_BUTTON_LEFT)){
    Vec2 cursor_position = mouse_input.GetMousePosition();

    Vec2 sign_place_pos;
    Vec2 diff = ((cursor_position - m_Board.GetGridTexture().TopLeft()));

    ObjectSize cell_size = m_Board.GetCellSize();
    sign_place_pos.x = diff.x / (cell_size.GetWidth());
    sign_place_pos.y = diff.y / (cell_size.GetHeight());
    
    if(Board::IsOnBoard(sign_place_pos) && !m_Board.CellIsOccupied(m_Players,sign_place_pos) && m_Board.IsCurrentTurn(m_Players[0]) && m_Board.GetBoardState() == Board::BoardState::EMPTY){
      m_Board.MakeMove(m_Players[0],sign_place_pos);
      m_Board.SwitchTurns();

      if(m_Players.size() > 1 && m_Players.back().IsBot()) {
        if(m_Board.IsCurrentTurn(m_Players.back())){
          Vec2 best_move = m_Board.FindBestMove(m_Players,m_Players[1],true);

          auto read_pos = m_Players[1].GetReadPos();
          auto& signs = m_Players[1].GetSigns();

          m_Board.MakeMove(m_Players[1],best_move);

          read_pos = m_Players[1].GetReadPos();
          
          m_Board.SwitchTurns();
        }
      }
      int index = m_Board.CalculateWinnerPlayerIndex(m_Players);
      if(index != -1){
        if(index == 0){
          m_Board.SetBoardState(Board::BoardState::WIN);
        }else{
          m_Board.SetBoardState(Board::BoardState::LOSE);
        }
      }else if(m_Board.IsTie(m_Players)){
        m_Board.SetBoardState(Board::BoardState::TIE);
      }
    }
  }
}
#pragma GCC diagnostic push // Also works for clang compiler
#pragma GCC diagnostic ignored "-Wswitch"

void Playing::Update(float dt){
  if(m_Board.GetBoardState() != Board::BoardState::EMPTY){
    if(!m_StartTimerOnce){
      m_WinnerTextShowTimer.Start();
      m_StartTimerOnce = true;
    }
    if(m_WinnerTextShowTimer.GetTicks() < (10 * 1000)){
      double t = std::clamp(0.0,1.0,m_WinnerTextShowTimer.GetTicks() * 0.0001);
      m_WinnerColor.a = 255 + t * (0 - 255); //lerp

      switch(m_Board.GetBoardState()){
        case Board::BoardState::WIN:{
          m_WinnerText.LoadText(m_Renderer,"You won!",m_WinnerColor);
          break;
        }
        case Board::BoardState::LOSE:{
          m_WinnerText.LoadText(m_Renderer,"You lost!",m_WinnerColor);
          break;
        }
        case Board::BoardState::TIE:{
          m_WinnerText.LoadText(m_Renderer,"Tie!",m_WinnerColor);
          break;
        }
      }
    }else{
      m_SceneManager->TransitionTo<Menu>();
    }
  }
}

#pragma GCC diagnostic pop

#pragma GCC diagnostic push // Also works for clang compiler
#pragma GCC diagnostic ignored "-Wswitch"

void Playing::Render(){
  m_Renderer->Render(m_BackgroundTexture);
  m_Board.Render();

  if(m_Board.GetBoardState() != Board::BoardState::EMPTY){
    auto board_state = m_Board.GetBoardState();
    switch(board_state){
      case Board::BoardState::WIN:{
        for(auto& move : m_Board.GetWinningSequence(m_Players[0])){
          m_Board.HighlightCell(m_Renderer,move,{41,255,104,255});
        }
        break;
      }
      case Board::BoardState::LOSE:{
        for(auto& move : m_Board.GetWinningSequence(m_Players[1])){
           m_Board.HighlightCell(m_Renderer,move,{255,0,41,204});
        }
        break;
      }
    }
  }

  for(auto& player : m_Players){
    for(size_t i = 0;i< player.GetReadPos();i++){
      auto& sign = player.GetSigns()[i];
      m_Renderer->Render(sign->GetTexture());
    }
  }
  
  if(m_Board.GetBoardState() != Board::BoardState::EMPTY){
    m_Renderer->Render(m_WinnerText);
  }
}
#pragma GCC diagnostic pop