#include "agb.h"

typedef struct anim_data
{
    sprite_ptr id;
    uint16_t index;
    uint8_t counter;
} anim_data;

typedef struct anim_data_node anim_data_node;
typedef struct anim_data_node
{
    anim_data data;
    anim_data_node *next;
} anim_data_node;

typedef void (*anim_func)(sprite_ptr sprite);

static void anim_func_frame(sprite_ptr sprite);
static void anim_func_delay(sprite_ptr sprite);
static void anim_func_loop(sprite_ptr sprite);
static void anim_func_jump(sprite_ptr sprite);
static void anim_func_end(sprite_ptr sprite);

static const anim_func s_anim_funcs[] =
{
    [anim_frame] = anim_func_frame,
    [anim_delay] = anim_func_delay,
    [anim_loop]  = anim_func_loop,
    [anim_jump]  = anim_func_jump,
    [anim_end]   = anim_func_end,
};

void sprite_objects_anims_play(sprite_ptr sprite)
{

}

void sprite_object_anim_start(sprite_ptr sprite, const anim_cmd *anim)
{

}

void sprite_object_anim_stop(sprite_ptr sprite)
{

}

static void anim_func_frame(sprite_ptr sprite)
{

}

static void anim_func_delay(sprite_ptr sprite)
{

}

static void anim_func_loop(sprite_ptr sprite)
{

}

static void anim_func_jump(sprite_ptr sprite)
{

}

static void anim_func_end(sprite_ptr sprite)
{

}
