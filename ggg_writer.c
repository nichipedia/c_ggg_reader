#include <stdlib.h>
#include <stdio.h>
#include "ggg_writer"


FILE* createOutputFile(char* file) {
    FILE* pFile = fopen(file, "wb");
    return pFile;
}

int writeGrid(float* grid, int resolution, FILE* pFile) {
    int nx;
    int ny;
    if (resolution == TWO_MINUTE_RESOLUTION) {
        nx = 10800;
        ny = 5400;
    } else {
        fprintf(stderr, "\n Resolution %d not yet supported!\n", resolution);
        exit(0);
    }
    long result = fwrite(grid, sizeof(float), nx*ny, pFile);
    return 1;
}
