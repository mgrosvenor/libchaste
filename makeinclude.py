import os
import sys

tmp = sys.argv[1]


for l in open("%s/.tmp.dirs" %tmp):
    cmd = "mkdir -p %s/include/%s" %( tmp, l[2:-1] )
    print cmd
    os.system(cmd)

for l in open("%s/.tmp.headers" % tmp):
    cmd = "cp %s %s/include/%s" %( l[2:-1], tmp, l[2:-1] )
    print cmd
    os.system(cmd)

    
    
