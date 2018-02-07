//header
#include <OpenGL/gl3.h>
#include <GLUT/glut.h>
#include<stdio.h>

//globals

GLuint buddha;
float buddhaRot;
char ch='1';
void keyboard (unsigned char key, int x, int y);
int count = 1;
int countF = 1;
int tempV;
float vertices[49991][6] = {0,0,0,0,0,0};
float faces[100001][6] = {0,0,0,0,0,0};
int light1X = 1.0;
int light2X = -1.0;
int lightSwitch = 0;

//functions
void genFaceNormals();


//.obj loader code begins

void loadObj(char *fname)
{
    
    FILE *fp, *fp2, *fp3;
    int read;
    GLfloat x, y, z;
    GLint a, b, c;
    GLint d, e, f;
    char ch, ch2, ch3;
    buddha=glGenLists(1);
    
    printf("Fetching Verticies...");
    fp=fopen(fname,"r");
    if (!fp) {
        printf("can't open file %s\n", fname);
        exit(1);
    }
    glPointSize(1.0);
    glNewList(buddha, GL_COMPILE);
    {
        glPushMatrix();
        while(!(feof(fp)))
        {
            read=fscanf(fp,"%c %f %f %f",&ch,&x,&y,&z);
            if(read==4&&ch=='v'){
                vertices[count][0] = x;
                vertices[count][1] = y;
                vertices[count][2] = z;
                count++;
            }
        }
    }
    fclose(fp);
    
    printf("\nFetching faces...");
    fp2=fopen(fname,"r");
    while(!(feof(fp2)))
    {
        read=fscanf(fp2,"%c %d %d %d",&ch2,&a,&b,&c);
        if(read==4&&ch2=='f'){
            faces[countF][0] = a;
            faces[countF][1] = b;
            faces[countF][2] = c;
            genFaceNormals(countF);
            countF++;
        }
        
    }
    fclose(fp2);
    
    printf("\nCalculating vertex normals...");
    for (int i = 1; i <= 49991; i++) {
        int adjFaces = 0;
        float totalI = 0;
        float totalJ = 0;
        float totalK = 0;
        
        //for each vertice, find all faces that contain that vertex and add their normal vectors together.
        //then average the normal vectors for the adjacent faces and add it to the corresponding vertex in the array
        for (int j =0; j <= 100000; j++){
            if(faces[j][0] == i){
                totalI = totalI +faces[j][3];
                totalJ = totalJ +faces[j][4];
                totalK = totalK +faces[j][5];
                adjFaces++;
            }
            if(faces[j][1] == i){
                totalI = totalI +faces[j][3];
                totalJ = totalJ +faces[j][4];
                totalK = totalK +faces[j][5];
                adjFaces++;
            }
            if(faces[j][2] == i){
                totalI = totalI +faces[j][3];
                totalJ = totalJ +faces[j][4];
                totalK = totalK +faces[j][5];
                adjFaces++;
            }
            else continue;
        }
        vertices[i][3] = (totalI/adjFaces);
        vertices[i][4] = (totalJ/adjFaces);
        vertices[i][5] = (totalK/adjFaces);
        
        printf("Vertex Norms: %3.8f %3.8f %3.8f \n", vertices[i][3],vertices[i][4],vertices[i][5]);
    }
    
    
    printf("Drawing buddha...");
    fp3=fopen(fname,"r");
    while(!(feof(fp3)))
    {
        read=fscanf(fp3,"%c %d %d %d",&ch3,&d,&e,&f);
        if(read==4&&ch3=='f'){
//            printf("i: %3.2f", vertices[d][3]);
//            printf("j: %3.2f", vertices[d][4]);
//            printf("k: %3.2f", vertices[d][5]);
            glBegin(GL_POLYGON);
            glNormal3f(vertices[d][3], vertices[d][4], vertices[d][5]);
            glVertex3f(vertices[d][0], vertices[d][1], vertices[d][2]);
            glNormal3f(vertices[e][3], vertices[e][4], vertices[e][5]);
            glVertex3f(vertices[e][0], vertices[e][1], vertices[e][2]);
            glNormal3f(vertices[f][3], vertices[f][4], vertices[f][5]);
            glVertex3f(vertices[f][0], vertices[f][1], vertices[f][2]);
            glEnd();
        }
        
    }
    
    glPopMatrix();
    glEndList();
    fclose(fp3);
}

