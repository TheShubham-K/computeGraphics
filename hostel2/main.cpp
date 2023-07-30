#include<stdio.h>
#include<math.h>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include<errno.h>
#include<GL\GL.h>
#include<GL\GLU.h>
#include <time.h>
#include<GL\glut.h>
#include "Humanoid.h"

float y = 1, x = -30, z = 120; // initially 5 units south of origin
float deltaMove = 0.0; // initially camera doesn't move
// Camera direction
float lx = 0.0, lz = -1; // camera points initially along y-axis
//float angle = 0.0; // angle of rotation for the camera direction
//float deltaAngle = 0.0; // additional angle change when dragging
float angle = 0.0; // angle of rotation for the camera direction
float deltaAngle = 0.0; // additional angle change when dragging

// Mouse drag control
int isDragging = 0; // true when dragging
int xDragStart = 0; // records the x-coordinate when dragging starts
int move=0;
int vertmove=0;
// flag
int flag=0;
double mgo=0, mvo=0;

unsigned char header[54];        // Each BMP file begins by a 54-bytes header
unsigned int dataPos;            // Position in the file where the actual data begins
unsigned int width, height;
unsigned int imageSize;            // = width*height*3
unsigned char * data=NULL;        // Actual RGB data
GLUquadricObj *Cylinder;
GLUquadricObj *Disk;

Human *human;


// font
void *currentfont;

void setFont(void *font)
{
    currentfont=font;
}

void draw_board()
{
    glColor3f(0.177,0.564,1);
    glBegin(GL_QUADS);
        glVertex3f(0,0,0);
        glVertex3f(1,0,0);
        glVertex3f(1,2,0);
        glVertex3f(0,2,0);
        glVertex3f(9,0,0);
        glVertex3f(10,0,0);
        glVertex3f(10,2,0);
        glVertex3f(9,2,0);
    glEnd();
    glColor3f(0.690,0.878,0.901);
    glBegin(GL_QUADS);
        glVertex3f(0,2,0);
        glVertex3f(10,2,0);
        glVertex3f(10,4,0);
        glVertex3f(0,4,0);
    glEnd();
}


void draw_tri_board()
{
    glColor3f(0.690,0.878,0.901);
    glBegin(GL_TRIANGLES);
        glVertex3f(1,0,0);
        glVertex3f(0,1,0);
        glVertex3f(0,0,1);
    glEnd();
}


void draw_msg_board(char text[90],char side='/0')
{
    glColor3f(0.690,0.878,0.901);
    glBegin(GL_QUADS);
        glVertex3f(0,2,0);
        glVertex3f(4,2,0);
        glVertex3f(4,3,0);
        glVertex3f(0,3,0);
    glEnd();
    glPushMatrix();
    glTranslatef(0,2.5,0.1);
    glScalef(.001, .001, .01);
    glLineWidth(2);
    glColor3f(0,0,0);
    for (int c=0; text[c] != 0; ++c){
        glutStrokeCharacter(GLUT_STROKE_ROMAN, text[c]);
    }
    glPopMatrix();
}



GLuint loadBMP_custom(const char * imagepath)
{
    // Open the file
    FILE * file = fopen(imagepath,"rb");
    if (!file)
    {
        printf("Image could not be opened\n");
        printf("Error %d \n", errno);
        return 0;
    }
    if ( fread(header, 1, 54, file)!=54 )
    { // If not 54 bytes read : problem
        printf("Not a correct BMP file\n");
        return false;
    }
    if ( header[0]!='B' || header[1]!='M' )
    {
        printf("Not a correct BMP file\n");
        return 0;
    }
    dataPos    = *(int*)&(header[0x0A]);
    imageSize  = *(int*)&(header[0x22]);
    width      = *(int*)&(header[0x12]);
    height     = *(int*)&(header[0x16]);
    // Some BMP files are misformatted, guess missing information
    if (imageSize==0)
        imageSize=width*height*3; // 3 : one byte for each Red, Green and Blue component
    if (dataPos==0)
        dataPos=54; // The BMP header is done that way
    // Create a buffer
    data = new unsigned char [imageSize];
    // Read the actual data from the file into the buffer
    fread(data,1,imageSize,file);
    //Everything is in memory now, the file can be closed
    fclose(file);
}

void draw_map()
{
    GLuint Texture = loadBMP_custom("field.bmp");
    glEnable(GL_TEXTURE_2D);
    GLuint textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    // Set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // Set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // Load image, create texture and generate mipmaps
    glTexImage2D(GL_TEXTURE_2D, 0,GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);

    glColor3f(1.0f,1.0f,1.0f);
        glBegin(GL_QUADS);
        glTexCoord2f(0.0f, 0.0f);    glVertex3f(-15.0f, -3.0f, 140.0f);
        glTexCoord2f(10.0f, 0.0f);    glVertex3f(30.0f, -3.0f, 140.0f);
        glTexCoord2f(10.0f, 10.0f);    glVertex3f( 30.0f, -3.0f, 100.0f);
        glTexCoord2f(0.0f, 10.0f);    glVertex3f( -15.0f, -3.0f, 100.0f);
        glEnd();

    glBindTexture(GL_TEXTURE_2D, 0); // Unbind texture when done, so we won't accidentily mess up our texture.
    delete []data;
    glDeleteTextures(1, &textureID);
    glDisable(GL_TEXTURE_2D);
}

void draw_idol()
{
    GLuint Texture = loadBMP_custom("Vishnu.bmp");
    glEnable(GL_TEXTURE_2D);
    GLuint textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    // Set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // Set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // Load image, create texture and generate mipmaps
    glTexImage2D(GL_TEXTURE_2D, 0,GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);


    glColor3f(1.0f,1.0f,1.0f);
        glBegin(GL_QUADS);
        glTexCoord2f(0.0f, 0.0f);    glVertex3f(0.0f, .0f, 0.0f);
        glTexCoord2f(1.0f, 0.0f);    glVertex3f(4.0f, 0.0f, 0.0f);
        glTexCoord2f(1.0f, 1.0f);    glVertex3f( 4.0f, 5.0f, 0.0f);
        glTexCoord2f(0.0f, 1.0f);    glVertex3f( 0.0f, 5.0f, 0.0f);
        glEnd();

    glBindTexture(GL_TEXTURE_2D, 0); // Unbind texture when done, so we won't accidentily mess up our texture.
    delete []data;
    glDeleteTextures(1, &textureID);
    glDisable(GL_TEXTURE_2D);
}



void drawstring(float x,float y,float z,char *string)
{
    char *c;
    int len = (int) strlen(string);
    int i;
    glRasterPos3f(x,y,z);
    for(i = 0;i<len;i++)
    {
        glColor3f(0.0,0.0,0.0);
        glutBitmapCharacter(currentfont,string[i]);
    }
}


void restrict()			//restrict movement within boundaries
{
    if(x>200) x=200;
    else if(x<-200) x=-200;
    if(y>120) y=120;
    else if(y<0.5) y=0.5;
    if(z>200) z=200;
    else if(z<-200) z=-200;
}




