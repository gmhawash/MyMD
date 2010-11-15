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
#include "stdafx.h"
#include "QRBG.h"

#include <sys/types.h>
#include <string.h>		// memcpy


#ifdef PLATFORM_WIN
	// windows includes
#	define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers

#	include <winsock2.h>
#	include <WS2tcpip.h>
#	include <windows.h>
#	include <errno.h>

#	define GetLastSocketError()	WSAGetLastError()

#	pragma comment(lib, "ws2_32.lib")	// link with winsock2 library
#endif

#ifdef PLATFORM_LINUX
	// linux includes
#	include <sys/socket.h>
#	include <netinet/in.h>
#	include <arpa/inet.h> 
#	include <netdb.h>
#	include <unistd.h>

#	include <errno.h>
	extern int errno;

#	define GetLastSocketError() errno

#endif

#define ASSERT(assertion)

// WinSock's recv() fails if called with too large buffer size, so
// limit maximum amount of data that could be received in
// one recv() call.
//#define INTERNAL_SOCKET_MAX_BUFFER	65536
#define INTERNAL_SOCKET_MAX_BUFFER	1048576

const char* QRBG::ServerResponseDescription[] = {
	"OK",
	"Service was shutting down",
	"Server was/is experiencing internal errors",
	"Service said we have requested some unsupported operation",
	"Service said we sent an ill-formed request packet",
	"Service said we were sending our request too slow",
	"Authentication failed",
	"User quota exceeded"
};

const char* QRBG::ServerResponseRemedy[] = {
	"None",
	"Try again later",
	"Try again later",
	"Upgrade your client software",
	"Upgrade your client software",
	"Check your network connection",
	"Check your login credentials",
	"Try again later, or contact Service admin to increase your quota(s)"
};


// Initializes class data members, initializes network subsystem.
// Throws exceptions upon failure (memory / winsock).
QRBG::QRBG(size_t cacheSize /*= DEFAULT_CACHE_SIZE*/) /* throw(NetworkSubsystemError, bad_alloc) */
: port(0), hSocket(-1)
, outBuffer(NULL)
, inBuffer(NULL)
, outBufferSize(4096) /* WARNING: 'outBuffer' MUST be large enough to store whole request header before sending! */
, inBufferSize(cacheSize)
, inBufferNextElemIdx(inBufferSize) {
	*szHostname = *szUsername = *szPassword = 0;

	// initialize socket subsystem
#ifdef PLATFORM_WIN
	// initialize winsock

	WORD wVersionRequested;
	WSADATA wsaData;

	wVersionRequested = MAKEWORD(2, 2);

	int err = WSAStartup(wVersionRequested, &wsaData);
	if (err != 0) {
		// we could not find a usable WinSock DLL (Winsock v2.2 was requested/required)
		throw NetworkSubsystemError();
	}

	// Confirm that the WinSock DLL supports 2.2.
	// Note: if the DLL supports versions greater than 2.2 in addition to 2.2, 
	// it will still return 2.2 in wVersion since that is the version we requested.
	if (LOBYTE(wsaData.wVersion) != 2 || HIBYTE(wsaData.wVersion) != 2) {
		// we could not find a usable WinSock DLL
		WSACleanup( );
		throw NetworkSubsystemError();
	}
#elif defined(PLATFORM_LINUX)
	// there's no need for linux sockets to init
#endif

	// if memory allocation fails, propagate exception to the caller
	outBuffer = new byte[outBufferSize];
	inBuffer = new byte[inBufferSize];
}

QRBG::~QRBG() {
	delete[] outBuffer;
	delete[] inBuffer;
}

// Re-initializes the cache buffer.
// New cache size (in bytes) must be AT LEAST 8 bytes (to accommodate the largest type - double),
// however this function shall not allow new cache size to be less then MINIMAL_CACHE_SIZE bytes.
// Returns: success?
bool QRBG::defineCache(size_t cacheSize /* = DEFAULT_CACHE_SIZE */) {
	if (cacheSize < MINIMAL_CACHE_SIZE)
		return false;

	// try to alloc new cache buffer..
	byte* newBuffer;
	try {
		newBuffer = new byte[cacheSize];
	} catch (...) {
		return false;
	}

	// ok, now delete old one and start using newly allocated one!
	delete[] inBuffer;
	inBuffer = newBuffer;
	inBufferSize = cacheSize;
	inBufferNextElemIdx = inBufferSize;

	return true;
}

//////////////////////////////////////////////////////////////////////////
//
// private (worker) methods
//

