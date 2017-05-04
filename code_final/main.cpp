#include "headers.h"
#include "vao.h"
#include "block.h"
#include "brick.h"
#include "canon.h"
#include "bullet.h"
#include "bucket.h"
#include "mirror.h"
#include "main.h"
#include "controls.h"
#include "score.h"

/**************************
 * Customizable functions *
 **************************/

GLuint programID;
glm::mat4 world_model_mat;
const char GameHeading[10] = "Game";
float triangle_rot_dir = 1;
float rectangle_rot_dir = 1;
bool triangle_rot_status = false;
bool rectangle_rot_status = false;
VAO *triangle, *rectangle;
float camera_rotation_angle = 90;
float rectangle_rotation = 0;
float triangle_rotation = 0;
struct GLMatrices Matrices;
int window_width = 700;
int window_height = 700;
float min_x = -4.0f;
float max_x = 4.0f;
float min_y = -4.0f;
float max_y = 4.0f;

map<string, int> obj;

int selected;

set<Brick*> a;
set<Bullet*> b;
vector<Bucket*> d;
set<Mirror*> m;
Canon * c;
Block * border[4];
bool mypause = false;

vector<Brick*> delbrick;
vector<Bullet*> delbullet;
ISoundEngine* engine;

GLFWwindow* window; // window desciptor/handle

void get_coords(double * _x, double * _y)
{
    int width, height;
	glfwGetWindowSize(window, &width, &height);

    double x, y;
    glfwGetCursorPos (window, &x, &y);

    y = y - height/2; y = -ortho*y*2.0f/height;
    x = x - width/2; x = ortho*x*2.0f/width;
    *_x = x;
    *_y = y;
}

void draw ()
{
    // clear the color and depth in the frame buffer
    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // use the loaded shader program
    // Don't change unless you know what you are doing
    glUseProgram (programID);
    // Matrices.view = glm::lookAt( eye, target, up ); // Rotating Camera for 3D
    Matrices.view = glm::lookAt(glm::vec3(0,0,3), glm::vec3(0,0,0), glm::vec3(0,1,0)); // Fixed camera for 2D (ortho) in XY plane

    delbrick.clear();
    for(set<Brick*>::iterator i = a.begin(); i != a.end(); i++)
        if((*i)->is_out())
            delbrick.push_back(*i);
    for(int i=0; i<delbrick.size(); i++)
        a.erase(delbrick[i]);
    delbullet.clear();
    for(set<Bullet*>::iterator i = b.begin(); i != b.end(); i++)
        if((*i)->is_out())
            delbullet.push_back(*i);
    for(int i=0; i<delbullet.size(); i++)
        b.erase(delbullet[i]);

    if(keep_firing)
    {
      float time_now = glfwGetTime();
      if(time_now - last_bullet_time > min_time_between_bullets)
      {
          pair<pair<float, float>, pair<float, float> > st = c->get_bullet_start();
          b.insert(new Bullet(st.first.first, st.first.second, st.second.first, st.second.second));
          engine->play2D("./media/fire.wav");
          last_bullet_time = time_now;
      }
    }

    //delbrick.clear();
    bool br = false;
    for(set<Bullet*>::iterator i=b.begin(); i!=b.end(); i++)
    {
      for(set<Brick*>::iterator j=a.begin(); j!=a.end(); j++)
        if((*i)->has_collided(*j))
        {
            a.erase(*j);
            b.erase(*i);
            score_brick_bullet((*j)->get_color());
            engine->play2D("./media/hit3.wav");
            br = true;
            break;
        }
        if(br)break;
    }

    delbrick.clear();
    for(int i=0; i<d.size(); i++)
        for(set<Brick*>::iterator j=a.begin(); j!=a.end(); j++)
            if(d[i]->has_collided(*j))
            {
                score_brick_bucket(i, (*j)->get_color(), d[i]->get_color());
                delbrick.push_back(*j);
                engine->play2D("./media/catch.wav");
                //cout << d[i]->get_collection() << endl;
            }
    for(int i=0; i<delbrick.size(); i++)
        a.erase(delbrick[i]);

    for(set<Mirror*>::iterator j=m.begin(); j!=m.end(); j++)
        (*j)->update();

    for(set<Bullet*>::iterator i=b.begin(); i!=b.end(); i++)
        for(set<Mirror*>::iterator j=m.begin(); j!=m.end(); j++)
            if((*i)->has_collided(*j))
                (*i)->change_dir(*j);

    if(!mypause)
    for(set<Bullet*>::iterator i=b.begin(); i!=b.end(); i++)
        (*i)->update();

    for(set<Bullet*>::iterator i=b.begin(); i!=b.end(); i++)
        (*i)->draw(world_model_mat);

    if(!mypause)
    for(set<Brick*>::iterator i=a.begin(); i!=a.end(); i++)
        (*i)->update();

    for(set<Brick*>::iterator i=a.begin(); i!=a.end(); i++)
        (*i)->draw(world_model_mat);

    if(funnel_rotation)
        c->update_funnel_rotation(funnel_rotation * canon_funnel_rotation_unit);

    c->set_vy(canon_move*canon_move_speed);
    c->update();
    c->draw(world_model_mat);

    for(int i=0; i<d.size(); i++)
    {
        d[i]->set_vx(bucket_move[i]*bucket_speed);
        d[i]->update();
        d[i]->draw(world_model_mat);
    }

    for(set<Mirror*>::iterator i=m.begin(); i!=m.end(); i++)
        (*i)->draw(world_model_mat);
    for(int i=0; i<4; i++)
        border[i]->draw(world_model_mat);

}

