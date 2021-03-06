#ifdef KITSCHY_DEBUG_MEMORY
#include "debug_memorymanager.h"
#endif


#ifdef _WIN32
#include "windows.h"
#endif

#include "assert.h"

#include "GLES/gl.h"
#include "SDL.h"
#include "SDL_image.h" 
// #include "SDL_ttf.h"

#include "auxiliar.h"
#include "2DCMC.h"
#include "GLTile.h"
#include "SDL_glutaux.h"

#include "debug.h"


GLTile::GLTile(char *fname)
{
    SDL_Surface *sfc = IMG_Load(fname);
#ifdef __DEBUG_MESSAGES
    if (!sfc) {
        output_debug_message("GLTile: Cannot open: %s\n",fname);
    }
#endif
    assert(sfc);

    if (sfc->format->BytesPerPixel != 4) {
        SDL_Surface *sfc2 = SDL_CreateRGBSurface(
                SDL_SWSURFACE, sfc->w, sfc->h, 32, RMASK, GMASK, BMASK, AMASK);
        SDL_SetAlpha(sfc2, 0, 0);
        SDL_BlitSurface(sfc, 0, sfc2, 0);
        SDL_FreeSurface(sfc);
        sfc = sfc2;
    }

    init(sfc);
} /* GLTile::GLTile */

GLTile::GLTile(SDL_Surface *sfc)
{
    assert(sfc);
    init(sfc);
} /* GLTile::GLTile */


GLTile::GLTile(SDL_Surface *sfc, int ax, int ay, int adx, int ady)
{
    assert(sfc);

    SDL_Surface *sfc2 = SDL_CreateRGBSurface(SDL_SWSURFACE, adx, ady, 32, RMASK, GMASK, BMASK, AMASK);
    SDL_Rect r = { .x = ax, .y = ay, .w = adx, .h = ady };
    SDL_SetAlpha(sfc, 0, 0);
    SDL_BlitSurface(sfc, &r, sfc2, 0);

    init(sfc2);
} /* GLTile::GLTile */


GLTile::~GLTile()
{
    SDL_FreeSurface(tile);
    glDeleteTextures(1, &tex);
}  /* GLTile::~GLTile */


void GLTile::init(SDL_Surface *sfc)
{
    clamp = false;
    smooth = false;

    g_dx = 0;
    g_dy = 0;
    hot_x = 0;
    hot_y = 0;

    tile = sfc;

    g_dx = tile->w;
    g_dy = tile->h;
    x = 0;
    y = 0;
    dx = tile->w;
    dy = tile->h;
    tex = createTexture(tile, &tex_coord_x, &tex_coord_y);

    compute_cmc();
} /* GLTile::set */


void GLTile::compute_cmc(void)
{
    cmc_x1 = 0; cmc_y1 = 0;
    cmc_x2 = 0; cmc_y2 = 0;
    bool first = true;
    for (int i = 0; i < g_dy; i++) {
        for (int j = 0; j < g_dx; j++) {
            Uint32 c = getpixel(tile, j, i);
            if ((c & AMASK) != 0) {
                if (first) {
                    first = false;
                    cmc_x1 = cmc_x2 = j;
                    cmc_y1 = cmc_y2 = i;
                } else {
                    if (j < cmc_x1) cmc_x1 = j;
                    if (j > cmc_x2) cmc_x2 = j;
                    if (i < cmc_y1) cmc_y1 = i;
                    if (i > cmc_y2) cmc_y2 = i;
                }
            }
        }
    }

    update_cmc();
} /* GLTile::compute_cmc */


void GLTile::set_clamp(void)
{
    clamp = true;
    smooth = false;

    tex = createTextureClamp(tile, &tex_coord_x, &tex_coord_y);
} /* GLTile::set_clamp */


void GLTile::set_smooth(void)
{
    clamp = false;
    smooth = true;

    tex = createTextureSmooth(tile, &tex_coord_x, &tex_coord_y);
} /* GLTile::set_smooth */