// Connects to service server and on success, stores connected socket handle
// in private class member.
// Upon failure, exception is thrown.
void QRBG::Connect() throw(ConnectError) {
	if (hSocket != -1) {
		// we're already connected
		return;
	}

	if (!*szHostname || !port) {
		// server address is not defined
		throw ConnectError();
	}

	int hsock = static_cast<int>( socket(AF_INET, SOCK_STREAM, 0) );
	if (hsock == -1) {
		// failed to create socket
		throw ConnectError();
	}

	// try to resolve 'hostname', if we fail, assume 'hostname' as an IP address
	struct sockaddr_in addr;
	struct hostent *hent = gethostbyname(szHostname);
	if (hent == NULL)
		addr.sin_addr.s_addr = inet_addr(szHostname);
	else
		memcpy(&addr.sin_addr, hent->h_addr, hent->h_length);
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);

	if (connect(hsock, (struct sockaddr *)&addr, sizeof(addr))) {
		// failed to connect
		throw ConnectError();
	}

	hSocket = hsock;
}

// Closes connection with service server (if connection was ever established)
void QRBG::Close() throw() {
	// disallow further sends and receives..
	shutdown(hSocket, 2);

	// delete socket descriptor
#if WIN32
	closesocket(hSocket);
#else
	close(hSocket);
#endif

	// for future checks if socket is closed
	hSocket = -1;
}

// Fills the 'buffer' with maximum of 'count' bytes. Actual number of bytes copied into the buffer
// is returned.
// If local cache buffer doesn't contain enough bytes, then a request to the service for 'count' bytes is sent, 
// (blocking while receiving response), and the 'buffer' is filled with the returned data (random bytes).
// Otherwise, the 'buffer' is filled with locally cached data.
// If user requested less then cache size, the cache is refilled and data is returned from the cache.
// Upon failure, exceptions are thrown.
// Returns: count of bytes (received) copied into the supplied buffer.
size_t QRBG::AcquireBytes(byte* buffer, size_t count) throw(ConnectError, CommunicationError, ServiceDenied) {

	// timer start
#	if defined(PLATFORM_WIN)
		timeStart = GetTickCount();
#	elif defined(PLATFORM_LINUX)
		gettimeofday(&timeStart, NULL);
#	endif

	// actual data acquisition
	size_t nCopied = 0;
	if (count <= inBufferSize) {
		EnsureCachedEnough(count);
		nCopied = AcquireBytesFromCache(buffer, count);
	} else {
		nCopied = AcquireBytesFromService(buffer, count);
	}

	// timer end
#	if defined(PLATFORM_WIN)
		timeEnd = GetTickCount();
#	elif defined(PLATFORM_LINUX)
		gettimeofday(&timeEnd, NULL);
#	endif

	return nCopied;
}


// Fills the 'buffer' with exactly 'count' bytes, explicitly from the local cache.
// Number of bytes copied into the buffer is returned ('count' on success, '0' on failure).
size_t QRBG::AcquireBytesFromCache(byte* buffer, size_t count) throw() {
	// fill the buffer from cache (if anyway possible)..
	if (IsCachedEnough(count)) {
		memcpy(buffer, inBuffer + inBufferNextElemIdx, count);
		inBufferNextElemIdx += count;
		return count;
	}
	// ..or fail
	return 0;
}

