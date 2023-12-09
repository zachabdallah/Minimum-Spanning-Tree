#include <iostream>
using namespace std;
#include <fstream>
#include <string>
#include <vector>
#include "Node.h"
#define x 15

class Edge{
public:
    int v1;
    int v2;
    double weight;
    Edge(int v1, int v2, double weight){
        this->v1 = v1;
        this->v2 = v2;
        this->weight = weight;
    }
    void printEdge(){
        cout << v1 << "---" << v2 << ": " << weight << endl;
    }
};
//here I have an edge class that creates a relationship with 2 vertices and explains their weight
class Graph{
public:
    int V; //vertice amount
    vector<Edge> edges; //a vector of edge relations to two different vertices
    Graph(int V){
        this->V = 2 * V;
    } //this is an undirected graph so im storing every edge twice
    void addEdge(int v1, int v2, double weight){
        Edge e(v1, v2, weight);
        edges.push_back(e);
    }
    void printGraph(){
        for (int i = 0; i < this->V; ++i){
            edges[i].printEdge();
        }
    }
};

class UnionFind{
public:
    vector<int> parent; //a set for maintaining parent value
    vector<int> rank;   //a set for maintaining node ranks
    //I need to treat all nodes in the graph as trees themselves
    UnionFind(int n){
        //this essentially resizes the vectors to a specified input number which should be 30
        parent.resize(n);
        for (int i = 0; i < n; ++i){
            parent[i] = -1;
        }
    }
    int find(int v){
        if (parent[v] == -1){
            return v; //its parent is itself
        }
        parent[v] = find(parent[v]);//otherwise, recurse until you find the parent
        int vertex_root = parent[v]; //this line is just for clarity
        return vertex_root;
    }
    void merge(int v1, int v2){
        v1 = find(v1); //root of v1
        v2 = find(v2); //root of v2
        if (v1 == v2){
            return; //if the roots of the 2 sets are equal, that means they're part of the same set (tree). You can't merge them because that will cause a cycle. Therefore, the edge that ran through this function will not be added to the mst. Basically, UF is an algorithm that joins disjoint sets. In our case of finding an MST, we can't join two things that are part of the same set because we know it would cause a cycle. The find function finds the parent, but that also tells us which set that is in because we're treating the parent like it's the root of a tree.
                    //If they are not part of the same set then we create a new set which is the union of the two : (a U b)
        }
        parent[v2] = v1; //since we merged them, now they need to share parents
    }
};//some versions of this class also hold a private vector<int> member called rank. Basically, rank[i] increments by 1 every time a new set is unified with the i'th vertex as the new root of the unified set. Its purpose is such that when we unify sets in the future, we try to unify upon a lower ranking root so that the runtime complexity of the algorithm is better. This is the case because sets are treated like trees, and a higher rank sort of implies a bigger height of the tree; in this way we can reference the root node faster in a more balanced tree. However, I did not implement this because I do not really care about runtime right now and it was also confusing to implement.

bool compare(const Edge &e1, const Edge &e2){
    bool cmp = e1.weight < e2.weight;
    return cmp;
}
void kruskalMST(Graph &graph){
    vector<Edge> result;
    sort(graph.edges.begin(), graph.edges.end(), compare); //this is an STL sort that orders the edges based on their weight from highest to lowest //third optional parameter of telling how to sort, we can't directly do arithmetic comparisons so we have to specify how to compare
    //graph.printGraph();
    //kruskals sorts edges from smallest to highest weight
    UnionFind uf(graph.V); //create a unionfind class with two vectors size V
    for (int i = 0; i < graph.edges.size(); ++i){//size of vector as a public variable in g holding all the edges
        Edge e = graph.edges[i];//one edge at a time
        if (uf.find(e.v1) != uf.find(e.v2)){ //if the parents are not the same, merge them
            uf.merge(e.v1, e.v2); //this if statement is basically the algorithm
            result.push_back(e);
        }
    }
    cout << "Edges in the MST: " << endl;
    for (int i = 0; i < result.size(); ++i){
        cout << result[i].v1 << " ------ " << result[i].v2 << " : weight: " << result[i].weight << endl;
    }
}

int main(){
    ifstream myFile;
    myFile.open("/Users/zachabdallah/Downloads/UAA/Spring 23/A311/homework/Homework 4/Vertices.txt");
    int verticeAmount;
    string whitespace_spam = " ";
    if (myFile.is_open()){
        myFile >> verticeAmount; //take the first input of the file
        cout << "Amount of locations: " << verticeAmount << endl << endl;
        myFile >> whitespace_spam;
    }
    else{
        cout << "file not found" << endl;
        return 0;
    }
    Graph g(verticeAmount);
    Node graph[verticeAmount];
    while (myFile.good()){
        int location_number = 0;
        string location_name = "not set in main";
        int cardinality = 0;
        //declare
        
        getline(myFile, location_name);
        string nextNodeInfo = "no info";
        for (int i = 0; i < verticeAmount; ++i){
            //part 1 of loop -- (number, name, cardinality)
            myFile >> location_number;
            myFile.get(); //the line below was reading in a newline character so we're getting rid of it here
            getline(myFile, location_name);
            myFile >> cardinality;
            
            //part 2 of loop -- (neighbors and edge weights)
            int nextNode;
            double weight;
            int neighborArray[cardinality];
            double neighborWeights[cardinality];
            for (int i = 0; i < cardinality; ++i){
                //basically, we want to continue to add to information to the node based on how many other nodes it's connected to
                //Then, we create a node class with all of this information passed to it by 2 different arrays
                myFile >> nextNode;
                myFile >> weight;
                neighborArray[i] = nextNode;
                neighborWeights[i] = weight;
            }
            
            //part 3 of the loop -- create the node and put it in our graph array
            Node location(location_number, location_name, cardinality, neighborArray, neighborWeights); //create a node
            graph[i] = location; //put it in the array
            //graph[i].print();
            myFile >> whitespace_spam;
        }
    } //create node array
    for (int i = 0; i < verticeAmount; ++i){
        for (int y = 0; y < graph[i].getCardinality(); ++y){
            g.addEdge(i+1, graph[i].getNeighbor(y), graph[i].getWeight(y));
            //cout << i + 1 << " " << graph[i].getNeighbor(y) << " " << graph[i].getWeight(y) << endl;
        }
    }//create actual graph
    //this algorithm is basically centered around checking if their is a cycle. That's the only hard part, because obviously it would take two seconds to pick the lowest edge and check if we have (v-1) edges.
    //checking if there is a cycle is basically done by using the unionFind algorithm, which was implemented here in a class. The UF algorithm basically separates vertices into clusters, which allows you to determine if two vertices belong to the same cluster and if adding an edge will produce a cycle.
    //Bascially, we don't want to insert an edging connecting two nodes if they have the same parent
    kruskalMST(g);
    return 0;
}
