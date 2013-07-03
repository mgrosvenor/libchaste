/*
 * timestamp.h
 *
 *  Created on: Jul 3, 2013
 *      Author: mgrosvenor
 */

#ifndef TIMESTAMP_H_
#define TIMESTAMP_H_

#include "../types/types.h"

m6_str generate_iso_timestamp(m6_bool use_gmt, m6_word subseconds, m6_bool incl_tz_offset);

#endif /* TIMESTAMP_H_ */
