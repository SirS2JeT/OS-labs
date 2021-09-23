/**
* \file AI.c
* \brief Functions related to AI.
* \details This file is separated in 2 parts :
*          1 - functions that are used by an AI main function
*          2 - AI main function : they have to return the choosen direction to go.
*/

#include <stdlib.h>     //for 'rand()'
#include <stdbool.h>
#include <math.h>
#include "types.h"

#include "AI.h"


// Helpers =============================================================
/**
* \fn int detect(worm* s, direction c, field* map);
* \brief function that returns 1 if the 's' worm can go in the
*        'c' direction without dying.
*/
int detect(worm* s, direction c, field* map){
    coord start = s->body.front();
    int a=start.x;
    int b=start.y;
    if (c == direction::UP) {
        return !(map->f[a-1][b] == square::WALL || map->f[a-1][b] == square::SNAKE || map->f[a-1][b] == square::SCHLANGA);
    } else if (c == direction::LEFT) {
        return !(map->f[a][b-1] == square::WALL || map->f[a][b-1] == square::SNAKE || map->f[a][b-1] == square::SCHLANGA);
    } else if (c == direction::RIGHT) {
        return !(map->f[a][b+1] == square::WALL || map->f[a][b+1] == square::SNAKE || map->f[a][b+1] == square::SCHLANGA);
    } else if (c == direction::DOWN) {
        return !(map->f[a+1][b] == square::WALL || map->f[a+1][b] == square::SNAKE || map->f[a+1][b] == square::SCHLANGA);
    }
    else{return 1;}
}

/**
* \fn bool not_in(coord c, coord* tableau, int taille);
* \returns True if the 'c' coord is not in 'array' (of length 'size').
*          False otherwise.
*/
bool not_in(coord c, coord* array, int size){
    int i;
    for(i=0;i<size;i++){
        if (are_equal(c,array[i])){
            return false;
        }
    }
    return true;
}

/**
* \fn rec(field* map, coord c, coord* tableau, int* i);
* \brief Base of the recursive function spread.
* Return how many empty squares are free in a given space.
* Marks every square already visited in the array "tableau".
*/
int rec(field* map, coord c, coord* tableau, int* i){
    coord up=coord_after_dir(c, direction::UP);
    coord down=coord_after_dir(c, direction::DOWN);
    coord left=coord_after_dir(c, direction::LEFT);
    coord right=coord_after_dir(c, direction::RIGHT);

    if (get_square_at(map,c) != square::EMPTY || !not_in(c,tableau,i[0]) || i[0]>20){
        return 0;
    }
    else{
        tableau[i[0]]=c;
        i[0]++;

        int zz=rec(map,up,tableau,i);
        int ze=rec(map,left,tableau,i);
        int zr=rec(map,right,tableau,i);
        int zt=rec(map,down,tableau,i);
        return (1+zz+ze+zr+zt);
    }
    return 0;
}

/**
* \fn float dist(coord depart, coord arrivee);
* \brief Return the euclidian distance between 2 points.
*/
float dist(coord depart, coord arrivee){
    return (sqrtf((depart.x-arrivee.x)*(depart.x-arrivee.x) + (depart.y-arrivee.y)*(depart.y-arrivee.y)));
}

/**
* \fn compare_aggro(float a, float b);
* \brief Used in best_aggro.
* The function used to compare distances in aggro_dist.
*/
bool compare_aggro(float a, float b){
    if (a==0){
        return false;
    }
    else if (b==0){
        return true;
    }
    else {
        return (a<b);
    }
}

/**
* \fn direction best_aggro(float a, float b, float c, float d, worm* s, field* map)
* \brief Base of the aggro_dist AI.
* Returns the best choice between the different distances for an aggressiv AI (therefore the shortest).
*/
direction best_aggro(float a, float b, float c, float d, worm* s, field* map){
    if ((compare_aggro(c,a)) && (compare_aggro(c,b)) && (compare_aggro(c,d)) && detect(s,direction::LEFT,map)){
        return direction::LEFT;
    }
    else if ((compare_aggro(b,a)) && (compare_aggro(b,c)) && (compare_aggro(b,d)) && detect(s,direction::DOWN,map)){
        return direction::DOWN;
    }
    else if ((compare_aggro(a,b)) && (compare_aggro(a,c)) && (compare_aggro(a,d)) && detect(s,direction::UP,map)){
        return direction::UP;
    }
    else if ((compare_aggro(d,a)) && (compare_aggro(d,b)) && (compare_aggro(d,c)) && detect(s,direction::RIGHT,map)){
        return direction::RIGHT;
    }
    else{
        return spread(s,map);
    }
}

/**
* \fn compare_def(float a, float b);
* \brief Used in best_def.
* The function used to compare distances in defensif_dist.
*/
bool compare_def(float a, float b){
    if (a==0){
        return false;
    }
    else if (b==0){
        return true;
    }
    else {
        return (a>b);
    }
}

