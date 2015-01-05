/* CS61C Fa14 Project 1
 *
 * ANY MODIFICATIONS TO THIS FILE WILL BE OVERWRITTEN DURING GRADING
 */

#ifndef MAKE_QTREE_H
#define MAKE_QTREE_H

/* Implement this in make_qtree.c */
int homogenous(unsigned char *depth_map, int map_width, int x, int y, int section_width);

/* Implement this in make_qtree.c */
qNode *depth_to_quad(unsigned char *depth_map, int image_width);

/* Function that frees the quadtree created */
void free_qtree(qNode *qtree_root);

#endif
