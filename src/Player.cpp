#include "Player.hpp"

Player::Player(const sf::RenderWindow &window, const sf::Vector2f &pos)
{
    _windowSize = window.getSize();
    _circle.setRadius(6);
    _circle.setOrigin(sf::Vector2f{_circle.getRadius(), _circle.getRadius()});
    _circle.setFillColor(sf::Color::Yellow);
    _circle.setPosition(pos);
    _rectangle.setFillColor(sf::Color::White);
    _line = sf::VertexArray(sf::LinesStrip, 2);
    _line[0].color = sf::Color::Green;
    _line[1].color = sf::Color::Green;
    _fov = 60.0f * PI / 180;
    _angle = 270.0f * PI / 180;
    _speed = 5.0f;
    _forward = false;
    _rotateLeft = false;
    _rotateRight = false;
    _backward = false;
    _strafeLeft = false;
    _strafeRight = false;
}

Player::~Player()
{
}

float Player::magnitude(sf::Vector2f vec2)
{
    return std::sqrt(vec2.x * vec2.x + vec2.y * vec2.y);
}

sf::Vector2f Player::normalize(sf::Vector2f vec2)
{
    float length = magnitude(vec2);
    return sf::Vector2f(vec2.x / length, vec2.y / length);
}

sf::Vector2f Player::rotatePointAroundCenter(sf::Vector2f center, sf::Vector2f point, float angleRad)
{
    float s = sin(angleRad);
    float c = cos(angleRad);

    point.x -= center.x;
    point.y -= center.y;

    float xnew = point.x * c - point.y * s;
    float ynew = point.x * s + point.y * c;

    point.x = xnew + center.x;
    point.y = ynew + center.y;

    return point;
}

void Player::event(const sf::Event &event)
{
    if (event.type == sf::Event::KeyPressed) {
        if (event.key.code == sf::Keyboard::Q)
            _rotateLeft = true;
        if (event.key.code == sf::Keyboard::E)
            _rotateRight = true;
        if (event.key.code == sf::Keyboard::W)
            _forward = true;
        if (event.key.code == sf::Keyboard::S)
            _backward = true;
        if (event.key.code == sf::Keyboard::A)
            _strafeLeft = true;
        if (event.key.code == sf::Keyboard::D)
            _strafeRight = true;
        if (event.key.code == sf::Keyboard::Add && _fov <= 360 * PI / 180)
            _fov += 2 * PI / 180;
        if (event.key.code == sf::Keyboard::Subtract && _fov > 0 * PI / 180)
            _fov -= 2 * PI / 180;
    }
    if (event.type == sf::Event::KeyReleased) {
        if (event.key.code == sf::Keyboard::Q)
            _rotateLeft = false;
        if (event.key.code == sf::Keyboard::E)
            _rotateRight = false;
        if (event.key.code == sf::Keyboard::W)
            _forward = false;
        if (event.key.code == sf::Keyboard::S)
            _backward = false;
        if (event.key.code == sf::Keyboard::A)
            _strafeLeft = false;
        if (event.key.code == sf::Keyboard::D)
            _strafeRight = false;
    }
}

void Player::update(const Map::cell_t *cellmap)
{
    raycast(cellmap);
    if (_rotateLeft)
        _angle -= 4 * PI / 180;
    if (_rotateRight)
        _angle += 4 * PI / 180;
    if (_forward)
        _circle.move(getPlayerDirection(_angle, _speed));
    if (_backward)
        _circle.move(getPlayerDirection(_angle, -_speed));
    if (_strafeRight) {
        sf::Vector2f dir = getPlayerDirection(_angle, _speed);
        dir = rotatePointAroundCenter(sf::Vector2f{0.0f, 0.0f}, dir, PI / 2);
        _circle.move(dir);
    }
    if (_strafeLeft) {
        sf::Vector2f dir = getPlayerDirection(_angle, _speed);
        dir = rotatePointAroundCenter(sf::Vector2f{0.0f, 0.0f}, dir, -PI / 2);
        _circle.move(dir);
    }
}

sf::Vector2f Player::getPlayerDirection(const float angleRad, const float len)
{
    return sf::Vector2f(len * cos(angleRad), len * sin(angleRad));
}

