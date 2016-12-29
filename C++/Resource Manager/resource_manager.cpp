//
//  resource_manager.cpp
//  Resource Manager
//
//  Created by Samrat Nagarjuna Pasila on 10/11/14.
//  Copyright (c) 2014 Samrat Nagarjuna Pasila. All rights reserved.
//

#include "resource_manager.h"
#include <iostream>

#define tr(c,i) for(typeof((c).begin()) i = (c).begin(); i != (c).end(); i++)

/**
 * Get pointer to the resource
 * @params
 * res_name: Unique name of the resource
 * @return Pointer to the resource if available else NULL
 */
NodePtr ResourceManager::get_res_ptr(std::string res_name) {
    res_map_it res_it = res_map.find(res_name);
    return res_it != res_map.end() ? res_it->second : NULL;
}

/**
 * Create a new resource node if not available else if already existing
 * return the pointer to the node
 * @params
 * res_name: Unique name of the resource
 * @return pointer to the node created or existing
 */
NodePtr ResourceManager::create_res(std::string res_name) {
    NodePtr res_ptr = get_res_ptr(res_name);
    if (!res_ptr) {
        res_ptr = new Node(res_name);
        res_map[res_name] = res_ptr;
    }
    return res_ptr;
}

/**
 * Connect nodes based on dependency
 * @params
 * res_name: Unique name of the resource
 * depending_res_name: Unique name of the resource depending on res_name
 */
void ResourceManager::connect(std::string res_name,
                              std::string depending_res_name) {
    NodePtr res_ptr = create_res(res_name);
    NodePtr depending_res_ptr = create_res(depending_res_name);
    connect(res_ptr, depending_res_ptr);
}

/**
 * Connect nodes based on dependency
 * @params
 * res_ptr: Pointer to the resource
 * depending_res_ptr: Pointer to resource depending on node pointed by res_ptr
 */
void ResourceManager::connect(NodePtr res_ptr, NodePtr depending_res_ptr) {
    res_ptr->add_depending_node(depending_res_ptr);
    depending_res_ptr->add_dependent_node(res_ptr);
}

/**
 * Delete a resource
 * @params
 * res_name: Unique name of the resource to be deleted
 */
void ResourceManager::delete_res(std::string res_name) {
    NodePtr res_ptr = get_res_ptr(res_name);
    if (res_ptr) {
        res_ptr->set_usable(false);
        res_ptr->remove_dependency();
        res_map.erase(res_ptr->get_name());
        delete res_ptr;
    }
}

/**
 * Display the existing resouces with depending and dependent nodes
 */
void ResourceManager::display_resources() {
    tr(res_map, it) {
        NodePtr res_ptr = it->second;
        std::cout << it->first << ": ";
        std::cout << res_ptr->is_usable() << std::endl;
        res_ptr->print_depending_nodes();
        std::cout << std::endl;
        res_ptr->print_dependent_nodes();
        std::cout << std::endl;
    }
}

/**
 * Deletes all the resouces
 */
ResourceManager::~ResourceManager() {
    std::cout << "Resource Manger Deleting.." << std::endl;
    tr(res_map, it) {
        delete it->second;
    }
    std::cout << "Program successfully terminated!!" << std::endl;
}