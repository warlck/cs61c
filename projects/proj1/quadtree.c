/* CS61C Fa14 Project 1
 *
 * Converts a bitmap array into its quadtree representation.
 *
 * ANY MODIFICATIONS TO THIS FILE WILL BE OVERWRITTEN DURING GRADING
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "quadtree.h"
#include "make_qtree.h"
#include "utils.h"

#define MIN(a,b) ((a) < (b) ? a : b)
/* You may find these functions helpful in debugging */ //added comment
int print_basic(){
    qNode *qtree_root;

    unsigned char tiny[] = { 56 };

    unsigned char small[] = {
        0x00, 0x00, 0x09, 0x0A,
        0x00, 0x00, 0x0B, 0x0C,
        0x25, 0x26, 0x26, 0xA0,
        0x25, 0x26, 0x26, 0xA1
    };

    unsigned char empty[] = {
        0xFF, 0xFF, 0xFF, 0xFF,
        0xFF, 0xFF, 0xFF, 0xFF,
        0xFF, 0xFF, 0xFF, 0xFF,
        0xFF, 0xFF, 0xFF, 0xFF
    };

    qtree_root = depth_to_quad(tiny, 1);
    print_qtree(qtree_root);
    print_compressed(qtree_root);
    free_qtree(qtree_root);

    qtree_root = depth_to_quad(small, 4);
    print_qtree(qtree_root);
    print_compressed(qtree_root);
    free_qtree(qtree_root);

    qtree_root = depth_to_quad(empty, 4);
    print_qtree(qtree_root);
    print_compressed(qtree_root);
    free_qtree(qtree_root);

    // If you want to save arrays as graycale bitmaps for testing, check out save_image() in utils.c
    // save_image("image/small-input.bmp", small, 4, 4);
    return 0;

}

int main(int argc, char **argv) {
    Image img;
    qNode* qtree_root;
    unsigned int mask;
    int width, begin_x, center_y, counter = 0;
    unsigned char *image;

    if (argc < 2) {
        print_basic();
    } else {
        img = load_image(argv[1]);

        mask = 1 << (sizeof(int)*8 - 1);
        width = MIN(img.width, img.height);
        while(!(width & mask)) mask = mask >> 1;

        if (width == mask && img.width == img.height) {
            qtree_root = depth_to_quad(img.data, img.width);
            print_qtree(qtree_root);
            print_compressed(qtree_root);
            free(img.data);
        } else {
            width = mask;
            begin_x = (img.width >> 1) - (width >> 1);
            center_y = img.height >> 1;
            image = (unsigned char *) malloc(width * width * sizeof(unsigned char));

            for (int i = center_y - (width>>1); i < center_y + (width>>1); i++) {
                memcpy(image + (counter * width), img.data + (i * img.width) + begin_x, width);
                counter++;
            }

            qtree_root = depth_to_quad(image, width);
            print_qtree(qtree_root);
            print_compressed(qtree_root);
            free_qtree(qtree_root);
            free(image);
            free(img.data);
        }

        return 0;
    }
}

/* Helper for print_qtree */
void recurse_print_qtree(qNode *qtree_root, int depth) {
    printf("%*s{\n", depth * 2, "");
    printf("%*ssize: %d\n", (depth + 1) * 2, "", qtree_root->size);
    printf("%*sx: %d, y: %d\n", (depth + 1) * 2, "", qtree_root->x, qtree_root->y);
    printf("%*svalue: %d\n", (depth + 1) * 2, "", qtree_root->gray_value);
    if (!qtree_root->leaf) {
        printf("%*schildren => [\n", (depth + 1) * 2, "");
        recurse_print_qtree(qtree_root->child_NW, depth+1);
        recurse_print_qtree(qtree_root->child_NE, depth+1);
        recurse_print_qtree(qtree_root->child_SE, depth+1);
        recurse_print_qtree(qtree_root->child_SW, depth+1);
        printf("%*s]\n", depth*2, "");
    }
    printf("%*s}\n", depth*2, "");
}

/* Prints out the quadtree */
void print_qtree(qNode *qtree_root) {
    if(!qtree_root) return;
    printf("== Printing QuadTree ==\n");
    recurse_print_qtree(qtree_root, 0);
    printf("\n");
}

/* Helper for print_compressed */
void recurse_print_comp(qNode *qtree_root) {
    if (!qtree_root->leaf) {
        recurse_print_comp(qtree_root->child_NW);
        recurse_print_comp(qtree_root->child_NE);
        recurse_print_comp(qtree_root->child_SE);
        recurse_print_comp(qtree_root->child_SW);
    } else {
        printf("[value: %d, size: %d] ", qtree_root->gray_value, qtree_root->size);
    }
}

/* Prints out the compressed quadtree */
void print_compressed(qNode *qtree_root) {
    if(!qtree_root) return;
    printf("== Printing Compressed Depth Map ==\n");
    recurse_print_comp(qtree_root);
    printf("\n\n");
}
