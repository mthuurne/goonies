#ifdef KITSCHY_DEBUG_MEMORY
#include "debug_memorymanager.h"
#endif

#ifdef _WIN32
#include "windows.h"
#endif

#include "stdio.h"
#include "math.h"
#include "stdlib.h"
#include "string.h"

#include "GLES/gl.h"
#include "SDL.h"
#include "SDL_mixer.h"

#include "List.h"
#include "Symbol.h"
#include "2DCMC.h"
#include "auxiliar.h"
#include "GLTile.h"
#include "2DCMC.h"
#include "keyboardstate.h"
#include "randomc.h"
#include "VirtualController.h"

#include "GLTManager.h"
#include "SoundManager.h"
#include "SFXManager.h"
#include "MusicManager.h"
#include "GObject.h"
#include "GO_enemy.h"
#include "GO_fratelli.h"
#include "GMap.h"
#include "TheGoonies.h"
#include "TheGooniesApp.h"

#include "font_extractor.h"

#define GAME_FADE_IN_TIME 25

int TheGooniesApp::game_cycle(KEYBOARDSTATE *k)
{
    if (m_game_state == 0 && m_state_cycle >= GAME_FADE_IN_TIME) {
        m_game_state = 1;
    }
    if (m_game_state == 2) {
        if ((k->keyboard[SDLK_y] && !k->old_keyboard[SDLK_y])
#if defined(GCW)
            || (k->keyboard[SDLK_SPACE] && !k->old_keyboard[SDLK_SPACE])
#endif
        ) {
            m_game_state = 4;
            m_state_cycle = GAME_FADE_IN_TIME ;
        } else if ((k->keyboard[SDLK_n] && !k->old_keyboard[SDLK_n])
            || (k->keyboard[m_keys_configuration[GKEY_QUIT]] && !k->old_keyboard[m_keys_configuration[GKEY_QUIT]])
#if defined(GCW)
            || (k->keyboard[SDLK_LALT] && !k->old_keyboard[SDLK_LALT])
#endif
        ) {
            m_game_state = 3;
            m_state_cycle = 0;
            if (!m_game->game_paused()) m_SFXM->SFX_resume_continuous();
        }
    }

    if (m_game_state == 3 && m_state_cycle >= GAME_FADE_IN_TIME) {
        m_game_state = 1;
    }

    if (m_game_state == 4 && m_state_cycle >= GAME_FADE_IN_TIME*2) {
        m_title_state = 0;
        m_title_option_selected = 0;
        m_title_current_menu = 0;
        m_title_waiting_keystroke = false;
        m_title_next_menu = 0;
        return THEGOONIES_STATE_TITLE;
    }

    if (m_game_state == 4) {
        float f = 0;
        f = float(GAME_FADE_IN_TIME*2 - m_state_cycle) / (GAME_FADE_IN_TIME*2);
        m_game->set_music_volume((int)(m_music_volume*f),m_MusicM);
        m_game->set_sfx_volume((int)(m_sfx_volume*f));
        m_game->set_ambient_volume((int)(m_ambient_volume*f),m_MusicM);
    }

    if (k->keyboard[m_keys_configuration[GKEY_QUIT]] &&
            !k->old_keyboard[m_keys_configuration[GKEY_QUIT]] && m_game_state == 1) {
        if (!m_game->game_paused()) m_SFXM->SFX_pause_continuous();
        m_game_state = 2;
        m_state_cycle = 0;
    }

    if (m_game_state != 2) {
        m_vc->new_cycle();

        // we only take the first joystick into account
        // FIXME: this could be more generic...

        if (k->keyboard[m_keys_configuration[GKEY_UP]] || k->keyboard[(k->joystick_0_pos + 0 * k->joystick_size + 2)])
            m_vc->m_joystick[VC_UP] = true;
        else
            m_vc->m_joystick[VC_UP] = false;

        if (k->keyboard[m_keys_configuration[GKEY_RIGHT]] || k->keyboard[(k->joystick_0_pos + 0 * k->joystick_size + 1)])
            m_vc->m_joystick[VC_RIGHT] = true;
        else
            m_vc->m_joystick[VC_RIGHT] = false;

        if (k->keyboard[m_keys_configuration[GKEY_DOWN]] || k->keyboard[(k->joystick_0_pos + 0 * k->joystick_size + 3)])
            m_vc->m_joystick[VC_DOWN] = true;
        else
            m_vc->m_joystick[VC_DOWN] = false;

        if (k->keyboard[m_keys_configuration[GKEY_LEFT]] || k->keyboard[(k->joystick_0_pos + 0 * k->joystick_size)])
            m_vc->m_joystick[VC_LEFT] = true;
        else
            m_vc->m_joystick[VC_LEFT] = false;

        if (k->keyboard[m_keys_configuration[GKEY_FIRE]] || k->keyboard[(k->joystick_0_pos + 0 * k->joystick_size + 4)])
            m_vc->m_button[0] = true;
        else
            m_vc->m_button[0] = false;

        m_vc->m_button[1] = false;

        if (k->keyboard[m_keys_configuration[GKEY_PAUSE]])
            m_vc->m_pause = true;
        else
            m_vc->m_pause = false;
        if (k->keyboard[m_keys_configuration[GKEY_QUIT]])
            m_vc->m_quit = true;
        else
            m_vc->m_quit = false;

        if (!m_game->cycle(m_vc, m_GLTM, m_SFXM, m_MusicM)) {
            if (m_game->levelCompleteP()) {
                // Go to the level finished state ( ... )
                m_gameover_state = 0;
                m_interlevel_state = 0;
                m_interlevel_cycle = 0;
                if (m_current_level == 4)
                {
                    m_endsequence_speed = 0;
                    m_endsequence_shift = 0;
                    return THEGOONIES_STATE_ENDSEQUENCE;
                }
                else
                    return THEGOONIES_STATE_INTERLEVEL;
            } else {
                m_gameover_state = 0;
                m_MusicM->music_play(MUSIC_ID,"gameover",m_music_volume,0);
                return THEGOONIES_STATE_GAMEOVER;
            }
        }
    }

    return THEGOONIES_STATE_GAME;
}

