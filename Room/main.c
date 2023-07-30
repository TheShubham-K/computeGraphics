
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#include<GL/glut.h>
#include<math.h>
#include<stdio.h>
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#include<GL/glut.h>
#include<math.h>
#include<stdio.h>
#include <stdbool.h>
#include<string.h>
#define PI 3.1416

void display1();
void ship();

// limbs length
float upper_middle_leg_length = 4.0f;
float lower_leg_length = 1.0f;
float upper_middle_arm_length = 3.0f;
float lower_arm_length = 1.0f;

// general angles
float upper_torso_angle = 0.0f;
float lower_torso_angle = 0.0f;
float head_angle = 0.0f;

// right side angles
float r_shoulder_angle = 0.0f;
float r_elbow_angle = 0.0f;
float r_wrist_angle = 0.0f;
float r_hip_angle = 0.0f;
float r_knee_angle = 0.0f;
float r_ankle_angle = 0.0f;

// left side angles
float l_shoulder_angle = 0.0f;
float l_elbow_angle = 0.0f;
float l_wrist_angle = 0.0f;
float l_hip_angle = 0.0f;
float l_knee_angle = 0.0f;
float l_ankle_angle = 0.0f;

// limb selection
char limb;

void modify_world(float x, float y, float direction, float angle) {
	// move the world to the right position and rotate it
	glTranslatef(direction*x, y, 0.0f);
	glRotatef(angle, 0.0f, 0.0f, 1.0f);
}

void draw_junction(float x, float y) {  // draw body part's intersection point
	glColor3f(1.0f, 0.0f, 0.0f);
    glBegin(GL_POINTS);
        glVertex2f(x, y);
    glEnd();
	glColor3f(0.0f, 1.0f, 0.0f);
}

void color_white_if(char right_limb, char left_limb, int direction) {
	// set color to white if the limb got selected. 'limb' is the limb selected by the user (a char '1'-'6')
	// direction=1 := right limb  |  direction=2 := left limb
	if ((limb == right_limb && direction == 1) || (limb == left_limb && direction == -1))
		glColor3f(1.0f, 1.0f, 1.0f);
}

void draw_limb(int direction, float angle, float world_x, float world_y,
			   float right_limb, float left_limb, float limb_x, float limb_y) {
	modify_world(world_x, world_y, direction, angle);
	color_white_if(right_limb, left_limb, direction);

	glBegin(GL_LINES);
		glVertex2f(0.0f, 0.0f);
		glVertex2f(direction*limb_x, limb_y);
	glEnd();

	glColor3f(0.0f, 1.0f, 0.0f);
}

void draw_middle_limb(int direction, float angle, float world_x, float world_y,
					  float right_limb, float left_limb, float limb_x, float limb_y) {
	draw_limb(direction, angle, world_x, world_y, right_limb, left_limb, limb_x, limb_y);
	draw_junction(direction*limb_x, limb_y);
}

void draw_upper_torso() {
	draw_limb(1, upper_torso_angle, 0.0f, 0.0f, '6', '6', 0.0f, 3.0f);
	glBegin(GL_LINE_STRIP);
		glVertex2f(0.0f, 3.0f);
		glVertex2f(3.0f, 3.0f);
		glVertex2f(-3.0f, 3.0f);
	glEnd();
	draw_junction(3.0f, 3.0f);
	draw_junction(-3.0f, 3.0f);
	draw_junction(0.0f, 0.0f);
	draw_junction(0.0f, 3.0f);
}


// ARM FUNCTIONS
void draw_elbow(int direction, float angle)
{ draw_middle_limb(direction, angle, 3.0f, 3.0f, '4', '3', upper_middle_arm_length, 0.0f); }

void draw_wrist(int direction, float angle)
{ draw_middle_limb(direction, angle, upper_middle_arm_length, 0.0f, '4', '3', upper_middle_arm_length, 0.0f); }

void draw_hand(int direction, float angle)
{ draw_limb(direction, angle, upper_middle_arm_length, 0.0f, '4', '3', 0.0f, lower_arm_length); }

void draw_arm(int direction, float shoulder_angle, float elbow_angle, float wrist_angle) {
	glPushMatrix();
		glPushMatrix();
			draw_elbow(direction, shoulder_angle);
			glPushMatrix();
				draw_wrist(direction, elbow_angle);
				draw_hand(direction, wrist_angle);
			glPopMatrix();
		glPopMatrix();
	glPopMatrix();
}

void draw_right_arm() {	draw_arm(1, r_shoulder_angle, r_elbow_angle, r_wrist_angle); }
void draw_left_arm() { draw_arm(-1, l_shoulder_angle, l_elbow_angle, l_wrist_angle); }


// LEG FUNCTIONS
void draw_lower_torso() {
	glBegin(GL_LINES);
		glVertex2f(0.0f, 0.0f);
		glVertex2f(0.0f, -3.0f);
		glVertex2f(2.0f, -3.0f);
		glVertex2f(-2.0f, -3.0f);
	glEnd();

	draw_junction(2.0f, -3.0f);
	draw_junction(-2.0f, -3.0f);
}

void draw_knee(int direction, float angle)
{ draw_middle_limb(direction, angle, 2.0f, -3.0f, '2', '1', 0.0f, -upper_middle_leg_length); }

void draw_ankle(int direction, float angle)
{ draw_middle_limb(direction, angle, 0.0f, -upper_middle_leg_length, '2', '1', 0.0f, -upper_middle_leg_length); }

void draw_foot(int direction, float angle)
{ draw_limb(direction, angle, 0.0f, -upper_middle_leg_length, '2', '1', lower_leg_length, 0.0f); }

void draw_leg(int direction, float hip_angle, float knee_angle, float ankle_angle) {
	glPushMatrix();
		glPushMatrix();
			draw_knee(direction, hip_angle);
			glPushMatrix();
				draw_ankle(direction, knee_angle);
				draw_foot(direction, ankle_angle);
			glPopMatrix();
		glPopMatrix();
	glPopMatrix();
}

void draw_right_leg() { draw_leg(1, r_hip_angle, r_knee_angle, r_ankle_angle); }
void draw_left_leg() { draw_leg(-1, l_hip_angle, l_knee_angle, l_ankle_angle); }


// HEAD FUNCTION
void draw_head() {
	glColor3f(0.0f, 1.0f, 0.0f);
	color_white_if('5', '5', 1);
	modify_world(0.0f, 3.0f, 1, head_angle);

	// draw neck
	glBegin(GL_LINES);
		glVertex2f(0.0f, 0.0f);
		glVertex2f(0.0f, 1.0f);
	glEnd();

	glTranslatef(0.0f, 3.0f, 0.0f);

	// we draw a gluDisk with same inner and outer
	// radius' to get a circle for the head
	GLUquadric *quad = gluNewQuadric();
	gluDisk (quad, 2.0f, 2.1f, 100.0f, 1.0f);

	glColor3f(0.0f, 1.0f, 0.0f);
}

/**/
int i, j, k;
int slide=0;
bool doorOpen=true,flagCori=false,flagBed1=false,flagBed2=false,flagKitchen=false,flagToil=false; float fanAngle=0.0;
void *font1;
GLfloat ax=0,bx=0,cx=0,dx=0,str=500.0,mn=500.0;


// angle of rotation for the camera direction
float angle=0.0,angle1=0.0;
// actual vector representing the camera's direction
float lx=0.0f,lz=-1.0f,ly=0.0f;
// XZ position of the camera
float x=0.0f,z=5.0f;


int curX,curY;


GLfloat rotx=0,roty=0.5,rotz=3,camx=0,camy=0,camz=0/*,angle=0*/;
void mytimer(int n)
{
//printf("Timer running");
if(true)
    {
        printf("Fan should");
        fanAngle+=5;


        if(fanAngle>360.0)
                fanAngle=0;
        glutTimerFunc(10,mytimer,0);
    }
glutPostRedisplay();
}
void processSpecialKeys(int key, int xx, int yy)
{

    float fraction = 0.1f;
    printf("x,z %f %f\n",x,z);
    if(slide==2 || slide==3 || slide==4 || slide==10 ||slide==15){
        if(z>=2.35 ){
                z=2.35;
        }
        if(z<=0.8){
            z=0.8;
        }
        switch(key){
		case GLUT_KEY_UP :
			z -= 0.1;
			break;
		case GLUT_KEY_DOWN :
			z += 0.1;
			break;

        }
        glutPostRedisplay();
        return;
    }
    switch(key){
            case GLUT_KEY_LEFT :
			//angle -= 0.01f;
			x -= cos(angle)*0.1;
			z -=sin(angle)*0.1;
			//lz = -cos(angle);
			break;
		case GLUT_KEY_RIGHT :
			x+=cos(angle)*0.1;
			z +=sin(angle)*0.1;
			break;
		case GLUT_KEY_UP :
			x += lx * fraction;
			z += lz * fraction;
			break;
		case GLUT_KEY_DOWN :
			x -= lx * fraction;
			z -= lz * fraction;
			break;

        }

    glutPostRedisplay();
}


void mousemain(int btn,int st,int x,int y)
{
    printf("x:%d y:%d\n",x,y);

 /*   if(slide==0&&btn==GLUT_LEFT_BUTTON&& x>669&&x<729 && y>530&&y<550)// insert coordinates for house door knob
        {
           slide=5;
            rotx=0;roty=0.5;rotz=2;camx=0;camy=0.35;camz=0;
            glutTimerFunc(200,mytimer,0);
            glutPostRedisplay();
        }
    if(slide==5&&btn==GLUT_LEFT_BUTTON&& x>552&&x<635 && y>530&&y<551)// insert coordinates for house door knob
        {
           slide=1;
            rotx=0;roty=0.5;rotz=2;camx=0;camy=0.35;camz=0;
            glutTimerFunc(200,mytimer,0);
            glutPostRedisplay();
        }
    if(slide==1&&btn==GLUT_LEFT_BUTTON&& x>242&&x<280 && y>510&&y<520)// insert coordinates for begin journey
        {
            slide=2;
            x=0;z=1.8;camx=0;camy=0.35;camz=0;
            lx=0.0f,lz=-1.0f,ly=0.0f;
            glutPostRedisplay();
        }
    if(slide==2&&btn==GLUT_LEFT_BUTTON&& x>144&&x<174 && y>466 &&y<500)//0.01,0.01,+.1
        {
            slide=3;
            rotx=0;roty=0.65;rotz=2.5;camx=0;camy=0.4;camz=0;
            glutTimerFunc(10,mytimer,0);
            glutPostRedisplay();
        }
     if(slide==2&&btn==GLUT_LEFT_BUTTON&& x>1093&&x<1125&& y>450 &&y<485)//-0.01,0.01,+.1
        {
            rotx=0;roty=0.65;rotz=2.5;camx=0;camy=0.4;camz=0;
            slide=4;
            glutTimerFunc(10,mytimer,0);
            glutPostRedisplay();
        }*/
    if(btn==GLUT_RIGHT_BUTTON&&st==GLUT_DOWN)
    {

        switch(slide)
        {
            case 3: {
            slide=2;
            x=0;z=1.8;camx=0;camy=0.35;camz=0;
            lx=0.0f,lz=-1.0f,ly=0.0f;break;
            glutPostRedisplay();

        }break;
            case 4: {
            slide=3;
            x=0;z=1.8;camx=0;camy=0.35;camz=0;
            lx=0.0f,lz=-1.0f,ly=0.0f;break;
            flagCori=false;
            glutPostRedisplay();

        }break;
            case 2: {
           slide=1;
            x=0;z=1.8;camx=0;camy=0.35;camz=0;
            lx=0.0f,lz=-1.0f,ly=0.0f;
            glutTimerFunc(200,mytimer,0);
            glutPostRedisplay();

        }break;
            case 10: {
            slide=4;
            x=0;z=1.8;camx=0;camy=0.35;camz=0;
            lx=0.0f,lz=-1.0f,ly=0.0f;break;
            glutPostRedisplay();

        }break;
        case 15: {
            slide=10;
            x=0;z=1.8;camx=0;camy=0.35;camz=0;
            lx=0.0f,lz=-1.0f,ly=0.0f;break;
            glutPostRedisplay();

        }break;

            case 1: slide=5;break;
            case 5: slide=0;break;
            case 0: exit(0);
        }
    }
}
void keyboard1(unsigned char c,int x,int y){


        if(c=='2'){
            slide=3;
            flagBed1=true;
        }
        else if(c=='3'){
            slide=4;
            flagBed2=true;
        }
        else if(c=='4'){
            slide=10;
            flagKitchen=true;
        }


       else if(c=='5'){
            slide=15;
            flagToil=true;

        }
      else if(c=='1'){
            slide=2;

            flagCori=true;
            glutPostRedisplay();

    }
}



