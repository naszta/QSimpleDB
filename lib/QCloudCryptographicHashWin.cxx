#include "QCloudCryptographicHash.h"

#include <Windows.h>

QByteArray QCloudCryptographicHash::sha256_hash( const QByteArray &data )
{
  QByteArray retval;

  HCRYPTPROV hProv = 0;
  HCRYPTHASH hHash = 0;

  if ( ! CryptAcquireContextW( &hProv, NULL, NULL, PROV_RSA_AES, CRYPT_VERIFYCONTEXT ) )
    return retval;

  if ( ! CryptCreateHash( hProv, CALG_SHA_256, 0, 0, &hHash ) )
  {
    CryptReleaseContext(hProv, 0);
    return retval;
  }

  if( ! CryptHashData( hHash, reinterpret_cast<const BYTE *>( data.data() ), data.size(), 0 ) )
  {
    CryptDestroyHash(hHash);
    CryptReleaseContext(hProv, 0);
    return retval;
  }

  DWORD cbHashSize = 0, dwCount = sizeof(DWORD);
  if( ! CryptGetHashParam( hHash, HP_HASHSIZE, (BYTE *)&cbHashSize, &dwCount, 0 ) )
  {
    CryptDestroyHash(hHash);
    CryptReleaseContext(hProv, 0);
    return retval;
  }

  retval.resize(cbHashSize);
  if ( ! CryptGetHashParam( hHash, HP_HASHVAL, reinterpret_cast<BYTE*>( retval.data() ), &cbHashSize, 0) )
  {
    retval.resize(0);
    CryptDestroyHash(hHash);
    CryptReleaseContext(hProv, 0);
    return retval;
  }

  CryptDestroyHash(hHash);
  CryptReleaseContext(hProv, 0);
  return retval;
}
