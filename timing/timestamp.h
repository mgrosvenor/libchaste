/*
 * timestamp.h
 *
 *  Created on: Jul 3, 2013
 *      Author: mgrosvenor
 */

#ifndef TIMESTAMP_H_
#define TIMESTAMP_H_

#include "../types/types.h"

ch_str generate_iso_timestamp(ch_bool use_gmt, ch_word subseconds, ch_bool incl_tz_offset);

#endif /* TIMESTAMP_H_ */
