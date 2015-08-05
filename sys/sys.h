/*
 * sys.h
 *
 *  Created on: Apr 23, 2015
 *      Author: mgrosvenor
 */

#ifndef SYS_H_
#define SYS_H_

#ifdef __APPLE__
#   include "sys_apple.h"
#else 
#   include "sys_linux.h"
#endif
//#else
//#   warning "LibChaste Needs to be ported to this system"
//#endif


#endif /* SYS_H_ */