static void draw_dark_overlay(float alpha)
{
    glEnable(GL_COLOR_MATERIAL);

    glColor4f(0, 0, 0, alpha);

    glNormal3f(0.0, 0.0, 1.0);
#ifndef HAVE_GLES
    glBegin(GL_QUADS);
    glVertex3f(0, 0, 0);
    glVertex3f(0, 480, 0);
    glVertex3f(640, 480, 0);
    glVertex3f(640, 0, 0);
    glEnd();
#else
    GLfloat vtx[] = {
        0, 0, 0,
        0, 480, 0,
        640, 480, 0,
        640, 0, 0
    };

    glEnableClientState(GL_VERTEX_ARRAY);

    glVertexPointer(3, GL_FLOAT, 0, vtx);
    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

    glDisableClientState(GL_VERTEX_ARRAY);
#endif
}

static void draw_quit_confirm(int t)
{
    draw_dark_overlay(float(t) / (GAME_FADE_IN_TIME * 2));

    int x = 320 - (GAME_FADE_IN_TIME - t) * 24;
    font_print_c(x, 150, 0, 0, 1, "font", "do you want to quit?");
    font_print_c(x, 200, 0, 0, 1,
#if defined(GCW)
            "font_hl", "y : quit / b : back"
#else
            "font_hl", "y / n"
#endif
            );
}

void TheGooniesApp::game_draw(void)
{
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT);

    m_game->draw(m_GLTM);

    switch (m_game_state) {
        case 0:
            draw_dark_overlay(float(GAME_FADE_IN_TIME - m_state_cycle)
                    / GAME_FADE_IN_TIME);
            break;
        case 2:
            draw_quit_confirm(min(m_state_cycle, GAME_FADE_IN_TIME));
            break;
        case 3:
            draw_quit_confirm(GAME_FADE_IN_TIME - m_state_cycle);
            break;
        case 4:
            draw_dark_overlay(float(m_state_cycle) / (GAME_FADE_IN_TIME * 2));
            break;
    }
}
