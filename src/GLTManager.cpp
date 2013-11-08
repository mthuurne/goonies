#ifdef KITSCHY_DEBUG_MEMORY
#include "debug_memorymanager.h"
#endif


#ifdef _WIN32
#include "windows.h"
#else
#include "dirent.h"
#endif

#include "GLES/gl.h"
#include "SDL.h"
#include "SDL_image.h"

#include "stdlib.h"
#include "string.h"
#include "List.h"

#include "auxiliar.h"
#include "2DCMC.h"
#include "Symbol.h"
#include "GLTile.h"

#include "GLTManager.h"

#include "debug.h"


GLTManager::GLTManager()
{
    m_hash = new List<GLTManagerNode>[SYMBOL_HASH_SIZE];
    m_hash_smooth = new List<GLTManagerNode>[SYMBOL_HASH_SIZE];
} /* GLTManager::GLTManager */


GLTManager::~GLTManager()
{
    delete []m_hash;
    m_hash = 0;
    delete []m_hash_smooth;
    m_hash_smooth = 0;
} /* GLTManager::~GLTManager */


GLTile *GLTManager::get
    (char *name)
{
    Symbol *s = new Symbol(name);
    GLTile *t = get
                    (s);
    delete s;
    return t;
} /* GLTManager::get */


GLTile *GLTManager::get(Symbol *name)
{
    GLTManagerNode *n;
    int h = name->hash_function();

    m_hash[h].Rewind();
    while (m_hash[h].Iterate(n)) {
        if (n->m_name->cmp(name))
            return n->m_tile;
    } /* while */

    // load the tile:
    {
        char filename[256];
        GLTile *t;

        sprintf(filename, "graphics/%s.png", name->get());
        t = new GLTile(filename);

        t->set_clamp();
        n = new GLTManagerNode();
        n->m_tile = t;
        n->m_name = new Symbol(name);
        m_hash[h].Add(n);

        float fx = 1.0f, fy = 1.0f;

        sprintf(filename, "graphics/%s.nfo", name->get
                ());
        {
            FILE *fp;
            fp = fopen(filename, "r");
            if (fp != 0) {
                char tmp[80], tmp2[80];
                while (!feof(fp)) {
                    fgets(tmp, 80, fp);
                    if (1 == sscanf(tmp, "%s", tmp2)) {
                        if (strcmp(tmp2, "hs") == 0 ||
                                strcmp(tmp2, "HS") == 0) {
                            int hx, hy;
                            sscanf(tmp, "%s %i %i", tmp2, &hx, &hy);
                            t->set_hotspot((int)((hx / fx) + 0.5F), (int)((hy / fy) + 0.5F));
                        } // if
                        if (strcmp(tmp2, "bb") == 0 ||
                                strcmp(tmp2, "BB") == 0) {
                            int x1, y1, x2, y2;
                            sscanf(tmp, "%s %i %i %i %i", tmp2, &x1, &y1, &x2, &y2);
                            t->set_cmc((int)((x1 / fx) + 0.5F), (int)((y1 / fy) + 0.5F), (int)((x2 / fx) + 0.5F), (int)((y2 / fy) + 0.5F));
                        } // if
                    } // if
                } // while
                fclose(fp);
            } // if
        }

        return t;
    }
} /* GLTManager::get */



GLTile *GLTManager::get_smooth(char *name)
{
    Symbol *s = new Symbol(name);
    GLTile *t = get_smooth(s);
    delete s;
    return t;
} /* GLTManager::get_smooth */


