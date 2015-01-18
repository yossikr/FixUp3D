/*
 * SimpleLogWriter.cpp
 *
 *  Created on: 17.01.2015
 *      Author: Forsaken
 */

#include "SimpleLogWriter.h"
#include <sstream>
#include <TCHAR.H>
#include <stdio.h>

using namespace std;

namespace Core {

SimpleLogWriter::SimpleLogWriter(const TCHAR* debugFilename) {
	hFile = CreateFile(debugFilename, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    if (hFile == INVALID_HANDLE_VALUE) {
    	TCHAR debug[256];
    	_stprintf(debug, "Can't create file '%s'.", debugFilename);
    	MessageBox(NULL, debug, TEXT("UpUsbIntercept"), MB_OK);
    }
}

SimpleLogWriter::~SimpleLogWriter() {
	if (hFile != INVALID_HANDLE_VALUE) {
		CloseHandle(hFile);
		hFile = INVALID_HANDLE_VALUE;
	}
}

SimpleLogWriter* SimpleLogWriter::writeLong(ULONG number) {
	if (hFile == INVALID_HANDLE_VALUE) {
		return this;
	}
	stringstream	stream;
	stream << number << flush;
	const char* sBuffer = stream.str().c_str();
	DWORD		dwBufferLen = stream.str().length();
    DWORD 		dwBytesWritten = 0;
    BOOL bErrorFlag = WriteFile(hFile, sBuffer, dwBufferLen, &dwBytesWritten, NULL);
    if (!bErrorFlag) {
    	MessageBoxA(NULL, sBuffer, "UpUsbIntercept: writeLong failed", MB_OK);
    }
    return this;
}

SimpleLogWriter* SimpleLogWriter::writeFloat(FLOAT number) {
	if (hFile == INVALID_HANDLE_VALUE) {
		return this;
	}
	stringstream	stream;
	stream << number << flush;
	const char* sBuffer = stream.str().c_str();
	DWORD		dwBufferLen = stream.str().length();
    DWORD 		dwBytesWritten = 0;
    BOOL bErrorFlag = WriteFile(hFile, sBuffer, dwBufferLen, &dwBytesWritten, NULL);
    if (!bErrorFlag) {
    	MessageBoxA(NULL, sBuffer, "UpUsbIntercept: writeLong failed", MB_OK);
    }
    return this;
}

SimpleLogWriter* SimpleLogWriter::writeString(const char* text) {
	if (hFile == INVALID_HANDLE_VALUE) {
		return this;
	}
	stringstream	stream;
	stream << text << flush;
	DWORD		dwBufferLen = strlen(stream.str().c_str());
    DWORD 		dwBytesWritten = 0;
    BOOL bErrorFlag = WriteFile(hFile, stream.str().c_str(), dwBufferLen, &dwBytesWritten, NULL);
    if (!bErrorFlag) {
    	char* debug = new char[64];
    	sprintf(debug, "Failed to write '%s' to file. (length: %lu)", stream.str().c_str(), dwBufferLen);
    	MessageBoxA(NULL, debug, "UpUsbIntercept: writeString failed", MB_OK);
    }
    return this;
}

SimpleLogWriter* SimpleLogWriter::writeBinaryBuffer(PVOID buffer, ULONG bufferLength) {
	if (hFile == INVALID_HANDLE_VALUE) {
		return this;
	}
	char*	sBuffer = new char[2];
    DWORD 	dwBytesWritten = 0;
	BOOL 	bErrorFlag = false;
	for (ULONG i = 0; i < bufferLength; i++) {
		sprintf(sBuffer, "%02x", *((PUCHAR)buffer + i));
		bErrorFlag = WriteFile(hFile, sBuffer, 2, &dwBytesWritten, NULL);
	    if (!bErrorFlag) {
	    	MessageBoxA(NULL, sBuffer, "UpUsbIntercept: writeBinaryBuffer failed", MB_OK);
	    }
	}
    return this;
}

} /* namespace Core */
