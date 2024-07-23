// std includes
#include <stdbool.h>
#include <stdio.h>

#include "unity_ifx.h"
#include "Logger.h"

// Method used by Unity to output a single character 
void putCharacter(char c) {
    printf("%c", c);
    flush();
}

// Method used by Unity to flush the output
void flushCharacter() {
    flush();
}

