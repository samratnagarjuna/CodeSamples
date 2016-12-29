//
//  main.cpp
//  Resource Manager
//
//  Created by Samrat Nagarjuna Pasila on 10/11/14.
//  Copyright (c) 2014 Samrat Nagarjuna Pasila. All rights reserved.
//

#include <iostream>
#include "resource_manager.h"

int main(int argc, const char * argv[])
{
    ResourceManager mngr;
    mngr.connect("handgun", "bullets");
    mngr.connect("bullets", "ore");
    mngr.connect("bombs", "ore");
    mngr.connect("turret", "bullets");
    mngr.delete_res("bullets");
    mngr.display_resources();
    return 0;
}

