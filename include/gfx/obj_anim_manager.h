#ifndef GUARD_AGB_GFX_OBJ_ANIM_H
#define GUARD_AGB_GFX_OBJ_ANIM_H

#include "agb.h"

typedef enum anim_cmd_type
{
    anim_frame,
    anim_delay,
    anim_loop,
    anim_jump,
    anim_end,
} anim_cmd_type;

typedef struct anim_cmd_frame
{
    uint32_t frame:9;
    uint32_t duration:16;
    uint32_t v_flip:1;
    uint32_t h_flip:1;
} anim_cmd_frame;

typedef struct anim_cmd
{
    anim_cmd_type type;
    union
    {
        anim_cmd_frame frame;
        uint16_t delay;
        uint8_t loop;
        uint16_t target_cmd;
    };
} anim_cmd;

void sprite_objects_anims_play(sprite_ptr sprite);
void sprite_object_anim_start(sprite_ptr sprite, const anim_cmd *anim);
void sprite_object_anim_stop(sprite_ptr sprite);

#endif // GUARD_AGB_GFX_OBJ_ANIM_H