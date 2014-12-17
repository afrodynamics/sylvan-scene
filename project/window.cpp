#include <iostream>

#ifndef __APPLE__
#include "glee.h"
#include <GL/glut.h>
#else
#include <GLUT/glut.h>
#endif

#include <string>
#include <sstream>
#include <math.h>
#include "main.h"
#include "Window.h"
#include "SpotLight.h"
#include "SkyBox.h"
#include "Terrain.h"
#include "BezierPatch.h"
#include "Cylinder.h"
#include "Cone.h"
#include "Material.h"
#include "BezierCurve.h"
#include "BezierSpline.h"
#include "Particles.h"
#include "Util.h"

using namespace std;

int Window::width  = 512;   // set window width in pixels here
int Window::height = 512;   // set window height in pixels here
double Window::deltaTime = 0;  // milliseconds elapsed between frames
double Window::fov = 60.0;  // perspective frustum vertical field of view in degrees
int Window::currentFPS = 60; // we hope
int Window::tdepth = 5;

namespace Scene
{

    // Scene Graph & Camera objects 
    Camera *camera = nullptr;
    MatrixTransform *world = nullptr; // Top level of the scene graph
    ObjModel *eagle = nullptr;
    PointLight *ptLight;
    SkyBox *sky;
    vector<Node*> nodeList;
    vector<Robot*> robotList;
    vector<Plane> frustumList = vector<Plane>(6); // Culling doesn't work
    Shader *shader;
    BezierPatch *waterPatch;
    BezierSpline* eagleTrajectory;
    Terrain *terrain;
    MatrixTransform *terrainScale, *skyBoxScale, *terrainTranslate, *treeTranslate;
    TreeGen *tgen;
    Tree *tree;
    Particles *snow;

    // Boolean Flahs
    bool showBounds = false;
    bool showFps = false;
    bool showEagleTrajectory = true;
    bool shaderOn = false;
    bool fullscreen = false;
    bool isSnowing = false;
    bool stopEagle = false;
    double znear = 1.0;
    double zfar = 1000; //1000.0;
    float eaglePos = 0.0;

    GLuint textures[7];
    GLuint sky_left, sky_right, sky_up, sky_down, sky_front, sky_back;

    // Create a new robot at the given position in world coordinates
    Robot* createRobot(Vector3& pos) {
        double x, y, z;
        x = pos.getX();
        y = pos.getY();
        z = pos.getZ();
        Robot *roboPtr = new Robot(x, y, z);
        robotList.push_back(roboPtr);
        return roboPtr;
    };

