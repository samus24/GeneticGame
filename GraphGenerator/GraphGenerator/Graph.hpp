#ifndef GRAPH_HPP
#define GRAPH_HPP

#include <set>
#include <queue>
#include "Pair.hpp"
#include "SFML\Graphics.hpp"
#include "Node.hpp"

struct Link {
	Node* from;
	Node* to;
	unsigned int cc;
};

class Graph : public sf::Drawable, public sf::Transformable{
public:
	Graph(unsigned int order, float density, unsigned int resX, unsigned int resY){
		srand(time(NULL));
		m_selected = -1;
		m_marked = -1;
		m_nodes.clear();
		// Generate 3 times order random nodes
		for (int i = 0; i < order*3; ++i){
			int width = getRandom(minSize, maxSize);
			int height = getRandom(minSize, maxSize);
			int posX = getRandom(0, resX - width);
			int posY = getRandom(0, resY - height);
			m_nodes.push_back(Node(i, sf::Vector2f(width, height), sf::Vector2f(posX,posY)));
		}

		// Sort them (larger first)
		std::sort(m_nodes.begin(), m_nodes.end());
		std::reverse(m_nodes.begin(), m_nodes.end());

		// Remove intersecting nodes
		for (int i = 0; i < m_nodes.size(); ++i){
			for (int j = i + 1; j < m_nodes.size(); ++j){
				Node a = m_nodes.at(i);
				Node b = m_nodes.at(j);
				if (a.intersects(b)){
					m_nodes.erase(m_nodes.begin() + j);	// erase b (nodes at right of b will be shifted left 1 position
					j--;								// so, we need to re-check the same position again.
				}
			}
		}

		// Remove overflow of nodes if exists
		while (m_nodes.size() > order){
			m_nodes.pop_back();
		}

		std::vector<Link> m_links;

		// Establish all links between nodes
		m_links.clear();
		for (int i = 0; i < m_nodes.size(); ++i){
			for (int j = i + 1; j < m_nodes.size(); ++j){
				Link l;
				l.from = &(m_nodes.at(i));
				l.to = &(m_nodes.at(j));
				l.cc = 0;
				m_links.push_back(l);
			}
		}

		// Delete random links until get a 3% of them
		int finalLinks = m_links.size() * density;
		while (m_links.size() > finalLinks){
			m_links.erase(m_links.begin() + getRandom(0, m_links.size() - 1));
		}

		for (int i = 0; i < m_links.size(); ++i){
			Link l = m_links.at(i);
			unsigned int fromNode = l.from->getId();
			unsigned int toNode = l.to->getId();
			m_conections[fromNode].push_back(l);
			m_conections[toNode].push_back(l);
		}

		calculateConnectedComponents();
	}

	unsigned int getOrder() const{
		return this->m_nodes.size();
	}

	unsigned int getDensity() const{
		unsigned int nLinks = 0;
		for (int i = 0; i < m_connectedComp.size(); ++i){
			nLinks += m_connectedComp[i].size();
		}
		return nLinks;
	}

	unsigned int getConnectedNodes() const{
		std::set<unsigned int> connected;
		for (Node n : m_nodes){
			if (getGradeOfNode(n) > 0){
				connected.emplace(n.getId());
			}
			
		}
		return connected.size();
	}
	unsigned int getUnconnectedNodes() const{
		return m_nodes.size() - getConnectedNodes();
	}

	const std::vector<std::set<unsigned int>> getConnectedComponents() const{
		return m_connectedComp;
	}

	Node getSelectedNode() const{
		if (m_selected != -1){
			return m_nodes.at(m_selected);
		}
		else return Node(-1, sf::Vector2f(0, 0), sf::Vector2f(0, 0));
	}

	unsigned int getGradeOfNode(Node n) const{
		unsigned int grade = 0;
		try{
			grade = m_conections.at(n.getId()).size();
			return grade;
		}
		catch (std::out_of_range e){
			return 0;
		}
	}

	bool selectNode(sf::Vector2i point){
		sf::Vector2f p(point);
		for (int i = 0; i < m_nodes.size(); ++i){
			if (m_nodes.at(i).contains(p)){
				m_nodes.at(i).setSelected(true);
				m_selected = i;
				return true;
			}
		}
		m_selected = -1;
		return false;
	}

	void releaseNode(){
		if (m_selected != -1){
			m_nodes.at(m_selected).setSelected(false);
			m_selected = -1;
		}
	}

	void moveSelectedNodeTo(sf::Vector2i point){
		if (m_selected != -1){
			m_nodes.at(m_selected).setPos(sf::Vector2f(point));
		}
	}

	bool markNode(sf::Vector2i point){
		sf::Vector2f p(point);
		for (int i = 0; i < m_nodes.size(); ++i){
			if (m_nodes.at(i).contains(p)){
				m_nodes.at(i).toggleMarked();
				m_marked = (m_marked == -1) ? i : -1;
				return (m_marked != -1);
			}
		}
		m_marked = -1;
		return false;
	}

