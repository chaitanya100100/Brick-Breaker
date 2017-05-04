#include "headers.h"
#include "vao.h"
#include "block.h"
#include "brick.h"
#include "bullet.h"
#include "mirror.h"

float ax, ay, axx, ayy, bx, by, vx1, vy1, vx2, vy2, vx3, vy3;
glm::vec3 v0, v1;
float det, a1, a2, b1, b2, c1, c2;

bool has_collided(Block * & a, pair<float, float> ao, Block * & b, pair<float, float> bo)
{
    ax = a->get_x() + ao.first;
    ay = a->get_y() + ao.second;
    bx = b->get_x() + bo.first;
    by = b->get_y() + bo.second;

    return
        (bx < ax + a->get_width()) &&
        (ax < bx + b->get_width()) &&
        (by < ay + a->get_height()) &&
        (ay < by + b->get_height())
    ;
}

bool has_collided(Block * & a, pair<float, float> ao, Triangle * & b, pair<float, float> bo)
{
    ax = a->get_x() + ao.first;
    ay = a->get_y() + ao.second;
    axx = ax + a->get_width();
    ayy = ay + a->get_height();

    v0 = b->get_vertex(0);
    v1 = b->get_vertex(1);

    vy1 = bo.second;
    vx1 = bo.first;
    vx2 = bo.first + v0[0];
    vy2 = bo.second + v0[1];
    vx3 = bo.first + v1[0];
    vy3 = bo.second + v1[1];

    return
        (vx1 <= axx && vx1 >= ax && vy1 <= ayy && vy1 >= ay) ||
        (vx2 <= axx && vx2 >= ax && vy2 <= ayy && vy2 >= ay) ||
        (vx3 <= axx && vx3 >= ax && vy3 <= ayy && vy3 >= ay)
    ;
}

bool has_collided(Block * & a, pair<float, float> ao, Triangle * & b, pair<float, float> bo, float r)
{
    ax = a->get_x(); //+ ao.first;
    ay = a->get_y(); //+ ao.second;
    axx = ax + a->get_width();
    ayy = ay + a->get_height();
    //if(ao.first > 0.5)
    //cout << ao.first << " " << ao.second << endl;
    v0 = b->get_vertex(0);
    v1 = b->get_vertex(1);

    vx1 = bo.first - ao.first;
    vy1 = bo.second - ao.second;
    vx2 = bo.first + v0[0] - ao.first;
    vy2 = bo.second + v0[1] - ao.second;
    vx3 = bo.first + v1[0] - ao.first;
    vy3 = bo.second + v1[1] - ao.second;

    glm::mat4 q = glm::mat4(glm::rotate(float(-r*M_PI/180.0f), glm::vec3(0.0f, 0.0f, 1.0f)));
    glm::vec4 h = glm::vec4(vx1, vy1, 0.0f, 0.0f);
    h = q * h;
    vx1 = h[0];
    vy1 = h[1];

    h = glm::vec4(vx2, vy2, 0.0f, 0.0f);
    h = q * h;
    vx2 = h[0];
    vy2 = h[1];

    h = glm::vec4(vx3, vy3, 0.0f, 0.0f);
    h = q * h;
    vx3 = h[0];
    vy3 = h[1];

    return
        (vx1 <= axx && vx1 >= ax && vy1 <= ayy && vy1 >= ay) ||
        (vx2 <= axx && vx2 >= ax && vy2 <= ayy && vy2 >= ay) ||
        (vx3 <= axx && vx3 >= ax && vy3 <= ayy && vy3 >= ay)
    ;
}

bool has_collided(Triangle * & b, pair<float, float> bo, Block * & a, pair<float, float> ao)
{
    return has_collided(a, ao, b, bo);
}

bool has_collided(Triangle * & b, pair<float, float> bo, Block * & a, pair<float, float> ao, float r)
{
    return has_collided(a, ao, b, bo, r);
}

