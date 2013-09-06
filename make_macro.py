import sys

out = open(sys.argv[1] + ".marco", "w")
for line in open(sys.argv[1]):
    out.write(line[:-1] + "\\\n")

