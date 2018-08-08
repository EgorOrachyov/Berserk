//
// Created by Egor Orachyov on 06.08.2018.
//

#ifndef BERSERKENGINE_FILEREADINGTESTING_H
#define BERSERKENGINE_FILEREADINGTESTING_H

#include <cstdio>
#include <cstdlib>

void FileReadingTest()
{

    char buffer[256] = {0};
    char some_string[] = "f 43/2/43 7/90/12 45/98/1 11/12/13";

    float v1,v2,v3,v4, vt1,vt2,vt3,vt4, vn1,vn2,vn3,vn4;
    int num_of_scanned = -1;

    num_of_scanned = sscanf(some_string, "%s %f/%f/%f %f/%f/%f %f/%f/%f %f/%f/%f", buffer,
                            &v1, &vt1, &vn1,
                            &v2, &vt2, &vn2,
                            &v3, &vt3, &vn3,
                            &v4, &vt4, &vn4
    );

    printf("Scanned %i, scanned string %s \n", num_of_scanned, buffer);
}

#endif //BERSERKENGINE_FILEREADINGTESTING_H
