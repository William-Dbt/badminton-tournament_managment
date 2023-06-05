#include "Player.hpp"

Player::Player(const std::string name) : _name(name) {}

Player::~Player() {}

void    Player::setName(const std::string& name) {
    this->_name = name;
}
