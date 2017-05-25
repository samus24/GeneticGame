#ifndef CHEST_HPP
#define CHEST_HPP

#include<stdlib.h>
#include<time.h>

enum Elem
{
	MAX_LIFE, ONE_LIFE, KEY
};

class Chest
{
	private:
		Elem elem;
		int posX;
		int posY;

	public:
		Chest()
		{
			srand(time(NULL));
			elem = rand() % (4 - 1);
			posX = 0;
			posY = 0;
		}

		Elem getElem()
		{
			return elem;
		}

		int getPosX()
		{
			return posX;
		}

		int getPosY()
		{
			return posY;
		}
};

#endif
