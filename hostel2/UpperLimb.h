//
//  UpperLimb.h
//  glutCpp
//
//  Created by Ruben Cuadra on 2/26/17.
//  Copyright © 2017 Ruben Cuadra. All rights reserved.
//

#pragma once

class Arm : public Part
{
public:
    Arm(float minRotation, float maxRotation,float minRevolution,float maxRevolution):Part(minRotation,maxRotation,0,0,minRevolution,maxRevolution){}
    
    void render()
    {
        glMaterialfv(GL_FRONT, GL_AMBIENT, matColor);
        glTranslatef(0,0.15,0);     //Movemos para generar el hombro
        glRotatef(rotation,1,0,0);
        glRotatef(revolution,0,0,1);
        glTranslatef(0,-0.15,0);    //Regresamos para generar el brazo
        
        glPushMatrix();
            glScalef(0.18,0.3,0.22);     //Escalamos
            glutSolidSphere(1,20,20);   //Dibujamos
        glPopMatrix();
        glTranslatef(0,-0.2,0);     //Aqui estamos en el codo
    }
};

class Forearm : public Part
{
public:
    Forearm(float minRotation,float maxRotation,float minTorsion,float maxTorsion):Part(minRotation,maxRotation,minTorsion,maxTorsion,0,0){}
    
    void render()
    {
        glMaterialfv(GL_FRONT, GL_AMBIENT, matColor);
        glRotatef(rotation,1,0,0);
        glRotatef(torsion,0,1,0);
        
        glTranslatef(0,-0.3,0); //Movernos al centro del antebrazo
        
        glPushMatrix();
            glScalef(0.12, 0.3,0.1);
            glutSolidSphere(1,20,20);
        glPopMatrix();
        
        glTranslatef(0,-0.3,0); //Aqui estamos en la muñeca
    }
};

class Hand : public Part
{
public:
    Hand(float minRotation,float maxRotation,float minTorsion,float maxTorsion):Part(minRotation,maxRotation,minTorsion,maxTorsion,0,0){}
    
    void render()
    {
        glMaterialfv(GL_FRONT, GL_AMBIENT, matColor);
        glRotatef(rotation,1,0,0);
        glRotatef(torsion,0,1,0);
        
        glTranslatef(0,-0.1,0); //Movernos al centro de la mano
        
        glPushMatrix();
            glScalef(0.1,0.1,0.1);
            glutSolidOctahedron();
        glPopMatrix();
    }
};

class UpperLimb
{
private:
    Arm* arm;
    Hand *hand;
    Forearm *forearm;

public:
    bool rotateArm(float toAngle) {return arm->setRotation(toAngle);}
    bool revArm(float toAngle)    {return arm->setRevolution(toAngle);}
    
    bool rotForearm(float toAngle){return forearm->setRotation(toAngle); }
    bool torForearm(float toAngle){return forearm->setTorsion(toAngle); }
    
    bool rotHand(float toAngle){return hand->setRotation(toAngle); }
    bool torHand(float toAngle){return hand->setTorsion(toAngle); }
    
    void setHandColor(float r,float g,float b){ hand->setMaterialColor(r,g,b); }
    void setForearmColor(float r,float g,float b) { forearm->setMaterialColor(r,g,b);}
    void setarmColor(float r,float g,float b) { arm->setMaterialColor(r,g,b);}
    
    UpperLimb(float minRotArm,float maxRotArm,float minRevArm,float maxRevArm,
              float minForeRotation,float maxForeRotation, float minForeTor,float maxForeTor,
              float minHandRotation, float maxHandRotation, float minHandTor, float maxHandTor)
    {
        arm =     new Arm(minRotArm,maxRotArm,minRevArm,maxRevArm);
        forearm = new Forearm(minForeRotation, maxForeRotation, minForeTor, maxForeTor);
        hand = new Hand( minHandRotation,maxHandRotation,minHandTor,maxHandTor );
    }
    
    void render()
    {
        glPushMatrix();
            arm->render();
            forearm->render();
            hand->render();
        glPopMatrix();
    }
};
