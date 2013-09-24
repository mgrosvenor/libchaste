/*
 * spooky_hash.h
 *
 *  Created on: Sep 23, 2013
 *      Author: mgrosvenor
 */

#ifndef SPOOKY_HASH_H_
#define SPOOKY_HASH_H_

/*
 * This is a port of Bob Jenkins Spooky Hash from C++ to C. Port by M. P. Grosvenor.
 * http://burtleburtle.net/bob/hash/spooky.html
 */

// SpookyHash: a 128-bit noncryptographic hash function
// By Bob Jenkins, public domain
//   Oct 31 2010: alpha, framework + SpookyHash::Mix appears right
//   Oct 31 2011: alpha again, Mix only good to 2^^69 but rest appears right
//   Dec 31 2011: beta, improved Mix, tested it for 2-bit deltas
//   Feb  2 2012: production, same bits as beta
//   Feb  5 2012: adjusted definitions of uint* to be more portable
//   Mar 30 2012: 3 bytes/cycle, not 4.  Alpha was 4 but wasn't thorough enough.
//   August 5 2012: SpookyV2 (different results)
//
// Up to 3 bytes/cycle for long messages.  Reasonably fast for short messages.
// All 1 or 2 bit deltas achieve avalanche within 1% bias per output bit.
//
// This was developed for and tested on 64-bit x86-compatible processors.
// It assumes the processor is little-endian.  There is a macro
// controlling whether unaligned reads are allowed (by default they are).
// This should be an equally good hash on big-endian machines, but it will
// compute different results on them than on little-endian machines.
//
// Google's CityHash has similar specs to SpookyHash, and CityHash is faster
// on new Intel boxes.  MD4 and MD5 also have similar specs, but they are orders
// of magnitude slower.  CRCs are two or more times slower, but unlike
// SpookyHash, they have nice math for combining the CRCs of pieces to form
// the CRCs of wholes.  There are also cryptographic hashes, but those are even
// slower than MD5.
//

#include <stddef.h>
#include <stdint.h>

#define INLINE inline
typedef  uint64_t  uint64;
typedef  uint32_t  uint32;
typedef  uint16_t  uint16;
typedef  uint8_t   uint8;


#define sc_numVars 12
#define sc_blockSize (sc_numVars * 8)
#define sc_bufSize (sc_blockSize * 2)
#define sc_const ( 0xdeadb0efdeadbeefLL )
#define SPOOKY_HASH_INIT_STATE = { .sc_numVars = sc_numVars, .sc_blockSize = sc_blockSize, .sc_bufSize = sc_bufSize, .sc_const = sc_const, 0 }


typedef struct {
    uint64 m_data[2*sc_numVars];   // unhashed data, for partial messages
    uint64 m_state[sc_numVars];  // internal state of the hash
    size_t m_length;             // total length of the input so far
    uint8  m_remainder;          // length of unhashed data stashed in m_data
} spooky_hash_state;



//
// SpookyHash: hash a single message in one call, produce 128-bit output
//
// message to hash
// length of message in bytes
// in/out: in seed 1, out hash value 1
// in/out: in seed 2, out hash value 2
void spooky_Hash128( const void *message, size_t length, uint64 *hash1, uint64 *hash2);
uint64 spooky_Hash64( const void *message, size_t length, uint64 seed);
uint32 spooky_Hash32( const void *message,  size_t length,  uint32 seed);

//
// Init: initialize the context of a SpookyHash
//
// any 64-bit value will do, including 0
// different seeds produce independent hashes
void spooky_Init(spooky_hash_state* s,  uint64 seed1, uint64 seed2);

//
// Update: add a piece of a message to a SpookyHash state
//
// message fragment
// length of message fragment in bytes
void spooky_Update(spooky_hash_state* s,  const void *message,  size_t length);


//
// Final: compute the hash for the current SpookyHash state
//
// This does not modify the state; you can keep updating it afterward
//
// The result is the same as if SpookyHash() had been called with
// all the pieces concatenated into one message.
//
// out only: first 64 bits of hash value.
// out only: second 64 bits of hash value.
void spooky_Final(spooky_hash_state* s, uint64 *hash1, uint64 *hash2);




#endif /* SPOOKY_HASH_H_ */
