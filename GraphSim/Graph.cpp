#include "Graph.h"

// privatne metode

void Graph::bfsForInfluence(Vertex* s, int& nodesReached)
{
	// Posto zelim da koristim moj maxprioqueue opet koji koristim za dijkstru
	// jer je lakse od pravljenje sortirane liste, a on sortira po u->d,
	// neka ovde u->f bude "daljina" a u->d bude konacna verovatnoca
	for (Vertex* v = this->verticesHead; v != nullptr; v = v->next)
	{
		v->f = 0.0f;
		v->visited = false;
		v->prev = nullptr;
	}
	s->f = 1.0f;
	nodesReached = 0;
	Queue<Vertex*> q;
	q.ENQUEUE(s);
	while (!q.QUEUEEMPTY())
	{
		Vertex* u = q.DEQUEUE();
		for (Edge* e = u->adjInverse; e != nullptr; e = e->next)
		{
			if (e->to == u) {
				continue;
			}

			if (!e->to->visited)
			{
				e->to->f = u->f * e->weight;
				e->to->prev = u;
				q.ENQUEUE(e->to);
				nodesReached++;
			}

		}
		u->visited = true;
	}
};

void Graph::dfs()
{
	//koristim float iako moze samo int da ne bih gledao upozorenje compilera
	float time = 0.0f;
	for (Vertex* v = this->verticesHead; v != nullptr; v = v->next)
	{
		v->d = 0.0f;
		v->f = 0.0f;
		v->color = WHITE;
		v->prev = nullptr;
		v->visited = false;
	}
	for (Vertex* v = this->verticesHead; v != nullptr; v = v->next)
	{
		if (v->color == WHITE) {
			Stack<Vertex*> s;
			s.PUSH(v);
			while (!s.STACKEMPTY()) {
				Vertex* u = s.POP();
				if (!u->visited) {
					time++;
					u->d = time;
					u->color = GRAY;
					u->visited = true;
					s.PUSH(u);
					if (!u->adj) continue;
					for (Edge* i = u->adj; i != nullptr; i = i->next) {
						if (i->to->color == WHITE) {
							i->to->prev = u;
							s.PUSH(i->to);
						}
					}
				}
				else {
					time++;
					u->f = time;
					u->color = BLACK;
				}
			}
		}
	}
}

List<List<Vertex*>*>* Graph::strongestComponents()
{
	List<List<Vertex*>*>* listOfStrongestComponents = new List<List<Vertex*>*>;

	// PRVI DFS
	List<Vertex*>* verticesByFinishTime = new List<Vertex*>;
	float time = 0.0f;
	for (Vertex* v = this->verticesHead; v != nullptr; v = v->next)
	{
		v->d = 0.0f;
		v->f = 0.0f;
		v->color = WHITE;
		v->prev = nullptr;
		v->visited = false;
	}
	for (Vertex* v = this->verticesHead; v != nullptr; v = v->next)
	{
		if (v->color == WHITE) {
			Stack<ProcessingVertex> s;
			ProcessingVertex currVert = ProcessingVertex(v, false);
			s.PUSH(currVert);
			while (!s.STACKEMPTY()) {
				ProcessingVertex dfsU = s.POP();
				Vertex* u = dfsU.v;
				bool uWasProcessed = dfsU.processed;
				if (uWasProcessed) {
					time++;
					u->f = time;
					u->color = BLACK;
					verticesByFinishTime->addNode(u);
					continue;
				}
				if (!u->visited) {
					time++;
					u->d = time;
					u->color = GRAY;
					u->visited = true;
					currVert = ProcessingVertex(u, true);
					s.PUSH(currVert);
					if (!u->adj) continue;
					for (Edge* i = u->adj; i != nullptr; i = i->next) {
						if (i->to->color == WHITE) {
							i->to->prev = v;
							ProcessingVertex edgeVert = ProcessingVertex(i->to, false);
							s.PUSH(edgeVert);
						}
					}
				}
			}
		}
	}

	// DRUGI DFS NA TRANSPONOVANOM GRAFU
	for (Vertex* v = this->verticesHead; v != nullptr; v = v->next)
	{
		v->color = WHITE;
		v->prev = nullptr;
		v->visited = false;
	}
	for (Node<Vertex*>* vNode = verticesByFinishTime->head; vNode != nullptr; vNode = vNode->next)
	{
		if (vNode->info->color == WHITE) {
			List<Vertex*>* component = new List<Vertex*>;
			Stack<ProcessingVertex> s;
			ProcessingVertex currVert = ProcessingVertex(vNode->info, false);
			s.PUSH(currVert);
			while (!s.STACKEMPTY()) {
				ProcessingVertex dfsU = s.POP();
				Vertex* u = dfsU.v;
				bool uWasProcessed = dfsU.processed;
				if (uWasProcessed) {
					u->color = BLACK;
					component->addNode(u);
					continue;
				}
				if (!u->visited) {
					u->color = GRAY;
					u->visited = true;
					currVert = ProcessingVertex(u, true);
					s.PUSH(currVert);
					if (!u->adjInverse) continue;
					for (Edge* i = u->adjInverse; i != nullptr; i = i->next) {
						if (i->to->color == WHITE) {
							i->to->prev = u;
							ProcessingVertex edgeVert = ProcessingVertex(i->to, false);
							s.PUSH(edgeVert);
						}
					}
				}
			}
			listOfStrongestComponents->addNode(component);
		}
	}
	return listOfStrongestComponents;
};

