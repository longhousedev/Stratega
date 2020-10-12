#pragma once
#include <SFML/Graphics.hpp>

//TILES
static const int TILE_WIDTH = 256;
static const int TILE_HEIGHT = 144;
static const int TILE_WIDTH_HALF = TILE_WIDTH / 2;
static const int TILE_HEIGHT_HALF = TILE_HEIGHT / 2;
static const int  TILE_ORIGIN_X = 0;
static const int TILE_ORIGIN_Y = 112;

//Other sprites
static const int  TILE_OFFSET_ORIGIN_X = TILE_WIDTH / 2;
static const int TILE_OFFSET_ORIGIN_Y = 112 / 2;

//Units
static const int  UNIT_OFFSET_ORIGIN_X = static_cast<int>(TILE_WIDTH / 2);
static const int UNIT_OFFSET_ORIGIN_Y = static_cast<int>(TILE_HEIGHT / 1.2);

static const int  RTS_UNIT_OFFSET_ORIGIN_X = static_cast<int>(TILE_WIDTH / 2);
static const int RTS_UNIT_OFFSET_ORIGIN_Y = static_cast<int>(TILE_HEIGHT / 1.3);

static sf::Vector2f toISO(int x, int y)
{
	auto isoX  = (x - y) * TILE_WIDTH_HALF;
	auto isoY  = (x + y) * TILE_HEIGHT_HALF;

	return sf::Vector2f(isoX, isoY);
}

static sf::Vector2f toISO(float x, float y)
{
	auto isoX = (x - y) * TILE_WIDTH_HALF;
	auto isoY = (x + y) * TILE_HEIGHT_HALF;

	return sf::Vector2f(isoX, isoY);
}
static sf::Vector2f toGridFloat(sf::Vector2f vec)
{
	auto x = ((vec.x - TILE_WIDTH_HALF) / TILE_WIDTH_HALF + vec.y / TILE_HEIGHT_HALF) / 2;
	auto y = (vec.y / TILE_HEIGHT_HALF - (vec.x - TILE_WIDTH_HALF) / TILE_WIDTH_HALF) / 2;

	return sf::Vector2f((x), (y));
}

static sf::Vector2i toGrid(sf::Vector2f vec)
{
	auto x = ((vec.x - TILE_WIDTH_HALF) / TILE_WIDTH_HALF + vec.y / TILE_HEIGHT_HALF) / 2;
	auto y = (vec.y / TILE_HEIGHT_HALF - (vec.x - TILE_WIDTH_HALF) / TILE_WIDTH_HALF) / 2;
	
	return sf::Vector2i(static_cast<int>(x), static_cast<int>(y));
}