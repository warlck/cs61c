import Sliding, argparse
from pprint import pprint

level_to_pos = {}
pos_to_level = {}

def slidingBfsSolver(puzzle, width, height, max_level=-1):
    """BF visit the entire puzzle graph, build level_to_pos, pos_to_level structures."""
    solution = puzzle
    level = 0
    level_to_pos[level] = [solution] ### level 0 consists of a single solution
    pos_to_level[solution] = level

    ### While there are still positions on the frontier
    ### (seen for the first time in the last iteration)
    while level_to_pos[level] and (max_level==-1 or level < max_level):
        level += 1
        level_to_pos[level] = []
        ### For every position on the last level (these farthest-away positions are the "frontier")
        for position in level_to_pos[level-1]:
            ### For every child of those frontier positions
            for child in Sliding.children(width, height, position):
                ### If it's the first time we've seen this child
                if child not in pos_to_level:
                    ### Update the mappings to remember it, and it will be part of the new frontier
                    pos_to_level[child] = level
                    level_to_pos[level].append(child)

    del level_to_pos[level] ### the last level is always empty, so remove it.
    pprint(level_to_pos)


def main():
    parser = argparse.ArgumentParser(
            description="Returns back the entire solution graph.")
    parser.add_argument("-H", "--height", type=int, default=2,
            help="height of the puzzle")
    parser.add_argument("-W", "--width", type=int, default=2,
            help="width of the puzzle")
    args = parser.parse_args()

    p = Sliding.solution(args.width, args.height)
    slidingBfsSolver(p, args.width, args.height)


if __name__ == "__main__":
    main()
