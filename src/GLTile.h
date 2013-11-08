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
        int get_n_parts(void)
        {
            return nparts;
        }

        class C2DCMC *get_cmc(void)
        {
                return cmc;
        };

        SDL_Surface *get_tile(int i)
        {
            return tile[i];
        }

        static int get_memory_used(void)
        {
            return memory_used;
        }
        static void recheck_textures(void);

    private:
        void set(char *fname);
        void set(SDL_Surface *sfc);

        void draw_toffs(float r, float g, float b, float a, float toffs_x, float toffs_y);

        Uint32 get_pixel(int x, int y);

        void compute_cmc(void);

        void load_textures(void);
        void free(void);

        static int required_texture_check;
        static int required_texture_reload;
        static int memory_used;

        int last_texture_check;

        int nparts;
        bool smooth;
        bool clamp;

        int g_dx, g_dy;
        int hot_x, hot_y;

        SDL_Surface **tile;
        int *x, *y;
        int *dx, *dy;
        float *tex_coord_x, *tex_coord_y;
        GLuint *tex;

        class C2DCMC *cmc;

        bool m_drawn; // This variable tells if the tile has been drawn at least one time
        // It is used for the following reason:
        // - the "draw" function of this class tries to detect when a texture is not in memory to reload all the textures
        // - however, in some opengl implementations a texture is not in memory till the first time it's drawn.
        // - thus if a tile has still not been drawn, it's normal that it is not in memory and the texture reload
        //   process should not start.
};

#endif
