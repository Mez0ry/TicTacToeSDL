#ifndef __BOARD_HPP__
#define __BOARD_HPP__
#include "Texture.hpp"
#include <array>
#include <stack>
#include <numeric>
#include <unordered_map>
#include "Player.hpp"
#include <bitset>
#include "TextureGuard.hpp"
#include "SDL2/SDL_blendmode.h"

class Renderer;
class Player;

enum class TerminalState : int{
  WIN = 10, TIE = 0, LOSE = -10, EMPTY = 0x1337
};

struct MoveInfo{
  MoveInfo(Player& pl,const Vec2i& pos) : player(pl),move(pos){}

  MoveInfo(MoveInfo&& other) : player(other.player), move(std::move(other.move)){}
  MoveInfo(const MoveInfo& other) : player(other.player), move(other.move){}
  
  MoveInfo& operator=(const MoveInfo& other){
    player = other.player;
    move = other.move;
    return (*this);
  }

  MoveInfo& operator=(MoveInfo&& other){
    player = other.player;
    move = std::move(other.move);
    return (*this);
  }

  Player& player;
  Vec2i move;
};

class Board {
private:
  Core::Ref<Renderer> m_Renderer;
public:
  enum class BoardState : uint8_t{
    WIN,LOSE,TIE,EMPTY
  };
public:
  Board(const Core::Ref<Renderer> renderer);
  ~Board();

  void OnResize(Core::Ref<Window> window);
  void OnDestroy();
  void Render();
  
  static Vec2i TopLeft(){
    return Board::m_BoardTopLeft;
  }

  static bool IsOnBoard(const Vec2i& pos){
    return (pos.y < m_BoardSize.y && pos.x < m_BoardSize.x && pos.y > -1 && pos.x > -1);
  }

  ObjectSize GetCellSize() const {
    return m_CellSize;
  }

  Texture& GetGridTexture() {return m_GridTexture;}

  void RegisterMove(const Vec2i& move,Player& player);
  void UnRegisterMove(const Vec2i& move);

  void MakeMove(Player& player,const Vec2i& move_to);
  void MakePseudoMove(Player& player,const Vec2i& move_to);

  /**
   * @brief unmakes move that was made last
  */
  void UnmakeMove();

  bool CellIsOccupied(std::vector<Player>& players,const Vec2i& cell) const;

  TerminalState GetTerminalState(std::vector<Player>& players) const;

  int CalculateWinnerPlayerIndex(std::vector<Player>& players) const;

  bool CheckColsForWin(Player& player) const;
  bool CheckRowsForWin(Player& player) const;
  bool CheckDiagonalsForWin(Player& player) const;

  std::vector<Vec2i> GetWinningSequenceCols(Player& player) const;
  std::vector<Vec2i> GetWinningSequenceRows(Player& player) const;
  std::vector<Vec2i> GetWinningSequenceDiags(Player& player) const;
  std::vector<Vec2i> GetWinningSequence(Player& player) const;

  std::vector<Vec2i> GetMoves(Player& player) const;

  int Minimax(std::vector<Player>& players,Player& player, Player& opponent ,int alpha,int beta,int depth, bool is_maximazing);
  Vec2i FindBestMove(std::vector<Player>& players,Player& current_player,bool is_opponent);

  std::vector<Vec2i> GetEmptyCells() const;

  bool IsCurrentTurn(Player& player){
    return (m_CurrentTurn == player.GetSignType());
  }
  
  void SwitchTurns(){
    m_CurrentTurn = (SignType)((!static_cast<bool>(m_CurrentTurn)));
  }
  
  void SetBoardState(Board::BoardState state){
    m_BoardState = state;
  }

  BoardState GetBoardState() const {return m_BoardState;}

  bool IsTie() const {return (GetEmptyCells().empty());}

  void HighlightCell(const Core::Ref<Renderer> renderer,const Vec2i& cell,Color color);

private:
  static Vec2i m_BoardTopLeft;
  Texture m_GridTexture;
  static Vec2i m_BoardSize;
  ObjectSize m_CellSize = {100,100};
  std::vector<MoveInfo> m_MovesVec;
  SignType m_CurrentTurn;
  BoardState m_BoardState;
};

#endif //! __BOARD_HPP__