void Graph::dijkstra(Vertex* s)
{
	vertexMaxPrioQueue pq;
	for (Vertex* u = this->verticesHead; u != nullptr; u = u->next)
	{
		u->d = -1.0f;
		if (u == s) {
			u->d = 1.0f;
		}
		u->prev = nullptr;
		pq.Insert(u);
	}
	while (!pq.QUEUEEMPTY())
	{
		Vertex* u = pq.ExtractMax();
		for (Edge* v = u->adj; v != nullptr; v = v->next)
		{
			if (v->to->d < u->d * v->weight)
			{
				v->to->d = u->d * v->weight;
				if (v->to->d > 1) {
					v->to->d = 1;
				}
				if (v->to->d >= 0) {
					v->to->prev = u;
				}
			}
		}
	}
};

void Graph::printPath(Vertex* u, Vertex* v)
{
	Stack<ProcessingVertex> s;
	ProcessingVertex currVert = ProcessingVertex(v, false);
	s.PUSH(currVert);

	while (!s.STACKEMPTY()) {
		ProcessingVertex procX = s.POP();
		Vertex* x = procX.v;
		bool wasXProcessed = procX.processed;
		if (u == x || wasXProcessed) {
			if (x == u) {
				std::cout << x->name;
			}
			else {
				std::cout << x->name << " - " << x->d;
			}
			if (x != v)
			{
				std::cout << " -> ";
			};
		}
		else {
			if (!x->prev && x != u) {
				std::cout << "Error! No path from node " << u->name << " to node " << v->name << '\n';
				return;
			}
			else {
				s.PUSH({ x, true });
				if (x->prev) {
					currVert = ProcessingVertex(x->prev, false);
					s.PUSH(currVert);
				}
			}
		}
	}
	std::cout << '\n';
}

// ovde koristim vec postojece vertex atribute da ne bih dodavao nove
// u->d oznacava finalnu verovatnocu, dok v->f oznacava "put" koji bismo racunali u bfs odnosno verovatnoce do tog cvora
vertexMaxPrioQueue* Graph::influencers()
{

	int nodesReached = 0;
	vertexMaxPrioQueue* influencersList = new vertexMaxPrioQueue;

	for (Vertex* u = this->verticesHead; u != nullptr; u = u->next)
	{
		u->d = 0.0f;
		bfsForInfluence(u, nodesReached);
		for (Vertex* v = this->verticesHead; v != nullptr; v = v->next)
		{
			if (v != u)
			{
				u->d += v->f;
			}
		}
		u->d = u->d / (this->numberOfVertices - 1) * nodesReached;
		influencersList->Insert(u);
	}

	return influencersList;
};

Vertex* Graph::findKthInfluential(int k)
{
	vertexMaxPrioQueue* influencersList = influencers();
	if (k > influencersList->heapsize) {
		std::cout << "Error! K is larger than amount of nodes in graph!\n";
		return nullptr;
	}
	Vertex* kthInfluential = nullptr;
	while (k > 0)
	{
		kthInfluential = influencersList->ExtractMax();
		k--;
	}
	return kthInfluential;
};

// javne metode

bool Graph::createGraph(std::string filename)
{
	if (loadGraphFromFile(filename)) {
		return true;
	}
	else {
		std::cout << "Error! Failed to load graph from file!" << std::endl;
		return false;
	}
};