void Player::raycast(const Map::cell_t *cellmap)
{
    _rays.clear();
    sf::Vector2f rayStart = sf::Vector2f{_circle.getPosition().x / CASE_SIZE, _circle.getPosition().y / CASE_SIZE};
    sf::Vector2i mapCheck = sf::Vector2i{static_cast<int>(rayStart.x), static_cast<int>(rayStart.y)};
    if (mapCheck.x >= 0 && mapCheck.x < _windowSize.x / CASE_SIZE && mapCheck.y >= 0 && mapCheck.y < _windowSize.y / CASE_SIZE) {
        if (cellmap[static_cast<int>(mapCheck.x + mapCheck.y * _windowSize.x / CASE_SIZE)].exist)
            return;
    } else return;

    sf::Vector2f playerDir = sf::Vector2f{_circle.getPosition().x + getPlayerDirection(_angle, 1).x, _circle.getPosition().y + getPlayerDirection(_angle, 1).y};
    for (float i = -(_fov / 2 * 180 / PI); i < (_fov / 2 * 180 / PI); i += 0.2f) {
        mapCheck = sf::Vector2i{static_cast<int>(rayStart.x), static_cast<int>(rayStart.y)};
        sf::Vector2f rayLength = {0.0f, 0.0f};
        sf::Vector2i step = {0, 0};
        sf::Vector2f rotatedDir = rotatePointAroundCenter(_circle.getPosition(), playerDir, i * PI / 180);
        sf::Vector2f rayDir = normalize(sf::Vector2f{rotatedDir.x / CASE_SIZE - rayStart.x, rotatedDir.y / CASE_SIZE - rayStart.y});
        sf::Vector2f rayUnitStepSize = sf::Vector2f{std::sqrt(1 + (rayDir.y / rayDir.x) * (rayDir.y / rayDir.x)), std::sqrt(1 + (rayDir.x / rayDir.y) * (rayDir.x / rayDir.y))};

        if (rayDir.x < 0) {
            step.x = -1;
            rayLength.x = (rayStart.x - static_cast<float>(mapCheck.x)) * rayUnitStepSize.x;
        } else {
            step.x = 1;
            rayLength.x = (static_cast<float>(mapCheck.x + 1) - rayStart.x) * rayUnitStepSize.x;
        }
        if (rayDir.y < 0) {
            step.y = -1;
            rayLength.y = (rayStart.y - static_cast<float>(mapCheck.y)) * rayUnitStepSize.y;
        } else {
            step.y = 1;
            rayLength.y = (static_cast<float>(mapCheck.y + 1) - rayStart.y) * rayUnitStepSize.y;
        }

        bool tileFound = false;
        float maxDistance = 100.0f;
        float distance = 0.0f;
        bool side = false;
        while (!tileFound && distance < maxDistance) {
            if (rayLength.x < rayLength.y) {
                mapCheck.x += step.x;
                distance = rayLength.x;
                rayLength.x += rayUnitStepSize.x;
                side = true;
            } else {
                mapCheck.y += step.y;
                distance = rayLength.y;
                rayLength.y += rayUnitStepSize.y;
                side = false;
            }
            if (mapCheck.x >= 0 && mapCheck.x < _windowSize.x / CASE_SIZE && mapCheck.y >= 0 && mapCheck.y < _windowSize.y / CASE_SIZE) {
                if (cellmap[static_cast<int>(mapCheck.x + mapCheck.y * _windowSize.x / CASE_SIZE)].exist)
                    tileFound = true;
            }
        }
        if (tileFound) {
            _rays.push_back(std::make_pair(sf::Vector2f{(rayStart.x + rayDir.x * distance) * CASE_SIZE, (rayStart.y + rayDir.y * distance) * CASE_SIZE}, side));
        }
    }
}

void Player::drawMinimap(sf::RenderWindow &window)
{
    _line[0].position = _circle.getPosition();
    for (auto it : _rays) {
        _line[1].position = it.first;
        window.draw(_line);
    }
    window.draw(_circle);
}

void Player::draw3D(sf::RenderWindow &window)
{
    float offsetX = 0.0f;
    float length = 0.0f;
    sf::Uint8 color = 0;
    for (auto it : _rays) {
        length = magnitude(sf::Vector2f{it.first.x - _circle.getPosition().x, it.first.y - _circle.getPosition().y});
        _rectangle.setSize(sf::Vector2f{window.getSize().x / static_cast<float>(_rays.size() - 1), 100 * _windowSize.y / length});
        _rectangle.setOrigin(sf::Vector2f{_rectangle.getSize().x, _rectangle.getSize().y / 2});
        _rectangle.setPosition(sf::Vector2f{offsetX, static_cast<float>(window.getSize().y / 2)});
        length = (length / 200);
        if (length < 1.0f)
            length = 1.0f;
        color = 200.0f / length;
        if (it.second) color /= 1.3;
        _rectangle.setFillColor(sf::Color{color, color, color, 255});
        offsetX += _rectangle.getSize().x;
        window.draw(_rectangle);
    }
}