GLTile *GLTManager::get_smooth(Symbol *name)
{
    GLTManagerNode *n;
    int h = name->hash_function();

    m_hash_smooth[h].Rewind();
    while (m_hash_smooth[h].Iterate(n)) {
        if (n->m_name->cmp(name))
            return n->m_tile;
    } /* while */

    // load the tile:
    {
        char filename[256];
        GLTile *t;

        sprintf(filename, "graphics/%s.png", name->get());
        t = new GLTile(filename);

        t->set_smooth();
        //  t->set_clamp();
        n = new GLTManagerNode();
        n->m_tile = t;
        n->m_name = new Symbol(name);
        m_hash_smooth[h].Add(n);

        sprintf(filename, "graphics/%s.nfo", name->get
                ());
        {
            float fx = 1.0f, fy = 1.0f;
            FILE *fp;
            fp = fopen(filename, "r");
            if (fp != 0) {
                char tmp[80], tmp2[80];
                while (!feof(fp)) {
                    fgets(tmp, 80, fp);
                    if (1 == sscanf(tmp, "%s", tmp2)) {
                        if (strcmp(tmp2, "hs") == 0 ||
                                strcmp(tmp2, "HS") == 0) {
                            int hx, hy;
                            sscanf(tmp, "%s %i %i", tmp2, &hx, &hy);
                            t->set_hotspot((int)((hx / fx) + 0.5F), (int)((hy / fy) + 0.5F));
                        } // if
                        if (strcmp(tmp2, "bb") == 0 ||
                                strcmp(tmp2, "BB") == 0) {
                            int x1, y1, x2, y2;
                            sscanf(tmp, "%s %i %i %i %i", tmp2, &x1, &y1, &x2, &y2);
                            t->set_cmc((int)((x1 / fx) + 0.5F), (int)((y1 / fy) + 0.5F), (int)((x2 / fx) + 0.5F), (int)((y2 / fy) + 0.5F));
                        } // if
                    } // if
                } // while
                fclose(fp);
            } // if
        }

        return t;
    }
} /* GLTManager::get_smooth */



// helper function for the cache
// filter the dir specified for only ogg/wav/mp3
#ifndef _WIN32
#ifdef __APPLE__
int filter_png(struct dirent *de)
{
#else
int filter_png(const struct dirent *de)
{
#endif //__APPLE__
    if (strcmp(".png", de->d_name + strlen(de->d_name) - 4) == 0)
    {
        return (1);
    } else
    {
        return (0);
    }
}
#endif

// helper function for the cache;
// remove file extensions, since Santi's Sound.cpp doesn't like 'em
char *GLTManager::remove_extension(char *filename)
{
    char *p = strrchr(filename, '\\');
    if (p != NULL) {
        p = strrchr(p + 1, '.');
    } else {
        p = strrchr(filename, '.');
    }
    if (p != NULL) {
        *p = '\0';
    }
    return filename;
}

// read specified dir for png and cache them
void GLTManager::cache(void)
{
    // FIXME: unfortunately the entire game has a hardcoded use of the sound/ dir...
    char path[256];
    char *filename;
    sprintf(path, "graphics");

#ifdef _WIN32

    WIN32_FIND_DATA finfo;
    HANDLE h;
    char pattern[256], tmp[256];
    sprintf(pattern, "%s/*.*", path);

    h = FindFirstFile(pattern, &finfo);
    if (h != INVALID_HANDLE_VALUE) {
        strcpy(tmp, finfo.cFileName);
        while (FindNextFile(h, &finfo) == TRUE) {
            if (
                (
                    finfo.cFileName[strlen(finfo.cFileName) - 1] == 'g' &&
                    finfo.cFileName[strlen(finfo.cFileName) - 2] == 'n' &&
                    finfo.cFileName[strlen(finfo.cFileName) - 3] == 'p'
                )
                &&
                finfo.cFileName[strlen(finfo.cFileName) - 4] == '.'
            ) {
                filename = remove_extension(finfo.cFileName);
                sprintf(tmp, "%s", filename);
                get(tmp);
            }
        }
    }
#else
    struct dirent **namelist;
    char tmp[512];
    int i, n;

    // get alpha sorted list of png names from "path"
    if ((n = scandir(path, &namelist, filter_png, alphasort)) > 0) {
        for (i = 0; i < n; i++) {
            filename = remove_extension(namelist[i]->d_name);
            snprintf(tmp, 512, "%s", filename);
            get (tmp);
        }
        free(namelist);
    }
#endif
}


GLTManagerNode::~GLTManagerNode()
{
	if (m_tile!=0) delete m_tile;
	m_tile=0;
	if (m_name!=0) delete m_name;
	m_name=0;
} /* GLTManagerNode::~GLTManagerNode */

