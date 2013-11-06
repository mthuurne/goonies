// A few functions from GLU, adapted for GL ES (no doubles).

#ifndef __MICROGLU
#define __MICROGLU

void gluPerspective(float fovy, float aspect, float zNear, float zFar);
void gluLookAt(float eyex, float eyey, float eyez,
               float centerx, float centery, float centerz,
               float upx, float upy, float upz);

#endif // __MICROGLU