void drawstring(float x,float y,float z,char *msg)
{

    glRasterPos3f(x,y,z);
    for(int i=0;i<strlen(msg);i++)
    {

        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24,msg[i]);
    }
}
void door(){
        glPushMatrix();
        glColor3ub(104, 40, 0);
        glTranslatef(0,0,0);
        glScalef(0.5,2,0.05);
        glutSolidCube(1);
    glPopMatrix();
    glPushMatrix();
        glColor3ub(137, 52, 0);
        glTranslatef(0,0.5,0);
        glScalef(0.4,0.7,0.08);
        glutSolidCube(1);
    glPopMatrix();
    glPushMatrix();
        glColor3ub(137, 52, 0);
        glTranslatef(0,-0.5,0);
        glScalef(0.4,0.7,0.08);
        glutSolidCube(1);
    glPopMatrix();
    glPushMatrix();
        glColor3ub(255, 250, 216);
        glTranslatef(0.15,0,0);
        glutSolidSphere(0.05,32,32);
    glPopMatrix();
}
void door2(){
        glPushMatrix();
        glColor3ub(65, 92, 109);
        glTranslatef(0,0,0);
        glScalef(0.5,2,0.05);
        glutSolidCube(1);
    glPopMatrix();
    glPushMatrix();
        glColor3ub(26, 38, 45);
        glTranslatef(0,0.5,0);
        glScalef(0.4,0.7,0.08);
        glutSolidCube(1);
    glPopMatrix();
    glPushMatrix();
        glColor3ub(26, 38, 45);
        glTranslatef(0,-0.5,0);
        glScalef(0.4,0.7,0.08);
        glutSolidCube(1);
    glPopMatrix();
    glPushMatrix();
        glColor3ub(26, 38, 45);
        glTranslatef(0.15,0,0);
        glutSolidSphere(0.05,32,32);
    glPopMatrix();
}
void lamp(){
    glPushMatrix();
        glScalef(0.6,1,0.6);

        /*-----------------Lamp pole-----------*/
        glPushMatrix();
            glColor3f(1,0,0);
            glTranslatef(0,0.4,0);
            glScalef(0.2,0.3,0.4);
            glutSolidSphere(1,32,32);
        glPopMatrix();

        /*-----------------Lamp shade-----------*/
        glPushMatrix();
            GLUquadricObj * qobj = gluNewQuadric();
            glTranslatef(0,1.3,0);
            glScalef(2,2,2);
            glColor3ub(253, 255, 178);
            glRotatef(90, 1.0f, 0.0f, 0.0f);
            gluCylinder(qobj, 0.15, 0.3, 0.35, 50, 50);
            gluDeleteQuadric(qobj);
        glPopMatrix();
    glPopMatrix();
}


