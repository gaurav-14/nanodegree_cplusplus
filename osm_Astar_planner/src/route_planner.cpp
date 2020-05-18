#include "route_planner.h"
#include <algorithm>

RoutePlanner::RoutePlanner(RouteModel &model, float start_x, float start_y, float end_x, float end_y): m_Model(model) {
    // Convert inputs to percentage:
    start_x *= 0.01;
    start_y *= 0.01;
    end_x *= 0.01;
    end_y *= 0.01;

    //find the closest nodes to the starting and ending coordinates.
    start_node = &m_Model.FindClosestNode(start_x,start_y);
    end_node = &m_Model.FindClosestNode(end_x,end_y);
}

//calculate hvalues based on distance function between two nodes
float RoutePlanner::CalculateHValue(RouteModel::Node const *node) {
  		return node->distance(*(this->end_node));
}

//expand openlist of node yet to be visited
void RoutePlanner::AddNeighbors(RouteModel::Node *current_node) {
        current_node->FindNeighbors();
        for(auto node:current_node->neighbors){
          node->parent = current_node;
          node->h_value = this->CalculateHValue(node);
          node->g_value = current_node->g_value + node->distance(*current_node);

          this->open_list.push_back(node);
          node->visited = true;
        }
}

//helper function to compare two node f_values for sorting them
bool RoutePlanner::Compare(RouteModel::Node *n1, RouteModel::Node *n2){
	float f1 = n1->g_value + n1->h_value;
  	float f2 = n2->g_value + n2->h_value;
  
  	return f1>f2;
}

//sort the openlist and return nextnode with minimum fvalue
RouteModel::Node *RoutePlanner::NextNode() {
	float prev_f = std::numeric_limits<float>::max();
  	float f = 0.0;
  	RouteModel::Node* lowsumNode = nullptr;
  
  	std::sort(open_list.begin(),open_list.end(),this->Compare);  
  	lowsumNode = open_list.back();
  	open_list.pop_back();

   	return lowsumNode;
}

//construct final path based on all ndoes having minimum f-values
std::vector<RouteModel::Node> RoutePlanner::ConstructFinalPath(RouteModel::Node *current_node) {
        // Create path_found vector
        distance = 0.0f;
        std::vector<RouteModel::Node> path_found;

  	RouteModel::Node *curNode = current_node;
  	while(curNode){
            path_found.push_back(*curNode);
            if(curNode->parent)
                distance += curNode->distance(*curNode->parent);
            curNode = curNode->parent;
        }
  	std::reverse(path_found.begin(),path_found.end());
  
        distance *= m_Model.MetricScale(); // Multiply the distance by the scale of the map to get meters.
        return path_found;
}

//main Astar loop function
void RoutePlanner::AStarSearch() {
  RouteModel::Node *current_node = nullptr;  
  
  //fill open_list with start_node
  open_list.push_back(start_node);  
  this->start_node->visited = true;
  
  while(open_list.size() > 0){	
        current_node = NextNode();
        AddNeighbors(current_node);
     
        if(current_node->distance(*end_node) == 0){
            m_Model.path = ConstructFinalPath(end_node);
            break;
        }
  }      
}
