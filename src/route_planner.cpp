#include "route_planner.h"
#include <algorithm>

RoutePlanner::RoutePlanner(RouteModel &model, float start_x, float start_y, float end_x, float end_y): m_Model(model) {
    // NOTE: Passing "model" argument as initialization list
    // Convert inputs to percentage:
    start_x *= 0.01;
    start_y *= 0.01;
    end_x *= 0.01;
    end_y *= 0.01;

    // TODO 2: Use the m_Model.FindClosestNode method to find the closest nodes to the starting and ending coordinates.
    // Store the nodes you find in the RoutePlanner's start_node and end_node attributes.
    start_node = &m_Model.FindClosestNode(start_x, start_y);
    end_node = &m_Model.FindClosestNode(end_x, end_y);
}


// TODO 3: Implement the CalculateHValue method.
// Tips:
// - You can use the distance to the end_node for the h value.
// - Node objects have a distance method to determine the distance to another node.

float RoutePlanner::CalculateHValue(RouteModel::Node const *node) {
    return node->distance(*end_node);
}


// TODO 4: Complete the AddNeighbors method to expand the current node by adding all unvisited neighbors to the open list.
// Tips:
// - Use the FindNeighbors() method of the current_node to populate current_node.neighbors vector with all the neighbors.
// - For each node in current_node.neighbors, set the parent, the h_value, the g_value. 
// - Use CalculateHValue below to implement the h-Value calculation.
// - For each node in current_node.neighbors, add the neighbor to open_list and set the node's visited attribute to true.

void RoutePlanner::AddNeighbors(RouteModel::Node *current_node) {
    current_node->FindNeighbors();
    for(auto ng: current_node->neighbors){
        ng->g_value = current_node->g_value+current_node->distance(*ng);
        ng->h_value = CalculateHValue(ng);
        ng->parent = current_node;
        this->open_list.push_back(ng);
        ng->visited = true;
    }
}


// TODO 5: Complete the NextNode method to sort the open list and return the next node.
// Tips:
// - Sort the open_list according to the sum of the h value and g value.
// - Create a pointer to the node in the list with the lowest sum.
// - Remove that node from the open_list.
// - Return the pointer.

RouteModel::Node *RoutePlanner::NextNode() {
    std::sort(open_list.begin(), open_list.end(), [](const RouteModel::Node *a, const RouteModel::Node *b) -> bool
    {
        // Sort from highest to lowest to get the last element easily and remove it
        return a->g_value + a->h_value > b->g_value + b->h_value;
    });
    auto next_node = open_list.back();
    open_list.pop_back();
    return next_node;
}


// TODO 6: Complete the ConstructFinalPath method to return the final path found from your A* search.
// Tips:
// - This method should take the current (final) node as an argument and iteratively follow the 
//   chain of parents of nodes until the starting node is found.
// - For each node in the chain, add the distance from the node to its parent to the distance variable.
// - The returned vector should be in the correct order: the start node should be the first element
//   of the vector, the end node should be the last element.

std::vector<RouteModel::Node> RoutePlanner::ConstructFinalPath(RouteModel::Node *current_node) {
    // Create path_found vector
    distance = 0.0f;
    std::vector<RouteModel::Node> path_found;

    // TODO: Implement your solution here.
    while(current_node->x != start_node->x && current_node->y != start_node->y){
        auto parent_node = current_node->parent;
        distance += current_node->distance(*parent_node);
        // NOTE: Creates a copy of "parent_node", see how to pass a reference to address memory
        // Check here: https://stackoverflow.com/a/2275095
        path_found.push_back(*current_node);
        current_node = parent_node;
    }
    // The start node avoids enter into the loop
    path_found.push_back(*current_node);
    std::cout << "Path found!" << '\n';
    // Starting node must be the first element of the vector
    std::reverse(path_found.begin(), path_found.end());
    int counter = 0;
    for(auto node: path_found){
        std::cout << "Node " << counter << " x=" << node.x << " y=" << node.y << '\n';
        counter++;
    }
    distance *= m_Model.MetricScale(); // Multiply the distance by the scale of the map to get meters.
    return path_found;

}


// TODO 7: Write the A* Search algorithm here.
// Tips:
// - Use the AddNeighbors method to add all of the neighbors of the current node to the open_list.
// - Use the NextNode() method to sort the open_list and return the next node.
// - When the search has reached the end_node, use the ConstructFinalPath method to return the final path that was found.
// - Store the final path in the m_Model.path attribute before the method exits. This path will then be displayed on the map tile.

void RoutePlanner::AStarSearch() {
    RouteModel::Node *current_node = nullptr;

    // TODO: Implement your solution here.
    open_list.push_back(start_node);
    current_node = start_node;
    current_node->visited = true;
    while(current_node->x != end_node->x && current_node->y != end_node->y){
        AddNeighbors(current_node);
        current_node = NextNode();
    }
    std::cout << "Path found! " << '\n';
    // We assume the search has found "end_node"
    auto final_path = ConstructFinalPath(current_node);
    std::cout << "Path constructed!" << '\n';
    m_Model.path = final_path;
}