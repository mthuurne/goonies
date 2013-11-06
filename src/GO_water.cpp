#ifdef KITSCHY_DEBUG_MEMORY
#include "debug_memorymanager.h"
#endif

#ifdef _WIN32
#include "windows.h"
#endif

#include "math.h"
#include "stdlib.h"
#include "string.h"

#include "GLES/gl.h"
#include "GL/glu.h"
#include "SDL.h"
#include "SDL_image.h"
#include "SDL_mixer.h"

#include "List.h"

#include "auxiliar.h"
#include "2DCMC.h"
#include "Symbol.h"
#include "GLTile.h"
#include "keyboardstate.h"

#include "GLTManager.h"
#include "SoundManager.h"
#include "SFXManager.h"
#include "GObject.h"
#include "GMap.h"

#include "GO_water.h"

#include "GObjectCreator.h"

extern bool water_reflection;

GO_water::GO_water(int x, int y, int sfx_volume) : GObject(x, y, sfx_volume)
{
    m_class = new Symbol(water_symbol);
}

bool GO_water::cycle(VirtualController *k, GMap *map, int layer, TheGoonies *game, GLTManager *GLTM, SFXManager *SFXM)
{
    m_state_cycle++;
    return true;
}

void GO_water::draw(GLTManager *GLTM)
{
	// we need different colors if the water reflection effect is off
// Commented out here, as waterreflections are not shown on pandora anyways
//	if (water_reflection)
//		glColor4f(0.3f, 0.3f, 1.0f, 0.5f);
//	else 
		glColor4f(0.11f, 0.11f, 0.35f, 0.5f);

//    glNormal3f(0.0, 0.0, 1.0);
#if !defined(HAVE_GLES)
    glBegin(GL_QUADS);
    {
        glVertex3f(m_x, m_y, 0);
        glVertex3f(m_x, m_y + 20, 0);
        glVertex3f(m_x + 20, m_y + 20, 0);
        glVertex3f(m_x + 20, m_y, 0);
    }
    glEnd();
#else
	GLfloat vtx6[] = {
	m_x, m_y, 0,
	m_x, m_y + 20, 0,
	m_x + 20, m_y + 20, 0,
	m_x + 20, m_y, 0
	};
      glEnableClientState(GL_VERTEX_ARRAY);
 
      glVertexPointer(3, GL_FLOAT, 0, vtx6);
      glDrawArrays(GL_TRIANGLE_FAN,0,4);
 
      glDisableClientState(GL_VERTEX_ARRAY);
#endif

/*	if (water_reflection) {
		bool tmp;
		
		GLuint tname = m_map->get_water_info_texture();
		
		tmp = (glIsEnabled(GL_TEXTURE_2D) ? true : false);
		if (!tmp) {
			glEnable(GL_TEXTURE_2D);
		}
		
		glBindTexture(GL_TEXTURE_2D, tname);
#if !defined(HAVE_GLES)
		glBegin(GL_QUADS);
		{
			float fx = m_x / 640.0f, fy = (400 - (400 - m_y) * 3) / 400.0f;
			float fdx = 20 / 640.0f, fdy = 20 / 133.33f;

			float fsx = float(sin(m_x + m_state_cycle * 0.1) * 0.02f);
			float fsx2 = float(sin(m_x + 20 + m_state_cycle * 0.1) * 0.02f);
			float fsy = float(sin(m_x + m_state_cycle * 0.15) * 0.01f);
			float fsy2 = float(sin(m_x + 20 + m_state_cycle * 0.15) * 0.01f);

			glTexCoord2f(fx + fsx, fy + fsy);
			glVertex3f(m_x, m_y, 0);

			glTexCoord2f(fx + fsx, fy + fdy + fsy2);
			glVertex3f(m_x, m_y + 20, 0);

			glTexCoord2f(fx + fdx + fsx2, fy + fdy + fsy2);
			glVertex3f(m_x + 20, m_y + 20, 0);

			glTexCoord2f(fx + fdx + fsx2, fy + fsy);
			glVertex3f(m_x + 20, m_y, 0);
		}
		glEnd();
#else
			float fx = m_x / 640.0f, fy = (400 - (400 - m_y) * 3) / 400.0f;
			float fdx = 20 / 640.0f, fdy = 20 / 133.33f;

			float fsx = float(sin(m_x + m_state_cycle * 0.1) * 0.02f);
			float fsx2 = float(sin(m_x + 20 + m_state_cycle * 0.1) * 0.02f);
			float fsy = float(sin(m_x + m_state_cycle * 0.15) * 0.01f);
			float fsy2 = float(sin(m_x + 20 + m_state_cycle * 0.15) * 0.01f);

			GLfloat vtx9[] = {
			m_x, m_y, 0,
			m_x, m_y + 20, 0,
			m_x + 20, m_y + 20, 0,
			m_x + 20, m_y, 0
			};
			GLfloat tex9[] = {
			fx + fsx, fy + fsy,
			fx + fsx, fy + fdy + fsy2,
			fx + fdx + fsx2, fy + fdy + fsy2,
			m_x + 20, m_y, 0
			};
      glEnableClientState(GL_VERTEX_ARRAY);
      glEnableClientState(GL_TEXTURE_COORD_ARRAY);
 
      glVertexPointer(3, GL_FLOAT, 0, vtx9);
      glTexCoordPointer(2, GL_FLOAT, 0, tex9);
      glDrawArrays(GL_TRIANGLE_FAN,0,4);
 
      glDisableClientState(GL_VERTEX_ARRAY);
      glDisableClientState(GL_TEXTURE_COORD_ARRAY);
#endif
		if (!tmp) {
			glDisable(GL_TEXTURE_2D);
		}
	} */
}

bool GO_water::is_a(Symbol *c)
{
    if (c->cmp(water_symbol))
        return true;

    return GObject::is_a(c);
}

bool GO_water::is_a(char *c)
{
    bool retval;
    Symbol *s = new Symbol(c);

    retval = is_a(s);

    delete s;

    return retval;
}
