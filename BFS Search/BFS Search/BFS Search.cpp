// BFS Search.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

// AI assignment 2.cpp : This file contains the 'main' function. Program execution begins and ends there.

#include <iostream>

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
    bool visited = false;
    int row = -1;
    int col = -1;
};


class Grid
{
public:

    void findPath(Grid& grid)
    {
        queue<Node*> q;
        q.push(grid.start);
        grid.start->visited = true;
        grid.start->parent = nullptr;

        while (!q.empty())
        {
            Node* current = q.front();
            q.pop();

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
                    q.push(neighbor);
                }
            }
        }
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

/*
BFS algorithm starts from the "start" node and uses a queue to explore neighbor nodes.

The algo. explores nodes and marks them as visited when it is added to the queue

It repeats this exploring each node in queue until end node reached, or queue empty.

When "end" node found, it back tracks from start to end to construct the path

I used a "Node" struct to represent each cell in the grid.

In the grid class, findPath() implements my BSF
algo, and a vector to store the path from start to end nodes.

I used 'z' on my string file to mark my start node
Then used 'y' to mark end nodes

I also connected neighboring nodes to eachother in the grid, this allows
my BFS to exploe all reachalbe nodes.

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
