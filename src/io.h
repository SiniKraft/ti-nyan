#ifndef io_include_file
#define io_include_file

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    uint24_t seconds;
    uint24_t killed_shitters;
    char name[13];
    bool defined;  // If there was any problem loading the file
} BestScoreData;

extern uint8_t TryWriteBestScore(const uint24_t *seconds, const uint24_t *killed, const char name[13], BestScoreData *bsd);
extern uint8_t TryReadBestScore(BestScoreData* bsd);
extern uint8_t TryReadCurrentSkin(char c_skin[9]);
extern uint8_t TryWriteCurrentSkin(char c_skin[9]);

#ifdef __cplusplus
}
#endif

#endif

