from pyspark import SparkContext
import Sliding, argparse

def bfs_map(value):
    """ YOUR CODE HERE """
    prev = [(value[0], value[1])]
    #hashID = Sliding.board_to_hash(WIDTH, HEIGHT, value[0]) #value[0]
    #currBoard = Sliding.hash_to_board(WIDTH, HEIGHT, hashID) # ask manny if this is the correct to call this method
    if value[1] == level:
        #convert from int to board
        # do I save it, not sure? ask Manny , using hashID since it was declared down?
        # hashID = Sliding.board_to_hash(WIDTH, HEIGHT, prev[0][0]) #value[0]
        #currBoard = Sliding.hash_to_board(WIDTH, HEIGHT, value[0]) # ask manny if this is the correct to call this method
        # also what would number be?
        children = Sliding.children(WIDTH, HEIGHT, Sliding.hash_to_board(WIDTH, HEIGHT, value[0])) # not sure value[0], currBoard
        #currBoard = Sliding.board_to_hash(WIDTH, HEIGHT, children[0])
        curr = []
        for i in range(0, len(children)):
            curr.append((Sliding.board_to_hash(WIDTH, HEIGHT, children[i]), level+1))
            #nextID = Sliding.board_to_hash(WIDTH, HEIGHT, children[0]) #children[0]
        return prev + curr
    #nextID = Sliding.board_to_hash(WIDTH, HEIGHT, children[0]) #children[0]
    return prev

def bfs_reduce(value1, value2):
    """ YOUR CODE HERE """
    return min(value1, value2)

#comment out if errors
#def partitionHash(value):
    #currBoard = Sliding.hash_to_board(WIDTH, HEIGHT, value)
    #return hash(value)

def solve_puzzle(master, output, height, width, slaves):
    global HEIGHT, WIDTH, level
    HEIGHT=height
    WIDTH=width
    level = 0

    sc = SparkContext(master, "python")

    """ YOUR CODE HERE """
    sol = Sliding.solution(WIDTH, HEIGHT)
    hashID = Sliding.board_to_hash(WIDTH, HEIGHT, sol) #board(obj) to hash(int) #either sol or value[0], is this here?
    rdd = sc.parallelize([(hashID, level)])

    prev_count = 0
    count = rdd.count()

    k = 0
    i = 0
    #put this here since I am assuming this part?   next try uncomment this
    #hashID = Sliding.board_to_hash(WIDTH, HEIGHT, sol) #board(obj) to hash(int) #either sol or value[0], is this here?
    while prev_count < count:
        rdd = rdd.flatMap(bfs_map)
        if k % 4 == 0:
            #board = Sliding.hash_to_board(WIDTH, HEIGHT, hashID)
            rdd = rdd.partitionBy(16, hash) #else try Sliding.board_to_hash
        rdd = rdd.reduceByKey(bfs_reduce)
        level += 1
        if i % 4 == 0:
            prev_count = count
            count = rdd.count()

        k += 1
        i += 1
    boardState = Sliding.board_to_hash(WIDTH, HEIGHT, sol) #board(obj) to hash(int) #either sol or value[0], is this here?, so it is an int
    #PARTITION_COUNT = slaves
    #slaves = 12
    #output = str(pos[1]) + " " + str(pos[0])
    rdd.coalesce(slaves).saveAsTextFile(output) # Let NUM_WORKERS be the number of workers (6 or 12), this is the new way IS IT SLAVES
    sc.stop()



""" DO NOT EDIT PAST THIS LINE

You are welcome to read through the following code, but you
do not need to worry about understanding it.
"""

def main():
    """
    Parses command line arguments and runs the solver appropriately.
    If nothing is passed in, the default values are used.
    """
    parser = argparse.ArgumentParser(
            description="Returns back the entire solution graph.")
    parser.add_argument("-M", "--master", type=str, default="local[8]",
            help="url of the master for this job")
    parser.add_argument("-O", "--output", type=str, default="solution-out",
            help="name of the output file")
    parser.add_argument("-H", "--height", type=int, default=2,
            help="height of the puzzle")
    parser.add_argument("-W", "--width", type=int, default=2,
            help="width of the puzzle")
    parser.add_argument("-S", "--slaves", type=int, default=6,
            help="number of slaves executing the job")
    args = parser.parse_args()

    global PARTITION_COUNT
    PARTITION_COUNT = args.slaves * 16

    # call the puzzle solver
    solve_puzzle(args.master, args.output, args.height, args.width, args.slaves)

# begin execution if we are running this file directly
if __name__ == "__main__":
    main()
