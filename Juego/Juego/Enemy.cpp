#include "Enemy.hpp"


std::vector<Coord> adyacentes(const Dungeon::Matrix &m, Coord c){
	std::vector<Coord> ady;
	Coord up(c.x, c.y - 1);
	if (m.validCoordinates(up.x, up.y) && !Dungeon::isLocked(m[up.x][up.y])) ady.push_back(up);
	Coord down(c.x, c.y + 1);
	if (m.validCoordinates(down.x, down.y) && !Dungeon::isLocked(m[down.x][down.y])) ady.push_back(down);
	Coord left(c.x - 1, c.y);
	if (m.validCoordinates(left.x, left.y) && !Dungeon::isLocked(m[left.x][left.y])) ady.push_back(left);
	Coord rigth(c.x + 1, c.y);
	if (m.validCoordinates(rigth.x, rigth.y) && !Dungeon::isLocked(m[rigth.x][rigth.y])) ady.push_back(rigth);
	return ady;
}


std::vector<Coord> astar(const Dungeon::Matrix &m, Coord orig, Coord dest){
	std::unordered_map<Coord, Coord, CoordHash> parents;
	std::priority_queue<Casilla, std::vector<Casilla>, Compare> queue;
	Casilla actual(0, orig);
	parents[actual.pos] = actual.pos;
	queue.push(actual);
	bool encontrado = false;
	while (!queue.empty() && !encontrado){
		actual = queue.top();
		queue.pop();
		if (actual.pos == dest){
			encontrado = true;
			break;
		}
		std::vector<Coord> ady = adyacentes(m, actual.pos);
		for (Coord c : ady){
			if (parents.find(c) == parents.end()){
				Casilla n(c.distancia(dest), c);
				queue.push(n);
				parents[c] = actual.pos;
			}
		}
	}
	std::vector<Coord> path;
	if (encontrado){
		Coord a = dest;
		path.push_back(a);
		while (a != orig){
			a = parents[a];
			path.push_back(a);
		}
		std::reverse(path.begin(), path.end());
	}
	return path;
}

Enemy::Enemy(const sf::Texture& texture):
	LivingEntity(texture),
	_playerDetected(false),
	_isAttacking(false),
	_isBlocking(false),
	_decision(sf::seconds(1)),
	_impervious(IMPERVIOUSTIME)
{
	_ai = 0;
}

Enemy::Enemy(const sf::Texture& texture, unsigned int maxHP, unsigned int hp, sf::Vector2f speed, float attack) :
	LivingEntity(texture, maxHP, hp, speed, attack),
	_playerDetected(false),
	_isAttacking(false),
	_isBlocking(false),
	_decision(sf::seconds(1)),
	_impervious(IMPERVIOUSTIME)
{
	_ai = 0;
}

void Enemy::update(sf::Time dt, LivingEntity player, const Dungeon &dungeon, const TileMap &tiles){
	_decision -= dt;
	_heal -= dt;
	if (_decision <= sf::Time::Zero){
		_isAttacking = false;
		_isBlocking = false;
		if (_actions.empty())
			ai(player, dungeon, tiles);
		executeAction(_actions.front(), player, dungeon, tiles);
		_actions.pop();
		_decision = DECISIONTIME;
	}
	LivingEntity::update(dt);
}

void Enemy::setAI(unsigned int ai){
	_ai = ai;
}

