#include "agb.h"

typedef struct anim_loop
{
    uint8_t id;
    uint8_t loops;
} anim_loop;

typedef struct anim_data
{
    const anim_cmd *anims;
    const sprite_object_tiles *tiles;
    sprite_ptr sprite;
    int16_t cmd;
    uint16_t counter;
    anim_loop *loop_data;
} anim_data;

typedef struct anim_data_node anim_data_node;
typedef struct anim_data_node
{
    anim_data data;
    anim_data_node *next;
} anim_data_node;

typedef int16_t (*anim_func)(anim_data *);

static anim_loop *anim_loop_data(const anim_cmd *anim);
static int16_t anim_func_frame(anim_data *anim);
static int16_t anim_func_delay(anim_data *anim);
static int16_t anim_func_loop_start(anim_data *anim);
static int16_t anim_func_loop_end(anim_data *anim);
static int16_t anim_func_jump(anim_data *anim);
static int16_t anim_func_end(anim_data *anim);

EWRAM_DATA static anim_data_node *s_sprite_object_anim_data_head = NULL;

static const anim_func s_anim_funcs[] =
{
    [anim_frame]      = anim_func_frame,
    [anim_delay]      = anim_func_delay,
    [anim_loop_start] = anim_func_loop_start,
    [anim_loop_end]   = anim_func_loop_end,
    [anim_jump]       = anim_func_jump,
    [anim_end]        = anim_func_end,
};

void sprite_object_anim_play(sprite_ptr sprite)
{
    anim_data_node *curr = s_sprite_object_anim_data_head;

    while (curr)
    {
        if (curr->data.sprite == sprite)
        {
            if (!curr->data.counter)
                curr->data.cmd += s_anim_funcs[curr->data.anims[curr->data.cmd].type](&curr->data);
            else
                curr->data.counter--;
        }
        
        curr = curr->next;
    }
}

void sprite_object_anim_start(sprite_ptr sprite, const anim_cmd *anim)
{
    anim_data_node *curr = s_sprite_object_anim_data_head;
    anim_data_node *node;

    while (curr)
    {
        if (curr->data.sprite == sprite)
        {
            curr->data.anims = anim;
            curr->data.tiles = sprite_object_tiles_data(sprite);
            curr->data.counter = 0;
            curr->data.cmd = 0;
            free(curr->data.loop_data);
            curr->data.loop_data = anim_loop_data(anim);
            return;
        }
        curr = curr->next;
    }

    node = malloc(sizeof(anim_data_node));
    node->data.sprite = sprite;
    node->data.anims = anim;
    node->data.tiles = sprite_object_tiles_data(sprite);
    node->data.counter = 0;
    node->data.cmd = 0;
    node->data.loop_data = anim_loop_data(anim);
    node->next = s_sprite_object_anim_data_head;
    s_sprite_object_anim_data_head = node;
}

void sprite_object_anim_stop(sprite_ptr sprite)
{
    anim_data_node *curr = s_sprite_object_anim_data_head;
    anim_data_node *prev = s_sprite_object_anim_data_head;

    if (curr->data.sprite == sprite)
    {
        s_sprite_object_anim_data_head = curr->next;
        free(curr->data.loop_data);
        free(curr);
    }

    while (curr && (curr->data.sprite != sprite))
    {
        prev = curr;
        curr = curr->next;
    }

    AGB_ASSERT(curr, "sprite doesn't have an animation!");

    prev->next = curr->next;
    free(curr->data.loop_data);
    free(curr);
}

static anim_loop *anim_loop_data(const anim_cmd *anim)
{
    anim_loop *ptr = NULL;
    uint32_t index = 0;
    uint32_t loops = 0;

    for (uint32_t i = 0; anim[i].type != anim_end; i++)
    {
        if (anim[i].type == anim_loop_start)
            loops++;
    }

    if (loops)
    {
        ptr = malloc(sizeof(anim_loop) * loops);

        for (uint32_t i = 0; anim[i].type != anim_end; i++)
        {
            if (anim[i].type == anim_loop_start)
            {
                ptr[index].id = anim[i].loop_start.id;
                ptr[index].loops = anim[i].loop_start.loops;
                index++;
            }
        }
    }

    return ptr;
}

static int16_t anim_func_frame(anim_data *anim)
{
    size_t frame_size = sprite_object_tiles_size(anim->sprite->shape, anim->sprite->size, anim->sprite->bpp);
    void *buffer = malloc(anim->tiles->size);

    anim->counter = anim->anims[anim->cmd].frame.duration;

    if (!anim->sprite->affine && !anim->sprite->double_size)
    {
        anim->sprite->h_flip = anim->anims[anim->cmd].frame.h_flip;
        anim->sprite->v_flip = anim->anims[anim->cmd].frame.v_flip;
    }

    switch (anim->tiles->compression)
    {
        case COMPRESSION_NONE:
            dma_copy_32(anim->tiles->data, buffer, anim->tiles->size);
            break;
        case COMPRESSION_LZ77:
            lz77_uncomp_wram(anim->tiles->data, buffer);
            break;
        case COMPRESSION_RL:
            rl_uncomp_wram(anim->tiles->data, buffer);
            break;
        case COMPRESSION_HUFF:
            huff_uncomp(anim->tiles->data, buffer);
            break;
    }

    dma_copy_32(buffer + (frame_size * anim->anims[anim->cmd].frame.frame), (void *)(OBJ_VRAM0 + (anim->sprite->tile_num << 5)), frame_size);
    free(buffer);
    return 1;
}

static int16_t anim_func_delay(anim_data *anim)
{
    anim->counter = anim->anims[anim->cmd].delay.delay;
    return 1;
}

static int16_t anim_func_loop_start(anim_data *anim)
{
    uint32_t i = 0;

    while (anim->loop_data[i].id != anim->anims[anim->cmd].loop_start.id)
    {
        i++;
    }

    if (anim->loop_data[i].loops)
    {
        anim->loop_data[i].loops--;
    }
    else
    {
        anim->loop_data[i].loops = anim->anims[anim->cmd].loop_start.loops - 1;
    }

    return 1;
}

static int16_t anim_func_loop_end(anim_data *anim)
{
    uint32_t i = 0;

    while (anim->loop_data[i].id != anim->anims[anim->cmd].loop_start.id)
    {
        i++;
    }

    if (anim->loop_data[i].loops)
    {
        return (i - anim->cmd) + 1;
    }
    
    return 1;
}

static int16_t anim_func_jump(anim_data *anim)
{
    return anim->anims[anim->cmd].jump.target - anim->cmd;
}

static int16_t anim_func_end(anim_data *anim)
{
    //sprite_object_anim_stop(anim->sprite);
    return 0;
}