// Fills the 'buffer' with maximum of 'count' bytes, explicitly from the remote QRBG Service.
// Actual number of bytes copied into the buffer is returned.
// Upon failure, exceptions are thrown.
// Returns: count of bytes (received) copied into the supplied buffer.
size_t QRBG::AcquireBytesFromService(byte* buffer, size_t count) throw(ConnectError, CommunicationError, ServiceDenied) {
	// connect to the service server,
	// propagate exception to the caller
	Connect();

	//
	// prepare and send the request
	//

	// NOTE: we're using plain authentication.

/*
	Client first (and last) packet:
	Size [B]		Content
	--------------	--------------------------------------------------------
	1				operation, from OperationCodes enum
	if operation == GET_DATA_AUTH_PLAIN, then:
	2				content size (= 1 + username_len + 1 + password_len + 4)
	1				username_len (must be > 0 and <= 100)
	username_len	username (NOT zero padded!)
	1				password_len (must be > 0 and <= 100)
	password_len	password in plain 8-bit ascii text (NOT zero padded!)
	4				bytes of data requested

	Server first (and last) packet:
	Size [B]		Content
	--------------	--------------------------------------------------------
	1				response, from ServerResponseCodes enum
	1				response details - reason, from RefusalReasonCodes
	4				data_len, bytes of data that follow
	data_len		data
*/

	// header structure looks like this:
	// 	struct tClientHeader {
	// 		uint8	eOperation;		// MUST BE eOperation == GET_DATA_AUTH_PLAIN for struct remainder to hold
	// 		uint16	cbContentSize;
	// 		uint8	cbUsername;
	// 		char	szUsername[cbUsername];
	// 		uint8	cbPassword;
	// 		char	szPassword[cbPassword];
	// 		uint32	cbRequested;
	// 	};
	// however, two issues obstruct direct structure usage:
	//	1) we don't know username/password length apriori
	//	2) we must convert all numeric values to network order (big endian)
	// so, we'll fill output buffer byte-by-byte...

	uint8 eOperation = GET_DATA_AUTH_PLAIN;
	uint8 cbUsername = static_cast<uint8>( strlen(szUsername) );
	uint8 cbPassword = static_cast<uint8>( strlen(szPassword) );
	uint32 cbRequested = static_cast<uint32>( count );
	uint16 cbContentSize = sizeof(cbUsername) + cbUsername + sizeof(cbPassword) + cbPassword + sizeof(cbRequested);

	uint32 bytesToSend = sizeof(eOperation) + sizeof(cbContentSize) + cbContentSize;

	ASSERT(outBufferSize >= bytesToSend);
	byte* pRequestBuffer = outBuffer;

	*(uint8*)pRequestBuffer = eOperation, pRequestBuffer += sizeof(eOperation);
	*(uint16*)pRequestBuffer = htons(cbContentSize), pRequestBuffer += sizeof(cbContentSize);
	*(uint8*)pRequestBuffer = cbUsername, pRequestBuffer += sizeof(cbUsername);
	memcpy(pRequestBuffer, szUsername, cbUsername), pRequestBuffer += cbUsername;
	*(uint8*)pRequestBuffer = cbPassword, pRequestBuffer += sizeof(cbPassword);
	memcpy(pRequestBuffer, szPassword, cbPassword), pRequestBuffer += cbPassword;
	*(uint32*)pRequestBuffer = htonl(cbRequested), pRequestBuffer += sizeof(cbRequested);

	int ret = send(hSocket, (const char*)outBuffer, bytesToSend, 0);
	if (ret == -1) {
		// failed to send data request to the server
		Close();
		throw CommunicationError();
	}
	if (ret != bytesToSend) {
		// failed to send complete data request to the server
		Close();
		throw CommunicationError();
	}

	//
	// receive header (assuming GET_DATA_AUTH_PLAIN, as we requested)
	//

	// server response header structure looks like this:
	// 	struct tServerHeader {
	// 		uint8 response;		// actually from enum ServerResponseCodes
	// 		uint8 reason;		// actually from enum RefusalReasonCodes
	// 		uint32 cbDataLen;	// should be equal to cbRequested, but we should not count on it!
	// 	};
	// however, to avoid packing and memory aligning portability issues,
	// we'll read input buffer byte-by-byte...
	ServerResponseCodes eResponse;
	RefusalReasonCodes eReason;
	uint32 cbDataLen = 0;

	const uint32 bytesHeader = sizeof(uint8) + sizeof(uint8) + sizeof(uint32);
	byte header[bytesHeader];

	uint32 bytesReceived = 0;
	uint32 bytesToReceiveTotal = bytesHeader;
	uint32 bytesToReceiveNow = 0;

	// receive header
	while ( (bytesToReceiveNow = bytesToReceiveTotal - bytesReceived) > 0 ) {

		int ret = recv(hSocket, (char*)(header + bytesReceived), bytesToReceiveNow, 0);

		if (ret != -1) {
			if (ret > 0) {
				// data received
				bytesReceived += ret;

				// parse the server response
				if (bytesReceived >= 2*sizeof(uint8)) {
					eResponse = (ServerResponseCodes) header[0];
					eReason = (RefusalReasonCodes) header[1];

					// process server response...
					if (eResponse != OK) {
						Close();
						throw ServiceDenied(eResponse, eReason);
					}

					if (bytesReceived >= bytesToReceiveTotal) {
						cbDataLen = ntohl( *((u_long*)(header + 2*sizeof(uint8))) );
					}
				}

			} else {
				// recv() returns 0 if connection was closed by server
				Close();
				throw CommunicationError();
			}

		} else {
			int nErr = GetLastSocketError();
			if (nErr == EAGAIN) {
				// wait a little bit, and try again
			} else {
				// some socket(network) error occurred; 
				// it doesn't matter what it is, declare failure!
				Close();
				throw CommunicationError();
			}
		}
	}


	//
	// receive data
	//

	bytesReceived = 0;
	bytesToReceiveTotal = cbDataLen;

	while ( (bytesToReceiveNow = bytesToReceiveTotal - bytesReceived) > 0 ) {
		// limit to maximal socket buffer size used
		bytesToReceiveNow = bytesToReceiveNow < INTERNAL_SOCKET_MAX_BUFFER ? 
							bytesToReceiveNow : INTERNAL_SOCKET_MAX_BUFFER;

		int ret = recv(hSocket, (char*)(buffer + bytesReceived), bytesToReceiveNow, 0);

		if (ret != -1) {
			if (ret > 0) {
				// data received
				bytesReceived += ret;
			} else {
				// recv() returns 0 if connection was closed by server
				Close();
				throw CommunicationError();
			}

		} else {
			int nErr = GetLastSocketError();
			if (nErr == EAGAIN) {
				// wait a little bit, and try again
			} else {
				// some socket(network) error occurred; 
				// it doesn't matter what it is, declare failure!
				Close();
				throw CommunicationError();
			}
		}
	}

	Close();

	// we succeeded.
	return bytesReceived;
}

