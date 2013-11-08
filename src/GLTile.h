#ifndef __BRAIN_GL_TILE
#define __BRAIN_GL_TILE

/* OpenGL HW accelerated 2D tiles */

class GLTile
{
    public:
        GLTile(char *fname);
        GLTile(SDL_Surface *sfc);
        GLTile(SDL_Surface *sfc, int x, int y, int dx, int dy);

        ~GLTile();

        void set_smooth(void);

        void set_clamp(void);

        void set_hotspot(int hx, int hy);
        void set_cmc(int x1, int y1, int x2, int y2);

        void draw(void);
        void draw(float x, float y, float z, float angle, float scale);

        void draw(float r, float g, float b, float a);
        void draw(float r, float g, float b, float a, float x, float y, float z, float angle, float scale);

        void draw_toffs(float r, float g, float b, float a, float x, float y, float z, float angle, float scale, float toffs_x, float toffs_y);

        void draw_cmc(void);
        void draw_cmc(float dx, float dy, float dz, float angle, float zoom);
        void draw_cmc(float r, float g, float b, float a);
        void draw_cmc(float r, float g, float b, float a, float dx, float dy, float dz, float angle, float zoom);

        int get_dx(void)
        {
            return g_dx;
        }
        int get_dy(void)
        {
            return g_dy;
        }

        class C2DCMC *get_cmc(void)
        {
                return cmc;
        };

    private:
        void init(SDL_Surface *sfc);

        void draw_toffs(float r, float g, float b, float a, float toffs_x, float toffs_y);

        Uint32 get_pixel(int x, int y);

        void compute_cmc(void);

        void free(void);

        bool smooth;
        bool clamp;

        int g_dx, g_dy;
        int hot_x, hot_y;

        SDL_Surface *tile;
        int x, y;
        int dx, dy;
        float tex_coord_x, tex_coord_y;
        GLuint tex;

        class C2DCMC *cmc;
};

#endif
