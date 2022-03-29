#include "Core.hpp"

Core::Core()
{
    _window.create(sf::VideoMode(1920, 1080), "Raycasting Engine");
    _window.setFramerateLimit(60);
    _maps.push_back(Map(MAP_PATH + std::string("test.txt"), _window));
    _selected = 0;
    _player = new Player(_window, _maps[_selected].getPlayerPos());
    _basicView = _window.getView();
    _minimap = sf::View(sf::FloatRect(0, 0, _window.getSize().x, _window.getSize().y));
    _minimap.setViewport(sf::FloatRect(0.0f, 0.0f, 0.2f, 0.2f));
    // _maps.push_back(Map(_window));

}

Core::~Core()
{
    delete _player;
}

void Core::event()
{
    while (_window.pollEvent(_event)) {
        if (_event.type == sf::Event::Closed)
            _window.close();
        _player->event(_event);
        _maps[_selected].event(_event, _window);
    }
}

void Core::update()
{
    _player->update(_maps[_selected].getCellmap());
}

void Core::display()
{
    _window.clear();
    _window.setView(_basicView);
    _player->draw3D(_window);
    _window.setView(_minimap);
    _maps[_selected].draw(_window);
    _player->drawMinimap(_window);
    _window.display();
}

void Core::run()
{
    while (_window.isOpen()) {
        event();
        update();
        display();
    }
}