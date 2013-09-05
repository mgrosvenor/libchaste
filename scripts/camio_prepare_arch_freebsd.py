import subprocess
import sys
import os
import shutil

from camio_deps import *
from camio_prepare_repo_deps import *


############################################################################################################################
#Use freebsd pkg tools to manage packages

def check_pkg():
    print "CamIO2 Prepare: check_pkg not implemented!"
    return True 
    sys.exit(-1)
    
    #cmd = "dpkg --version"
    result = subprocess.call(cmd, shell=True, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
    if result:
        print "CamIO2 Prepare: Could not find \"dpkg\" which is necessary for running CamIO2 Prepare on Ubuntu" 
        print "CamIO2 Prepare: Fatal Error! Exiting now."
        sys.exit(1)

    return not result 


def find_pkg(dep):
    print "CamIO2 Prepare: find_pkg not implemented!"
    #sys.exit(-1)
 
    #This one is special since we use it to figure out the rest...
    if dep == "pkg":
        return check_pkg()

    if dep not in pkg_deps:
        return None

    return True
    #cmd = "dpkg -s " + apt_deps[dep] + " | grep \"Status: install ok installed\""
    result = subprocess.call(cmd, shell=True, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
    return not result #subprocess returns 0 on sucess, 1 on failure


def install_pkg(dep):
    print "CamIO2 Prepare: install_pkg not implemented"
    #sys.exit(-1)

    if dep not in pkg_deps:
        return None

    deppkg = pkg_deps[dep]
    
    return True
    
    #cmd = "sudo apt-get -y install " + deppkg 
    print "                Running \"" + cmd + "\""
    result = subprocess.call(cmd, shell=True)
    return not result #subprocess returns 0 on sucess, 1 on failure

#############################################################################################################################
#Use a script to install dependencies

def find_script(dep):
    path = None

    if dep in scripts_linux:
        path = scripts_linux[dep][1]

    if path :
        return os.path.exists("../" + path)

    return None


def install_script(dep):
    if dep not in scripts_freebsd:
        return None

    depscript = scripts_freebsd[dep]
    cmd = depscript[0]
    path = depscript[1]

    os.makedirs("../" + path)

    #Convenience for script writers
    curr_dir = os.getcwd()
    os.chdir("../")

    print cmd
    result = subprocess.call(cmd, shell=True)

    os.chdir(curr_dir)

    if result:
        shutil.rmtree("../" + path)

    return not result #subprocess returns 0 on sucess, 1 on failure



############################################################################################################################
# Generic installer for freebsd systems
# Used "pman" as a generic package manager. This may be pkg or pkgng or something else

def install_error(dep,result):
        if result == None: #No valid installer for this
            print "\nCamIO2 Prepare: The depency \"" + dep + "\" cannot be resolved.\n"\
                  "                The dependency does not exist in the list of known package, repository or script based"\
                  " resources.\n"\
                  "                Installation failed.\n"\
                  "                This is a CamIO2 Preapre internal error."


        if result == False: #Tried to install but failed
            print "\nCamIO2 Prepare: Could not install dependency \"" + dep + "\"\n"\
                  "The installation script failed."


        print "CamIO2 Preapre: Fatal Error! Exiting now."
        sys.exit(-1)


def install_platform(required, optional, pman_name, find_pman, install_pman):

    #Figure out what needs to be installed and do it
    for req in required:
        sys.stdout.write("CamIO2 Prepare: Looking for " + req + "...")

        #Try using pman to find requirement, if it returns false, the requirement belongs to pman, but is not installed
        result = find_pman(req)
        if result == True: #The package is managed by pman, and is installed
            sys.stdout.write(" Found package.\n")
            continue
        elif result == False: #The package is managed by pman, but is not installed
            #Try to install using pman
            sys.stdout.write(" Installing using %s...\n" % pman_name)
            result = install_pman(req)
            if result == True: #Winner, the package is now installed
                continue
            else: #Failure. Give up. 
                install_error(req, result)


        #Try using git/svn/... to find requirement, if it returns false, the requirement belongs to a repo, but is not installed
        result = find_repo(req)
        if result == True: #The requirement is managed by a repo, and is installed
            sys.stdout.write(" Found repo.\n")
            continue
        elif result == False: #The requiement is managed by a repo, but is not installed
            #Try to install using apt
            sys.stdout.write(" Installing using repo...\n")
            result = install_repo(req)
            if result == True: #Winner, the requirement is now installed
                continue
            else: #Failure. Give up. 
                install_error(req, result)


        #Try using a script to find requirement, if it returns false, the requirement belongs to a script, but is not installed
        result = find_script(req)
        if result == True: #The requirement is managed by a script, and is installed
            sys.stdout.write(" Found script.\n")
            continue
        if result == False: #The requiement is managed by a script, but is not installed
            #Try to install using script
            sys.stdout.write(" Installing using script...\n")
            result = install_script(req)
            if result == True: #Winner, the requirement is now installed
                continue
            else: #Failure. Give up. 
                install_error(req, result)


        #Tried everything we could think of, give up now. 
        install_error(req, result)


############################################################################################################################

def install(uname):
    install_platform(required, optional, "pkg", find_pkg, install_pkg)

    print "CamIO2 Prepare: install_freebsd not implemented"
    print "CamIO2 Prepare: Fatal Error. Exiting now."
    sys.exit(-1)
