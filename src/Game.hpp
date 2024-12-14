#ifndef __CHESS_GAME_HPP__
#define __CHESS_GAME_HPP__
#include <cstdint>
#include "SceneManager.hpp"

class Game
{
public:
    Game();
    ~Game();

    bool Run();
private:
    void HandleInput();
    void Update(float dt);
    void Render();

private:
    bool m_bIsRunning;
    SceneManager m_SceneManager;
};
#endif //!__CHESS_GAME_HPP__