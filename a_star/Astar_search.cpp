#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
using std::cout;
using std::ifstream;
using std::istringstream;
using std::string;
using std::vector;
using std::abs;

enum class State {kEmpty, kObstacle, kClosed};

// directional deltas
const int delta[4][2]{{-1, 0}, {0, -1}, {1, 0}, {0, 1}};

vector<State> ParseLine(string line) {
    istringstream sline(line);
    int n;
    char c;
    vector<State> row;
    while (sline >> n >> c && c == ',') {
      if (n == 0) {
        row.push_back(State::kEmpty);
      } else {
        row.push_back(State::kObstacle);
      }
    }
    return row;
}


vector<vector<State>> ReadBoardFile(string path) {
  ifstream myfile (path);
  vector<vector<State>> board{};
  if (myfile) {
    string line;
    while (getline(myfile, line)) {
      vector<State> row = ParseLine(line);
      board.push_back(row);
    }
  }
  return board;
}

//expands neighbours looking for navigable spots and adds it openlist
void ExpandNeighbors(const vector<int>& currNode, const int goal[2], vector<vector<int>> &open, const vector<vector<State>> &grid) {
  // TODO: Get current node's data.
        int x = currNode[0];
        int y = currNode[1];
        int g = currNode[2];
        int x2,y2,g2,h2;

        //Loop through current node's potential neighbors.
        for(int i=0;i<4;i++){
            x2 = x + delta[i][0];
            y2 = y + delta[i][1];

            if(CheckValidCell(x2,y2,grid)){
                g2 = g + 1;
                h2 = Heuristic(x2,y2,goal[0],goal[1]);
                AddToOpen(x2,y2,g2,h2,open,grid);
            }
        }
}

// Calculate the manhattan distance
int Heuristic(int x1, int y1, int x2, int y2) {
  return abs(x2 - x1) + abs(y2 - y1);
}

//compare two f-values of a node
bool Compare(vector<int> node1, vector<int> node2)
{
    bool isgreater = false;

    //cost function of two nodes
    int f1= node1[2] + node1[3];
    int f2 = node2[2] + node2[3];

    if(f1 > f2)
      isgreater = true;

    return isgreater;
}

void AddToOpen(int x,int y,int g,int h, vector<vector<int>> &openNodesList, vector<vector<State>> &grid)
{
    vector<int> openNode{x,y,g,h};
    openNodesList.push_back(openNode);
    grid[x][y] = State::kClosed;
}

//checks if cell doesnot present obstacles or is not visited one
bool CheckValidCell(int x,int y,vector<vector<State>> &grid){

    bool isValidcell;
        if(x < grid.size() && y < grid[0].size()){
        if(grid.at(x)[y] == State::kObstacle || grid.at(x)[y] == State::kClosed)
          isValidcell = false;
        else
          isValidcell = true;
    }
    else
      isValidcell = false;

    return isValidcell;
}

//sort 2d vectors according in descending order
void CellSort(vector<vector<int>> *v) {
  sort(v->begin(), v->end(), Compare);
}

/**
 * Implementation of A* search algorithm
 */
vector<vector<State>> Search(vector<vector<State>> grid, int init[2], int goal[2])
{
  //openNode list
  vector<vector<int>> open{};

  //Initialize the starting node.
  int g = 0;
  int x = init[0];
  int y = init[1];
  int h = Heuristic(x,y,goal[0],goal[1]);

  //add the starting node to the open vector.
  AddToOpen(x,y,g,h,open,grid);

  while(open.size() != 0){
    //Sort the open list using CellSort, and get the current node.
    CellSort(&open);

    //last one has min f value
    auto current = open.back();
    x = current[0];
    y = current[1];
    open.pop_back();

    //adding to a path towards goal
    grid[x][y] = State::kPath;

    //Check if you've reached the goal. If so, return grid.
    if(x == goal[0] && y == goal[1])
      return grid;

    // If we're not done, expand search to current node's neighbors. This step will be completed in a later quiz.
    // ExpandNeighbors

  }

  cout << "No path found!" << "\n";
  return open;
}


string CellString(State cell) {
  switch(cell) {
    case State::kObstacle: return "⛰️   ";
    default: return "0   ";
  }
}


void PrintBoard(const vector<vector<State>> board) {
  for (int i = 0; i < board.size(); i++) {
    for (int j = 0; j < board[i].size(); j++) {
      cout << CellString(board[i][j]);
    }
    cout << "\n";
  }
}

int main() {
  int init[2]{0, 0};
  int goal[2]{4, 5};
  auto board = ReadBoardFile("1.board");
  auto solution = Search(board, init, goal);
  PrintBoard(solution);
}