void cabinet1(){
    glPushMatrix();
        glScalef(0.6,1,0.6);
            /*----------------Cabinet Top------------------------*/
        glPushMatrix();
            glColor3ub(28, 7, 0);
            glTranslatef(0,0.1,0);
            glScalef(1,0.05,1);
            glutSolidCube(1);
        glPopMatrix();
        glPushMatrix();
            glTranslatef(0,0.25,0);
            /*----------------Cabinet part1 ------------------------*/
            glPushMatrix();
                glColor3ub(58, 15, 0);
                glTranslatef(0,-0.3,0);
                glScalef(0.8,0.3,0.8);
                glutSolidCube(1);
            glPopMatrix();

            /*----------------Cabinet part2 ------------------------*/
            glPushMatrix();
                glColor3ub(81, 20, 0);
                glTranslatef(0,-0.6,0);
                glScalef(0.8,0.3,0.8);
                glutSolidCube(1);
            glPopMatrix();

            /*----------------Cabinet part3 ------------------------*/
            glPushMatrix();
                glColor3ub(58, 15, 0);
                glTranslatef(0,-0.9,0);
                glScalef(0.8,0.3,0.8);
                glutSolidCube(1);
            glPopMatrix();

            /*----------------Cabinet handle3 ------------------------*/
            glPushMatrix();
                glColor3f(1,1,0);
                glTranslatef(0,-0.9,.5);
                glScalef(0.2,0.025,0.05);
                glutSolidCube(1);
            glPopMatrix();

            /*----------------Cabinet handle1 ------------------------*/
            glPushMatrix();
                glColor3f(1,1,0);
                glTranslatef(0,-0.6,.5);
                glScalef(0.2,0.025,0.05);
                glutSolidCube(1);
            glPopMatrix();

            /*----------------Cabinet handle2 ------------------------*/
            glPushMatrix();
                glColor3f(1,1,0);
                glTranslatef(0,-0.3,.5);
                glScalef(0.2,0.025,0.05);
                glutSolidCube(1);
            glPopMatrix();
        glPopMatrix();
    glPopMatrix();
}
void bed1(){
    glPushMatrix();
        glScalef(0.75,1,0.75);
        /*----------------Mattress------------------------*/
        glPushMatrix();//Mattress
            glColor3ub(221, 221, 221);
            glTranslatef(0,0.1,0);
            glScalef(1,0.15,1.25);
            glutSolidCube(1);
        glPopMatrix();
    /*----------------Bed Cot------------------------*/
        glPushMatrix();//Bed Cot
            glColor3ub(147, 39, 0);
            glScalef(1.1,0.2,1.35);
            glutSolidCube(1);
        glPopMatrix();
    /*----------------Bed headrest------------------------*/
        glPushMatrix();//Bed headrest
            glColor3ub(79, 21, 0);
            glTranslatef(0,-0.1,-0.65);
            glScalef(1,0.4,0.1);
            glTranslatef(0,1,-0.1);
            glRotatef(90,1,0,0);
            glScalef(1.1,0.2,1.35);
            glutSolidCube(1);
        glPopMatrix();
    /*----------------Pillow------------------------*/
        glPushMatrix();//Pillow1
            GLUquadricObj * qobj = gluNewQuadric();
            glTranslatef(0.1,0.2,-0.4);
            glScalef(0.75,0.75,0.75);
           glColor3ub(255, 255, 255);
            glRotatef(90, 0.0f, 0.0f, 1.0f);
            glRotatef(90, 1.0f, 0.0f, 0.0f);
            gluCylinder(qobj, 0.1, 0.1, 0.35, 50, 50);
            gluDeleteQuadric(qobj);
        glPopMatrix();

        glPushMatrix();//Pillow2
            GLUquadricObj * qobj2 = gluNewQuadric();
            glTranslatef(-0.4,0.2,-0.4);
            glScalef(0.75,0.75,0.75);
            glColor3ub(255, 255, 255);
            glRotatef(90, 0.0f, 0.0f, 1.0f);
            glRotatef(90, 1.0f, 0.0f, 0.0f);
            gluCylinder(qobj2, 0.1, 0.1, 0.35, 50, 50);
            gluDeleteQuadric(qobj2);
        glPopMatrix();
    glPopMatrix();
}
void wardrobe(){
    glPushMatrix();
        /*------------Left door----------------*/
        glPushMatrix();
            glScalef(1,1,1);
            glPushMatrix();
                    glColor3ub(209, 177, 131);
                    glTranslatef(0,0,0.025);
                    glScalef(0.3,1.5,0.25);
                    glutSolidCube(1);
            glPopMatrix();
        glPopMatrix();

        /*------------Right door----------------*/
        glPushMatrix();
            glScalef(1,1,1);
            glPushMatrix();
                    glColor3ub(209, 177, 131);
                    glTranslatef(0.305,0,0.025);
                    glScalef(0.3,1.5,0.25);
                    glutSolidCube(1);
            glPopMatrix();
        glPopMatrix();

        /*------------Frame----------------*/
        glPushMatrix();
            glScalef(1,1,1);
            glPushMatrix();
                    glColor3ub(237, 205, 158);
                    glTranslatef(0.15,00,0);
                    glScalef(0.70,1.6,0.26);
                    glutSolidCube(1);
            glPopMatrix();
        glPopMatrix();
        /*------------Knob1-----------------*/
        glPushMatrix();
            glScalef(0.05,0.05,0.05);
            glTranslatef(1,1,2.5);
            glColor3ub(135, 135, 135);
            glutSolidSphere(1,32,32);
        glPopMatrix();
        /*------------Knob2-----------------*/
        glPushMatrix();
            glScalef(0.05,0.05,0.05);
            glTranslatef(5,1,2.5);
            glColor3ub(135, 135, 135);
            glutSolidSphere(1,32,32);
        glPopMatrix();
    glPopMatrix();
}
void photoFrame1(){
    /*-------------Frame-------------*/
    glPushMatrix();
            glScalef(1.2,0.75,0.1);
            glPushMatrix();
                    glColor3ub(104, 69, 37);
                    glutSolidCube(1);
            glPopMatrix();
    glPopMatrix();

    /*-------------Picture-------------*/
    glPushMatrix();
            glScalef(1,0.6,0.1);
            glTranslatef(0,0,0.05);
            glPushMatrix();
                    glColor3ub(108, 196, 37);
                    glutSolidCube(1);
            glPopMatrix();
    glPopMatrix();
}
void ac(){
    /*--------------------------AC---------------------------*/


    glPushMatrix();
        glColor3f(1,1,1);
        glTranslatef(0.647,0.852,0.2495);
        glRotatef(-30,0,0,1);
        glRotatef(330,0,0,1);
        glRotatef(90,0,1,0);
        glRotatef(90,1,0,0);
        glScalef(0.001,0.05,0.5);
        glutSolidCube(1);
    glPopMatrix();

    glPushMatrix();
        glColor3f(1,1,1);
        glTranslatef(0.43,0.725,0);
        glRotatef(30,0,0,1);
        glScalef(0.001,0.05,0.5);
        glutSolidCube(1);
    glPopMatrix();




    glPushMatrix();
        glColor3f(1,1,1);
        glTranslatef(0.45,0.8,0);
        glScalef(0.06,0.1,0.5);
        glutSolidCube(1);
    glPopMatrix();


    glPushMatrix();
        glColor3f(1,1,1);
        glTranslatef(0.43,0.725,0);
        glRotatef(30,0,0,1);
        glScalef(0.001,0.05,0.5);
        glutSolidCube(1);
    glPopMatrix();

   glPushMatrix();
        glColor3f(1,1,1);
        glTranslatef(0.46,0.725,0);
        glScalef(0.025,0.04,0.5);
        glutSolidCube(1);
    glPopMatrix();
/*----------------------------AC end-----------------------------*/
}
void photoFrame2(){
    /*-------------Frame-------------*/


    glPushMatrix();
            glScalef(1.2,0.75,0.1);
            glPushMatrix();
                    glColor3ub(104, 69, 37);
                    glutSolidCube(1);
            glPopMatrix();
    glPopMatrix();

    /*-------------Picture-------------*/
    glPushMatrix();


            glScalef(1,0.6,0.1);
            glTranslatef(0,0,0.05);
            glPushMatrix();

                    glColor3ub(216, 108, 207);
                    glutSolidCube(1);
            glPopMatrix();
    glPopMatrix();

}
void fridge(){
    glPushMatrix();
        glScalef(1,1,1);
        /*-----------Fridge------------*/
        glPushMatrix();
                glScalef(1,1,1);
                glPushMatrix();
                        glColor3ub(183, 183, 183);
                        glTranslatef(0,0,0.025);
                        glScalef(0.3,0.9,0.25);
                        glutSolidCube(1);
                glPopMatrix();
        glPopMatrix();

        /*-----------Freezer------------*/
        glPushMatrix();
                glScalef(1,1,1);
                glPushMatrix();
                        glColor3ub(183, 183, 183);
                        glTranslatef(0,0.6525,0.025);
                        glScalef(0.3,0.4,0.25);
                        glutSolidCube(1);
                glPopMatrix();
        glPopMatrix();
        /*-----------Freezer handle------------*/
        glPushMatrix();
                glScalef(1,1,1);
                glPushMatrix();
                        glColor3ub(119, 119, 119);
                        glTranslatef(-0.125,0.6025,0.04);
                        glScalef(0.025,0.2,0.25);
                        glutSolidCube(1);
                glPopMatrix();
        glPopMatrix();

        /*-----------Fridge handle------------*/
        glPushMatrix();
                glScalef(1,1,1);
                glPushMatrix();
                        glColor3ub(119, 119, 119);
                        glTranslatef(-0.125,0.3025,0.04);
                        glScalef(0.025,0.25,0.25);
                        glutSolidCube(1);
                glPopMatrix();
        glPopMatrix();
    glPopMatrix();
}
void gasCylinder(){
    glPushMatrix();
        glScalef(1,1,1);
            /*-------------Center cylinder-----------*/
            glPushMatrix();
                    GLUquadricObj * qobj2 = gluNewQuadric();
            //        glTranslatef(-0.4,0.2,-0.4);
                    glScalef(1,1,1);
                    glColor3ub(150, 0, 0);
               //     glRotatef(90, 0.0f, 0.0f, 1.0f);
                    glRotatef(90, 1.0f, 0.0f, 0.0f);
                    gluCylinder(qobj2, 0.1, 0.1, 0.35, 50, 50);
                    gluDeleteQuadric(qobj2);
            glPopMatrix();
            /*-------------Top bottom sphere-----------*/
            glPushMatrix();
                glColor3ub(150, 0, 0);
                glutSolidSphere(0.1,32,32);
            glPopMatrix();
            glPushMatrix();
                glColor3ub(150, 0, 0);
                glTranslatef(0,-0.35,0);
                glutSolidSphere(0.1,32,32);
            glPopMatrix();
             /*-------------Bottom cylinder-----------*/
            glPushMatrix();
                    GLUquadricObj * qobj1 = gluNewQuadric();
                    glTranslatef(0,-0.26,0);
                    glScalef(0.5,0.5,0.5);
                    glColor3ub(150, 0, 0);
               //     glRotatef(90, 0.0f, 0.0f, 1.0f);
                    glRotatef(90, 1.0f, 0.0f, 0.0f);
                    gluCylinder(qobj1, 0.15, 0.15, 0.35, 50, 50);
                    gluDeleteQuadric(qobj1);
            glPopMatrix();
            /*-------------Top cylinder-----------*/
            glPushMatrix();
                    GLUquadricObj * qobj3 = gluNewQuadric();
                    glTranslatef(0,0.2,0);
                    glScalef(0.5,0.5,0.5);
                    glColor3ub(150, 0, 0);
               //     glRotatef(90, 0.0f, 0.0f, 1.0f);
                    glRotatef(90, 1.0f, 0.0f, 0.0f);
                    gluCylinder(qobj3, 0.15, 0.15, 0.35, 50, 50);
                    gluDeleteQuadric(qobj3);
            glPopMatrix();
    glPopMatrix();
}
void sofa(){
    glPushMatrix();
    glScalef(1,1,1);
    /*--------------Sofa ----------*/
        glPushMatrix();
                glColor3ub(0, 112, 80);
  //                      glTranslatef(0,0,0.025);
                glScalef(1,0.275,0.3);
                glutSolidCube(1);
        glPopMatrix();
        /*---------Sofa legs-------------*/
        glPushMatrix();
                glColor3ub(63, 27, 0);
                glTranslatef(-0.46,-0.2,0);
                glScalef(0.025,0.25,0.28);
                glutSolidCube(1);
        glPopMatrix();
        glPushMatrix();
                glColor3ub(63, 27, 0);
                glTranslatef(0.46,-0.2,0);
                glScalef(0.025,0.25,0.28);
                glutSolidCube(1);
        glPopMatrix();
        /*---------Sofa pillows1-------------*/
        glPushMatrix();
        glColor3ub(0, 49, 175);
        glScalef(0.7,0.7,0.5);
        glTranslatef(-0.6,0.275,-2.8);
        glRotatef(90,1,0,0);
        glTranslatef(0,3,0);
            glPushMatrix();
                    GLUquadricObj * qobj2 = gluNewQuadric();
            //        glTranslatef(-0.4,0.2,-0.4);
                    glScalef(1,1,1);
               //     glRotatef(90, 0.0f, 0.0f, 1.0f);
                    glRotatef(90, 1.0f, 0.0f, 0.0f);
                    gluCylinder(qobj2, 0.1, 0.1, 0.35, 50, 50);
                    gluDeleteQuadric(qobj2);
            glPopMatrix();
            /*-------------Top bottom sphere-----------*/
            glPushMatrix();
                glutSolidSphere(0.1,32,32);
            glPopMatrix();
            glPushMatrix();
                glTranslatef(0,-0.35,0);
                glutSolidSphere(0.1,32,32);
            glPopMatrix();
        glPopMatrix();
        /*---------Sofa pillows2-------------*/

        glPushMatrix();
        glColor3ub(0, 49, 175);
        glScalef(0.7,0.7,0.5);
        glTranslatef(0.6,0.275,-2.8);
        glRotatef(90,1,0,0);
        glTranslatef(0,3,0);
            glPushMatrix();
                    GLUquadricObj * qobj3 = gluNewQuadric();
            //        glTranslatef(-0.4,0.2,-0.4);
                    glScalef(1,1,1);
               //     glRotatef(90, 0.0f, 0.0f, 1.0f);
                    glRotatef(90, 1.0f, 0.0f, 0.0f);
                    gluCylinder(qobj3, 0.1, 0.1, 0.35, 50, 50);
                    gluDeleteQuadric(qobj3);
            glPopMatrix();
            /*-------------Top bottom sphere-----------*/
            glPushMatrix();
                glutSolidSphere(0.1,32,32);
            glPopMatrix();
            glPushMatrix();
                glTranslatef(0,-0.35,0);
                glutSolidSphere(0.1,32,32);
            glPopMatrix();
        glPopMatrix();
    glPopMatrix();
}
void bookshelf(){
        /*-------Book shelf--------------*/
    glPushMatrix();
                glColor3ub(170, 127, 83);
                glScalef(0.05,2,0.3);
                glutSolidCube(1);
    glPopMatrix();
    glPushMatrix();
                glColor3ub(170, 127, 83);
                glTranslatef(0.97,0.5,0);
                glRotatef(90,0,0,1);
                glScalef(0.05,2,0.3);
                glutSolidCube(1);
    glPopMatrix();
    glPushMatrix();
                glColor3ub(170, 127, 83);
                glTranslatef(0.97,0.0,0);
                glRotatef(90,0,0,1);
                glScalef(0.05,2,0.3);
                glutSolidCube(1);
    glPopMatrix();
    glPushMatrix();
                glColor3ub(170, 127, 83);
                glTranslatef(0.97,-0.5,0);
                glRotatef(90,0,0,1);
                glScalef(0.05,2,0.3);
                glutSolidCube(1);
    glPopMatrix();
    glPushMatrix();
                glColor3ub(170, 127, 83);
                glTranslatef(0.97,1,0);
                glRotatef(90,0,0,1);
                glScalef(0.05,2,0.3);
                glutSolidCube(1);
    glPopMatrix();
    glPushMatrix();
        glTranslatef(1.95,0,0);
        glRotatef(180,0,0,1);
        glPushMatrix();
                glColor3ub(170, 127, 83);
  //                      glTranslatef(0,0,0.025);
                glScalef(0.05,2,0.3);
                glutSolidCube(1);
        glPopMatrix();
        glPushMatrix();
                glColor3ub(170, 127, 83);
                glTranslatef(0.97,1,0);
                glRotatef(90,0,0,1);
                glScalef(0.05,2,0.3);
                glutSolidCube(1);
        glPopMatrix();
    glPopMatrix();
    /*--------------Books----------------*/
    /*------------2nd rack----------------*/
    glPushMatrix();//Book red 2nd rack
        glTranslatef(0.3,0,0);
        glRotatef(-10,0,0,1);
        glPushMatrix();
                glColor3ub(255, 0, 0);
                glTranslatef(1.5,0.45,0.025);
                glScalef(0.05,0.4,0.2);
                glutSolidCube(1);
        glPopMatrix();
    glPopMatrix();
    glPushMatrix();//Book red 2nd rack
        glTranslatef(0.25,0.05,0);
        glRotatef(-10,0,0,1);
        glPushMatrix();
                glColor3ub(150, 0, 0);
                glTranslatef(1.5,0.45,0.025);
                glScalef(0.05,0.45,0.2);
                glutSolidCube(1);
        glPopMatrix();
    glPopMatrix();
    /*-----------Books 3rd rack----------*/
    glPushMatrix();//Book 3rd rach
        glTranslatef(-1.45,-0.73,0);
        glPushMatrix();
                glColor3ub(0, 0, 50);
                glTranslatef(1.5,0.45,0.025);
                glScalef(0.05,0.4,0.2);
                glutSolidCube(1);
        glPopMatrix();
    glPopMatrix();
    glPushMatrix();//Book 3rd rach
        glTranslatef(-1.39,-0.73,0);
        glPushMatrix();
                glColor3ub(0, 0, 130);
                glTranslatef(1.5,0.45,0.025);
                glScalef(0.08,0.45,0.2);
                glutSolidCube(1);
        glPopMatrix();
    glPopMatrix();
    glPushMatrix();//Book 3rd rach
        glTranslatef(-1.33,-0.73,0);
        glPushMatrix();
                glColor3ub(255, 0, 50);
                glTranslatef(1.5,0.45,0.025);
                glScalef(0.05,0.42,0.2);
                glutSolidCube(1);
        glPopMatrix();
    glPopMatrix();
    glPushMatrix();//Book 3rd rach
        glTranslatef(-1.28,-0.75,0);
        glPushMatrix();
                glColor3ub(255, 150, 0);
                glTranslatef(1.5,0.45,0.025);
                glScalef(0.05,0.35,0.2);
                glutSolidCube(1);
        glPopMatrix();
    glPopMatrix();
    glPushMatrix();//Book 3rd rach
        glTranslatef(-1.23,-0.75,0);
        glPushMatrix();
                glColor3ub(255, 200, 0);
                glTranslatef(1.5,0.45,0.025);
                glScalef(0.05,0.35,0.2);
                glutSolidCube(1);
        glPopMatrix();
    glPopMatrix();
    glPushMatrix();//Book 3rd rach
        glTranslatef(-1.18,-0.75,0);
        glPushMatrix();
                glColor3ub(150, 150, 0);
                glTranslatef(1.5,0.45,0.025);
                glScalef(0.05,0.35,0.2);
                glutSolidCube(1);
        glPopMatrix();
    glPopMatrix();
    glPushMatrix();//Book 3rd rach
        glTranslatef(-1.135,-0.79,0);
        glPushMatrix();
                glColor3ub(0, 144, 0);
                glTranslatef(1.5,0.45,0.025);
                glScalef(0.015,0.3,0.2);
                glutSolidCube(1);
        glPopMatrix();
    glPopMatrix();
    glPushMatrix();//Book 3rd rach
        glTranslatef(-1.150,-0.79,0);
        glPushMatrix();
                glColor3ub(200, 0, 0);
                glTranslatef(1.5,0.45,0.025);
                glScalef(0.015,0.3,0.2);
                glutSolidCube(1);
        glPopMatrix();
    glPopMatrix();
    glPushMatrix();//Book 3rd rach
        glTranslatef(-1.120,-0.79,0);
        glPushMatrix();
                glColor3ub(10, 123, 0);
                glTranslatef(1.5,0.45,0.025);
                glScalef(0.015,0.3,0.2);
                glutSolidCube(1);
        glPopMatrix();
    glPopMatrix();
    glPushMatrix();//Book 3rd rach
        glTranslatef(-1.105,-0.79,0);
        glPushMatrix();
                glColor3ub(0, 23, 56);
                glTranslatef(1.5,0.45,0.025);
                glScalef(0.015,0.3,0.2);
                glutSolidCube(1);
        glPopMatrix();
    glPopMatrix();
    glPushMatrix();//Book 3rd rach
        glTranslatef(-1.090,-0.79,0);
        glPushMatrix();
                glColor3ub(0, 144, 0);
                glTranslatef(1.5,0.45,0.025);
                glScalef(0.015,0.3,0.2);
                glutSolidCube(1);
        glPopMatrix();
    glPopMatrix();
    glPushMatrix();//Book 3rd rach
        glTranslatef(-1.075,-0.79,0);
        glPushMatrix();
                glColor3ub(255, 144, 0);
                glTranslatef(1.5,0.45,0.025);
                glScalef(0.015,0.3,0.2);
                glutSolidCube(1);
        glPopMatrix();
    glPopMatrix();
    glPushMatrix();//Book 3rd rach
        glTranslatef(-0.915,-0.975,0);
        glRotatef(10,0,0,1);
        glPushMatrix();
                glColor3ub(206, 205, 204);
                glTranslatef(1.5,0.45,0.025);
                glScalef(0.08,0.4,0.2);
                glutSolidCube(1);
        glPopMatrix();
    glPopMatrix();
    glPushMatrix();//Book 3rd rach
        glTranslatef(1.5,-1.95,0);
        glRotatef(90,0,0,1);
        glPushMatrix();
                glColor3ub(255, 0, 0);
                glTranslatef(1.5,0.45,0.025);
                glScalef(0.05,0.4,0.2);
                glutSolidCube(1);
        glPopMatrix();
    glPopMatrix();
    glPushMatrix();//Book 3rd rach
        glScalef(0.9,1,1);
        glTranslatef(1.6,-1.9,0);
        glRotatef(90,0,0,1);
        glPushMatrix();
                glColor3ub(10, 10, 10);
                glTranslatef(1.5,0.45,0.025);
                glScalef(0.05,0.4,0.2);
                glutSolidCube(1);
        glPopMatrix();
    glPopMatrix();
    /*-------------Cone---------------*/
    glPushMatrix();
        glColor3ub(59, 255, 10);
        glTranslatef(1.5,0.5,0.05);
        glScalef(0.2,0.2,0.2);
        glRotatef(-90,1,0,0);
        glutSolidCone(0.5,2,32,32);
    glPopMatrix();
    /*--------------Ball---------------*/
    glPushMatrix();
        glColor3ub(246, 255, 0);
        glTranslatef(1.1,0.6,0.05);
        glScalef(0.2,0.2,0.2);
        glutSolidSphere(0.5,32,32);
    glPopMatrix();
}
void table1(){
    /*--------------Table top------------------*/
    glPushMatrix();
                glColor3ub(15, 0, 99);
                glScalef(1.3,0.05,0.65);
                glutSolidCube(1);
    glPopMatrix();
    /*----------Table legs---------------------*/
    glPushMatrix();
        glTranslatef(0,-0.375,0);
        glPushMatrix();
                    glTranslatef(0.6,0,0.275);
                    glColor3ub(170, 127, 83);
                    glScalef(0.05,0.75,0.05);
                    glutSolidCube(1);
        glPopMatrix();
        glPushMatrix();
                    glTranslatef(0.6,0,-0.275);
                    glColor3ub(170, 127, 83);
                    glScalef(0.05,0.75,0.05);
                    glutSolidCube(1);
        glPopMatrix();
        glPushMatrix();
                    glTranslatef(-0.6,0,0.275);
                    glColor3ub(170, 127, 83);
                    glScalef(0.05,0.75,0.05);
                    glutSolidCube(1);
        glPopMatrix();
        glPushMatrix();
                    glTranslatef(-0.6,0,-0.275);
                    glColor3ub(170, 127, 83);
                    glScalef(0.05,0.75,0.05);
                    glutSolidCube(1);
        glPopMatrix();
    glPopMatrix();
}

