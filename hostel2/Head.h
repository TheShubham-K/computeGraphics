#ifdef __APPLE__
    #include <GLUT/glut.h>
#elif _WIN32 || __linux__
    #include <GL/glut.h>
#endif
#include "Part.h"
#pragma once

class Head : public Part
{
public:
    Head(float minR, float maxR,float minT,float maxT):Part(minR,maxR,minT,maxT,0,0){}

    void render(void)
    {
        glMaterialfv(GL_FRONT, GL_AMBIENT, matColor);
        glPushMatrix();
            glRotatef(torsion,0,1,0);
            glRotatef(rotation,1,0,0);
            glutSolidSphere(0.3,20,20);
            glRotatef(180, 1,0,0);         //Cono sale al reves
            glutSolidCone(0.2,0.4, 20,20); //Nose
        glPopMatrix();
    }
};
