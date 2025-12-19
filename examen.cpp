#include <iostream>

// Clase abstracta Mamifero
class Mamifero {
public:
    virtual void amamantar() const = 0;
    virtual ~Mamifero() {}
};

// Clase abstracta AnimalOviparo
class AnimalOviparo {
public:
    virtual void ponerHuevo() const = 0;
    virtual ~AnimalOviparo() {}
};

// Clase abstracta AnimalAcuatico
class AnimalAcuatico {
public:
    virtual void nadar() const = 0;
    virtual ~AnimalAcuatico() {}
};

// Clase Ornitorrinco con herencia múltiple
class Ornitorrinco : public Mamifero, public AnimalOviparo, public AnimalAcuatico {
public:
    void amamantar() const override {
        std::cout << "El ornitorrinco amamanta a sus crias." << std::endl;
    }

    void ponerHuevo() const override {
        std::cout << "El ornitorrinco pone huevos." << std::endl;
    }

    void nadar() const override {
        std::cout << "El ornitorrinco nada en el agua." << std::endl;
    }
};

int main() {
    Ornitorrinco o;

    o.amamantar();
    o.ponerHuevo();
    o.nadar();
     std::cout << "estudiante:" << std::endl;
    std::cout << "LIZETH ADELA RODRIGUEZ PADILLA" << std::endl;

    return 0;
   
}
