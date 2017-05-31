#ifndef BUTTON_HPP
#define BUTTON_HPP

#include "ResourceIdentifiers.hpp"
#include "ResourceHolder.hpp"
#include "Utils.hpp"

#include <SFML/Graphics.hpp>

#include <vector>
#include <string>
#include <memory>
#include <functional>

class Button : public sf::Drawable, public sf::Transformable{
public:
	typedef std::shared_ptr<Button>		Ptr;
	typedef std::function<void()>		Callback;


public:
	Button(const FontHolder& fonts, const TextureHolder& textures) : 
		mCallback()
		, mTexture(textures.get(Textures::ButtonNormal))
		, mSprite()
		, mText("", fonts.get(Fonts::Alagard), 18)
		, mIsToggle(false)
	{
		mSprite.setTexture(mTexture);

		sf::FloatRect bounds = mSprite.getLocalBounds();
		mText.setPosition(bounds.width / 2.f, bounds.height / 2.f);
	}

	void setCallback(Callback callback){
		mCallback = std::move(callback);
	}

	void setText(const std::string& text){
		mText.setString(text);
		centerOrigin(mText);
	}

	void setToggle(bool flag){
		mIsToggle = flag;
	}

	bool isSelectable() const{
		return true;
	}

	void select(){
		mIsSelected = true;
		mSprite.setColor(sf::Color::Blue);
	}

	void deselect(){
		mIsSelected = false;

		mSprite.setColor(sf::Color::Transparent);
	}

	void activate(){
		mIsActive = true;

		// If we are toggle then we should show that the button is pressed and thus "toggled".
		if (mIsToggle){
			mSprite.rotate(180);
			mSprite.setColor(sf::Color::Yellow);
		}
				
		if (mCallback)
			mCallback();

		// If we are not a toggle then deactivate the button since we are just momentarily activated.
		if (!mIsToggle)
			deactivate();
	}
	void deactivate(){
		mIsActive = false;
		mSprite.rotate(180);
		if (mIsToggle)
		{
			// Reset texture to right one depending on if we are selected or not.
			if (mIsSelected)
				mSprite.setColor(sf::Color::Blue);
			else
				mSprite.setColor(sf::Color::Transparent);
		}
	}

	void handleEvent(const sf::Event& event){

	}


private:
	void draw(sf::RenderTarget& target, sf::RenderStates states) const{
		states.transform *= getTransform();
		target.draw(mSprite, states);
		target.draw(mText, states);
	}


private:
	bool				mIsSelected;
	bool				mIsActive;

	Callback				mCallback;
	const sf::Texture&		mTexture;
	sf::Sprite				mSprite;
	sf::Text				mText;
	bool					mIsToggle;
};

#endif