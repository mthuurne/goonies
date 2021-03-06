#ifndef _THE_GOONIES_OBJECT_JUMPINGSKULL
#define _THE_GOONIES_OBJECT_JUMPINGSKULL

class GO_jumpingskull : public GO_skull
{
    public:
        GO_jumpingskull(int x, int y, int sfx_volume, int facing);

        virtual bool cycle(VirtualController *k, GMap *map, int layer, class TheGoonies *game, GLTManager *GLTM, SFXManager *SFXM);
        virtual void draw(GLTManager *GLTM);

        virtual bool player_hit(int *experience, int *score);
        virtual int enemy_hit(void);

        virtual bool is_a(Symbol *c);
        virtual bool is_a(char *c);

    protected:
        int m_original_y;
        float m_old_s;
};

#endif

