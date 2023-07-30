//
//  Torso.h
//  glutCpp
//
//  Created by Ruben Cuadra on 2/24/17.
//  Copyright © 2017 Ruben Cuadra. All rights reserved.
//


#ifdef __APPLE__
    #include <GLUT/glut.h>
#elif _WIN32 || __linux__
    #include <GL/glut.h>
#endif
#pragma once

class Torso
{
private:
    float minTorsionChest,maxTorsionChest;
    float minTorsionStomach,maxTorsionStomach;
    float torsionC, torsionS;
    GLfloat *matColor;
public:
    Torso(float minChest,float maxChest,float minStomach, float maxStomach)
    : minTorsionChest(minChest),maxTorsionChest(maxChest),minTorsionStomach(minStomach),maxTorsionStomach(maxStomach),torsionC(0),torsionS(0)
    {
        matColor = new GLfloat[4];
        matColor[0] = 0.3;
        matColor[1] = 0.3;
        matColor[2] = 0.3;
        matColor[3] = 1.0;
    }
    
    bool setChestTorsion(float torsion)
    {
        torsion = ((int)torsion)%365;
        if (torsion<maxTorsionChest && torsion>minTorsionChest)
        {
            this->torsionC = torsion;
            return true;
        }
        return false;
    }
    bool setStomachTorsion(float torsion)
    {
        torsion = ((int)torsion)%365;
        if (torsion<maxTorsionStomach && torsion>minTorsionStomach)
        {
            this->torsionS = torsion;
            return true;
        }
        return false;
    }
    
    float getChestTorsion(){return torsionC;}
    float getStomachTorsion(){return torsionS;}
    void setMaterialColor(float r,float g,float b)
    {
        matColor[0] = r;
        matColor[1]=g;
        matColor[2]=b;
    }
    float getRed(){return matColor[0];}
    float getBlue(){return matColor[1];}
    float getGreen(){return matColor[2];}
    void render(void)
    {
        glMaterialfv(GL_FRONT, GL_AMBIENT, matColor);
        glPushMatrix(); //Stomach
            glRotatef(torsionS,0,1,0);
            glTranslatef(0,-0.1,0);
            glScalef(0.75,0.6,0.5);
            glutSolidCube(1);
        glPopMatrix();
        
        glPushMatrix(); //Chest
            glTranslatef(0,0.35,0);
            glRotatef(torsionC,0,1,0);
            glScalef(1.2,0.65,0.7); //Escala para un Rectangulo
            glScalef(0.4,0.3,0.4);   //Dode es mas grande
            glutSolidDodecahedron();
        glPopMatrix();
    }
};
