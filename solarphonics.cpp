#define GLEW_STATIC
#include <utility.h>
#include <GLFW/glfw3.h>
#include <string>
#include <cmath>
#include <m3d.h>
#include <stack>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#define PI 3.1415

#undef GLFW_KEY_Y
#undef GLFW_KEY_Z

#define GLFW_KEY_Y 90
#define GLFW_KEY_Z 89


/*!
 *  THE SP..
 */

GLuint windowWidth = 600;
GLuint windowHeight = 600;

GLfloat max_ang = PI * 2;
GLfloat ang_inc = PI / 8.f;
GLfloat rot_inc = PI / 36.f;

float deltaTime = .0f;
float lastFrame = .0f;
float timeFactor;

glm::vec3 cameraPos;
glm::vec3 cameraFront;
glm::vec3 cameraUp;

bool bCull = true, bDepth = true, bOutline = false;
GLfloat x_rot = .0f;
GLfloat y_rot = .0f;
GLfloat z_rot = .0f;

GLfloat tetX = .0f;
GLfloat tetY = .0f;
GLfloat tetZ = -100.0f;

GLfloat cubeX = 100.0f;
GLfloat cubeY = .0f;
GLfloat cubeZ = .0f;

GLfloat octX = -100.0f;
GLfloat octY = .0f;
GLfloat octZ = .0f;

float viewX = .0f, viewY = .0f, viewZ = .0f;

std::stack<M3DMatrix44f> matrixStack;

glm::mat4 viewMat;
glm::mat4 projMat;
glm::mat4 modelMat;
glm::mat4 modelViewMat;
glm::mat4 rotationMat;
glm::mat4 translateMat;
glm::mat4 lookAtMat;
glm::mat4 perspectiveMat;
glm::mat4 mvpMat;

float speed = 1000.f;

GLfloat deg_to_rad(GLfloat deg)
{
    return deg * PI / 180.f;
}

GLfloat rad_to_deg(GLfloat rad)
{
    return rad * 180.f / PI;
}

