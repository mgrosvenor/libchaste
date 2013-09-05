#! /bin/sh

#This scrpit kicks off the install process, it checks that a sufficiently new 
#version of python is available assuming that it is, starts the installer script


#Try to find python or accept the first command line arg as the ptyhon path
python_dir=$(which python)

if [ $# -gt 0 ] 
then
   python_dir="$1"
fi

if [ ! -f "$python_dir" ]
then   
    echo "Could not find python in \"$python_dir\", Python is not installed."
    echo "Please install it before continuing,"
fi

cd scripts
python camio_prepare_boot.py


