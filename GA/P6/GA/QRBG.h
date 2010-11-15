/******************************************************************************
*
* Declaration of the QRBG Service Access class. (For definition see: QRBG.cpp 
* file)
*
* Designed and written by Radomir Stevanovic, Jan/2007.
* Developed in Rudjer Boskovic Institute, Zagreb, Croatia.
* Contact: stevie@ieee.org.
*
* Last revision: 2007-07-19
* Version: 0.31
*
* NOTE: QRBG code compiles on both Linux and Windows machines (tested with 
*       gcc/g++ 3.2.2 and Microsoft Visual C++ 8.0)
*
*****************************************************************************
*
* Copyright (c) 2007 Radomir Stevanovic and Rudjer Boskovic Institute.
*
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in
* all copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
* THE SOFTWARE.
*
*/

#pragma once
#pragma warning( disable : 4290 )	// disable VC++ stupid warning: "C++ exception specification ignored except to indicate a function is not __declspec(nothrow)"

#include <stddef.h>		// size_t
#include <exception>	// class exception
#include <typeinfo>		// typeid

typedef unsigned char	byte;

// following defines are Visual C/C++ (Windows) and GCC/G++ (Linux) compatible, 
// on both 32-bit and 64-bit machines
typedef char		int8;
typedef short int	int16;
typedef int			int32;
typedef long long	int64;

typedef unsigned char		uint8;
typedef unsigned short int	uint16;
typedef unsigned int		uint32;
typedef unsigned long long	uint64;

// definition of a platform flag
// we support only Windows and Linux!
#ifdef WIN32
#	define PLATFORM_WIN
#else
#	define PLATFORM_LINUX
#endif

#ifdef PLATFORM_LINUX
#	include <sys/time.h>
#endif

// default service network address and port
#define QRBG_SERVICE_DEFAULT_HOSTNAME	"random.irb.hr"
#define QRBG_SERVICE_DEFAULT_PORT		1227

// when downloading small amounts of data (byte, etc.), downloaded 
// data shall be cached to minimize network overhead impact
// one can specify cache size in bytes when constructing QRBG object,
// or this default cache size shall be used
#define DEFAULT_CACHE_SIZE	4096
#define MINIMAL_CACHE_SIZE     1

using namespace std;

class QRBG {
public:
	//////////////////////////////////////////////////////////////////////////
	//
	// service communication codes
	//

	enum OperationCodes {
		GET_DATA_AUTH_PLAIN = 0x00,	// request data, authenticate using plain method
		GET_DATA_AUTH_CERT = 0x01,	// request data, authenticate using certificates
		GET_INFO_AUTH_PLAIN,
		GET_INFO_AUTH_CERT
	};

	enum ServerResponseCodes {
		OK = 0,				// everything is ok (user found, quota not exceeded), sending data
		SERVER_STOPPING,	// server is stopping (or at least it's shutting down this connection!)
		SERVER_ERROR,		// internal server error
		UNKNOWN_OP,			// client requested unknown/unsupported operation
		ILL_FORMED_REQUEST,	// client sent an ill-formed request packet
		TIMEOUT,			// timeout while receiving the request from client
		AUTH_FAILED,		// user could not be authenticated - see enum RefusalReasonCodes
		QUOTA_EXCEEDED		// user quota is (or would be exceeded) - see enum RefusalReasonCodes
	};

	static const char* ServerResponseDescription[];
	static const char* ServerResponseRemedy[];

	enum RefusalReasonCodes {
		NONE = 0x00,

		//
		// bytes per time period quotas
		//
		BYTE_QUOTA_EXCEEDED_FOR_SESSION = 0x10,	// requested to much data in one session
		BYTE_QUOTA_WOULD_EXCEED_FOR_EON,		// by serving this request, eon quota would be exceeded (request less data)
		BYTE_QUOTA_EXCEEDED_FOR_EON,			// eon quota is already exceeded
		BYTE_QUOTA_WOULD_EXCEED_FOR_YEAR,		// by serving this request, yearly quota would be exceeded (request less data)
		BYTE_QUOTA_EXCEEDED_FOR_YEAR,			// yearly quota is already exceeded
		BYTE_QUOTA_WOULD_EXCEED_FOR_MONTH,		// by serving this request, monthly quota would be exceeded (request less data)
		BYTE_QUOTA_EXCEEDED_FOR_MONTH,			// monthly quota is already exceeded
		BYTE_QUOTA_WOULD_EXCEED_FOR_DAY,		// by serving this request, daily quota would be exceeded (request less data)
		BYTE_QUOTA_EXCEEDED_FOR_DAY,			// daily quota is already exceeded