    // Initialize pointers with defaults
    void setup() {

        // Scene setup
        camera = new Camera(
            Vector3(0, -50, 100), Vector3(0, -50, 0), Vector3(0, 1, 0)
        );

        world = new MatrixTransform(); // Top level of the scene graph
        waterPatch = new BezierPatch();
        terrain = new Terrain(); // Procedural generator FTW
        Matrix4 scl = Matrix4::scale(125,125,125);
        Matrix4 skyScale = Matrix4::scale(250,250,250);
        snow = new Particles(125, 125, 125);
        BezierCurve curve1 = BezierCurve(Vector4(0, 75, 0, 1), Vector4 (-50, 10, 30, 1), Vector4(-100, 60, 0, 1), Vector4(-10, 0, 10, 1));
        BezierCurve curve2 = BezierCurve(Vector4(-10, 0, 10, 1), Vector4 (0, -25, 100, 1), Vector4(5, -5, 20, 1), Vector4(100, 10, 0, 1));
        BezierCurve curve3 = BezierCurve(Vector4(100, 10, 0, 1), Vector4(75, 20, -20, 1), Vector4(10, 75, 10, 1), Vector4(0, 75, 0, 1));
        eagleTrajectory = new BezierSpline();
        eagleTrajectory->push(curve1);
        eagleTrajectory->push(curve2);
        eagleTrajectory->push(curve3);
        eagleTrajectory->closeLoop();
        eagle = new ObjModel();
        tgen = new TreeGen(5,5,35,5);
        tgen->initialize();

        Matrix4 terTransMtx = Matrix4::translate(0.0,-125.0,0.0);
        Matrix4 cylTransMtx = Matrix4::translate(0.0,-100.0,0.0);
        treeTranslate = new MatrixTransform(cylTransMtx);
        terrainScale = new MatrixTransform( scl );
        skyBoxScale = new MatrixTransform( skyScale );
        terrainTranslate = new MatrixTransform( terTransMtx );
        ptLight = new PointLight(0, 100, 0);
        ptLight->setAmbient(0.25, 0.25, 0.25, 1);
        ptLight->setSpecular(.5, .5, .5, 1);
        ptLight->setDiffuse(.35, .35, .35, 0);
        ptLight->enableMat(true); // Turn on material for the light

        // Load a bind the textures
        
        sky = new SkyBox(); // Still don't have a good texture class here

        glGenTextures(7, textures); // This needs to be made OOP

        sky->right = Window::loadPPM("tex/right1.ppm",1024,1024,0);
        sky->left = Window::loadPPM("tex/left1.ppm",1024,1024,1);
        sky->front = Window::loadPPM("tex/front1.ppm",1024,1024,2);
        sky->back = Window::loadPPM("tex/back1.ppm",1024,1024,3);
        sky->top = Window::loadPPM("tex/top1.ppm",1024,1024,4);
        sky->base = Window::loadPPM("tex/base1.ppm",1024,1024,5);

        snow->textureID = Window::loadPPM("tex/snow.ppm", 1024, 1024, 6);

        /*  Assign texture locations into the vertex & fragment shader  */
        // This did not belong inside of loadPPM
        
        shader = new Shader("shaders/reflection_map.vert", "shaders/reflection_map.frag", true);
        shader->printLog("Shader Compiler: ");
        GLuint texLoc;
        for (int texID = 0; texID < 6; texID++) {
            switch (texID) {
                case 0: texLoc = glGetUniformLocationARB(Scene::shader->pid,"right"); break;
                case 1: texLoc = glGetUniformLocationARB(Scene::shader->pid,"left"); break;
                case 2: texLoc = glGetUniformLocationARB(Scene::shader->pid,"front"); break;
                case 3: texLoc = glGetUniformLocationARB(Scene::shader->pid,"back"); break;
                case 4: texLoc = glGetUniformLocationARB(Scene::shader->pid,"top"); break;
                case 5: texLoc = glGetUniformLocationARB(Scene::shader->pid,"base"); break;
            }
            glUniform1i( texLoc, 0 ); // The zero here determines what kind of texture this is
        }

        // Now we only have 1 scene graph, less hacky than before
        world->mtx.identity();
        world->addChild( ptLight );
        world->addChild( terrainTranslate ); 
        world->addChild( skyBoxScale );
        world->addChild( treeTranslate );
        world->addChild( eagleTrajectory );
        terrainTranslate->addChild( terrainScale );
        terrainScale->addChild( terrain ); // water patch
        skyBoxScale->addChild( sky );

        // Initiate tree
        tree = tgen->generate(); // Any number greater than 5 results in 2 FPS!!!
        treeTranslate->addChild(tree);

        // ObjModels are scene graph compatible
        eagle->cppParseFile("objectmodels/eagle.obj");
        eagle->setMaterial(Vector4(0.35, 0.25, 0.2, 1), Vector4(0.5, 0.5, 0.5, 1), Vector4(0, 0, 0, 1), Vector4(0.5, 0.5, 0.5, 1));
        
        // Affix shaders to individual scene graph nodes
        terrain->setShader( shader ); // This patch should have a shader

    };
    // Deallocate all kinds of stuff
    void dealloc() {
        for (auto iter = nodeList.begin(); iter != nodeList.end(); iter++) {
            delete *iter;
            *iter = nullptr;
        }
        delete eagle;
        delete eagleTrajectory;
        delete ptLight; 
        delete tgen;
        delete tree;
        delete waterPatch;
        delete terrainScale, terrainTranslate, skyBoxScale;
        delete sky;
        delete terrain;
        delete world;
        sky = nullptr;
        waterPatch = nullptr; terrainScale = terrainTranslate = nullptr;
        eagle = nullptr;
        ptLight = nullptr;
        cerr << "Dellocating memory..." << endl;
    };
};


