#!/usr/bin/python

import matplotlib.pyplot as plt
import numpy as np
import subprocess as sp 
import sys
from basic_units import milisecs

benchmarks = [
    "skynet_thread",
    "skynet_async",
    "skynet_fiber_detach",
    "skynet_fiber_join",
    "skynet_fiber_shared_detach",
    "skynet_fiber_shared_join"
]

def run_single(name, n):
    return int(sp.check_output(" ".join(["./" + name, str(n)]), shell=True))
    

def run_all():
    agents = int(sys.argv[1])
    results = {}
    for b in benchmarks:
        # ignore those - too much agents for ordinary threads
        if b in ("skynet_async", "skynet_thread") and agents > 10000:
            continue

        results[b] = run_single(b, agents)

    return results

def main():
    results = run_all()

    x = [k.replace('skynet_', '') for k in results.keys()]
    
    fig, (p1, p2, p3, p4) = plt.subplots(nrows=4, sharex=True)
    p1.yaxis.set_units(milisecs)
     
    p1.bar(x, results.values())
    p1.
    #plt.xticks(rotation='vertical')
    
    plt.show()

main()

