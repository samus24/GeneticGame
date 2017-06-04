#ifndef TILEMAP_HPP
#define TILEMAP_HPP

#include <SFML/Graphics.hpp>
#include "Dungeon.hpp"
#include "Utils.hpp"

class TileMap : public sf::Drawable, public sf::Transformable
{
public:

	bool load(const std::string& tileset, sf::Vector2u tileSize, Dungeon::Matrix tiles, unsigned int width, unsigned int height)
	{
		// load the tileset texture
		if (!m_tileset.loadFromFile(tileset))
			return false;

		_width = width;
		_height = height;
		// resize the vertex array to fit the level size
		m_vertices.setPrimitiveType(sf::Quads);
		m_vertices.resize(width * height * 4);

		sf::Vector2f origin((WINDOW_WIDTH - (width * TILESIZE.x)) / 2.f, (WINDOW_HEIGHT - (height * TILESIZE.y)) / 2.f);

		// populate the vertex array, with one quad per tile
		for (unsigned int i = 0; i < width; ++i)
			for (unsigned int j = 0; j < height; ++j)
			{
				// get the current tile number
				int tileNumber = tiles.cells[i][j];
				int key = -1;
				if (tileNumber == Dungeon::WALL){
					key++;
					for (size_t k = 1; k <= 4; ++k) {
						if (hasWall(k, tiles, i, j, width, height)) {
							key *= 10;
							key += k;
						}
					}

				}
				else if (tileNumber >= 0){
					tileNumber = 0;
				}
				else{
					tileNumber = std::abs(tileNumber);
				}

				// find its position in the tileset texture
				int tu = tileNumber % (m_tileset.getSize().x / tileSize.x);
				int tv = tileNumber / (m_tileset.getSize().x / tileSize.x);

				if (key != -1) {
					sf::IntRect wallCoords;
					wallCoords = WALLTILE.at(key);
					tu = wallCoords.left / TILESIZE.x;
					tv = wallCoords.top / TILESIZE.y;
				}

				// get a pointer to the current tile's quad
				sf::Vertex* quad = &m_vertices[(i + j * width) * 4];

				// define its 4 corners
				quad[0].position = origin + sf::Vector2f(i * tileSize.x, j * tileSize.y);
				quad[1].position = origin + sf::Vector2f((i + 1) * tileSize.x, j * tileSize.y);
				quad[2].position = origin + sf::Vector2f((i + 1) * tileSize.x, (j + 1) * tileSize.y);
				quad[3].position = origin + sf::Vector2f(i * tileSize.x, (j + 1) * tileSize.y);

				// define its 4 texture coordinates
				quad[0].texCoords = sf::Vector2f(tu * tileSize.x, tv * tileSize.y);
				quad[1].texCoords = sf::Vector2f((tu + 1) * tileSize.x, tv * tileSize.y);
				quad[2].texCoords = sf::Vector2f((tu + 1) * tileSize.x, (tv + 1) * tileSize.y);
				quad[3].texCoords = sf::Vector2f(tu * tileSize.x, (tv + 1) * tileSize.y);
			}

		return true;
	}

	sf::IntRect getBoundsFromCell(size_t x, size_t y) const{
		sf::Vector2f corner = getCoordsFromCell(x, y);
		return sf::IntRect(corner.x, corner.y, TILESIZE.x, TILESIZE.y);
	}
	sf::Vector2f getCoordsFromCell(sf::Vector2u cell) const{
		return getCoordsFromCell(cell.x, cell.y);
	}

	sf::Vector2f getCoordsFromCell(size_t x, size_t y) const{
		const sf::Vertex* quad = &m_vertices[(x + y * _width) * 4];
		return quad[0].position;
	}

	sf::Vector2u getCellFromCoords(sf::Vector2f coords) const{
		return getCellFromCoords(coords.x, coords.y);
	}

	sf::Vector2u getCellFromCoords(float x, float y) const{
		const sf::Vertex* quad;
		sf::IntRect r;
		r.width = TILESIZE.x;
		r.height = TILESIZE.y;
		for (unsigned int i = 0; i < _width; ++i)
			for (unsigned int j = 0; j < _height; ++j)
			{
				quad = &m_vertices[(i + j * _width) * 4];
				r.left = quad[0].position.x;
				r.top = quad[0].position.y;
				if (r.contains(sf::Vector2i(x, y))){
					return sf::Vector2u(i, j);
				}
			}

		return sf::Vector2u(_width, _height);
	}

private:

	bool hasWall(int orientation, Dungeon::Matrix tiles, int x, int y, unsigned int width, unsigned int height) const{
		switch (orientation)
		{
		case 1:
			return (y!=0) && (tiles[x][y - 1] == Dungeon::WALL);
		case 2:
			return (x != 0) && (tiles[x - 1][y] == Dungeon::WALL);
		case 3:
			return (y != height - 1) && (tiles[x][y + 1] == Dungeon::WALL);
		case 4:
			return (x != width - 1) && (tiles[x + 1][y] == Dungeon::WALL);
		default:
			return false;
		}
	}




	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		// apply the transform
		states.transform *= getTransform();

		// apply the tileset texture
		states.texture = &m_tileset;

		// draw the vertex array
		target.draw(m_vertices, states);
	}

	sf::VertexArray m_vertices;
	size_t _width;
	size_t _height;
	sf::Texture m_tileset;
};

#endif