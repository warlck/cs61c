#!/bin/bash

#PBS -N CS61C
#PBS -V

### Request email when job begins and ends
#PBS -m bea

### Specify email address to use for notification.
#PBS -M [email]@berkeley.edu

# CHANGE PPN to set number of cores to use...
#PBS -l nodes=1:ppn=1
#PBS -q batch

### change to the directory qsub was called from
cd $PBS_O_WORKDIR

### workaround to fix a problem in Linux
export GOTOBLAS_MAIN_FREE=1

# name of the file to execute
./benchmark > bench-output.txt
