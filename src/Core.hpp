#ifndef CORE_HPP_
#define CORE_HPP_

#ifdef _WIN32
    #define MAP_PATH "../../maps/"
#else
    #define MAP_PATH "maps/"
#endif

#include <vector>

#include "SFML/Graphics.hpp"
#include "Exception.hpp"
#include "Player.hpp"
#include "Map.hpp"

class Core {
    public:
        Core();
        ~Core();

        void run();

    protected:
    private:
        sf::RenderWindow _window;
        sf::Event _event;
        Player *_player;
        std::vector<Map> _maps;
        int _selected;
        sf::View _minimap;
        sf::View _basicView;

        void event();
        void update();
        void display();
};

#endif /* !CORE_HPP_ */