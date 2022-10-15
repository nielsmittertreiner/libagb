#ifndef GUARD_AGB_AGB_H
#define GUARD_AGB_AGB_H

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

#include "agb/defines.h"
#include "agb/io_regs.h"
#include "agb/types.h"

#include "bios/swi_affine.h"
#include "bios/swi_checksum.h"
#include "bios/swi_cpu_set.h"
#include "bios/swi_math.h"
#include "bios/swi_multiboot.h"
#include "bios/swi_reset.h"
#include "bios/swi_uncomp.h"
#include "bios/swi_wait.h"

#include "utility/benchmark.h"
#include "utility/math.h"
#include "utility/print.h"
#include "utility/random.h"
#include "utility/string.h"
#include "utility/task.h"

#include "hardware/input.h"
#include "hardware/irq.h"
#include "hardware/memory.h"

#include "gfx/bg.h"
#include "gfx/bg_bmp.h"
#include "gfx/palette.h"
#include "gfx/obj_vram_manager.h"
#include "gfx/obj_manager.h"

#endif // GUARD_AGB_AGB_H