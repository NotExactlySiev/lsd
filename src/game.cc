#include "game.hh"

Game::Game(GameConfig *config) : Parent(config->code) {
    m_Config = config;
    // set_curr_dir
    //
    //
    // TODO loads of shit here
    PreInit();
}

void Game::PreInit() {
    // TODO set random seed
}

void Game::Init(GPU* gpu, Pad* pad) {
    if (!m_Initialized) {
        Parent::Init(gpu, pad);
    }
}

void Game::Func0() {
    if (m_Config->unk3 != 0) {
        //
    }
}
