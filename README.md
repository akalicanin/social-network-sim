# Social network simulator
  
This is a graph-based, adjacency-list social network simulator.  
Nodes represent users, edges represent relationships (followings, connections).

  
## How it works

Node names can be at most 20 characters long.  
  
Edges are represented by two nodes and an edge weight.  
The edge weight determines how strong the connection between two users is. It is a number in the range [0, 1].  
In a real social network, it would represent the likelihood of someone's posts appearing on your page.  

  - ```Node1 -> Node2-0.80```
  
Means that Node1 follows Node2, and that connection is 0.8 strong. This means that there is an 80% likelihood of Node2's posts appearing on Node1's curated page. 
  
Edge weights are multiplicative. In a scenario with three nodes like this:

  - ```Node1 -> Node2 -> Node3```

Which, in this model, would be represented as:

  - ```Node1 -> Node2-0.50```  
  - ```Node2 -> Node3-0.50```  

(Node1 follows Node2 with 0.5 strength, Node2 follows Node3 with 0.5 strength)  

The likelihood of Node3's post appearing on Node1's page is 0.5*0.5=0.25, so 25%.

## Graph text file structure  
  
There are two example files provided with nodes:
  - network15.txt
  - network30.txt
  
They are networks of size 15 and 30.  
The format for writing your own is as follows:  

- First line: a number that is the amount of nodes (users) in the graph.
- Second line: a number that is the amount of edges (relationships) in the graph.
- Third line: names of all nodes seperated by a blank character
- Remaining X lines: (where X is amount of edges) define the relationships, with the format:
- ``` [First node name] [Second node name] [Edge weight] ``` 
