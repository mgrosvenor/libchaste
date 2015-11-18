#!/bin/sh

# All documentation should either go in the cake source file as part of the --help option 
# or in the help2man.include file.  This script then will parse that information to 
# create the manual page and the README file.  
# Do not edit cake.1 or README explicilty. Let this script create them.

help2man --output=cake.1 --no-info --include=help2man.include ./cake
nroff -c -mandoc cake.1 | col -b > README

