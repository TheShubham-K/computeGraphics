//
//  Part.h
//  glutCpp
//
//  Created by Ruben Cuadra on 2/26/17.
//  Copyright © 2017 Ruben Cuadra. All rights reserved.
//

#pragma once
class Part
{
protected:
    float minTorsion,maxTorsion;
    float minRotation,maxRotation;
    float minRevolution,maxRevolution;
    float torsion,rotation,revolution; //Angles
    GLfloat *matColor;
    Part(float minRot, float maxRot,float minT,float maxT,float minRev, float maxRev):
    minTorsion(minT),maxTorsion(maxT),
    minRotation(minRot),maxRotation(maxRot),
    minRevolution(minRev),maxRevolution(maxRev),
    torsion(0),rotation(0),revolution(0)
    {
        matColor = new GLfloat[4];
        matColor[0] = 0.3;
        matColor[1] = 0.3;
        matColor[2] = 0.3;
        matColor[3] = 1.0;
    }//Constructor
    
public:
    bool setRotation(float rotation)
    {
        rotation = ((int)rotation)%365;
        if (rotation<maxRotation && rotation>minRotation)
        {
            this->rotation = rotation;
            return true;
        }
        return false;
    }
    bool setTorsion (float tor)
    {
        tor = ((int)tor)%365;
        if (tor < maxTorsion && tor > minTorsion)
        {
            this->torsion = tor;
            return true;
        }
        return false;
    }
    bool setRevolution (float rev)
    {
        rev = ((int)rev)%365;
        
        if (rev < maxRevolution && rev > minRevolution)
        {
            this->revolution = rev;
            return true;
        }
        return false;
    }
    float getTorsion(){return torsion;}
    float getRotation(){return rotation;}
    float getRevolution(){return revolution;}
    
    void setMaterialColor(float r,float g,float b)
    {
        matColor[0] = r;
        matColor[1]=g;
        matColor[2]=b;
    }
    
    float getRed(){return matColor[0];}
    float getBlue(){return matColor[1];}
    float getGreen(){return matColor[2];}
  
    virtual void render() = 0;
};
