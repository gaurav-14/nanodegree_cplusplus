#include <algorithm>
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

enum class State {kEmpty, kObstacle, kClosed, kPath, kStart, kFinish};

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

//checks if cell doesnot present obstacles or is not visited one
bool CheckValidCell(int x,int y,vector<vector<State>> &grid){
   bool isValidcell;
   if(x >= 0 && x < grid.size() && y >= 0 && y < grid[0].size()){
        if(grid[x][y] == State::kEmpty)
            isValidcell = true;
        else
            isValidcell = false;
    }
    else
      isValidcell = false;

    return isValidcell;
}

// Calculate the manhattan distance
int Heuristic(int x1, int y1, int x2, int y2) {
  return abs(x2 - x1) + abs(y2 - y1);
}

//compare two f-values of a node
bool Compare(vector<int> node1, vector<int> node2)
{
    //cost function of two nodes
    int f1= node1[2] + node1[3];
    int f2 = node2[2] + node2[3];

    if(f1 > f2)
        return true;
    else
        return false;
}

void AddToOpen(int x,int y,int g,int h, vector<vector<int>> &openNodesList, vector<vector<State>> &grid)
{
    vector<int> openNode{x,y,g,h};
    openNodesList.push_back(openNode);
    grid[x][y] = State::kClosed;
}

//sort 2d vectors according in descending order
void CellSort(vector<vector<int>> *v) {
  sort(v->begin(), v->end(), Compare);
}


//expands neighbours looking for navigable spots and adds it openlist
void ExpandNeighbors(const vector<int>& currNode,int goal[2], vector<vector<int>> &open, vector<vector<State>> &grid) {
        //Get current node's data.
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

  while(open.size() > 0){
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
    if(x == goal[0] && y == goal[1]){
        grid[init[0]][init[1]] = State::kStart;
        grid[goal[0]][goal[1]] = State::kFinish;
        return grid;
    }

    ExpandNeighbors(current,goal,open,grid);

  }

  cout << "No path found!" << "\n";
  return vector<vector<State>>{};
}


string CellString(State cell) {
    switch(cell) {
      case State::kObstacle: return "⛰️   ";
      case State::kPath: return "🚗   ";
      case State::kStart: return "🚦   ";
      case State::kFinish: return "🏁  ";

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
  auto board = ReadBoardFile("../1.board");
  auto solution = Search(board, init, goal);

  PrintBoard(solution);
}
