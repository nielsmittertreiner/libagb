#include "agb.h"

#define MAX_SPRITE_PALETTE_SLOTS    16

typedef struct sprite_palette_alloc_node sprite_palette_alloc_node;
typedef struct sprite_palette_alloc_node
{
    const void *data;
    uint16_t palette_num;
    uint8_t references;
    bpp_mode bpp;
    size_t size;
    sprite_palette_alloc_node *next;
} sprite_palette_alloc_node;

static uint16_t sprite_object_palette_alloc_data(const sprite_object_palette *palette);
static uint8_t sprite_palette_slots_free(void);

EWRAM_DATA static sprite_palette_alloc_node *s_sprite_palette_alloc_head = NULL;
EWRAM_DATA static uint8_t s_sprite_palette_slots_used = 0;

uint16_t sprite_object_palette_alloc(const sprite_object_palette *palette)
{
    uint16_t palette_num = sprite_object_palette_alloc_data(palette);

    palette_load(palette->data, pltt_offset(palette_num + 16), palette->compression, palette->size);
    return palette_num;
}

static uint16_t sprite_object_palette_alloc_data(const sprite_object_palette *palette)
{
    sprite_palette_alloc_node *curr = s_sprite_palette_alloc_head;
    sprite_palette_alloc_node *node;

    if (curr == NULL)
    {
        node = malloc(sizeof(sprite_palette_alloc_node));
        node->data = palette->data;
        node->palette_num = 15 * (!palette->bpp);
        node->references = 1;
        node->bpp = palette->bpp;
        node->size = palette->size;
        node->next = NULL;
        s_sprite_palette_alloc_head = node;
        s_sprite_palette_slots_used = 1;
        return node->palette_num;
    }

    while (curr->next)
    {
        //AGB_ASSERT(!(curr->data != palette->data) && (curr->bpp == BPP_8 && palette->bpp == BPP_8), "A different 8BPP palette has already been loaded!");

        if (curr->data == palette->data)
        {
            curr->references++;
            return curr->palette_num;
        }

        if (curr->palette_num - curr->next->palette_num - 1)
        {
            node = malloc(sizeof(sprite_palette_alloc_node));
            node->data = palette->data;
            node->palette_num = curr->palette_num - 1;
            node->references = 1;
            node->bpp = palette->bpp;
            node->size = palette->size;
            node->next = curr->next;
            curr->next = node;
            s_sprite_palette_slots_used = 1;
            return node->palette_num;
        }

        curr = curr->next;
    }

    AGB_ASSERT(sprite_palette_slots_free() >= ((palette->size + PLTT_SIZEOF_4BPP - 1) & -PLTT_SIZEOF_4BPP) >> 5, "Not enough space for this palette!");

    node = malloc(sizeof(sprite_palette_alloc_node));
    node->data = palette->data;
    node->palette_num = 15 * (!palette->bpp);
    node->references++;
    node->bpp = palette->bpp;
    node->size = palette->size;
    node->next = NULL;
    curr->next = node;
    s_sprite_palette_slots_used++;
    return node->palette_num;
}

void sprite_object_palette_free(uint16_t palette_num)
{

}

inline static uint8_t sprite_palette_slots_free(void)
{
    return MAX_SPRITE_PALETTE_SLOTS - s_sprite_palette_slots_used;
}