void cateen_inner()		//canteen block interiors
{
    int k;
    glPushMatrix();
        glTranslatef(-65,0.1,-74);
            glColor3f(0.5,0.5,0.5);

            glBegin(GL_QUADS);
                glVertex3f(0,0,0);
                glVertex3f(75,0,0);
                glVertex3f(75,0,-35);
                glVertex3f(0,0,-35);
            glEnd();


            glColor3f(0,0,0);
            /**
            for(k=2;k<18;k+=4)
            {
                glBegin(GL_QUADS);
                    glVertex3f(k,0,-0.8);
                    glVertex3f(k,4,-0.1);
                    glVertex3f(k+2,4,-0.1);
                    glVertex3f(k+2,0,-0.1);
                glEnd();
            }
            */
            /*
            for(k=-2;k>-38;k-=6)
                {
                    glBegin(GL_QUADS);
                        glVertex3f(0.1,0,k);
                        glVertex3f(0.1,4,k);
                        glVertex3f(0.1,4,k-2);
                        glVertex3f(0.1,0,k-2);
                    glEnd();
                }*/
    glPopMatrix();
    glPushMatrix();
        glTranslatef(-50,0.1,-30);
        /*
            for(k=-2;k>-38;k-=6)
                    {
                        glBegin(GL_QUADS);
                            glVertex3f(-0.1,0,k);
                            glVertex3f(-0.1,4,k);
                            glVertex3f(-0.1,4,k-2);
                            glVertex3f(-0.1,0,k-2);
                        glEnd();
                    }
                */
    glPopMatrix();
    glPushMatrix();
        glTranslatef(-50,0.1,-70);
            /*
            for(k=-2;k>-18;k-=4)
                    {
                        glBegin(GL_QUADS);
                            glVertex3f(k,0,0.1);
                            glVertex3f(k,4,0.1);
                            glVertex3f(k-2,4,0.1);
                            glVertex3f(k-2,0,0.1);
                        glEnd();
                        }
                        */
    glPopMatrix();

    // making the floors for the canteen
    for(int i=0;i<35;i+=5)
    {
        // main part displaying floors of the canteen
        glPushMatrix();
            glTranslatef(-63,10+i,-75);
                glColor3f(1,0.894,0.709);
                glBegin(GL_QUADS);
                    glVertex3f(0,0,0);
                    glVertex3f(64,0,0);
                    glVertex3f(64,0,-4);
                    glVertex3f(0,0,-4);
                glEnd();
                glColor3f(1,0.972,0.862);
                glBegin(GL_QUADS);
                    glVertex3f(4,0,-4);
                    glVertex3f(64,0,-4);
                    glVertex3f(64,2,-4);
                    glVertex3f(4,2,-4);
                glEnd();


                // front windows of the cateen

                /**
                */
                glColor3f(0,0,0);
                for(k=3;k<60;k+=7)
                {
                    glBegin(GL_QUADS);
                        glVertex3f(k,0,-0.1);
                        glVertex3f(k,4,-0.1);
                        glVertex3f(k+2,4,-0.1);
                        glVertex3f(k+2,0,-0.1);
                    glEnd();
                }

                // positioning the floors of the canteen

                glPushMatrix();
                    glTranslatef(68,0,0);
                        glColor3f(1,0.894,0.709);
                        glBegin(GL_QUADS);
                            glVertex3f(0,0,0);
                            glVertex3f(0,0,-40);
                            glVertex3f(-4,0,-40);
                            glVertex3f(-4,0,0);
                        glEnd();
                        glColor3f(1,0.972,0.862);
                        glBegin(GL_QUADS);
                            glVertex3f(-4,0,-4);
                            glVertex3f(-4,0,-40);
                            glVertex3f(-4,2,-40);
                            glVertex3f(-4,2,-4);
                        glEnd();
                    glColor3f(0,0,0);
                    /**
                    right side windows of the canteen
                    */
                    for(k=-3;k>-40;k-=8)
                    {
                        glBegin(GL_QUADS);
                            glVertex3f(-0.1,0,k);
                            glVertex3f(-0.1,4,k);
                            glVertex3f(-0.1,4,k-2);
                            glVertex3f(-0.1,0,k-2);
                        glEnd();
                    }
                    /**
                    back corridor of each floor
                    */
                    glTranslatef(0,0,-40);
                    glColor3f(1,0.894,0.709);
                    glBegin(GL_QUADS);
                        glVertex3f(0,0,0);
                        glVertex3f(-64,0,0);
                        glVertex3f(-64,0,4);
                        glVertex3f(0,0,4);
                    glEnd();
                    glColor3f(1,0.972,0.862);
                    glBegin(GL_QUADS);
                        glVertex3f(-4,0,4);
                        glVertex3f(-64,0,4);
                        glVertex3f(-64,2,4);
                        glVertex3f(-4,2,4);
                    glEnd();
                    glColor3f(0,0,0);
                    /**
                    back side windows
                    */
                    for(k=-3;k>-60;k-=7)
                    {
                        glBegin(GL_QUADS);
                            glVertex3f(k,0,0.1);
                            glVertex3f(k,4,0.1);
                            glVertex3f(k-2,4,0.1);
                            glVertex3f(k-2,0,0.1);
                        glEnd();
                    }
                glPopMatrix();
                glColor3f(1,0.894,0.709);
                /**
                left side corridor
                */
                glBegin(GL_QUADS);
                    glVertex3f(0,0,0);
                    glVertex3f(0,0,-40);
                    glVertex3f(4,0,-40);
                    glVertex3f(4,0,0);
                glEnd();
                glColor3f(1,0.972,0.862);
                /**
                left side reeling
                */
                glBegin(GL_QUADS);
                    glVertex3f(4,0,-4);
                    glVertex3f(4,0,-36);
                    glVertex3f(4,2,-36);
                    glVertex3f(4,2,-4);
                glEnd();
                glColor3f(0,0,0);
                for(k=-3;k>-40;k-=8)
                {
                    glBegin(GL_QUADS);
                        glVertex3f(0.1,0,k);
                        glVertex3f(0.1,4,k);
                        glVertex3f(0.1,4,k-2);
                        glVertex3f(0.1,0,k-2);
                    glEnd();
                }
        glPopMatrix();
        }

}



class temple		//construction of temple
{
    float stair[4][3];
    float room[8][3];
    float ceil[6][3];
public:
    temple()
    {
        stair[0][0]=0;stair[0][1]=0;stair[0][2]=0;
        stair[1][0]=12;stair[1][1]=0;stair[1][2]=0;
        stair[2][0]=12;stair[2][1]=0;stair[2][2]=-12;
        stair[3][0]=0;stair[3][1]=0;stair[3][2]=-12;

        room[0][0]=0;room[0][1]=0;room[0][2]=0;
        room[1][0]=0;room[1][1]=6;room[1][2]=0;
        room[2][0]=0;room[2][1]=6;room[2][2]=-7;
        room[3][0]=0;room[3][1]=0;room[3][2]=-7;
        room[4][0]=7;room[4][1]=0;room[4][2]=-7;
        room[5][0]=7;room[5][1]=6;room[5][2]=-7;
        room[6][0]=7;room[6][1]=6;room[6][2]=0;
        room[7][0]=7;room[7][1]=0;room[7][2]=0;

        ceil[0][0]=0;ceil[0][1]=6;ceil[0][2]=4;
        ceil[1][0]=3.5;ceil[1][1]=9;ceil[1][2]=4;
        ceil[2][0]=7;ceil[2][1]=6;ceil[2][2]=4;
        ceil[3][0]=7;ceil[3][1]=6;ceil[3][2]=-9;
        ceil[4][0]=3.5;ceil[4][1]=9;ceil[4][2]=-9;
        ceil[5][0]=0;ceil[5][1]=6;ceil[5][2]=-9;
    }
    void disp_stair(int x, int y, int z)
    {
        glColor3f(1,0.960,0.933);
        glBegin(GL_QUADS);
            glVertex3f(stair[0][0]-x,stair[0][1]-y,stair[0][2]+z);
            glVertex3f(stair[1][0]+x,stair[1][1]-y,stair[1][2]+z);
            glVertex3f(stair[2][0]+x,stair[2][1]-y,stair[2][2]-z);
            glVertex3f(stair[3][0]-x,stair[3][1]-y,stair[3][2]-z);
        glEnd();
        glColor3f(0.933,0.913,0.8);
        glBegin(GL_QUADS);
            glVertex3f(stair[0][0]-x,stair[0][1]-y,stair[0][2]+z);
            glVertex3f(stair[0][0]-x,stair[0][1]-1-y,stair[0][2]+z);
            glVertex3f(stair[1][0]+x,stair[1][1]-1-y,stair[1][2]+z);
            glVertex3f(stair[1][0]+x,stair[1][1]-y,stair[1][2]+z);

            glVertex3f(stair[1][0]+x,stair[1][1]-y,stair[1][2]+z);
            glVertex3f(stair[1][0]+x,stair[1][1]-1-y,stair[1][2]+z);
            glVertex3f(stair[2][0]+x,stair[2][1]-1-y,stair[2][2]-z);
            glVertex3f(stair[2][0]+x,stair[2][1]-y,stair[2][2]-z);

            glVertex3f(stair[2][0]+x,stair[2][1]-y,stair[2][2]-z);
            glVertex3f(stair[3][0]-x,stair[3][1]-y,stair[3][2]-z);
            glVertex3f(stair[3][0]-x,stair[3][1]-1-y,stair[3][2]-z);
            glVertex3f(stair[2][0]+x,stair[2][1]-1-y,stair[2][2]-z);

            glVertex3f(stair[3][0]-x,stair[3][1]-y,stair[3][2]-z);
            glVertex3f(stair[0][0]-x,stair[0][1]-y,stair[0][2]+z);
            glVertex3f(stair[0][0]-x,stair[0][1]-1-y,stair[0][2]+z);
            glVertex3f(stair[3][0]-x,stair[3][1]-1-y,stair[3][2]-z);
        glEnd();
    }