		//
		// concurrent connections quota AND connection count per time period quotas
		//
		CONCURRENT_CONNECTIONS_QUOTA_EXCEEDED = 0x20,	// maximum number of allowed parallel requests for authenticated user is already being served (wait and try again)
		CC_QUOTA_EXCEEDED_PER_MINUTE,			// user connections-per-minute limit exceeded (wait and try again)
		CC_QUOTA_EXCEEDED_PER_HOUR,				// user connections-per-hour limit exceeded (wait and try again)
		CC_QUOTA_EXCEEDED_PER_DAY,				// user connections-per-day limit exceeded (wait and try again)
		CC_QUOTA_EXCEEDED_PER_MONTH,			// user connections-per-month limit exceeded (wait and try again)
		CC_QUOTA_EXCEEDED_PER_YEAR,				// user connections-per-year limit exceeded (wait and try again)
		CC_QUOTA_EXCEEDED_PER_EON				// user connections-per-eon limit exceeded (that's all folks!)
	};


	//////////////////////////////////////////////////////////////////////////
	//
	// class interface
	//
public:

	//
	// exceptions
	//

	class InvalidArgumentError : public exception {};
	class NetworkSubsystemError : public exception {};
	class ConnectError : public exception {};
	class CommunicationError : public exception {};
	class ServiceDenied : public exception { 
	public:
		ServerResponseCodes ServerResponse; 
		RefusalReasonCodes RefusalReason;
	public:
		ServiceDenied(ServerResponseCodes response, RefusalReasonCodes reason) : ServerResponse(response), RefusalReason(reason) {}		
		const char* what() { return ServerResponseDescription[ServerResponse]; }
		const char* cure() { return ServerResponseRemedy[ServerResponse]; }
		const char* why() { return what(); }
	};


	//
	// constructor/destructor
	//

	QRBG(size_t cacheSize = DEFAULT_CACHE_SIZE) /* throw(NetworkSubsystemError, bad_alloc) */;
	~QRBG();


	//
	// initialization functions
	//

	bool defineCache(size_t cacheSize = DEFAULT_CACHE_SIZE);

	void defineServer(const char* qrbgServerAddress = QRBG_SERVICE_DEFAULT_HOSTNAME, unsigned int qrbgServerPort = QRBG_SERVICE_DEFAULT_PORT) 
		throw(InvalidArgumentError);

	void defineUser(const char* qrbgUsername, const char* qrbgPassword) 
		throw(InvalidArgumentError);

	// for future use:
	void defineUser(const char* qrbgCertificateStore) 
		throw(InvalidArgumentError);

	// maximum accepted lengths for: hostname, username and password
	enum {HOSTNAME_MAXLEN = 255, USERNAME_MAXLEN = 100, PASSWORD_MAXLEN = 100};


	//
	// data "getter" functions
	//
	
	// by type...

	byte getByte() 
		throw(ConnectError, CommunicationError, ServiceDenied);
	int getInt() 
		throw(ConnectError, CommunicationError, ServiceDenied);
	long getLongInt() 
		throw(ConnectError, CommunicationError, ServiceDenied);
	float getFloat() 
		throw(ConnectError, CommunicationError, ServiceDenied);
	double getDouble() 
		throw(ConnectError, CommunicationError, ServiceDenied);

	size_t getBytes(byte* buffer, size_t count) 
		throw(ConnectError, CommunicationError, ServiceDenied);
	size_t getInts(int* buffer, size_t count) 
		throw(ConnectError, CommunicationError, ServiceDenied);
	size_t getLongInts(long* buffer, size_t count) 
		throw(ConnectError, CommunicationError, ServiceDenied);
	size_t getFloats(float* buffer, size_t count) 
		throw(ConnectError, CommunicationError, ServiceDenied);
	size_t getDoubles(double* buffer, size_t count) 
		throw(ConnectError, CommunicationError, ServiceDenied);

	// by bitlength

	int8 getInt8() 
		throw(ConnectError, CommunicationError, ServiceDenied);
	int16 getInt16() 
		throw(ConnectError, CommunicationError, ServiceDenied);
	int32 getInt32() 
		throw(ConnectError, CommunicationError, ServiceDenied);
	int64 getInt64() 
		throw(ConnectError, CommunicationError, ServiceDenied);

