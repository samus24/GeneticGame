#ifndef BOOK_RESOURCEIDENTIFIERS_HPP
#define BOOK_RESOURCEIDENTIFIERS_HPP


// Forward declaration of SFML classes
namespace sf
{
	class Texture;
	class Font;
	class SoundBuffer;
	class Music;
}

namespace Textures
{
	enum ID
	{
		TitleScreen,
		LoadingScreen,
		GameOverScreen,
		Title,
		Controls,
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

namespace Musics
{
	enum ID
	{
		Start,
		Loading,
		Game
	};
}

namespace Sounds
{
	enum ID
	{
		Key,
		Lose,
		Sword,
		Portal,
		Chest,
		Hurt,
		Fireball
	};
}

// Forward declaration and a few type definitions
template <typename Resource, typename Identifier>
class ResourceHolder;
template <typename Resource, typename Identifier>
class RealMusicHolder;

typedef ResourceHolder<sf::Texture, Textures::ID>	TextureHolder;
typedef ResourceHolder<sf::Font, Fonts::ID>			FontHolder;
typedef RealMusicHolder<sf::Music, Musics::ID>		MusicHolder;
typedef ResourceHolder<sf::SoundBuffer, Sounds::ID>	SoundHolder;

#endif // BOOK_RESOURCEIDENTIFIERS_HPP