    void disp_room()
    {
        glColor3f(0.803,0.803,0.756);
        glBegin(GL_QUADS);
            glVertex3fv(room[0]);
            glVertex3fv(room[1]);
            glVertex3fv(room[2]);
            glVertex3fv(room[3]);
            glVertex3fv(room[3]);
            glVertex3fv(room[2]);
            glVertex3fv(room[5]);
            glVertex3fv(room[4]);
            glVertex3fv(room[4]);
            glVertex3fv(room[5]);
            glVertex3fv(room[6]);
            glVertex3fv(room[7]);
            glVertex3fv(room[1]);
            glVertex3fv(room[2]);
            glVertex3fv(room[5]);
            glVertex3fv(room[6]);
            glVertex3fv(room[0]);
            glVertex3f(room[0][0]+1,room[0][1],room[0][2]);
            glVertex3f(room[1][0]+1,room[1][1],room[1][2]);
            glVertex3fv(room[1]);
            glVertex3fv(room[7]);
            glVertex3f(room[7][0]-1,room[7][1],room[7][2]);
            glVertex3f(room[6][0]-1,room[6][1],room[6][2]);
            glVertex3fv(room[6]);
        glEnd();
    }
    void disp_ceil()
    {
        glColor3f(1,0.843,0);
        glBegin(GL_TRIANGLES);
            glVertex3fv(ceil[2]);
            glVertex3fv(ceil[1]);
            glVertex3fv(ceil[0]);
            glVertex3fv(ceil[3]);
            glVertex3fv(ceil[4]);
            glVertex3fv(ceil[5]);
        glEnd();
        glColor3f(0.933,0.866,0.509);
        glBegin(GL_POLYGON);
            glVertex3fv(ceil[2]);
            glVertex3fv(ceil[1]);
            glVertex3fv(ceil[4]);
            glVertex3fv(ceil[3]);
        glEnd();
        glBegin(GL_POLYGON);
            glVertex3fv(ceil[0]);
            glVertex3fv(ceil[1]);
            glVertex3fv(ceil[4]);
            glVertex3fv(ceil[5]);
        glEnd();
    }
    void draw_pil()
    {
        GLUquadricObj *quadratic;
        quadratic = gluNewQuadric();
        glPushMatrix();
        glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);
        glColor3f(0.933,0.866,0.509);
        gluCylinder(quadratic,0.5,0.5,6.0f,32,32);
        glPopMatrix();
    }
    void draw_mesh()
    {
        glColor3f(1,0.843,0);
        for(float i=0;i<0.9;i+=0.2)
            for(float j=0;j<6;j+=0.2)
            {
                glBegin(GL_LINE_LOOP);
                    glVertex3f(i,j,0);
                    glVertex3f(i+0.2,j,0);
                    glVertex3f(i+0.2,j+0.2,0);
                    glVertex3f(i,j+0.2,0);
                glEnd();
            }
    }
    void disp_temple()
    {
        disp_stair(0,0,0);
        glPushMatrix();
            disp_stair(2,1,2);
            disp_stair(4,2,4);

        glPopMatrix();
        glPushMatrix();
        glTranslatef(8,0,-2);
        glRotatef(90,0,1,0);
            disp_room();
            disp_ceil();
            glPushMatrix();
            glTranslatef(0.4,0,2.5);
                draw_pil();
            glTranslatef(6.2,0,0);
                draw_pil();
            glPopMatrix();
            glPushMatrix();
            glTranslatef(1.5,0,-3);
                draw_idol();
            glPopMatrix();
            glPushMatrix();
            glTranslatef(1,0,0);
                draw_mesh();
            glTranslatef(4,0,0);
                draw_mesh();
            glPopMatrix();
        glPopMatrix();

    }

}temp;


class ground		//construction of the football field
{
    float bordr[4][3];
public:
    ground()
    {
        bordr[0][0]=0;bordr[0][1]=-2;bordr[0][2]=0;
        bordr[1][0]=40;bordr[1][1]=-2;bordr[1][2]=0;
        bordr[2][0]=40;bordr[2][1]=-2;bordr[2][2]=-40;
        bordr[3][0]=0;bordr[3][1]=-2;bordr[3][2]=-40;
    }
    void ground_disp()
    {
        float t=6;
        for(int i=0;i<3;i++,t-=2)
        {
            glPushMatrix();
            glTranslatef(0,i,0);
            glColor3f(0.803,0.701,0.545);
            glBegin(GL_POLYGON);
                glVertex3f(bordr[0][0],bordr[0][1],bordr[0][2]);
                glVertex3f(bordr[1][0],bordr[1][1],bordr[1][2]);
                glVertex3f(bordr[1][0],bordr[1][1],bordr[1][2]-t);
                glVertex3f(bordr[0][0],bordr[0][1],bordr[0][2]-t);
            glEnd();
            glColor3f(0.545,0.474,0.368);
            glBegin(GL_POLYGON);
                glVertex3f(bordr[0][0],bordr[0][1],bordr[0][2]-t);
                glVertex3f(bordr[1][0],bordr[1][1],bordr[1][2]-t);
                glVertex3f(bordr[1][0],bordr[1][1]-1,bordr[1][2]-t);
                glVertex3f(bordr[0][0],bordr[0][1]-1,bordr[0][2]-t);
            glEnd();
            glColor3f(0.803,0.701,0.545);
            glBegin(GL_POLYGON);
                glVertex3f(bordr[1][0],bordr[1][1],bordr[1][2]);
                glVertex3f(bordr[2][0],bordr[2][1],bordr[2][2]);
                glVertex3f(bordr[2][0]-t,bordr[2][1],bordr[2][2]);
                glVertex3f(bordr[1][0]-t,bordr[1][1],bordr[1][2]);
            glEnd();
            glColor3f(0.545,0.474,0.368);
            glBegin(GL_POLYGON);
                glVertex3f(bordr[1][0]-t,bordr[1][1],bordr[1][2]);
                glVertex3f(bordr[2][0]-t,bordr[2][1],bordr[2][2]);
                glVertex3f(bordr[2][0]-t,bordr[2][1]-1,bordr[2][2]);
                glVertex3f(bordr[1][0]-t,bordr[1][1]-1,bordr[1][2]);
            glEnd();
            glColor3f(0.803,0.701,0.545);
            glBegin(GL_POLYGON);
                glVertex3f(bordr[2][0],bordr[2][1],bordr[2][2]);
                glVertex3f(bordr[3][0],bordr[3][1],bordr[3][2]);
                glVertex3f(bordr[3][0],bordr[3][1],bordr[3][2]+t);
                glVertex3f(bordr[2][0],bordr[2][1],bordr[2][2]+t);
            glEnd();
            glColor3f(0.545,0.474,0.368);
            glBegin(GL_POLYGON);
                glVertex3f(bordr[2][0],bordr[2][1],bordr[2][2]+t);
                glVertex3f(bordr[3][0],bordr[3][1],bordr[3][2]+t);
                glVertex3f(bordr[3][0],bordr[3][1]-1,bordr[3][2]+t);
                glVertex3f(bordr[2][0],bordr[2][1]-1,bordr[2][2]+t);
            glEnd();
            glColor3f(0.803,0.701,0.545);
            glBegin(GL_POLYGON);
                glVertex3f(bordr[3][0],bordr[3][1],bordr[3][2]);
                glVertex3f(bordr[0][0],bordr[0][1],bordr[0][2]);
                glVertex3f(bordr[0][0]+t,bordr[0][1],bordr[0][2]);
                glVertex3f(bordr[3][0]+t,bordr[3][1],bordr[3][2]);
            glEnd();
            glColor3f(0.545,0.474,0.368);
            glBegin(GL_POLYGON);
            glVertex3f(bordr[0][0]+t,bordr[0][1],bordr[0][2]);
                glVertex3f(bordr[3][0]+t,bordr[3][1],bordr[3][2]);
                glVertex3f(bordr[3][0]+t,bordr[3][1]-1,bordr[3][2]);
                glVertex3f(bordr[0][0]+t,bordr[0][1]-1,bordr[0][2]);
            glEnd();
            glPopMatrix();
        }
        glPushMatrix();
        glTranslatef(16.5,-3,-7);
        glColor3f(0.827,0.827,0.827);
        glLineWidth(10);
        glBegin(GL_LINE_LOOP);
            glVertex3f(0,0,0);
            glVertex3f(0,2,0);
            glVertex3f(4,2,0);
            glVertex3f(4,0,0);
        glEnd();
        glPopMatrix();
        glPushMatrix();
        glTranslatef(16.5,-3,-33);
        glColor3f(0.827,0.827,0.827);
        glLineWidth(10);
        glBegin(GL_LINE_LOOP);
            glVertex3f(0,0,0);
            glVertex3f(0,2,0);
            glVertex3f(4,2,0);
            glVertex3f(4,0,0);
        glEnd();
        glPopMatrix();
    }
}fball;



