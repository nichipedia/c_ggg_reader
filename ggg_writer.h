#define ONE_ARC_DEGREE_RESOLUTION 0
#define FIVE_MINUTE_RESOLUTION    1
#define TWO_MINUTE_RESOLUTION     2


FILE* createOutputFile(char* file);
int writeGrid(float* grid, int resolution);
