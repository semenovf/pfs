#ifndef __CWT_CRYPTO_HPP__
#define __CWT_CRYPTO_HPP__
 
#include <cwt/cwt.hpp>
#include <cwt/bytearray.hpp>
#include <cwt/string.hpp>

CWT_NS_BEGIN

class DLL_API Crypto
{
	enum CryptoType {
		Crypto_Block_AES
	};

public:
	Crypto(CryptoType type = Crypto_Block_AES) : m_type(type) { }

	Crypto(const Crypto &other) : m_type(other.m_type) { }
	Crypto & operator = (const Crypto &other) { m_type = other.m_type; return *this; }

	ByteArray encrypt(const ByteArray & data, const ByteArray & passphrase = ByteArray()) const;
/*
	ByteArray encrypt(const ByteArray & key, const String & data) const;
	bool decrypt(const ByteArray & key, const ByteArray & encrypted, ByteArray & decrypted) const;
	bool decrypt(const ByteArray & key, const ByteArray & encrypted, String & decrypted) const;
*/

private:
	CryptoType m_type;
};

CWT_NS_END

#endif /* __CWT_CRYPTO_HPP__ */