class building			//construction of the block buildings
{
    float structure[8][3];
public:
    building(float a, float b, float c)
    {
        structure[0][0]=0;structure[0][1]=0;structure[0][2]=0;
        structure[1][0]=a;structure[1][1]=0;structure[1][2]=0;
        structure[2][0]=a;structure[2][1]=b;structure[2][2]=0;
        structure[3][0]=0;structure[3][1]=b;structure[3][2]=0;
        structure[4][0]=0;structure[4][1]=0;structure[4][2]=c;
        structure[5][0]=a;structure[5][1]=0;structure[5][2]=c;
        structure[6][0]=a;structure[6][1]=b;structure[6][2]=c;
        structure[7][0]=0;structure[7][1]=b;structure[7][2]=c;
    }
    void disp_build(char text[15],char side='/0')
    {
        float door[3];
        glColor3f(1,0.980,0.980);
        glBegin(GL_QUADS);
            glVertex3fv(structure[0]);
            glVertex3fv(structure[1]);
            glVertex3fv(structure[2]);
            glVertex3fv(structure[3]);
        glEnd();
        glBegin(GL_QUADS);
                glVertex3fv(structure[0]);
                glVertex3fv(structure[4]);
                glVertex3fv(structure[7]);
                glVertex3fv(structure[3]);
        glEnd();
        glBegin(GL_QUADS);
            glVertex3fv(structure[4]);
            glVertex3fv(structure[5]);
            glVertex3fv(structure[6]);
            glVertex3fv(structure[7]);
        glEnd();
        glBegin(GL_QUADS);
                glVertex3fv(structure[1]);
                glVertex3fv(structure[2]);
                glVertex3fv(structure[6]);
                glVertex3fv(structure[5]);
        glEnd();

        if(structure[1][0]>(-1*structure[4][2]))
        {
            for(float i=10; i<structure[2][1]; i+=10)
            {
                glPushMatrix();
                glTranslatef(0,i,0);
                for(float j=5; j<structure[1][0]; j+=15)
                {
                    glColor3f(0,0,0);
                    glBegin(GL_POLYGON);
                        glVertex3f(j,0,0.1);
                        glVertex3f(j+5,0,0.1);
                        glVertex3f(j+5,5,0.1);
                        glVertex3f(j,5,0.1);
                    glEnd();
                    glBegin(GL_POLYGON);
                        glVertex3f(j,0,structure[4][2]-0.1);
                        glVertex3f(j+5,0,structure[4][2]-0.1);
                        glVertex3f(j+5,5,structure[4][2]-0.1);
                        glVertex3f(j,5,structure[4][2]-0.1);
                    glEnd();
                }
                for(float j=0;j<structure[1][0];j+=15)
                {
                    glColor3f(1,0,0);
                    glBegin(GL_POLYGON);
                        glVertex3f(j,-10,0.1);
                        glVertex3f(j+2,-10,0.1);
                        glVertex3f(j+2,10,0.1);
                        glVertex3f(j,10,0.1);
                    glEnd();
                    glBegin(GL_POLYGON);
                        glVertex3f(j,-10,structure[4][2]-0.1);
                        glVertex3f(j+2,-10,structure[4][2]-0.1);
                        glVertex3f(j+2,10,structure[4][2]-0.1);
                        glVertex3f(j,10,structure[4][2]-0.1);
                    glEnd();
                }
                glPopMatrix();
            }
            glColor3f(0,0,0);
            door[0]=(structure[1][0]/2);
            glBegin(GL_POLYGON);
                glVertex3f(door[0]-4,0,0.2);
                glVertex3f(door[0]+4,0,0.2);
                glVertex3f(door[0]+4,7,0.2);
                glVertex3f(door[0]-4,7,0.2);
            glEnd();
            glPushMatrix();
            glTranslatef(10,0,3);
            draw_board();
            glPushMatrix();
            glTranslatef(1,2,0.1);
            glScalef(.01, .01, .01);
            glLineWidth(2);
            glColor3f(0,0,0);
            for (int c=0; text[c] != 0; ++c)
                glutStrokeCharacter(GLUT_STROKE_ROMAN, text[c]);
            glPopMatrix();
            glPopMatrix();
        }
        else
        {
            for(float i=10; i<structure[2][1]; i+=10)
            {
                glPushMatrix();
                glTranslatef(0,i,0);
                for(float j=-5; j>structure[4][2]; j-=15)
                {
                    glColor3f(0,0,0);
                    glBegin(GL_POLYGON);
                        glVertex3f(-0.1,0,j);
                        glVertex3f(-0.1,0,j-5);
                        glVertex3f(-0.1,5,j-5);
                        glVertex3f(-0.1,5,j);
                    glEnd();
                    glBegin(GL_POLYGON);
                        glVertex3f(structure[1][0]+0.1,0,j);
                        glVertex3f(structure[1][0]+0.1,0,j-5);
                        glVertex3f(structure[1][0]+0.1,5,j-5);
                        glVertex3f(structure[1][0]+0.1,5,j);
                    glEnd();
                }
                for(float j=0;j>structure[4][2];j-=15)
                {
                    glColor3f(1,0,0);
                    glBegin(GL_POLYGON);
                        glVertex3f(-0.1,-10,j);
                        glVertex3f(-0.1,-10,j-2);
                        glVertex3f(-0.1,10,j-2);
                        glVertex3f(-0.1,10,j);
                    glEnd();
                    glBegin(GL_POLYGON);
                        glVertex3f(structure[1][0]+0.1,-10,j);
                        glVertex3f(structure[1][0]+0.1,-10,j-2);
                        glVertex3f(structure[1][0]+0.1,10,j-2);
                        glVertex3f(structure[1][0]+0.1,10,j);
                    glEnd();
                }
                glPopMatrix();
            }
            door[2]=(structure[4][2]/2);
            door[0]=structure[1][0];
            glColor3f(0,0,0);
            if(side=='r')
            {
                glBegin(GL_POLYGON);
                    glVertex3f(door[0]+0.2,0,door[2]-4);
                    glVertex3f(door[0]+0.2,0,door[2]+4);
                    glVertex3f(door[0]+0.2,7,door[2]+4);
                    glVertex3f(door[0]+0.2,7,door[2]-4);
                glEnd();
                glPushMatrix();
                glTranslatef(door[0]+3,0,-2);
                glRotatef(90,0,1,0);
                draw_board();
                glPushMatrix();
                glTranslatef(1,2,0.1);
                glScalef(.01, .01, .01);
                glLineWidth(2);
                glColor3f(0,0,0);
                for (int c=0; text[c] != 0; ++c)
                    glutStrokeCharacter(GLUT_STROKE_ROMAN, text[c]);
                glPopMatrix();
                glPopMatrix();
            }
            else if(side=='l')
            {
                glBegin(GL_POLYGON);
                    glVertex3f(-0.2,0,door[2]-4);
                    glVertex3f(-0.2,0,door[2]+4);
                    glVertex3f(-0.2,7,door[2]+4);
                    glVertex3f(-0.2,7,door[2]-4);
                glEnd();
                glPushMatrix();
                glTranslatef(-3,0,-10);
                glRotatef(-90,0,1,0);
                draw_board();
                glPushMatrix();
                glTranslatef(1,2,0.1);
                glScalef(.01, .01, .01);
                glLineWidth(2);
                glColor3f(0,0,0);
                for (int c=0; text[c] != 0; ++c)
                    glutStrokeCharacter(GLUT_STROKE_ROMAN, text[c]);
                glPopMatrix();
                glPopMatrix();
            }
        }
        glPushMatrix();
        glTranslatef(0,10,0);
        glColor3f(0,0,1);
        for(int i=0;i<structure[2][1]-5;i+=5)
        {
            glBegin(GL_LINES);
                glVertex3f(structure[0][0],i,structure[0][2]+0.1);
                glVertex3f(structure[1][0],i,structure[0][2]+0.1);
                glVertex3f(structure[0][0]-0.1,i,structure[0][2]);
                glVertex3f(structure[0][0]-0.1,i,structure[4][2]);
                glVertex3f(structure[4][0],i,structure[4][2]-0.1);
                glVertex3f(structure[5][0],i,structure[4][2]-0.1);
                glVertex3f(structure[5][0]+0.1,i,structure[5][2]);
                glVertex3f(structure[1][0]+0.1,i,structure[1][2]);

            glEnd();
        }
        glPopMatrix();
    }
};
building block3a(20,30,-40);
building block3(30,40,-40);
building canteen(75,50,-35);
building block4(50,40,-50);
building block2(40,30,-30);
building block1(30,40,-40);




