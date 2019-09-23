#include <stdio.h>
#include <stdlib.h>
#include "ggg_reader.h"

float *read_GGG_File(char *file) {
    // Need code to parse the file name and say HEY this is a valid ggg file....OR even dynamically read the correct resolution. Will not be to hard....just need to implement correctly...
    // Or maybe have serperate functions for seperate resolutions? 
    // Might be best to go that route. The readFloat function will not know the bounds unless otherwise specified...
    FILE *pFile;
    pFile = fopen(file, "rb");
    float *pdata;
    pdata = (float*) malloc (4*10800*5400);
    long result = fread(pdata, 4, (10800*5400), pFile);
    fclose(pFile);
    return pdata;
}

float readFloat(float *data, int r, int c) {
    // Prob need some code to say HEY this is way out of bounds...
    float value = *(data + r*10800 + c);
    return value;
}

// Main here for testing purposes!!
int main() {
    float *data = read_GGG_File("/home/nmoran/data/2m/GL_ELEVATION_M_ASL_ETOPO2v2.2m.ggg");
    int i;
    int j;
    for (i = 0; i < 5400; i++) {
        for (j = 0; j < 10800; j++) {
            printf("Value: %f\n", readFloat(data, i, j));
        }
    }
    free(data);
    return 0;
}
