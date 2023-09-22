// AStarSearch.cpp : This file contains the 'main' function. Program execution begins and ends there.

/*

    The A* search is implemented in the findPath() in the Grid class

    In find path priority queue openSet is used to keep track of the nodes to
    visit next. The nodes in openSet are sorted by their fScore values,
    which is the sum of the gScore (the cost of the path from the start
    node to the current node) and the heuristic (an estimate of the cost
    of the cheapest path from the current node to the end node).

    The algorithm works by repeatedly selected the node with lowest fScore
    from openSet and explorin neighbors. For each unvisited neighbor node,
    the algorithm updates its gScore, heuristic, and fScore values, and adds it
    openSet. If neighbor node has already been visited, algo checks whether the
    current path to the node has a lower fScore than the previous path.
    If so, node fScore and parent updated

    Once the algo reaches end node, much like my BSF implementation, it prints
    the path in reverse, to make it readable for human eyes.

    LEGEND
Start node
    S
End Node
    E
Path traversed
    .
Visited Nodes
    *
Wall nodes
    square
Unvisited Nodes
    -

*/

#include <iostream>
#include <ctime>
#include <vector>
#include <string>
#include <sstream>
#include <queue>
#include <cmath>

using namespace std;
struct Node
{
    bool isWall = false;
    bool start = false;
    bool end = false;
    Node* parent;
    vector<Node*> cn; // Connected nodes 
    double cost = 1.0;
    double heuristic = 0.0; // added heuristic value for A* search
    double fScore = 0.0; // added fScore for A* search
    bool visited = false;
    int row = -1;
    int col = -1;
};


// added compare function for priority queue in A* search
class CompareNodes {
public:
    bool operator() (Node* node1, Node* node2) const {
        return node1->fScore > node2->fScore;
    }
};

class Grid
{

private:
    // added heuristic function for A* search
    double calculateHeuristic(Node* current, Node* end)
    {
        double dx = abs(current->col - end->col);
        double dy = abs(current->row - end->row);
        return dx + dy;
    }


public:
    static const int SIZE = 20;
    static vector<vector<Node*>> map;
    //Node* map[SIZE][SIZE];
    Node* start = nullptr;
    Node* end = nullptr;
    vector<Node*> path;
    string mazeFile =
        "x x x x x x x x x x x x x x x x x x x x "
        "x z o o x o x o o o o o o o x o x o o x "
        "x x x o o o x x x o x x x o o o x o o x "
        "x o x o x o o o x o o o x o x o o o x x "
        "x o o o x x x o o o x o o o x x x o o x "
        "x o x x x o x x x o o o x x x o x x x x "
        "x o o x x o x x x x x o o o x o x o o x "
        "x o o x x o x x x x x o o o o o o o o x "
        "x x o x x o o o o o x x o x x o o o o x "
        "x o o o o o x o x o o o o o o o x o x x "
        "x x o o x o x x x x x x o o x o x o x x "
        "x o o o x o x o o o o x o o x o x o o x "
        "x x x o o o x x x o x x x o o o x x x x "
        "x o x o x o o o x o x o x o x o o o x x "
        "x o o o x x x o o o x o x o x x x o o x "
        "x o x x x o x x x o o o x o x o x x x x "
        "x o o o x o x o o o x o o o x o x o o x "
        "x o o o o o o o o x x o o o o o o o o x "
        "x o o x x o o o o x x x o x x o o o y x "
        "x x x x x x x x x x x x x x x x x x x x ";

    void findPath(Grid& grid)
    {
        priority_queue<Node*, vector<Node*>, CompareNodes> openSet; // priority queue for A* search
        openSet.push(grid.start);
        grid.start->visited = true;
        grid.start->parent = nullptr;
        grid.start->fScore = grid.start->heuristic; // initialize start node fScore with its heuristic value

        while (!openSet.empty())
        {
            Node* current = openSet.top();
            openSet.pop();

            if (current == grid.end)
            {
                // Backtrack from end node to start node to construct path
                Node* node = grid.end;
                while (node != nullptr)
                {
                    grid.path.push_back(node);
                    node = node->parent;
                }
                // Print the coordinates in reverse order
                for (auto it = grid.path.rbegin(); it != grid.path.rend(); ++it)
                {
                    cout << "(" << (*it)->row << "," << (*it)->col << ")" << endl;
                }
                return;
            }

            for (Node* neighbor : current->cn)
            {
                if (!neighbor->visited && !neighbor->isWall)
                {
                    neighbor->visited = true;
                    neighbor->parent = current;
                    neighbor->heuristic = calculateHeuristic(neighbor, grid.end); // calculate heuristic for neighbor node
                    neighbor->fScore = current->fScore + neighbor->cost + neighbor->heuristic; // update fScore for neighbor node
                    openSet.push(neighbor);
                }
                else if (current->fScore + neighbor->cost + neighbor->heuristic < neighbor->fScore)
                {
                    neighbor->parent = current;
                    neighbor->fScore = current->fScore + neighbor->cost + neighbor->heuristic; // update fScore for neighbor node
                }
            }
        }
    }