class building_a			//construction of the watchmen room
{
    float structure[8][3];
public:
    building_a(float a, float b, float c)
    {
        structure[0][0]=0;
        structure[0][1]=0;
        structure[0][2]=0;
        structure[1][0]=a;
        structure[1][1]=0;
        structure[1][2]=0;
        structure[2][0]=a;
        structure[2][1]=b;
        structure[2][2]=0;
        structure[3][0]=0;
        //structure[3][1]=b;
        structure[3][2]=0;
        structure[4][0]=0;
        structure[4][1]=0;
        structure[4][2]=c;
        structure[5][0]=a;
        structure[5][1]=0;
        structure[5][2]=c;
        structure[6][0]=a;
        structure[6][1]=b;
        structure[6][2]=c;
        structure[7][0]=0;
        structure[7][1]=b;
        structure[7][2]=c;
    }
    void disp_build()
    {
        float door[3];
        glColor3f(1,0.980,0.980);
        glBegin(GL_QUADS);
            glVertex3fv(structure[0]);
            glVertex3fv(structure[1]);
            glVertex3fv(structure[2]);
            glVertex3fv(structure[3]);
        glEnd();
        glBegin(GL_QUADS);
                glVertex3fv(structure[0]);
                glVertex3fv(structure[4]);
                glVertex3fv(structure[7]);
                glVertex3fv(structure[3]);
        glEnd();
        glBegin(GL_QUADS);
            glVertex3fv(structure[4]);
            glVertex3fv(structure[5]);
            glVertex3fv(structure[6]);
            glVertex3fv(structure[7]);
        glEnd();
        glBegin(GL_QUADS);
                glVertex3fv(structure[1]);
                glVertex3fv(structure[2]);
                glVertex3fv(structure[6]);
                glVertex3fv(structure[5]);
        glEnd();

        if(structure[1][0]>(-1*structure[4][2]))
        {
            for(float i=10; i<structure[2][1]; i+=10)
            {
                glPushMatrix();
                glTranslatef(0,i,0);
                for(float j=0;j<structure[1][0];j+=15)
                {
                    glColor3f(1,0,0);
                    glBegin(GL_POLYGON);
                        glVertex3f(j,-10,0.1);
                        glVertex3f(j+2,-10,0.1);
                        glVertex3f(j+2,10,0.1);
                        glVertex3f(j,10,0.1);
                    glEnd();
                    glBegin(GL_POLYGON);
                        glVertex3f(j,-10,structure[4][2]-0.1);
                        glVertex3f(j+2,-10,structure[4][2]-0.1);
                        glVertex3f(j+2,10,structure[4][2]-0.1);
                        glVertex3f(j,10,structure[4][2]-0.1);
                    glEnd();
                }
                glPopMatrix();
            }
            glPushMatrix();
            glTranslatef(10,0,3);
            glPopMatrix();
        }
        glPushMatrix();
        glTranslatef(0,10,0);
        glColor3f(0,0,1);

        /**
            roof top lines
        */
        for(int i=0;i<structure[2][1]-5;i+=5)
        {
            glBegin(GL_LINES);
                glVertex3f(structure[0][0],i,structure[0][2]+0.1);
                glVertex3f(structure[1][0],i,structure[0][2]+0.1);
                glVertex3f(structure[0][0]-0.1,i,structure[0][2]);
                glVertex3f(structure[0][0]-0.1,i,structure[4][2]);
                glVertex3f(structure[4][0],i,structure[4][2]-0.1);
                glVertex3f(structure[5][0],i,structure[4][2]-0.1);
                glVertex3f(structure[5][0]+0.1,i,structure[5][2]);
                glVertex3f(structure[1][0]+0.1,i,structure[1][2]);

            glEnd();
        }
        glPopMatrix();
    }
};
building_a watchmen(9,10,-8);




void update(void)
{
    if (deltaMove) { // update camera position
        x += deltaMove*lx * 0.90;
        z += deltaMove*lz * 0.90;

    }
    if(vertmove==1)  y+=1.0;
    if(vertmove==-1) y-=1.0;
    restrict();
    glutPostRedisplay(); // redisplay everything
}

void reshape(int w, int h)
{
  glViewport(0, 0, w, h);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluOrtho2D(0, w, h, 0);
  glMatrixMode(GL_MODELVIEW);
}


void disp_roads()			//display the roads in the campus
{
    glColor3f(0.411,0.411,0.411);
    glBegin(GL_QUADS);
        glVertex3f(-40,0.1,90);
        glVertex3f(-40,0.1,-70);
        glVertex3f(-20,0.1,-70);
        glVertex3f(-20,0.1,90);
    glEnd();
    glBegin(GL_QUADS);
        glVertex3f(-20,0.1,55);
        glVertex3f(90,0.1,55);
        glVertex3f(90,0.1,60);
        glVertex3f(-20,0.1,60);
    glEnd();
    glBegin(GL_QUADS);
        glVertex3f(-20,0.1,75);
        glVertex3f(40,0.1,75);
        glVertex3f(40,0.1,80);
        glVertex3f(-20,0.1,80);
    glEnd();
    glBegin(GL_QUADS);
        glVertex3f(35,0.1,75);
        glVertex3f(35,0.1,-70);
        glVertex3f(40,0.1,-70);
        glVertex3f(40,0.1,75);
    glEnd();
}

void trees()			//draw a tree
{
    GLUquadricObj *quadratic;
    GLUquadricObj *quadratic1;
    quadratic1 = gluNewQuadric();
    quadratic = gluNewQuadric();
    glPushMatrix();
    glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);
    glColor3f(0.721,0.525,0.043);
    gluCylinder(quadratic,1,1,10.0f,32,32);
    glPopMatrix();
    glTranslatef(0,2,0);
        glPushMatrix();
    float k=0;
    for(int i=0,j=0;i<3;i++,j+=0.5,k+=0.15)
    {
        glTranslatef(0,1.8,0);
        glColor3f(0.133+k,0.545+k,0.133-k);
        glPushMatrix();
        glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);
            gluCylinder(quadratic1,4-j,0,4.0f,32,32);
        glPopMatrix();
    }
    glPopMatrix();
}

