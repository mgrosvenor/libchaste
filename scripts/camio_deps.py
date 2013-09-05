############################################################################################################################
# CAMIO2 DEPENDENCIES
# This is a central file with all of the dependency configurations for CamIO2. 
# It contains the dependency definitions as well as tools to set them up. 
# All of the hard coded constants for dependencies are in this file
############################################################################################################################

#Top level list of apps/libraries required to build CamIO2

required = [ "bash", "unzip", "make", "clang", "svn", "cake", "libbstring", "libcgen", "libchaste" ]

#List of arch specific apps required
required_linux_ubuntu = [ "dpkg", "apt", "wget" ]
required_linux_debian = [ "dpkg", "apt", "wget" ]
required_linux_fedora = [ "rpm", "yum", "wget" ]
required_darwin = [ "curl" ]
required_freebsd = [ "pkg_add", "fetch" ]

#list of apps/libraries optional to build CamIO2 "exotic" transports
optional = ["libnetmap", "libzeromq", "libopenssl" ]

#All arch deps rolled into one structure to make it easy to pass around
required_arch = {
                    "Ubuntu"  : required_linux_ubuntu,
                    "debian"  : required_linux_debian,
                    "Fedora"  : required_linux_fedora,
                    "Darwin"  : required_darwin,
                    "FreeBSD" : required_freebsd,
                }


############################################################################################################################


#Translation from local package name to the name used by RPM (Fedora/Centos)
rpm_deps    = {
                "make":     "make",
                "clang":    "clang",
                "svn":      "subversion",
                "wget":     "wget",
                "unzip":    "unzip",
                "yum" :     "yum",
                "bash":     "bash",
              }


#Translation from local package name to the name used by APT (Ubuntu/Debian)
apt_deps    = {
                "make":     "make",
                "clang":    "clang",
                "svn":      "subversion",
                "apt":      "apt",
                "wget":     "wget",
                "unzip":    "unzip",
                "bash":     "bash",
              }

#Translation from local package name to the name used by pkg_add (FreeBSD)
pkg_deps  = {
                "make":     "make",
                "clang":    "clang",
                "svn":      "subversion",
                "apt":      "apt",
                "fetch":    "fetch",
                "unzip" :   "unzip",
                "bash":     "bash",
              }


############################################################################################################################
#Since we fetch the source code from source code repositories, these are assumed to use the same tools for all platforms
#if this is not the case, platform specific repos will need to be fetched from the platform specific arch file. 

#Translation from local packge name to the (svn path, install path, bash build script)
svn_deps    = {
                "libcgen"    : ("http://c-generic-library.googlecode.com/svn/trunk/", "deps/libcgen", None),
              }

#Translation from the local package name to the (git path, install path, bash build script)
git_deps    = {
                "cake"  : ("https://github.com/Zomojo/Cake.git", "deps/cake", None),
                "libchaste" : ("https://github.com/mgrosvenor/libchaste.git", "deps/libchaste", None),
              }


############################################################################################################################
# Tools for dealing with http/zip/tar based tools. 

def linux_wget_script( url, destination):
    return  "wget " + "-O " + destination + " " + url

def linux_unzip_script( source, destination):
    return  "unzip " + source + " -d " + destination


############################################################################################################################
#Libbstring

libbstring_url = "http://sourceforge.net/projects/bstring/files/bstring/rc31/bstrlib-05122010.zip/download"
libbstring_zip_dest = "deps/libbstring/libbstring.zip"
libbstring_dest = "deps/libbstring/"

libbstring_linux_script = linux_wget_script( libbstring_url, libbstring_zip_dest) + " && " + \
                          linux_unzip_script( libbstring_zip_dest, libbstring_dest) + "\n"
libbstring_freebsd_script = ""
libbstring_darwin_script  = ""

############################################################################################################################


#script based dependencies (for those that don't use apt-get/dpkg/svn/git or some other standard tool)
#(script for install, destination directory)

scripts_linux = {
                    "libbstring" : ( libbstring_linux_script, libbstring_dest),
                }

scripts_freebsd = {
                    "libbstring" : ( libbstring_freebsd_script, libbstring_dest),
                  }

scripts_darwin = {
                    "libbstring" : ( libbstring_darwin_script, libbstring_dest),
                 }


############################################################################################################################