void GLTile::set_hotspot(int hx, int hy)
{
    hot_x = hx;
    hot_y = hy;

    update_cmc();
} /* GLTile::set_hotspot */


void GLTile::set_cmc(int x1, int y1, int x2, int y2)
{
    cmc_x1 = x1; cmc_y1 = y1;
    cmc_x2 = x2; cmc_y2 = y2;

    update_cmc();
} /* GLTile::set_cmc */


void GLTile::update_cmc(void)
{
    float x[2] = { cmc_x1 - hot_x, cmc_x2 - hot_x };
    float y[2] = { cmc_y1 - hot_y, cmc_y2 - hot_y };
    cmc.set(x, y, 2);
} /* GLTile::update_cmc */


void GLTile::draw(void)
{
    draw(1, 1, 1, 1);
} /* GLTile::draw */


void GLTile::draw(float r, float g, float b, float a)
{
    bool tmp;
    // bool tmp2;

    tmp = (glIsEnabled(GL_TEXTURE_2D) ? true : false);
    // tmp2=(glIsEnabled(GL_COLOR_MATERIAL) ? true:false);
    if (!tmp)
        glEnable(GL_TEXTURE_2D);
    // if (!tmp2) glEnable(GL_COLOR_MATERIAL);

    glBindTexture(GL_TEXTURE_2D, tex);

    glColor4f(r, g, b, a);
    glNormal3f(0.0, 0.0, 1.0);
#ifndef HAVE_GLES
    glBegin(GL_QUADS);
    glTexCoord2f(0, 0);
    glVertex3f(float(x - hot_x), float(y - hot_y), 0);

    glTexCoord2f(0, tex_coord_y);
    glVertex3f(float(x - hot_x), float(y + dy - hot_y), 0);

    glTexCoord2f(tex_coord_x, tex_coord_y);
    glVertex3f(float(x + dx - hot_x), float(y + dy - hot_y), 0);

    glTexCoord2f(tex_coord_x, 0);
    glVertex3f(float(x + dx - hot_x), float(y - hot_y), 0);

    glEnd();
#else
    GLfloat vtx1[] = {
        float(x - hot_x), float(y - hot_y), 0,
        float(x - hot_x), float(y + dy - hot_y), 0,
        float(x + dx - hot_x), float(y + dy - hot_y), 0,
        float(x + dx - hot_x), float(y - hot_y), 0
    };
    GLfloat tex1[] = {
        0, 0,
        0, tex_coord_y,
        tex_coord_x, tex_coord_y,
        tex_coord_x, 0
    };

    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);

    glVertexPointer(3, GL_FLOAT, 0, vtx1);
    glTexCoordPointer(2, GL_FLOAT, 0, tex1);
    glDrawArrays(GL_TRIANGLE_FAN,0,4);

    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
#endif

    // if (!tmp2) glDisable(GL_COLOR_MATERIAL);
    if (!tmp)
        glDisable(GL_TEXTURE_2D);
} /* GLTile::draw */



void GLTile::draw(float x, float y, float z, float angle, float scale)
{
    glPushMatrix();
    glTranslatef(x, y, z);
    if (scale != 1)
        glScalef(scale, scale, scale);
    if (angle != 0)
        glRotatef(angle, 0, 0, 1);

    draw();

    glPopMatrix();
} /* GLTile::draw */


void GLTile::draw(float r, float g, float b, float a, float x, float y, float z, float angle, float scale)
{
    glPushMatrix();
    glTranslatef(x, y, z);
    if (scale != 1)
        glScalef(scale, scale, scale);
    if (angle != 0)
        glRotatef(angle, 0, 0, 1);

    draw(r, g, b, a);

    glPopMatrix();
} // GLTile::draw