//----------------------------------------------------------------------------
// Callback method called when system is idle.
void Window::idleCallback()
{

    static long frame = 0, time, timebase = 0;

    // Call draw on the Scene
    displayCallback(); // call display routine to show the cube

    // Update the particle system
    if (Scene::snow != nullptr)
      Scene::snow->update();

    // Update the eagle's position along the Bezier spline
    if( !Scene::stopEagle ) {
        if ( Scene::eaglePos < 0.9975 ) {
            Scene::eaglePos += 0.0025;
        }
        else {
            Scene::eaglePos = 0.0;
        }
    }

    /* FPS Counter courtesy of Lighthouse3D */

    frame++; // Increment the number of frames we've drawn
    time = glutGet(GLUT_ELAPSED_TIME);

    // Reset the counter and print the FPS if our timer has lasted longer than a 
    // second.
    if (time - timebase > 1000) {
        // Always calculate delta time
        deltaTime = (time - timebase) - 1000;
        Window::currentFPS = frame * 1000 / (time - timebase);
        timebase = time; // Set timebase to the current time
        frame = 0; // Reset frame counter
    }
};

//----------------------------------------------------------------------------
// Callback method called by GLUT when graphics window is resized by the user
void Window::reshapeCallback(int w, int h)
{

  glViewport(0, 0, w, h);  // set new viewport size
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective( Window::fov, 
                  double(width)/(double)height, 
                  Scene::znear, 
                  Scene::zfar);  // set perspective projection viewing frustum
  glMatrixMode(GL_MODELVIEW);

  // Update Window::...
  Window::width = w;
  Window::height = h;

};

//----------------------------------------------------------------------------
// Callback method called by GLUT when window readraw is necessary or when glutPostRedisplay() was called.
void Window::displayCallback()
{

  // No reason to allocate a new mat4 every call, just update it if necessary
  static Matrix4 invCam = Matrix4();
  static Matrix4 invCamRot = Matrix4();

  printGLError("GL Error in displayCallback: "); // Print any GL errors we might get

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);  // clear color and depth buffers
  glMatrixMode(GL_MODELVIEW);  // make sure we're in Modelview mode

  invCam.identity();
  invCamRot.identity();
  if ( Scene::camera != nullptr && Scene::world != nullptr ) {
    invCam = Scene::camera->getGLMatrix();
    invCamRot = invCam * Scene::world->getMatrix();
  }
  else
    return; // With no camera matrix, there's no point trying to draw

  // Draw our scene so long as it is actually in memory
  if ( Scene::camera && Scene::world ) {
    if ( Scene::isSnowing ) {
        Scene::snow->render();
    }

    double angle = Scene::eagleTrajectory->getAngle(Scene::eaglePos); // this could be precomputed
    angle += 90; //eagle should be facing to the right by default
    Vector4 position = invCamRot * Scene::eagleTrajectory->calcPoint(Scene::eaglePos);
    Matrix4 eagleMatrix = /* invCamRot **/ Matrix4::translate(position.getX(), position.getY(), position.getZ()) * Matrix4::rotY(angle);

    glColor3f(0.35, 0.25, 0.2);
    Scene::eagle->draw(eagleMatrix);

    // Enable environment mapping on our patch
    if ( Scene::shaderOn && Scene::terrain != nullptr ) {
        Scene::terrain->enableShader( Scene::shaderOn );
    }
    else if ( Scene::terrain != nullptr ) {
        Scene::terrain->enableShader( Scene::shaderOn );
    }
      
    // Draw the scene graph
    Scene::world->draw( invCam );
  }

  // Show FPS on screen if the flag is set
  if ( Scene::showFps ) {

    // Build the string
    stringstream fpsCounter;
    double const aspect = (double)Window::width/(double)Window::height;
    fpsCounter << "FPS: " << Window::currentFPS << endl;
    string built = fpsCounter.str();

    // Clear the zbuffer, set matrix mode to projection
    glClear(GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D(0.0, Window::width, 0.0, Window::height );

    // Push an identity matrix onto the stack
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    // OpenGL's bottom left corner is (0,0) in screen coordinates
    glRasterPos2i( 0, Window::height - 24 );
    for( int i = 0; i < built.size(); i++ ) {
      glutBitmapCharacter( GLUT_BITMAP_TIMES_ROMAN_24, built.at(i) );
    }

    // Pop the extra matrices we created off of their respective stacks
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix(); // pop GL_MODELVIEW
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();

  }

  glFlush();  
  glutSwapBuffers();

};