void table2(){
    /*--------------Table top------------------*/
    glPushMatrix();
                glColor3ub(0, 61, 53);
                glScalef(1.3,0.05,0.65);
                glutSolidCube(1);
    glPopMatrix();
    /*----------Table legs---------------------*/
    glPushMatrix();
        glTranslatef(0,-0.375,0);
        glPushMatrix();
                    glTranslatef(0.6,0,0.275);
                    glColor3ub(151, 255, 147);
                    glScalef(0.05,0.75,0.05);
                    glutSolidCube(1);
        glPopMatrix();
        glPushMatrix();
                    glTranslatef(0.6,0,-0.275);
                    glColor3ub(151, 255, 147);
                    glScalef(0.05,0.75,0.05);
                    glutSolidCube(1);
        glPopMatrix();
        glPushMatrix();
                    glTranslatef(-0.6,0,0.275);
                    glColor3ub(151, 255, 147);
                    glScalef(0.05,0.75,0.05);
                    glutSolidCube(1);
        glPopMatrix();
        glPushMatrix();
                    glTranslatef(-0.6,0,-0.275);
                    glColor3ub(151, 255, 147);
                    glScalef(0.05,0.75,0.05);
                    glutSolidCube(1);
        glPopMatrix();
    glPopMatrix();
}


void bunkerBed(){
    /*----------Bunker sides----------*/
    glPushMatrix();
        glTranslatef(0,0,0);
        glScalef(0.5,1.05,0.025);
        glColor3ub(118, 0, 155);
        glutSolidCube(1);
    glPopMatrix();
    glPushMatrix();
        glTranslatef(0,0,1);
        glScalef(0.5,1.05,0.025);
        glColor3ub(82, 0, 150);
        glutSolidCube(1);
    glPopMatrix();
    /*---------Bunker bottom support--------*/
    glPushMatrix();
        glTranslatef(0,2.13,0);
        glScalef(1,8.5,0.75);
        glTranslatef(0,-0.3,0.675);
        glScalef(1,1,0.75);
        glRotatef(90,1,0,0);
        glScalef(0.5,1.75,0.025);
        glColor3ub(9, 0, 117);
        glutSolidCube(1);
    glPopMatrix();
    /*---------Bunker top support--------*/
    glPushMatrix();
        glScalef(1,1,0.75);
        glTranslatef(0,0.25,0.675);
        glScalef(1,1,0.75);
        glRotatef(90,1,0,0);
        glScalef(0.5,1.75,0.025);
        glColor3ub(0, 105, 117);
        glutSolidCube(1);
    glPopMatrix();
    /*---------Bunker top mattress--------*/
    glPushMatrix();
        glScalef(0.9,2.25,0.7);
        glTranslatef(0,0.12,0.7);
        glScalef(1,1,0.75);
        glRotatef(90,1,0,0);
        glScalef(0.5,1.75,0.025);
        glColor3ub(252, 227, 83);
        glutSolidCube(1);
    glPopMatrix();

    /*---------Bunker bottom mattress--------*/
    glPushMatrix();
        glScalef(0.9,2.25,0.7);
        glTranslatef(0,-0.1275,0.7);
        glScalef(1,1,0.75);
        glRotatef(90,1,0,0);
        glScalef(0.5,1.75,0.025);
        glColor3ub(255, 132, 56);
        glutSolidCube(1);
    glPopMatrix();
    /*----------Ladder-----------------------*/
    glPushMatrix();
                glTranslatef(-0.26,-0.025,0.7);
                glColor3ub(9, 0, 117);
                glScalef(0.025,0.55,0.025);
                glutSolidCube(1);
    glPopMatrix();
    glPushMatrix();
                glTranslatef(-0.26,-0.025,0.9);
                glColor3ub(9, 0, 117);
                glScalef(0.025,0.55,0.025);
                glutSolidCube(1);
    glPopMatrix();
    glPushMatrix();//Ladder steps
                glTranslatef(-0.26,-0.025,0.8);
                glColor3ub(9, 0, 117);
                glRotatef(90,1,0,0);
                glScalef(0.025,0.2,0.025);
                glutSolidCube(1);
    glPopMatrix();
    glPushMatrix();
                glTranslatef(-0.26,-0.125,0.8);
                glColor3ub(9, 0, 117);
                glRotatef(90,1,0,0);
                glScalef(0.025,0.2,0.025);
                glutSolidCube(1);
    glPopMatrix();
    glPushMatrix();
                glTranslatef(-0.26,-0.225,0.8);
                glColor3ub(9, 0, 117);
                glRotatef(90,1,0,0);
                glScalef(0.025,0.2,0.025);
                glutSolidCube(1);
    glPopMatrix();
    glPushMatrix();
                glTranslatef(-0.26,0.075,0.8);
                glColor3ub(9, 0, 117);
                glRotatef(90,1,0,0);
                glScalef(0.025,0.2,0.025);
                glutSolidCube(1);
    glPopMatrix();
    glPushMatrix();
                glTranslatef(-0.26,0.175,0.8);
                glColor3ub(9, 0, 117);
                glRotatef(90,1,0,0);
                glScalef(0.025,0.2,0.025);
                glutSolidCube(1);
    glPopMatrix();
}
void ceilingLamp(){
    glPushMatrix();
        glPushMatrix();//Wire
                GLUquadricObj * qobj2 = gluNewQuadric();
                glColor3ub(35, 35, 35);
                glTranslatef(0,1.0,0);
                glScalef(1,1,1);
                   //     glRotatef(90, 0.0f, 0.0f, 1.0f);
                glRotatef(90, 1.0f, 0.0f, 0.0f);
                gluCylinder(qobj2, 0.02, 0.02, 0.5, 50, 50);
                gluDeleteQuadric(qobj2);
        glPopMatrix();
        glTranslatef(0,0,0);
        glPushMatrix();//Shade
        glColor3ub(35, 35, 35);
                GLUquadricObj * qobj3 = gluNewQuadric();
                glTranslatef(0,0.5,0);
                glScalef(1,1,1);
                   //     glRotatef(90, 0.0f, 0.0f, 1.0f);
                glRotatef(90, 1.0f, 0.0f, 0.0f);
                gluCylinder(qobj3, 0.1, 0.4, 0.35, 50, 50);
                gluDeleteQuadric(qobj3);
        glPopMatrix();
        glPushMatrix();//Bulb
        glColor3ub(255,0,0);
        glTranslatef(0,0.25,0);
        glScalef(0.5,2,0.5);
            glutSolidSphere(0.1,32,32);
        glPopMatrix();
    glPopMatrix();
}
void shower(){
    glPushMatrix();
        glPushMatrix();//Wire
                GLUquadricObj * qobj2 = gluNewQuadric();
                glColor3ub(145, 145, 145);
                glTranslatef(0,1.0,0);
                glScalef(1,1,1);
                   //     glRotatef(90, 0.0f, 0.0f, 1.0f);
                glRotatef(90, 1.0f, 0.0f, 0.0f);
                gluCylinder(qobj2, 0.05, 0.05, 0.5, 50, 50);
                gluDeleteQuadric(qobj2);
        glPopMatrix();
        glTranslatef(0,0,0);
        glPushMatrix();//Shade
        glColor3ub(145, 145, 145);
                GLUquadricObj * qobj3 = gluNewQuadric();
                glTranslatef(0,0.5,0);
                glScalef(1,1,1);
                   //     glRotatef(90, 0.0f, 0.0f, 1.0f);
                glRotatef(90, 1.0f, 0.0f, 0.0f);
                gluCylinder(qobj3, 0.1, 0.4, 0.35, 50, 50);
                gluDeleteQuadric(qobj3);
        glPopMatrix();
    glPopMatrix();
}




