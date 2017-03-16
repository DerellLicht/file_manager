//****************************************************************************
//  Copyright (c) 2016-2017  Daniel D Miller
//  DFileRead.cpp - File-handling class
//  This module, which has been entirely compiled from public-domain sources, 
//  is itself declared in the public domain.
//  
//  Collected and Organized by:  Dan Miller
//****************************************************************************

#include <windows.h>
#include <stdio.h>   //  FILE*
#include <io.h>
#include <fcntl.h>
// #include <sys/types.h>
#include <sys/stat.h>   //  _stat()

//lint -esym(1762, DFileReadBinary::read_buffer)

#include "common.h"
#include "DFileRead.h"

//********************************************************************
DFileReadBinary::DFileReadBinary(char *read_filename) :
   fname(NULL),
   hdl(0),
   file_length(0),
   fstatus((DWORD)0)
{
   fname = new char[MAX_FNAME_LEN+1];
   memset(fname, 0, MAX_FNAME_LEN+1);
   hdl = _open(read_filename, _O_BINARY | _O_RDONLY);
   if (hdl == -1) {
      fstatus = (int) GetLastError();
      // syslog("%s: %s\n", read_filename, get_system_message(fstatus));
      return ;
   }
   // syslog("%s: file opened successfully\n", read_filename);
}

//********************************************************************
DFileReadBinary::~DFileReadBinary()
{
   if (hdl > 0) {
      _close(hdl);
      hdl = 0 ;
   }
   if (fname != NULL) {
      delete[] fname ;
      fname = NULL ;
      file_length = 0 ;
   }
}

//********************************************************************
uint DFileReadBinary::length(void)
{
   if (hdl <= 0  ||  fstatus != 0) {
      return 0;
   }
   if (file_length == 0) {
      struct _stat buf;
      _fstat(hdl, &buf );
      file_length = (uint) buf.st_size ;
   }
   return file_length;
}

//********************************************************************
int DFileReadBinary::read_buffer(u8 *inbuf, uint inlen)
{
   if (hdl <= 0  ||  fstatus != 0) {
      return -(int)ERROR_FILE_NOT_FOUND;
   }
   int rbytes = _read(hdl, inbuf, inlen);
   if (rbytes > 0) {
      return rbytes;
   }
   // if (feof(fd)) {
   //    return -(int)ERROR_HANDLE_EOF;
   // }
   return -(int)GetLastError();
}

