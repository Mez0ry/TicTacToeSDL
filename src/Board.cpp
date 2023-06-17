#include "Board.hpp"
#include "Renderer.hpp"

Vec2 Board::m_BoardSize = {3,3};
Vec2 Board::m_BoardTopLeft = {0,0};

Board::Board(const Base::Ref<Renderer> renderer) : m_CurrentTurn(SignType::CROSS),m_BoardState(BoardState::EMPTY){
  m_Renderer = renderer;
    
  m_GridTexture.LoadTexture(m_Renderer,"resources/seperate_grid.png");
  m_GridTexture.SetRect<SourceRect>({0,0},ObjectSize(318,318));
  m_GridTexture.SetRect({0,0},ObjectSize(318,318));
}

Board::~Board(){
 
}

void Board::OnResize(Base::Ref<Window> window){
  auto[win_w,win_h] = window->GetWindowSize();

  m_BoardTopLeft.x = (win_w - (m_CellSize.GetWidth() * 1.4f) * m_BoardSize.x) / 2;
  m_BoardTopLeft.y = (win_h - (m_CellSize.GetHeight() * 1.4f) * m_BoardSize.y) / 2;

  int square_width = (win_w * 0.375 / m_BoardSize.x);
	int square_height = (win_h * 0.375 / m_BoardSize.y);

	if (square_width > square_height) {
		square_width = square_height;
	}
	else {
		square_height = square_width;
	}

  m_CellSize = ObjectSize(square_width,square_height);

  m_GridTexture.SetSize(ObjectSize((m_CellSize.GetWidth() * m_BoardSize.x) + 18, (m_CellSize.GetHeight() * m_BoardSize.y) + 18));

}
void Board::OnDestroy(){
  m_MovesVec.clear();
  m_CurrentTurn = SignType::CROSS;
  m_BoardState = BoardState::EMPTY;
}

void Board::Render(){
  m_Renderer->Render(m_GridTexture);
}

void Board::MakeMove(Player& player,const Vec2& move_to){
  MakePseudoMove(player,move_to);
  RegisterMove(move_to,player);
}

void Board::MakePseudoMove(Player& player,const Vec2& move_to){
  auto sign = player.GetReadSign();
  if(sign == nullptr){
    return;
  }
  
  const Vec2 grid_top_left = m_GridTexture.TopLeft();

  Vec2 offset;
  offset.x = move_to.x * (m_CellSize.GetWidth() + 10);
  offset.y = move_to.y * (m_CellSize.GetHeight() + 10);

  Vec2 new_pos;
  new_pos.x = (grid_top_left.x + (m_CellSize.GetWidth() / 2) - (sign->GetSize().GetWidth() / 2))   + offset.x;
  new_pos.y = (grid_top_left.y + (m_CellSize.GetHeight() / 2) - (sign->GetSize().GetHeight() / 2)) + offset.y;

  sign->GetTexture().SetPosition(new_pos);
  sign->SetPosition(move_to);
}

void Board::RegisterMove(const Vec2& move,Player& player){
  m_MovesVec.push_back(MoveInfo(player,move));
}

void Board::UnRegisterMove(const Vec2& move){
  auto it = m_MovesVec.erase(std::remove_if(m_MovesVec.begin(),m_MovesVec.end(),[&](const MoveInfo& info){return (move == info.move);}),m_MovesVec.end());
}

void Board::UnmakeMove(){
  if(m_MovesVec.empty()) return;
  
  uint32_t read_pos = m_MovesVec.back().player.GetReadPos();
  if(read_pos == 0){ 
    return;
  }

  m_MovesVec.back().player.DecreaseReadPos();
  m_MovesVec.pop_back();
}

bool Board::CellIsOccupied(std::vector<Player>& players,const Vec2& cell) const{
  for(auto& player : players){
    auto& signs = player.GetSigns();
    uint32_t read_pos = player.GetReadPos();
    if(read_pos == 0){ 
      continue;
    }
      
    if(signs[read_pos - 1]->GetPosition() == cell){
      return true;
    }
  }
  return false;
}

TerminalState Board::GetTerminalState(std::vector<Player>& players) const{
  auto index = CalculateWinnerPlayerIndex(players);
  if(index != -1){
    if(index == 0){
      return TerminalState::WIN;
    }else{
      return TerminalState::LOSE;
    }
  }

  if(IsTie(players)){
    return TerminalState::TIE;
  }
  
  return TerminalState::EMPTY;
}

