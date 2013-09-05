#Checks that there is a recent enough version of Python on this machine
#and if so, kicks off the main dependency checker / installer

import sys

req_version = (2,6)
cur_version = sys.version_info

if cur_version >= req_version:
    import camio_prepare
    camio_prepare.main(False)
else:
    print "Your Python interpreter is older than version 2.5. CamIO requires version 2.5 or later. Please upgrade if you'd like to continue using CamIO"
  
