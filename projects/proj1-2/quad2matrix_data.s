.data
# Quadtree Node representation
# Recall that quadtree representation uses the following format:
#     struct qNode {
#         int leaf;
#         int size;
#         int x;
#         int y;
#         int gray_value;
#         qNode *child_NW, *child_NE, *child_SE, *child_SW;
#     }
        .word
q00:    0, 8, 0, 0, 256, qx00, qx40, qx44, qx04

qx00:   1, 4, 0, 0,   1, 0, 0, 0, 0
qx40:   0, 4, 4, 0, 256, qxx40, qxx60, qxx62, qxx42
qx44:   0, 4, 4, 0, 256, qxx44, qxx64, qxx66, qxx46
qx04:   1, 4, 0, 4,   2, 0, 0, 0, 0

qxx40:  1, 2, 4, 0,   3, 0, 0, 0, 0
qxx60:  1, 2, 6, 0,   4, 0, 0, 0, 0
qxx62:  0, 2, 6, 2, 256, qxxx62, qxxx72, qxxx73, qxxx63
qxx42:  1, 2, 4, 2,   5, 0, 0, 0, 0
qxx44:  1, 2, 4, 4,   6, 0, 0, 0, 0
qxx64:  1, 2, 6, 4,   7, 0, 0, 0, 0
qxx66:  0, 2, 6, 6, 256, qxxx66, qxxx76, qxxx77, qxxx67
qxx46:  1, 2, 4, 6,   8, 0, 0, 0, 0

qxxx62: 1, 1, 6, 2,   9, 0, 0, 0, 0
qxxx72: 1, 1, 7, 2,  10, 0, 0, 0, 0
qxxx73: 1, 1, 7, 3,  11, 0, 0, 0, 0
qxxx63: 1, 1, 6, 3,  12, 0, 0, 0, 0
qxxx66: 1, 1, 6, 6,  13, 0, 0, 0, 0
qxxx76: 1, 1, 7, 6,  14, 0, 0, 0, 0
qxxx77: 1, 1, 7, 7,  15, 0, 0, 0, 0
qxxx67: 1, 1, 6, 7,  16, 0, 0, 0, 0
