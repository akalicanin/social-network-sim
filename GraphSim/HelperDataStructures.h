#ifndef _HELPER_DS_
#define _HELPER_DS_

// ZA GRAF

struct Vertex;

struct Edge {

	Vertex* from;
	Vertex* to;

	float weight;
	Edge* next;

	Edge(Vertex* from, Vertex* to, float weight) : from(from), to(to), weight(weight), next(nullptr) {}
};

enum vertexColor { WHITE, GRAY, BLACK }; // ZA DFS. Takodje ako ikad treba da se traze back/cross/forward ivice u DFS.

struct Vertex {
	std::string name;
	Edge* adj;
	Edge* adjInverse;

	float d, f;
	bool visited;
	vertexColor color;
	Vertex* prev;

	// Samo za listu cvorova
	Vertex* next;

	Vertex(std::string name) : name(name), adj(nullptr), adjInverse(nullptr), d(-1), f(-1), visited(false), color(WHITE), prev(nullptr), next(nullptr) {};
};

// za iterativni DFS kako bi pravilno racunao cvorove da dodam u listu sortiranu po zavrsnom vremenu
// takodje i za printpath. jednostavno prispajanje zastavice obradjeno/neobradjeno uz neki cvor
struct ProcessingVertex {
	Vertex* v;
	bool processed;

	ProcessingVertex(Vertex* v, bool processed) : v(v), processed(processed) {};

	ProcessingVertex() : v(nullptr), processed(false) {};

};

// OSTATAK. Template structovi

template <typename T>
struct Stack {
	static const int size = 200;
	T stack[size] = {};
	int top = -1;
	T POP() {
		if (this->STACKEMPTY()) {
			return T();
		}
		else {
			return this->stack[this->top--];
		}
	};
	void PUSH(T x) {
		if (STACKFULL()) {
			return;
		}
		this->stack[++this->top] = x;
	};
	bool STACKEMPTY() {
		if (this->top == -1) {
			return true;
		}
		else {
			return false;
		}
	};
	bool STACKFULL() {
		if (top == size - 1) {
			return true;
		}
		else {
			return false;
		}
	};
};

template <typename T>
struct Queue
{
	static const int length = 200;
	T queue[length];
	int head = 0;
	int tail = 0;

	void ENQUEUE(T x) {
		if (this->QUEUEFULL()) {
			return;
		}
		else {
			this->queue[this->tail] = x;
			this->tail = (this->tail + 1) % this->length;
		}
	};

	T DEQUEUE() {
		if (this->QUEUEEMPTY()) {
			return T();
		}
		else {
			T x = this->queue[this->head];
			this->head = (this->head + 1) % this->length;
			return x;
		}
	};

	bool QUEUEEMPTY() {
		if (this->head == this->tail) {
			return true;
		}
		else {
			return false;
		}
	};

	bool QUEUEFULL() {
		if (this->head == (this->tail + 1) % this->length) {
			return true;
		}
		else {
			return false;
		}
	};

};


template <typename T>
struct Node
{
	T info;
	Node* next = nullptr;

	Node() : next(nullptr), info(T()) {};
};

template <typename T>
struct List
{
	Node<T>* head = nullptr;
	int count = 0;

	void addNode(T info)
	{
		Node<T>* n = new Node<T>;
		n->info = info;
		n->next = head;
		head = n;
		count++;
	}

	void removeNodeAtHead()
	{
		if (!head) return;
		Node<T>* temp = head;
		head = head->next;
		delete temp;
		count--;
	}

	~List() 
	{
		while (head)
		{
			removeNodeAtHead();
		}
	}
};


// Heap kao max prio queue za bilo kakvo sortiranje po zavrsnom vremenu itd.
// Koristi Vertex->d kao kljuc po kom sortira (koristim za dijkstra i kth influential)
typedef struct VertexMaxHeap
{
	static const int length = 500;
	Vertex* heap[length];
	int heapsize = 0;

	int Parent(int i) {
		return i / 2;
	}

	int Left(int i) {
		return 2 * i;
	}

	int Right(int i) {
		return 2 * i + 1;
	}

	void MaxHeapify(int i)
	{
		while (true)
		{
			int largest = -9999;
			int l = Left(i);
			int r = Right(i);
			if (l <= heapsize && heap[l]->d > heap[i]->d) {
				largest = l;
			}
			else {
				largest = i;
			}
			if (r <= heapsize && heap[r]->d > heap[largest]->d) {
				largest = r;
			}
			if (largest != i) {
				Vertex* temp = heap[i];
				heap[i] = heap[largest];
				heap[largest] = temp;
				i = largest;
			}
			else {
				break;
			}
		}
	}

	void Insert(Vertex* u) {
		heapsize++;
		heap[heapsize] = u;
		IncreaseKey(heapsize, u->d);
	}

	void IncreaseKey(int i, float key)
	{
		if (heap[i]->d > key)
		{
			return;
		}
		heap[i]->d = key;
		while (i > 1 && heap[Parent(i)]->d < heap[i]->d)
		{
			Vertex* temp = heap[Parent(i)];
			heap[Parent(i)] = heap[i];
			heap[i] = temp;
			i = Parent(i);
		}
	}

	Vertex* ExtractMax()
	{
		if (QUEUEEMPTY()) {
			return nullptr;
		}
		Vertex* u = heap[1];
		heap[1] = heap[heapsize];
		heapsize--;
		MaxHeapify(1);
		return u;
	}

	bool QUEUEEMPTY() {
		return (heapsize < 1);
	}

} vertexMaxPrioQueue;


#endif