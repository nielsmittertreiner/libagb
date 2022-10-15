#include "agb.h"

typedef struct sprite_object_node sprite_object_node;
typedef struct sprite_object_node
{
    sprite_object object;
    sprite_object_node *next;
} sprite_object_node;

IWRAM_DATA oam_data g_oam_buffer[MAX_OAM_ENTRIES];
EWRAM_DATA oam_matrix g_oam_matrix_buffer[MAX_OAM_MATRICES];
EWRAM_DATA sprite_object g_sprites[MAX_OAM_ENTRIES];

EWRAM_DATA uint8_t s_sprite_object_count = 0;
EWRAM_DATA static sprite_object_node *s_sprite_object_list = NULL;

sprite_ptr sprite_object_create(const sprite_object_template *template, vec2i pos)
{
    sprite_object_node *curr = s_sprite_object_list;
    sprite_object_node *node;
    sprite_object object;

    AGB_ASSERT(s_sprite_object_count < MAX_OAM_ENTRIES, "OAM is full!");

    object.pos = vec2i_to_vec2fp(pos);
    object.affine_mode = template->affine_mode;
    object.object_mode = template->object_mode;
    object.mosaic = template->mosaic;
    object.bpp = template->tiles->bpp;
    object.shape = template->shape;
    object.size = template->size;
    object.matrix_num = 0; // if affine, allocate matrix, maybe increase usage counter
    object.tile_num = sprite_tiles_alloc(template->tiles);
    object.palette_num = 0; // allocate palette, increase usage counter
    object.priority = template->priority;
    object.sub_priority = template->sub_priority;
    object.func = template->func;

    if (curr == NULL)
    {
        node = malloc(sizeof(sprite_object_node));
        node->object = object;
        node->next = NULL;
        s_sprite_object_list = node;
        s_sprite_object_count++;
        return &node->object;
    }

    while (curr->next)
    {
        if (((object.priority << 5 | object.sub_priority) << 8 | object.pos.y) < ((curr->object.priority << 5 | curr->object.sub_priority) << 8 | curr->object.pos.y))
        {
            node = malloc(sizeof(sprite_object_node));
            node->object = object;
            node->next = curr->next;
            curr->next = node;
            s_sprite_object_count++;
            return &node->object;
        }
        curr = curr->next;
    }

    node = malloc(sizeof(sprite_object_node));
    node->object = object;
    node->next = NULL;
    curr->next = node;
    s_sprite_object_count++;
    return &node->object;
}

void sprite_object_destroy(sprite_ptr sprite_ptr)
{
    
}

// copy all sprite_objects from linked list to oam_data linked list
void sprite_objects_commit(void)
{
    sprite_object_node *curr = s_sprite_object_list;
    oam_data oam;
    uint8_t i = 0;

    while (curr != NULL)
    {
        oam.x = vec2fp_to_vec2i(curr->object.pos).x;
        oam.y = vec2fp_to_vec2i(curr->object.pos).y;
        oam.affine_mode = curr->object.affine_mode;
        oam.object_mode = curr->object.object_mode;
        oam.mosaic = curr->object.mosaic;
        oam.bpp = curr->object.bpp;
        oam.shape = curr->object.shape;
        oam.size = curr->object.size;
        oam.matrix_num = (curr->object.matrix_num | (curr->object.h_flip << 3) | (curr->object.v_flip << 4));
        oam.tile_num = curr->object.tile_num;
        oam.priority = curr->object.priority;
        oam.palette_num = curr->object.palette_num;
        DmaCopy16(3, &oam, (oam_data *)OAM + i, sizeof(oam_data));
        //cpu_copy_16(&oam, (oam_data *)OAM + i, sizeof(oam_data));
        curr = curr->next;
        i++;
    }
}

void sprite_objects_copy(void)
{
    sprite_object_node *curr = s_sprite_object_list;
    oam_data oam;
    uint8_t index = 0;

    while (curr != NULL)
    {
        oam.x = vec2fp_to_vec2i(curr->object.pos).x;
        oam.y = vec2fp_to_vec2i(curr->object.pos).y;
        oam.affine_mode = curr->object.affine_mode;
        oam.object_mode = curr->object.object_mode;
        oam.mosaic = curr->object.mosaic;
        oam.bpp = curr->object.bpp;
        oam.shape = curr->object.shape;
        oam.size = curr->object.size;
        oam.matrix_num = (curr->object.matrix_num | (curr->object.h_flip << 3) | (curr->object.v_flip << 4));
        oam.tile_num = curr->object.tile_num;
        oam.priority = curr->object.priority;
        oam.palette_num = curr->object.palette_num;
        
        g_oam_buffer[index++] = oam;
        curr = curr->next;
    }

    DmaCopy32(3, g_oam_buffer, OAM, sizeof(g_oam_buffer));
}