void initLight()
{
    GLfloat white[]={1,1,1};
	GLfloat ambient_light[]={.5,.5,.5,.5};
    GLfloat diffuse_light[]={1,1,1,1};
    GLfloat lightpos[]={2,0,-1,1};
    glLightfv(GL_LIGHT0,GL_POSITION,lightpos);
    glLightfv(GL_LIGHT0,GL_AMBIENT,ambient_light);
    glLightfv(GL_LIGHT0,GL_DIFFUSE,diffuse_light);
    glLightfv(GL_LIGHT0,GL_SPECULAR,white);


}
void wallbed()
{
	glPushMatrix();
	glScalef(1.6,0.05,1.6);
	glutSolidCube(1);
	glPopMatrix();
}



void wall()
{
	glPushMatrix();
	glScalef(1,0.05,1);
	glutSolidCube(1);
	glPopMatrix();
}

void fan()
{

     glPushMatrix();
     glRotatef(fanAngle, 0.0f, 1.0f, 0.0f);
        glPushMatrix();
                glScalef(0.5,0.05,0.5);
                glColor3ub(20, 20, 20);

                glutSolidSphere(1,32,32);
    glPopMatrix();

    glPushMatrix();
        glTranslatef(0,0,1.3);
                glScalef(0.5,0.05,3);
                glColor3ub(20, 20, 20);

                glutSolidCube(1);
    glPopMatrix();

    glPushMatrix();
        glTranslatef(01.3,0,1.7);

                glColor3ub(20, 20, 20);
                glRotatef(40,0,1,0);
                glScalef(0.5,0.05,3);
                glTranslatef(0,0,-1.3);
                glutSolidCube(1);
    glPopMatrix();

    glPushMatrix();
        glTranslatef(-01.3,0,1.7);

                glColor3ub(20, 20, 20);
                glRotatef(-40,0,1,0);
                glScalef(0.5,0.05,3);
                glTranslatef(0,0,-1.3);
                glutSolidCube(1);
    glPopMatrix();

    glPushMatrix();//Wire
                GLUquadricObj * qobj2 = gluNewQuadric();
                glColor3ub(20, 20, 20);
                glTranslatef(0,2.0,0);
                glScalef(1,1,1);
                   //     glRotatef(90, 0.0f, 0.0f, 1.0f);
                glRotatef(90, 1.0f, 0.0f, 0.0f);
                gluCylinder(qobj2, 0.05, 0.05, 2, 50, 50);
                gluDeleteQuadric(qobj2);
    glPopMatrix();
    glPopMatrix();
}
void falseCeiling(){
    /*------------False ceiling----------------*/
    glPushMatrix();
        glColor3ub(137, 0, 0);
        glTranslatef(0,01.0,0.15);
        glScalef(1,0.01,0.2);
        glutSolidCube(1);
    glPopMatrix();
    glPushMatrix();
        glColor3ub(137, 0, 0);
        glTranslatef(0.3,0.0,0.55);
        glRotatef(90,0,1,0);
        glTranslatef(0,01.0,0.15);
        glScalef(1,0.01,0.2);
        glutSolidCube(1);
    glPopMatrix();
    glPushMatrix();
        glColor3ub(137, 0, 0);
        glTranslatef(-0.3,0.0,0.55);
        glRotatef(90,0,1,0);
        glTranslatef(0,01.0,-0.15);
        glScalef(1,0.01,0.2);
        glutSolidCube(1);
    glPopMatrix();
    glPushMatrix();
        glScalef(1.1,1,1);
        glTranslatef(0,0,1.0);
        glColor3ub(137, 0, 0);
        glTranslatef(0,01.0,0.15);
        glScalef(1,0.01,0.2);
        glutSolidCube(1);
    glPopMatrix();
    glPushMatrix();
        glTranslatef(0,0,0.25);
        glColor3ub(191, 40, 40);
        glTranslatef(0,01.0,0.15);
        glScalef(0.175,0.01,0.175);
        glutSolidCube(1);
    glPopMatrix();
    glPushMatrix();
        glTranslatef(0,0,0.75);
        glColor3ub(191, 40, 40);
        glTranslatef(0,01.0,0.15);
        glScalef(0.175,0.01,0.175);
        glutSolidCube(1);
    glPopMatrix();
    glPushMatrix();
        glTranslatef(-0.25,0,0.5);
        glColor3ub(191, 40, 40);
        glTranslatef(0,01.0,0.15);
        glScalef(0.175,0.01,0.175);
        glutSolidCube(1);
    glPopMatrix();
    glPushMatrix();
        glTranslatef(0.25,0,0.5);
        glColor3ub(191, 40, 40);
        glTranslatef(0,01.0,0.15);
        glScalef(0.175,0.01,0.175);
        glutSolidCube(1);
    glPopMatrix();
    glPushMatrix();
        glTranslatef(0,0.05,-0.17);
        glScalef(2.5,1,2.0);
        glTranslatef(0,-0.1,0.3);
        glColor3ub(219, 116, 116);
        glTranslatef(0,01.0,0.15);
        glScalef(0.175,0.01,0.175);
        glutSolidCube(1);
    glPopMatrix();
    glPushMatrix();
        glTranslatef(0,1.0,0.4);
        glColor3ub(251, 255, 153);
        glScalef(0.025,0.025,0.025);
        glutSolidSphere(1,32,32);
    glPopMatrix();
    glPushMatrix();
        glColor3ub(251, 255, 153);
        glTranslatef(0,1.0,0.9);
        glScalef(0.025,0.025,0.025);
        glutSolidSphere(1,32,32);
    glPopMatrix();
    glPushMatrix();
        glColor3ub(251, 255, 153);
        glTranslatef(-0.2775,1.0,0.65);
        glScalef(0.025,0.025,0.025);
        glutSolidSphere(1,32,32);
    glPopMatrix();
    glPushMatrix();
        glTranslatef(0.2775,1.0,0.65);
        glColor3ub(251, 255, 153);
        glScalef(0.025,0.025,0.025);
        glutSolidSphere(1,32,32);
    glPopMatrix();
    glPushMatrix();
        glTranslatef(0,0.04,0.3);
        glScalef(1,1,1);
        glTranslatef(0,-0.1,0.3);
        glColor3ub(253, 255, 196);
        glTranslatef(0,01.0,0.15);
        glScalef(0.175,0.01,0.175);
        glutSolidCube(1);
    glPopMatrix();
}
void bedroom1()
{


    glClearColor(1,1,1,1);
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
    gluPerspective(40,1,0.25,10);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	if(flagBed1){
        x=0;z=2.25;camx=0;camy=0.35;camz=0;
                lx=0.0f,lz=-1.0f,ly=0.0f;
                angle=0;angle1=0;
            flagBed1=false;
	}
	gluLookAt(x,0.4f,z,x+lx,0.5f+ly,z+lz,0.0f,1.0f,0.0f);

    glColor3ub(247, 242, 180); //brown
    glPushMatrix();
    glScalef(1,1,4);
	wallbed();
    glPopMatrix();
//roof
	glPushMatrix();
	glScalef(1,1,4);
	glTranslatef(0,1.2,0);
	glColor3ub(200, 200, 200);
	wallbed();
	glPopMatrix();
//left
	glPushMatrix();
	glColor3ub(255, 97, 0);
	glScalef(1,1,4);
	glTranslatef(-0.78,0.7,0);
	glRotatef(90,0,0,1);
	glPushMatrix();
	glScalef(1,1,4);
	glScalef(1.5,0.05,1.6);
	glutSolidCube(1);
	glPopMatrix();
	glPopMatrix();

	//back
	glColor3ub(255, 136, 0);
	glPushMatrix();
	glTranslatef(0,0.7,-0.85);
	glRotatef(-90,1,0,0);
	wallbed();
	glPopMatrix();
//right
	glPushMatrix();
	glScalef(1,1,4);
    glColor3ub(255, 97, 0);
	glTranslatef(0.78,0.7,0);
	glRotatef(90,0,0,1);
	glPushMatrix();
	glScalef(1.5,0.05,1.6);
	glutSolidCube(1);
	glPopMatrix();
	glPopMatrix();

	glPushMatrix();
        glScalef(0.75,1,1);
        glTranslatef(0,0.1,0);
        bed1();
    glPopMatrix();
    glPushMatrix();//carpet
        glScalef(0.8,0.05,1.4);
        glTranslatef(0,0.2,0);
        glColor3ub(0, 0, 61);
        glutSolidCube(1);
    glPopMatrix();
    glPushMatrix();
        glScalef(0.5,0.25,0.5);
        glTranslatef(1,1,-1);
        cabinet1();
    glPopMatrix();
    glPushMatrix();
        glTranslatef(0.7,-0.08,0);
        glRotatef(-90,0,1,0);
        glScalef(0.5,0.5,0.5);
        glTranslatef(1,1,0);
        wardrobe();
    glPopMatrix();
    glPushMatrix();
        glTranslatef(0.5,0.25,-0.45);
        glScalef(0.15,0.15,0.15);
        lamp();
    glPopMatrix();

    glPushMatrix();
        glTranslatef(-0.5,0.3,-0.6);
        glScalef(0.25,0.25,0.25);
        door2();
    glPopMatrix();
    glPushMatrix();
        glTranslatef(-0.75,0.6,0.0);
        glRotatef(90,0,1,0);
        glScalef(0.25,0.25,0.25);
        photoFrame1();
    glPopMatrix();
    falseCeiling();

    glPushMatrix();
        glTranslatef(0,0.2,-0.3);
        glScalef(1,1,1);
        glRotatef(90,0,1,0);
        ac();

    glPopMatrix();
    glFlush();

}
void chair(){
        glPushMatrix();
        glScalef(0.4,1,1);
        table1();
    glPopMatrix();
    glPushMatrix();
                    glRotatef(-10,0,0,1);
                    glTranslatef(0.2,0.2,0.275);
                    glColor3ub(170, 127, 83);
                    glScalef(0.05,0.4,0.05);
                    glutSolidCube(1);
    glPopMatrix();
    glPushMatrix();
                    glRotatef(-10,0,0,1);
                    glTranslatef(0.2,0.2,-0.275);
                    glColor3ub(170, 127, 83);
                    glScalef(0.05,0.4,0.05);
                    glutSolidCube(1);
    glPopMatrix();
    glPushMatrix();
                glTranslatef(0.275,0.4,0);
                glRotatef(80,0,0,1);
                glScalef(0.2,1.1,1);
                glColor3ub(0, 20, 155);
                glScalef(1.3,0.05,0.65);
                glutSolidCube(1);
    glPopMatrix();
}
void bedroom2()
{
    glClearColor(1,1,1,1);
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
    gluPerspective(40,1,0.25,10);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	if(flagBed2){
        x=0;z=2.35;camx=0;camy=0.35;camz=0;
                lx=0.0f,lz=-1.0f,ly=0.0f;
                angle=0;angle1=0;
            flagBed2=false;
	}
	gluLookAt(x,0.4f,z,x+lx,0.5f+ly,z+lz,0.0f,1.0f,0.0f);

    glColor3ub(244, 188, 75); //brown
    glPushMatrix();
    glScalef(1,1,4);
	wallbed();
    glPopMatrix();
//roof
	glPushMatrix();
	glScalef(1,1,4);
	glTranslatef(0,1.2,0);
	glColor3ub(255, 255, 255);
	wallbed();
	glPopMatrix();
//left
	glPushMatrix();
	glColor3ub(201, 210, 255);
	glScalef(1,1,4);
	glTranslatef(-0.78,0.7,0);
	glRotatef(90,0,0,1);
	glPushMatrix();
	glScalef(1,1,4);
	glScalef(1.5,0.05,1.6);
	glutSolidCube(1);
	glPopMatrix();
	glPopMatrix();

	//back
	glColor3ub(54, 0, 130);
	glPushMatrix();
	glTranslatef(0,0.7,-0.85);
	glRotatef(-90,1,0,0);
	wallbed();
	glPopMatrix();
//right
	glPushMatrix();
	glScalef(1,1,4);
    glColor3ub(201, 210, 255);
	glTranslatef(0.78,0.7,0);
	glRotatef(90,0,0,1);
	glPushMatrix();
	glScalef(1.5,0.05,1.6);
	glutSolidCube(1);
	glPopMatrix();
	glPopMatrix();


    glPushMatrix();
    glTranslatef(-0.72,0.4,-0.7);
    glScalef(0.35,0.35,0.35);
    bookshelf();
    glPopMatrix();

    glPushMatrix();//False ceiling
        glTranslatef(0,1.15,0.5);
        glScalef(1,0.01,2);
        glColor3ub(134, 116, 219);
        glutSolidCube(1);
    glPopMatrix();

glPushMatrix();
        glTranslatef(0.75,0.3,1.1);
        glRotated(-90,0,1,0);
        glScalef(0.5,0.5,0.5);
        door2();
    glPopMatrix();
    glPushMatrix();
        glTranslatef(0.3,0.55,-0.7);
        glScalef(0.65,0.65,2.2);
        wardrobe();
    glPopMatrix();

    glPushMatrix();
        glTranslatef(0.6,0.4,0.0);
        glScalef(0.7,0.7,0.7);
        bunkerBed();
    glPopMatrix();

    glPushMatrix();//carpet
        glTranslatef(0,0.0,0.4);
        glScalef(0.8,0.05,1.4);
        glTranslatef(0,0.2,0);
        glColor3ub(244, 242, 255);
        glutSolidCube(1);
    glPopMatrix();

    glPushMatrix();
        glTranslatef(-0.6,0.2,0.5);
        glRotatef(90,0,1,0);
        glScalef(0.5,0.5,0.5);
        glTranslatef(0,0.2,0);
        glColor3ub(244, 242, 255);
        table1();
    glPopMatrix();
    glPushMatrix();
        glTranslatef(-0.4,0,0.6);
  //      glRotatef(90,0,1,0);
        glScalef(0.4,0.4,0.4);
        glTranslatef(0,0.5,0);
        chair();
    glPopMatrix();
     glPushMatrix();
        glTranslatef(-0.77,0.4,0.2);
        glScalef(0.8,0.8,0.8);
        glRotatef(90,0,1,0);
        glScalef(1,0.8,0.8);
        glTranslatef(0,0.5,0);
        photoFrame2();
    glPopMatrix();

    glPushMatrix();
        glTranslatef(0,1.0,0.8);
        glScalef(0.1,0.1,0.1);
        fan();
    glPopMatrix();
/**
// draw uppder side of the doll
	glPushMatrix();
	glScalef(0.2,0.2,0.2);
		draw_upper_torso();

		glPushMatrix();
		glScalef(0.1,0.1,0.1);
			draw_head();
		glPopMatrix();
glScalef(0.1,0.1,0.1);
		draw_right_arm();
		glScalef(0.1,0.1,0.1);
		draw_left_arm();
	glPopMatrix();

	//// draw lower side of the doll
	glPushMatrix();
	glScalef(0.1,0.1,0.1);
		draw_lower_torso();
		glScalef(0.1,0.1,0.1);
		draw_right_leg();
		glScalef(0.1,0.1,0.1);
		draw_left_leg();
	glPopMatrix();
*/
    glFlush();

}
void kitchenCeilingCabinet(){
        /*----------------Cabinet part1 ------------------------*/
            glPushMatrix();
                glColor3ub(148, 255, 0);
                glTranslatef(0,-0.3,0);
                glScalef(0.8,0.3,0.8);
                glutSolidCube(1);
            glPopMatrix();

            /*----------------Cabinet part2 ------------------------*/
            glPushMatrix();
                glColor3ub(148, 255, 0);
                glTranslatef(0,-0.6,0);
                glScalef(0.8,0.3,0.8);
                glutSolidCube(1);
            glPopMatrix();
            /*----------------Cabinet part3 ------------------------*/
            glPushMatrix();
                glColor3ub(148, 255, 0);
                glTranslatef(0,-0.6,0);
                glScalef(0.8,0.3,0.8);
                glutSolidCube(1);
            glPopMatrix();
            /*----------------Cabinet handle1 ------------------------*/
            glPushMatrix();
                glColor3f(255, 255, 255);
                glTranslatef(0,-0.65,.5);
                glScalef(0.2,0.025,0.05);
                glutSolidCube(1);
            glPopMatrix();
}
void kitchenCeilingCabinet2(){
        glPushMatrix();
        glScalef(0.6,1,0.6);
        glPushMatrix();
            glTranslatef(0,0.25,0);
            /*----------------Cabinet part1 ------------------------*/
            glPushMatrix();
                glColor3ub(148, 255, 0);
                glTranslatef(0,-0.3,0);
                glScalef(0.8,0.3,0.8);
                glutSolidCube(1);
            glPopMatrix();

            /*----------------Cabinet part2 ------------------------*/
            glPushMatrix();
                glColor3ub(148, 255, 0);
                glTranslatef(0,-0.6,0);
                glScalef(0.8,0.3,0.8);
                glutSolidCube(1);
            glPopMatrix();

            /*----------------Cabinet handle ------------------------*/
            glPushMatrix();
                glColor3ub(255, 255, 255);
                glTranslatef(0,-0.3,.5);
                glScalef(0.2,0.025,0.05);
                glutSolidCube(1);
            glPopMatrix();
        glPopMatrix();
    glPopMatrix();
}
void kitchOven(){
        glPushMatrix();
        glScalef(0.6,1,0.6);
        glPushMatrix();
            glTranslatef(0,0.25,0);
            /*----------------Cabinet part1 ------------------------*/
            glPushMatrix();
                glColor3ub(130, 130, 130);
                glTranslatef(0,-0.3,0);
                glScalef(0.8,0.3,0.8);
                glutSolidCube(1);
            glPopMatrix();

            /*----------------Cabinet part2 ------------------------*/
            glPushMatrix();
                glColor3ub(130, 130, 130);
                glTranslatef(0,-0.6,0);
                glScalef(0.8,0.3,0.8);
                glutSolidCube(1);
            glPopMatrix();

            glPushMatrix();//Black
                glColor3ub(0, 0, 0);
                glTranslatef(0,-0.4,0.175);
                glScalef(0.6,0.25,0.5);
                glutSolidCube(1);
            glPopMatrix();

            glPushMatrix();
                glColor3ub(60, 60, 60);
                glTranslatef(0,-0.4,0.2);
                glScalef(0.5,0.2,0.5);
                glutSolidCube(1);
            glPopMatrix();

            glPushMatrix();
                glColor3ub(200, 200, 200);
                glTranslatef(0,-0.2,0.3);
                glScalef(0.5,0.025,0.5);
                glutSolidCube(1);
            glPopMatrix();
            glPushMatrix();
                glColor3ub(0, 0, 0);
                glTranslatef(0,-0.6,0.16);
                glScalef(0.8,0.01,0.5);
                glutSolidCube(1);
            glPopMatrix();

        glPopMatrix();
    glPopMatrix();
}
void kitchen()
{
    glClearColor(1,1,1,1);
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
    gluPerspective(40,1,0.25,10);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	if(flagBed1){
        x=0;z=2.25;camx=0;camy=0.35;camz=0;
                lx=0.0f,lz=-1.0f,ly=0.0f;
                angle=0;angle1=0;
            flagBed1=false;
	}
	gluLookAt(x,0.4f,z,x+lx,0.5f+ly,z+lz,0.0f,1.0f,0.0f);

    glColor3ub(237, 237, 237); //brown
    glPushMatrix();
    glScalef(1,1,4);
	wallbed();
    glPopMatrix();
//roof
	glPushMatrix();
	glScalef(1,1,4);
	glTranslatef(0,1.2,0);
	glColor3ub(200, 200, 200);
	wallbed();
	glPopMatrix();
//left
	glPushMatrix();
	glColor3ub(0, 109, 74);
	glScalef(1,1,4);
	glTranslatef(-0.78,0.7,0);
	glRotatef(90,0,0,1);
	glPushMatrix();
	glScalef(1,1,4);
	glScalef(1.5,0.05,1.6);
	glutSolidCube(1);
	glPopMatrix();
	glPopMatrix();

	//back
	glColor3ub(255, 255, 255);
	glPushMatrix();
	glTranslatef(0,0.7,-0.85);
	glRotatef(-90,1,0,0);
	wallbed();
	glPopMatrix();
//right
	glPushMatrix();
	glScalef(1,1,4);
    glColor3ub(0, 109, 74);
	glTranslatef(0.78,0.7,0);
	glRotatef(90,0,0,1);
	glPushMatrix();
	glScalef(1.5,0.05,1.6);
	glutSolidCube(1);
	glPopMatrix();
	glPopMatrix();
    /*---------Top kitchen Cabin--------*/
	glPushMatrix();
	glTranslatef(0,1.2,-0.7);
	glScalef(0.25,0.5,0.4);
    kitchenCeilingCabinet();
	glPopMatrix();
	glPushMatrix();
	glTranslatef(0.205,1.2,-0.7);
	glScalef(0.25,0.5,0.4);
    kitchenCeilingCabinet();
	glPopMatrix();
    glPushMatrix();
	glTranslatef(0.41,1.2,-0.7);
	glScalef(0.25,0.5,0.4);
    kitchenCeilingCabinet();
	glPopMatrix();
    glPushMatrix();
	glTranslatef(0.615,1.2,-0.7);
	glScalef(0.25,0.5,0.4);
    kitchenCeilingCabinet();
	glPopMatrix();
	glPushMatrix();
	glTranslatef(0,1.2,-0.7);
	glScalef(0.25,0.5,0.4);
    kitchenCeilingCabinet();
	glPopMatrix();
	/*----*/
	glPushMatrix();
	glTranslatef(-0.205,1.2,-0.7);
	glScalef(0.25,0.5,0.4);
    kitchenCeilingCabinet();
	glPopMatrix();
    glPushMatrix();
	glTranslatef(-0.41,1.575,-0.7);
	glScalef(0.25,3,0.4);
    kitchenCeilingCabinet();
	glPopMatrix();
    glPushMatrix();
	glTranslatef(-0.615,1.575,-0.7);
	glScalef(0.25,3,0.4);
    kitchenCeilingCabinet();
	glPopMatrix();

	glPushMatrix();
        glTranslatef(-0.7,0,0.3);
        glRotatef(90,0,1,0);
        glTranslatef(0,0.1,0);
        glScalef(0.8,0.8,0.8);
        fridge();
	glPopMatrix();
    /*----------Slabs-----------------*/
    glPushMatrix();
        glColor3f(0,0,0);
 //       glTranslatef(-0.7,0,0.3);
 //       glRotatef(90,0,1,0);
        glTranslatef(0.45,0.35,-0.9);
        glScalef(1.5,0.01,0.8);
        glutSolidCube(1);
	glPopMatrix();
	/*---------------Bottom cabin---------*/
	glPushMatrix();
	glTranslatef(-0.18,0.29,-0.65);
	glScalef(0.5,0.6,0.5);
    kitchenCeilingCabinet2();
    glPopMatrix();
    glPushMatrix();
	glTranslatef(0.0675,0.29,-0.65);
	glScalef(0.5,0.6,0.5);
    kitchenCeilingCabinet2();
    glPopMatrix();
    glPushMatrix();
	glTranslatef(0.315,0.29,-0.65);
	glScalef(0.5,0.6,0.5);
    kitchOven();
    glPopMatrix();
    glPushMatrix();
	glTranslatef(0.5625,0.29,-0.65);
	glScalef(0.5,0.6,0.5);
    kitchenCeilingCabinet2();
    glPopMatrix();
    glPushMatrix();
	glTranslatef(0.81,0.29,-0.65);
	glScalef(0.5,0.6,0.5);
    kitchenCeilingCabinet2();
    glPopMatrix();

    glPushMatrix();
        glTranslatef(0.2,0.6,-0.75);
        glScalef(0.5,0.25,0.01);
        glColor3ub(48, 162, 255);
        glutSolidCube(1);
    glPopMatrix();
        glPushMatrix();
        glTranslatef(0.2,0.6,-0.8);
        glScalef(0.6,0.325,0.01);
        glColor3ub(0, 107, 24);
        glutSolidCube(1);
    glPopMatrix();

    glPushMatrix();
        glTranslatef(0.6,0.2,0.5);
        glRotatef(90,0,1,0);
        glScalef(0.5,0.5,0.5);
        glTranslatef(0,0.2,0);
        glColor3ub(244, 242, 255);
        table2();
    glPopMatrix();

    glPushMatrix();//Stove
        glTranslatef(0.2,0.37,-0.6);
        glScalef(0.3,0.03,0.1);
        glColor3ub(80,80,80);
        glutSolidCube(1);
    glPopMatrix();

    glPushMatrix();
        glTranslatef(0.6,0.36,0.4);
        glScalef(0.07,0.07,0.07);
        glColor3ub(173, 0, 0);
        glutSolidTeapot(1);
    glPopMatrix();

    glPushMatrix();//Stove
        glTranslatef(0.6,0.35,-0.65);
        glScalef(0.2,0.03,0.2);
        glColor3ub(191, 191, 191);
        glutSolidCube(1);
    glPopMatrix();

        glPushMatrix();//Tap
        glTranslatef(0.6,0.4,-1.0);
        glScalef(0.01,0.01,0.4);
        glColor3ub(191, 191, 191);
        glutSolidSphere(1,32,32);
    glPopMatrix();

    glFlush();

}
void bucket1(){
    glPushMatrix();//Shade
        glColor3ub(76, 192, 255);
                GLUquadricObj * qobj3 = gluNewQuadric();
                glTranslatef(0,0.5,0);
                glScalef(1,1,1);
                   //     glRotatef(90, 0.0f, 0.0f, 1.0f);
                glRotatef(90, 1.0f, 0.0f, 0.0f);
                gluCylinder(qobj3, 0.3, 0.2, 0.9, 50, 50);
                gluDeleteQuadric(qobj3);
    glPopMatrix();
}
void bucket2(){
    glPushMatrix();//Shade
        glColor3ub(209, 29, 29);
                GLUquadricObj * qobj3 = gluNewQuadric();
                glTranslatef(0,0.5,0);
                glScalef(1,1,1);
                   //     glRotatef(90, 0.0f, 0.0f, 1.0f);
                glRotatef(90, 1.0f, 0.0f, 0.0f);
                gluCylinder(qobj3, 0.3, 0.2, 0.9, 50, 50);
                gluDeleteQuadric(qobj3);
    glPopMatrix();
}
void tap(){
    glPushMatrix();//Shade
        glColor3ub(122, 122, 122);
                GLUquadricObj * qobj3 = gluNewQuadric();
                glRotatef(90, 0.0f, 0.0f, 1.0f);
                glTranslatef(0,0.5,0);
                glScalef(1,1,1);
                   //     glRotatef(90, 0.0f, 0.0f, 1.0f);
                glRotatef(90, 1.0f, 0.0f, 0.0f);
                gluCylinder(qobj3, 0.2, 0.2, 1.3, 50, 50);
                gluDeleteQuadric(qobj3);
    glPopMatrix();
    glPushMatrix();
        glTranslatef(-0.5,0.0,0);
        glColor3ub(122, 122, 122);
        glScalef(0.3,0.3,0.3);
        glutSolidSphere(1,32,32);
    glPopMatrix();
}
void toilet()
{
    glClearColor(1,1,1,1);
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
    gluPerspective(40,1,0.25,10);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	if(flagToil){
        x=0;z=2.25;camx=0;camy=0.35;camz=0;
                lx=0.0f,lz=-1.0f,ly=0.0f;
                angle=0;angle1=0;
            flagToil=false;
	}
	gluLookAt(x,0.4f,z,x+lx,0.5f+ly,z+lz,0.0f,1.0f,0.0f);

    glColor3ub(114, 34, 106); //brown
    glPushMatrix();
    glScalef(1,1,4);
	wallbed();
    glPopMatrix();
//roof
	glPushMatrix();
	glScalef(1,1,4);
	glTranslatef(0,1.2,0);
	glColor3ub(255, 224, 252);
	wallbed();
	glPopMatrix();
//left
	glPushMatrix();
	glTranslatef(0.4,0,0);
	glColor3ub(255, 193, 248);
	glScalef(1,1,4);
	glTranslatef(-0.78,0.7,0);
	glRotatef(90,0,0,1);
	glPushMatrix();
	glScalef(1,1,4);
	glScalef(1.5,0.05,1.6);
	glutSolidCube(1);
	glPopMatrix();
	glPopMatrix();

	//back
	glColor3ub(255, 53, 232);
	glPushMatrix();
	glTranslatef(0,0.7,-0.85);
	glRotatef(-90,1,0,0);
	wallbed();
	glPopMatrix();
//right
	glPushMatrix();
	glTranslatef(-0.4,0,0);
	glScalef(1,1,4);
    glColor3ub(255, 193, 248);
	glTranslatef(0.78,0.7,0);
	glRotatef(90,0,0,1);
	glPushMatrix();
	glScalef(1.5,0.05,1.6);
	glutSolidCube(1);
	glPopMatrix();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0,1.0-0.0,0);
	glScalef(0.3,0.3,0.3);
        ceilingLamp();
	glPopMatrix();

   glPushMatrix();
    glColor3ub(255, 255, 255);
    glTranslatef(0,0.6,-0.9);
	glScalef(0.4,0.2,0.25);
	glutSolidCube(1);
	glPopMatrix();

	glPushMatrix();
    glColor3ub(255, 255, 255);
    glTranslatef(0,0.25,-0.7);
	glScalef(0.15,0.1,0.5);
	glutSolidCube(1);
	glPopMatrix();

		glPushMatrix();
    glColor3ub(255, 255, 255);
    glTranslatef(0,0.18,-0.7);
	glScalef(0.1,0.1,0.5);
	glutSolidCube(1);
	glPopMatrix();

    glPushMatrix();
    glColor3ub(255, 255, 255);
    glTranslatef(0,0.3,-0.7);
	glScalef(0.11,0.009,0.14);
	glutSolidSphere(1,32,32);
	glPopMatrix();

    glPushMatrix();
    glColor3ub(100, 100, 100);
    glTranslatef(0.15,0.6,-0.7);
	glScalef(0.02,0.02,0.02);
	glutSolidSphere(1,32,32);
	glPopMatrix();


	glPushMatrix();
	glTranslatef(-0.1,0.4,0);
	glRotatef(-45,0,0,1);
	glTranslatef(0,0.5,0);
	glScalef(0.15,0.15,0.15);
        shower();
	glPopMatrix();

    glPushMatrix();
	glTranslatef(0.2,0.05,0.5);
	glScalef(0.25,0.4,0.25);
        bucket1();
	glPopMatrix();

    glPushMatrix();
	glTranslatef(0.2,0.05,0.9);
	glScalef(0.25,0.2,0.25);
        bucket2();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.35,0.35,0.4);
	glScalef(0.1,0.1,0.1);
        tap();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.35,0.35,0.7);
	glScalef(0.1,0.1,0.1);
        tap();
	glPopMatrix();

    glPushMatrix();
	glTranslatef(0.42,0.45,00);
	glScalef(0.1,0.1,0.1);
        tap();
	glPopMatrix();


    glFlush();

}
void corridor()
{
	glClearColor(1,1,1,1);
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
    gluPerspective(40,1,0.25,10);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	if(flagCori){
        x=0;z=2.25;camx=0;camy=0.35;camz=0;
                lx=0.0f,lz=-1.0f,ly=0.0f;
                angle=0;angle1=0;
            flagCori=false;
	}
	//gluLookAt(rotx,roty,rotz,camx,camy,camz,0,1,0);
    gluLookAt(x,0.4f,z,x+lx,0.5f+ly,z+lz,0.0f,1.0f,0.0f);
    /*-------------------------------------*/

	glColor3ub(224,145,0);
    glPushMatrix();
        glScalef(1,1,4);
        wall();
    glPopMatrix();

	glPushMatrix();
	glTranslatef(0,1,0);
	glScalef(1,1,4);
	glColor3ub(130,175,255);
	wall();
	glPopMatrix();

	glPushMatrix();
	glColor3ub(0,165,255);
	glTranslatef(-0.5,0.5,0);
	glScalef(1,1,4);
	glRotatef(90,0,0,1);
	wall();
	glPopMatrix();

	//back
	glColor3ub(0,144,255);
	glPushMatrix();
	glTranslatef(0,0.5,-0.5);
	glRotatef(-90,1,0,0);
	wall();
	glPopMatrix();

	glPushMatrix();
	glColor3ub(0,165,255);
	glTranslatef(0.5,0.5,0);
	glScalef(1,1,4);
	glRotatef(90,0,0,1);
	wall();
	glPopMatrix();

    glPushMatrix();
        glScalef(1,1,2);
        glTranslatef(-0.42,-0.2,0.1);
        glRotatef(90,0,1,0);
        glTranslatef(0,0.5,0);
        glScalef(0.25,0.25,0.25);
        door();
    glPopMatrix();
    glPushMatrix();
        glScalef(1,1,2);
        glTranslatef(0.42,-0.2,0.3);
        glRotatef(90,0,1,0);
        glTranslatef(0,0.5,0);
        glScalef(0.25,0.25,0.25);
        door();
    glPopMatrix();
    glPushMatrix();
        glTranslatef(0.85,0,1.0);
        glRotatef(180,0,1,0);
        glScalef(1,1,2);
        glTranslatef(0.42,-0.2,0.4);
        glRotatef(90,0,1,0);
        glTranslatef(0,0.5,0);
        glScalef(0.25,0.25,0.25);
        door();
    glPopMatrix();

	//frame
	glPushMatrix();
	glTranslatef(-.02,0.4,-.4);
	glScalef(0.1,0.1,0.1);
    glColor3f(0,0,0);
    glBegin(GL_QUADS);
    //    glColor3f(1,1,0);
        glVertex3f(-2,-1.5,0);
 //       glColor3f(1,0,0);
        glVertex3f(3,-1.5,0);
 //       glColor3f(0,1,0);
        glVertex3f(3,2.5,0);
    //    glColor3f(0,0,1);
        glVertex3f(-2,2.5,0);
 	glEnd();
    glPopMatrix();

    glPushMatrix();
	glTranslatef(-.02,0.4,-.4);
	glScalef(0.1,0.1,0.1);
    glColor3f(0.5,0.5,0.5);
    glBegin(GL_QUADS);
    //    glColor3f(1,1,0);
        glVertex3f(-2.1,-1.8,-0.05);
 //       glColor3f(1,0,0);
        glVertex3f(3.1,-1.8,-0.05);
 //       glColor3f(0,1,0);
        glVertex3f(3.1,2.6,-0.05);
    //    glColor3f(0,0,1);
        glVertex3f(-2.1,2.6,-0.05);
 	glEnd();

     glBegin(GL_QUADS);
    //    glColor3f(1,1,0);
        glVertex3f(-3,-1.5,-0.05);
 //       glColor3f(1,0,0);
        glVertex3f(-2.7,-1.5,-0.05);
 //       glColor3f(0,1,0);
        glVertex3f(-2.7,2.3,-0.05);
    //    glColor3f(0,0,1);
        glVertex3f(-3,2.3,-0.05);
 	glEnd();

 	glTranslatef(6.6,0,0);
 	glColor3f(0,0,0);
 	glPushMatrix();
         glBegin(GL_QUADS);
    //    glColor3f(1,1,0);
        glVertex3f(-3,-1.5,-0.05);
 //       glColor3f(1,0,0);
        glVertex3f(-2.7,-1.5,-0.05);
 //       glColor3f(0,1,0);
        glVertex3f(-2.7,2.3,-0.05);
    //    glColor3f(0,0,1);
        glVertex3f(-3,2.3,-0.05);
 	glEnd();
 	glPopMatrix();

    glPopMatrix();
    ac();
    glPushMatrix();
        glTranslatef(0.03,0.14,0.8);
        glScalef(0.5,0.3,0.5);
        sofa();
        glutPostRedisplay();
    glPopMatrix();

	glFlush();
}
void display()
{
	glClear(GL_COLOR_BUFFER_BIT);
	display1();

	glFlush();

}






