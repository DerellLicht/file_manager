//******************************************************************************************
//  read_file_solo.cpp - this is a single-file solution for reading all lines in a text file; 
//  for when I just want a simple, quick single-file application.
//******************************************************************************************

#include <windows.h>
#include <stdio.h>

//lint -esym(818, argv)  // Pointer parameter could be declared as pointing to const
//lint -esym(715, argc)  // Symbol 'argc' not referenced
//lint -e10              // Expecting '}'
#define  INSTR_LEN   128

//*********************************************************************
static void usage(void)
{
   puts("Usage: read_file_solo read_filename_spec");
}

//*********************************************************************
int main(int argc, char **argv)
{
   FILE *infile ;
   char rstr[INSTR_LEN+1] ;

   infile = fopen(argv[1], "rt") ;
   if (infile == 0) {
      usage() ;
      return 1;
   }
   //  loop thru all lines in file
   unsigned lcount = 0 ;
   while (fgets(rstr, INSTR_LEN, infile) != 0) {
      lcount++ ;
   }
   fclose(infile) ;
   printf("%u file read in %s\n", lcount, argv[1]) ;

   return 0;
}