void genFaceNormals(int faceNum){
    //get vertice numbers
    int a = faces[faceNum][0];
    int b = faces[faceNum][1];
    int c = faces[faceNum][2];
    
    //get x,y,z coordinates of all vertices that make up the face
    float aX = vertices[a][0];
    float aY = vertices[a][1];
    float aZ = vertices[a][2];
    float bX = vertices[b][0];
    float bY = vertices[b][1];
    float bZ = vertices[b][2];
    float cX = vertices[c][0];
    float cY = vertices[c][1];
    float cZ = vertices[c][2];
    
    
    //calculate two edge vectors for the face
    float edge1X = aX - bX;
    float edge1Y = aY - bY;
    float edge1Z = aZ - bZ;
    float edge2X = cX - bX;
    float edge2Y = cY - bY;
    float edge2Z = cZ - bZ;
    
    
    //use the edge vectors to perform the cross product and generae the normal vector
    float i = (edge1Y * edge2Z) - (edge2Y * edge1Z);
    float j = -1*((edge1X * edge2Z) - (edge2X * edge1Z));
    float k = (edge1X * edge2Y) - (edge2X * edge1Y);
    
    
    //assign i,j,k (normal vector of the face) to the current face in the array
    faces[faceNum][3] = i;
    faces[faceNum][4] = j;
    faces[faceNum][5] = k;

    
}

//.obj loader code ends here










void reshape(int w,int h)
{    
	glViewport(0,0,w,h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
    gluPerspective (60, (GLfloat)w / (GLfloat)h, 0.1, 1000.0);
	//glOrtho(-25,25,-2,2,0.1,100);
	glMatrixMode(GL_MODELVIEW);
}

void draw()
{
 	glPushMatrix();
 	glTranslatef(0,0,-105);
 	glColor3f(1.0,0.23,0.27);
 	glScalef(50,50,50);
 	glRotatef(-90,1,0,0);
    glRotatef(buddhaRot,0,0,1);
 	glCallList(buddha);
 	glPopMatrix();
 	buddhaRot=buddhaRot+0.6;
 	if(buddhaRot>360)buddhaRot=buddhaRot-360;
    
}

void display(void)
{  
   	glClearColor (0.0,0.0,0.0,1.0); 
   	glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   	glLoadIdentity();
 	draw();
    GLfloat light_position1[] = {light1X, 0.0, 0.0, 1.0};
    glLightfv(GL_LIGHT0, GL_POSITION, light_position1);
    GLfloat light_position2[] = {light2X, 0.0, 0.0, 1.0};
    glLightfv(GL_LIGHT1, GL_POSITION, light_position2);

   	glutSwapBuffers(); //swap the buffers

}

void keyboard (unsigned char key, int x, int y)
{
    switch (key)
    {
        case 'q':
            glutDestroyWindow("Interactive drawing");
            exit (0);
            break;
        case 'Q':
            glutDestroyWindow("Interactive drawing");
            exit (0);
            break;
        case 'j':
            if (lightSwitch == 1){
                light1X = light1X + 50;
            }
            if (lightSwitch == 0){
                light2X = light2X + 50;
            }
            break;
        case 'l':
            if (lightSwitch == 1){
                light1X = light1X - 50;
            }
            if (lightSwitch == 0){
                light2X = light2X - 50;
            }
            break;
        case '0':
            lightSwitch = 0;
            break;
        case '1':
            lightSwitch = 1;
            break;
        default:
            break;
    }
}


int main(int argc,char **argv)
{
	glutInit(&argc,argv);
	glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGB|GLUT_DEPTH);
	glutInitWindowSize(800,450);
	glutInitWindowPosition(20,20);
	glutCreateWindow("ObjLoader");
	glutReshapeFunc(reshape);
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
	glutIdleFunc(display);
    
    //Load in the budhha object
    loadObj("data/buddha.obj");
    
    GLfloat position0[] = {1.0, 0.0, 0.0, 1.0};
    GLfloat diffuse0[] = {1.0, 0.0, 0.0, 1.0};
    GLfloat specular0[] = {1.0, 1.0, 1.0, 1.0};
    GLfloat ambient0[] = {0.1, 0.1, 0.1, 1.0};
    
    GLfloat position1[] = {-1.0, 0.0, 0.0, 1.0};
    GLfloat diffuse1[] = {0.0, 0.0, 1.0, 1.0};
    GLfloat specular1[] = {1.0, 1.0, 1.0, 1.0};
    GLfloat ambient1[] = {0.1, 0.1, 0.1, 1.0};
    
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHT1);
    
    glLightfv(GL_LIGHT0, GL_POSITION, position0);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse0);
    glLightfv(GL_LIGHT0, GL_SPECULAR, specular0);
    glLightfv(GL_LIGHT0, GL_AMBIENT, ambient0);
    glLightfv(GL_LIGHT1, GL_POSITION, position1);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, diffuse1);
    glLightfv(GL_LIGHT1, GL_SPECULAR, specular1);
    glLightfv(GL_LIGHT1, GL_AMBIENT, ambient1);
    
    
    glEnable(GL_NORMALIZE);
    glEnable(GL_RESCALE_NORMAL);
    glEnable(GL_COLOR_MATERIAL);
    glShadeModel(GL_SMOOTH);
    glutMainLoop();
	return 0;
}
