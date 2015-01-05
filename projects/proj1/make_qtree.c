/*
 * PROJ1-1: YOUR TASK B CODE HERE
 *
 * Feel free to define additional helper functions.
 */

#include <stdlib.h>
#include <stdio.h>
#include "quadtree.h"
#include "make_qtree.h"
#include "utils.h"

#define ABS(x) (((x) < 0) ? (-(x)) : (x))

int homogenous(unsigned char *depth_map, int map_width, int x, int y, int section_width) {

    /* YOUR CODE HERE */
    int currPix = y * map_width + x;
    unsigned char* ptr = depth_map;//just added this, extra pointer
    for (int i = 0; i < currPix; i++) {
        ptr++;
    }
    unsigned char value = *ptr;
    for (int j = 0; j < section_width; j++) { 
        for (int k = 0; k < section_width; k++) { 
            if (value != *ptr) {
                return 256;
            }
            ptr++;
        }
        ptr += section_width;
    }
    return value;
}

qNode *helper(unsigned char *depth_map, int map_width, int x, int y, int section_width){

    qNode *currNode = malloc(sizeof(qNode));
    if (currNode == NULL) {
        allocation_failed();
    }

    if (homogenous(depth_map, map_width, x, y, section_width) != 256) w{  ///leaf so no children
        currNode -> gray_value = homogenous(depth_map, map_width, x, y, section_width);
        currNode -> leaf = 1;
        currNode -> x = x;
        currNode -> y = y;
        currNode ->size = section_width; 
        return currNode;
    } else {
        currNode -> leaf = 0;
        currNode -> x = x;
        currNode -> y = y;
        currNode ->size = section_width;
        currNode -> child_NW = helper(depth_map, map_width, x, y, section_width/2);
        currNode -> child_NE = helper(depth_map, map_width, x + section_width/2, y, section_width/2);
        currNode -> child_SE = helper(depth_map, map_width, x + section_width/2, y + section_width/2, section_width/2);
        currNode -> child_SW = helper(depth_map, map_width, x, y + section_width/2, section_width/2);
        currNode -> gray_value = homogenous(depth_map, map_width, x, y, section_width);
        return currNode; 
    }
}

qNode *depth_to_quad(unsigned char *depth_map, int map_width) {

    /* YOUR CODE HERE */   
    return helper(depth_map, map_width, 0, 0, map_width);

}

void free_qtree(qNode *qtree_node) {
    if(qtree_node) {
        if(!qtree_node->leaf){
            free_qtree(qtree_node->child_NW);
            free_qtree(qtree_node->child_NE);
            free_qtree(qtree_node->child_SE);
            free_qtree(qtree_node->child_SW);
        }
        free(qtree_node);
    }
}
