#ifndef CHARACTER
#define CHARACTER

class Character {
    private:
        int x, y;
        int hp;
        bool dead;
        int direction;
        LayzerManager layzer_manager;

    public:
        bool isDead();
        void move();
        void layzerAttack();
};  

#endif