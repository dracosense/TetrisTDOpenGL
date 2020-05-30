#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <string>
#include <math.h>
#include <time.h>
#include <algorithm>
#include <map>
#include <vector>
#include <set>
#include <tuple>
#include <deque>

using namespace std;

const GLfloat msqsx = 1.0f, msqsy = 1.0f, msqsz = 1.0f; // map square size x, y ,x
const GLfloat mapdeep = 10.0f; // map deep
const GLfloat menudeep = 1.0f; // menu deep
const GLint msx = 20, msy = 24; // map size x, map size y
const GLint texturenum = 0;
const GLint inttextmax = 0;
const GLint acttowernum = 4;
const GLint startplayerlive = 10;
const GLdouble tupdatenum = 0.1f;
const GLdouble tupdateconst = 1.6d;
GLdouble tsellconst = 0.7d;
GLuint texture[texturenum];

GLfloat startstring[3] = {0.0f, 0.0f, 0.0f};
GLdouble mp[3] = {0.0f, 0.0f, 0.0f};
GLdouble wp[3] = {450.0f, 800.0f, 0.0f};
bool mb[3] = {false, false, false};
GLdouble mpos[3] = {0.0f, 0.0f, 0.0f}; // mouse pos
GLdouble wpu[3] = {0.0f, 0.0f, 0.0f}; // up window pos
GLdouble wpd[3] = {0.0f, 0.0f, 0.0f}; // down window pos

bool addtime = false;
GLint dtime = 0;
GLint mgametime = 0;

GLint movetime = 0;
GLint movetimer = 1000;

GLint creaturesteptime = 0;
GLint creaturesteptimer = 1000;

GLint createwavetime = 0;

GLint wavetime = 0;
GLint wavetimer = 1000;

GLint playerlive = startplayerlive;

GLint worldid = 0;

bool addnewobj = false;

GLint addtowerf = -1;

GLint gameres = 0;

GLint acttobj = -1, acttn = -1; // act tower obj, act tower n

GLint attackdrawtime = 0;
GLint attackdrawtimer = 250;

string waytoapp;

void square2d(GLfloat X, GLfloat Y, GLfloat X2, GLfloat Y2)
{
	glBegin(GL_QUADS);
	glVertex2d(X, Y);
	glVertex2d(X, Y2);
	glVertex2d(X2, Y2);
	glVertex2d(X2, Y);
	glEnd();

}

void square(GLfloat X, GLfloat Y, GLfloat Z, GLfloat X2, GLfloat Y2, GLfloat Z2, GLfloat R, GLfloat G, GLfloat B, GLfloat A)
{
	glBegin(GL_QUADS);
	glColor4f(R, G, B, A);
	glVertex3d(X, Y, Z);
	if (X == X2)
	{
		glVertex3d(X, Y2, Z);
		glVertex3d(X2, Y2, Z2);
		glVertex3d(X, Y, Z2);
	}
	if (Y == Y2)
	{
		glVertex3d(X2, Y, Z);
		glVertex3d(X2, Y2, Z2);
		glVertex3d(X, Y, Z2);
	}
	if (Z == Z2)
	{
		glVertex3d(X2, Y, Z);
		glVertex3d(X2, Y2, Z2);
		glVertex3d(X, Y2, Z);
	}
	glEnd ();
}

void square(GLfloat X, GLfloat Y, GLfloat Z, GLfloat X2, GLfloat Y2, GLfloat Z2, GLfloat R, GLfloat G, GLfloat B)
{
	square(X, Y, Z, X2, Y2, Z2, R, G, B, 1.0f);
}

void square(GLfloat X, GLfloat Y, GLfloat Z, GLfloat R, GLfloat G, GLfloat B)
{
	square(0.0f, 0.0f, 0.0f, X, Y, Z, R, G ,B);
}

void square(GLfloat X, GLfloat Y, GLfloat Z, GLfloat R, GLfloat G, GLfloat B, GLfloat A)
{
	square(0.0f, 0.0f, 0.0f, X, Y, Z, R, G, B, A);
}

void tsquare(GLint tname, GLfloat X, GLfloat Y, GLfloat Z, GLfloat X2, GLfloat Y2, GLfloat Z2,
			GLfloat X3, GLfloat Y3, GLfloat Z3, GLfloat X4, GLfloat Y4, GLfloat Z4)
{
	glBindTexture(GL_TEXTURE_2D, texture[tname]);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f); glVertex3d (X, Y, Z);
	glTexCoord2f(0.0f, 1.0f); glVertex3d (X2, Y2, Z2);
	glTexCoord2f(1.0f, 1.0f); glVertex3d (X3, Y3, Z3);
	glTexCoord2f(1.0f, 0.0f); glVertex3d (X4, Y4, Z4);
	glEnd();
}

void tsquare(GLfloat X, GLfloat Y, GLfloat Z,  GLfloat X2, GLfloat Y2, GLfloat Z2)
{
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f); glVertex3d(X, Y, Z);
	if (X == X2)
	{
		glTexCoord2f(0.0f, 1.0f); glVertex3d(X, Y2, Z);
		glTexCoord2f(1.0f, 1.0f); glVertex3d(X, Y2, Z2);
		glTexCoord2f(1.0f, 0.0f); glVertex3d(X, Y, Z2);
	}
	if (Y == Y2)
	{
		glTexCoord2f(0.0f, 1.0f); glVertex3d(X2, Y, Z);
		glTexCoord2f(1.0f, 1.0f); glVertex3d(X2, Y, Z2);
		glTexCoord2f(1.0f, 0.0f); glVertex3d(X, Y, Z2);
	}
	if (Z == Z2)
	{
		glTexCoord2f(0.0f, 1.0f); glVertex3d(X, Y2, Z);
		glTexCoord2f(1.0f, 1.0f); glVertex3d(X2, Y2, Z);
		glTexCoord2f(1.0f, 0.0f); glVertex3d(X2, Y, Z);
	}
	glEnd();
}

void tsquare(GLfloat X, GLfloat Y, GLfloat X2, GLfloat Y2)
{
	if (X2 < X)
	{
		swap(X, X2);
	}
	if (Y2 < Y)
	{
		swap(Y2, Y);
	}
	glBegin (GL_QUADS);
	glTexCoord2f(0.0f, 0.0f); glVertex2d(X, Y);
	glTexCoord2f(0.0f, 1.0f); glVertex2d(X, Y2);
	glTexCoord2f(1.0f, 1.0f); glVertex2d(X2, Y2);
	glTexCoord2f(1.0f, 0.0f); glVertex2d(X2, Y);
	glEnd();
}

void line(GLfloat X, GLfloat Y, GLfloat Z, GLfloat X2, GLfloat Y2, GLfloat Z2, GLfloat width)
{
	glLineWidth (width);
	glBegin(GL_LINES);
	glVertex3d(X, Y, Z);
	glVertex3d(X2, Y2, Z2);
	glEnd();
}

