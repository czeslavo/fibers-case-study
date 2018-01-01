#!/usr/bin/python

import matplotlib.pyplot as plt
import numpy as np
import subprocess as sp 
import sys

benchmarks = [
    "skynet_thread",
    "skynet_async",
    "skynet_fiber_detach",
    "skynet_fiber_join",
    "skynet_fiber_shared_detach",
    "skynet_fiber_shared_join"
]

def run_single(name, n):
    results = []
    for i in range(10):
        results.append(
            int(sp.check_output(" ".join(["./" + name, str(n)]), shell=True)))

    return np.average(results)
    

def run_all(agents):
    results = {}
    for b in benchmarks:
        # ignore those - too much agents for ordinary threads
        if b in ("skynet_async", "skynet_thread") and agents > 10000:
            continue

        results[b] = run_single(b, agents)

    return results

def main():
    if len(sys.argv) < 2:
        print "usage: " + sys.argv[0] + " number_of_agents"
        exit(1)

    agents = int(sys.argv[1])
    results = run_all(agents)

    x = [k.replace('skynet_', '') for k in results.keys()]
    
    fig, ax = plt.subplots()
    plt.title("Skynet n = " + str(agents))
    bars = plt.bar(x, results.values(), color='gray')
    plt.ylabel('czas wykonania [ms]')
    plt.xticks(rotation=90)

    max_height = max([b.get_height() for b in bars])
    for b in bars:
        height = b.get_height()
        x = b.get_x() + b.get_width() / 2.
        y = height * 1.01 
        ax.text(x,
                y, "%d" % int(height),
                ha='center', va='bottom')

    plt.show()

main()

