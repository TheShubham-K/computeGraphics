//
//  LowerLimb.h
//  glutCpp
//
//  Created by Ruben Cuadra on 2/26/17.
//  Copyright © 2017 Ruben Cuadra. All rights reserved.
//

#pragma once

class Thigh : public Part //Entre cadera y rodilla
{
public:
    Thigh(float minRotation, float maxRotation,float minRevolution,float maxRevolution):Part(minRotation,maxRotation,0,0,minRevolution,maxRevolution){}
    void render()
    {
        
        glMaterialfv(GL_FRONT, GL_AMBIENT, matColor);
        glTranslatef(0,0.45,0);     //Movemos para generar la articulacion
            glRotatef(rotation,1,0,0);
            glRotatef(revolution,0,0,1);
        glTranslatef(0,-0.45,0);    //Regresamos para generar el muslo
        
        glPushMatrix();             //Pintamos el musculo
            glScalef(0.12,0.3,0.1);
            glutSolidDodecahedron();
        glPopMatrix();
        
        glTranslatef(0,-0.5,0);
    }
};

class Calf : public Part    //Entre rodilla y tobillo, solo tiene el movimiento de la rodilla
{
public:
    Calf(float minRotation,float maxRotation):Part(minRotation,maxRotation,0,0,0,0){}
    void render()
    {
        glMaterialfv(GL_FRONT, GL_AMBIENT, matColor);
        glRotatef(rotation,1,0,0);
        glTranslatef(0, -0.2, 0);
        glPushMatrix();             //Pintamos pantorrilla
            glScalef(0.12,0.4,0.2);
            glutSolidIcosahedron();
        glPopMatrix();
        
        glTranslatef(0,-0.35,0); //Nos vamos al pie
    }

};
class Foot : public Part //Pie
{
public:
    Foot(float minRotation,float maxRotation,float minTorsion,float maxTorsion):Part(minRotation,maxRotation,minTorsion,maxTorsion,0,0){}
    
    void render()
    {
        glMaterialfv(GL_FRONT, GL_AMBIENT, matColor);
        glRotatef(180,1,0,0); //Para que salga viendo hacia nosotros
        glRotatef(rotation,1,0,0);
        glRotatef(torsion,0,1,0);
        glutSolidCone(0.07,0.35,20, 20);
    }
    
};

class LowerLimb
{
private:
    Thigh * thigh;
    Calf * calf;
    Foot * foot;
    
public:
    LowerLimb(float minRotThigh,float maxRotThigh,float minRevThigh,float maxRevThigh,
              float minCalfRotation,float maxCalfRotation,
              float minFootRotation, float maxFootRotation, float minFootTor, float maxFootTor)
    {
        thigh = new Thigh(minRotThigh,maxRotThigh,minRevThigh,maxRevThigh);
        calf = new Calf(minCalfRotation,maxCalfRotation);
        foot = new Foot(minFootRotation,maxFootRotation,minFootTor,maxFootTor);
    }
    
    bool rotateThigh(float toAngle) {return thigh->setRotation(toAngle);}
    bool revThigh(float toAngle) { return thigh->setRevolution(toAngle); }
    
    bool rotateCalf(float toAngle) {return calf->setRotation(toAngle);}
    
    bool rotateFoot(float toAngle) {return foot->setRotation(toAngle);}
    bool torsionFoot(float toAngle) {return foot->setTorsion(toAngle);}
    
    void setThighColor(float r,float g,float b){ thigh->setMaterialColor(r,g,b); }
    void setCalfColor(float r,float g,float b) { calf->setMaterialColor(r,g,b);}
    void setFootColor(float r,float g,float b) { foot->setMaterialColor(r,g,b);}
    
    void render()
    {
        glPushMatrix();
            thigh->render();
            calf->render();
            foot->render();
        glPopMatrix();
    }
};
