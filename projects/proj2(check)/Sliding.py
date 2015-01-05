import string

def solution(W, H):
    """
    Returns the solution board configuration for the sliding
    puzzle with the provided dimensions.
    """
    return tuple(string.ascii_uppercase[0:(W*H) - 1] + "-")

def swap(board, i, j):
    boardL = list(board)
    boardL[i], boardL[j] = boardL[j], boardL[i]
    return tuple(boardL)

def children(W, H, board):
    """
    Returns a list of all children board configuration from
    the given board configuration.
    """
    i = board.index("-")
    children = []
    if i % W != 0:      ### not on left edge
        children.append(swap(board, i,i-1))
    if i % W != (W-1):  ### not on right edge
        children.append(swap(board, i,i+1))
    if i >= W:          ### not on top edge
        children.append(swap(board, i,i-W))
    if i < W*(H-1):     ### not on bottom edge
        children.append(swap(board, i,i+W))
    return children
