#include "QCloudCryptographicHash.h"

#define UNROLL_LOOPS 1

#define SHA256_DIGEST_SIZE 32
#define SHA256_BLOCK_SIZE  64

inline void pack32( const unsigned char * src, quint32 &trg )
{
  trg = static_cast<quint32>(src[3]) |
    ( static_cast<quint32>(src[2]) <<  8 ) |
    ( static_cast<quint32>(src[1]) << 16 ) |
    ( static_cast<quint32>(src[0]) << 24 );
}

inline void unpack32( const quint32 &src, unsigned char * trg )
{
  trg[3] = static_cast<unsigned char>( src & 0xFF );
  trg[2] = static_cast<unsigned char>( ( src >>  8 ) & 0xFF );
  trg[1] = static_cast<unsigned char>( ( src >> 16 ) & 0xFF );
  trg[0] = static_cast<unsigned char>( ( src >> 24 ) & 0xFF );
}

inline quint32 sha256_ch( const quint32 &x, const quint32 &y, const quint32 &z )
{
  return ( ( x & y ) ^ ( ~x & z ) );
}

inline quint32 sha256_maj( const quint32 &x, const quint32 &y, const quint32 &z )
{
  return ( ( x & y ) ^ ( x & z ) ^ ( y & z ) );
}

inline quint32 shift_right( const quint32 &x, int n )
{
  return (x >> n);
}

inline quint32 rotate_right( const quint32 &x, int n )
{
  return ( ( x >> n ) | ( x << ( ( sizeof(quint32) << 3 ) - n ) ) );
}

inline quint32 sha256_f1( const quint32 &x )
{
  return rotate_right( x,  2 ) ^ rotate_right( x, 13 ) ^ rotate_right( x, 22 );
}

inline quint32 sha256_f2( const quint32 &x )
{
  return rotate_right( x,  6 ) ^ rotate_right( x, 11 ) ^ rotate_right( x, 25 );
}

inline quint32 sha256_f3( const quint32 &x )
{
  return rotate_right( x,  7 ) ^ rotate_right( x, 18 ) ^ shift_right( x, 3 );
}

inline quint32 sha256_f4( const quint32 &x )
{
  return rotate_right( x, 17 ) ^ rotate_right( x, 19 ) ^ shift_right( x, 10 );
}


#ifdef UNROLL_LOOPS
#define SHA256_SCR(i)                         \
{                                             \
  w[i] = sha256_f4(w[i-2]) + w[i-7] + sha256_f3(w[i-15]) + w[i-16]; \
}
#define SHA256_EXP(a, b, c, d, e, f, g, h, j)               \
{                                                           \
  t1 = wv[h] + sha256_f2(wv[e]) + sha256_ch(wv[e], wv[f], wv[g]) + sha256_k[j] + w[j]; \
  t2 = sha256_f1(wv[a]) + sha256_maj(wv[a], wv[b], wv[c]);  \
  wv[d] += t1;                                              \
  wv[h] = t1 + t2;                                          \
}
#endif

quint32 sha256_k[64] = {
  0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5,
  0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5,
  0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3,
  0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174,
  0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc,
  0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da,
  0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7,
  0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967,
  0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13,
  0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85,
  0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3,
  0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070,
  0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5,
  0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3,
  0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208,
  0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2
};

