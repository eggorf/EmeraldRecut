#ifndef GUARD_MAUVILLE_OLD_MAN_H
#define GUARD_MAUVILLE_OLD_MAN_H

struct BardSound
{
    u16 length;
    u16 pitch;
};

struct BardSong
{
    u8 lyricsIndex;
    u8 soundIndex;
    u8 timer;
    u8 state;
    s16 length; // Length of the sound for the word currently being sung (i.e. the sum of 'length' in all the current word's phonemes).
    u16 volume;
    s16 pitch;
    s16 voiceInflection;
    u16 lyrics[NUM_BARD_SONG_WORDS];
    struct BardSound sounds[0];
    const struct BardSoundTemplate *soundTemplates;
};

void SetMauvilleOldMan(void);
u8 GetCurrentMauvilleOldMan(void);
void SetMauvilleOldManObjEventGfx(void);
void ResetMauvilleOldManFlag(void);

#endif // GUARD_MAUVILLE_OLD_MAN_H
