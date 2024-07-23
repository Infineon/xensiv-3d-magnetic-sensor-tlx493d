// std includes
#include <malloc.h>
#include <stddef.h>

// project c includes
// common to all sensors
#include "tlx493d_types.h"

// common to same generation of sensors

// sensor specific includes

// project cpp includes
#include "types.h"
#include "BoardSupportUsingKit2Go.h"



bool tlx493d_initBoardSupport(TLx493D_t *sensor, Kit2GoBoardSupport *bsc) {
    sensor->boardSupportInterface.boardSupportObj.k2go_obj                = (TLx493D_Kit2GoBoardSupportObject_t *) malloc(sizeof(TLx493D_Kit2GoBoardSupportObject_t));
    sensor->boardSupportInterface.boardSupportObj.k2go_obj->k2go          = bsc;
    sensor->boardSupportInterface.boardSupportObj.k2go_obj->isToBeDeleted = false;
    return true;
}