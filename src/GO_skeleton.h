#ifndef _THE_GOONIES_OBJECT_SKELETON
#define _THE_GOONIES_OBJECT_SKELETON

class GO_skeleton : public GO_enemy
{
    public:
        GO_skeleton(int x, int y, int sfx_volume, int facing);

        virtual bool cycle(VirtualController *k, GMap *map, int layer, class TheGoonies *game, GLTManager *GLTM, SFXManager *SFXM);
        virtual void draw(GLTManager *GLTM);

        virtual bool player_hit(int *experience, int *score);
        virtual int enemy_hit(void);

        virtual bool is_a(Symbol *c);
        virtual bool is_a(char *c);


    protected:

        int m_walking_channel;

        int m_facing;
        int m_time_since_last_bone;

        int m_turning_counter;
        int m_previous_facing;
};

#endif

