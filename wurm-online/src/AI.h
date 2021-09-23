/**
* \file AI.h
*/

#ifndef H_AI
#define H_AI

// CONSTANTS ============================================================
#define IA_MAX_PICK 20 /**< maximum times that the IA tries
                            picking a random direction before giving up.
                            Used to avoid infinite picking.*/

// PROTOTYPES ==========================================================
// Helpers =============================================================
int detect(worm* s, direction c, field* map);
bool not_in(coord c, coord* tableau, int taille);
int rec(field* map, coord c, coord* tableau, int* i);
float dist(coord depart, coord arrivee);
bool compare_aggro(float a, float b);
direction best_aggro(float a, float b, float c, float d, worm* s, field* map);
bool compare_def(float a, float b);
direction best_def(float a, float b, float c, float d, worm* s, field* map);

// AI main functions ===================================================
direction rngesus(worm* s);
direction rngesus2(worm* s, field* map);
direction spread(worm* s,field* map);
direction aggro_dist(worm* s, field* map, worm* enemy);
direction defensif_dist(worm* s, field* map, worm* enemy);
direction heat_map(worm* s, field* map);

#endif
