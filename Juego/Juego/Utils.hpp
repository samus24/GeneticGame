#ifndef UTILS_HPP
#define UTILS_HPP

#include <SFML/Graphics.hpp>
#include <unordered_map>

const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;
const sf::Vector2f WINDOW_RES(WINDOW_WIDTH, WINDOW_HEIGHT);

const sf::Vector2u TILESIZE(16,16);
const sf::IntRect  TILE0(0, 0, 16, 16);

const sf::Vector2f BUTTONSIZE(280, 60);
const sf::IntRect  BUTTONSELECTED1RECT(15, 125, BUTTONSIZE.x, BUTTONSIZE.y);
const sf::IntRect  BUTTONNORMAL1RECT(15, 203, BUTTONSIZE.x, BUTTONSIZE.y);
const sf::IntRect  BUTTONSELECTED2RECT(15, 281, BUTTONSIZE.x, BUTTONSIZE.y);
const sf::IntRect  BUTTON2NORMALRECT(15, 359, BUTTONSIZE.x, BUTTONSIZE.y);
const sf::IntRect  LOCKED_KEY(0, 0, 64, 64);
const sf::IntRect  UNLOCKED_KEY(64, 0, 64, 64);
const sf::IntRect  FULL_HEART(0, 0, 16, 16);
const sf::IntRect  HALF_HEART(16, 0, 16, 16);
const sf::IntRect  EMPTY_HEART(32, 0, 16, 16);
const sf::IntRect  ATTACK_BOOST(48, 0, 16, 16);
const sf::IntRect  SPEED_BOOST(64, 0, 16, 16);
const sf::IntRect  ATTACK_SKULL(80, 0, 16, 16);

const sf::IntRect UPRECT(0, 48, TILESIZE.x, TILESIZE.y);
const sf::IntRect DOWNRECT(0, 0, TILESIZE.x, TILESIZE.y);
const sf::IntRect LEFTRECT(0, 16, TILESIZE.x, TILESIZE.y);
const sf::IntRect RIGHTRECT(0, 32, TILESIZE.x, TILESIZE.y);

const sf::IntRect ATTACKDOWN(0, 64, TILESIZE.x, TILESIZE.y);
const sf::IntRect ATTACKUP(48, 64, TILESIZE.x, TILESIZE.y);
const sf::IntRect ATTACKRIGHT(16, 64, TILESIZE.x, TILESIZE.y);
const sf::IntRect ATTACKLEFT(32, 64, TILESIZE.x, TILESIZE.y);

const float NORMALSPEED = 70;

const int MAX_HEALTH = 6;

const std::string TILEPATH = "Media/Textures/TileMap.png";

const sf::Time HELPTIME = sf::seconds(4);
const sf::Time TPCOOLDOWN = sf::seconds(3);
const sf::Time DAMAGECOOLDOWN = sf::milliseconds(300);
const sf::Time TIMEPERFRAME = sf::milliseconds(300);
const sf::Time IMPERVIOUSTIME = sf::milliseconds(500);
const sf::Time DECISIONTIME = sf::milliseconds(300);
const sf::Time HEALTIME = sf::seconds(3);
const sf::Time ATTACKTIME = sf::seconds(1);

// POINTS
const unsigned int KEYPOINTS = 100;
const unsigned int KILLPOINTS = 50;
const unsigned int CHESTPOINTS = 10;
const unsigned int DUNGEONPOINTS = 500;
const unsigned int ALIVEPOINTS = 1;			// Points per second alive

const int NSPRITES = 4;
const std::vector<std::string> LOADINGMSG = {
	"Loading Resources",
	"You are about to enter in a dangerous world\n    Prepare yourself to fight for your life \n   whilst brave enemies try to destroy you.",
	"Thank you Mario! But our princess is in another castle.",
	"There are no choices.\n Nothing but a straight line.",
	"You've met with a terrible fate, haven't you?",
	"There's no use crying over every mistake, \n you just keep on trying until you run out of cake",
	"We all make choices, but in the end, \n our choices make us",
	"Mess with the best, you will die like the rest",
	"It's dangerous to go alone! Take this"
};

const std::unordered_map<int, sf::IntRect> WALLTILE = {
	{ 0, sf::IntRect(32, 48, TILESIZE.x, TILESIZE.y)},
	{ 1, sf::IntRect(16, 48, TILESIZE.x, TILESIZE.y) },
	{ 2, sf::IntRect(32, 64, TILESIZE.x, TILESIZE.y) },
	{ 3, sf::IntRect(16, 64, TILESIZE.x, TILESIZE.y) },
	{ 4, sf::IntRect(0, 64, TILESIZE.x, TILESIZE.y) },
	{ 12, sf::IntRect(32, 64, TILESIZE.x, TILESIZE.y) },
	{ 13, sf::IntRect(0, 48, TILESIZE.x, TILESIZE.y) },
	{ 14, sf::IntRect(0, 64, TILESIZE.x, TILESIZE.y) },
	{ 23, sf::IntRect(32, 32, TILESIZE.x, TILESIZE.y) },
	{ 24, sf::IntRect(16, 32, TILESIZE.x, TILESIZE.y) },
	{ 34, sf::IntRect(0, 32, TILESIZE.x, TILESIZE.y) },
	{ 123, sf::IntRect(80, 48, TILESIZE.x, TILESIZE.y) },
	{ 124, sf::IntRect(64, 64, TILESIZE.x, TILESIZE.y) },
	{ 134, sf::IntRect(48, 48, TILESIZE.x, TILESIZE.y) },
	{ 234, sf::IntRect(64, 32, TILESIZE.x, TILESIZE.y) },
	{ 1234, sf::IntRect(64, 48, TILESIZE.x, TILESIZE.y) }
};

void centerOrigin(sf::Sprite& sprite);

void centerOrigin(sf::Text& text);

#endif