#include <vector>
#include <unordered_set>
#include <queue>
#include <fstream>
#include <iostream>
#include <sstream>

using namespace std;
int rows;
int columns;

enum Direction
{
    Diagonal,
    Linear
};
class Node
{
private:
public:
    int row;
    int column;
    int value;
    bool visited = false;
    Node *caller = nullptr;
    Direction direction;
    vector<Node *> adjacents;

    ~Node();
};
// unordered_set<Node> visitedNodes;
queue<Node *> nextUp;

vector<vector<Node *>> maze;
vector<Node *> path;

// Load the maze from input file constructing the nodes as it goes
// Reads in "input.txt file"
void loadMaze()
{
    // Load file
    bool firstLine = true;
    string line;
    char c;
    ifstream FileRead;
    FileRead.open("../input.txt");
    int nodeRow = 0;
    int nodeColumn = 0;
    while (getline(FileRead, line))
    {
        // New line
        stringstream streamReader(line);
        vector<Node *> tempVect;
        int x;
        if (firstLine)
        {
            // Get size of maze
            streamReader >> rows;
            streamReader >> columns;
            // cout << "Rows: " << rows << " Columns: " << columns << endl;
            firstLine = false;

            continue;
        }
        while (streamReader >> x)
        {
            // Make new node
            Node *newNode = new Node;
            newNode->row = nodeRow;
            newNode->column = nodeColumn;
            newNode->value = x;
            if (x < 0)
            {
                newNode->direction = Diagonal;
            }
            else
            {
                newNode->direction = Linear;
            }
            tempVect.push_back(newNode);
            nodeColumn++;
        }
        maze.push_back(tempVect);
        nodeRow++;
        nodeColumn = 0;
    }
    FileRead.close();
    // cout << "Maze built... " << endl;
}

// Find Solution
void mazeBFS(int destinationRow, int destinationColumn)
{
    while (!nextUp.empty())
    {
        // 1) Pop off next node
        Node *currentNode = nextUp.front();
        nextUp.pop();

        // 2) Check if destination
        if (currentNode->row == destinationRow && currentNode->column == destinationColumn)
        {
            // Found
            while (currentNode->row != 0 || currentNode->column != 0)
            {
                path.insert(path.begin(), currentNode);

                currentNode = currentNode->caller;
            }
            path.insert(path.begin(), currentNode);

            // Display route taken
            for (Node *node : path)
            {
                cout << "(" << node->row + 1 << "," << node->column + 1 << ") ";
            }
            cout << endl;
            return;
        }
        else
        {
            // 3) Get its childeren
            for (Node *node : currentNode->adjacents)
            {
                // 4) Check if child node has already been visited
                if (!node->visited)
                {
                    // Set caller of each new node to current node for backtracking later
                    node->caller = currentNode;
                    node->visited = true;

                    // 5) Add each to que
                    nextUp.push(node);
                }
            }
        }
        // 6) Go to 1
    }
    cout << "No path available" << endl;
}
void printMaze()
{
    for (int i = 0; i < maze.size(); i++)
    {
        cout << "{";
        for (int j = 0; j < maze[i].size(); j++)
        {
            cout << maze[i][j]->value << ",";
        }
        cout << "}" << endl;
    }
}

void printAdj()
{
    // Print values
    for (int i = 0; i < maze.size(); i++)
    {
        for (int j = 0; j < maze[i].size(); j++)
        {
            cout << maze[i][j]->value << "  : Adjacent [ ";
            for (Node *node : maze[i][j]->adjacents)
            {
                cout << node->value << ", ";
            }
            cout << "]" << endl;
        }
    }

    cout << "----------------------------------------------" << endl;
    // Print (row, column)
    for (int i = 0; i < maze.size(); i++)
    {
        for (int j = 0; j < maze[i].size(); j++)
        {
            cout << maze[i][j]->row << "," << maze[i][j]->column << "  : Adjacent [ ";
            for (Node *node : maze[i][j]->adjacents)
            {
                cout << "(" << node->row << "," << node->column << "), ";
            }
            cout << "]" << endl;
        }
    }
}
void addAdj()
{
    // Loop through all rows and columns of Node matrix
    for (int i = 0; i < maze.size(); i++)
    {
        for (int j = 0; j < maze[i].size(); j++)
        {
            Node *currentNode = maze[i][j];

            // If linear direction check that all four directions are legal if so add the adjacents list
            if (currentNode->direction == Linear)
            {
                if (i - currentNode->value >= 0)
                {
                    currentNode->adjacents.push_back(maze[i - currentNode->value][j]);
                }
                if (i + currentNode->value < rows)
                {
                    currentNode->adjacents.push_back(maze[i + currentNode->value][j]);
                }
                if (j + currentNode->value < columns)
                {
                    currentNode->adjacents.push_back(maze[i][j + currentNode->value]);
                }
                if (j - currentNode->value >= 0)
                {
                    currentNode->adjacents.push_back(maze[i][j - currentNode->value]);
                }
            }

            // If diagonal direction check that all four directions are legal if so add the adjacents list
            else if (currentNode->direction == Diagonal)
            {
                int value = abs(currentNode->value);
                if ((i - value >= 0) && (j - value >= 0))
                {
                    currentNode->adjacents.push_back(maze[i - value][j - value]);
                }
                if (i + value < rows && j + value < columns)
                {
                    currentNode->adjacents.push_back(maze[i + value][j + value]);
                }
                if (i - value >= 0 && j + value < columns)
                {
                    currentNode->adjacents.push_back(maze[i - value][j + value]);
                }
                if (i + value < rows && j - value >= 0)
                {
                    currentNode->adjacents.push_back(maze[i + value][j - value]);
                }
            }
        }
    }
}

int main()
{
    // Load the maze from input file constructing the nodes as it goes
    loadMaze();
    // printMaze();

    // After matrix is built use it to get the adjacents for the nodes
    addAdj();
    // printAdj();

    // Add start node to list to check
    nextUp.push(maze[0][0]);

    // Find Solution (8,8)
    mazeBFS(rows - 1, columns - 1);
}