/*
bool onSegment(glm::vec3 p, glm::vec3 q, glm::vec3 r)
{
    if (q[0] <= max(p[0], r[0]) && q[0] >= min(p[0], r[0]) &&
            q[1] <= max(p[1], r[1]) && q[1] >= min(p[1], r[1]))
        return true;
    return false;
}

int orientation(glm::vec3 p, glm::vec3 q, glm::vec3 r)
{
    float val = (q[1] - p[1]) * (r[0] - q[0]) -
              (q[0] - p[0]) * (r[1] - q[1]);

    if (abs(val) < 1e-6) return 0;  // colinear
    return (val > 0)? 1: 2; // clock or counterclock wise
}


bool line_seg_intersect(glm::vec3 p1, glm::vec3 q1, glm::vec3 p2, glm::vec3 q2)
{
    int o1 = orientation(p1, q1, p2);
    int o2 = orientation(p1, q1, q2);
    int o3 = orientation(p2, q2, p1);
    int o4 = orientation(p2, q2, q1);

    // General case
    if (o1 != o2 && o3 != o4)
        return true;

    // Special Cases
    // p1, q1 and p2 are colinear and p2 lies on segment p1q1
    if (o1 == 0 && onSegment(p1, p2, q1)) return true;

    // p1, q1 and p2 are colinear and q2 lies on segment p1q1
    if (o2 == 0 && onSegment(p1, q2, q1)) return true;

    // p2, q2 and p1 are colinear and p1 lies on segment p2q2
    if (o3 == 0 && onSegment(p2, p1, q2)) return true;

     // p2, q2 and q1 are colinear and q1 lies on segment p2q2
    if (o4 == 0 && onSegment(p2, q1, q2)) return true;

    return false;
}

bool isInside(glm::vec3 polygon[], int n, glm::vec3 p)
{
    glm::vec3 extreme(1.0e10, p[1]);

    // Count intersections of the above line with sides of polygon
    int count = 0, i = 0;
    do
    {
        int next = (i+1)%n;

        // Check if the line segment from 'p' to 'extreme' intersects
        // with the line segment from 'polygon[i]' to 'polygon[next]'
        if (doIntersect(polygon[i], polygon[next], p, extreme))
        {
            // If the point 'p' is colinear with line segment 'i-next',
            // then check if it lies on segment. If it lies, return true,
            // otherwise false
            if (orientation(polygon[i], p, polygon[next]) == 0)
               return onSegment(polygon[i], p, polygon[next]);

            count++;
        }
        i = next;
    } while (i != 0);

    // Return true if count is odd, false otherwise
    return count&1;  // Same as (count%2 == 1)
}

bool is_colliding(Block * a, glm::mat4 at, Block * b, glm::mat4 bt)
{
    glm::vec3 poly[4], goly[4];
    at = a->get_transform(at);
    bt = a->get_transform(bt);

    poly[0] = at * glm::vec3(0.0f);
    poly[1] = at * glm::vec3(a->width, 0.0f, 0.0f);
    poly[2] = at * glm::vec3(a->width, a->height, 0.0f);
    poly[3] = at * glm::vec3(0.0f, a->height, 0.0f);

    goly[0] = bt * glm::vec3(0.0f);
    goly[1] = bt * glm::vec3(b->width, 0.0f, 0.0f);
    goly[2] = bt * glm::vec3(b->width, b->height, 0.0f);
    goly[3] = bt * glm::vec3(0.0f, b->height, 0.0f);

    for(int i=0; i<4; i++)
        if(isInside(poly, 4, goly[i]))
            return true;
    return false;
}

bool is_colliding(Brick * p, Bullet * q)
{
    glm::mat4 pt = p->get_transform(glm::mat4(1.0f));
    glm::mat4 qt = q->get_transform(glm::mat4(1.0f));

    for(vector<Block*>::iterator i=(p->blocks).begin(); i!=(p->blocks).end(); i++)
        for(vector<Block*>::iterator j=(q->blocks).begin(); j!=(q->blocks).end(); j++)
            if(is_colliding(*i, pt, *j, qt)) return true;


    for(vector<Block*>::iterator i=(p->blocks).begin(); i!=(p->blocks).end(); i++)
        for(vector<Triangle*>::iterator j=(q->triangles).begin(); j!=(q->triangles).end(); j++)
            if(is_colliding(*i, pt, *j, qt)) return true;

    for(vector<Triangle*>::iterator i=(p->triangles).begin(); i!=(p->triangles).end(); i++)
        for(vector<Block*>::iterator j=(q->blocks).begin(); j!=(q->blocks).end(); j++)
            if(is_colliding(*i, pt, *j, qt)) return true;

    return false;
}
*/
