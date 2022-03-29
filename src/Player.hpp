#ifndef PLAYER_HPP_
#define PLAYER_HPP_

#define PI 3.141592653589793

#include "SFML/Graphics.hpp"
#include "Map.hpp"

class Player {
    public:
        Player(const sf::RenderWindow &window, const sf::Vector2f &pos);
        ~Player();

        void event(const sf::Event &event);
        void update(const Map::cell_t *cellmap);
        void draw3D(sf::RenderWindow &window);
        void drawMinimap(sf::RenderWindow &window);

    protected:
    private:
        sf::Vector2u _windowSize;
        float _fov;
        float _angle;
        sf::CircleShape _circle;
        sf::VertexArray _line;
        sf::RectangleShape _rectangle;
        std::vector<std::pair<sf::Vector2f, bool>> _rays;
        float _speed;
        bool _rotateLeft;
        bool _rotateRight;
        bool _forward;
        bool _backward;
        bool _strafeLeft;
        bool _strafeRight;

        void raycast(const Map::cell_t *cellmap);
        float magnitude(sf::Vector2f vec2);
        void drawDebug(sf::RenderWindow &window);
        sf::Vector2f normalize(sf::Vector2f vec2);
        sf::Vector2f getPlayerDirection(const float angleRad, const float len);
        sf::Vector2f rotatePointAroundCenter(sf::Vector2f center, sf::Vector2f point, float angleRad);
};

#endif /* !PLAYER_HPP_ */