	size_t getInt8s(int8* buffer, size_t count) 
		throw(ConnectError, CommunicationError, ServiceDenied);
	size_t getInt16s(int16* buffer, size_t count) 
		throw(ConnectError, CommunicationError, ServiceDenied);
	size_t getInt32s(int32* buffer, size_t count) 
		throw(ConnectError, CommunicationError, ServiceDenied);
	size_t getInt64s(int64* buffer, size_t count) 
		throw(ConnectError, CommunicationError, ServiceDenied);

	// as a template

	template<class _Type>
	_Type get() throw(ConnectError, CommunicationError, ServiceDenied) {

		if (typeid(_Type) == typeid(byte))
			return getByte();
		else if (typeid(_Type) == typeid(int))
			return getInt();
		else if (typeid(_Type) == typeid(long)) 
			return getLongInt();
		else if (typeid(_Type) == typeid(float)) 
			return getFloat();
		else if (typeid(_Type) == typeid(double)) 
			return getDouble();

		else if (typeid(_Type) == typeid(int8))
			return getInt8();
		else if (typeid(_Type) == typeid(int16))
			return getInt16();
		else if (typeid(_Type) == typeid(int32))
			return getInt32();
		else if (typeid(_Type) == typeid(int64))
			return getInt64();

		else
			return 0;
	}

	template<class _Type>
	size_t get(_Type* buffer, size_t count) throw(ConnectError, CommunicationError, ServiceDenied) {

		if (typeid(_Type) == typeid(byte)) 
			return getBytes((byte*)buffer, count);
		else if (typeid(_Type) == typeid(int)) 
			return getInts((int*)buffer, count);
		else if (typeid(_Type) == typeid(long)) 
			return getLongInts((long*)buffer, count);
		else if (typeid(_Type) == typeid(float)) 
			return getFloats((float*)buffer, count);
		else if (typeid(_Type) == typeid(double)) 
			return getDoubles((double*)buffer, count);

		else if (typeid(_Type) == typeid(int8))
			return getInt8s((int8*)buffer, count);
		else if (typeid(_Type) == typeid(int16))
			return getInt16s((int16*)buffer, count);
		else if (typeid(_Type) == typeid(int32))
			return getInt32s((int32*)buffer, count);
		else if (typeid(_Type) == typeid(int64))
			return getInt64s((int64*)buffer, count);

		else 
			return 0;
	}


	//
	// info functions
	//

	// returns duration (in sec) of serving the last request for data
	double getLastDownloadDuration();


	//////////////////////////////////////////////////////////////////////////
	// private methods
	//
private:
	// defines service address
	char szHostname[HOSTNAME_MAXLEN + 1];
	unsigned int port;

	// defines user
	char szUsername[USERNAME_MAXLEN + 1];
	char szPassword[PASSWORD_MAXLEN + 1];

	// socket handle used with socket system calls
	int hSocket;

	// temporary buffer used only during service request prep. and req. sending 
	// (declared class wide for speed optimization)
	byte* outBuffer;
	const size_t outBufferSize;
	
	// inBuffer is used to cache bytes for user's non-bulk data requests (getByte, getInt...)
	byte* inBuffer;
	size_t inBufferSize;
	size_t inBufferNextElemIdx;	

	// temporary variables for timing measurements, platform independent
#	if defined(PLATFORM_WIN)
	unsigned long timeStart, timeEnd;
#	elif defined(PLATFORM_LINUX)
	struct timeval timeStart, timeEnd;
#	endif

	// connects to the service server (which is defined with defineServer(...) public method)
	void Connect() 
		throw(ConnectError);
	
	// closes connection with the service server (if connection was established)
	void Close() 
		throw();
	
	// fills the 'buffer' with 'count' bytes either from local cache, or from QRBG remote service
	size_t AcquireBytes(byte* buffer, size_t count) 
		throw(ConnectError, CommunicationError, ServiceDenied);

	// copies 'count' bytes from local cache to 'buffer'
	size_t AcquireBytesFromCache(byte* buffer, size_t count) 
		throw();

	// sends the request to the Service for 'count' bytes and fills 'buffer' with returned data (random bytes)
	// it authenticates using credentials defined with defineUser(...) public method
	size_t AcquireBytesFromService(byte* buffer, size_t count) 
		throw(ConnectError, CommunicationError, ServiceDenied);

	// tests if cache contains at least 'size' bytes
	bool IsCachedEnough(size_t size)
		throw();
	
	// refills the inBuffer (local data cache) if it doesn't contain at least 'size' bytes of data
	void EnsureCachedEnough(size_t size) 
		throw(ConnectError, CommunicationError, ServiceDenied);
};