void changeSize(GLFWwindow* window, int w, int h)
{
    GLfloat nRange = 100.f;
    if(!h) h = 1;
    glViewport(0, 0, w, h);
    float aspect = float(w)/h;

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    perspectiveMat = glm::perspective(1.0472f, aspect, .1f, 1000.f);
    glLoadMatrixf(glm::value_ptr(perspectiveMat));
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void setupRC()
{
    glClearColor(.0f, .0f, .0f, 1.f);
    glColor3f(.0f, 1.f, .0f);
    glShadeModel(GL_FLAT);
    glFrontFace(GL_CW);
}




void drawTet()
{
     glFrontFace(GL_CCW);
    GLfloat x, y, angle;
    int iPivot = 1;
    float a = 50.f;
    float hz = tan(deg_to_rad(30)) * a/2.f;
    float h_oz = sqrt(3)/2 * a;
    float hy = tan(deg_to_rad(60)) * hz * 2.f;

        glColor3f(.0f, 1.f, .0f);
    glBegin(GL_TRIANGLES);
        glVertex3f(-a/2, 0.f, 0.f);
        glVertex3f( 0.f, hy, hz);
        glVertex3f( a/2, 0.f, .0f);
    glEnd();

    glColor3f(1.f, 0.f, 0.f);
    glBegin(GL_TRIANGLES);
        glVertex3f( -a/2, 0.f, 0.f);
        glVertex3f(0.f, 0.f, h_oz);
        glVertex3f( 0.f, hy, hz);
    glEnd();

    glColor3f(0.f, 0.f, 1.f);
    glBegin(GL_TRIANGLES);
        glVertex3f( a/2, 0.f, 0.f);
        glVertex3f( 0.f, hy, hz);
        glVertex3f(0.f, 0.f, h_oz);
    glEnd();

    glFrontFace(GL_CW);
    glColor3f(1.f, 1.f, 0.f);
    glBegin(GL_TRIANGLES);
        glVertex3f(-a/2, 0.f, 0.f);
        glVertex3f( 0.f, 0.f, h_oz);
        glVertex3f( a/2, 0.f, .0f);
    glEnd();
}



void drawCube()
{
    float a = 50.f;
    glFrontFace(GL_CW);
    ///front = green..
    glColor3f(0.f, 1.f, 0.f);
    glBegin(GL_TRIANGLE_STRIP);
        glVertex3f(-a/2.f, -a/2.f, 0.f);
        glVertex3f(-a/2.f,  a/2.f, 0.f);
        glVertex3f( a/2.f,  -a/2.f, 0.f);
        glVertex3f( a/2.f,  a/2.f, 0.f);
    glEnd();

    ///right = red..
    glColor3f(1.f, 0.0f, .0f);
    glBegin(GL_TRIANGLE_STRIP);
        glVertex3f(a/2.f, -a/2.f, .0f);
        glVertex3f(a/2.f, a/2.f,  .0f);
        glVertex3f(a/2.f, -a/2.f, -a);
        glVertex3f(a/2.f, a/2.f,  -a);
    glEnd();

    ///left = orange..
    glColor3f(1.0f, .5f, .1f);
    glBegin(GL_TRIANGLE_STRIP);
        glVertex3f(-a/2.f, -a/2.f, -a);
        glVertex3f(-a/2.f, a/2.f,  -a);
        glVertex3f(-a/2.f, -a/2.f, .0f);
        glVertex3f(-a/2.f, a/2.f,  .0f);
    glEnd();

    ///back = blue..
    glColor3f(0.0f, 0.0f, 1.f);
    glBegin(GL_TRIANGLE_STRIP);
        glVertex3f( a/2.f,  -a/2.f, -a);
        glVertex3f( a/2.f,  a/2.f, -a);
        glVertex3f(-a/2.f, -a/2.f, -a);
        glVertex3f(-a/2.f,  a/2.f, -a);
    glEnd();

    ///top = white..
    glColor3f(1.f, 1.f, 1.f);
    glBegin(GL_TRIANGLE_STRIP);
        glVertex3f(-a/2.f, a/2.f, 0.f);
        glVertex3f(-a/2.f, a/2.f, -a);
        glVertex3f( a/2.f, a/2.f, .0f);
        glVertex3f( a/2.f, a/2.f, -a);
    glEnd();

    ///bottom = yellow..
    glColor3f(1.f, 1.f, 0.f);
    glBegin(GL_TRIANGLE_STRIP);
        glVertex3f( a/2.f, -a/2.f, 0.f);
        glVertex3f( a/2.f, -a/2.f, -a);
        glVertex3f(-a/2.f, -a/2.f, .0f);
        glVertex3f(-a/2.f, -a/2.f, -a);
    glEnd();

}

void drawOct()
{
    float h = 40.f;
    float a = 25.f;

    ///top pyramid..
    glColor3f(1.f, 0.f, 0.f);
    glBegin(GL_TRIANGLES);
        glVertex3f(-a, .0f, 0.f);
        glVertex3f( 0.f, h, -a);
        glVertex3f( a, .0f, 0.f);
    glEnd();

    glColor3f(.0f, 1.f, .0f);
    glBegin(GL_TRIANGLES);
        glVertex3f(a, 0.f, 0.f);
        glVertex3f( 0.f, h, -a);
        glVertex3f(a, 0.f, -2* a);
    glEnd();

    glColor3f(.0f, .0f, 1.f);
    glBegin(GL_TRIANGLES);
        glVertex3f(a, 0.f, -2.f * a);
        glVertex3f( 0.f, h, -a);
        glVertex3f(-a, 0.f, -2.f * a);
    glEnd();

    glColor3f(1.f, 1.f, .0f);
    glBegin(GL_TRIANGLES);
        glVertex3f(-a, 0.f, -2.f * a);
        glVertex3f( 0.f, h, -a);
        glVertex3f(-a, 0.f, 0.f);
    glEnd();

    ///bottom pyramid..
    glColor3f(0.f, 1.f, 1.f);
    glBegin(GL_TRIANGLES);
        glVertex3f( a, .0f, 0.f);
        glVertex3f( 0.f, -h, -a);
        glVertex3f(-a, .0f, 0.f);
    glEnd();

    glColor3f(1.0f, 0.f, 1.0f);
    glBegin(GL_TRIANGLES);
        glVertex3f(a, 0.f, -2* a);
        glVertex3f( 0.f, -h, -a);
        glVertex3f(a, 0.f, 0.f);
    glEnd();

    glColor3f(.5f, .5f, .5f);
    glBegin(GL_TRIANGLES);
        glVertex3f(-a, 0.f, -2.f * a);
        glVertex3f( 0.f, -h, -a);
        glVertex3f(a, 0.f, -2.f * a);
    glEnd();

    glColor3f(1.f, 1.f, 1.0f);
    glBegin(GL_TRIANGLES);
        glVertex3f(-a, 0.f, 0.f);
        glVertex3f( 0.f, -h, -a);
        glVertex3f(-a, 0.f, -2.f * a);
    glEnd();
}

void handleRotation()
{
    glm::vec3 front;

    front.x = cos(y_rot) * cos(x_rot);
    front.y = sin(x_rot);
    front.z = sin(y_rot) * cos(x_rot);
    cameraFront = glm::normalize(front);
}

void keycallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if(key == GLFW_KEY_X && action)
    {
        x_rot += rot_inc;
        if(x_rot >= max_ang)
            x_rot -= max_ang;
        handleRotation();
    }
    if(key == GLFW_KEY_Y && action)
    {
        y_rot += rot_inc;
        if(y_rot >= max_ang)
            y_rot -= max_ang;
        handleRotation();
    }
    if(key == GLFW_KEY_Z && action)
    {
        z_rot += rot_inc;
        if(z_rot >= max_ang)
            z_rot -= max_ang;

        handleRotation();
    }
    if(key == GLFW_KEY_I && action)
    {
        x_rot -= rot_inc;
        if(x_rot <= .0f)
            x_rot += max_ang;

        handleRotation();
    }
    if(key == GLFW_KEY_O && action)
    {
        y_rot -= rot_inc;
        if(y_rot <= .0f)
            y_rot += max_ang;
        handleRotation();
    }
    if(key == GLFW_KEY_P && action)
    {
        z_rot -= rot_inc;
        if(z_rot <= .0f)
            z_rot += max_ang;
        handleRotation();
    }

    if(key == GLFW_KEY_SPACE && action)
    {
        //restores the default matrix..
        cameraPos = glm::vec3(0, 0, 0);
    }

    if(key == GLFW_KEY_A && action)
    {
        cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * speed * deltaTime;
    }

    if(key == GLFW_KEY_W && action)
    {
        cameraPos += speed * cameraFront * deltaTime;
    }

    if(key == GLFW_KEY_S && action)
    {
        cameraPos -= speed * cameraFront * deltaTime;
    }

    if(key == GLFW_KEY_D && action)
    {
        cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * speed * deltaTime;
    }

    if(key == GLFW_KEY_F && action)
    {
        //toggles face culling..
        bCull = !bCull;
        std::cout << "[F] CULL FACE:                            " << std::boolalpha << bCull << std::endl;
        std::cout << "[T] DEPTH TEST:                           " << bDepth << std::endl;
        std::cout << "[R] OUTLINE (TRUE = FILL, FALSE = WF):    " << bOutline << std::noboolalpha << std::endl << std::endl;
    }

    if(key == GLFW_KEY_T && action)
    {
        //toggled depth testing..
        bDepth = !bDepth;
        std::cout << "[F] CULL FACE:                            " << std::boolalpha << bCull << std::endl;
        std::cout << "[T] DEPTH TEST:                           " << bDepth << std::endl;
        std::cout << "[R] OUTLINE (TRUE = FILL, FALSE = WF):    " << bOutline << std::noboolalpha << std::endl << std::endl;
    }

    if(key == GLFW_KEY_R && action)
    {
        //toggles either the background to be filled or wireframed..
        bOutline = !bOutline;
        std::cout << "[F] CULL FACE:                            " << std::boolalpha << bCull << std::endl;
        std::cout << "[T] DEPTH TEST:                           " << bDepth << std::endl;
        std::cout << "[R] OUTLINE (TRUE = FILL, FALSE = WF):    " << bOutline << std::noboolalpha << std::endl << std::endl;
    }
}