bool Enemy::isPlayerInFront(LivingEntity player, const Dungeon &dungeon, const TileMap &tiles) const{
	bool ret = false;
	int x = 0, y = 0;
	int incx = 0, incy = 0;
	auto m = dungeon.getRoom(dungeon.getSelectedRoom());
	bool end = false;
	switch (this->getFacing()) {
	case NORTH:
		incy--;
		break;
	case SOUTH:
		incy++;
		break;
	case EAST:
		incx++;
		break;
	case WEST:
		incx--;
		break;
	default:
		break;
	}
	sf::Vector2u pos = tiles.getCellFromCoords(this->getPosition());
	x = pos.x; y = pos.y;
	while (!end && m.validCoordinates(x, y)) {
		if (Dungeon::isLocked(m[x][y])) {
			end = true;
		}
		else {
			x += incx;
			y += incy;
		}
	}
	x -= incx;
	y -= incy;
	unsigned int rectWidth = std::max(TILESIZE.x, std::abs(x - (int)pos.x)*TILESIZE.x);
	unsigned int rectHeight = std::max(TILESIZE.y, std::abs(y - (int)pos.y)*TILESIZE.y);
	sf::FloatRect r;
	switch (this->getFacing()) {
	case NORTH:
		r = sf::FloatRect(this->getPosition().x, this->getPosition().y - rectHeight, rectWidth, rectHeight);
		break;
	case SOUTH:
		r = sf::FloatRect(this->getPosition().x, this->getPosition().y + TILESIZE.y, rectWidth, rectHeight);
		break;
	case WEST:
		r = sf::FloatRect(this->getPosition().x - rectWidth, this->getPosition().y, rectWidth, rectHeight);
		break;
	case EAST:
		r = sf::FloatRect(this->getPosition().x + TILESIZE.x, this->getPosition().y, rectWidth, rectHeight);
		break;
	default:
		break;
	}
	return r.intersects(player.getFloatBounds());
}

bool Enemy::isBlocked(const Dungeon &dungeon, const TileMap &tiles) const{
	auto m = dungeon.getRoom(dungeon.getSelectedRoom());
	sf::Vector2u pos = tiles.getCellFromCoords(this->getPosition());
	switch (this->getFacing()) {
	case NORTH:
		pos.y--;
		break;
	case SOUTH:
		pos.y++;
		break;
	case EAST:
		pos.x++;
		break;
	case WEST:
		pos.x--;
		break;
	default:
		break;
	}
	return (m.validCoordinates(pos.x, pos.y) && Dungeon::isLocked(m[pos.x][pos.y]));
}

bool Enemy::isPlayerInRange(LivingEntity player, const Dungeon &dungeon, const TileMap &tiles) const{
	sf::FloatRect bound = this->getFloatBounds();
	switch (this->getFacing()) {
	case NORTH:
		bound.top -= TILESIZE.y;
		break;
	case SOUTH:
		bound.top += TILESIZE.y;
		break;
	case EAST:
		bound.left -= TILESIZE.x;
		break;
	case WEST:
		bound.left += TILESIZE.x;
		break;
	default:
		break;
	}
	return bound.intersects(player.getFloatBounds());
}

void Enemy::changeState(){
	this->_playerDetected = true;
}

void Enemy::attack(){
	this->_isAttacking = true;
}

void Enemy::block(){
	this->_isBlocking = true;
}

void Enemy::heal(){
	if (_heal <= sf::Time::Zero){
		this->increaseHealth(1);
		_heal = HEALTIME;
	}
}

void Enemy::forward(){
	switch (this->getFacing()) {
	case NORTH:
		setSpeed(sf::Vector2f(0, -NORMALSPEED));
		break;
	case SOUTH:
		setSpeed(sf::Vector2f(0, NORMALSPEED));
		break;
	case EAST:
		setSpeed(sf::Vector2f(NORMALSPEED, 0));
		break;
	case WEST:
		setSpeed(sf::Vector2f(-NORMALSPEED, 0));
		break;
	default:
		break;
	}
}

void Enemy::backward(){
	switch (this->getFacing()) {
	case NORTH:
		setSpeed(sf::Vector2f(0, NORMALSPEED));
		break;
	case SOUTH:
		setSpeed(sf::Vector2f(0, -NORMALSPEED));
		break;
	case EAST:
		setSpeed(sf::Vector2f(-NORMALSPEED, 0));
		break;
	case WEST:
		setSpeed(sf::Vector2f(NORMALSPEED, 0));
		break;
	default:
		break;
	}
}