// Tests if cache buffer contains at least 'size' bytes.
// Returns: 'true' if it does, and 'false' otherwise.
bool QRBG::IsCachedEnough(size_t size) throw() {
	return inBufferNextElemIdx + size <= inBufferSize;
}

// Ensures that input buffer (local data cache) contains at least 'size' elements.
// In other words, it refills the 'inBuffer' (local data cache) if it doesn't contain at 
// least 'size' bytes of data.
//
// Throws an exception, as indication of failure, in two cases:
//  1) we failed to acquire bytes from service
//  2) we failed to acquire EXACTLY enough bytes to fill WHOLE input buffer
//
void QRBG::EnsureCachedEnough(size_t size) throw(ConnectError, CommunicationError, ServiceDenied) {
	// timer reset
	timeEnd = timeStart;

	if (IsCachedEnough(size))
		return;

	try {
		if (AcquireBytesFromService(inBuffer, inBufferSize) != inBufferSize) 
			throw CommunicationError();
		inBufferNextElemIdx = 0;
	} catch (...) {
		inBufferNextElemIdx = inBufferSize;	// since inBuffer may now be corrupted
		throw;
	}
}


//////////////////////////////////////////////////////////////////////////
//
// info methods
//

#ifdef PLATFORM_WIN
// windows version

// returns the duration (in sec) of the last AcquireBytes(..)
double QRBG::getLastDownloadDuration() {
	return (timeEnd - timeStart) / 1000.0;
}

#elif defined(PLATFORM_LINUX)
// linux version

double QRBG::getLastDownloadDuration() {
	return (timeEnd.tv_sec - timeStart.tv_sec) + 1e-6 * (timeEnd.tv_usec - timeStart.tv_usec);
}

#endif


//////////////////////////////////////////////////////////////////////////
//
// public (interface) methods
//

void QRBG::defineServer(const char* qrbgAddress, unsigned int qrbgPort) throw(InvalidArgumentError) {
	// check parameters
	int len;
	for (len = 0; len <= HOSTNAME_MAXLEN && qrbgAddress[len]; len++);
	if (len > HOSTNAME_MAXLEN) 
		throw InvalidArgumentError();

	if (qrbgPort > 0xFFFF)
		throw InvalidArgumentError();

	// save server settings
	strncpy(szHostname, qrbgAddress, HOSTNAME_MAXLEN);
	port = qrbgPort;
}

void QRBG::defineUser(const char* qrbgUsername, const char* qrbgPassword) throw(InvalidArgumentError) {
	// check parameters
	int len;

	for (len = 0; len <= USERNAME_MAXLEN && qrbgUsername[len]; len++);
	if (len > USERNAME_MAXLEN) 
		throw InvalidArgumentError();

	for (len = 0; len <= PASSWORD_MAXLEN && qrbgPassword[len]; len++);
	if (len > PASSWORD_MAXLEN) 
		throw InvalidArgumentError();

	// save user authentication records
	strncpy(szUsername, qrbgUsername, USERNAME_MAXLEN);
	strncpy(szPassword, qrbgPassword, PASSWORD_MAXLEN);
}