void printMatrix(M3DMatrix44f mat)
{
    std::cout
    << mat[0] << " "
    << mat[1] << " "
    << mat[2] << " "
    << mat[3] << std::endl;

    std::cout << mat[4] << " " << mat[5] << " " << mat[6] << " " << mat[7] << std::endl;

    std::cout << mat[8] << " " << mat[9] << " " << mat[10] << " " << mat[11] << std::endl;
    std::cout << mat[12] << " " << mat[13] << " " << mat[14] << " " << mat[15] << std::endl; std::cout << std::endl;
}

float identity[16] =
{
    1.f, .0f,  .0f,  .0f,
    0.f, 1.f,  .0f,  .0f,
    0.f, .0f, 1.0f,  .0f,
    0.f, .0f,  .0f, 1.0f
};

void renderScene()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    if(bCull)
        glEnable(GL_CULL_FACE);
    else
        glDisable(GL_CULL_FACE);

    if(bDepth)
        glEnable(GL_DEPTH_TEST);
    else
        glDisable(GL_DEPTH_TEST);

    if(bOutline)
        glPolygonMode(GL_BACK, GL_POINT);
    else
        glPolygonMode(GL_BACK, GL_FILL);
    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glPushMatrix();

    ///view matrix..
    viewMat = glm::mat4(1.f);
    viewMat *= glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);

    ///model matrix..
    modelMat = glm::mat4(1.f);
    modelMat *= glm::translate(glm::mat4(1.f), glm::vec3(cubeX, cubeY, cubeZ));

    modelViewMat = viewMat * modelMat;

    glMatrixMode(GL_MODELVIEW);
    glLoadMatrixf(glm::value_ptr(modelViewMat));

    glm::mat4 model1 = glm::mat4(1.f);
    model1 *= glm::translate(glm::mat4(1.f), glm::vec3(tetX, tetY, tetZ));
    modelViewMat = viewMat * model1;
    drawOct();
    glMatrixMode(GL_MODELVIEW);
    glLoadMatrixf(glm::value_ptr(modelViewMat));

    drawTet();
    glm::mat4 model2 = glm::mat4(1.f);
    model2 *= glm::translate(glm::mat4(1.f), glm::vec3(octX, octY, octZ));
    modelViewMat = viewMat * model2;

    glMatrixMode(GL_MODELVIEW);
    glLoadMatrixf(glm::value_ptr(modelViewMat));
    drawCube();

    glPopMatrix();
}

int main()
{
GLFWwindow* window;
    if(!glfwInit()) return -1;
    window = glfwCreateWindow(windowWidth, windowHeight, "solarphonics", NULL, NULL);
    if(!window) {glfwTerminate(); return -1;}
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, changeSize);
    glfwSetKeyCallback(window, keycallback);

    GLenum err = glewInit();

    cameraFront.x = cos(y_rot) * cos(x_rot);
    cameraFront.y = sin(x_rot);
    cameraFront.z = sin(y_rot) * cos(x_rot);
    cameraFront = glm::normalize(cameraFront);
    cameraPos = glm::vec3(.0f, .0f, 0.f);
    cameraUp = glm::vec3(.0f, 1.f, .0f);

    if(err != GLEW_OK)
    {
        std::cerr << "ERROR: " << glewGetErrorString(err) << std::endl;
        glfwTerminate();
        return -1;
    }

    changeSize(window, windowWidth, windowHeight);

    while(!glfwWindowShouldClose(window))
    {
        float currentFrame = (float) glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        glfwPollEvents();

        setupRC();
        renderScene();
        glfwSwapBuffers(window);
    }
    return 0;
}
