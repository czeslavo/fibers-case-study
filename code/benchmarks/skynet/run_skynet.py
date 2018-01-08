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

# uruchamia pojedynczy test `name` o rozmiarze `n` 
# 10-krotnie, zwracaja średni czas
def run_single(name, n):
    print "Running " + name + "..."
    results = []
    for i in range(10):
        results.append(
            int(sp.check_output(" ".join(["./" + name, str(n)]), shell=True)))

    return np.average(results)
    

# uruchamia wszystkie testy z listy `benchmarks` 
# dla rozmiaru problemu `agents`
def run_all(agents):
    results = {}
    for b in benchmarks:
        # nie uruchamiaj testów z użyciem wątków, kiedy rozmiar zbyt duży
        if b in ("skynet_async", "skynet_thread") and agents > 10000:
            continue

        # zapisz wynik w słowniku
        results[b] = run_single(b, agents)
    
    return results


# wyrysuj wykres dla wyników `results`
def plot_results(results):
    # skróc nazwy 
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


def main():
    if len(sys.argv) < 2:
        print "usage: " + sys.argv[0] + " number_of_agents"
        exit(1)

    agents = int(sys.argv[1])
    
    # przeprowadz wszystkie testy 
    results = run_all(agents)

    # wyświetl wykres
    plot_results(results)

main()