void line(GLfloat X, GLfloat Y, GLfloat X2, GLfloat Y2, GLfloat width)
{
	line(X, Y, 0.0f, X2, Y2, 0.0f, width);
}

void squareline(GLfloat X, GLfloat Y, GLfloat X2, GLfloat Y2, GLfloat W)
{
	line(X, Y, X, Y2, W);
	line(X, Y, X2, Y, W);
	line(X2, Y, X2, Y2, W);
	line(X, Y2, X2, Y2, W);
}

void tsquare(GLint tname, GLfloat X, GLfloat Y, GLfloat Z, GLfloat X2, GLfloat Y2, GLfloat Z2)
{
	glBindTexture(GL_TEXTURE_2D, texture[tname]);
	tsquare(X, Y, Z, X2, Y2, Z2);
}

void tsquare(GLint tname1, GLint tname2, GLfloat X, GLfloat Y, GLfloat Z, GLfloat X2, GLfloat Y2, GLfloat Z2)
{
	glEnable(GL_BLEND);
	glDisable(GL_DEPTH_TEST);
	glBlendFunc(GL_DST_COLOR, GL_ZERO);
	glBindTexture(GL_TEXTURE_2D, texture[tname1]);
	tsquare(X, Y, Z, X2, Y2, Z2);
	glBlendFunc(GL_ONE, GL_ONE);
	glBindTexture(GL_TEXTURE_2D, texture[tname2]);
	tsquare(X, Y, Z, X2, Y2, Z2);
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_BLEND);
}

void tsquare(GLint tname, GLfloat X, GLfloat Y, GLfloat X2, GLfloat Y2)
{
	glBindTexture(GL_TEXTURE_2D, texture[tname]);
	tsquare(X, Y, X2, Y2);
}

void tsquare(GLint tname1, GLint tname2, GLfloat X, GLfloat Y, GLfloat X2, GLfloat Y2)
{
	tsquare(tname1, tname2, X, Y, 0.0f, X2, Y2, 0.0f);
}

void initGL()
{
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClearDepth(100.0f);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_2D);
	glDepthFunc(GL_LEQUAL);
	glShadeModel(GL_SMOOTH);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
}

void loadtexture()
{
	GLint i = 0;
	char *a[texturenum] = {};
	for (i = 0; i < texturenum; i++)
	{
		int imagepos = 0, imagew = 0, imageh = 0, imagesize = 0;
		unsigned char imagedata_s[54];
		unsigned char *imagedata;
		FILE *imagefile = fopen(a[i], "rb");
		fread (imagedata_s, sizeof(char), 54, imagefile);
		imagew = *(int*)&(imagedata_s[0x12]);
		imageh = *(int*)&(imagedata_s[0x16]);
		imagesize = *(int*)&(imagedata_s[0x22]);
		imagepos = *(int*)&(imagedata_s[0x0A]);
		if (imagesize == 0)
		{
			imagesize = imagew * imageh * 4;// RGBA
		}
		if (imagepos == 0)
		{
			imagepos = 54;
		}
		imagedata = (unsigned char*)malloc(imagesize * sizeof(unsigned char));
		fread (imagedata, sizeof(char), imagesize, imagefile);
		for (GLint i = 0; (i + 2) < imagesize; i+=4)
		{
			swap(imagedata[i], imagedata[i + 2]);
		}
		fclose(imagefile);
		glGenTextures(1, &texture[i]);
		glBindTexture(GL_TEXTURE_2D, texture[i]);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexImage2D(GL_TEXTURE_2D, 0, 4, imagew, imageh, 0, GL_RGBA, GL_UNSIGNED_BYTE, imagedata);
		free(imagedata);
    }
}

void text(string s)
{
	GLint i = 0;
	for (i = 0; i < s.size(); i++)
	{
		if (s[i] == '\n')
		{
			startstring[1] -= 0.007f;
			glRasterPos3f(startstring[0], startstring[1], startstring[2]);
		}
		else
		{
			glutBitmapCharacter (GLUT_BITMAP_TIMES_ROMAN_24, s[i]);
		}
	}
}

void text(GLint s)
{
	GLint i = 0, a = 0, b = 100000LL;
	char c;
	bool d = false;
	for (i = inttextmax; i > 0; i--)
	{
		a = (s / b) % 10;
		c = '0' + a;
		if (a > 0 || d)
		{
			d = true;
			glutBitmapCharacter (GLUT_BITMAP_TIMES_ROMAN_24, c);
		}
		b /= 10;
	}
	if (d == false)
	{
		glutBitmapCharacter (GLUT_BITMAP_TIMES_ROMAN_24, '0');
	}
}

void text(string s, GLfloat tx, GLfloat ty, GLfloat tz)
{
	glRasterPos3f (tx, ty, tz + 0.0001f);
	startstring[0] = tx;
	startstring[1] = ty;
	startstring[2] = tz + 0.0001f;
	text(s);
}

void text(GLint s, GLfloat tx, GLfloat ty, GLfloat tz)
{
	glRasterPos3f (tx, ty, tz + 0.0001f);
	startstring[0] = tx;
	startstring[1] = ty;
	startstring[2] = tz + 0.0001f;
	text(s);
}

class color
{
public:
    GLfloat r, g, b, a;
    color()
    {
        r = 0.0f;
        g = 0.0f;
        b = 0.0f;
        a = 1.0f;
    }
    void getc(GLfloat r2, GLfloat g2, GLfloat b2)
    {
        r = r2;
        g = g2;
        b = b2;
    }
    void setc()
    {
        glColor4f(r, g, b, a);
    }
    ~color()
    {

    }
};

class button
{
public:
	GLfloat bx, by, bsx, bsy;
	bool clickb;
	color onc, offc;
	button(GLfloat bposx, GLfloat bposy, GLfloat bx2, GLfloat by2)
	{
		clickb = false;
		bx = bposx;
		by = bposy;
		bsx = bx2;
		bsy = by2;
		onc.r = 0.6f;
		onc.g = 0.6f;
		onc.b = 0.6f;
		offc.r = 0.1f;
		offc.g = 0.1f;
		offc.b = 0.1f;
	}
	bool bclick(GLfloat mx, GLfloat my)
	{
		if (mx >= bx && mx < bsx && my >= by && my < bsy)
		{
			clickb = true;
			if (mb[0])
			{
				mb[0] = false;
				return true;
			}
		}
		return false;
	}
	void drawb()
	{
		if (clickb)
		{
			onc.setc();
		}
		else
        {
            offc.setc();
        }
		square2d(bx, by, bsx, bsy);
	}
	~button()
	{
	}
};

class objc // obj color
{
public:
    color c;
    objc()
    {

    }
    ~objc()
    {

    }
};

