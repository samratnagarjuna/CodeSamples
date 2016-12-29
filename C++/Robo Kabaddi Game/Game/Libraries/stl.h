//
//  stl.h
//  XbeeInterface
//
//  Created by Samrat Nagarjuna Pasila on 15/09/14.
//  Copyright (c) 2014 Samrat Nagarjuna Pasila. All rights reserved.
//

#ifndef XbeeInterface_stl_h
#define XbeeInterface_stl_h

#include <vector>

#define traverse(c,i) \
    for(typeof((c).begin()) i = (c).begin(); i != (c).end(); i++)
#define loop(i,n) for (int i = 0 ; i < n ; i++)

#endif
