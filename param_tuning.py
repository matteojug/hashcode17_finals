import os
from itertools import product
from subprocess import Popen
import subprocess
import random
from shutil import copyfile

def exec_with_params(params):
    #set the executable name
    exename = ""
    if os.name == "nt":
        #windows
        exename = "main.exe"
    else:
        #linux
        exename = "./main"

    #create command line with parameters
    command = [exename] + [str(i) for i in params]

    proc = Popen(command, stdout = subprocess.PIPE)

    last_line = ""
    for line in proc.stdout:
        if line != "":
            last_line = line

    res = last_line.split(" ")
    return (res[0], int(res[1]))


# OPT1: WRITE HERE YOUR RANDOM PARAM GENERATOR
def random_params():
    while True:
        a = []
        a.append(2)
        a.append(random.randint(0, 10))
        a.append(random.randint(0, 10))
        a.append(0)
        a.append(0)
        a.append(random.randint(-10, 0))
        a.append(random.randint(0, 10))
        yield a

# CHOOSE
domains = [[3],range(1,6),range(1,6)]
iterator = product(*domains)

iterator = random_params()

#let's try the combinations
best_score = -1
best_file = 0

max_iterations = 100 #-1 means all combinations
import time

#~ curr_combination = [random.randint(-1,1) for i in xrange(7)]
#~ curr_combination[0] = 1

#~ delta = 3
#~ # random climbing
#~ for i in xrange(max_iterations):
    #~ print "[{}] Base: {}".format(i, curr_combination)
    #~ combination = curr_combination[:]
    #~ idx = random.randint(1,6)
    #~ combination[idx] -= delta/2
    #~ res = []
    #~ for j in xrange(delta):
        #~ filename, val = exec_with_params(combination)
        #~ print("[{}:{}] Params: {} => {} (vs best {})".format(i, j, combination, val, best_score))
        #~ if val > best_score:
            #~ best_score = val
            #~ curr_combination = combination[:]
            #~ print("[NEW BEST] {} => {}".format(combination, val))
            #~ #now copy file
            #~ time.sleep(1)
            #~ copyfile(filename, "{}.best".format(filename))
        #~ combination[idx] += 1

for i, combination in enumerate(iterator):
    if i == max_iterations:
        break
    filename, val = exec_with_params(combination)
    print("[{}] Params: {} => {} (vs best {})".format(i, combination, val, best_score))
    if val > best_score:
        best_score = val
        print("[NEW BEST] {} => {}".format(combination, val))
        #now copy file
        time.sleep(1)
        copyfile(filename, "{}.best".format(filename))
