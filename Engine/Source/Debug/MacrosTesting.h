//
// Created by Egor Orachyov on 26.06.2018.
//

#ifndef BERSERKENGINE_MACROSTESTING_H
#define BERSERKENGINE_MACROSTESTING_H

#include <stdio.h>

char buffer[512];
char msg_buffer[512];

#define ERROR(MSG, ...) \
{ \
    sprintf(msg_buffer, "[ERROR] (LINE %li:FUNCTION %s:FILE %s)", __LINE__, __FUNCTION__, __FILE__); \
    sprintf(buffer, MSG, ##__VA_ARGS__); \
    fprintf(stderr, "%s\n%s", msg_buffer, buffer); \
}

void error() {
    ERROR("This is the test method with: %i %i %i", 1, 2, 3);
}

#endif //BERSERKENGINE_MACROSTESTING_H