void GLTile::draw_toffs(float r, float g, float b, float a, float toffs_x, float toffs_y)
{
    bool tmp;
    // bool tmp2;
    int i;
    float real_x_offs, real_y_offs;

    tmp = (glIsEnabled(GL_TEXTURE_2D) ? true : false);
    // tmp2=(glIsEnabled(GL_COLOR_MATERIAL) ? true:false);
    if (!tmp)
        glEnable(GL_TEXTURE_2D);
    // if (!tmp2) glEnable(GL_COLOR_MATERIAL);

    real_x_offs = tex_coord_x * toffs_x;
    real_y_offs = tex_coord_y * toffs_y;
    glBindTexture(GL_TEXTURE_2D, tex);

    glColor4f(r, g, b, a);
    glNormal3f(0.0, 0.0, 1.0);
#ifndef HAVE_GLES
    glBegin(GL_QUADS);

    glTexCoord2f(real_x_offs, real_y_offs);
    glVertex3f(float(x - hot_x), float(y - hot_y), 0);

    glTexCoord2f(real_x_offs, real_y_offs + tex_coord_y);
    glVertex3f(float(x - hot_x), float(y + dy - hot_y), 0);

    glTexCoord2f(real_x_offs + tex_coord_x, real_y_offs + tex_coord_y);
    glVertex3f(float(x + dx - hot_x), float(y + dy - hot_y), 0);

    glTexCoord2f(real_x_offs + tex_coord_x, real_y_offs);
    glVertex3f(float(x + dx - hot_x), float(y - hot_y), 0);

    glEnd();
#else
    GLfloat vtx2[] = {
        float(x - hot_x), float(y - hot_y), 0,
        float(x - hot_x), float(y + dy - hot_y), 0,
        float(x + dx - hot_x), float(y + dy - hot_y), 0,
        float(x + dx - hot_x), float(y - hot_y), 0
    };
    GLfloat tex2[] = {
        real_x_offs, real_y_offs,
        real_x_offs, real_y_offs + tex_coord_y,
        real_x_offs + tex_coord_x, real_y_offs + tex_coord_y,
        real_x_offs + tex_coord_x, real_y_offs
    };
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);

    glVertexPointer(3, GL_FLOAT, 0, vtx2);
    glTexCoordPointer(2, GL_FLOAT, 0, tex2);
    glDrawArrays(GL_TRIANGLE_FAN,0,4);

    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);

#endif

    // if (!tmp2) glDisable(GL_COLOR_MATERIAL);
    if (!tmp)
        glDisable(GL_TEXTURE_2D);
} /* GLTile::draw_toffs */




void GLTile::draw_toffs(float r, float g, float b, float a, float x, float y, float z, float angle, float scale, float toffs_x, float toffs_y)
{
    glPushMatrix();
    glTranslatef(x, y, z);
    if (scale != 1)
        glScalef(scale, scale, scale);
    if (angle != 0)
        glRotatef(angle, 0, 0, 1);

    draw_toffs(r, g, b, a, toffs_x, toffs_y);

    glPopMatrix();
} // GLTile::draw_toffs



void GLTile::draw_cmc(void)
{
    cmc.draw(1, 1, 1, 1);
} /* GLTile::draw */


void GLTile::draw_cmc(float dx, float dy, float dz, float angle, float zoom)
{
    glPushMatrix();
    glTranslatef(dx, dy, dz);
    if (zoom != 1)
        glScalef(zoom, zoom, zoom);
    if (angle != 1)
        glRotatef(angle, 0, 0, 1);
    cmc.draw(1, 1, 1, 1);
    glPopMatrix();
} /* GLTile::draw */



void GLTile::draw_cmc(float r, float g, float b, float a)
{
    cmc.draw(r, g, b, a);
} /* GLTile::draw */


void GLTile::draw_cmc(float r, float g, float b, float a, float dx, float dy, float dz, float angle, float zoom)
{
    glPushMatrix();
    glTranslatef(dx, dy, dz);
    if (zoom != 1)
        glScalef(zoom, zoom, zoom);
    if (angle != 1)
        glRotatef(angle, 0, 0, 1);
    cmc.draw(r, g, b, a);
    glPopMatrix();
} /* GLTile::draw */
