#ifndef _BRAIN_KEYBOARDSTATE
#define _BRAIN_KEYBOARDSTATE

class KEYBOARDSTATE
{
    public:
        KEYBOARDSTATE();
        ~KEYBOARDSTATE();

        void copy(KEYBOARDSTATE *lvk);

        void cycle(void);

        bool save(FILE *fp);
        bool load(FILE *fp);

		bool key_press(int key);			/* returns if a key has been pressed				*/
											/* it returns true, just when the key is pressed	*/
											/* and keeps returning yes each 2 cycles after 20	*/  

        int n_joysticks;
        SDL_Joystick **joysticks;

        int joystick_0_pos, joystick_size; /* start of the first joystick and joystick size */
        int LVK_ID;

        int k_size;       /* number of inputs */
        unsigned char *old_keyboard;
        unsigned char *keyboard;
		int *time_pressed;					// amount of cycles that the key has been pressed
        List<SDL_keysym> keyevents;
};

#endif