    Grid() {

        stringstream ss(mazeFile, ios::in);
        for (int i = 0; i < SIZE; i++)
        {
            for (int j = 0; j < SIZE; j++)
            {
                char buffer;
                Node* n = new Node();
                ss >> buffer;
                n->row = i;
                n->col = j;
                if (buffer == 'x')
                {
                    n->isWall = true;
                }
                if (buffer == 'z')
                {
                    n->start = true;
                }
                if (buffer == 'y')
                {
                    n->end = true;
                }
                map[i][j] = n;
            }
        }

        // Connect nodes
        for (int i = 0; i < SIZE; i++)
        {
            for (int j = 0; j < SIZE; j++)
            {

                // IF statements will stop trouble when
                // Assigning border nodes
                if ((i - 1) >= 0)
                {
                    map[i][j]->cn.push_back(map[i - 1][j]);
                }
                if ((i + 1) <= SIZE - 1)
                {
                    map[i][j]->cn.push_back(map[i + 1][j]);
                }
                if ((j - 1) >= 0)
                {
                    map[i][j]->cn.push_back(map[i][j - 1]);
                }
                if ((j + 1) <= SIZE - 1)
                {
                    map[i][j]->cn.push_back(map[i][j + 1]);
                }

            }
        }
        // Initialize start node
        for (int i = 0; i < SIZE; i++) {
            for (int j = 0; j < SIZE; j++) {
                if (map[i][j]->start) {
                    start = map[i][j];
                }
            }
        }

        // Initialize end node
        for (int i = 0; i < SIZE; i++) {
            for (int j = 0; j < SIZE; j++) {
                if (map[i][j]->end) {
                    end = map[i][j];
                }
            }
        }


        findPath(*this); // find path from start to end

    }

    void displayEmptyMap(Grid& grid)
    {
        for (int i = 0; i < SIZE; i++)
        {
            for (int j = 0; j < SIZE; j++)
            {
                if (map[i][j]->isWall)
                {
                    cout << char(254) << " ";
                }
                else if (map[i][j]->start)
                {
                    cout << "S" << " ";
                }
                else if (map[i][j]->end)
                {
                    cout << "E" << " ";
                }
                else
                {
                    cout << "  ";
                }
            }
            cout << endl;
        }
    }

    

    void displayMap(Grid& grid)
    {
        // Iterate through the map and print each node
        for (int i = 0; i < SIZE; i++)
        {
            for (int j = 0; j < SIZE; j++)
            {
                Node* n = map[i][j];

                // Print the start node
                if (n->start)
                {
                    cout << "S ";
                }
                // Print the end node
                else if (n->end)
                {
                    cout << "E ";
                }
                // Print the path nodes
                else if (find(grid.path.begin(), grid.path.end(), n) != grid.path.end())
                {
                    cout << ". ";
                }
                // Print the visited nodes
                else if (n->visited)
                {
                    cout << "* ";
                }
                // Print the wall nodes
                else if (n->isWall)
                {
                    cout << char(254) << " ";
                }
                // Print the unvisited nodes
                else
                {
                    cout << "- ";
                }
            }
            cout << endl;
        }
    }

};

void printPath(Grid& grid) {
    if (grid.path.empty()) {
        cout << "No path found." << endl;
        return;
    }
    cout << "Path: ";
    for (int i = 0; i < grid.path.size(); i++) {
        cout << "(" << grid.path[i]->row << ", " << grid.path[i]->col << ")";
        if (i < grid.path.size() - 1) {
            cout << " -> ";
        }
    }
    cout << endl;
}

vector<vector<Node*>> Grid::map(Grid::SIZE, vector<Node*>(Grid::SIZE, nullptr));



int main()
{
    Grid grid;
    
    grid.findPath(grid);
    
    grid.displayEmptyMap(grid);
    cout << endl;
    cout << endl;
    grid.displayMap(grid);
 

    //printPath(grid);
    return 0;
}

