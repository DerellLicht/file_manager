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
/* #include <fcntl.h> */
// #include <sys/types.h>
#include <sys/stat.h>   //  _stat()

#include "common.h"
#include "DFileRead.h"

//lint -esym(1740, DFileReadText::char_buf)

//********************************************************************
//  empty virtual constructor
//********************************************************************
DFileRead::~DFileRead()
{
   
}

//********************************************************************
DFileReadText::DFileReadText(char *read_filename) :
   fname(NULL),
   fd(NULL),
   file_length(0),
   fstatus((DWORD)0),
   char_buf(NULL),
   char_buf_len(0)
{
   fname = new char[MAX_FNAME_LEN+1];
   memset(fname, 0, MAX_FNAME_LEN+1);
   fd = fopen(read_filename, "rt");
   if (fd == NULL) {
      fstatus = (int) GetLastError();
      // syslog("%s: %s\n", read_filename, get_system_message(fstatus));
      return ;
   }
   // syslog("%s: file opened successfully\n", read_filename);
}

//********************************************************************
DFileReadText::~DFileReadText()
{
   if (fd != NULL) {
      fclose(fd);
      fd = NULL ;
   }
   if (fname != NULL) {
      delete[] fname ;
      fname = NULL ;
      file_length = 0 ;
   }
}

//********************************************************************
uint DFileReadText::length(void)
{
   if (fd == NULL  ||  fstatus != 0) {
      return 0;
   }
   if (file_length == 0) {
      struct _stat buf;
      _fstat(_fileno(fd), &buf );
      file_length = (uint) buf.st_size ;
   }
   return file_length;
}

//********************************************************************
// DFileReadText.cpp  104  Warning 669: 
//    Possible data overrun for function 'fgets(char *, int, struct _iobuf *)', 
//    argument 2 (size=128) exceeds argument 1 (size=2)
//    [Reference: file read_file.cpp: lines 86, 89; 
//                file DFileReadText.cpp: lines 37, 93, 97, 99, 100]
//********************************************************************
char *DFileReadText::read_line(uint inlen)
{
   if (fd == NULL  ||  fstatus < 0) {
      fstatus = -(int)ERROR_FILE_NOT_FOUND;
      return NULL;
   }
   if (inlen == 0) {
      if (char_buf == NULL) {
         fstatus = -(int)ERROR_FILE_NOT_FOUND;
         return NULL;
      }
   }
   else {   //  inlen > 0
      if (char_buf == NULL) {
          char_buf_len = inlen ;
          char_buf = (char *) new char[char_buf_len+1];
      }
      //  if caller requests larger buffer, re-alloc existing buffer
      else if (inlen > char_buf_len) {
          delete[] char_buf ;
          char_buf_len = inlen ;
          char_buf = (char *) new char[char_buf_len+1];
      }
   }

   char *bptr = fgets(char_buf, (int) char_buf_len, fd); //lint !e669
   if (bptr != NULL) {
      strip_newlines(char_buf);
      fstatus = (int) strlen(char_buf);
      return char_buf;
   }
   if (feof(fd)) {
      fstatus = -(int)ERROR_HANDLE_EOF;
      return NULL;
   }
   fstatus = -(int)GetLastError();
   return NULL;
}

