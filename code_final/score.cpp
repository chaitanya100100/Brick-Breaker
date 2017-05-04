#include "headers.h"
#include "vao.h"
#include "block.h"
#include "brick.h"
#include "bucket.h"
#include "score.h"

int black_hit = 0;
int collect_score = 100;
int hit_score = 100;

void display_score()
{
    cout << "---------------------------------------------------------------------------------" << endl;
    int x, y, z;
    cout << "RED   COLLECTED : " << d[0]->get_collection() << " x " << collect_score << " = " << (x = d[0]->get_collection()*collect_score) << endl;
    cout << "GREEN COLLECTED : " << d[1]->get_collection() << " x " << collect_score << " = " << (y = d[1]->get_collection()*collect_score) << endl;
    cout << "BLACK HIT       : " << black_hit              << " x " << hit_score     << " = " << (z = hit_score*black_hit) << endl;
    cout << endl << "TOTAL           : " << x+y+z << endl;
    cout << "---------------------------------------------------------------------------------" << endl;
}

void score_brick_bucket(int bucket_number, int brick_color, int bucket_color)
{
    if(brick_color == black)
    {
        cout << "Game Over" << endl;
        display_score();
        exit(0);
    }
    if(brick_color == bucket_color)
        d[bucket_number]->update_collection(+1);
    else
        d[bucket_number]->update_collection(-1);
    display_score();
}

void score_brick_bullet(int brick_color)
{
    if(brick_color == black)
    {
        black_hit += 1;
        display_score();
    }
}
