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

/**
 * Implementation of A* search algorithm
 */
vector<vector<State>> Search(vector<vector<State>> grid, int init[2], int goal[2])
{
  //openNode list
  vector<vector<int>> open{};

  //Initialize the starting node.
  int g = 0;
  int h = Heuristic(init[0],init[1],goal[0],goal[1]);

  //add the starting node to the open vector.
  AddToOpen(init[0],init[1],g,h,open,grid);

  cout << "No path found!" << "\n";
  return std::vector<vector<State>> {};
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