bool Graph::loadGraphFromFile(std::string fileName)
{
	std::ifstream dat;
	dat.open(fileName);
	if (!dat.is_open()) {
		std::cout << "Error opening file: " << fileName << "!\n";
		return false;
	}
	std::string line;
	int vertNum, edgeNum = 0;
	dat >> line; // ucitavanje broja cvorova
	try {
		vertNum = std::stoi(line);
		dat >> line;
		edgeNum = std::stoi(line);
	}
	catch (std::invalid_argument const& ex) {
		std::cout << "Error! " << ex.what() << std::endl;
		dat.close();
		return false;
	}
	for (int i = 0; i < vertNum; i++) {
		std::string name;
		dat >> name;
		if (!(this->addVertex(name)))
		{
			dat.close();
			return false;
		};
	}
	for (int i = 0; i < edgeNum; i++) {
		std::string name1, name2;
		float weight;
		dat >> name1;
		dat >> name2;
		dat >> weight; // OVDE DIREKTNO IZ FSTREAMA U FLOAT?
		Vertex* from = this->findVertex(name1);
		Vertex* to = this->findVertex(name2);
		if (!from) {
			std::cout << "Error! Node not found: " << name1 << '\n';
			dat.close();
			return false;
		}
		if (!to) {
			std::cout << "Error! Node not found: " << name2 << '\n';
			dat.close();
			return false;
		}
		this->addEdge(from, to, weight);
	}
	dat.close();
	return true;

};

void Graph::deleteGraph()
{
	while (this->verticesHead)
	{
		this->removeVertexByPointer(this->verticesHead);
	}
};

bool Graph::addVertex(std::string name)
{
	if (name.size() > 20) { std::cout << "Error! Name for node too long. Max 20 chars.\n"; return false; }
	if (this->findVertex(name) != nullptr) {
		std::cout << "Error! Node already exists: " << name << '\n';
		return false;
	}
	Vertex* v = new Vertex(name);
	if (!this->verticesHead) {
		this->verticesHead = v;
	}
	else {
		v->next = this->verticesHead;
		this->verticesHead = v;
	}
	this->numberOfVertices++;
	return true;
};

bool Graph::removeVertexByPointer(Vertex* v)
{
	if (!v) {
		//std::cout << "Greska! Cvor sa imenom " << name << " nije pronadjen za brisanje!\n";
		return false;
	}
	while (v->adj) {
		this->removeEdge(v, v->adj->to);
	}

	if (this->verticesHead == v) {
		this->verticesHead = v->next;
	}

	for (Vertex* curr = this->verticesHead; curr != nullptr; curr = curr->next) {
		if (curr->next == v) {
			// Dok smo ovde, izlancamo ga iz liste vertexa.
			curr->next = v->next;
		}
		removeEdge(curr, v);
	}

	this->numberOfVertices--;
	delete v;
	return true;
};

bool Graph::removeVertex(std::string name)
{
	Vertex* v = findVertex(name);
	if (!v) {
		//std::cout << "Greska! Cvor sa imenom " << name << " nije pronadjen za brisanje!\n";
		return false;
	}
	while (v->adj) {
		this->removeEdge(v, v->adj->to);
		//zbog inverznih listi, ovo poteze brisanje (this->removeEdge(v->adj->to, v).
	}

	if (this->verticesHead == v) {
		this->verticesHead = v->next;
	}

	for (Vertex* curr = this->verticesHead; curr != nullptr; curr = curr->next) {
		if (curr->next == v) {
			// Dok smo ovde, izlancamo ga iz liste vertexa.
			curr->next = v->next;
		}
		removeEdge(curr, v);
	}

	this->numberOfVertices--;
	delete v;
	return true;
};

bool Graph::addEdge(Vertex* from, Vertex* to, float weight)
{
	if (this->findEdge(from, to) != nullptr) {
		//std::cout << "Greska! Vec postoji ivica od cvora " << from->name << " do cvora " << to->name << '!\n';
		return false;
	}
	Edge* e = new Edge(from, to, weight);
	e->next = from->adj;
	from->adj = e;
	Edge* eInverse = new Edge(to, from, weight);
	eInverse->next = to->adjInverse;
	to->adjInverse = eInverse;
	this->numberOfEdges++;
	return true;
};

