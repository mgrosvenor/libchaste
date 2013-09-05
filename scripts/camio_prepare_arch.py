#A platform neutral detection mechanism for the platform
#this will spit out the search and install functions for any given platform

import platform
import sys
import os

from camio_deps import *


#Uname examples
# Fedora 14:
# uname() = ('Linux', 'nf-test109.cl.cam.ac.uk', '2.6.35.14-106.fc14.x86_64', '#1 SMP Wed Nov 23 13:07:52 UTC 2011', 'x86_64', 'x86_64')
# linux_distribution() = ('Fedora', '14', 'Laughlin')
#
# MacOS X 10.8:
# uname() = ('Darwin', 'bumblebee.cl.cam.ac.uk', '12.4.0', 'Darwin Kernel Version 12.4.0: Wed May  1 17:57:12 PDT 2013; root:xnu-2050.24.15~1/RELEASE_X86_64', 'x86_64', 'i386')
# mac_ver() = ('10.8.4', ('', '', ''), 'x86_64')
#
# Debian 6:
# uname() = ('Linux', '(none)', '2..32-5-686', '#1 SMP Sun Sep 23 09:49:36 UTC 2012', 'i686', '')
# linux_distribution = ('debian', '6.0.6', '')
# 
# Ubuntu 12.04:
# uname() = ('Linux', 'ubuntu', '3.2.0-38-generic', '#60-Ubuntu SMP Wed Feb 13 13:22:43 UTC 2013', 'x86_64', 'x86_64') 
# linux_distrubtion() = ('Ubuntu', '12.04', 'precise')
#
# Ubuntu 13.04:
# uname() = ('Linux', 'ubuntu', '3.8.0-25-generic', '#37-Ubuntu SMP Thu Jun 6 20:47:07 UTC 2013', 'x86_64', 'x86_64')
# linux_distribution = ('Ubuntu', '13.04', 'raring')
#
# FreeBSD 9.1:
# ('FreeBSD', 'pcbsd-7337', '9.1-RELEASE', 'FreeBSD 9.1-RELEASE #2: Tue Nov 27 03:45:16 UTC 2012     root@darkstar:/usr/obj/pcbsd-build90/fbsd-source/9.1/sys/GENERIC', 'amd64', 'amd64')


#Figures out what platform we are on
def platform_install():

    #Gather some platform stats
    ostype   = os.name
    uname    = platform.uname()
    system   = uname[0]
    hostname = uname[1]
    version  = uname[2]
    build    = uname[3]
    cpu      = uname[4]
    arch     = uname[5]

    #Figure out what OS we're using, make sure it's posix
    if(ostype != "posix"):
        print "CamIO2 does not support Operating Systems other than POSIX. Your operating system is \"" + os_type + "\"."
        return

    #Figure out what OS we're using
    if(system == "Linux"):
        print "CamIO2 Prepare: Detected system is running \"" + system + "\"..."
        import camio_prepare_arch_linux
        distro = platform.linux_distribution()
        return camio_prepare_arch_linux.install(uname,distro)

    if(system == "Darwin"):
        print "CamIO2 Prepare: Detected system is running \"" + system + "\"..."
        import camio_prepare_arch_darwin
        mac_ver = platform.mac_ver()
        return camio_prepare_arch_darwin.install(uname,mac_ver)

    if(system == "FreeBSD"):
        print "CamIO2 Prepare: Detected system is running \"" + system + "\"..."
        import camio_prepare_arch_freebsd
        return camio_prepare_arch_freebsd.install(uname )
   
    print "CamIO2 Prepare: Could not detect Operating System. Expected to find, Linux, FreeBSD or Darwin (MacOS X), but instead found \"" + system + "\"."
    print "CamIO2 Prepare: Fatal Error! Exiting now."
    sys.exit(-1)



