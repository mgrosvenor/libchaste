import sys

out = open(sys.argv[1] + ".marco", "w")

type = "i64"

found = 0

for line in open(sys.argv[1]):
    if not found :
        if line.find("#define define") >= 0 or line.find("#define declare") >= 0:
            
            found = 1
        else:
            out.write(line)
            continue

    line = line.replace("_%s_" % type, "_##TYPE##_")
    line = line.replace("_%s" % type, "_##TYPE")
    line = line.replace("%s" % type, "TYPE")
    out.write(line[:-1] + "\\\n")





