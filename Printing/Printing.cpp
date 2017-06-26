//Defines the entry point for the console application.
#include <iostream>
#include <math.h>
#include <set>
#include <vector>
#include <queue>
#include <sstream>
#include <cstdlib>
#include <string>
using namespace std;

/*
This function reads a line of int values into a vector function and returns that
vector.
*/
vector<int> readlineofints(int count) {
    // Input values
    vector<int> linevalues(count);
    for (int j = 0; j < count; j++) {
        int val;
        cin >> val;
        linevalues[j] = val;
    }
    return linevalues; // Return line values as a vector
}

//Code taken from http://www.geeksforgeeks.org/dinics-algorithm-maximum-flow/
// A structure to represent a edge between two vertex
struct Edge {
    int target_vertex; // Vertex v (or "to" vertex) of a directed edge u-v. "From" vertex u can be obtained using
                       // index in adjacent array.

    int flow; // flow of data in edge

    int capacity; //Capacity

    int reverse_edge; // To store index of reverse edge in adjacency list so that we can quickly find it.
};

// Residual Graph
class Graph {
    int Vertices; // number of vertex
    int *level; // stores level of a node
    vector< Edge > *adj;
public:
    Graph(int V) {
        adj = new vector<Edge>[V];
        this->Vertices = V;
        level = new int[V];
    }

    // add edge to the graph
    void addEdge(int u, int v, int Capacity) {
        // Forward edge : 0 flow and C capacity
        Edge a{ v, (int)0, Capacity, adj[v].size() };

        // Back edge : 0 flow and 0 capacity
        Edge b{ u, (int)0, (int)0, adj[u].size() };

        adj[u].push_back(a);
        adj[v].push_back(b); // reverse edge
    }

    // add dual-direction edge to the graph
    void addDualEdge(int u, int v, int Capacity) {
        // Forward edge : 0 flow and C capacity
        Edge a{ v, (int)0, Capacity, adj[v].size() };

        // Back edge : 0 flow and 0 capacity
        Edge b{ u, (int)0, Capacity, adj[u].size() };

        adj[u].push_back(a);
        adj[v].push_back(b); // reverse edge
    }

    // delete dual-direction edge to the graph
    void deleteEdge(int u, int v) {
        //Iterate through edges on vertex u
        for (int i = 0; i < adj[u].size(); i++) {
            if (adj[u][i].target_vertex == v) { //If the edge matches vertex v
                adj[u].erase(adj[u].begin() + i); //Delete the edge
            }
        }
        //Iterate through edges on vertex v and delete reverse edge
        for (int i = 0; i < adj[v].size(); i++) {
            if (adj[v][i].target_vertex == u) { //If the edge matches vertex u
                adj[v].erase(adj[v].begin() + i); //Delete the edge
            }
        }
    }

    //reset graph
    void resetFlows() {
        //Iterate through edges on vertex u
        for (int i = 0; i < Vertices; i++) {
            for (int j = 0; j < adj[i].size(); j++) {
                adj[i][j].flow = 0;
            }
        }
    }

    // add forward-only edge to the graph
    void addForwardEdge(int u, int v, int C) {
        // Forward edge : 0 flow and C capacity
        Edge a{ v, (int)0, C, adj[v].size() };

        adj[u].push_back(a);
    }

    // Finds if more flow can be sent from s to t.
    // Also assigns levels to nodes.
    bool BFS(int s, int t) {
        for (int i = 0; i < Vertices; i++)
            level[i] = -1;

        level[s] = 0; // Level of source vertex

                      // Create a queue, enqueue source vertex and mark source vertex as visited here
                      // level[] array works as visited array also.
        deque< int > q;
        q.push_back(s);

        vector<Edge>::iterator i;
        while (!q.empty()) {
            int u = q.front();
            q.pop_front();
            for (i = adj[u].begin(); i != adj[u].end(); i++) {
                Edge &e = *i;
                if (level[e.target_vertex] < 0 && e.flow < e.capacity) {
                    // Level of current vertex is, level of parent + 1
                    level[e.target_vertex] = level[u] + 1;

                    q.push_back(e.target_vertex);
                }
            }
        }

        // IF we can not reach to the sink we return false else true
        return level[t] < 0 ? false : true;
    }

