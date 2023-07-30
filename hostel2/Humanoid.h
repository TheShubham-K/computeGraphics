//
//  Humanoid.h
#pragma once
#include "Torso.h"
#include "Head.h"
#include "UpperLimb.h"
#include "LowerLimb.h"

class Humanoid //Padre para personajes humanoides
{
protected:
    Torso *torso;
    Head *head;
    
    UpperLimb *right_upper;
    UpperLimb *left_upper;
    
    LowerLimb *right_lower;
    LowerLimb *left_lower;
    
public:
    virtual void render() = 0; //Puede cambiar acomodo de objetos
    
    //Torso
    bool rotateTorso_Stomach(float toAngle){ return torso->setStomachTorsion(toAngle); }
    bool rotateTorso_Chest  (float toAngle){ return torso->setChestTorsion  (toAngle); }
    //Head
    bool verticalRotateHead(float toAngle) {return head->setRotation(toAngle);}
    bool rotateHead(float toAngle) {return head->setTorsion(toAngle);}
    //Arms
        //Right
    bool rotateRightArm(float toAngle) {return right_upper->rotateArm(toAngle); }
    bool revolRightArm(float toAngle)  {return right_upper->revArm(toAngle); }
    bool rotRightForearm(float toAngle){return right_upper->rotForearm(toAngle); }
    bool torRightForearm(float toAngle){return right_upper->torForearm(toAngle); }
    bool rotRightHand(float toAngle){return right_upper->rotHand(toAngle); }
    bool torRightHand(float toAngle){return right_upper->torHand(toAngle); }
        //Left
    bool rotateLeftArm(float toAngle) {return left_upper->rotateArm(toAngle); }
    bool revolLeftArm(float toAngle)  {return left_upper->revArm(toAngle); }
    bool rotLeftForearm(float toAngle){return left_upper->rotForearm(toAngle); }
    bool torLeftForearm(float toAngle){return left_upper->torForearm(toAngle); }
    bool rotLeftHand(float toAngle){return left_upper->rotHand(toAngle); }
    bool torLeftHand(float toAngle){return left_upper->torHand(toAngle); }
    //Legs
        //Right
    bool rotateRightThigh(float toAngle) {return right_lower->rotateThigh(toAngle); }
    bool revolRightThigh(float toAngle)  {return right_lower->revThigh(toAngle); }
    
    bool rotRightCalf(float toAngle){return right_lower->rotateCalf(toAngle); }
    
    bool rotateRightFoot(float toAngle) {return right_lower->rotateFoot(toAngle); }
    bool torRightFoot(float toAngle) {return right_lower->torsionFoot(toAngle); }
        //Left
    bool rotateLeftThigh(float toAngle) {return left_lower->rotateThigh(toAngle); }
    bool revolLeftThigh(float toAngle)  {return left_lower->revThigh(toAngle); }
    
    bool rotLeftCalf(float toAngle){return left_lower->rotateCalf(toAngle); }
    
    bool rotateLeftFoot(float toAngle) {return left_lower->rotateFoot(toAngle); }
    bool torLeftFoot(float toAngle) {return left_lower->torsionFoot(toAngle); }
};

class Human : public Humanoid
{
public:
    Human()
    {
        torso = new Torso(-30,30,-20,20);
        head = new Head(-60,80,-90,90);
        
        right_upper = new UpperLimb(-90,180,-100,10,0,90,-20,20,-10,40,-20,20);
        left_upper = new UpperLimb(-90,180,-10,100,0,90,-20,20,-10,40,-20,20);
        
        right_lower = new LowerLimb(-45,90,-60,10, -90,0, -20,40, -45,45);
        left_lower = new LowerLimb(-45,90,-10,60, -90,0, -20,40, -45,45);
        //Colores de piel
        float sk_r = 0.949019608;
        float sk_g = 0.862745098;
        float sk_b = 0.662745098;
        //Red sweater
        torso->setMaterialColor(1,0,0);
        right_upper->setarmColor(1,0,0);
        right_upper->setForearmColor(1,0,0);
        left_upper->setarmColor(1,0,0);
        left_upper->setForearmColor(1,0,0);
        //Blue Jeans
        right_lower->setThighColor(0,0,1);
        right_lower->setCalfColor(0,0,1);
        left_lower->setThighColor(0,0,1);
        left_lower->setCalfColor(0,0,1);
        //Hands/Head/Feet
        head->setMaterialColor(sk_r, sk_g, sk_b);
        left_upper->setHandColor(sk_r, sk_g, sk_b);
        right_upper->setHandColor(sk_r, sk_g, sk_b);
        left_lower->setFootColor(sk_r, sk_g, sk_b);
        right_lower->setFootColor(sk_r, sk_g, sk_b);
    }
    void render()
    {
        torso->render();
        
        glPushMatrix(); //Lo de arriba se afecta por la rotacion de hombros
            glRotatef( torso->getChestTorsion() , 0,1,0);
        
            glPushMatrix();
                glTranslatef(0,0.95,0); //Subir y pintar cabeza
                head->render();
            glPopMatrix();
        
            glPushMatrix();
                glTranslatef(-0.7,0.2,0);   //Movernos izq y pintar brazo
                right_upper->render();
            glPopMatrix();
        
            glPushMatrix();
                glTranslatef(0.7,0.2,0);    //Movernos der y pintar brazo
                left_upper->render();
            glPopMatrix();
        glPopMatrix();
        
        glPushMatrix(); //Lo de abajo se afecta por rotacion de cadera(Stomach)
        
            glRotatef( torso->getStomachTorsion() , 0,1,0);
        
            glPushMatrix();
                glTranslatef(-0.2,-0.75,0); //Bajar y pintar Derecha
                right_lower->render();
            glPopMatrix();
        
            glPushMatrix();
                glTranslatef(0.2,-0.75,0); //Bajar y pintar Izq
                left_lower->render();
            glPopMatrix();
        
        glPopMatrix();
    }
};