//----------------------------------------------------------------------------
// Callback method called by GLUT when keys are pressed
//    char * key     - the key pressed
//    int x          - mouse x position
//    int y          - mouse y position
//    
//    https://www.opengl.org/resources/libraries/glut/spec3/node49.html
//    See main.cpp line 56
void Window::keyboardCallback(unsigned char key, int x, int y) {

  Matrix4 transformation;
  transformation.identity(); // Make sure the Matrix isn't utter garbage

  switch (key) {
  case 27:
      // Close gracefully and dealloc stuff.
      Scene::dealloc(); 
      exit(0); 
      break;
  case 'b':
      Scene::showBounds = !Scene::showBounds;
      Scene::world->showBoundingBox(Scene::showBounds);
      cerr << "Bounding spheres are " << (Scene::showBounds ? "on" : "off") << endl;
      break;
  case 'e':
      Scene::shaderOn = !Scene::shaderOn;
      cerr << "Shader is " << (Scene::shaderOn == true ? "on" : "off") << " for the terrain." << endl;
      break;
  case 'f':
      Scene::showFps = !Scene::showFps;
      cerr << "FPS counter is " << (Scene::showFps ? "on" : "off") << endl;
      break;

  // Regenerate the terrain/trees
  case 't':
      Scene::terrain->generate();
      cout << "New terrain generated!" << endl;
      break;
  case 'g':
      cerr << Scene::tgen->genString(3) << endl;
      break;
  case 'G':
      delete Scene::tree;
      Scene::treeTranslate->removeChild(Scene::tree);
      Scene::tree = Scene::tgen->generate(tdepth);
      Scene::treeTranslate->addChild(Scene::tree);
      break;

  // Allow wasd movement control of camera
  case 'w':
    Scene::camera->moveForward();
    break;
  case 'a':
    Scene::camera->moveLeft();
    break;
  case 's':
    Scene::camera->moveBackward();
    break;
  case 'd':
    Scene::camera->moveRight();
    break;
  case ' ':
    Scene::camera->moveUp();
    break;
  case 'z':
    Scene::camera->moveDown();
    break;
  // Allow WASD rotation control of camera
  case 'W':
    Scene::camera->lookAt(1, 1);
    break;
  case 'A':
    Scene::camera->lookAt(1, 0);
    break;
  case 'S':
    Scene::camera->lookAt(0, 1);
    break;
  case 'D':
    Scene::camera->lookAt(0, 0);
    break;
  case 'o':
      transformation = Matrix4::rotY(-5.0);
      Scene::world->getMatrix().transformWorld(transformation);
      break;
  case 'O':
      transformation = Matrix4::rotY(+5.0);
      Scene::world->getMatrix().transformWorld(transformation);
      break;
  case 'r':
      Scene::world->getMatrix().identity();
      Scene::camera->reset();
	  break;

  // On/Off Toggles
    
  case '1':
    Scene::isSnowing = !Scene::isSnowing;
    cerr << (Scene::isSnowing ? "It is" : "It is not") << " snowing" << endl;
    break;
  case '2':
      Scene::eaglePos = 0.0;
      break;
  case '3':
      Scene::stopEagle = !Scene::stopEagle;
      break;
  case '4':
      Scene::showEagleTrajectory = !Scene::showEagleTrajectory; 
      Scene::eagleTrajectory->drawSpline = Scene::showEagleTrajectory;
      break;
  default:
    cerr << "Pressed: " << key << endl;
    break;
  }
};

