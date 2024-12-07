from io import TextIOWrapper
import random as rand
import time

def gen_graph(size : int, density: float, outfile: TextIOWrapper):
    outfile.write("{}\n".format(size))
    for _ in range(0, size):
        for _ in range(0, size):
            r = rand.random()
            t : int
            if r <= density:
                t = 1
            else:
                t = 0
            outfile.write("{v} ".format(v=t))
        outfile.write("\n")    
    outfile.write("p(1) == {}\n\n".format(density))

if __name__ == "__main__":
    size_min = int(input("Size min: "))
    size_max = int(input("Size max: "))
    step = int(input("Step size: "))
    count = int(input("Count per size: "))
    density_t = input("Graph density (0 <= d <= 1; 'r' for random densities) ")
    filename_p = input("Target filename prefix: ")
    random_d = False

    try:
        density = float(density_t)
    except ValueError:
        random_d = True
        
    rand.seed(time.time())
    
    for s in range(size_min, size_max + 1, step):
        outfile = open("{prefix}_{size}.txt".format(prefix= filename_p, size= s), "wt")
        outfile.write("{s}\n\n".format(s=count))
        for _ in range(0, count):
            p : float
            if (random_d):
                p = rand.random()
            else:
                p = density
            
            gen_graph(s, p, outfile)
        outfile.close()
    
    print("Done")