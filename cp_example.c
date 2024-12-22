#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

#define READBUFSIZE 128

/* Copy a file a few bytes at a time */
/* Inefficient compared to reading/writing larger blocks */
/* of data, but illustrates many stdio functions. */

int main(int argc, char **argv) {
  FILE *fin, *fout, *fout1;
  char readbuf[READBUFSIZE];
  size_t readlen;

  // Take the filenames from command line arguments
  if (argc != 3) {
    fprintf(stderr, "usage: ./cp_example infile outfile\n");
    return EXIT_FAILURE;
  }

  // Open the input file
  fin = fopen(argv[1], "rb");  // "rb" --> read, binary mode
  if (fin == NULL) {
    fprintf(stderr, "%s -- ", argv[1]);
    perror("fopen for read failed");
    return EXIT_FAILURE;
  }

  // Open the output file for odd bytes
  fout = fopen(argv[2], "wb");  // "wb" --> truncate & write, binary mode
  if (fout == NULL) {
    fprintf(stderr, "%s -- ", argv[2]);
    perror("fopen for write failed");
    return EXIT_FAILURE;
  }

  // open second file for evens
  fout1 = fopen(argv[3], "wb");  // "wb" --> truncate & write, binary mode
  if (fout == NULL) {
    fprintf(stderr, "%s -- ", argv[2]);
    perror("fopen for write failed");
    return EXIT_FAILURE;
  }


  // Read from the file, write to fout.
  while ((readlen = fread(readbuf, 1, READBUFSIZE, fin)) > 0) {
    if (readlen == 1) { // if byte is odd
      if (fwrite(readbuf, 1, readlen, fout) < readlen) {
        perror("fwrite failed");
        return EXIT_FAILURE;
      }
    } else { //if byte is even
      if (fwrite(readbuf, 1, readlen, fout1) < readlen) {
        perror("fwrite failed");
        return EXIT_FAILURE;
      }
    }
  }

  // Test to see if we encountered an error while reading.
  if (ferror(fin)) {
    perror("fread failed");
    return EXIT_FAILURE;
  }
  fclose(fin);
  fclose(fout);

  return EXIT_SUCCESS;
}