/** Load a ppm file from disk, then loads it into OpenlGL.
 @input filename The location of the PPM file.  If the file is not found, an error message
        will be printed and this function will return 0
 @input width This will be modified to contain the width of the loaded image, or 0 if file not found
 @input height This will be modified to contain the height of the loaded image, or 0 if file not found
 @return Returns the RGB pixel data as interleaved unsigned chars (R0 G0 B0 R1 G1 B1 R2 G2 B2 .... etc) or 0 if an error ocured

 Stolen from Jurgen
**/
GLuint Window::loadPPM(const char *filename, int width, int height, int texID) {

    // GLuint texture[1];     // storage for one texture
    const int BUFSIZE = 128;
    FILE* fp;
    unsigned int read;
    unsigned char* rawData; // texture pixel data
    char buf[3][BUFSIZE];
    char* retval_fgets;
    size_t retval_sscanf;

    if ( (fp=fopen(filename, "rb")) == NULL)
    {
        std::cerr << "error reading ppm file, could not locate " << filename << std::endl;
        width = 0;
        height = 0;
        return NULL;
    }

    // Read magic number:
    retval_fgets = fgets(buf[0], BUFSIZE, fp);

    // Read width and height:
    do
    {
        retval_fgets=fgets(buf[0], BUFSIZE, fp);
    } while (buf[0][0] == '#');
    retval_sscanf=sscanf(buf[0], "%s %s", buf[1], buf[2]);
    width  = atoi(buf[1]);
    height = atoi(buf[2]);

    // Read maxval:
    do
    {
      retval_fgets=fgets(buf[0], BUFSIZE, fp);
    } while (buf[0][0] == '#');

    // Read image data:
    rawData = new unsigned char[width * height * 3];
    read = fread(rawData, width * height * 3, 1, fp);
    fclose(fp);
    if (read != 1)
    {
        std::cerr << "error parsing ppm file, incomplete data" << std::endl;
        delete[] rawData;
        width = 0;
        height = 0;
        return NULL;
    }

    // Load this file into an OpenGL texture object!
  
    // Create ID for texture
    // glGenTextures(1, &texture[0]);   

    // Set this texture to be the one we are working with
    //glBindTexture(GL_TEXTURE_2D, texture[0]);

    printGLError("GL Error in loadPPM: "); // Print a GL error if one occurred
        
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, Scene::textures[texID]); // This is the critical line!

    // Generate the texture
    glTexImage2D(GL_TEXTURE_2D, 0, 3, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, rawData);

    // Set bi-linear filtering for both minification and magnification
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); 
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Make sure no bytes are padded:
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    // Select GL_MODULATE to mix texture with polygon color for shading:
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

    // Use bilinear interpolation:
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    return Scene::textures[texID];
}

//----------------------------------------------------------------------------
// Callback method called by GLUT when keys are pressed
//    int key        - the key pressed
//    int x          - mouse x position
//    int y          - mouse y position
//    
//    https://www.opengl.org/resources/libraries/glut/spec3/node54.html
//    See main.cpp line 56
void Window::functionKeysCallback(int key, int x, int y) {
  switch (key) {
  case GLUT_KEY_F1:
      Scene::fullscreen = !Scene::fullscreen;
      if ( Scene::fullscreen ) {
          glutFullScreen();
      }
      else {
        glutReshapeWindow(640,480);
        // glutRepositionWindow(0,0)
      }
      break;
  case GLUT_KEY_F2:
    tdepth = 2;
    break;
  case GLUT_KEY_F3:
    tdepth = 3;
    break;
  case GLUT_KEY_F4:
    tdepth = 4;
    break;
  case GLUT_KEY_F5:
    tdepth = 5;
    break;
  case GLUT_KEY_F6:
    tdepth = 6;
    break;
  case GLUT_KEY_F7:
    tdepth = 7;
    break;
  default:
      cout << "Pressed a function key or trigged glutSpecialFunc" << endl;
      break;
  }
};

// Checks to see if a GL error has occurred, and if so it prints out
// the gluErrorString
void Window::printGLError(string str) {
    int err = glGetError();
    if (err != GL_NO_ERROR) cerr << str << gluErrorString( err ) << endl;
};
