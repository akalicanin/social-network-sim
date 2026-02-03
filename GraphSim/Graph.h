#ifndef _GRAPH_H
#define _GRAPH_H

#include <iostream>
#include <fstream>
#include <string>

#include "HelperDataStructures.h"

class Graph
{
private:

	// Bfs za k-tu najuticajniju
	void bfsForInfluence(Vertex* s, int& nodesReached);

	// Regularan dfs
	void dfs();

	// Koristi sopstvena dva izmenjena dfs
	List<List<Vertex*>*>* strongestComponents();
	
	// Izmenjen dijkstra za najvece verovatnoce
	void dijkstra(Vertex* s);

	void printPath(Vertex* u, Vertex* v);

	// ovde koristim vec postojece vertex atribute da ne bih dodavao nove
	// u->d oznacava finalnu verovatnocu, dok v->f oznacava "put" koji bismo racunali u bfs odnosno verovatnoce do tog cvora
	vertexMaxPrioQueue* influencers();

	Vertex* findKthInfluential(int k);

public:

	int numberOfVertices;
	int numberOfEdges;

	Vertex* verticesHead;

	Graph() : numberOfVertices(0), numberOfEdges(0), verticesHead(nullptr)
	{
	};
	~Graph()
	{
		this->deleteGraph();
	};

	bool createGraph(std::string filename);

	bool loadGraphFromFile(std::string fileName);

	void deleteGraph();

	bool addVertex(std::string name);

	bool removeVertexByPointer(Vertex* v);
	
	bool removeVertex(std::string name);

	bool addEdge(Vertex* from, Vertex* to, float weight);

	bool removeEdge(Vertex* from, Vertex* to);

	bool alterEdgeWeight(Vertex* from, Vertex* to, float amount);

	void printGraph();

	void printInverseGraph();

	void printPathFromAtoB(Vertex* a, Vertex* b);

	Vertex* findVertex(std::string name);

	Edge* findEdge(Vertex* from, Vertex* to);

	Edge* findInverseEdge(Vertex* from, Vertex* to);

	void printStrongestComponent();

	void printKthInfluential(int k);

};

#endif
