//****************************************************************************
//  Copyright (c) 2016-2017  Daniel D Miller
//  DFileRead.cpp - File-handling class
//  This module, which has been entirely compiled from public-domain sources, 
//  is itself declared in the public domain.
//  
//  Collected and Organized by:  Dan Miller
//****************************************************************************

//lint -esym(1704, DFileReadText::DFileReadText, DFileReadBinary::DFileReadBinary)
//lint -esym(1716, DFileRead::length, DFileRead::status)

#define  MAX_FNAME_LEN  1024

class DFileRead {
private:
   
public:
   virtual ~DFileRead();
   virtual uint length(void) = 0 ;
   virtual int status(void) const = 0;
   
};

class DFileReadText : public DFileRead {
private:
   char *fname;
   FILE *fd ;
   uint file_length ;
   int fstatus ;
   
   //  text-file read data
   char *char_buf ;
   uint char_buf_len ;
   
   DFileReadText();  // define default constructor
   //  disable assignment and copy operators
   DFileReadText &operator=(const DFileReadText &src) ;
   DFileReadText(const DFileReadText&);

public:
   DFileReadText(char *read_filename);
   ~DFileReadText();
   char *read_line(uint inlen);
   uint length(void) ;
   int status(void) const 
      { return fstatus ; } ;
} ;

class DFileReadBinary : public DFileRead {
private:
   char *fname;
   // FILE *fd ;
   int hdl ;
   uint file_length ;
   int fstatus ;
   
   DFileReadBinary();  // define default constructor
   //  disable assignment and copy operators
   DFileReadBinary &operator=(const DFileReadBinary &src) ;
   DFileReadBinary(const DFileReadBinary&);

public:
   DFileReadBinary(char *read_filename);
   ~DFileReadBinary();
   int read_buffer(u8 *inbuf, uint inlen);
   uint length(void) ;
   int status(void) const 
      { return fstatus ; } ;
} ;

