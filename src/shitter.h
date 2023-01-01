#ifndef shitter_include_file
#define shitter_include_file

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    bool defined;
    uint24_t x;
    uint8_t y;
    uint8_t angle;
    uint8_t x_step;
    uint8_t y_step;
    uint8_t angle_step;
    uint8_t size;
} Shitter;

typedef struct {
    uint8_t length;
    Shitter list[5];
} ShitterList;


#ifdef __cplusplus
}
#endif

#endif
