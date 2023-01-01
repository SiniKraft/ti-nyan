#ifndef laser_include_file
#define laser_include_file

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    bool defined;
    uint24_t x;
    uint8_t y;
} LaserSprite;

typedef struct {
    LaserSprite list[3];
    uint8_t length;
} LaserList;

extern void TryCreateLaser(uint24_t x, uint8_t y, LaserList *laser_list);
extern void UpdateAndRenderLasers(LaserList *laser_list);

#ifdef __cplusplus
}
#endif

#endif