int Board::CalculateWinnerPlayerIndex(std::vector<Player>& players) const{
  int winner_index = 0;

  for(auto& player : players){
    for(int row_index = 0,col_index = 0;row_index < m_BoardSize.y && col_index < m_BoardSize.x; row_index++,col_index++){
      if(CheckColsForWin(player) || CheckRowsForWin(player) || CheckDiagonalsForWin(player)){
        return winner_index;
      }
    }
    winner_index++;
  }

  return -1;
}

bool Board::CheckColsForWin(Player& player) const{
  return (GetWinningSequenceCols(player).size() == m_BoardSize.x);
}

bool Board::CheckRowsForWin(Player& player) const{
  return (GetWinningSequenceRows(player).size() == m_BoardSize.y);
}

bool Board::CheckDiagonalsForWin(Player& player) const{
  return (GetWinningSequenceDiags(player).size() == std::min(m_BoardSize.x, m_BoardSize.y));
}

std::vector<Vec2> Board::GetWinningSequenceCols(Player& player) const{
  std::vector<Vec2> move_seq;
  auto moves = GetMoves(player);

  for(int i  = 0;i<m_BoardSize.y;i++){
    for(int j = 0;j<m_BoardSize.x;j++){
      Vec2 pos = {j,i};
      auto it = std::find_if(moves.begin(),moves.end(),[&](const Vec2& move){return (move == pos);});
      if(it != moves.end()){
        move_seq.push_back(pos);
        if(move_seq.size() == m_BoardSize.x){
          return move_seq;
        }
      }
    }
    move_seq.clear();
  }

  return move_seq;
}

std::vector<Vec2> Board::GetWinningSequenceRows(Player& player) const{
  std::vector<Vec2> move_seq;
  auto moves = GetMoves(player);

  for(int i = 0;i<m_BoardSize.x;i++){
    for(int j = 0;j<m_BoardSize.y;j++){
      Vec2 pos = {i,j};
      auto it = std::find_if(moves.begin(),moves.end(),[&](const Vec2& move){return (move == pos);});
      if(it != moves.end()){
        move_seq.push_back(pos);
        if(move_seq.size() == m_BoardSize.y){
          return move_seq;
        }
      }
    }
    move_seq.clear();
  }

  return move_seq;
}

std::vector<Vec2> Board::GetWinningSequenceDiags(Player& player) const{
  std::vector<Vec2> move_seq;
  auto moves = GetMoves(player);
  
  for(int i = 0;i<m_BoardSize.y && i < m_BoardSize.x;i++){
    Vec2 pos = {i,i};

    auto it = std::find_if(moves.begin(),moves.end(),[&](const Vec2& move){return (move == pos);});
    if(it != moves.end()){
      move_seq.push_back(pos);
      if(move_seq.size() == std::min(m_BoardSize.x, m_BoardSize.y)){
        return move_seq;
      }
    }
  }

  move_seq.clear();

  for(int i = m_BoardSize.x - 1,k = 0;i >= 0;i--){
    Vec2 pos = {i,k++};

    auto it = std::find_if(moves.begin(),moves.end(),[&](const Vec2& move){return (move == pos);});
    if(it != moves.end()){
      move_seq.push_back(pos);
      if(move_seq.size() == std::min(m_BoardSize.x, m_BoardSize.y)){
        return move_seq;
      }
    }
  }

  move_seq.clear();
  return move_seq;
}

std::vector<Vec2> Board::GetWinningSequence(Player& player) const{
  if(std::vector<Vec2> cols = GetWinningSequenceCols(player),rows = GetWinningSequenceRows(player),diags = GetWinningSequenceDiags(player); !cols.empty() || !rows.empty() || !diags.empty()){
    if(!cols.empty()){
      return cols;
    }else if(!rows.empty()){
      return rows;
    }else if(!diags.empty()){
      return diags;
    }
  }
  return {};
}

std::vector<Vec2> Board::GetMoves(Player& player) const{
  if(player.GetReadPos() == 0) return {};
  
  std::vector<Vec2> moves;

  auto& signs = player.GetSigns();

  for(int i = 0;i < player.GetReadPos();i++){
    moves.push_back(signs[i]->GetPosition());
  }
  
  return moves;
}
#pragma GCC diagnostic push // Also works for clang compiler
#pragma GCC diagnostic ignored "-Wswitch"

