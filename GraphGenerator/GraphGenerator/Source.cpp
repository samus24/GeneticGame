#include <SFML\Graphics.hpp>

#include <iostream>
#include <fstream>
#include "Graph.hpp"
#include "Button.hpp"
#include "ScrollBar.hpp"

#define MAX_NODES 250
#define MAX_DENSITY 1


std::string getGraphInfo(Graph* g){
	std::string graphInfo = "Nodes: " + std::to_string(g->getOrder()) + "\nLinks: " + std::to_string(g->getDensity()) +
		"\nConnected Nodes: " + std::to_string(g->getConnectedNodes()) + "\nUnconnected nodes: " + std::to_string(g->getUnconnectedNodes());

	std::vector<std::set<unsigned int>> conComps = g->getConnectedComponents();
	graphInfo += "\nConnected Components: " + std::to_string(conComps.size());
	int i = 0;
	int pos = 0;
	for (std::set<unsigned int> cc : conComps){
		graphInfo += "\n\tCC" + std::to_string(i) + ": ";
		for (unsigned int n : cc){
			graphInfo += std::to_string(n) + ", ";
			++pos;
			if (pos > 10){
				graphInfo += "\n\t\t";
				pos = 0;
			}
		}
		pos = 0;
		++i;
	}
	graphInfo += "\n";

	return graphInfo;
}

int main(){

	sf::RenderWindow window(sf::VideoMode(1000,750), "Graph Generator");

	Graph* g = new Graph(50, 0.03, 600, 600);	// Number of nodes, density of links, resolution (x,y) to emplace rectangles
	std::string graphInfo = getGraphInfo(g);

	sf::Font f;
	f.loadFromFile("arial.ttf");
	sf::Text t(graphInfo, f);
	t.setCharacterSize(15);
	t.setPosition(620, 0);
	t.setFillColor(sf::Color::White);

	ReLoadButton rb(sf::Vector2f(800+10, 600), sf::Vector2f(30, 30));
	ExportButton eb(sf::Vector2f(800+50, 600), sf::Vector2f(30, 30));
	OrganizeButton ob(sf::Vector2f(800+90, 600), sf::Vector2f(30, 30));

	std::vector<std::string> nodes;
	for (int i = 0; i < MAX_NODES; ++i){
		nodes.push_back(std::to_string(i));
	}

	std::vector<std::string> density;
	for (int i = 0; i < MAX_DENSITY * 101; ++i){
		density.push_back(std::to_string(i) + "%");
	}

	sf::ScrollBar sbNodes(sf::Vector2f(700 + 10, 650), sf::Vector2f(250, 20), nodes, sf::Orientation::HORIZONTAL);
	sf::ScrollBar sbDensity(sf::Vector2f(700 + 10, 700), sf::Vector2f(250, 20), density, sf::Orientation::HORIZONTAL);

	sbNodes.setSelectedIndex(50);
	sbDensity.setSelectedIndex(3);

	sf::Text nodesText("Nodes", f);
	nodesText.setCharacterSize(15);
	nodesText.setPosition(660, 650);
	nodesText.setFillColor(sf::Color::White);

	sf::Text densityText("Density", f);
	densityText.setCharacterSize(15);
	densityText.setPosition(650, 700);
	densityText.setFillColor(sf::Color::White);

	bool selecting = false, marking = false;
	sf::Mouse::Button button = sf::Mouse::Button::Middle;
	while (window.isOpen())
	{
		// handle events
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
			else if (event.type == sf::Event::MouseButtonPressed){
				if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)){
					button = sf::Mouse::Button::Left;
				}
				else if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Right)){
					button = sf::Mouse::Button::Right;
				}
			}
			else if (event.type == sf::Event::MouseButtonReleased){
				int indexNodes = sbNodes.getIndexFromPointer(sf::Mouse::getPosition(window));
				int indexDensity = sbDensity.getIndexFromPointer(sf::Mouse::getPosition(window));
				switch (button){
				case sf::Mouse::Button::Left:
					if (rb.contains(sf::Vector2f(sf::Mouse::getPosition(window)))){
						delete(g);
						g = new Graph(std::stoi(sbNodes.getSelectedElement()), std::stof(sbDensity.getSelectedElement())*(0.01), 600, 600);
						graphInfo = getGraphInfo(g);
						selecting = false;
						marking = false;
					}
					else if (eb.contains(sf::Vector2f(sf::Mouse::getPosition(window)))){
						std::ofstream myfile;
						myfile.open("graphData.json");
						myfile << g->serialize();
						myfile.close();
					}
					else if (ob.contains(sf::Vector2f(sf::Mouse::getPosition(window)))){
						g->organize();
					}
					else if (indexNodes != -1){
						sbNodes.setSelectedIndex(indexNodes);
					}
					else if (indexDensity != -1){
						sbDensity.setSelectedIndex(indexDensity);
					}
					else{ 
						selecting = !selecting;
						if (selecting){
							selecting = g->selectNode(sf::Mouse::getPosition(window));
						}
						else{
							g->releaseNode();
						}
					}
					break;
				case sf::Mouse::Button::Right:
					if (!selecting){
						if (!marking){
							marking = g->markNode(sf::Mouse::getPosition(window));
						}
						else{
							marking = !g->linkNodes(sf::Mouse::getPosition(window));
							if (!marking){
								graphInfo = getGraphInfo(g);
							}
						}
					}
					break;
				default:
					break;
				}
				
			}
		}

		if (selecting){
			g->moveSelectedNodeTo(sf::Mouse::getPosition(window));
		}
		t.setString("");
		Node s = g->getSelectedNode();
		if (s.getId() == -1){
			t.setString(graphInfo + "\nSelected Node: NONE");
		}
		else{
			t.setString(graphInfo + "\nSelected Node: " + std::to_string(s.getId()) + "\n\tNode grade: " + std::to_string(g->getGradeOfNode(s)));
		}
		
		// draw the map
		window.clear();
		window.draw(*g);
		window.draw(t);
		window.draw(rb);
		window.draw(eb);
		window.draw(ob);
		window.draw(nodesText);
		window.draw(densityText);
		sbNodes.draw(&window);
		sbDensity.draw(&window);
		window.display();
	}
}