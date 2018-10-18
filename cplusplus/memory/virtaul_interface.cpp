#include <iostream>
using namespace std;

class FooInterface {
    public:
    virtual ~FooInterface() = default;
    virtual void Foo() = 0;
};

class BarInterface {
    public:
    virtual ~BarInterface() = default;

    virtual void Bar() = 0;
};

class Concrete : public BarInterface ,public FooInterface {
    public:
    void Bar() override { cout << "Bar()" << endl; }
    void Foo() override { cout << "Foo()" << endl; }
};

int main() {
    Concrete c;
    c.Foo();
    c.Bar();

    FooInterface* foo = &c;
    foo->Foo();

    BarInterface* bar = (BarInterface*)(&c);
    bar->Bar(); // Prints "Foo()" - WTF?
}
