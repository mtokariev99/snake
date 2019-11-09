//  Snake.cpp
//  snake
//  Copyright © 2019 Nikita Tokariev. All rights reserved.
#include "Snake.h"
#include "Config.hpp"
#include <ncurses.h>

Snake::Snake()
    : m_direction( Direction::RIGHT ), m_isDead( false ) 
{
    m_position.x = Config::deskWidth/2;
    m_position.y = Config::deskHeight/2;
}

void Snake::draw() const {
    switch ( m_direction ) {
        case Direction::LEFT :
            mvprintw( m_position.y, m_position.x, "<" );
            break;

        case Direction::RIGHT :
            mvprintw( m_position.y, m_position.x, ">" );
            break;

        case Direction::UP :
            mvprintw( m_position.y, m_position.x, "^" );
            break;

        case Direction::DOWN :
            mvprintw( m_position.y, m_position.x, "v" );
            break;

        default:
            break;
    }

    for ( const auto& seg : m_snakeBody ) {
        mvprintw( seg.position.y, seg.position.x, "o" );
    }
}

void Snake::update() {
    move();
    checkCollision();
}

void Snake::extend() {
    m_snakeBody.emplace_back(m_snakeBody.empty() ? m_position.x : m_snakeBody.back().position.x, 
                             m_snakeBody.empty() ? m_position.y : m_snakeBody.back().position.y);
}

void Snake::onInput( const int& t_playerInput ) {
    changeDirection( t_playerInput );
}

void Snake::changeDirection( const int& t_playerInput ) {
    switch ( t_playerInput ) {
        case KEY_LEFT :
            m_direction = m_direction != Direction::RIGHT ? Direction::LEFT : m_direction;
            break;

        case KEY_RIGHT :
            m_direction = m_direction != Direction::LEFT ? Direction::RIGHT : m_direction;
            break;

        case KEY_UP :
            m_direction = m_direction != Direction::DOWN ? Direction::UP : m_direction;
            break;

        case KEY_DOWN :
            m_direction = m_direction != Direction::UP ? Direction::DOWN : m_direction;
            break;

        default : 
            break;
    }
}

void Snake::move() {
    for ( int i = m_snakeBody.size()-1; i > 0; --i ) {
        m_snakeBody[ i ].position.x = m_snakeBody[ i-1 ].position.x;
        m_snakeBody[ i ].position.y = m_snakeBody[ i-1 ].position.y;
    }

    if ( !m_snakeBody.empty() ) {
        m_snakeBody.front().position.x = m_position.x;
        m_snakeBody.front().position.y = m_position.y;
    }

    switch ( m_direction ) {
        case Direction::LEFT : 
            --m_position.x;
            break;

        case Direction::RIGHT : 
            ++m_position.x;
            break;

        case Direction::UP : 
            --m_position.y;
            break;

        case Direction::DOWN : 
            ++m_position.y;
            break;

        default : 
            return;
    }
}

void Snake::checkCollision() {
    if (m_position.x == Config::deskWidth-1 ||
        m_position.y == Config::deskHeight-1 ||
        m_position.x == 0 || m_position.y == 0) 
    {
        m_isDead = true;
    }

    for ( int i = 0; i < m_snakeBody.size(); ++i ) {
        if (m_snakeBody[i].position.x == m_position.x && 
            m_snakeBody[i].position.y == m_position.y) 
        {
            cut( m_snakeBody.begin()+i );
        }
    }
}

void Snake::cut( const std::vector<SnakeSegment>::iterator& t_cutFrom ) {
    m_snakeBody.erase( t_cutFrom, m_snakeBody.end() );
}