//
//  node.cpp
//  Resource Manager
//
//  Created by Samrat Nagarjuna Pasila on 10/11/14.
//  Copyright (c) 2014 Samrat Nagarjuna Pasila. All rights reserved.
//

#include "node.h"
#include <iostream>

#define tr(c,i) for(typeof((c).begin()) i = (c).begin(); i != (c).end(); i++)

Node::Node(std::string name) {
    init(name);
}

void Node::init(std::string name) {
    this->name = name;
    set_usable(true);
}

void Node::set_usable(bool usable) {
    this->usable = usable;
    update_dependents();
}

std::string Node::get_name() {
    return name;
}

/**
 * Recursively updates dependencies between nodes
 */
void Node::update_dependents() {
    if (!usable) {
        tr(dependent_nodes, it) {
            if ((*it)->is_usable()) {
                (*it)->set_usable(false);
            }
        }
    }
    else {
        tr(dependent_nodes, it) {
            if (!(*it)->is_usable()) {
                (*it)->check_and_update_usability();
            }
        }
    }
}

/**
 * Checks if all the depending nodes are usable and updates usability of current
 * node as well as dependent nodes
 */
void Node::check_and_update_usability() {
    bool usable = true;
    tr(depending_nodes, it) {
        if (!(*it)->is_usable()) {
            usable = false;
            break;
        }
    }
    set_usable(usable);
}


bool Node::is_usable() {
    return usable;
}

void Node::add_dependent_node(NodePtr node_ptr) {
    dependent_nodes.insert(node_ptr);
}

void Node::remove_dependent_node(NodePtr node_ptr) {
    dependent_nodes.erase(node_ptr);
}

void Node::remove_depending_node(NodePtr node_ptr) {
    depending_nodes.erase(node_ptr);
}

void Node::add_depending_node(NodePtr node_ptr) {
    depending_nodes.insert(node_ptr);
}

void Node::remove_dependency() {
    tr(dependent_nodes, it) {
        (*it)->remove_depending_node(this);
    }
}

void Node::print_depending_nodes() {
    std::cout << "Depending nodes:" << std::endl;
    tr(depending_nodes, it) {
        std::cout << (*it)->get_name() << " | ";
    }
    std::cout << std::endl;
}

void Node::print_dependent_nodes() {
    std::cout << "Dependent nodes:" << std::endl;
    tr(dependent_nodes, it) {
        std::cout << (*it)->get_name() << " | ";
    }
    std::cout << std::endl;
}

Node::~Node() {
    std::cout << "Deleting resource: " << name << std::endl;
}