int Board::Minimax(std::vector<Player>& players,Player& player, Player& opponent ,int alpha,int beta,int depth, bool is_maximazing){
  auto terminal_state = GetTerminalState(players);

  switch(terminal_state){
    case TerminalState::LOSE:{

    }
    case TerminalState::WIN:{
      int score = static_cast<int>(terminal_state);
      return ((score < 0) ? (depth - score) : (score - depth));
      break;
    }
    case TerminalState::TIE:{
      return 0;
      break;
    }
  }

  if(is_maximazing){
    int best_score = std::numeric_limits<int>::min();

    for(auto& empty_cell : GetEmptyCells(players)){

      MakeMove(player,empty_cell);

      int eval = Minimax(players,player,opponent,alpha,beta,depth+1, !is_maximazing);
      best_score = std::max(best_score,eval);
      alpha = std::max(alpha,eval);

      UnmakeMove();

      if(beta <= alpha){
        break;
      }
    }

    return best_score;
  }else{
    int best_score = std::numeric_limits<int>::max();

    for(auto& empty_cell : GetEmptyCells(players)){
      MakeMove(opponent,empty_cell);

      int eval = Minimax(players,player,opponent,alpha,beta,depth + 1,!is_maximazing);
      best_score = std::min(best_score,eval);
      beta = std::min(beta,eval);

      UnmakeMove();
      
      if(beta <= alpha){
        break;
      }
    }

    return best_score;
  }
  
  return -1;
}
#pragma GCC diagnostic pop

Vec2 Board::FindBestMove(std::vector<Player>& players,Player& current_player,bool is_opponent){
  if(players.size() < 2) return {-1,-1};

  int best_score = (is_opponent) ? std::numeric_limits<int>::max() : std::numeric_limits<int>::min();
  Vec2 best_move;

  auto is_best_score = (is_opponent) ? [](int move_score, int best_score){return (move_score < best_score);} : [](int move_score, int best_score){return (move_score > best_score);};
  
  auto available_cells = GetEmptyCells(players);

  for(auto& empty_cell : available_cells){
    MakeMove(current_player,empty_cell);
    int move_score = Minimax(players,players[0],players[1],std::numeric_limits<int>::min(),std::numeric_limits<int>::max(),0,is_opponent);
    UnmakeMove();

    if(is_best_score(move_score,best_score)){
      best_move = empty_cell;
      best_score = move_score;
    }

  }

  return best_move;
}

std::vector<Vec2> Board::GetEmptyCells(std::vector<Player>& players) const{
  std::vector<Vec2> total_cells;

  for(auto i  = 0;i<m_BoardSize.y;i++){
    for(int j = 0;j < m_BoardSize.x;j++){
      total_cells.push_back({j,i});
    }
  }
  
  if(m_MovesVec.empty()) 
    return total_cells;
  
  for(auto& move_info : m_MovesVec){
    total_cells.erase(std::remove_if(total_cells.begin(),total_cells.end(),[&](const Vec2& pos){return (pos == move_info.move);}),total_cells.end());
  }

  return total_cells;
}

void Board::HighlightCell(const Base::Ref<Renderer> renderer,const Vec2& cell,Color color){
  Texture texture;
  texture.SetSize(ObjectSize(m_CellSize.GetWidth() * 0.8f,m_CellSize.GetHeight() * 0.8f));
  auto texture_size = texture.GetSize();
  texture = Texture::CreateTexture(renderer,SDL_PIXELFORMAT_ABGR8888,SDL_TEXTUREACCESS_STREAMING,texture_size.GetWidth(),texture_size.GetHeight());
  
  TextureGuard texture_guard(texture);
  auto  pitch = texture_guard.GetPitch();
  auto* pixels = texture_guard.GetPixels();

  for(int i = 0;i<texture_size.GetWidth();i++){
    for(int j = 0;j < texture_size.GetHeight();j++){
      texture.SetPixelAt(pixels,texture_size.GetWidth(),{j,i},color);
    }
  }

  const Vec2 grid_top_left = m_GridTexture.TopLeft();
  Vec2 offset;
  offset.x = cell.x * (m_CellSize.GetWidth() + 10);
  offset.y = cell.y * (m_CellSize.GetHeight() + 10);

  Vec2 new_pos;
  new_pos.x = (grid_top_left.x + (m_CellSize.GetWidth() / 2) - (texture_size.GetWidth() / 2))   + offset.x;
  new_pos.y = (grid_top_left.y + (m_CellSize.GetHeight() / 2) - (texture_size.GetHeight() / 2)) + offset.y;

  SDL_SetTextureBlendMode(texture,SDL_BLENDMODE_BLEND);
  SDL_SetTextureAlphaMod(texture,150);
  texture.SetPosition(new_pos);
  m_Renderer->Render(texture);
}