#include <iostream>
#include <istream>
#include <string>

#include "Graph.h"

int main() {

	char MainMenuText[1000] =
		"=========== Main menu ===========\n"
		"\n"
		//"Komande mozete odabrati unosom\n"
		//"broja komande i pritiskom na enter.\n"
		"You can choose commands by entering\n"
		"in the number of a command.\n"
		"\n"
		//"Ako komanda zahteva dodatne parametre\n"
		//"njihov unos ce se zatraziti nakon\n"
		//"odabira komande.\n "
		"If a command requires additional parameters\n"
		"you will be asked to enter them\n"
		"after choosing the command.\n"
		"\n"
		"List of commands:\n"
		"\n"
		" 1. Load graph from a text file\n"
		" 2. Add node to graph\n"
		" 3. Remove node from graph\n"
		" 4. Add edge to graph\n"
		" 5. Remove edge from graph\n"
		" 6. Print graph\n"
		" 7. Remove graph\n"
		" 8. Like a post\n"
		" 9. Biggest connected component in graph\n"
		" 10. Print users on most likely path from one to another\n"
		" 11. Finding the k-th most influential user\n"
		"\n";

	int selectedCommand = 0;
	std::cout << MainMenuText << std::endl;

	bool graphInit = false;
	std::string line, name1, name2, weightstring = "";
	Vertex* a = nullptr;
	Vertex* b = nullptr;
	float weight = 0.0f;
	int k = 0;
	Graph* g = new Graph;

	while (true) {
		std::cout << "> ";
		std::getline(std::cin, line);
		if (line == "") continue;

		try {
			selectedCommand = std::stoi(line);
			if (selectedCommand > 11 || selectedCommand < 1) {
				std::cout << "Error! Invalid command number." << std::endl;
			}
		}
		catch (std::invalid_argument const& ex) {
			std::cout << "Error! " << ex.what() << std::endl;
			continue;
		}
		catch (std::out_of_range const& ex) {
			std::cout << "Error! " << ex.what() << std::endl;
			continue;
		}

		switch (selectedCommand) {
			case 1: // Ucitavanje grafa
				std::cout << "Enter file name: ";
				std::getline(std::cin, line);
				if (graphInit) {
					g->deleteGraph();
					graphInit = false;
				}
				if (g->createGraph(line))
				{
					graphInit = true;
				}
				break;
			case 2: // Dodavanje cvora
				std::cout << "Enter node name: ";
				std::getline(std::cin, line);
				g->addVertex(line);
				if (!graphInit) {
					graphInit = true;
					break;
				}
				break;
			case 3: // Uklanjanje cvora
				if (!graphInit) {
					std::cout << "Error! Graph not initialized.\n";
					break;
				}
				std::cout << "Enter node name for removal: ";
				std::getline(std::cin, line);
				if (!g->removeVertex(line)) {
					std::cout << "Error! Node with name: " << line << " doesn't exist.\n";
				}
				break;
			case 4: // Dodavanje ivice
				if (!graphInit) {
					std::cout << "Error! Graph not initialized.\n";
					break;
				}
				std::cout << "Enter name of first node: ";
				std::getline(std::cin, name1);
				std::cout << "Enter name of second node: ";
				std::getline(std::cin, name2);
				std::cout << "Enter edge weight: ";
				std::getline(std::cin, weightstring);
				try {
					weight = std::stof(weightstring);
				}
				catch (std::invalid_argument const& ex) {
					std::cout << "Error! " << ex.what() << std::endl;
					continue;
				}
				a = g->findVertex(name1);
				if (!a) {
					std::cout << "Error! Node with name: " << name1 << " doesn't exist!\n";
					break;
				}
				b = g->findVertex(name2);
				if (!b) {
					std::cout << "Error! Node with name: " << name2 << " doesn't exist!\n";
					break;
				}
				if (weight < 0 || weight > 1) {
					std::cout << "Error! Edge weight must be in range [0,1]!\n";
					break;
				}
				g->addEdge(a, b, weight);
				break;
			case 5: // Uklanjanje ivice
				if (!graphInit) {
					std::cout << "Error! Graph not initialized.\n";
					break;
				}
				std::cout << "Enter name of first node: ";
				std::getline(std::cin, name1);
				std::cout << "Enter name of second node: ";
				std::getline(std::cin, name2);
				a = g->findVertex(name1);
				if (!a) {
					std::cout << "Error! Node with name: " << name1 << " doesn't exist!\n";
					break;
				}
				b = g->findVertex(name2);
				if (!b) {
					std::cout << "Error! Node with name: " << name2 << " doesn't exist!\n";
					break;
				}
				if (!g->removeEdge(a, b)) {
					std::cout << "Error! Edge: " << name1 << " -> " << name2 << " doesn't exist!\n";
				}
				break;
			case 6: // Ispis grafa
				if (!graphInit) {
					std::cout << "Error! Graph not initialized.\n";
					break;
				}
				g->printGraph();
				//std::cout << "\n";
				//g->printInverseGraph();
				break;
			case 7: // Brisanje grafa
				if (!graphInit) {
					std::cout << "Error! Graph not initialized.\n";
					break;
				}
				g->deleteGraph();
				graphInit = false;
				break;
			case 8: // Lajkovanje
				if (!graphInit) {
					std::cout << "Error! Graph not initialized.\n";
					break;
				}
				std::cout << "Enter name of node that is liking a post: ";
				std::getline(std::cin, name1);
				std::cout << "Enter name of node that owns the post: ";
				std::getline(std::cin, name2);
				a = g->findVertex(name1);
				if (!a) {
					std::cout << "Error! Node with name: " << name1 << " doesn't exist!\n";
					break;
				}
				b = g->findVertex(name2);
				if (!b) {
					std::cout << "Error! Node with name: " << name2 << " doesn't exist!\n";
					break;
				}
				if (!g->alterEdgeWeight(a, b, 0.1f)) {
					std::cout << "Error! Edge between " << name1 << " and " << name2 << " doesn't exist!\n";
				}
				break;
			case 9: // Najveca komponenta
				if (!graphInit) {
					std::cout << "Error! Graph not initialized.\n";
					break;
				}
				g->printStrongestComponent();
				break;
			case 10: // Ispis korisnika na najverovatnijem putu od jednog do drugog
				if (!graphInit) {
					std::cout << "Error! Graph not initialized.\n";
					break;
				}
				std::cout << "Enter name of first node: ";
				std::getline(std::cin, name1);
				std::cout << "Enter name of second node: ";
				std::getline(std::cin, name2);
				a = g->findVertex(name1);
				if (!a) {
					std::cout << "Error! Node with name: " << name1 << " doesn't exist!\n";
					break;
				}
				b = g->findVertex(name2);
				if (!b) {
					std::cout << "Error! Node with name: " << name2 << " doesn't exist!\n";
					break;
				}
				g->printPathFromAtoB(a, b);
				break;
			case 11: // k-ta najuticajnija osoba
				if (!graphInit) {
					std::cout << "Error! Graph not initialized.\n";
					break;
				}
				std::cout << "Enter k: ";
				std::getline(std::cin, line);
				try {
					k = std::stoi(line);
				}
				catch (std::invalid_argument const& ex) {
					std::cout << "Error! " << ex.what() << std::endl;
					continue;
				}
				g->printKthInfluential(k);
			default:
				break;
		}

	}
}

