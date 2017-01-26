#ifdef CPP11_TEST_H
#define CPP11_TEST_H

class A
{
public:
    A() {}
    virtual ~A() {}
    virtual int foo() = 0;
    virtual int bar() = 0;
};

class B: public A
{
public:
    int foo() override { return 1; }
    int bar() override final { return 2; }
};

class C: public B
{
public:
    int foo() override final { return 3; }
//    int foo(int) override { return 100; }     // Will fail as signature does not match
    int foo(int) { return 100; }                // Will be accepted, but does not override
//    int bar() override final { return 4; }   // Will fail as the method was flagged final
};

#endif // #ifdef CPP11_TEST_H
