/*
 * camio_ring.h
 *
 *  Created on: Mar 26, 2013
 *      Author: mgrosvenor
 */

#ifndef CAMIO_RING_H_
#define CAMIO_RING_H_

#define CAMIO_RING_SLOT_COUNT (1024)
#define CAMIO_RING_SLOT_SIZE (4 * 1024)  //4K
#define CAMIO_RING_SLOT_AVAIL (CAMIO_RING_SLOT_SIZE - sizeof(uint64_t) * 2)
#define CAMIO_RING_MEM_SIZE ( CAMIO_RING_SLOT_COUNT * CAMIO_RING_SLOT_SIZE + sizeof(uint64_t))


#define CHECK_LEN_OK(len) \
    if(len > CAMIO_RING_SLOT_AVAIL){ \
        eprintf_exit("Length supplied (%lu) is greater than slot size (%lu, corruption is likely.\n", len, CAMIO_RING_SLOT_AVAIL ); \
    }


#define ring_connected (*(volatile uint64_t*)(priv->ring + CAMIO_RING_SLOT_COUNT * CAMIO_RING_SLOT_SIZE))

#endif /* CAMIO_RING_H_ */
