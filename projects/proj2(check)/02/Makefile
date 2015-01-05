MASTER="YOUR MASTER URL HERE"
SLAVE_COUNT=12
MASTER_URL="spark://$(MASTER):7077"
SOLVER=SlidingBfsSpark.py
UTIL=Sliding.py

.PHONY: clean

default: ec2-medium

ec2-medium:
	rm -rf small-puzzle-out
	~/spark-ec2/copy-dir ~/proj2-2
	PYTHONWARNINGS="ignore" time ~/spark/bin/spark-submit --py-files $(UTIL) $(SOLVER) --master=$(MASTER_URL) --output="medium-puzzle-out" --height=5 --width=2 --slaves=$(SLAVE_COUNT)

ec2-large:
	rm -rf large-puzzle-out
	~/spark-ec2/copy-dir ~/proj2-2
	PYTHONWARNINGS="ignore" time ~/spark/bin/spark-submit --py-files $(UTIL) $(SOLVER) --master=$(MASTER_URL) --output="large-puzzle-out" --height=4 --width=3 --slaves=$(SLAVE_COUNT)


clean:
	rm -rf *-puzzle-out*
	rm -rf *.pyc