// for future use:
void QRBG::defineUser(const char* qrbgCertificateStore) throw(InvalidArgumentError) {
	throw InvalidArgumentError();
}

//
// integer getter methods
//

/*
// old, faster, non-timed, method
#define IMPLEMENT_QRBG_GETTER(NAME, TYPE) \
	TYPE QRBG::NAME() throw(ConnectError, CommunicationError, ServiceDenied) { \
		EnsureCachedEnough(sizeof(TYPE)); \
		TYPE result = *((TYPE*)(inBuffer + inBufferNextElemIdx)); \
		inBufferNextElemIdx += sizeof(TYPE); \
		return result; \
	}
*/

#define IMPLEMENT_QRBG_GETTER(NAME, TYPE) \
	TYPE QRBG::NAME() throw(ConnectError, CommunicationError, ServiceDenied) { \
		TYPE result = 0xCC; \
		AcquireBytes((byte*)&result, sizeof(TYPE)); \
		return result; \
	}

// fills 'buffer' array of 'TYPE' elements with 'count' elements 
// and returns number of elements filled
#define IMPLEMENT_QRBG_ARRAY_GETTER(NAME, TYPE) \
	size_t QRBG::NAME(TYPE* buffer, size_t count) throw(ConnectError, CommunicationError, ServiceDenied) { \
		return AcquireBytes((byte*)buffer, sizeof(TYPE)*count) / sizeof(TYPE); \
	}

IMPLEMENT_QRBG_GETTER(getByte, byte)
IMPLEMENT_QRBG_GETTER(getInt, int)
IMPLEMENT_QRBG_GETTER(getLongInt, long)

IMPLEMENT_QRBG_ARRAY_GETTER(getBytes, byte)
IMPLEMENT_QRBG_ARRAY_GETTER(getInts, int)
IMPLEMENT_QRBG_ARRAY_GETTER(getLongInts, long)

IMPLEMENT_QRBG_GETTER(getInt8, int8)
IMPLEMENT_QRBG_GETTER(getInt16, int16)
IMPLEMENT_QRBG_GETTER(getInt32, int32)
IMPLEMENT_QRBG_GETTER(getInt64, int64)

IMPLEMENT_QRBG_ARRAY_GETTER(getInt8s, int8)
IMPLEMENT_QRBG_ARRAY_GETTER(getInt16s, int16)
IMPLEMENT_QRBG_ARRAY_GETTER(getInt32s, int32)
IMPLEMENT_QRBG_ARRAY_GETTER(getInt64s, int64)

//
// floating point getter methods
// (these also work on all types of byte ordered machines)
//

// returns: normalized float in range [0, 1>
float QRBG::getFloat() throw(ConnectError, CommunicationError, ServiceDenied) {
	uint32 data = 0x3F800000uL | (getInt32() & 0x00FFFFFFuL);
	return *((float*)&data) - 1.0f;
}

// returns: normalized double in range [0, 1>
double QRBG::getDouble() throw(ConnectError, CommunicationError, ServiceDenied) {
	uint64 data = 0x3FF0000000000000uLL | (getInt64() & 0x000FFFFFFFFFFFFFuLL);
	return *((double*)&data) - 1.0;
}

// returns: array of normalized floats in range [0, 1>
size_t QRBG::getFloats(float* buffer, size_t count) throw(ConnectError, CommunicationError, ServiceDenied) {
	ASSERT(sizeof(float) == sizeof(uint32));

	size_t acquired = AcquireBytes((byte*)buffer, sizeof(uint32)*count) / sizeof(uint32);

	register uint32 data;
	register int idx = (int)acquired;
	while (--idx >= 0) {
		data = 0x3F800000uL | (*((uint32*)(buffer+idx)) & 0x00FFFFFFuL);
		buffer[idx] = *((float*)&data) - 1.0f;
	}

	return acquired;
}

// returns: array of normalized doubles in range [0, 1>
size_t QRBG::getDoubles(double* buffer, size_t count) throw(ConnectError, CommunicationError, ServiceDenied) {
	ASSERT(sizeof(double) == sizeof(uint64));

	size_t acquired = AcquireBytes((byte*)buffer, sizeof(uint64)*count) / sizeof(uint64);

	register uint64 data;
	register int idx = (int)acquired;
	while (--idx >= 0) {
		data = 0x3FF0000000000000uLL | (*((uint64*)(buffer+idx)) & 0x000FFFFFFFFFFFFFuLL);
		buffer[idx] = *((double*)&data) - 1.0;
	}

	return acquired;
}