void draw_arch(char text[5])		//draw the arch
{
    glColor3f(1,0,0);
    glPushMatrix();
    glTranslatef(0,3.5,0);
    glScalef(4,7,2);
    glutSolidCube(1);
    glPopMatrix();
    glPushMatrix();
    glTranslatef(16,3.5,0);
    glScalef(4,7,2);
    glutSolidCube(1);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(8,9,0);
    glScalef(20,4,2);
    glutSolidCube(1);
    glPopMatrix();

    // middle rods
    /**
    glPushMatrix();
    glTranslatef(8,4,0);
    glScalef(1,1,2);
        glColor3f(0,0,0);
        glutSolidCube(1);
    glPopMatrix();
    */
    glPushMatrix();
    glTranslatef(1,8,1.1);
    glScalef(.01, .01, .01);
    glLineWidth(4.5);
    glColor3f(1,1,1);
    for (int c=0; text[c] != 0; ++c)
        glutStrokeCharacter(GLUT_STROKE_ROMAN, text[c]);
    glPopMatrix();
}

// front page
void frontscreen()
{
    glClearColor(1,0,0,1);
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

    glRasterPos3f(450,700,0);
    glColor3f(1,1,1);
    char msg8[]="NMAM Institute of Technology, NITTE ";
    for(int i=0;i<strlen(msg8);i++)
    {
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24,msg8[i]);
    }
    glRasterPos3f(450,600,0);
    glColor3f(1,1,1);
    char msg7[]="DEPARTMENT OF COMPUTER SCIENCE AND ENGINEERING";
    for(int i=0;i<strlen(msg7);i++)
    {
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24,msg7[i]);
    }
    glRasterPos3f(450,550,0);
    glColor3f(1,1,1);
    char msg6[]="A MINI PROJECT ON";
    for(int i=0;i<strlen(msg6);i++)
    {
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24,msg6[i]);
    }
    glRasterPos3f(450,450,0);
    glColor3f(1,1,1);
    char msg5[]="Hostel life simulation";
    for(int i=0;i<strlen(msg5);i++)
    {
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24,msg5[i]);
    }

    glRasterPos3f(450,400,0);
    glColor3f(1,1,1);
    char msg4[]="By:";
    for(int i=0;i<strlen(msg4);i++)
    {
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24,msg4[i]);
    }
    glRasterPos3f(450,300,0);
    glColor3f(1,1,1);
    char msg1[]="Shaun Serrao       (4NM18CS167) ";
    for(int i=0;i<strlen(msg1);i++)
    {
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24,msg1[i]);
    }
    glColor3f(1,1,1);
    glRasterPos3f(450,350,0);
    char msg2[]="Shubham Kumar      (4NM18CS189)";
    for(int i=0;i<strlen(msg2);i++)
    {
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24,msg2[i]);
    }

    glColor3f(1,1,1);
    glRasterPos3f(450,150,0);
    char msg3[]="Press Enter to start";
    for(int i=0;i<strlen(msg3);i++)
    {
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24,msg3[i]);
    }

    glutSwapBuffers();
}

void gate()
{
    if(mgo==0){

        // right gate
        glPushMatrix();
        glTranslatef(-36,0,90);
            glTranslatef(0,3.5,0);
            glScalef(0.1,6,0.5);
                glColor3f(0,0,0);
                glutSolidCube(1);
            glPopMatrix();
        glPopMatrix();

        glPushMatrix();
        glTranslatef(-35,0,90);
            glTranslatef(0,3.5,0);
            glScalef(0.1,6,0.5);
                glColor3f(0,0,0);
                glutSolidCube(1);
            glPopMatrix();
        glPopMatrix();

        glPushMatrix();
        glTranslatef(-34,0,90);
            glTranslatef(0,3.5,0);
            glScalef(0.1,6,0.5);
                glColor3f(0,0,0);
                glutSolidCube(1);
            glPopMatrix();
        glPopMatrix();
        glPushMatrix();
        glTranslatef(-33,0,90);
            glTranslatef(0,3.5,0);
            glScalef(0.1,6,0.5);
                glColor3f(0,0,0);
                glutSolidCube(1);
            glPopMatrix();
        glPopMatrix();

        glPushMatrix();
        glTranslatef(-32,0,90);
            glTranslatef(0,3.5,0);
            glScalef(0.1,6,0.5);
                glColor3f(0,0,0);
                glutSolidCube(1);
            glPopMatrix();
        glPopMatrix();

        glPushMatrix();
        glTranslatef(-31,0,90);
            glTranslatef(0,3.5,0);
            glScalef(0.1,6,0.5);
                glColor3f(0,0,0);
                glutSolidCube(1);
            glPopMatrix();
        glPopMatrix();
        glPushMatrix();
        glTranslatef(-30,0,90);
            glTranslatef(0,3.5,0);
            glScalef(0.1,6,0.5);
                glColor3f(0,0,0);
                glutSolidCube(1);
            glPopMatrix();
        glPopMatrix();


        glPushMatrix();
        glTranslatef(-29,0,90);
            glTranslatef(0,3.5,0);
            glScalef(0.1,6,0.5);
                glColor3f(0,0,0);
                glutSolidCube(1);
            glPopMatrix();
        glPopMatrix();

        glPushMatrix();
        glTranslatef(-28,0,90);
            glTranslatef(0,3.5,0);
            glScalef(0.1,6,0.5);
                glColor3f(0,0,0);
                glutSolidCube(1);
            glPopMatrix();
        glPopMatrix();

        glPushMatrix();
        glTranslatef(-27,0,90);
            glTranslatef(0,3.5,0);
            glScalef(0.1,6,0.5);
                glColor3f(0,0,0);
                glutSolidCube(1);
            glPopMatrix();
        glPopMatrix();
        glPushMatrix();
        glTranslatef(-26,0,90);
            glTranslatef(0,3.5,0);
            glScalef(0.1,6,0.5);
                glColor3f(0,0,0);
                glutSolidCube(1);
            glPopMatrix();
        glPopMatrix();

        glPushMatrix();
        glTranslatef(-25,0,90);
            glTranslatef(0,3.5,0);
            glScalef(0.1,6,0.5);
                glColor3f(0,0,0);
                glutSolidCube(1);
            glPopMatrix();
        glPopMatrix();

        // left gate
        glPushMatrix();
        glTranslatef(-24,0,90);
            glTranslatef(0,3.5,0);
            glScalef(0.05,6,0.5);
                glColor3f(0,0,0);
                glutSolidCube(1);
            glPopMatrix();
        glPopMatrix();
    }
}

void move_person()
{
    if(mvo==1){

        glPushMatrix();
        glTranslatef(-30, 3, 10);
            human->render();
        glPopMatrix();


    }
}

void matprop(GLfloat amb[], GLfloat dif[], GLfloat spec[], GLfloat shi[])
{
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, amb);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, dif);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, spec);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, shi);
}

void tableg(float llen, float lthk)
{
    glPushMatrix();
    glRotated(-90, 1, 0, 0);
    gluCylinder(Cylinder, lthk, lthk, llen, 32, 32);
    glPopMatrix();
}

void table(float tabwid, float tablen, float tabthk, float llen, float lthk)
{
    glPushMatrix();
    glPushMatrix();
    glTranslated(0, llen, 0);
    glScaled(tabwid, tabthk, tablen);
    glutSolidCube(1);
    glPopMatrix();
    float dist1 = .95 * tablen / 2 - lthk / 2;
    float dist2 = .95 * tabwid / 2 - lthk / 2;
    // front right leg
    glPushMatrix();
    glTranslated(dist2, 0, dist1);
    tableg(llen, lthk);
    glPopMatrix();
    //back right leg
    glPushMatrix();
    glTranslated(dist2, 0, -dist1);
    tableg(llen, lthk);
    glPopMatrix();
    //back left leg
    glPushMatrix();
    glTranslated(-dist2, 0, -dist1);
    tableg(llen, lthk);
    glPopMatrix();
    //front left leg
    glPushMatrix();
    glTranslated(-dist2, 0, dist1);
    tableg(llen, lthk);
    glPopMatrix();
    glPopMatrix();
}