bool Graph::removeEdge(Vertex* from, Vertex* to)
{
	Edge* prev = nullptr;
	Edge* curr = from->adj;
	if (curr == nullptr) {
		//std::cout << "Dati cvor " << from->name << " nema susedne cvoreve." << std::endl;
		return false;
	}

	while (curr != nullptr) {
		if (curr->to == to) {
			this->numberOfEdges--;
			if (curr == from->adj) {
				from->adj = from->adj->next;
				delete curr;
			}
			else {
				prev->next = curr->next;
				delete curr;
			}
			break;
		}
		prev = curr;
		curr = curr->next;
		if (!curr) {
			//std::cout << "Greska! Ne postoji ivica za brisanje od cvora " << from->name << " do cvora " << to->name << '!\n';
			return false;
		}
	}

	//sad je pocetni TO, a krajni je FROM.
	//curr = pocetni->adjInverse (to->adjInverse)
	//odnosno, inverzna lista ivica krece od cvora V ako je ivica bila U -> V.
	// sad idemo kroz inverznu adjacency listu cvora V (gde postoji granica V->U).

	prev = nullptr;
	curr = to->adjInverse;

	while (curr != nullptr) {
		if (curr->to == from) {
			if (curr == to->adjInverse) {
				to->adjInverse = to->adjInverse->next;
				delete curr;
			}
			else {
				prev->next = curr->next;
				delete curr;
			}
			return true;
		}
		prev = curr;
		curr = curr->next;
	}
	return false;
};

bool Graph::alterEdgeWeight(Vertex* from, Vertex* to, float amount) {
	Edge* e = findEdge(from, to);
	if (!e) {
		return false;
	}
	Edge* eInverse = findInverseEdge(to, from);
	e->weight += amount;
	if (e->weight > 1.0f) {
		e->weight = 1.0f;
	}
	eInverse->weight = e->weight;
	return true;
}

void Graph::printGraph()
{
	for (Vertex* curr = this->verticesHead; curr != nullptr; curr = curr->next) {
		std::cout << curr->name << " -> ";
		for (Edge* currEdge = curr->adj; currEdge != nullptr; currEdge = currEdge->next) {
			std::cout << currEdge->to->name << '-' << currEdge->weight;
			if (currEdge->next) {
				std::cout << " -> ";
			}
		}
		std::cout << '\n';
	}
};

void Graph::printInverseGraph()
{
	for (Vertex* curr = this->verticesHead; curr != nullptr; curr = curr->next) {
		std::cout << curr->name << " -> ";
		for (Edge* currEdge = curr->adjInverse; currEdge != nullptr; currEdge = currEdge->next) {
			std::cout << currEdge->to->name << '-' << currEdge->weight;
			if (currEdge->next) {
				std::cout << " -> ";
			}
		}
		std::cout << '\n';
	}
};

void Graph::printPathFromAtoB(Vertex* a, Vertex* b)
{
	this->dijkstra(a);
	this->printPath(a, b);
};

Vertex* Graph::findVertex(std::string name) {
	for (Vertex* curr = this->verticesHead; curr != nullptr; curr = curr->next) {
		if (!strcmp(name.c_str(), curr->name.c_str())) {
			return curr;
		}
	}
	return nullptr;
}

Edge* Graph::findEdge(Vertex* from, Vertex* to) {
	for (Edge* curr = from->adj; curr != nullptr; curr = curr->next) {
		if (curr->to == to) {
			return curr;
		}
	}
	return nullptr;
}

Edge* Graph::findInverseEdge(Vertex* from, Vertex* to) {
	for (Edge* curr = from->adjInverse; curr != nullptr; curr = curr->next) {
		if (curr->to == to) {
			return curr;
		}
	}
	return nullptr;
}

void Graph::printStrongestComponent()
{
	List<List<Vertex*>*>* components = this->strongestComponents();
	Node<List<Vertex*>*>* largestComponent = components->head;
	if (!largestComponent) return;
	for (Node<List<Vertex*>*>* it = components->head; it != nullptr; it = it->next) {
		if (it->info->count > largestComponent->info->count) {
			largestComponent = it;
		}
	}
	for (Node<Vertex*>* vNode = largestComponent->info->head; vNode != nullptr; vNode = vNode->next)
	{
		std::cout << vNode->info->name << " ";
	}
	std::cout << '\n';

}

void Graph::printKthInfluential(int k)
{
	Vertex* kthInfluential = findKthInfluential(k);
	if (!kthInfluential) return;
	std::cout << k << "th most influential node: " << kthInfluential->name << " with influence " << kthInfluential->d << '\n';
}
