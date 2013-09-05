#CamIO2 dependency checker / installer

import sys
import os
import platform
import camio_prepare_arch
from camio_deps import * #Suck up all the definitions in camio_deps

#list of paths required to exist
build_paths = ["deps"]

#Main entry point
def main(install_opt):

    #Make sure all the development paths are ready
    for path in build_paths:
        path = "../" + path #We're running inside of the scripts dir
        if not os.path.exists(path):
            print "CamIO2 Prepare: Making build path \"" + path + "\"..."
            os.mkdir(path)

    #Detect and run the platform specific installer
    camio_prepare_arch.platform_install()
   
