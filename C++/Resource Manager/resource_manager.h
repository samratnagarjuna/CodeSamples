//
//  resource_manager.h
//  Resource Manager
//
//  Created by Samrat Nagarjuna Pasila on 10/11/14.
//  Copyright (c) 2014 Samrat Nagarjuna Pasila. All rights reserved.
//

#ifndef __Resource_Manager__resource_manager__
#define __Resource_Manager__resource_manager__

#include "node.h"
#include <string>
#include <map>

typedef std::map<std::string, NodePtr>::iterator res_map_it;

class ResourceManager {
private:
    std::map<std::string, NodePtr> res_map;
    NodePtr get_res_ptr(std::string res_name);
    
public:
    NodePtr create_res(std::string res_name);
    void connect(NodePtr res_ptr, NodePtr depending_res_ptr);
    void connect(std::string res_name, std::string depending_res_name);
    void delete_res(std::string res_name);
    void display_resources();
    ~ResourceManager();
};

#endif /* defined(__Resource_Manager__resource_manager__) */