void cot(float cwid, float clen, float cthk, float llen, float lthk)
{
    GLfloat ambient1[] = {1, 0, .4, 1};
    GLfloat specular1[] = {1, 1, 1, 1};
    GLfloat diffuse1[] = {0.5, 0.5, 0.5, 1};
    GLfloat mat_shininess[] = {50};
    matprop(ambient1, diffuse1, specular1, mat_shininess);
    glPushMatrix();
    glTranslated(5.6, 0, .5);
    table(cwid, clen, cthk, llen, lthk);
    glPushMatrix();
    glTranslated(0, llen, clen / 2);
    GLdouble eqn[3] = {0.0, 1.0, 0.0};
    glPushMatrix();
    glClipPlane(GL_CLIP_PLANE0, eqn); //
    glEnable(GL_CLIP_PLANE0); //enable clip plane
    gluDisk(Disk, 0, cwid / 2, 32, 32);
    glPopMatrix();
    glDisable(GL_CLIP_PLANE0);
    glPopMatrix();
    glPushMatrix();
    glTranslated(0, llen, -clen / 2);
    glPushMatrix();
    glClipPlane(GL_CLIP_PLANE0, eqn);
    glEnable(GL_CLIP_PLANE0);
    glScaled(1, 1.5, 1);
    gluDisk(Disk, 0, cwid / 2, 32, 32);
    glPopMatrix();
    glDisable(GL_CLIP_PLANE0);
    glPopMatrix();
    glPopMatrix();
}
void cleg(float cllen, float clwid)
{
    glRotated(90, 1, 0, 0);
    gluCylinder(Cylinder, clwid, clwid, cllen, 32, 32);
}
void chair(float cblen, float cbwid, float cbthk, float cllen, float clwid)
{
    GLfloat ambient1[] = {.5, 1, .5, 1};
    GLfloat specular1[] = {1, 1, 1, 1};
    GLfloat diffuse1[] = {0.5, 0.5, 0.5, 1};
    GLfloat mat_shininess[] = {50};
    matprop(ambient1, diffuse1, specular1, mat_shininess);
    glPushMatrix();
    glTranslated(0, cllen, 0);
    //chair baseglPushMatrix();
    glScaled(cblen, cbthk, cbwid);
    glutSolidCube(1);
    glPopMatrix();
    float dist = cblen / 2 - clwid / 2;
    //chair legs
    glPushMatrix();
    glTranslated(dist, 0, dist);
    cleg(cllen, clwid);
    glPopMatrix();
    glPushMatrix();
    glTranslated(-dist, 0, dist);
    cleg(cllen, clwid);
    glPopMatrix();
    glPushMatrix();
    glTranslated(-dist, 0, -dist);
    cleg(cllen, clwid);
    glPopMatrix();
    glPushMatrix();
    glTranslated(dist, 0, -dist);
    cleg(cllen, clwid);
    glPopMatrix();
    //base pipes
    glPushMatrix();
    glTranslated(-.085, -clwid / 2, cbwid / 3);
    glRotated(90, 0, 1, 0);
    gluCylinder(Cylinder, -clwid, clwid, cblen, 32, 32);
    glPopMatrix();
    glPushMatrix();
    glTranslated(-.085, clwid / 2, -cbwid / 3);
    glRotated(90, 0, 1, 0);
    gluCylinder(Cylinder, clwid, clwid, cblen, 32, 32);
    glPopMatrix();
    //back support pipes
    glPushMatrix();
    glTranslated(-.085, -clwid / 2, cbwid / 3);
    glRotated(-90, 1, 0, 0);
    gluCylinder(Cylinder, clwid, clwid, cllen, 32, 32);
    glPopMatrix();
    glPushMatrix();
    glTranslated(-.085, -clwid / 2, -cbwid / 3);
    glRotated(-90, 1, 0, 0);
    gluCylinder(Cylinder, clwid, clwid, cllen, 32, 32);
    glPopMatrix();
    //back support
    glPushMatrix();
    glTranslated(-cblen / 2, cllen / 2 + cblen / 2, 0);
    glRotated(90, 0, 0, 1);
    glScaled(cblen, .01, cbwid);
    glutSolidCube(1);
    glPopMatrix();
    glPopMatrix();
}

void diningtable()
{
    glPushMatrix();
    glTranslated(3, 0, 1);
    glScaled(5.5, 5.5, 5.5);
    table(.3, .5, .025, .4, .005);
    //front left chair
    glPushMatrix();
    glTranslated(-.1, 0, .1);
    chair(.15, .15, .02, .3, .005);
    glPopMatrix();
    //back left chair
    glPushMatrix();
    glTranslated(-.1, 0, -.1);
    chair(1.15, 1.15, 1.02, .3, .005);
    glPopMatrix();
    //front right chair
    glPushMatrix();
    glTranslated(.1, 0, .1);
    glRotated(180, 0, 1, 0);
    chair(.15, .15, .02, .3, .005);
    glPopMatrix();
    //back right chair
    glPushMatrix();
    glTranslated(.1, 0, -.1);
    glRotated(180, 0, 1, 0);
    chair(.15, .15, .02, .3, .005);
    glPopMatrix();
    //back chair
    glPushMatrix();
    glTranslated(0, 0, -.27);
    glRotated(-90, 0, 1, 0);
    chair(.15, .15, .02, .3, .005);
    glPopMatrix();
    //front chair
    glPushMatrix();
    glTranslated(0, 0, .27);
    glRotated(90, 0, 1, 0);
    chair(.15, .15, .02, .3, .005);
    glPopMatrix();
    glPopMatrix();
}