class tower
{
public:
    bool f; // flag
    GLint time, timer;
    GLdouble r, cost; // range, cost
    GLdouble live, attack, shield, startlive; // live, attack, shield, startlive
    GLint utpcost, utrcost, utlcost; // update tower cost: param, range, add live
    color c;
    tower(GLint r2 = 0, GLint timer2 = 1000, GLdouble live2 = 0.0f, GLdouble attack2 = 0.0f, GLdouble shield2 = 0.0f, GLint cost2 = 0, GLint utpcost2 = 0, GLint utrcost2 = 0, GLint utlcost2 = 0)
    {
        f = false;
        r = r2;
        time = 0;
        timer = timer2;
        live = live2;
        startlive = live;
        attack = attack2;
        shield = shield2;
        c.r = 1.0f;
        c.g = 1.0f;
        c.b = 1.0f;
        cost = cost2;
        utpcost = utpcost2;
        utrcost = utrcost2;
        utlcost = utlcost2;
    }
    ~tower()
    {

    }
};

class towerobj
{
public:
    tower t;
    towerobj()
    {
    }
    ~towerobj()
    {

    }
};

class elem
{
public:
    GLint x, y;
    elem()
    {
        x = 0;
        y = 0;
    }
    ~elem()
    {

    }
};

class melem
{
public:
    GLint x, y; // pos x, pos y
    color c; // color
    tower t; // tower
    melem()
    {
        x = 0;
        y = 0;
    }
    ~melem()
    {

    }
};

class obj
{
public:
    GLint x, y;
    vector<elem> p;
    obj()
    {
        x = 0;
        y = 0;
    }
    ~obj()
    {

    }
};

class mobj // obj
{
public:
    GLint x, y;
    vector<melem> p;
    mobj()
    {
        x = 0;
        y = 0;
    }
    ~mobj()
    {

    }
};

class mplace
{
public:
    bool canmove;
    GLint obj, n;
    mplace()
    {
        canmove = false;
        obj = -1;
        n = -1;
    }
    ~mplace()
    {

    }
};

class creature
{
public:
    bool f, movef; // flag
    GLint range, time, timer; // range, time, timer
    pair<GLint, GLint> ppos; //previous pos x and previous pos y
    GLdouble attack, live, shield, speed, startlive; // attack, live, shield, speed, attack speed, start live
    color c;
    GLint cost;
    creature(GLint range2 = 0, GLint timer2 = 1000, GLdouble attack2 = 0.0f, GLdouble live2 = 0.0f, GLdouble shield2 = 0.0f, GLdouble speed2 = 1.0f, GLint cost2 = 0)
    {
        f = false;
        movef = false;
        range = range2;
        time = 0;
        timer = timer2;
        attack = attack2;
        live = live2;
        startlive = live;
        shield = shield2;
        speed = speed2;
        ppos = {-1, -1};
        cost = cost2;
        c.r = 1.0f;
        c.g = 0.0f;
        c.b = 0.0f;
    }
    ~creature()
    {

    }
};

class wave
{
public:
    vector<pair<GLint, GLint> > c; // creatures
    GLint timer; // time from previous wave to this wave
    GLint pos; // pos of wave
    GLint n; // num of creatures in wave
    wave()
    {
        timer = 0;
        pos = 0;
        n = 0;
    }
    ~wave()
    {

    }
};

class attackview
{
public:
    bool b; // true - attack creature/ false - attack tower
    pair<GLint, GLint> t; // tower
    pair<GLint, GLint> c; // creature
    attackview(pair<GLint, GLint> t2, pair<GLint, GLint> c2, bool b2)
    {
        t = t2;
        c = c2;
        b = b2;
    }
    ~attackview()
    {

    }
};

vector<vector<mplace> > gmap(msx, vector<mplace> (msy)); // game map
vector<vector<creature> > cmap(msx, vector<creature> (msy)); // creature map
vector<objc> gamec; // game color
vector<obj> gameobj; // game objects
map<long, mobj> mapobj; // objects on map
vector<towerobj> acttowers(acttowernum);
vector<deque<creature> > wavem(msx);
vector<creature> creaturetype;
deque<wave> wavecreatem;
vector<attackview> attackdrawm;
mobj actobj;

void searchwpos(GLdouble &mx, GLdouble &my, GLdouble &mz, GLdouble *winpos)
{
    GLint vp[4]; // viewport
    GLdouble p[16]; // projection
    GLdouble mv[16]; // modelview
    glGetIntegerv(GL_VIEWPORT, vp);
    glGetDoublev(GL_PROJECTION_MATRIX, p);
    glGetDoublev(GL_MODELVIEW_MATRIX, mv);
    glReadPixels(winpos[0], wp[1] - winpos[1] - 1, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &winpos[2]);
    gluUnProject(winpos[0], wp[1] - winpos[1] - 1, winpos[2], mv, p, vp, &mx, &my, &mz);
    mx *= 100.0f;
    my *= 100.0f;
    mz *= 100.0f;
}

void searchmpos(GLdouble &mx, GLdouble &my, GLdouble &mz)
{
    searchwpos(mx, my, mz, mp);
}

void initmpos()
{
    searchmpos(mpos[0], mpos[1], mpos[2]);
}

void initwpos()
{
    GLdouble wpnull[3] = {0.0f, 0.0f, 0.0f}; // null window pos
    searchwpos(wpu[0], wpd[1], wpd[2], wp);
    searchwpos(wpd[0], wpu[1], wpu[2], wpnull);
}

void settime()
{
	if (time(NULL) > dtime)
	{
		dtime = time(NULL);
		addtime = true;
		dtime = time(NULL);
	}
}

void breaktime()
{
	addtime = false;
}

void drawlive(GLdouble a, GLdouble b, GLdouble c, GLdouble d, GLdouble &live, GLdouble &startlive)
{
    if (live > startlive)
    {
        startlive = live;
    }
    glColor3f(0.4f, 0.4f, 0.4f);
    square2d(a, d, c, d - (d - b) * 1.0f / 5.0f);
    glColor3f(0.0f, 1.0f, 0.0f);
    square2d(a, d, a + (c - a) * (live / startlive), d - (d - b) * 1.0f / 5.0f);
    glColor3f(0.0f, 0.0f, 0.0f);
    squareline(a, d, c, d - (d - b) * 1.0f / 5.0f, 1.0f);
}

