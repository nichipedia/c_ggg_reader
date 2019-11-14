#include <stdio.h>
#include <stdlib.h>
#include "ggg_reader.h"

float *read_GGG_File(char *file, int resolution) {
    // Need code to parse the file name and say HEY this is a valid ggg file....OR even dynamically read the correct resolution. Will not be to hard....just need to implement correctly...
    // Or maybe have serperate functions for seperate resolutions? 
    // Might be best to go that route. The readFloat function will not know the bounds unless otherwise specified...
    FILE *pFile;
    int nx;
    int ny;
    if (resolution == 2) {
        nx = 10800;
        ny = 5400;
    } else {
        fprintf(stderr, "\n Resolution %d not yet supported!\n", resolution);
        exit(0);
    }
    pFile = fopen(file, "rb");
    float *pdata;
    pdata = (float*) malloc (sizeof(float)*nx*ny);
    long result = fread(pdata, sizeof(float), (nx*ny), pFile);
    if (result == 0) {
        fprintf(stderr, "\n Error reading UNISIPS file...Try again :P\n");
        exit(0);
    }
    fclose(pFile);
    return pdata;
}

float* getLatLonCoordinate(int r, int c, int resolution) {
    int nx;
    int ny;
    float delta;
    if (resolution == TWO_MINUTE_RESOLUTION) {
        if (r < 0 || r >= 5400) {
            fprintf(stderr, "\nRow %d out of bounds [0, 5400)", r);
            exit(0);
        }
        if (c < 0 || c >= 10800) {
            fprintf(stderr, "\nCol %d out of bounds [0, 10800)", c);
            exit(0);
        }
        nx = 10800;
        ny = 5400;
        delta = 2.0/60.0;
    } else {
        fprintf(stderr, "\nResolution %d not yet supported...Check header for supported resos!\n", resolution);
        exit(0);
    }
    float lat = (r * delta) - 89.5f;
    float lon = (c * delta) - 179.5f;
    float *coords = malloc(2 * sizeof(float));
    coords[0] = lon;
    coords[1] = lat;
    return coords;
}

int* getGridResolution(int resolution) {
    int x;
    int y;
    if (resolution == TWO_MINUTE_RESOLUTION) {
        x = 10800;
        y = 5400;
    } else {
        fprintf(stderr, "\n Resolution not supported!\n");
        exit(0);
    }
    int *gridResolutions = malloc(2*sizeof(int));
    gridResolutions[0] = x;
    gridResolutions[1] = y;
    return gridResolutions;
}

int* getGridCoordinates(float bbox[], int resolution) {
    size_t length = sizeof(bbox);
    if (length != 16) {
        fprintf(stderr, "\n BBOX should be four points, Minx, Miny, Maxx, Maxy...\n");
        exit(0);
    }
    float minlon = bbox[0];
    float minlat = bbox[1];
    float maxlon = bbox[2];
    float maxlat = bbox[4];
    if (minlat < -90.0f || minlat > 90.0f) {
        fprintf(stderr, "\n Min Latitude %f is not within the correct range!!\n", minlat);
        exit(0);
    }
    if (maxlat < -90.0f || maxlat > 90.0f) {
        fprintf(stderr, "\n Max Latitude %f is not within the correct range!!\n", maxlat);
        exit(0);
    }
    if (minlon < -180.0f || minlon > 180.0f) {
        fprintf(stderr, "\n Min Longitude %f is not within the correct range!!\n", minlon);
        exit(0);
    }
    if (maxlon < -180.0f || maxlon > 180.0f) {
        fprintf(stderr, "\n Max Longitude %f is not within the correct range!!\n", maxlon);
        exit(0);
    }
    float dx = 0;
    float dy = 0;
    if (resolution == 0) {
        dx = 360;
        dy = 180;
    } else if (resolution == 2) {
        dx = 2.0f/60.0f;
        dy = 2.0f/60.0f;
    } else if (resolution == 1) {
        dx = 5.0f/60.0f;
        dy = 5.0f/60.0f;
    } else {
        fprintf(stderr, "\nResolution %d not supported...Check the ggg_reader header file for supported resos!\n", resolution);
        exit(0);
    }
    int minx = (int) ((minlon+180.0f)/dx);
    int maxx = (int) ((maxlon+180.0f)/dx);
    int miny = (int) ((minlat+90.0f)/dy);   
    int maxy = (int) ((maxlat+90.0f)/dy);   
    int *coords = malloc(4*sizeof(int));
    coords[0] = minx;
    coords[1] = miny;
    coords[2] = maxx;
    coords[3] = maxy;
    return coords;
}

int* getGridCoordinate(float lat, float lon, int resolution) {
    if (lat < -90.0f || lat > 90.0f) {
        fprintf(stderr, "\n Latitude %f is not within the correct range!!\n", lat);
        exit(0);
    }
    if (lon < -180.0f || lon > 180.0f) {
        fprintf(stderr, "\n Longitude %f is not within the correct range!!\n", lon);
        exit(0);
    }
    float dx = 0;
    float dy = 0;
    if (resolution == 0) {
        dx = 360;
        dy = 180;
    } else if (resolution == 2) {
        dx = 2.0f/60.0f;
        dy = 2.0f/60.0f;
    } else if (resolution == 1) {
        dx = 5.0f/60.0f;
        dy = 5.0f/60.0f;
    } else {
        fprintf(stderr, "\nResolution %d not supported...Check the ggg_reader header file for supported resos!\n", resolution);
        exit(0);
    }
    int x = (int) ((lon+180.0f)/dx);
    int y = (int) ((lat+90.0f)/dy);
    int *coord = malloc(2*sizeof(int));
    coord[0] = x;
    coord[1] = y;
    return coord;
}


float readFloat(float *data, int r, int c, int resolution) {
    int x;
    if (resolution == TWO_MINUTE_RESOLUTION) {
        if (r < 0 || r >= 5400) {
            fprintf(stderr, "\nRow %d out of bounds [0, 5400)", r);
            exit(0);
        }
        if (c < 0 || c >= 10800) {
            fprintf(stderr, "\nCol %d out of bounds [0, 10800)", c);
            exit(0);
        }
        x = 10800;
    } else {
        fprintf(stderr, "\nResolution %d not yet supported...Check header for supported resos!\n", resolution);
        exit(0);
    }
    float value = *(data + r*x + c);
    return value;
}

// Main here for testing purposes!!
int test() {
    float *data = read_GGG_File("/home/nmoran/data/2m/GL_ELEVATION_M_ASL_ETOPO2v2.2m.ggg", 2);
    int i;
    int j;
    for (i = 0; i < 5400; i++) {
        for (j = 0; j < 10800; j++) {
            printf("Value: %f\n", readFloat(data, i, j, 2));
        }
    }
    free(data);
    return 0;
}