/* Initialize the OpenGL rendering properties */
/* Add all the models to be created here */
void initGL (GLFWwindow* window, int width, int height)
{
    c = new Canon();
    d.push_back(new Bucket(red));
    d.push_back(new Bucket(green));

    m.insert(new Mirror(1.0f, 1.0f, 50.0f));
    m.insert(new Mirror(2.5f, -1.0f, 70.0f));
    m.insert(new Mirror(-1.0f, 1.0f, -60.0f));

    for(int i=0; i<4; i++)
        border[i] = new Block();

    border[0]->initialize(min_x - 0.5f, min_y - 0.5f, 0.0f, 0.0f, 0.5f, max_y - min_y + 1.0f, 0.0f, 0.8f, 0.6f, 0.4f);
    border[1]->initialize(min_x - 0.5f, min_y - 0.5f, 0.0f, 0.0f, max_x - min_x + 1.0f, 0.5f, 0.0f, 0.8f, 0.6f, 0.4f);
    border[2]->initialize(max_x, min_y - 0.5f, 0.0f, 0.0f, 0.5f, max_y - min_y + 1.0f, 0.0f, 0.8f, 0.6f, 0.4f);
    border[3]->initialize(min_x - 0.5f, max_y, 0.0f, 0.0f, max_x - min_x + 1.0f, 0.5f, 0.0f, 0.8f, 0.6f, 0.4f);

    // Create and compile our GLSL program from the shaders
	programID = LoadShaders( "Sample_GL.vert", "Sample_GL.frag" );
	// Get a handle for our "MVP" uniform
	Matrices.MatrixID = glGetUniformLocation(programID, "MVP");


	reshapeWindow (window, width, height);

    // Background color of the scene
	glClearColor (0.4f, 0.4f, 0.9f, 0.0f); // R, G, B, A
	glClearDepth (1.0f);

	glEnable (GL_DEPTH_TEST);
	glDepthFunc (GL_LEQUAL);

    cout << "VENDOR: " << glGetString(GL_VENDOR) << endl;
    cout << "RENDERER: " << glGetString(GL_RENDERER) << endl;
    cout << "VERSION: " << glGetString(GL_VERSION) << endl;
    cout << "GLSL: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << endl;
}

void start_game()
{
    engine = createIrrKlangDevice();

    if (!engine)
    {
        printf("Could not startup engine\n");
        exit(EXIT_FAILURE);
    }
    engine->play2D("./media/background.wav", true);

    world_model_mat = glm::mat4(1.0f);
    obj["red_bucket"] = 0;
    obj["green_bucket"] = 1;
    obj["canon"] = 2;
    obj["brick"] = 3;
    obj["bullet"] = 4;
    obj["mirror"] = 5;
    obj["funnel"] = 6;
    obj["no"] = 10;

    selected = 0;
}

int main (int argc, char** argv)
{
    start_game();

	window = initGLFW(window_width, window_height);
	initGL (window, window_width, window_height);

    double last_update_time = glfwGetTime(), current_time;

    /* Draw in loop */
    while (!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // OpenGL Draw commands
        draw();

        // Swap Frame Buffer in double buffering
        glfwSwapBuffers(window);

        // Poll for Keyboard and mouse events
        glfwPollEvents();

        // Control based on time (Time based transformation like 5 degrees rotation every 0.5s)
        current_time = glfwGetTime(); // Time in seconds
        if ((current_time - last_update_time) >= 2.0) { // atleast 0.5s elapsed since last frame
            // do something every 0.5 seconds ..
            a.insert(new Brick());

            /*delbrick.clear();
            for(set<Brick*>::iterator i = a.begin(); i != a.end(); i++)
                if((*i)->is_out())
                    delbrick.push_back(*i);
            for(int i=0; i<delbrick.size(); i++)
                a.erase(delbrick[i]);
            delbullet.clear();
            for(set<Bullet*>::iterator i = b.begin(); i != b.end(); i++)
                if((*i)->is_out())
                    delbullet.push_back(*i);
            for(int i=0; i<delbullet.size(); i++)
                b.erase(delbullet[i]);*/

            last_update_time = current_time;
        }
    }
    engine->drop();
    glfwTerminate();
//  exit(EXIT_SUCCESS);
}