bool addobj(GLint x) // add obj to map
{
    bool b = true;
    x -= actobj.x / 2;
    if (x >= 0 && x + actobj.x <= msx)
    {
        for (long i = 0; i < actobj.p.size(); i++)
        {
            if (gmap[actobj.p[i].x + x][actobj.p[i].y].obj != -1)
            {
                b = false;
                break;
            }
        }
    }
    else
    {
        b = false;
    }
    if (b)
    {
        for (long i = 0; i < actobj.p.size(); i++)
        {
            actobj.p[i].x += x;
        }
        mapobj[worldid] = actobj;
        worldid++;
        for (long i = 0; i < actobj.p.size(); i++)
        {
            gmap[actobj.p[i].x][actobj.p[i].y].obj = worldid - 1;
            gmap[actobj.p[i].x][actobj.p[i].y].n = i;
        }
        return true;
    }
    else
    {
        return false;
    }
}

void newobj() // add new obj
{
    mobj a;
    GLint b = rand() % gameobj.size();
    GLint c = rand() % gamec.size();
    melem d;
    actobj = a;
    actobj.x = gameobj[b].x;
    actobj.y = gameobj[b].y;
    for (long i = 0; i < gameobj[b].p.size(); i++)
    {
        d.x = gameobj[b].p[i].x;
        d.y = gameobj[b].p[i].y;
        d.c = gamec[c].c;
        actobj.p.push_back(d);
    }
}

void moveobj(GLint objn)
{
    bool b = true;
    for (long i = 0; i < mapobj[objn].p.size(); i++)
    {
        if (gmap[mapobj[objn].p[i].x][mapobj[objn].p[i].y].canmove == false)
        {
            b = false;
        }
        gmap[mapobj[objn].p[i].x][mapobj[objn].p[i].y].canmove = false;
    }
    if (b)
    {
        for (long i = 0; i < mapobj[objn].p.size(); i++)
        {
            mapobj[objn].p[i].y++;
            gmap[mapobj[objn].p[i].x][mapobj[objn].p[i].y].obj = objn;
            gmap[mapobj[objn].p[i].x][mapobj[objn].p[i].y].n = i;
        }
    }
}

void placemove()
{
    for (long i = msx - 1; i >= 0; i--)
    {
        for (long j = msy - 1; j >= 0; j--)
        {
            if (gmap[i][j].obj != -1 && j != msy - 1 && (gmap[i][j + 1].obj == -1 || gmap[i][j + 1].obj == gmap[i][j].obj/*|| gmap[i][j + 1].canmove*/))
            {
                gmap[i][j].canmove = true;
            }
            else
            {
                gmap[i][j].canmove = false;
            }
        }
    }
}

void clearmap()
{
    for (long i = 0; i < msx; i++)
    {
        for (long j = 0; j < msy; j++)
        {
            if (gmap[i][j].obj >= 0)
            {
                if(mapobj.count(gmap[i][j].obj) == 0 || mapobj[gmap[i][j].obj].p[gmap[i][j].n].x != i || mapobj[gmap[i][j].obj].p[gmap[i][j].n].y != j)
                {
                    gmap[i][j].obj = -1;
                    gmap[i][j].n = -1;
                }
            }
            if (gmap[i][j].obj == -2)
            {
                gmap[i][j].obj = -1;
                gmap[i][j].obj = -1;
            }
        }
    }
}

void clearmapr() // claer map rows // ??
{
    creature a;
    bool b = false;
    for (long j = 0; j < (msy - 1); j++) // dont clear first up line ??
    {
        b = true;
        for (long i = 0; i < msx; i++)
        {
            if (gmap[i][j].obj == -1)
            {
                b = false;
                break;
            }
        }
        if (b)
        {
            for (long i = 0; i < msx; i++)
            {
                if ((mapobj[gmap[i][j].obj].p.size() - 1) > gmap[i][j].n)
                {
                    swap(mapobj[gmap[i][j].obj].p[gmap[i][j].n], mapobj[gmap[i][j].obj].p.back());
                }
                mapobj[gmap[i][j].obj].p.pop_back();
                if (mapobj[gmap[i][j].obj].p.size() == 0)
                {
                    mapobj.erase(gmap[i][j].obj);
                }
                cmap[i][j] = a;
                gmap[i][j].obj = -1;
                gmap[i][j].n = -1;
            }
        }
    }
}

void deletetower(GLint obj, GLint n)
{
    if (mapobj[obj].p[n].t.live <= 0 && mapobj[obj].p[n].t.f)
    {
        tower a;
        a.f = false;
        mapobj[obj].p[n].t = a;
    }
}

void towerattack(GLint obj, GLint n)
{
    melem *a = &mapobj[obj].p[n];
    vector<pair<GLint, GLint> > b;
    if (a->t.time + a->t.timer <= mgametime)
    {
        for (long i = a->x - a->t.r; i <= a->x + a->t.r; i++)
        {
            if (i >= 0 && i < msx)
            {
                for (long j = a->y - a->t.r; j <= a->y + a->t.r; j++)
                {
                    if (j >= 0 && j < msy)
                    {
                        if (cmap[i][j].f)
                        {
                            b.push_back({j, i});
                        }
                    }
                }
            }
        }
        sort(b.begin(), b.end());
        if (b.size() > 0)
        {
            attackview c({obj, n}, {b[0].second, b[0].first}, true);
            attackdrawm.push_back(c);
            cmap[b[0].second][b[0].first].live -= a->t.attack / (1.0f + cmap[b[0].second][b[0].first].shield);
        }
        a->t.time = mgametime;
    }
}

void deletemobj()
{
    for (auto it = mapobj.begin(); it != mapobj.end(); it++)
    {
        if (it->second.p.size() == 0)
        {
            auto a = it;
            it++;
            mapobj.erase(a);
            it--;
        }
        else
        {
            for (long i = 0; i < it->second.p.size(); i++)
            {
                deletetower(it->first, i);
            }
        }
    }
}

void mobjattack()
{
    for (auto it = mapobj.begin(); it != mapobj.end(); it++)
    {
        for (long i = 0; i < (it->second).p.size(); i++)
        {
            towerattack(it->first, i);
        }
    }
}

void mobjstep()
{
    deletemobj();
    if (movetime + movetimer <= mgametime)
    {
        placemove();
        for (auto it = mapobj.begin(); it != mapobj.end(); it++)
        {
            moveobj(it->first);
        }
        clearmap();
        clearmapr();
        movetime = mgametime;
    }
    mobjattack();
}

void drawplace(GLdouble a, GLdouble b, GLdouble c, GLdouble d, melem &x)
{
    x.c.setc();
    square2d(a, b, c, d);
    if (x.t.f)
    {
        x.t.c.setc();
        square2d(a + (c - a) / 4.0f, b + (d - b) / 4.0f, a + (c - a) * 3.0f / 4.0f, b + (d - b) * 3.0f / 4.0f);
        drawlive(a, b, c, d, x.t.live, x.t.startlive);
    }
}

void deletecreature(GLint x, GLint y)
{
    if (cmap[x][y].live <= 0)
    {
        gameres += cmap[x][y].cost;
        creature a;
        cmap[x][y] = a;
    }
}

