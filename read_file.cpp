#include <windows.h>
#include <stdio.h>

#include "common.h"
#include "DFileRead.h"

#define  INSTR_LEN   128

static DFileReadText *readfile = NULL ;
static DFileReadBinary *readfileb = NULL ;

static bool use_binary = false ;

//*********************************************************************
static void usage(void)
{
   puts("Usage: read_file [-b] read_filename");
   puts("-b indicates binary file; default is text file");
}

//*********************************************************************
int main(int argc, char **argv)
{
   char fname[MAX_FNAME_LEN+1] = "";
   int idx ;
   for (idx=1; idx<argc; idx++) {
      char *p = argv[idx];
      if (*p == '-') {
         p++ ;
         switch (*p) {
         case 'b':
            use_binary = true ;
            break;
         default:
            usage();
            return 1;
         }

      }
      else {
         strncpy(fname, p, MAX_FNAME_LEN);
         fname[MAX_FNAME_LEN] = 0 ;
      }
   }
   if (fname[0] == 0) {
      usage();
      return 1;
   }
   
   //  test binary form of class
   int fstatus ;
   if (use_binary) {
      u8 inbuf[1024];
      readfileb = new DFileReadBinary(fname);
      fstatus = readfileb->status();
      if (fstatus != 0) {
         printf("%s: %s\n", fname, get_system_message((uint) fstatus));
         return 1;
      }
      uint filelen = readfileb->length();
      printf("%s: file length=%u bytes\n", fname, filelen);
      while (LOOP_FOREVER) {
         int rbytes = readfileb->read_buffer(inbuf, 1024);
         if (rbytes <= 0) {
            //  rbytes == 0  indicates end-of-file
            if (rbytes < 0) {
               printf("exiting with [%d] %u status\n", rbytes, (uint) GetLastError());
            }
            break;
         }
         printf("read %d bytes\n", rbytes);
      }
      
   }
   //  test text form of class
   else {
      readfile = new DFileReadText(fname);
      fstatus = readfile->status();
      if (fstatus < 0) {
         printf("%s: %s\n", fname, get_system_message((uint) fstatus));
         return 1;
      }
      printf("%s: file length=%u bytes\n", fname, readfile->length());
      uint lcount = 0 ;
      char *inptr;
      uint inlen = INSTR_LEN;
      while (LOOP_FOREVER) {
         // int result = readfile->read_line(inlen);
         inptr = readfile->read_line(inlen);
         inlen = 0 ;  //  future reads will using existing buffer and length
         fstatus = readfile->status() ;
         if (inptr == NULL) {
            if (fstatus != -(int)ERROR_HANDLE_EOF) {
               printf("L%u: %s\n", lcount, get_system_message(fstatus)); //lint !e732
            }
            break;
         }
         lcount++;
      }
      printf("%u lines read\n", lcount);
   }
   delete readfile;

   return 0;
}
