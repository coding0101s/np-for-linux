#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int writef(const char* path, const char* content) {
   FILE* fp = fopen(path, "w");
   if (!fp) {
       return 1;
   }
   fprintf(fp, "%s", content);
   fclose(fp);
   return 0;
}

char* readf(const char* path) {
   FILE* fp = fopen(path, "r");
   long file_size;
   char* buffer;
   if (!fp) {
       return NULL;
   }
  
   fseek(fp, 0, SEEK_END);
   file_size = ftell(fp);
   fseek(fp, 0, SEEK_SET);

   buffer = (char*)malloc(file_size + 1);
   if (!buffer) {
       return NULL;
   }

   memset(buffer, 0, file_size+1);

   fread(buffer, 1, file_size, fp);

   fclose(fp);

   return buffer;
}