void creatureattack(GLint x, GLint y)
{
    vector<pair<GLint, pair<GLint, GLint> > > a;
    if (cmap[x][y].time + cmap[x][y].timer <= mgametime)
    {
        for (long i = x - cmap[x][y].range; i <= x + cmap[x][y].range; i++)
        {
            if (i >= 0 && i < msx)
            {
                for (long j = y - cmap[x][y].range; j <= y + cmap[x][y].range; j++)
                {
                    if (j >= 0 && j < msy)
                    {
                        if (gmap[i][j].obj >= 0 && mapobj[gmap[i][j].obj].p[gmap[i][j].n].t.f)
                        {
                            a.push_back({max(i - x, x - i) + max(j - y, y - j), {gmap[i][j].obj, gmap[i][j].n}});
                        }
                    }
                }
            }
        }
        sort(a.begin(), a.end());
        if (a.size() > 0)
        {
            attackview b({a[0].second.first, a[0].second.second}, {x, y}, false);
            attackdrawm.push_back(b);
            mapobj[a[0].second.first].p[a[0].second.second].t.live -= cmap[x][y].attack / (1.0f + mapobj[a[0].second.first].p[a[0].second.second].t.shield);
        } // ??
        cmap[x][y].time = mgametime;
    }
}

void creaturemove(GLint x, GLint y)
{
    pair<GLint, GLint> creaturego[4] = {{0, -1}, {1, 0}, {-1, 0}, {0, 1}};
    vector<pair<GLint, GLint> > a;
    creature c;
    GLint b = 0;
    if (y == 0)
    {
        playerlive--;
        cmap[x][y] = c;
    }
    else
    {
        for (long i = 0; i < 4; i++)
        {
            if ((x + creaturego[i].first) >= 0 && (y + creaturego[i].second) >= 0 && (x + creaturego[i].first) < msx && (y + creaturego[i].second) < msy)
            {
                if ((gmap[x + creaturego[i].first][y + creaturego[i].second].obj < 0 || mapobj[gmap[x + creaturego[i].first][y + creaturego[i].second].obj].p[gmap[x + creaturego[i].first][y + creaturego[i].second].n].t.f == false) && cmap[x + creaturego[i].first][y + creaturego[i].second].f == false && ((x + creaturego[i].first) != cmap[x][y].ppos.first || y + creaturego[i].second != cmap[x][y].ppos.second))
                {
                    if (creaturego[i].first == 0 && creaturego[i].second == -1)
                    {
                        a.push_back(creaturego[i]);
                    }
                    if (creaturego[i].first != 0 || creaturego[i].second != 1 || a.size() == 0)
                    {
                        a.push_back(creaturego[i]);
                    }
                }
            }
        }
        if (a.size() > 0)
        {
            b = rand() % a.size();
            cmap[x + a[b].first][y + a[b].second] = cmap[x][y];
            cmap[x + a[b].first][y + a[b].second].movef = false;
            cmap[x][y] = c;
            cmap[x][y].f = false;
            if (gmap[x + a[b].first][y + a[b].second].obj == -1)
            {
                gmap[x + a[b].first][y + a[b].second].obj = -2;
                gmap[x + a[b].first][y + a[b].second].n = -1;
            }
            cmap[x + a[b].first][y + a[b].second].ppos = {x, y};
            // if obj = -1 add obj elem
        }
        else
        {
            cmap[x][y].movef = false;
            if (gmap[x][y].obj == -1)
            {
                gmap[x][y].obj = -2;
                gmap[x][y].n = -1;
            }
        }
    }
}

void drawcreature(GLdouble a, GLdouble b, GLdouble c, GLdouble d, GLint x, GLint y)
{
    if (gmap[x][y].obj == -2)
    {
        glColor3f(1.0f, 1.0f, 1.0f);
        square2d(a, b, c, d);
    }
    cmap[x][y].c.setc();
    square2d(a + (c - a) * 1.0f / 4.0f, b + (d - b) * 1.0f / 4.0f, a + (c - a) * 3.0f / 4.0f, b + (d - b) * 3.0f / 4.0f);
    drawlive(a, b, c, d, cmap[x][y].live, cmap[x][y].startlive);
}

void createwave()
{
    if (wavecreatem.size() > 0 && createwavetime + (wavecreatem.back()).timer <= mgametime)
    {
        GLint a = 0;
        vector<GLint> b;
        creature c;
        for (long i = 0; i < (wavecreatem.back()).c.size(); i++)
        {
            for (long j = 0; j < (wavecreatem.back()).c[i].second; j++)
            {
                b.push_back((wavecreatem.back()).c[i].first);
            }
        }
        for (long i = 0; i < (wavecreatem.back()).n; i++)
        {
            a = rand() % b.size();
            if (b[a] == -1)
            {
                wavem[(wavecreatem.back()).pos].push_front(c);
            }
            else
            {
                wavem[(wavecreatem.back()).pos].push_front(creaturetype[b[a]]);
            }
        }
        wavecreatem.pop_back();
        createwavetime = mgametime;
    }
}

void wavegen()
{
    if (wavetime + wavetimer <= mgametime)
    {
        for (long i = 0; i < msx && i < wavem.size(); i++)
        {
            if (cmap[i][msy - 1].f == false && wavem[i].size() > 0)
            {
                cmap[i][msy - 1] = wavem[i].back();
                wavem[i].pop_back();
            }
        }
        wavetime = mgametime;
    }
}

void creaturestep()
{
    createwave();
    for (long i = 0; i < msx; i++)
    {
        for (long j = 0; j < msy; j++)
        {
            deletecreature(i, j);
        }
    }
    if (creaturesteptime + creaturesteptimer <= mgametime)
    {
        for (long i = 0; i < msx; i++)
        {
            for (long j = 0; j < msy; j++)
            {
                if (cmap[i][j].f)
                {
                    cmap[i][j].movef = true;
                }
            }
        }
        for (long i = 0; i < msx; i++)
        {
            for (long j = 0; j < msy; j++)
            {
                if (cmap[i][j].f && cmap[i][j].movef)
                {
                    creaturemove(i, j);
                }
            }
        }
        for (long i = 0; i < msx; i++)
        {
            for (long j = 0; j < msy; j++)
            {
                if (cmap[i][j].f)
                {
                    creatureattack(i, j);
                }
            }
        }
        creaturesteptime = mgametime;
    }
    wavegen();
}


void loadobj(string s)
{
    GLint n = 0;
    obj a;
    elem b;
    ifstream in;
    in.open(s);
    gameobj.clear();
    for (;in >> a.x;)
    {
        in >> a.y;
        in >> n;
        a.p.clear();
        for (long i = 0; i < n; i++)
        {
            in >> b.x >> b.y;
            a.p.push_back(b);
        }
        gameobj.push_back(a);
    }
    in.close();
}