void Enemy::turnLeft(){
	switch (this->getFacing()) {
	case NORTH:
		this->setFacing(Facing::EAST);
		break;
	case SOUTH:
		this->setFacing(Facing::WEST);
		break;
	case EAST:
		this->setFacing(Facing::NORTH);
		break;
	case WEST:
		this->setFacing(Facing::SOUTH);
		break;
	default:
		break;
	}
}

void Enemy::turnRight(){
	switch (this->getFacing()) {
	case NORTH:
		this->setFacing(Facing::WEST);
		break;
	case SOUTH:
		this->setFacing(Facing::EAST);
		break;
	case EAST:
		this->setFacing(Facing::SOUTH);
		break;
	case WEST:
		this->setFacing(Facing::NORTH);
		break;
	default:
		break;
	}
}

void Enemy::getCloserTo(LivingEntity player, const Dungeon &dungeon, const TileMap &tiles){
	auto enemyPos = tiles.getCellFromCoords(getPosition());
	auto path = astar(dungeon.getRoom(dungeon.getSelectedRoom()), Coord(enemyPos), Coord(tiles.getCellFromCoords(player.getPosition())));
	if (path.size() <= 1) return;
	if (path[1].x > enemyPos.x){
		setSpeed(sf::Vector2f(NORMALSPEED, 0));
	}
	else if (path[1].x < enemyPos.x){
		setSpeed(sf::Vector2f(-NORMALSPEED, 0));
	}
	else if (path[1].y > enemyPos.y){
		setSpeed(sf::Vector2f(0, NORMALSPEED));
	}
	else if (path[1].y < enemyPos.y){
		setSpeed(sf::Vector2f(0, -NORMALSPEED));
	}
}
void Enemy::executeAction(Actions a, LivingEntity player, const Dungeon &dungeon, const TileMap &tiles){
	switch (a)
	{
	case Enemy::ChangeState:
		changeState();
		break;
	case Enemy::Forward:
		forward();
		break;
	case Enemy::Backward:
		backward();
		break;
	case Enemy::Right:
		turnRight();
		break;
	case Enemy::Left:
		turnLeft();
		break;
	case Enemy::Advance:
		getCloserTo(player, dungeon, tiles);
		break;
	case Enemy::Attack:
		attack();
		break;
	case Enemy::Block:
		block();
		break;
	case Enemy::Heal:
		heal();
		break;
	default:
		break;
	}
}

void Enemy::ai(LivingEntity player, const Dungeon &dungeon, const TileMap &tiles){
	auto posP = player.getCenter();
	auto posE = getCenter();
	auto v = posP - posE;
	if (_ai == 0){
		if (!_playerDetected){
			// Patrol state
			if (isPlayerInFront(player, dungeon, tiles)){
				_actions.push(Actions::ChangeState);
			}
			else{
				_actions.push(Actions::Forward);
				if (isBlocked(dungeon, tiles)){
					_actions.push(Actions::Left);
				}
				else{
					_actions.push(Actions::Forward);
					_actions.push(Actions::Right);
					if (isPlayerInFront(player, dungeon, tiles)){
						_actions.push(Actions::Right);
					}
					else{
						_actions.push(Actions::Forward);
					}
				}
				_actions.push(Actions::Left);
			}
		}
		else{
			// Attack state
			switch (this->getHealth()){
			case 1:
				_actions.push(Actions::Heal);
				break;
			case 2:
				_actions.push(Actions::Heal);
				_actions.push(Actions::Backward);
				break;
			case 3:
				if (isPlayerInRange(player, dungeon, tiles)){
					_actions.push(Actions::Block);
				}
				else{
					_actions.push(Actions::Advance);
				}
				break;
			default:
				break;
			}
			_actions.push(Actions::Attack);
			_actions.push(Actions::Block);
		}
	}
	else{
		if (!_playerDetected){
			// Patrol state
		}
		else{
			// Attack state
		}
	}
}