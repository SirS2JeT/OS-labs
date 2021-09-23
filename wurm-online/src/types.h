/**
* \file types.h
*/

#ifndef H_TYPES
#define H_TYPES

#include <queue>
using namespace std;


// STRUCTURES ==========================================================
/**
* \typedef coord
* \brief struct Represents a couple of coordinates.
*/
struct coord {
    int x;  /**< x coordinate */
    int y;  /**< y coordinate */
};

/**
* \brief Allows to use the four main directions.
*/
enum class direction
{
    UP, DOWN, LEFT, RIGHT
};

/**
* \brief Gathers the possible content of a square of the field.
*/
enum class square
{
    EMPTY, WALL, SNAKE = 2, SCHLANGA = 3, FOOD, POPWALL, HIGHSPEED, LOWSPEED, FREEZE
};

/**
* \brief type of a worm, can be either a worm or a schlanga.
*/
enum class t_type
{
    s_Worm = 2, doll_Worm = 3
};

/**
* \typedef worm
* \brief Represents a worm
* \details 'body' is an array of 'coord'.
*          'head' holds the index of the coordinates of the head in 'body'
*          'tail' holds the index of the coordinates of the tail in 'body'
*          'dir' is the direction the worm is currently moving.
*/
struct worm {
    t_type type;    /**< type of worm, can be 'SCHLANGA' or 'SNAKE' */
    std::queue<coord> body;   /**< array containing the coords of every part of the worm*/
    direction dir;  /**< current direction the worm is faceing */
    bool add_size;
    
    int get_size() const {return body.size();}
};

/**
* \typedef field
* \brief Represents the arena on which the game is played
*/
struct field {
    square** f;     		/**< 2D array representing the field*/
    int width;     			/**< width of the field */
    int height;     		/**< height of the field */
    int timestep;				/**< basic speed of game */
    int speed;
    int freeze_worm1;		/**< freezing-time left for worm */
    int freeze_worm2;	/**< freezing-time left for worm2 */
};

// PROTOTYPES ==========================================================
// Constructors ========================================================
coord new_coord(int x, int y);
coord new_coord_empty();
field* new_field(int width, int height, int timestep, bool draw);
worm* new_worm(t_type type, int start_pos, field* map, bool draw);

// Destructors =========================================================
void free_worm(worm* s);
void free_field(field* map);
void free_all(field* map, worm* s1, worm* s2);

// Objects managment ===================================================
bool are_equal(coord c1, coord c2);
direction opposite(direction d);
direction turn_left(direction d);
direction turn_right(direction d);
square get_square_at(field* map, coord c);
void set_square_at(field* map, coord c, square stuff);
coord get_head_coord(worm* s);
coord get_tail_coord(worm* s);
coord coord_after_dir(coord c, direction dir);
coord remove_tail(field* map, worm* s);
void push_head(field* map, worm* s, coord head);

#endif
