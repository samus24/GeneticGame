#ifndef BOOK_RESOURCEIDENTIFIERS_HPP
#define BOOK_RESOURCEIDENTIFIERS_HPP


// Forward declaration of SFML classes
namespace sf
{
	class Texture;
	class Font;
}

namespace Textures
{
	enum ID
	{
		TitleScreen,
		LoadingScreen,
		GameOverScreen,
		Title,
		GUI,
		TileMap,
		Player,
		Enemy,
		Key,
		PlayerMods
	};
}

namespace Fonts
{
	enum ID
	{
		Main,
		Alagard,
		Pixel
	};
}

// Forward declaration and a few type definitions
template <typename Resource, typename Identifier>
class ResourceHolder;

typedef ResourceHolder<sf::Texture, Textures::ID>	TextureHolder;
typedef ResourceHolder<sf::Font, Fonts::ID>			FontHolder;

#endif // BOOK_RESOURCEIDENTIFIERS_HPP
