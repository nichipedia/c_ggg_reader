#include <stdio.h>
#include <stdlib.h>

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
    pdata = (float*) malloc (4*nx*ny);
    long result = fread(pdata, 4, (nx*ny), pFile);
    if (result == 0) {
        fprintf(stderr, "\n Error reading UNISIPS file...Try again :P\n");
        exit(0);
    }
    fclose(pFile);
    return pdata;
}

int[] getGridCoordinates(float bbox[], int resolution) {
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
    int coords[4] = {minx, miny, maxx, maxy};
    return coords;
}

int[] getGridCoordinate(float lat, float lon, int resolution) {
    if (lat < -90.0f || lat > 90.0f) {
        fprintf(stderr, "\n Latitude %f is not within the correct range!!\n", lat);
        exit(0);
    }
    if (lon < -180.0f || lon > 180.0f) {
        fprintf(stderr, "\n Longitude %f is not within the correct range!!\n", lat);
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
    int coord[2] = {x, y};
    return coord;
}


float readFloat(float *data, int r, int c, int resolution) {
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
