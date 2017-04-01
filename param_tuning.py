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
        a.append(1)
        a.append(random.randint(0, 100))
        yield a

#OPT2: WRITE HERE THE DOMAINS OF THE VARIABLES YOU WANT TO TEST
domains = [
    [1,2],
    [3,4],
    [5,6]
]

# CHOOSE
iterator = product(*domains)

#~ iterator = random_params()

#let's try the combinations
best_score = -1
best_file = 0

max_iterations = 10 #-1 means all combinations

for i, combination in enumerate(iterator):
    if max_iterations > 0 and i == max_iterations:
        break
    filename, val = exec_with_params(combination)
    print("trying combination n. {}, that is {}, resulted in score {}".format(i, combination, val))
    if val > best_score:
        best_score = val
        print("[NEW BEST] combination n. {}, that is {}, with score {}".format(i, combination, val))
        #now copy file
        copyfile(filename, "{}.best".format(filename))