void display1()
{
	glPushMatrix();
	glTranslated(100,70,1);  //x-axis,y-axis(above the water)
	ship();
	glPopMatrix();
	/**water();
	cloud_three();
    cloud_four();
    cloud_one();
    cloud_two();
    cloud_five();
    cloud_six();
*/
}
/*To draw plane*/
void water()
{
		glPushMatrix();
		//glPushMatrix();
		glTranslated(0,0,0.0);
		glColor4f(0.1,0.5,1.0,0.5);
		glBegin(GL_POLYGON);
		glVertex2f(0,0);
		glVertex2f(0,100);
		glVertex2f(10,96);
		glVertex2f(25,98);
		glVertex2f(39,94);
		glVertex2f(50,92);
		glVertex2f(70,98);
		glVertex2f(85,95);
		glVertex2f(95,96);
		glVertex2f(110,99);
		glVertex2f(128,97);
		glVertex2f(139,95);
		glVertex2f(145,97);
		glVertex2f(155,99);
		glVertex2f(172,95);
		glVertex2f(195,96);
		glVertex2f(212,95);
		glVertex2f(254,92);
		glVertex2f(284,96);
		glVertex2f(344,98);
		glVertex2f(360,93);
		glVertex2f(390,94);
		glVertex2f(410,99);
		glVertex2f(450,94);
		glVertex2f(485,100);
		glVertex2f(502,92);
		glVertex2f(552,92);
		glVertex2f(592,96);
		glVertex2f(630,105);
		glVertex2f(680,93);
		glVertex2f(720,97);
		glVertex2f(750,93);
		glVertex2f(800,95);
		glVertex2f(850,97);
		glVertex2f(880,108);
		glVertex2f(900,96);
		glVertex2f(920,93);
		glVertex2f(950,99);
		glVertex2f(980,92);
		glVertex2f(1000,99);
		glVertex2f(1000,0);
		glVertex2f(1600,10);
		glVertex2f(3000,280);

		glEnd();
		glPopMatrix();
}

