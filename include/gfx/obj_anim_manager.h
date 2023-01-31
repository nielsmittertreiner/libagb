#ifndef GUARD_AGB_GFX_OBJ_ANIM_H
#define GUARD_AGB_GFX_OBJ_ANIM_H

#include "agb.h"

typedef enum anim_cmd_type
{
    anim_frame,
    anim_delay,
    anim_loop_start,
    anim_loop_end,
    anim_jump,
    anim_end,
} anim_cmd_type;

typedef struct anim_cmd_frame
{
    uint16_t frame;
    uint16_t duration;
} anim_cmd_frame;

typedef struct anim_cmd_delay
{
    uint16_t delay;
} anim_cmd_delay;

typedef struct anim_cmd_jump
{
    uint16_t target;
} anim_cmd_jump;

typedef struct anim_cmd_loop_start
{
    uint8_t id;
    uint8_t loops;
} anim_cmd_loop_start;

typedef struct anim_cmd_loop_end
{
    uint8_t id;
} anim_cmd_loop_end;

typedef struct anim_cmd
{
    anim_cmd_type type;
    union
    {
        anim_cmd_frame frame;
        anim_cmd_delay delay;
        anim_cmd_loop_start loop_start;
        anim_cmd_loop_end loop_end;
        anim_cmd_jump jump;
    };
} anim_cmd;

void sprite_object_anim_play(sprite_ptr sprite);
void sprite_object_anim_start(sprite_ptr sprite, const anim_cmd *anim);
void sprite_object_anim_stop(sprite_ptr sprite);
const anim_cmd *sprite_object_active_anim(sprite_ptr sprite);

#endif // GUARD_AGB_GFX_OBJ_ANIM_H