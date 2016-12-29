//
//  node.h
//  Resource Manager
//
//  Created by Samrat Nagarjuna Pasila on 10/11/14.
//  Copyright (c) 2014 Samrat Nagarjuna Pasila. All rights reserved.
//

#ifndef __Resource_Manager__node__
#define __Resource_Manager__node__

#include <string>
#include <set>

class Node;
typedef Node* NodePtr;

class Node {
    
private:
    std::string name;
    bool usable;
    //Pointers to Nodes this node is depending on
    std::set<NodePtr> depending_nodes;
    //Pointers to Nodes dependent on this Node
    std::set<NodePtr> dependent_nodes;
    void init(std::string name);
    void update_dependents();
    void check_and_update_usability();
    
public:
    Node(std::string name);
    std::string get_name();
    bool is_usable();
    void set_usable(bool usable);
    void add_dependent_node(NodePtr node_ptr);
    void remove_dependent_node(NodePtr node_ptr);
    void add_depending_node(NodePtr node_ptr);
    void remove_depending_node(NodePtr node_ptr);
    void remove_dependency();
    void print_depending_nodes();
    void print_dependent_nodes();
    ~Node();
    
};

#endif /* defined(__Resource_Manager__node__) */
