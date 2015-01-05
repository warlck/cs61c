MASTER="ec2-54-91-5-115.compute-1.amazonaws.com"
SLAVE_COUNT=12
MASTER_URL="spark://$(MASTER):7077"
SOLVER=SlidingBfsSpark.py
UTIL=Sliding.py

.PHONY: clean, clean-medium, clean-large

default: ec2-medium

local-test:
	rm -rf local-test-puzzle-out
	PYTHONWARNINGS="ignore" time spark-submit --py-files $(UTIL) $(SOLVER) --master="local[8]" --output="local-test-puzzle-out" --height=5 --width=2 --slaves=4

ec2-test:
	rm -rf ec2-test-puzzle-out
	PYTHONWARNINGS="ignore" time ~/spark/bin/spark-submit --py-files $(UTIL) $(SOLVER) --master="local[8]" --output="ec2-test-puzzle-out" --height=5 --width=2 --slaves=4

ec2-medium:
	~/spark-ec2/copy-dir ~/proj2-2
	PYTHONWARNINGS="ignore" time ~/spark/bin/spark-submit --py-files $(UTIL) $(SOLVER) --master=$(MASTER_URL) --output="medium-puzzle-out" --height=5 --width=2 --slaves=$(SLAVE_COUNT)

ec2-large:
	~/spark-ec2/copy-dir ~/proj2-2
	PYTHONWARNINGS="ignore" time ~/spark/bin/spark-submit --py-files $(UTIL) $(SOLVER) --master=$(MASTER_URL) --output="large-puzzle-out" --height=4 --width=3 --slaves=$(SLAVE_COUNT)

clean-medium:
	~/ephemeral-hdfs/bin/hadoop fs -rmr hdfs:///user/root/medium-puzzle-out

clean-large:
	~/ephemeral-hdfs/bin/hadoop fs -rmr hdfs:///user/root/large-puzzle-out

clean:
	rm -rf *-puzzle-out*
	rm -rf *.pyc
