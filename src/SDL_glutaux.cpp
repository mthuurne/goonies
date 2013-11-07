#ifdef KITSCHY_DEBUG_MEMORY
#include "debug_memorymanager.h"
#endif

#ifdef _WIN32
#include "windows.h"
#endif

#include <GLES/gl.h>
#define GL_CLAMP GL_CLAMP_TO_EDGE

#include "SDL.h"
#include "SDL_image.h"

#include "stdio.h"
#include "math.h"
#include <cassert>

#include "auxiliar.h"

#include "GLTile.h"
#include "SDL_glutaux.h"

#include "TheGooniesCtnt.h"

int nearest_2pow(int n)
{
    int res = 2;

    for (res = 2;res < 2048;res *= 2) {
        if (res >= n)
            return res;
    } /* for */

    return res;
} /* nearest_2pow */

static void unpackTextureFromSurface(SDL_Surface *sfc, float *tx, float *ty)
{
    int szx = nearest_2pow(sfc->w);
    int szy = nearest_2pow(sfc->h);
    *tx = (sfc->w) / float(szx);
    *ty = (sfc->h) / float(szy);

    SDL_Surface *sfc2 = SDL_CreateRGBSurface(SDL_SWSURFACE, szx, szy, 32,
                                             RMASK, GMASK, BMASK, AMASK);
    SDL_SetAlpha(sfc, 0, 0);
    SDL_BlitSurface(sfc, 0, sfc2, 0);

    if (sfc2->pitch == szx * 4) {
        glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
    } else if (sfc2->pitch == (szx + 1) * 4) {
        glPixelStorei(GL_UNPACK_ALIGNMENT, 8);
    } else {
        fprintf(stderr, "Cannot align: %d bytes of pixels with pitch %d\n",
                        szx * 4, sfc2->pitch);
        assert(false);
    }

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, szx, szy, 0, GL_RGBA,
                 GL_UNSIGNED_BYTE, sfc2->pixels);

    SDL_FreeSurface(sfc2);
}

GLuint createTexture(SDL_Surface *sfc, float *tx, float *ty)
{
    if (sfc != 0) {
        unsigned int tname = 0;
        glGenTextures(1, &tname);
        glBindTexture(GL_TEXTURE_2D, tname);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
#if VIEWPORT_W == SCREEN_X && VIEWPORT_H == SCREEN_Y
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
#else
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
#endif

        unpackTextureFromSurface(sfc, tx, ty);

        return tname;
    } else {
        return 0;
    } /* if */
} /* createTexture */


GLuint createTextureClamp(SDL_Surface *sfc, float *tx, float *ty)
{
    if (sfc != 0) {
        unsigned int tname = 0;
        glGenTextures(1, &tname);
        glBindTexture(GL_TEXTURE_2D, tname);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

#if VIEWPORT_W == SCREEN_X && VIEWPORT_H == SCREEN_Y
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
#else
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
#endif

        unpackTextureFromSurface(sfc, tx, ty);

        return tname;
    } else {
        return 0;
    }
} /* createTextureClamp */

GLuint createTextureSmooth(SDL_Surface *sfc, float *tx, float *ty)
{
    if (sfc != 0) {
        unsigned int tname = 0;
        glGenTextures(1, &tname);
        glBindTexture(GL_TEXTURE_2D, tname);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        //  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP);
        //  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP);
        //  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
        //  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

        unpackTextureFromSurface(sfc, tx, ty);

        return tname;
    } else {
        return 0;
    }
} /* createTextureSmooth */


GLuint createTextureClampSmooth(SDL_Surface *sfc, float *tx, float *ty)
{
    if (sfc != 0) {
        unsigned int tname = 0;
        glGenTextures(1, &tname);
        glBindTexture(GL_TEXTURE_2D, tname);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

        unpackTextureFromSurface(sfc, tx, ty);

        return tname;
    } else {
        return 0;
    }
} /* createTextureClampSmooth */


GLuint createTextureFromScreen(int x, int y, int dx, int dy, float *tx, float *ty)
{
    unsigned int tname = 0;
    int szx, szy;

    szx = nearest_2pow(dx);
    szy = nearest_2pow(dy);
    *tx = (dx) / float(szx);
    *ty = (dy) / float(szy);

    glGenTextures(1, &tname);
    glBindTexture(GL_TEXTURE_2D, tname);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP);
    // glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP);
    // glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    // glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    glCopyTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, x, y, dx, dy);

    return tname;
} /* createTextureFromScreen */


/*
 
void gl_print_left(int x,int y,char *text,int r,int g,int b,TTF_Font *font)
{
 SDL_Surface *sfc;
 SDL_Color c;
 GLTile *t;
 
 c.r=r;
 c.g=g;
 c.b=b;
 
 sfc=TTF_RenderText_Blended(font,text,c);
 t=new GLTile(sfc);
 t->draw(x,y,0,1.0);
 SDL_FreeSurface(sfc);
 delete t;
} // gl_print_left  
 
 
void gl_print_center(int x,int y,char *text,int r,int g,int b,TTF_Font *font)
{
 SDL_Surface *sfc;
 SDL_Color c;
 GLTile *t;
 
 c.r=r;
 c.g=g;
 c.b=b;
 
 sfc=TTF_RenderText_Blended(font,text,c);
 t=new GLTile(sfc);
 t->set_hotspot(sfc->w/2,0);
 t->draw(x,y,0,1.0);
 SDL_FreeSurface(sfc);
 delete t;
} // gl_print_center  
*/

void gl_line(int x1, int y1, int x2, int y2, float r, float g, float b)
{
    glColor4f(r, g, b, 1.0f);

#if !defined(HAVE_GLES)
     glBegin(GL_LINES);
     glVertex3f(float(x1), float(y1), 0);
     glVertex3f(float(x2), float(y2), 0);
     glEnd();
#else
    GLfloat line[] = {
        float(x1), float(y1), 0,
        float(x2), float(y2), 0
    };
 
    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(3, GL_FLOAT, 0, line);
    glDrawArrays(GL_LINES,0,2);
    glDisableClientState(GL_VERTEX_ARRAY);
#endif

} /* gl_line */
