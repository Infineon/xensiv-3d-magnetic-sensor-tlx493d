#ifndef TLX493D_BOARD_SUPPORT_USING_KIT2GO_HPP
#define TLX493D_BOARD_SUPPORT_USING_KIT2GO_HPP


// std includes
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

// project c includes
#include "Kit2GoBoardSupport.h"
// common to all sensors
#include "tlx493d_types.h"


bool tlx493d_initBoardSupport(TLx493D_t *sensor, Kit2GoBoardSupport *bsc);


#endif // TLX493D_BOARD_SUPPORT_USING_KIT2GO_HPP