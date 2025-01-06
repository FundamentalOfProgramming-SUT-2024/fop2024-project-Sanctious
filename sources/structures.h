typedef enum {
    WINDOW,
    OBSTACLE,
    TRAP
} StructureType;


typedef struct{
    int damage;
} TrapExtra;

typedef struct{
    char sprite;
    void* StructureExtra;
    StructureType type;

} Structure;