//sprite_object *get_sprite_object_ptr(uint8_t id)
//{
//    return &g_sprites[id];
//}
//
//void copy_sprite_objects_to_oam_buffer(void)
//{
//    sprite_object *sprite;
//    
//    for (uint32_t i = 0; i < MAX_OAM_ENTRIES; i++)
//    {
//        sprite = get_sprite_object_ptr(s_sprite_order[i]);
//        if (sprite->active && sprite->visible)
//        {
//            g_oam_buffer[i].y = sprite->pos.y;
//            g_oam_buffer[i].affine_mode = sprite->affine;
//            g_oam_buffer[i].object_mode = sprite->objectMode;
//            g_oam_buffer[i].mosaic = sprite->mosaic;
//            g_oam_buffer[i].bpp = sprite->bpp;
//            g_oam_buffer[i].shape = sprite->shape;
//            g_oam_buffer[i].x = sprite->pos.x;
//            g_oam_buffer[i].matrix_num = (sprite->matrixNum | (sprite->hFlip << 3) | (sprite->vFlip << 4));
//            g_oam_buffer[i].size = sprite->shape;
//            g_oam_buffer[i].tile_num = sprite->tileNum;
//            g_oam_buffer[i].priority = sprite->priority;
//            g_oam_buffer[i].palette = sprite->paletteNum;
//        }
//    }
//}
//
//void copy_oam_matrix_buffer_to_oam_buffer(void)
//{
//    uint32_t base;
//
//    for (uint32_t i = 0; i < MAX_OAM_MATRICES; i++)
//    {
//        base = i << 2;
//        g_oam_buffer[base + 0].affine_param = g_oam_matrix_buffer[i].a;
//        g_oam_buffer[base + 1].affine_param = g_oam_matrix_buffer[i].b;
//        g_oam_buffer[base + 2].affine_param = g_oam_matrix_buffer[i].c;
//        g_oam_buffer[base + 3].affine_param = g_oam_matrix_buffer[i].d;
//    }
//}
//
//void build_sprite_object_priorities(void)
//{
//    sprite_object *sprite;
//
//    for (uint32_t i = 0; i < MAX_OAM_MATRICES; i++)
//    {
//        sprite = get_sprite_object_ptr(i);
//        s_sprite_priorities[i] = sprite->pos.z | (sprite->priority << 8);
//    }
//}
//
//void sort_sprite_objects(void)
//{
//    sprite_object *sprite1;
//    sprite_object *sprite2;
//    uint16_t sprite1Priority;
//    uint16_t sprite2Priority;
//    s16 sprite1Y;
//    s16 sprite2Y;
//    uint8_t shape;
//    uint32_t j;
//
//    for (uint32_t i = 1; i < MAX_OAM_ENTRIES; i++)
//    {
//        sprite1 = get_sprite_object_ptr(s_sprite_order[i - 1]);
//        sprite2 = get_sprite_object_ptr(s_sprite_order[i]);
//        sprite1Priority = s_sprite_priorities[s_sprite_order[i - 1]];
//        sprite2Priority = s_sprite_priorities[s_sprite_order[i]];
//        sprite1Y = sprite1->pos.y;
//        sprite2Y = sprite2->pos.y;
//        j = i;
//
//        if (sprite1Y >= DISPLAY_HEIGHT)
//            sprite1Y -= 256;
//
//        if (sprite2Y >= DISPLAY_HEIGHT)
//            sprite2Y -= 256;
//
//        if (sprite1->affine && sprite1->doubleSize && sprite1->size == OAM_SIZE_3)
//        {
//            shape = sprite1->shape;
//            if (shape == OAM_SQUARE || shape == OAM_V_RECTANGLE)
//            {
//                if (sprite1Y > 128)
//                    sprite1Y -= 256;
//            }
//        }
//
//        if (sprite2->affine && sprite2->doubleSize && sprite2->size == OAM_SIZE_3)
//        {
//            shape = sprite2->shape;
//            if (shape == OAM_SQUARE || shape == OAM_V_RECTANGLE)
//            {
//                if (sprite2Y > 128)
//                    sprite2Y -= 256;
//            }
//        }
//
//        while (j > 0 && ((sprite1Priority > sprite2Priority) || (sprite1Priority == sprite2Priority && sprite1Y < sprite2Y)))
//        {
//            uint8_t temp = s_sprite_order[j];
//            s_sprite_order[j] = s_sprite_order[j - 1];
//            s_sprite_order[j - 1] = temp;
//
//            j--;
//            if (j == 0)
//                break;
//
//            sprite1 = get_sprite_object_ptr(s_sprite_order[j - 1]);
//            sprite2 = get_sprite_object_ptr(s_sprite_order[j]);
//            sprite1Priority = s_sprite_priorities[s_sprite_order[j - 1]];
//            sprite2Priority = s_sprite_priorities[s_sprite_order[j - 1]];
//            sprite1Y = sprite1->pos.y;
//            sprite2Y = sprite2->pos.y;
//
//            if (sprite1Y >= DISPLAY_HEIGHT)
//                sprite1Y -= 256;
//
//            if (sprite2Y >= DISPLAY_HEIGHT)
//                sprite2Y -= 256;
//
//            if (sprite1->affine && sprite1->doubleSize && sprite1->size == OAM_SIZE_3)
//            {
//                shape = sprite1->shape;
//                if (shape == OAM_SQUARE || shape == OAM_V_RECTANGLE)
//                {
//                    if (sprite1Y > 128)
//                        sprite1Y -= 256;
//                }
//            }
//
//            if (sprite2->affine && sprite2->doubleSize && sprite2->size == OAM_SIZE_3)
//            {
//                shape = sprite2->shape;
//                if (shape == OAM_SQUARE || shape == OAM_V_RECTANGLE)
//                {
//                    if (sprite2Y > 128)
//                        sprite2Y -= 256;
//                }
//            }
//        }
//    }
//}