class sha256struct
{
public:
  sha256struct( void )
    : tot_len(0), len(0)
  {
    this->h[0] = 0x6a09e667;
    this->h[1] = 0xbb67ae85;
    this->h[2] = 0x3c6ef372;
    this->h[3] = 0xa54ff53a;
    this->h[4] = 0x510e527f;
    this->h[5] = 0x9b05688c;
    this->h[6] = 0x1f83d9ab;
    this->h[7] = 0x5be0cd19;
  };
  void addData( const unsigned char * message, unsigned int len )
  {
    unsigned int block_nb;
    unsigned int new_len, rem_len, tmp_len;
    const unsigned char * shifted_message;

    tmp_len = SHA256_BLOCK_SIZE - this->len;
    rem_len = len < tmp_len ? len : tmp_len;

    memcpy(&this->block[this->len], message, rem_len);

    if (this->len + len < SHA256_BLOCK_SIZE)
    {
      this->len += len;
      return;
    }

    new_len = len - rem_len;
    block_nb = new_len / SHA256_BLOCK_SIZE;

    shifted_message = message + rem_len;

    sha256_transf( this->block, 1 );
    sha256_transf( shifted_message, block_nb );

    rem_len = new_len % SHA256_BLOCK_SIZE;

    memcpy(this->block, &shifted_message[block_nb << 6],
      rem_len);

    this->len = rem_len;
    this->tot_len += (block_nb + 1) << 6;
  };
  void saveResult( unsigned char * digest )
  {
    unsigned int block_nb;
    unsigned int pm_len;
    unsigned int len_b;

#ifndef UNROLL_LOOPS
    int i;
#endif

    block_nb = (1 + ((SHA256_BLOCK_SIZE - 9)
      < (this->len % SHA256_BLOCK_SIZE)));

    len_b = (this->tot_len + this->len) << 3;
    pm_len = block_nb << 6;

    memset(this->block + this->len, 0, pm_len - this->len);
    this->block[this->len] = 0x80;
    unpack32(len_b, this->block + pm_len - 4);

    this->sha256_transf( this->block, block_nb );

#ifndef UNROLL_LOOPS
    for (i = 0 ; i < 8; i++) {
      unpack32(this->h[i], &digest[i << 2]);
    }
#else
    unpack32(this->h[0], &digest[ 0]);
    unpack32(this->h[1], &digest[ 4]);
    unpack32(this->h[2], &digest[ 8]);
    unpack32(this->h[3], &digest[12]);
    unpack32(this->h[4], &digest[16]);
    unpack32(this->h[5], &digest[20]);
    unpack32(this->h[6], &digest[24]);
    unpack32(this->h[7], &digest[28]);
#endif /* !UNROLL_LOOPS */
  };
private:
  void sha256_transf( const unsigned char * message, unsigned int block_nb )
  {
    quint32 w[64];
    quint32 wv[8];
    quint32 t1, t2;
    const unsigned char *sub_block;
    int i;

#ifndef UNROLL_LOOPS
    int j;
#endif

    for ( i = 0; i < static_cast<int>(block_nb); ++i )
    {
      sub_block = message + (i << 6);

#ifndef UNROLL_LOOPS
      for ( j = 0; j < 16; ++j )
      {
        pack32(&sub_block[j << 2], w[j]);
      }

      for ( j = 16; j < 64; ++j )
      {
        w[j] = sha256_f4(w[j-2]) + w[j-7] + sha256_f3(w[j-15]) + w[j-16];
      }

      for ( j = 0; j < 8; ++j )
      {
        wv[j] = this->h[j];
      }

      for ( j = 0; j < 64; ++j )
      {
        t1 = wv[7] + sha256_f2(wv[4]) + sha256_ch(wv[4], wv[5], wv[6]) + sha256_k[j] + w[j];
        t2 = sha256_f1(wv[0]) + sha256_maj(wv[0], wv[1], wv[2]);
        wv[7] = wv[6];
        wv[6] = wv[5];
        wv[5] = wv[4];
        wv[4] = wv[3] + t1;
        wv[3] = wv[2];
        wv[2] = wv[1];
        wv[1] = wv[0];
        wv[0] = t1 + t2;
      }

      for ( j = 0; j < 8; ++j )
      {
        this->h[j] += wv[j];
      }
#else
      pack32(&sub_block[ 0], w[ 0]); pack32(&sub_block[ 4], w[ 1]);
      pack32(&sub_block[ 8], w[ 2]); pack32(&sub_block[12], w[ 3]);
      pack32(&sub_block[16], w[ 4]); pack32(&sub_block[20], w[ 5]);
      pack32(&sub_block[24], w[ 6]); pack32(&sub_block[28], w[ 7]);
      pack32(&sub_block[32], w[ 8]); pack32(&sub_block[36], w[ 9]);
      pack32(&sub_block[40], w[10]); pack32(&sub_block[44], w[11]);
      pack32(&sub_block[48], w[12]); pack32(&sub_block[52], w[13]);
      pack32(&sub_block[56], w[14]); pack32(&sub_block[60], w[15]);

      SHA256_SCR(16); SHA256_SCR(17); SHA256_SCR(18); SHA256_SCR(19);
      SHA256_SCR(20); SHA256_SCR(21); SHA256_SCR(22); SHA256_SCR(23);
      SHA256_SCR(24); SHA256_SCR(25); SHA256_SCR(26); SHA256_SCR(27);
      SHA256_SCR(28); SHA256_SCR(29); SHA256_SCR(30); SHA256_SCR(31);
      SHA256_SCR(32); SHA256_SCR(33); SHA256_SCR(34); SHA256_SCR(35);
      SHA256_SCR(36); SHA256_SCR(37); SHA256_SCR(38); SHA256_SCR(39);
      SHA256_SCR(40); SHA256_SCR(41); SHA256_SCR(42); SHA256_SCR(43);
      SHA256_SCR(44); SHA256_SCR(45); SHA256_SCR(46); SHA256_SCR(47);
      SHA256_SCR(48); SHA256_SCR(49); SHA256_SCR(50); SHA256_SCR(51);
      SHA256_SCR(52); SHA256_SCR(53); SHA256_SCR(54); SHA256_SCR(55);
      SHA256_SCR(56); SHA256_SCR(57); SHA256_SCR(58); SHA256_SCR(59);
      SHA256_SCR(60); SHA256_SCR(61); SHA256_SCR(62); SHA256_SCR(63);

      wv[0] = this->h[0]; wv[1] = this->h[1];
      wv[2] = this->h[2]; wv[3] = this->h[3];
      wv[4] = this->h[4]; wv[5] = this->h[5];
      wv[6] = this->h[6]; wv[7] = this->h[7];

      SHA256_EXP(0,1,2,3,4,5,6,7, 0); SHA256_EXP(7,0,1,2,3,4,5,6, 1);
      SHA256_EXP(6,7,0,1,2,3,4,5, 2); SHA256_EXP(5,6,7,0,1,2,3,4, 3);
      SHA256_EXP(4,5,6,7,0,1,2,3, 4); SHA256_EXP(3,4,5,6,7,0,1,2, 5);
      SHA256_EXP(2,3,4,5,6,7,0,1, 6); SHA256_EXP(1,2,3,4,5,6,7,0, 7);
      SHA256_EXP(0,1,2,3,4,5,6,7, 8); SHA256_EXP(7,0,1,2,3,4,5,6, 9);
      SHA256_EXP(6,7,0,1,2,3,4,5,10); SHA256_EXP(5,6,7,0,1,2,3,4,11);
      SHA256_EXP(4,5,6,7,0,1,2,3,12); SHA256_EXP(3,4,5,6,7,0,1,2,13);
      SHA256_EXP(2,3,4,5,6,7,0,1,14); SHA256_EXP(1,2,3,4,5,6,7,0,15);
      SHA256_EXP(0,1,2,3,4,5,6,7,16); SHA256_EXP(7,0,1,2,3,4,5,6,17);
      SHA256_EXP(6,7,0,1,2,3,4,5,18); SHA256_EXP(5,6,7,0,1,2,3,4,19);
      SHA256_EXP(4,5,6,7,0,1,2,3,20); SHA256_EXP(3,4,5,6,7,0,1,2,21);
      SHA256_EXP(2,3,4,5,6,7,0,1,22); SHA256_EXP(1,2,3,4,5,6,7,0,23);
      SHA256_EXP(0,1,2,3,4,5,6,7,24); SHA256_EXP(7,0,1,2,3,4,5,6,25);
      SHA256_EXP(6,7,0,1,2,3,4,5,26); SHA256_EXP(5,6,7,0,1,2,3,4,27);
      SHA256_EXP(4,5,6,7,0,1,2,3,28); SHA256_EXP(3,4,5,6,7,0,1,2,29);
      SHA256_EXP(2,3,4,5,6,7,0,1,30); SHA256_EXP(1,2,3,4,5,6,7,0,31);
      SHA256_EXP(0,1,2,3,4,5,6,7,32); SHA256_EXP(7,0,1,2,3,4,5,6,33);
      SHA256_EXP(6,7,0,1,2,3,4,5,34); SHA256_EXP(5,6,7,0,1,2,3,4,35);
      SHA256_EXP(4,5,6,7,0,1,2,3,36); SHA256_EXP(3,4,5,6,7,0,1,2,37);
      SHA256_EXP(2,3,4,5,6,7,0,1,38); SHA256_EXP(1,2,3,4,5,6,7,0,39);
      SHA256_EXP(0,1,2,3,4,5,6,7,40); SHA256_EXP(7,0,1,2,3,4,5,6,41);
      SHA256_EXP(6,7,0,1,2,3,4,5,42); SHA256_EXP(5,6,7,0,1,2,3,4,43);
      SHA256_EXP(4,5,6,7,0,1,2,3,44); SHA256_EXP(3,4,5,6,7,0,1,2,45);
      SHA256_EXP(2,3,4,5,6,7,0,1,46); SHA256_EXP(1,2,3,4,5,6,7,0,47);
      SHA256_EXP(0,1,2,3,4,5,6,7,48); SHA256_EXP(7,0,1,2,3,4,5,6,49);
      SHA256_EXP(6,7,0,1,2,3,4,5,50); SHA256_EXP(5,6,7,0,1,2,3,4,51);
      SHA256_EXP(4,5,6,7,0,1,2,3,52); SHA256_EXP(3,4,5,6,7,0,1,2,53);
      SHA256_EXP(2,3,4,5,6,7,0,1,54); SHA256_EXP(1,2,3,4,5,6,7,0,55);
      SHA256_EXP(0,1,2,3,4,5,6,7,56); SHA256_EXP(7,0,1,2,3,4,5,6,57);
      SHA256_EXP(6,7,0,1,2,3,4,5,58); SHA256_EXP(5,6,7,0,1,2,3,4,59);
      SHA256_EXP(4,5,6,7,0,1,2,3,60); SHA256_EXP(3,4,5,6,7,0,1,2,61);
      SHA256_EXP(2,3,4,5,6,7,0,1,62); SHA256_EXP(1,2,3,4,5,6,7,0,63);

      this->h[0] += wv[0]; this->h[1] += wv[1];
      this->h[2] += wv[2]; this->h[3] += wv[3];
      this->h[4] += wv[4]; this->h[5] += wv[5];
      this->h[6] += wv[6]; this->h[7] += wv[7];
#endif /* !UNROLL_LOOPS */
    }
  };
  unsigned int tot_len;
  unsigned int len;
  unsigned char block[2 * SHA256_BLOCK_SIZE];
  quint32 h[8];
};

QByteArray QCloudCryptographicHash::sha256_hash( const QByteArray &data )
{
  QByteArray retval;
  retval.resize(SHA256_DIGEST_SIZE);

  sha256struct sha256data;

  sha256data.addData( reinterpret_cast<const unsigned char *>( data.data() ), data.size() );

  sha256data.saveResult( reinterpret_cast<unsigned char *>( retval.data() ) );

  return retval;
}
