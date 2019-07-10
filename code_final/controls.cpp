#include "headers.h"
#include "vao.h"
#include "block.h"
#include "brick.h"
#include "bullet.h"
#include "canon.h"
#include "controls.h"
/* Executed when a regular key is pressed/released/held-down */
/* Prefered for Keyboard events */

int canon_move = 0;
int funnel_rotation = 0;
bool keep_firing = false;
int bucket_move[2] = {0, 0};
float zoom_factor = 0.15f;
float ortho = 4.3f;
float hor_shift = 0.0f;
float ver_shift = 0.0f;
float pan_unit = 0.1f;

void zoom_change(int i)
{
    ortho += zoom_factor * i;
    Matrices.projection = glm::ortho(-ortho - ver_shift, ortho - ver_shift, -ortho - hor_shift, ortho - hor_shift, 0.1f, 500.0f);
}

void scroll(GLFWwindow* window, double x, double y)
{
    x = int(x);
    y = int(y);
    if(x==0)zoom_change(-y);
}

void change_brick_speed(int i)
{
    brick_speed += i*brick_speed_unit;
    brick_speed = max(brick_speed, max_brick_speed);
    brick_speed = min(brick_speed, min_brick_speed);

    for(set<Brick*>::iterator i = a.begin(); i != a.end(); i++)
        (*i)->set_vy(brick_speed);
}

void pan(int h, int v)
{
    hor_shift += h*pan_unit;
    ver_shift += v*pan_unit;
    Matrices.projection = glm::ortho(-ortho - ver_shift, ortho - ver_shift, -ortho - hor_shift, ortho - hor_shift, 0.1f, 500.0f);
}

void keyboard (GLFWwindow* window, int key, int scancode, int action, int mods)
{
     // Function is called first on GLFW_PRESS.

    if (action == GLFW_RELEASE) {
        switch (key) {
            case GLFW_KEY_SPACE:
                keep_firing = false;
                break;
            case GLFW_KEY_A:
                funnel_rotation = 0;
                //c->update_funnel_rotation(5.0f);
                break;
            case GLFW_KEY_D:
                funnel_rotation = 0;
                //c->update_funnel_rotation(-5.0f);
                break;
            case GLFW_KEY_S:
                canon_move = 0;
                break;
            case GLFW_KEY_F:
                canon_move = 0;
                break;
            case GLFW_KEY_RIGHT_CONTROL:
                bucket_move[0] = 0;
                break;
            case GLFW_KEY_RIGHT_ALT:
                bucket_move[1] = 0;
                break;
            case GLFW_KEY_LEFT_CONTROL:
                bucket_move[0] = 0;
                break;
            case GLFW_KEY_LEFT_ALT:
                bucket_move[1] = 0;
                break;
            case GLFW_KEY_UP:
                zoom_change(-1);
                break;
            case GLFW_KEY_DOWN:
                zoom_change(1);
                break;
            case GLFW_KEY_M:
                change_brick_speed(-1);
                break;
            case GLFW_KEY_N:
                change_brick_speed(1);
                break;
            case GLFW_KEY_I:
                pan(1, 0);
                break;
            case GLFW_KEY_K:
                pan(-1, 0);
                break;
            case GLFW_KEY_L:
                pan(0, 1);
                break;
            case GLFW_KEY_J:
                pan(0, -1);
                break;
            /*case GLFW_KEY_SPACE:
            {
                float time_now = glfwGetTime();
                if(time_now - last_bullet_time < min_time_between_bullets)break;
                pair<pair<float, float>, pair<float, float> > st = c->get_bullet_start();
                b.insert(new Bullet(st.first.first, st.first.second, st.second.first, st.second.second));
                last_bullet_time = time_now;
                // do something ..
                break;
            }*/
            case GLFW_KEY_P:
                mypause = !mypause;
            case GLFW_KEY_U:
                selected = 1;
                break;
            default:
                break;
        }
    }
    else if (action == GLFW_PRESS) {
        switch (key) {
            case GLFW_KEY_SPACE:
                keep_firing = true;
                break;
            case GLFW_KEY_A:
                funnel_rotation = 1;
                //c->update_funnel_rotation(5.0f);
                break;
            case GLFW_KEY_D:
                funnel_rotation = -1;
                //c->update_funnel_rotation(-5.0f);
                break;
            case GLFW_KEY_S:
                canon_move = 1;
                break;
            case GLFW_KEY_F:
                canon_move = -1;
                break;
            case GLFW_KEY_RIGHT_CONTROL:
                bucket_move[0] = 1;
                break;
            case GLFW_KEY_RIGHT_ALT:
                bucket_move[1] = 1;
                break;
            case GLFW_KEY_LEFT_ALT:
                bucket_move[1] = -1;
                break;
            case GLFW_KEY_LEFT_CONTROL:
                bucket_move[0] = -1;
                break;
            case GLFW_KEY_I:
                break;
            case GLFW_KEY_ESCAPE:
                quit(window);
                break;
            default:
                break;
        }
    }
}

/* Executed for character input (like in text boxes) */
void keyboardChar (GLFWwindow* window, unsigned int key)
{
	switch (key) {
		case 'Q':
		case 'q':
            quit(window);
            break;
		default:
			break;
	}
}

void handle_left_click_press()
{
    if(selected==obj["canon"] || selected==obj["funnel"])
        keep_firing = true;
}
double last_click;

void handle_left_click_release()
{
    double time_now = glfwGetTime();
    if(time_now - last_click < 0.2)
        selected = obj["no"];

    last_click = time_now;
    if(keep_firing)
        keep_firing = false;
    if(c->on_canon())
        selected = obj["canon"];
    else if(c->on_funnel())
        selected = obj["funnel"];
}

void handle_right_click_press()
{}

void handle_right_click_release()
{
    selected = obj["no"];
}

/* Executed when a mouse button is pressed/released */
void mouseButton (GLFWwindow* window, int button, int action, int mods)
{
    switch (button) {
        case GLFW_MOUSE_BUTTON_LEFT:
            if (action == GLFW_RELEASE)
                handle_left_click_release();
            else if(action == GLFW_PRESS)
                handle_left_click_press();
            break;
        case GLFW_MOUSE_BUTTON_RIGHT:
            if (action == GLFW_RELEASE)
                handle_right_click_release();
            else if(action == GLFW_PRESS)
                handle_right_click_press();
            break;
        default:
            break;
    }
}


/* Executed when window is resized to 'width' and 'height' */
/* Modify the bounds of the screen here in glm::ortho or Field of View in glm::Perspective */
void reshapeWindow (GLFWwindow* window, int width, int height)
{
    int fbwidth=width, fbheight=height;
    /* With Retina display on Mac OS X, GLFW's FramebufferSize
     is different from WindowSize */
    glfwGetFramebufferSize(window, &fbwidth, &fbheight);

	GLfloat fov = 90.0f;

	// sets the viewport of openGL renderer
	glViewport (0, 0, (GLsizei) fbwidth, (GLsizei) fbheight);

	// set the projection matrix as perspective
	/* glMatrixMode (GL_PROJECTION);
	   glLoadIdentity ();
	   gluPerspective (fov, (GLfloat) fbwidth / (GLfloat) fbheight, 0.1, 500.0); */
	// Store the projection matrix in a variable for future use
    // Perspective projection for 3D views
    // Matrices.projection = glm::perspective (fov, (GLfloat) fbwidth / (GLfloat) fbheight, 0.1f, 500.0f);

    // Ortho projection for 2D views
    Matrices.projection = glm::ortho(-ortho, ortho, -ortho, ortho, 0.1f, 500.0f);
}
