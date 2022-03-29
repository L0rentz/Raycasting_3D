#ifndef MAP_HPP_
#define MAP_HPP_

#include <fstream>
#include <vector>
#include <map>
#include <math.h>
#include <cstring>

#include "Exception.hpp"
#include "SFML/Graphics.hpp"

#define CASE_SIZE 32.0f

class Map {
    public:
        typedef struct s_line {
            sf::Vector2f A;
            sf::Vector2f B;
        } line_t;

        typedef struct s_cell {
            bool exist;
            sf::RectangleShape rect;
            bool isEdge[4];
            int edgeId[4];
        } cell_t;

        Map(const sf::RenderWindow &window);
        Map(const std::string &path, const sf::RenderWindow &window);
        ~Map();

        void draw(sf::RenderWindow &window);
        void event(const sf::Event &event, const sf::RenderWindow &window);
        std::map<int, line_t> &getMapvertex();
        cell_t *getCellmap();
        sf::Vector2f &getPlayerPos();

    protected:
    private:
        enum {
            TOP,
            BOTTOM,
            LEFT,
            RIGHT
        };

        sf::Vector2u _windowSize;
        float _caseSize;
        char *_filemap;
        cell_t *_cellmap;
        int _cellmapSize;
        int _cellmapWidth;
        std::map<int, line_t> _vertexmap;
        sf::CircleShape _vertice;
        sf::VertexArray _line;
        bool _leftPressed;
        bool _pressedExist;
        sf::Vector2f _playerPos;
        sf::RectangleShape _background;

        void generateCellmap();
        void constructorCommons(const sf::RenderWindow &window);
        void generateVertexmap();
        int getCellId(const sf::Vector2f &position) const;
        void getFileContent(const std::string &path);
        void drawVertexmap(sf::RenderWindow &window);
};

#endif /* !MAP_HPP_ */