	void unmarkNode(){
		if (m_marked != -1){
			m_nodes.at(m_marked).setMarked(false);
			m_marked = -1;
		}
	}

	bool linkNodes(sf::Vector2i point){
		sf::Vector2f p(point);
		Node nodeFrom = m_nodes.at(m_marked);
		for (int i = 0; i < m_nodes.size(); ++i){
			if (m_nodes.at(i).contains(p) && i != m_marked){
				Node nodeTo = m_nodes.at(i);
				bool connected = false;
				try{
					std::vector<Link> links = m_conections.at(nodeFrom.getId());
					for (int j = 0; j < links.size(); ++j){
						Link l = links.at(j);
						if (l.from->getId() == nodeTo.getId() || l.to->getId() == nodeTo.getId()){
							connected = true;
							links.erase(links.begin() + j);
							m_conections[nodeFrom.getId()] = links;
							links = m_conections.at(nodeTo.getId());
							for (int k = 0; k < links.size(); ++k){
								Link ll = links.at(k);
								if (ll.from->getId() == nodeFrom.getId() || ll.to->getId() == nodeFrom.getId()){
									links.erase(links.begin() + k);
									m_conections[nodeTo.getId()] = links;
									break;
								}
							}
							break;
						}
					}
				}
				catch (std::out_of_range e){
					// "From" node hasn't any links before
					std::vector<Link> links;
					m_conections[nodeFrom.getId()] = links;
				}
				if (!connected){
					Link l;
					l.from = &(m_nodes.at(m_marked));
					l.to = &(m_nodes.at(i));
					l.cc = 0;
					std::vector<Link> links = m_conections.at(nodeFrom.getId());
					links.push_back(l);
					m_conections[nodeFrom.getId()] = links;
					try{
						links = m_conections.at(nodeTo.getId());
					}
					catch (std::out_of_range e){
						links.clear();
					}
					links.push_back(l);
					m_conections[nodeTo.getId()] = links;

				}
				unmarkNode();
				calculateConnectedComponents();
				return true;
			}
			else if (m_nodes.at(i).contains(p) && i == m_marked){
				unmarkNode();
				return true;
			}
		}
		return false;
	}

	

	Node getNode(unsigned int id) const{
		for (int i = 0; i < m_nodes.size(); ++i){
			if (m_nodes.at(i).getId() == id){
				return m_nodes.at(i);
			}
		}
		return Node(-1, sf::Vector2f(0, 0), sf::Vector2f(0, 0));
	}

	int getNodePos(unsigned int id) const{
		for (int i = 0; i < m_nodes.size(); ++i){
			if (m_nodes.at(i).getId() == id){
				return i;
			}
		}
		return -1;
	}

	std::string serialize() const{
		std::string ret = "{";
		ret += "\"nCC\" : " + std::to_string(m_connectedComp.size()) + ",";
		ret += "\"CCs\" : [";
		for (std::set<unsigned int> cc : m_connectedComp){	// for each con. comp.
			ret += "{\"nnCC\" : " + std::to_string(cc.size()) +",";
			ret += "\"nodes\" : [";
			for (unsigned int id : cc){		// for each node
				Node n = getNode(id);
				ret += "{\"idNode\" : " + std::to_string(n.getId()) +",";
				sf::Vector2f size = n.getSize();
				ret += "\"width\" : " + std::to_string(static_cast<int>(size.x)) +",";
				ret += "\"height\" : " + std::to_string(static_cast<int>(size.y)) +",";
				try{
					std::vector<Link> neighbours = m_conections.at(id);
					ret += "\"nNeighbours\" : " + std::to_string(neighbours.size()) +",";
					ret += "\"neighbours\" : [";
					for (Link l : neighbours){	// for each neighbour
						ret += "{\"idNeigh\" : " + std::to_string(getOtherFromLink(n.getId(), l)) +"},";
					} // end neighbours
					ret.erase(ret.end()-1);	// Delete "," for last one
					ret += "]},";
				}
				catch (std::out_of_range e){
					ret += "\"nNeighbours\" : 0,";
					ret += "\"neighbours\" : []},";
				}
			}	// end nodes
			ret.erase(ret.end()-1);	// Delete "," for last one
			ret += "]},";
		}	// end con. comp.
		ret.erase(ret.end()-1);	// Delete "," for last one
		ret += "]}";

		return ret;
	}

	void organize(){
		unsigned int lastY;
		sf::Vector2f pos(10, 10);
		for (std::set<unsigned int> cc : m_connectedComp){
			lastY = organizeCC(cc, pos);
			if (lastY == -1){
				pos.x += 50;
				lastY = pos.y;
			}
			else{
				pos.x = 10;
			}
			pos.y = lastY;
		}
	}

private:

	unsigned int organizeCC(std::set<unsigned int> cc, sf::Vector2f pos){
		if (cc.size() == 1){
			int nodePos = getNodePos(*cc.begin());
			m_nodes[nodePos].setPos(pos);
			return -1;
		}
		else{
			std::queue<Pair<unsigned int, sf::Vector2f>> toAnalyze;
			std::set<unsigned int> organized;
			unsigned int lastY = pos.y;
			int i = 0;
			unsigned int otherChilds = 0;
			Pair<unsigned int, sf::Vector2f> presentNode;
			toAnalyze.push(Pair<unsigned int, sf::Vector2f>(*cc.begin(), pos));
			while (toAnalyze.size() > 0){
				presentNode = toAnalyze.front();
				toAnalyze.pop();
				organized.emplace(presentNode.first);
				int nodePos = getNodePos(presentNode.first);
				m_nodes[nodePos].setPos(presentNode.second);
				pos = presentNode.second;
				i = 0;
				otherChilds = 0;
				std::vector<Link> neigh = m_conections.at(presentNode.first);
				sf::Vector2f otherPos = pos;
				otherPos.x = pos.x + 50;
				for (Link l : neigh){
					int otherId = getOtherFromLink(presentNode.first, l);
					if (organized.find(otherId) == organized.end()){
						// other node hasn't been orginized yet
						otherPos.y = pos.y + otherChilds * 50;
						toAnalyze.push(Pair<unsigned int, sf::Vector2f>(otherId, otherPos));
						otherChilds++;
						bool hasUnorganizedChilds = false;
						std::vector<Link> childs = m_conections.at(otherId);
						for (Link ll : childs){
							if (organized.find(getOtherFromLink(otherId, ll)) == organized.end()){
								hasUnorganizedChilds = true;
								otherChilds++;
							}
						}
						if (hasUnorganizedChilds){
							otherChilds--;
						}
						lastY = std::max<unsigned int>(lastY, otherPos.y);
						++i;
					}
				}
			}
			return lastY + 50;
		}
	}

	unsigned int getOtherFromLink(unsigned int n, const Link l) const{
		if (l.from->getId() == n){
			return l.to->getId();
		}
		else{
			return l.from->getId();
		}
	}

	int getRandom(int from, int to){
		float random = (float)(rand() / (float)RAND_MAX);
		return from +  random * (to - from);
	}

	void calculateConnectedComponents(){
		m_connectedComp.clear();
		std::set<unsigned int> processed;
		unsigned int ncc = 1;	// For color of the links only
		for (Node n: m_nodes){
			if (processed.find(n.getId()) == processed.end()){
				// actual node hasn't been processed yet
				std::set<unsigned int> cc;
				calculateConnectedComponentFromNode(n, processed, cc, ncc);
				if (cc.size() > 1) ++ncc;	// only changes the color if the las cc has more than 1 node
				m_connectedComp.push_back(cc);
				
			}
		}
	}

	void calculateConnectedComponentFromNode(Node n, std::set<unsigned int> &processed, std::set<unsigned int> &cc, unsigned int ncc){
		processed.emplace(n.getId());
		cc.emplace(n.getId());
		try{
			std::vector<Link> links = m_conections.at(n.getId());
			for (int i = 0; i < links.size(); ++i){
				Link l = links.at(i);
				links[i].cc = ncc;
				m_conections[n.getId()] = links;
				Node* foreignNode = (l.from->getId() == n.getId()) ? l.to : l.from;
				if (processed.find(foreignNode->getId()) == processed.end()){
					// next node hasn't been processed yet
					calculateConnectedComponentFromNode(*(foreignNode), processed, cc, ncc);
				}
			}
		}
		catch (std::out_of_range e){
			return;
		}
	}

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		states.transform *= getTransform();

		for (Node n : m_nodes){
			try{
				std::vector<Link> links = m_conections.at(n.getId());
				for (Link l : links){
					sf::Color color;
					switch (l.cc){
					case 1:
						color = sf::Color::Red;
						break;
					case 2:
						color = sf::Color::Yellow;
						break;
					case 3:
						color = sf::Color::Cyan;
						break;
					case 4:
						color = sf::Color::Green;
						break;
					case 5:
						color = sf::Color::Magenta;
						break;
					case 6:
						color = sf::Color::Blue;
						break;
					default:
						color = sf::Color(255 - (l.cc * 3), 255 - (l.cc * 5), 255 - (l.cc));
					}
					sf::Vertex line[] =
					{
						sf::Vertex(l.from->getMiddlePoint(), color),
						sf::Vertex(l.to->getMiddlePoint(), color)
					};
					target.draw(line, 2, sf::Lines);
				}
			}
			catch (std::out_of_range e){

			}
			target.draw(n, states);
		}
		
	}

	const int minSize = 10;
	const int maxSize = 40;
	unsigned int m_order;
	std::vector<Node> m_nodes;
	unsigned int m_selected;
	unsigned int m_marked;

	std::map<unsigned int, std::vector<Link>> m_conections;
	std::vector<std::set<unsigned int>> m_connectedComp;
};

#endif 