void loadcolors(string s)
{
    objc a;
    ifstream in;
    in.open(s);
    gamec.clear();
    for (; in >> a.c.r;)
    {
        in >> a.c.g >> a.c.b;
        gamec.push_back(a);
    }
    in.close();
}

void loadwaves(string s)
{
    GLint n = 0;
    wave a;
    pair<GLint, GLint> b;
    ifstream in;
    in.open(s);
    for (;in >> a.n;)
    {
        in >> a.pos;
        in >> a.timer;
        in >> n;
        a.c.clear();
        for (long i = 0; i < n; i++)
        {
            in >> b.first >> b.second;
            a.c.push_back(b);
        }
        wavecreatem.push_front(a);
    }
    in.close();
}

void loadtowers(string s)
{
    towerobj a;
    ifstream in;
    in.open(s);
    for (long i = 0; in >> a.t.attack && i < acttowernum; i++)
    {
        in >> a.t.c.r >> a.t.c.g >> a.t.c.b;
        a.t.f = true;
        in >> a.t.live;
        a.t.startlive = a.t.live;
        in >> a.t.r;
        in >> a.t.shield;
        a.t.time = 0;
        in >> a.t.timer;
        in >> a.t.cost;
        in >> a.t.utpcost >> a.t.utrcost >> a.t.utlcost;
        acttowers[i] = a;
    }
    in.close();
}

void loadcreatures(string s)
{
    creature a;
    ifstream in;
    in.open(s);
    creaturetype.clear();
    for (; in >> a.attack;)
    {
        in >> a.c.r >> a.c.g >> a.c.b;
        a.f = true;
        in >> a.live;
        a.startlive = a.live;
        a.movef = false;
        a.ppos = {-1, -1};
        in >> a.range;
        in >> a.shield;
        in >> a.speed;
        a.time = 0;
        in >> a.timer;
        in >> a.cost;
        creaturetype.push_back(a);
    }
    in.close();
}

void init()
{
    loadobj(waytoapp + "obj.gameres");
    loadcolors(waytoapp + "color.gameres");
    loadwaves(waytoapp + "wave.gameres");
    loadtowers(waytoapp + "tower.gameres");
    loadcreatures(waytoapp + "creature.gameres");
    /*obj a;
    elem b;
    objc c;
    a.x = 3;
    a.y = 1;
    b.x = 0;
    b.y = 0;
    a.p.push_back(b);
    b.x = 1;
    b.y = 0;
    a.p.push_back(b);
    b.x = 2;
    b.y = 0;
    a.p.push_back(b);
    cmap[msx - 1][msy - 1].f = true;
    cmap[msx - 1][msy - 1].live = 50;
    acttowers[0].t.attack = 2;
    acttowers[0].t.live = 10;
    acttowers[0].t.r = 5;
    b.x = 2;
    b.y = 1;
    a.p.push_back(b);
    b.x = 3;
    b.y = 1;
    a.p.push_back(b);
    srand((unsigned)(rand() * time(NULL)));
    gameobj.push_back(a);
    gamec.push_back(c);*/
    newobj();
}

void initMonstersRow()
{
    for (long i = 0; i < msx; i++)
    {
        gmap[i][msy - 1].obj = -3;
        gmap[i][msy - 1].n = -1;
    }
}

void drawtmenu(GLdouble a, GLdouble b, GLdouble c, GLdouble d, GLdouble posx, GLdouble posy) // draw tower menu
{
    GLdouble e = max(a - c, c - a);
    GLdouble f = max(b - d, d - b);
    GLdouble g = min(a, c);
    GLdouble h = min(b, d);
    GLint *k = NULL;
    button addlives(g + e * 1.0d / 11.0d, h + f * 1.0d / 11.0d, g + e * 5.0d / 11.0d, h + f * 5.0d / 11.0d); // update parameters
    button updater(g + e * 1.0d / 11.0d, h + f  * 6.0d / 11.0d, g + e * 5.0d / 11.0d, h + f * 10.0d / 11.0d); // update range
    button sell(g + e * 6.0d / 11.0d, h + f * 1.0d / 11.0d, g + e * 10.0d / 11.0d, h + f * 5.0d / 11.0d); // add lives to tower
    button updatep(g + e * 6.0d / 11.0d, h + f * 6.0d / 11.0d, g + e * 10.0d / 11.0d, h + f * 10.0d / 11.0d); // sell tower
    if (posx >= a && posy >= b && posx < c && posy < d)
    {
        if (updatep.bclick(posx, posy))
        {
            k = &mapobj[acttobj].p[acttn].t.utpcost;
            if (gameres >= (*k))
            {
                mapobj[acttobj].p[acttn].t.attack *= (1.0d + tupdatenum);
                gameres -= (*k);
                mapobj[acttobj].p[acttn].t.cost += (*k);
                (*k) *= tupdateconst;
            }
        }
        if (updater.bclick(posx, posy))
        {
            k = &mapobj[acttobj].p[acttn].t.utrcost;
            if (gameres >= (*k))
            {
                mapobj[acttobj].p[acttn].t.r += 1.0d;//min(mapobj[acttobj].p[acttn].t.r * tupdatenum, 1.0d);
                gameres -= (*k);
                mapobj[acttobj].p[acttn].t.cost += (*k);
                (*k) *= tupdateconst;
            }
        }
        if (addlives.bclick(posx, posy))
        {
            k = &mapobj[acttobj].p[acttn].t.utlcost;
            if (gameres >= (*k))
            {
                mapobj[acttobj].p[acttn].t.live += tupdatenum * mapobj[acttobj].p[acttn].t.startlive;
                gameres -= (*k);
                mapobj[acttobj].p[acttn].t.cost += (*k);
                (*k) *= tupdateconst;
            }
        }
        if (sell.bclick(posx, posy))
        {
            gameres += mapobj[acttobj].p[acttn].t.cost * tsellconst;
            tower t;
            mapobj[acttobj].p[acttn].t = t;
            mapobj[acttobj].p[acttn].t.f = false;
        }
        mb[0] = false;
    }
    glColor3f(0.35f, 0.35f, 0.35f);
    square2d(a, b, c, d);
    updatep.drawb();
    updater.drawb();
    addlives.drawb();
    sell.drawb();
}