/**
* \fn direction best_def(float a, float b, float c, float d, worm* s, field* map)
* \brief Base of the defensif_dist AI.
* Returns the best choice between the different distances for a defensiv AI (therefore the longest).
*/
direction best_def(float a, float b, float c, float d, worm* s, field* map){
    if ((compare_def(c,a)) && (compare_def(c,b)) && (compare_def(c,d)) && detect(s, direction::LEFT,map)){
        return direction::LEFT;
    }
    else if ((compare_def(b,a)) && (compare_def(b,c)) && (compare_def(b,d)) && detect(s, direction::DOWN,map)){
        return direction::DOWN;
    }
    else if ((compare_def(a,b)) && (compare_def(a,c)) && (compare_def(a,d)) && detect(s, direction::UP,map)){
        return direction::UP;
    }
    else if ((compare_def(d,a)) && (compare_def(d,b)) && (compare_def(d,c)) && detect(s, direction::RIGHT,map)){
        return direction::RIGHT;
    }
    else{
        return spread(s,map);
    }
}

// AI main functions ===================================================
/**
* \fn dir rngesus(worm* s);
* \brief chooses a direction to move randomly. No wall avoiding.
* \details The function picks a random direction. If the direction is
*          the opposite of the direction the schlanga is moving, then it
*          repicks a direction, and so on... No wall avoiding.
*          You may have noticed that we affect an int to 'd' which is
*          a direction. That's possible because behind every enumeration
*          lies an int. 'UP' is in fact 0, 'DOWN' is in fact 1 ... etc
* \returns direction choosen
*/
direction rngesus(worm* s){
    direction d;

    do{
        d = direction(rand() % 4);
    }while(d == opposite(s->dir));

    return d;
}

/**
* \fn direction rngesus2(worm* s, field* map);
* \brief chooses a direction to move randomly. With wall and worm
*        avoiding mechanism.
*/
direction rngesus2(worm* s, field* map){
    direction dir;
    int pick_counter = 0;

    do{
        dir = direction(rand() % 4);
        pick_counter++;
    }while( (dir == opposite(s->dir) || !detect(s, dir, map))
                && pick_counter < IA_MAX_PICK);

    return dir;
}

/**
* \fn direction spread(worm* s,field* map);
* \brief Chooses a direction considering how much space is left to move in.
*        When the choice doesn't matter, rngesus2 will be used.
*/
direction spread(worm* s,field* map){
    coord tableft[(map->width)*(map->height)];
    coord tabright[(map->width)*(map->height)];
    coord tabup[(map->width)*(map->height)];
    coord tabdown[(map->width)*(map->height)];

    coord start=s->body.front();
    coord u=coord_after_dir(start,direction::UP);
    coord d=coord_after_dir(start,direction::DOWN);
    coord l=coord_after_dir(start,direction::LEFT);
    coord r=coord_after_dir(start,direction::RIGHT);

    int a1,a2,a3,a4;
    int i=0;

    a1=rec(map,l,tableft,&i);i=0;
    a2=rec(map,r,tabright,&i);i=0;
    a3=rec(map,d,tabdown,&i);i=0;
    a4=rec(map,u,tabup,&i);

    if ( (a1==a2 && a1==a3) || (a2==a3 && a2==a4) || (a3==a4 && a3==a1) || (a4==a2 && a4==a1)){
        return rngesus2(s,map);
    }
    else if (a1>=a2 && a1>=a3 && a1>=a4){
        return direction::LEFT;
    }
    else if (a2>=a3 && a2>=a4 && a2>=a1){
        return direction::RIGHT;
    }
    else if (a3>=a1 && a3>=a2 && a3>=a4){
        return direction::DOWN;
    }
    else if (a4>=a1 && a4>=a2 && a4>=a3){
        return direction::UP;
    }
    else {
        return rngesus2(s,map);
    }
}

/**
* \fn direction aggro_dist(worm* s, field* map, worm* enemy);
* \brief Chooses a direction the closest to the enemy's head.
* Avoids walls and reacts randomly once it is close enough.
*/
direction aggro_dist(worm* s, field* map, worm* enemy){
    coord end = get_head_coord(enemy);
    float a=0,b=0,c=0,d=0;
    coord start = get_head_coord(s);

    if (dist(start,end) < 6){
        spread(s,map);
    }

    else{ 
        if (detect(s,direction::UP,map)){
            a=dist(coord_after_dir(start,direction::UP),end);
        }
        if (detect(s,direction::DOWN,map)){
            b=dist(coord_after_dir(start,direction::DOWN),end);
        }
        if (detect(s,direction::LEFT,map)){
            c=dist(coord_after_dir(start,direction::LEFT),end);
        }
        if (detect(s,direction::RIGHT,map)){
            d=dist(coord_after_dir(start,direction::RIGHT),end);
        }
        return best_aggro(a,b,c,d,s,map);
    }
    return spread(s,map);
}

