#ifndef TEST_INCLUDES_H
#define TEST_INCLUDES_H


// std includes
#include <stdbool.h>

// project c includes
#include "corelib/interfaces/c/CommunicationInterface.h"
#include "corelib/interfaces/c/Logger.h"

// common to all sensors
#include "corelib/interfaces/c//tlx493d.h"
#include "corelib/tlx493d/tlx493d_types.h"
#include "corelib/tlx493d/tlx493d_common_defines.h"
#include "corelib/tlx493d/tlx493d_common.h"

// common to same generation of sensors
#include "corelib/tlx493d/gen_2/tlx493d_gen_2_common_defines.h"
#include "corelib/tlx493d/gen_2/tlx493d_gen_2_common.h"

#include "corelib/tlx493d/gen_3/tlx493d_gen_3_common_defines.h"
#include "corelib/tlx493d/gen_3/tlx493d_gen_3_common.h"// Unity c includes
#include "unity.h"
#include "unity_fixture.h"

// IFX addons
#include "unity_ifx.h"


#endif // TEST_INCLUDES_H