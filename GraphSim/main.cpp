#include <iostream>
#include <istream>
#include <string>

#include "Graph.h"

int main() {

	char MainMenuText[1000] =
		"========= Glavni meni =========\n"
		"\n"
		"Komande mozete odabrati unosom\n"
		"broja komande i pritiskom na enter.\n"
		"\n"
		"Ako komanda zahteva dodatne parametre\n"
		"njihov unos ce se zatraziti nakon\n"
		"odabira komande.\n "
		"\n"
		"Lista komandi:\n"
		"\n"
		" 1. Ucitavanje grafa pomocu tekstualnog fajla\n"
		" 2. Dodavanje cvora u graf\n"
		" 3. Uklanjanje cvora iz grafa\n"
		" 4. Dodavanje grane u graf\n"
		" 5. Uklanjanje grane iz grafa\n"
		" 6. Ispis reprezentacije grafa\n"
		" 7. Brisanje grafa iz memorije\n"
		" 8. Lajkovanje objave\n"
		" 9. Nalazenje najvece komponente u grafu\n"
		" 10. Ispis korisnika na najverovatnijem putu od jednog do drugog\n"
		" 11. Odredjivanje k-te osobe najveceg uticaja\n"
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
				std::cout << "Greska! Nevalidan broj komande." << std::endl;
			}
		}
		catch (std::invalid_argument const& ex) {
			std::cout << "Greska! " << ex.what() << std::endl;
			continue;
		}
		catch (std::out_of_range const& ex) {
			std::cout << "Greska! " << ex.what() << std::endl;
			continue;
		}

		switch (selectedCommand) {
			case 1: // Ucitavanje grafa
				std::cout << "Uneti naziv fajla: ";
				std::getline(std::cin, line);
				if (graphInit) {
					g->deleteGraph();
				}
				if (g->createGraph(line))
				{
					graphInit = true;
				}
				break;
			case 2: // Dodavanje cvora
				std::cout << "Uneti naziv cvora za umetanje: ";
				std::getline(std::cin, line);
				g->addVertex(line);
				if (!graphInit) {
					graphInit = true;
					break;
				}
				break;
			case 3: // Uklanjanje cvora
				if (!graphInit) {
					std::cout << "Greska! Graf nije inicijalizovan.\n";
					break;
				}
				std::cout << "Uneti naziv cvora za brisanje: ";
				std::getline(std::cin, line);
				if (!g->removeVertex(line)) {
					std::cout << "Greska! cvor sa imenom: " << line << " ne postoji da bi se obrisao.\n";
				}
				break;
			case 4: // Dodavanje ivice
				if (!graphInit) {
					std::cout << "Greska! Graf nije inicijalizovan.\n";
					break;
				}
				std::cout << "Uneti naziv pocetnog cvora grane: ";
				std::getline(std::cin, name1);
				std::cout << "Uneti naziv krajnjeg cvora grane: ";
				std::getline(std::cin, name2);
				std::cout << "Uneti tezinu grane: ";
				std::getline(std::cin, weightstring);
				try {
					weight = std::stof(weightstring);
				}
				catch (std::invalid_argument const& ex) {
					std::cout << "Greska! " << ex.what() << std::endl;
					continue;
				}
				a = g->findVertex(name1);
				if (!a) {
					std::cout << "Greska! Cvor sa imenom: " << name1 << " ne postoji!\n";
					break;
				}
				b = g->findVertex(name2);
				if (!b) {
					std::cout << "Greska! Cvor sa imenom: " << name2 << " ne postoji!\n";
					break;
				}
				if (weight < 0 || weight > 1) {
					std::cout << "Greska! Tezina ivice mora biti u opsegu [0,1]!\n";
					break;
				}
				g->addEdge(a, b, weight);
				break;
			case 5: // Uklanjanje ivice
				if (!graphInit) {
					std::cout << "Greska! Graf nije inicijalizovan.\n";
					break;
				}
				std::cout << "Uneti naziv pocetnog cvora grane: ";
				std::getline(std::cin, name1);
				std::cout << "Uneti naziv krajnjeg cvora grane: ";
				std::getline(std::cin, name2);
				a = g->findVertex(name1);
				if (!a) {
					std::cout << "Greska! Cvor sa imenom: " << name1 << " ne postoji!\n";
					break;
				}
				b = g->findVertex(name2);
				if (!b) {
					std::cout << "Greska! Cvor sa imenom: " << name2 << " ne postoji!\n";
					break;
				}
				if (!g->removeEdge(a, b)) {
					std::cout << "Greska! Ivica sa cvorovima: " << name1 << " -> " << name2 << " ne postoji da bi se obrisala.\n";
				}
				break;
			case 6: // Ispis grafa
				if (!graphInit) {
					std::cout << "Greska! Graf nije inicijalizovan.\n";
					break;
				}
				g->printGraph();
				//std::cout << "\n";
				//g->printInverseGraph();
				break;
			case 7: // Brisanje grafa
				if (!graphInit) {
					std::cout << "Greska! Graf nije inicijalizovan.\n";
					break;
				}
				g->deleteGraph();
				graphInit = false;
				break;
			case 8: // Lajkovanje
				if (!graphInit) {
					std::cout << "Greska! Graf nije inicijalizovan.\n";
					break;
				}
				std::cout << "Uneti naziv osobe koje lajkuje: ";
				std::getline(std::cin, name1);
				std::cout << "Uneti naziv osobe cija je objava: ";
				std::getline(std::cin, name2);
				a = g->findVertex(name1);
				if (!a) {
					std::cout << "Greska! Cvor sa imenom: " << name1 << " ne postoji!\n";
					break;
				}
				b = g->findVertex(name2);
				if (!b) {
					std::cout << "Greska! Cvor sa imenom: " << name2 << " ne postoji!\n";
					break;
				}
				if (!g->alterEdgeWeight(a, b, 0.1f)) {
					std::cout << "Greska! Odnos izmedju " << name1 << " i " << name2 << " ne postoji.\n";
				}
				break;
			case 9: // Najveca komponenta
				if (!graphInit) {
					std::cout << "Greska! Graf nije inicijalizovan.\n";
					break;
				}
				g->printStrongestComponent();
				break;
			case 10: // Ispis korisnika na najverovatnijem putu od jednog do drugog
				if (!graphInit) {
					std::cout << "Greska! Graf nije inicijalizovan.\n";
					break;
				}
				std::cout << "Uneti naziv pocetnog cvora grane: ";
				std::getline(std::cin, name1);
				std::cout << "Uneti naziv krajnjeg cvora grane: ";
				std::getline(std::cin, name2);
				a = g->findVertex(name1);
				if (!a) {
					std::cout << "Greska! Cvor sa imenom: " << name1 << " ne postoji!\n";
					break;
				}
				b = g->findVertex(name2);
				if (!b) {
					std::cout << "Greska! Cvor sa imenom: " << name2 << " ne postoji!\n";
					break;
				}
				g->printPathFromAtoB(a, b);
				break;
			case 11: // k-ta najuticajnija osoba
				if (!graphInit) {
					std::cout << "Greska! Graf nije inicijalizovan.\n";
					break;
				}
				std::cout << "Uneti k: ";
				std::getline(std::cin, line);
				try {
					k = std::stoi(line);
				}
				catch (std::invalid_argument const& ex) {
					std::cout << "Greska! " << ex.what() << std::endl;
					continue;
				}
				g->printKthInfluential(k);
			default:
				break;
		}

	}
}