/**
* \fn direction defensif_dist(worm* s, field* map, worm* enemy);
* \brief Chooses a direction the further from the enemy's head.
* Avoids walls and reacts randomly once it is far enough.
*/
direction defensif_dist(worm* s, field* map, worm* enemy){
    coord end = get_head_coord(enemy);
    float a=0,b=0,c=0,d=0;
    coord start = get_head_coord(s);

    if (dist(start,end) > 0.5*map->height){
        spread(s,map);
    }

    else{
        if (detect(s,direction::UP,map)){
            a=dist(coord_after_dir(start,direction::UP),end);
        }
        if (detect(s,direction::DOWN,map)){
            b=dist(coord_after_dir(start,direction::DOWN),end);
        }
        if (detect(s,direction::LEFT,map)){
            c=dist(coord_after_dir(start,direction::LEFT),end);
        }
        if (detect(s,direction::RIGHT,map)){
            d=dist(coord_after_dir(start,direction::RIGHT),end);
        }
        return best_def(a,b,c,d,s,map);
    }
    return spread(s,map);
}

/**
* \fn direction heat_map(worm* s, field* map);
* \brief AI based on a map heat of the field.
* Is attracted by the object that will put the enemy in a bad spot and/or the enemy
*/
direction heat_map(worm* s, field* map){
    float** heat;
    float** tampon;
    coord c;
    square q;
    int i,j,k;

    coord start=s->body.front();
    coord u=coord_after_dir(start,direction::UP);
    coord d=coord_after_dir(start,direction::DOWN);
    coord l=coord_after_dir(start,direction::LEFT);
    coord r=coord_after_dir(start,direction::RIGHT);

    int a1,a2,a3,a4;

    heat = (float**)malloc(map->height*sizeof(float*));
    tampon = (float**)malloc(map->height*sizeof(float*));
    for(j=0;j<map->height;j++){
        heat[j] = (float*)malloc(map->width*sizeof(float));
        tampon[j] = (float*)malloc(map->width*sizeof(float));
    }

    for(j=0;j<map->height;j++){
        for(k=0;k<map->width;k++){
            c=new_coord(j,k);
            q=get_square_at(map,c);
            switch(q){
            case square::WALL:
                heat[j][k]=-3;
                tampon[j][k]=-3;
                break;
            case square::SCHLANGA:
                heat[j][k]=-1;
                tampon[j][k]=-1;
                break;
            case square::SNAKE:
                heat[j][k]=+5;
                tampon[j][k]=+5;
                break;
            case square::FOOD:
                heat[j][k]=+2;
                tampon[j][k]=+2;
                break;
            case square::POPWALL:
                heat[j][k]=+3;
                tampon[j][k]=+3;
                break;
            case square::HIGHSPEED:
                heat[j][k]=+4;
                tampon[j][k]=+4;
                break;
            case square::LOWSPEED:
                heat[j][k]=-1;
                tampon[j][k]=-1;
                break;
            case square::FREEZE:
                heat[j][k]=+1;
                tampon[j][k]=+1;
                break;
            case square::EMPTY:
                heat[j][k]=0;
                tampon[j][k]=0;
                break;
            }
        }
    }

    for(i=0;i<5;i++){
        for(j=1;j<(map->height-1);j++){
            for(k=1;k<(map->width-1);k++){
                c=new_coord(j,k);
                if (get_square_at(map,c) == square::EMPTY){
                    tampon[j][k]=(heat[j-1][k-1]+heat[j-1][k]+heat[j-1][k+1]+heat[j][k-1]+heat[j][k]+heat[j][k+1]+heat[j+1][k-1]+heat[j+1][k]+heat[j+1][k+1])/9;
                }
            }
        }
        for(j=1;j<map->height;j++){
            for(k=1;k<map->width;k++){
                heat[j][k]=tampon[j][k];
            }
        }
    }

    a1=heat[u.x][u.y];
    a2=heat[d.x][d.y];
    a3=heat[l.x][l.y];
    a4=heat[r.x][r.y];

    for(j=0;j<map->height;j++){
        free(heat[j]);
        free(tampon[j]);
    }

    free(heat); free(tampon);

    if( (a1>a2) && (a1>a3) && (a1>a4) && detect(s,direction::UP,map) ){
        return direction::UP;
    }
    else if ( (a2>a1) && (a2>a3) && (a2>a4) && detect(s,direction::DOWN,map)){
        return direction::DOWN;
    }
    else if ( (a3>a2) && (a3>a1) && (a1>a4) && detect(s,direction::LEFT,map)){
        return direction::LEFT;
    }
    else if ( (a4>a2) && (a4>a1) && (a4>a3) && detect(s,direction::RIGHT,map)){
        return direction::RIGHT;
    }
    return spread(s,map);
}