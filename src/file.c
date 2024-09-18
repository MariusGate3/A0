#include <stdio.h>  // fprintf, stdout, stderr.
#include <stdlib.h> // exit, EXIT_FAILURE, EXIT_SUCCESS.
#include <string.h> // strerror.
#include <errno.h>  // errno.

enum file_type { // Enum for file types
  DATA,
  EMPTY,
  ASCII,
  ISO,
  UTF
};

const char * const FILE_TYPE_STRINGS[] = { // Array with string's for file types
  "data",
  "empty",
  "ASCII text",
  "ISO-8859 text",
  "UTF-8 Unicode text"
};

int print_error(char *path, int errnum) {
  return fprintf(stdout, "%s: cannot determine (%s)\n",
    path, strerror(errnum));
}

int is_ascii(unsigned char *buffer, size_t length) {
  for (size_t i = 0; i < length; i++) {
    unsigned char byte = buffer[i];
    if (!(
      (byte >= 0x20 && byte <= 0x7E) ||
      (byte >= 0x07 && byte <= 0x0D) ||
      byte == 0x1B
    )) {
      return 0;
    }
  }
  return 1;
}

int is_iso(unsigned char *buffer, size_t length) {
  for (size_t i = 0; i < length; i++) {
    unsigned char byte = buffer[i];
    if (!(
      (byte >= 0x20 && byte <= 0x7E) ||
      (byte >= 0x07 && byte <= 0x0D) ||
      byte == 0x1B ||
      (byte >= 0xA0 && byte <= 0xFF)
    )) {
      return 0;
    }
  }
  return 1;
}

int main(int argc, char *argv[]) {
  enum file_type result_file_type; // Result file type variable
  if (argc != 2) {
    printf("Usage: file path\n"); // Print usage message
    return EXIT_FAILURE;
  }
    
  FILE* file = fopen(argv[1], "r"); // Open the file for reading
  if (file == NULL) { // Check if file read fails
    print_error(argv[1], errno); // If it fails, print correct I/O error using print_error function.
    return EXIT_SUCCESS;
  }

  fseek(file,0,SEEK_END); // Move pointer to end of the file
  long file_size = ftell(file); // Check size of file
  fseek(file,0,SEEK_SET) // Set pointer back to start

  if (file_size == 0) { // If size of file is 0, it is empty
    result_file_type = EMPTY; // Set result file type to empty file
    printf("%s: %s\n", argv[1] ,FILE_TYPE_STRINGS[result_file_type]); // Print the filepath and empty file type
    fclose(file);
    return EXIT_SUCCESS;
  }

  unsigned char *buffer = malloc(file_size); // Allocate memory
  if (buffer == NULL) {
    fprintf(stderr, "Memory Allocation failed.\n");
    fclose(file);
    return EXIT_FAILURE;
  }

  fread(buffer,1,file_size,file);
  fclose(file);

  if(is_utf) {
    result_file_type = UTF;
  } else if (is_ascii(buffer, file_size)) {
    result_file_type = ASCII;
  } else if (is_iso(buffer, file_size)) {
    result_file_type = ISO;
  } else {
    result_file_type = DATA;
  }

  printf("%s: %s\n", argv[1], FILE_TYPE_STRINGS[result_file_type]);
  return EXIT_SUCCESS;
}