/* TO DRAW SHIP */
void ship()
{
    glColor3f(1.0,0.0,0.0);
    drawstring(500,600,0,"           INTRO  ");
    glColor3f(1.0,0.5,0.0);
    drawstring(500,570,0,"PRESS 1 TO VIEW HALL");
    drawstring(500,535,0,"PRESS 2 TO VIEW BEDROOM");
    drawstring(500,500,0,"PRESS 3 TO VIEW PLAYROOM");
    drawstring(500,465,0,"PRESS 4 TO VIEW KITCHEN");
    drawstring(500,430,0,"PRESS 5 TO VIEW WASHROOM");
}
void displayy()
{
     if(slide==1) display();
    if(slide==2) corridor();
    if(slide==3) bedroom1();
    if(slide==4) bedroom2();
    if(slide==10) kitchen();
    if(slide==15) toilet();

}
void circle(GLdouble rad)
{
    GLint points = 50;
    GLdouble delTheta = (2.0 * PI) / (GLdouble)points;
    GLdouble theta = 0.0;

    glBegin(GL_POLYGON);
    {
        for( i = 0; i <=50; i++, theta += delTheta )
        {
            glVertex2f(rad * cos(theta),rad * sin(theta));
        }
    }
    glEnd();
}


void drawrectangle()
{


    glBegin(GL_LINE_LOOP);

    glVertex2i(50,80);
    glVertex2i(70,80);
    glVertex2i(70,90);
    glVertex2i(50,90);
    glEnd();




}
void drawrectangle2()
{
    /*glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(1,0,1);*/
    //glLineWidth(30);
    glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
    glBegin(GL_POLYGON);
    glVertex2i(8,8);
    glVertex2i(8,93);
    glVertex2i(93,93);
    glVertex2i(93,8);
    glEnd();
}
void displays()
{

    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(1,0,0);
    drawstring(30,20,0,"Room SIMULATION");
    glColor3f(1,0,1);

    drawstring(55,85,0,"NEXT");
    drawrectangle();
    drawrectangle2();
    glutSwapBuffers();

}

