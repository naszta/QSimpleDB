#include "QHmacSha1.h"

#include <QCryptographicHash>
#include "QCloudCryptographicHash.h"

QHmacSha1::QHmacSha1( const QByteArray &password, QObject * parent )
  : QObject( parent )
{

}

QHmacSha1::~QHmacSha1( void )
{

}

QByteArray QHmacSha1::CalcHmacSha1( const QByteArray &baseString, const QByteArray &key )
{
  int blockSize = 64; // HMAC-SHA-1 block size, defined in SHA-1 standard
  QByteArray local_key(key);
  if (local_key.length() > blockSize)
  { // if local_key is longer than block size (64), reduce local_key length with SHA-1 compression
    local_key = QCryptographicHash::hash(local_key, QCryptographicHash::Sha1);
  }
  QByteArray innerPadding(blockSize, char(0x36)); // initialize inner padding with char "6"
  QByteArray outerPadding(blockSize, char(0x5c)); // initialize outer padding with char "\"
  // ascii characters 0x36 ("6") and 0x5c ("\") are selected because they have large 
  // Hamming distance (http://en.wikipedia.org/wiki/Hamming_distance)
  for ( int i = 0; i < local_key.length(); ++i)
  {
    innerPadding[i] = innerPadding[i] ^ local_key.at(i); // XOR operation between every byte in local_key and innerpadding, of local_key length
    outerPadding[i] = outerPadding[i] ^ local_key.at(i); // XOR operation between every byte in local_key and outerpadding, of local_key length
  }
  // result = hash ( outerPadding CONCAT hash ( innerPadding CONCAT baseString ) ).toBase64
  QByteArray total = outerPadding;
  QByteArray part = innerPadding;

  part.append(baseString);
  total.append(QCryptographicHash::hash(part, QCryptographicHash::Sha1));
  QByteArray hashed = QCryptographicHash::hash(total, QCryptographicHash::Sha1);
  return hashed;
}

QByteArray QHmacSha1::CalcHmacSha256( const QByteArray &baseString, const QByteArray &key )
{
  int blockSize = 64; // HMAC-SHA-1 block size, defined in SHA-1 standard
  QByteArray local_key(key);
  if (local_key.length() > blockSize)
  { // if local_key is longer than block size (64), reduce local_key length with SHA-1 compression
    local_key = QCloudCryptographicHash::sha256_hash(local_key);
  }
  QByteArray innerPadding(blockSize, char(0x36)); // initialize inner padding with char "6"
  QByteArray outerPadding(blockSize, char(0x5c)); // initialize outer padding with char "\"
  // ascii characters 0x36 ("6") and 0x5c ("\") are selected because they have large 
  // Hamming distance (http://en.wikipedia.org/wiki/Hamming_distance)
  for ( int i = 0; i < local_key.length(); ++i)
  {
    innerPadding[i] = innerPadding[i] ^ local_key.at(i); // XOR operation between every byte in local_key and innerpadding, of local_key length
    outerPadding[i] = outerPadding[i] ^ local_key.at(i); // XOR operation between every byte in local_key and outerpadding, of local_key length
  }
  // result = hash ( outerPadding CONCAT hash ( innerPadding CONCAT baseString ) ).toBase64
  QByteArray total = outerPadding;
  QByteArray part = innerPadding;

  part.append(baseString);
  total.append(QCloudCryptographicHash::sha256_hash(part));
  QByteArray hashed = QCloudCryptographicHash::sha256_hash(total);
  return hashed;
}