    // A DFS based function to send flow after BFS has figured out that there is a possible flow and
    // constructed levels. This function called multiple times for a single call of BFS.
    // flow : Current flow send by parent function call start[] : To keep track of next edge to be explored.
    //start[i] stores� count of edges explored from i.
    // u : Current vertex
    // t : Sink
    int sendFlow(int u, int flow, int t, int start[]) {
        // Sink reached
        if (u == t)
            return flow;

        // Traverse all adjacent edges one -by - one.
        for (start[u] = (int)0; start[u] < adj[u].size(); start[u]++) {
            // Pick next edge from adjacency list of u
            Edge &e = adj[u][start[u]];

            if (level[e.target_vertex] == level[u] + 1 && e.flow < e.capacity) {
                // find minimum flow from u to t
                int curr_flow = min(flow, e.capacity - e.flow);

                int temp_flow = sendFlow(e.target_vertex, curr_flow, t, start);

                // flow is greater than zero
                if (temp_flow > 0) {
                    // add flow� to current edge
                    e.flow += temp_flow;

                    // subtract flow from reverse edge
                    // of current edge
                    adj[e.target_vertex][e.reverse_edge].flow -= temp_flow;
                    return temp_flow;
                }
            }
        }

        return 0;
    }

    // Returns maximum flow in graph
    int DinicMaxflow(int s, int t) {
        for (int i = 0; i < Vertices; i++) {
            vector<Edge> this_vertex = adj[i];
        }

        // Corner case
        if (s == t)
            return -1;

        int total = 0;  // Initialize result

        // Augment the flow while there is path from source to sink
        while (BFS(s, t) == true) {
            // store how many edges are visited from V { 0 to V }
            int *start = new int[Vertices + 1];

            // while flow is not zero in graph from S to D
            while (int flow = sendFlow(s, (int)32767, t, start))

                // Add path flow to overall flow
                total += flow;
        }

        // return maximum flow
        return total;
    }
};

int main() {
    std::ios_base::sync_with_stdio(false);

    // get test case count
    int t;
    std::cin >> t;

    // loop over all the test cases
    for (int i = 1; i <= t; i++) {
        // Read in params
        vector<int> params = readlineofints(2);
        cin.ignore();

        // Initialize adjacency list graph
        Graph graph(params[0] + params[1] + 2);

        // Iterate over printers
        for (int j = 0; j < params[0]; j++) {
            string response;
            getline(cin, response);
            // Input edge
            vector<string> input;
            stringstream ss(response);
            string substr;
            while (ss.good()) {
                string substr;
                getline(ss, substr, ',');
                input.push_back(substr);
            }

            vector<int> this_edge;
            for (string x : input) {
                int dash_loc = x.find("-");
                if (dash_loc != -1) {
                    int start = stoi(x.substr(0, dash_loc));
                    int end = stoi(x.substr(dash_loc + 1));
                    for (int i = start; i <= end; i++) {
                        this_edge.push_back(i);
                    }
                }
                else {
                    this_edge.push_back(stoi(x));
                }
            }

            for (auto page : this_edge) {
                page += params[0]; //Add m to page and subtract 1 to decrement
                graph.addEdge(j + 1, page, 1); //Connect printer to page
            }
            graph.addEdge(0, j + 1, 1); //Add source to printer link
        }

        //Add page to sink connections
        for (int i = params[0] + 1; i < params[0] + params[1] + 1; i++) {
            graph.addEdge(i, params[0] + params[1] + 1, 1);
        }

        int x = graph.DinicMaxflow(0, params[0] + params[1] + 1);

        //Print result, incrementing to account for indexing offset at input
        if (x == params[1]) {
            std::cout << "Case #" << i << ": yes" << std::endl;
        }
        else {
            std::cout << "Case #" << i << ": no" << std::endl;
        }
    }
    return 0;
}