void drawstep()
{
    GLdouble mwp[3] = {(wpu[0] - wpd[0]) * menudeep, (wpu[1] - wpd[1]) * menudeep, 0.0f}; // max window pos
    GLdouble mapws[3] = {mwp[0] / (msx * 1.0f), (mwp[1] * 14.0f) / (msy * 20.0f), 0.0f}; // map window size
    GLdouble mmpos[3] = {(mpos[0] - wpd[0]) * menudeep, (mpos[1] - wpd[1]) * menudeep, 0.0f}; // menu mouse pos
    GLdouble actobjhs[2] = {mwp[0] * 5.0f / (12.0f * actobj.x), (mwp[1] * 5.0f) / (32.0f * actobj.y)}; // act obj help size
    GLdouble actobjbs = min(actobjhs[0], actobjhs[1]); // act obj block size
    GLdouble actobjms[2] = {max((GLdouble)0.0f, (actobjhs[0] - actobjbs) * actobj.x) / 2.0f + mwp[0] * 2.0f / 12.0f,
     max((GLdouble)0.0f, (actobjhs[1] - actobjbs) * actobj.y) / 2.0f + mwp[1] * 1.5f / 32.0f}; // act obj map size (without blocks)
	glTranslatef(wpd[0] * menudeep, wpd[1] * menudeep, -menudeep);
	glColor3f(0.9f, 0.9f, 0.9f);
	square2d(0.0f, 0.0f, mwp[0], mwp[1]);
	glColor3f(0.4f, 0.4f, 0.4f);
    square2d(0.0f, 0.0f, mwp[0], mwp[1] * 1.0f / 4.0f);
    square2d(0.0f, mwp[1] * 19.0f / 20.0f, mwp[0], mwp[1]);
    glColor3f(0.6f, 0.6f, 0.6f);
    square2d(mwp[0] * 1.5f / 12.0f, mwp[1] * 1.0f / 32.0f, mwp[0] * 7.5f / 12.0f, mwp[1] * 7.0f / 32.0f);
    button m1(mwp[0] * 8.0f / 12.0f, mwp[1] * 4.0f / 32.0f, mwp[0] * 11.5f / 12.0f, mwp[1] * 7.0f / 32.f);
    button m2(mwp[0] * 8.0f / 12.0f, mwp[1] * 1.25f / 32.0f, mwp[0] * 9.5f / 12.0f, mwp[1] * 3.25f / 32.0f);
    button m3(mwp[0] * 10.0f / 12.0f, mwp[1] * 1.25f / 32.0f, mwp[0] * 11.5f / 12.0f, mwp[1] * 3.25f / 32.0f);
    vector<button> m4;
    for (GLint i = 0; i < 4; i++)
    {
        button addm4(mwp[0] * 0.25f / 12.0f, mwp[1] * (0.875f + 1.75f * i) / 32.0f, mwp[0] * 1.25f / 12.0f, mwp[1] * (1.875f + 1.75f * i) / 32.0f);
        m4.push_back(addm4);
    }
    for (long i = 0; i < actobj.p.size(); i++)
    {
        pair<GLdouble, GLdouble> helpobjpos = {actobj.p[i].x * actobjbs + actobjms[0], actobj.p[i].y * actobjbs + actobjms[1]};
        drawplace(helpobjpos.first, helpobjpos.second, helpobjpos.first + actobjbs, helpobjpos.second + actobjbs, actobj.p[i]);
        glColor3f(0.6f, 0.6f, 0.6f);
        squareline(helpobjpos.first, helpobjpos.second, helpobjpos.first + actobjbs, helpobjpos.second + actobjbs, 3.0f);
        if (mb[0] && addtowerf >= 0 && mmpos[0] >= helpobjpos.first && mmpos[1] >= helpobjpos.second && mmpos[0] < helpobjpos.first + actobjbs && mmpos[1] < helpobjpos.second + actobjbs)
        {
            if (actobj.p[i].t.f == false && acttowers[addtowerf].t.cost <= gameres)
            {
                actobj.p[i].t = acttowers[addtowerf].t;
                actobj.p[i].t.f = true;
                gameres -= acttowers[addtowerf].t.cost;
                //addtowerf = -1;
                mb[0] = false;
            }
            else
            {
                if (actobj.p[i].t.f)
                {
                    tower a;
                    a.f = false;
                    actobj.p[i].t = a;
                    gameres += actobj.p[i].t.cost;
                    mb[0] = false;
                }
            }
        }
    }
    for (long j = 0; j < msy; j++)
    {
        for (long i = 0; i < msx; i++)
        {
            if (gmap[i][j].obj >= 0)
            {
                drawplace(i * mapws[0], j * mapws[1] + mwp[1] / 4.0f, (i + 1) * mapws[0], (j + 1) * mapws[1] + mwp[1] / 4.0f, mapobj[gmap[i][j].obj].p[gmap[i][j].n]);
            }
            if (gmap[i][j].obj == -3)
            {
                glColor3f(0.7f, 0.7f, 0.7f);
                square2d(i * mapws[0], j * mapws[1] + mwp[1] / 4.0f, (i + 1) * mapws[0], (j + 1) * mapws[1] + mwp[1] / 4.0f);
            }
            if (cmap[i][j].f)
            {
                drawcreature(i * mapws[0], j * mapws[1] + mwp[1] / 4.0f, (i + 1) * mapws[0], (j + 1) * mapws[1] + mwp[1] / 4.0f, i, j);
            }
            glColor3f(0.9f, 0.9f, 0.9f);
            squareline(i * mapws[0], j * mapws[1] + mwp[1] / 4.0f, (i + 1) * mapws[0], (j + 1) * mapws[1] + mwp[1] / 4.0f, 2.0f);
        }
    }
    if (m1.bclick(mmpos[0], mmpos[1]))
    {
        addnewobj = !addnewobj;
    }
    if (addnewobj)
    {
        m1.clickb = true;
    }
    m1.drawb();
    if (m2.bclick(mmpos[0], mmpos[1]))
    {
        swap(actobj.x, actobj.y);
        for (long i = 0; i < actobj.p.size(); i++)
        {
            swap(actobj.p[i].x, actobj.p[i].y);
            actobj.p[i].y = actobj.y - actobj.p[i].y - 1;
        }
    }
    m2.drawb();
    m3.bclick(mmpos[0], mmpos[1]);
    m3.drawb();
    for (long i = 0; i < m4.size(); i++)
    {
        if (m4[i].bclick(mmpos[0], mmpos[1]))
        {
            addtowerf = i;
        }
        if (addtowerf == i)
        {
            m4[i].clickb = true;
        }
        m4[i].drawb();
    }
    if (acttobj >= 0 && acttn >= 0 && mapobj.count(acttobj) != 0 && mapobj[acttobj].p.size() > acttn && mapobj[acttobj].p[acttn].t.f)
    {
        melem *a = &mapobj[acttobj].p[acttn];
        pair<GLdouble, GLdouble> tmdpos; // tower menu down pos
        pair<GLdouble, GLdouble> tmupos; // tower menu up pos
        glColor3f(0.0f, 1.0f, 0.0f);
        squareline(mapws[0] * (max(a->x - (GLint)a->t.r, 0)), mapws[1] * (max(a->y - (GLint)a->t.r, 0)) + mwp[1] / 4.0f, mapws[0] * (min(a->x + 1 + (GLint)a->t.r, msx)), mapws[1] * (min(a->y + 1 + (GLint)a->t.r, msy)) + mwp[1] / 4.0f, 3.0f);
        // act tower menu
        tmdpos.first = (a->x + 0.5d) * mapws[0];
        tmdpos.second = (a->y + 0.5d) * mapws[1] + mwp[1] / 4.0d;
        if (a->x * 2 >= msx)
        {
            tmupos.first = tmdpos.first;
            tmdpos.first = tmupos.first - msx * mapws[0] / 3.0d;
        }
        else
        {
            tmupos.first = tmdpos.first + msx * mapws[0] / 3.0d;
        }
        if (a->y * 2 >= msy)
        {
            tmupos.second = tmdpos.second;
            tmdpos.second = tmupos.second - msy * mapws[1] / 6.0d;
        }
        else
        {
            tmupos.second = tmdpos.second + msy * mapws[1] / 6.0d;
        }
        drawtmenu(tmdpos.first, tmdpos.second, tmupos.first, tmupos.second, mmpos[0], mmpos[1]);
    }
    else
    {
        acttobj = -1;
        acttn = -1;
    }
    if (mb[0] && mmpos[0] / mapws[0] >= 0 && mmpos[0] / mapws[0] < msx && (mmpos[1] - mwp[1] / 4.0f) / mapws[1] >= 0 && (mmpos[1] - mwp[1] / 4.0f) / mapws[1] < msy)
    {
        GLint a = mmpos[0] / mapws[0];
        GLint b = (mmpos[1] - mwp[1] / 4.0f) / mapws[1];
        if (gmap[a][b].obj == -1 && addnewobj)
        {
            if (addobj(a))
            {
                newobj();
                addnewobj = false; // ??
            }
            mb[0] = false;
        }
        else
        {
            if (gmap[a][b].obj >= 0 && mapobj[gmap[a][b].obj].p[gmap[a][b].n].t.f)
            {
                if (acttobj == gmap[a][b].obj && acttn == gmap[a][b].n)
                {
                    acttobj = -1;
                    acttn = -1;
                }
                else
                {
                    acttobj = gmap[a][b].obj;
                    acttn = gmap[a][b].n;
                }
                mb[0] = false;
            }
            else
            {
                acttobj = -1;
                acttn = -1;
            }
        }
    }
    if (attackdrawtime + attackdrawtimer <= mgametime)
    {
        attackdrawm.clear();
        attackdrawtime = mgametime;
    }
    else
    {
        /*for (long i = 0; i < attackdrawm.size(); i++)
        {
            if (mapobj.count(attackdrawm[i].t.first) == 0 || mapobj[attackdrawm[i].t.first].p.size() <= attackdrawm[i].t.second || mapobj[attackdrawm[i].t.first].p[attackdrawm[i].t.second].t.f == false || cmap[attackdrawm[i].c.first][attackdrawm[i].c.second].f == false)
            {
                if (i < attackdrawm.size() - 1)
                {
                    swap(attackdrawm[i], attackdrawm.back());
                }
                attackdrawm.pop_back();
                i--;
            }
        }*/
        for (long i = 0; i < attackdrawm.size(); i++)
        {
            GLint a = mapobj[attackdrawm[i].t.first].p[attackdrawm[i].t.second].x;
            GLint b = mapobj[attackdrawm[i].t.first].p[attackdrawm[i].t.second].y;
            if (attackdrawm[i].b)
            {
                glColor3f(1.0f, 0.0f, 0.0f);
            }
            else
            {
                glColor3f(0.0f, 0.0f, 1.0f);
            } // change colors to colors of towers and creatures ??
            line(mapws[0] * (a + 0.5f), mapws[1] * (b + 0.5f) + mwp[1] / 4.0f, mapws[0] * (attackdrawm[i].c.first + 0.5f), mapws[1] * (attackdrawm[i].c.second + 0.5f) + mwp[1] / 4.0f, 3.0f);
        }
    }
    glTranslatef(-wpd[0] * menudeep, -wpd[1] * menudeep, menudeep);
}

