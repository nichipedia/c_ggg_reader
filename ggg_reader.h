#define ONE_ARC_DEGREE_RESOLUTION 0
#define FIVE_MINUTE_RESOLUTION    1
#define TWO_MINUTE_RESOLUTION     2


void *read_GGG_File(char *file, int resolution);
float readFloat(void *d, int r, int c, int resolution);
int* getGridCoordinates(float* bbox, int resolution);
int* getGridCoordinate(float lat, float lon, int resolution);
int* getGridResolution(int resolution);
float* getLatLonCoordinate(int r, int c, int resolution);
