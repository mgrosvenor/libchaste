import sys

type = "i64"


out = open(sys.argv[1] + ".c", "w")
for line in open(sys.argv[1]):
    line = line.replace("_##TYPE##","_i64")
    line = line.replace("_##TYPE","_i64")
    line = line.replace("TYPE",type)
    if line[-2:] ==  "\\\n":
        out.write(line[:-2] + "\n"); 