void display(void)
{
	glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity ();
	mgametime = glutGet(GLUT_ELAPSED_TIME);
	settime();
	initmpos();
	initwpos();
	drawstep();
	mobjstep();
	creaturestep();
	breaktime();
	glutSwapBuffers();
	glFlush();
}

void keyboard(unsigned char button, int kx, int ky)
{
    switch (button)
    {
        case 27:
            exit(0);
            break;
            break;
        default:
            break;
    }

}

void keyboards(int button, int kxs, int kys) //special keyboard
{
	switch (button)
	{
		case GLUT_KEY_F1:
			exit(0);
			break;
		default:
			break;
	}
}

void mouse(int button, int click, int mx, int my)
{
	switch (button)
	{
		case GLUT_LEFT_BUTTON:
			if (click == GLUT_DOWN)
			{
				mb[0] = true;
			}
			else
			{
				mb[0] = false;
			}
			break;
		case GLUT_MIDDLE_BUTTON:
			if (click == GLUT_DOWN)
			{
				mb[1] = true;
			}
			else
			{
				mb[1] = false;
			}
			break;
		case GLUT_RIGHT_BUTTON:
			if (click == GLUT_DOWN)
			{
				mb[2] = true;
			}
			else
			{
				mb[2] = false;
			}
			break;
		default:
			break;
	}
	mp[0] = mx;
	mp[1] = my;
}

void pmotion(GLint mx, GLint my)
{
	mp[0] = mx;
	mp[1] = my;
}

void amotion(GLint mx, GLint my)
{
	mp[0] = mx;
	mp[1] = my;
}

void window(GLint w, GLint h)
{
	if (h == 0)
	{
		h = 1;
	}
	wp[0] = w;
	wp[1] = h;
	GLfloat view = w * 1.0f / h;
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glViewport(0, 0, w, h);
	gluPerspective(45, view, 0.01f, 100.0f);
	glMatrixMode(GL_MODELVIEW);
	//glEnable (GL_DEPTH_TEST);
}

void getwaytoapp(string s)
{
    long i = 0;
    for (i = s.size() - 1; i >= 0; i--)
    {
        if (s[i] == '/' || s[i] == '\\')
        {
            break;
        }
    }
    i++;
    if (i > 0)
    {
        waytoapp = s.substr(0, i);
    }
}

int main(int argc, char **argv)
{
    getwaytoapp(argv[0]);
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(wp[0], wp[1]);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("SmallGame");
	initGL();
	initMonstersRow();
	init();
	glutKeyboardFunc (keyboard);
	glutSpecialFunc (keyboards);
	glutMouseFunc (mouse);
	glutMotionFunc (amotion);
    glutPassiveMotionFunc (pmotion);
	glutReshapeFunc (window);
	glutIdleFunc (display);
	glutDisplayFunc (display);
	glutMainLoop ();
}