void myinit()
{
	glClearColor(0.1f,0.1f,0.1f,0.1f);
	glColor3f(1.0,0.0,0.0);
	glPointSize(1.0);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0.0,999.0,0.0,799.0);

   display();

}
void mouseMove(int mx, int my) {
  /*  if((slide!=1)&&(slide!=3)&&(slide!=4))
        return;*/
    if(mx!=curX){
        if(mx>curX){
            angle += 0.01f;
			lx = sin(angle);
			lz = -cos(angle);
        }
        else{
            angle -= 0.01f;
			lx = sin(angle);
			lz = -cos(angle);
        }
    }
    if(my!=curY){
        if(my>curY){
            angle1 -= 0.006f;
			ly = sin(angle1);
		//	lz = -cos(angle);
        }
        else{
            angle1 += 0.006f;
			ly = sin(angle1);
	//		lz = -cos(angle);
        }
    }
   printf("x:%d y:%d\n",mx,my);
    curX=mx;
    curY=my;
    glutPostRedisplay();
}

void MouseButton(int button, int state, int x, int y)
{


  // Respond to mouse button presses.
  // If button1 pressed, mark this state so we know in motion function.
    printf("%d\n",x);
    printf("%d\n",y);
    if(state==GLUT_DOWN&&button==GLUT_LEFT_BUTTON&&(x>500&&x<1500)&&(y>500&&y<1500))
    {

    glutInitDisplayMode(GLUT_DEPTH);
        glutInitWindowSize(1300,900);
        glutInitWindowPosition(0,0);
  glutCreateWindow("ROOM SIMULATION");
        glutDisplayFunc(display);

myinit();
glutDisplayFunc(displayy);
glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_COLOR_MATERIAL);
	glColorMaterial ( GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE ) ;
	glEnable(GL_NORMALIZE);
 glutMouseFunc(mousemain);
 glutMotionFunc(mouseMove);
    glutKeyboardFunc(keyboard1);
glutSpecialFunc(processSpecialKeys);
    glutTimerFunc(200,mytimer,0);
	initLight();
    glutMainLoop();



    }
}


void init()
{
     glClearColor(1,1,1,1);

    gluOrtho2D(0,100,100,0);
}

int main(int argc, char* argv[])
{

glutInit(&argc, argv);

	glutInitWindowSize(800,800);
	glutInitWindowPosition(0,0);
	glutCreateWindow(" intro");
	init();

	 glutDisplayFunc(displays);
     glutMouseFunc(MouseButton);

	 	glutMainLoop();

}
