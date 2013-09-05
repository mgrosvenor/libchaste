import os
import subprocess
import shutil
from camio_deps import *


#Since we fetch the source code from source code repositories, these are assumed to build for all platforms
#if this is not the case, platform specific repos will need to be fetched from the platform specific arch file. 

def install_dep_svn(dep):
    svn_str   = svn_deps[dep][0]
    path_str  = "../" + svn_deps[dep][1]
    build_str = svn_deps[dep][2]

    os.makedirs(path_str) 
    cmd = "svn checkout " + svn_str + " " + path_str
    print "CamIO2 Prepare: Running " + cmd
    result = subprocess.call(cmd, shell=True)
    if result:
        shutil.rmtree(path_str)
        print "CamIO2 Prepare: Could not install \"" + dep + "\" using svn"
    
    return not result


def install_dep_git(dep):
    git_str   = git_deps[dep][0]
    path_str  = "../" + git_deps[dep][1]
    build_str = git_deps[dep][2]

    os.makedirs(path_str) 
    cmd = "git clone " + git_str + " " + path_str
    print "CamIO2 Prepare: Running " + cmd
    result = subprocess.call(cmd, shell=True)
    if result:
        shutil.rmtree(path_str)
        print "CamIO2 Prepare: Could not install \"" + dep  + "\" using git"
    
    return not result



def find_repo(dep):
    path = None
    
    if dep in svn_deps:       
        path = svn_deps[dep][1]
    elif dep in git_deps:
        path = git_deps[dep][1]
     
    if path :
        return os.path.exists("../" + path)
    
    return None


def install_repo(dep):

    if dep in svn_deps:       
        return install_dep_svn(dep)
    elif dep in git_deps:
        return install_dep_git(dep)

    return None


