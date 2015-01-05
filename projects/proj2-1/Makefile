MASTER="local[8]"
SOLVER=SlidingBfsSpark.py

.PHONY: clean

default: run-small

run-small:
	rm -rf small-puzzle-out.txt
	PYTHONWARNINGS="ignore" time spark-submit $(SOLVER) --master=$(MASTER) --output="small-puzzle-out.txt" --height=2 --width=2

run-medium:
	rm -rf medium-puzzle-out.txt
	PYTHONWARNINGS="ignore" time spark-submit $(SOLVER) --master=$(MASTER) --output="medium-puzzle-out.txt" --height=3 --width=3

run-large:
	rm -rf large-puzzle-out.txt
	PYTHONWARNINGS="ignore" time spark-submit $(SOLVER) --master=$(MASTER) --output="large-puzzle-out.txt" --height=5 --width=2

clean:
	rm -rf *-puzzle-out*
	rm -rf *.pyc