void display()
{

            float w = 1800;
            float h = 1800;
            float ratio =  ((float) w) / ((float) h); // window aspect ratio
            glMatrixMode(GL_PROJECTION); // projection matrix is active
            glLoadIdentity(); // reset the projection
            gluPerspective(45.0, ratio, 0.1, 100.0); // perspective transformation
            glMatrixMode(GL_MODELVIEW); // return to modelview mode
            glViewport(0, 0, w, h);

            // starting point of the display

            glClearColor(0.7,0.7,1,0);
            glClear( GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
            glEnable(GL_DEPTH_TEST);
            glLoadIdentity();


            gluLookAt(
            x,      y,      z,
            x+lx,    y,        z+lz,
            0.0,    1.0,    0.0);

            printf("\nz=%f\tx=%f\n",z,x);
            glColor3f(0,1,0);

            /**glBegin(GL_QUADS);
                glVertex3f(-200,0,200);
                glVertex3f(200,0,200);
                glVertex3f(200,0,20);
                glVertex3f(-200,0,20);
                glVertex3f(-200,0,20);
                glVertex3f(-15,0,20);
                glVertex3f(-15,0,-100);
                glVertex3f(-200,0,-100);
                glVertex3f(-15,0,-200);
                glVertex3f(200,0,-200);
                glVertex3f(200,0,-20);
                glVertex3f(-15,0,-20);
                glVertex3f(25,0,-20);
                glVertex3f(200,0,-20);
                glVertex3f(200,0,20);
                glVertex3f(25,0,20);

                glVertex3f(-15,0,20);
                glVertex3f(30,0,20);
                glVertex3f(30,0,-20);
                glVertex3f(-15,0,-20);
                glEnd();*/



            draw_map();
            disp_roads();
            for(int i=-10;i<40;i+=10)
            {
                glPushMatrix();
                    glTranslatef(i,0,67);
                    trees();
                glPopMatrix();
            }
            for(int i=45;i<90;i+=10)
            {
                glPushMatrix();
                    glTranslatef(i,0,65);
                    trees();
                glPopMatrix();
                glPushMatrix();
                    glTranslatef(i,0,75);
                    trees();
                glPopMatrix();
            }
            /**
            for(int i=-10;i<35;i+=10)
            {
                glPushMatrix();
                    glTranslatef(i,0,25);
                    trees();
                glPopMatrix();
            }
            */


            glPushMatrix();
            glTranslatef(-38,0,90);
                draw_arch("Nets Boys Main Hostel");
            glPopMatrix();

            gate();

            glPushMatrix();
            glTranslatef(-70,0,80);
                block3a.disp_build("BLOCK 3",'r');
            glPopMatrix();

            glPushMatrix();
            glTranslatef(-85,0,25);
                block3.disp_build("BLOCK 3",'r');
            glPopMatrix();

            /**
            outer structure of canteen building
            */

            glPushMatrix();
            glTranslatef(-65,0,-74);
                canteen.disp_build("CANTEEN");
            glPopMatrix();
            cateen_inner();

            /**
            chairs and tables
            first column
            */

            glPushMatrix();
            glTranslatef(-20,0,-80);
                diningtable();
            glPopMatrix();

            glPushMatrix();
            glTranslatef(-30,0,-80);
                diningtable();
            glPopMatrix();

            glPushMatrix();
            glTranslatef(-40,0,-80);
                diningtable();
            glPopMatrix();

            glPushMatrix();
            glTranslatef(-50,0,-80);
                diningtable();
            glPopMatrix();

            glPushMatrix();
            glTranslatef(-60,0,-80);
                diningtable();
            glPopMatrix();


            /**
            second column
            */

            glPushMatrix();
            glTranslatef(-20,0,-85);
                diningtable();
            glPopMatrix();

            glPushMatrix();
            glTranslatef(-30,0,-85);
                diningtable();
            glPopMatrix();

            glPushMatrix();
            glTranslatef(-40,0,-85);
                diningtable();
            glPopMatrix();

            glPushMatrix();
            glTranslatef(-50,0,-85);
                diningtable();
            glPopMatrix();

            glPushMatrix();
            glTranslatef(-60,0,-85);
                diningtable();
            glPopMatrix();

            /**
            third column
            */

            glPushMatrix();
            glTranslatef(-20,0,-90);
                diningtable();
            glPopMatrix();

            glPushMatrix();
            glTranslatef(-30,0,-90);
                diningtable();
            glPopMatrix();

            glPushMatrix();
            glTranslatef(-40,0,-90);
                diningtable();
            glPopMatrix();

            glPushMatrix();
            glTranslatef(-50,0,-90);
                diningtable();
            glPopMatrix();

            glPushMatrix();
            glTranslatef(-52,3,-85);
            glRotated(90, 0, -1, 0);
                human->render();
            glPopMatrix();






            // block 4
            glPushMatrix();
            glTranslatef(-15,0,50);
                block4.disp_build("BLOCK 4",'l');
            glPopMatrix();

            glPushMatrix();
            glTranslatef(-25,0,100);
                watchmen.disp_build();
            glPopMatrix();

            glPushMatrix();
            glTranslatef(-25, 3, 100);
            glRotated(90, 0, 1, 0);
                human->render();
            glPopMatrix();


            glPushMatrix();
            glTranslatef(-30, 3, 100);
                human->render();
            glPopMatrix();

            //move_person();

            glPushMatrix();
            glTranslatef(50,0,50);
                block2.disp_build("BLOCK 5");
            glPopMatrix();
            glPushMatrix();
            glTranslatef(50,0,0);
                block1.disp_build("BLOCK 2",'l');
            glPopMatrix();

            glPushMatrix();
            glTranslatef(-2,0,-20);
                block1.disp_build("BLOCK 1",'l');
            glPopMatrix();


            /**
            glPushMatrix();
            glTranslatef(-30, 4, 100);
            glRotated(90, -1, 0, 0);
                draw_tri_board();
            glPopMatrix();
            */

            glPushMatrix();
            glTranslatef(-30, 2, 100);
            glRotated(0, 1, 0, 10);
                draw_msg_board("watchmen I'm shaun, i have to get inside so open the gate.");
            glPopMatrix();


            glPushMatrix();
            glTranslatef(-25, 2, 100);
            glRotated(0, 1, 0, 0);
                draw_msg_board("Sure let me see your iD card");
            glPopMatrix();

            glPushMatrix();
            glTranslatef(-57,7,-31.5);
            glRotated(90, 0, 1, 0);
                draw_msg_board("Vishnu is the second god in the Hindu triumvirate (orTrimurti).");
            glPopMatrix();


            glPushMatrix();
            glTranslatef(-69,3,-28);

                temp.disp_temple();
            glPopMatrix();


            glPushMatrix();
    glTranslatef(-15,0,140);
        fball.ground_disp();
    glPopMatrix();



            glutSwapBuffers();
            glFlush();


}
//init
void myinit()
{
    glClearColor(0.0, 0.0, 1.0, 0.0);
	glColor3f(1.0f, 1.0f, 1.0f);
	glPointSize(0.0);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0.0, 1200.0, 0.0, 800.0);
	human = new Human();
	Cylinder = gluNewQuadric();
	//to set automatic normals
    gluQuadricNormals(Cylinder, GLU_SMOOTH);
    Disk = gluNewQuadric();
}


void pressKey(unsigned char key, int xx, int yy)
{
    switch (key) {
        case 13: if(key==13)
        {
            glutDisplayFunc(display);
        }
        glutPostRedisplay();
        break;
        case 'w' : deltaMove = 1.0;glutIdleFunc(update); break;
        case 's' : deltaMove = -1.0;glutIdleFunc(update); break;
        case 'u' : vertmove=1;glutIdleFunc(update);break;
        case 'j' : vertmove=-1;glutIdleFunc(update);break;
    }

}

void releaseKey(unsigned char key, int x, int y)
{
    switch (key) {
        case 'w' : deltaMove = 0.0; glutIdleFunc(NULL);break;
        case 's' : deltaMove = 0.0; glutIdleFunc(NULL);break;
        case 'u' : vertmove=0;glutIdleFunc(NULL);break;
        case 'j' : vertmove=0;glutIdleFunc(NULL);break;

    }
}

void mouseMove(int x, int y)
{
    if (isDragging) { // only when dragging
        // update the change in angle
        deltaAngle = (x - xDragStart) * -0.005;
        // camera's direction is set to angle + deltaAngle
        lx = sin(angle + deltaAngle);
        lz = -cos(angle + deltaAngle);
        glutIdleFunc(update);
    }
}

void mouseButton(int button, int state, int x, int y)
{

    if (button == GLUT_LEFT_BUTTON) {
        if (state == GLUT_DOWN) {
            isDragging = 1;
            xDragStart = x;
        }
    }
    else  { /* (state = GLUT_UP) */
        angle += deltaAngle; // update camera turning angle
        isDragging = 0; // no longer dragging
        glutIdleFunc(NULL);

    }

}

void move_menu(int m)
{
    switch(m)
    {
    case 1:
        if(mvo == 0)
            mvo=1;
        else
            mvo=0;
        break;
    }
}

void gate_menu(int m)
{
    switch(m)
    {
    case 1:
        if(mgo == 0)
            mgo=1;
        else
            mgo=0;
        break;
    }
}

void main_menu(int m)
{
    switch(m)
    {
        case 1:
            exit(0);
    }
}
void menu()
{

    int sub_menu1=glutCreateMenu(gate_menu);
    glutAddMenuEntry("main gate",1);
    glutCreateMenu(main_menu);
    int sub_menu2 = glutCreateMenu(move_menu);
    glutAddMenuEntry("Move Student", 2);
    glutCreateMenu(main_menu);
    glutAddMenuEntry("quit",1);
    glutAddSubMenu("open/close door",sub_menu1);
    glutAddSubMenu("Walk/Stop Student", sub_menu2);
    glutAttachMenu(GLUT_RIGHT_BUTTON);
}


int main(int argc,char**argv)
{   glutInit(&argc,argv);
     glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowSize(1500,1500);
    glutInitWindowPosition(0,0);
    glutCreateWindow("Hostel Life Simulation");

    glutDisplayFunc(frontscreen);
    //glutReshapeFunc(reshape);
    glutIgnoreKeyRepeat(1);
    //glutKeyboardFunc(myKeyboardFunc);
    glutKeyboardFunc(pressKey);

    glutKeyboardUpFunc(releaseKey);

    glutMouseFunc(mouseButton); // process mouse button push/release
    glutMotionFunc(mouseMove);// process mouse dragging motion
    menu();
    myinit();
    glutFullScreen();
    glEnable(GL_DEPTH_TEST);
    glutMainLoop();
}

