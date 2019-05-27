#ifndef MAZE_H
#define MAZE_H

//! Forme d'un intersection
#define direction   char
#define top         1
#define down        2
#define right       4
#define left        8
#define all         15

class maze
{
public:
    maze() {}

protected:


    //Connecte le nœud au voisin aléatoire (si possible) et renvoie
    //adresse du nœud suivant qui doit être visité
    node *link(node *n);

    //! Structure définissant un noeud (case)
    typedef struct
    {
        int x, y;           // Position du noeud
        node *parent;       // Parant du noeud dans le plan
        direction dirs;     // Directiond déjà exploré
        direction n_exp;    // Direction a explorer
    } node;

    //! Athributs
    node *root;
    int width, height;
};

#endif // MAZE_H
