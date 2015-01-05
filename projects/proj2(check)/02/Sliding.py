import math, string

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

def board_to_hash(W, H, board):
    def renumber(n,L):
        """This takes a list L of numbers whose elements are 0 to n (missing k) and
        lowers all the numbers above k to return a list 0 to n-1 permuted."""
        return list(map(lambda k:k if (k<n) else k-1,L))

    def hash_list(L):
        """A traditional hash of a permutation of a list of numbers from 0-n.

        For example,
        hash_list([0,1,2]) ==> 0
        hash_list([0,2,1]) ==> 1
        hash_list([1,0,2]) ==> 2
        hash_list([1,2,0]) ==> 3
        hash_list([2,0,1]) ==> 4
        hash_list([2,1,0]) ==> 5"""
        if not L:
            return 0
        else:
            return math.factorial(len(L)-1)*L[0] + hash_list(renumber(L[0],L[1:]))

    boardL = list(board)
    boardL[board.index("-")] = chr(ord('A')-1)  ### the 0
    renumbered_board = list(map(lambda c:ord(c)-ord('A')+1, boardL))
    return hash_list(renumbered_board)

def hash_to_board(W, H, number):
    def unhash_helper(i, N, array):
        """
        0  -ABC
        1  -ACB
        2  -BAC
        3  -BCA
        4  -CAB
        5  -CBA
        6  A-BC
        7  A-CB
        8  AB-C
        9  ABC-
        10 AC-B
        11 ACB-
        12 B-AC
        13 B-CA
        14 BA-C
        15 BAC-
        16 BC-A
        17 BCA-
        18 C-AB
        19 C-BA
        20 CA-B
        21 CAB-
        22 CB-A
        23 CBA-
        """
        if N == 1:
            return (array[0],)
        else:
            thisdigit = i / math.factorial(N-1)
            remainder = i % math.factorial(N-1)
            thisnumber = array[thisdigit]
            del array[thisdigit]
            return (thisnumber,) + unhash_helper(remainder, N-1, array)
    N = W * H
    boardL = list("-" + string.ascii_uppercase[0:N-1])
    return tuple(unhash_helper(number, N, boardL))
