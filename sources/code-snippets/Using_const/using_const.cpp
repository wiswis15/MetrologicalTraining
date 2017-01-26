#include <iostream>

int main()
{
    int i;
    const int ic = 1;               // Must initialize
    int * pi;
    const int * pci;                // const int * and int const * mean the same thing
    int const * pic;
    int * const cpi = &i;           // Must initialize
    const int * const cpci = &i;    // Must initialize

    int & ri = i;                   // Must initialize
    const int & rci = i;            // Must initialize
                                    // const int & and int const & mean the same thing
    int const & ric = i;            // Must initialize
                                    // using & const is actually nonsense, as a reference is always const,
                                    // but it will result in a reference to a const object
    int & const cri = i;            // Must initialize
    const int & const crci = i;     // Must initialize

    i = 2;
    //ic = 2;                       // Won't compile as ic is const int

    *pi = 2;
    //*pci = 2;                     // Won't compile as pci is a pointer to const int
    //*pic = 2;                     // Won't compile as pic is a pointer to const int
    *cpi = 2;
    //*cpci = 2;                    // Won't compile as cpci is a pointer to const int

    pi = &i;
    //pi = &ic;                     // Won't compile as pi is a pointer to int, and we're assigning a pointer to const int
    pci = &i;
    pci = &ic;
    pic = &i;
    pic = &ic;
    //cpi = &i;                     // Won't compile as cpi is a const pointer
    pi++;
    pci++;
    pic++;
    //cpi++;                        // Won't compile as cpi is a const pointer

    ri = ic;                        // Sets i to value of ic
    //rci = ic;                     // Won't compile as rci is a reference to const int
    //ric = ic;                     // Won't compile as ric is a reference to const int
    //cri = ic;                     // Won't compile as cri is a reference to const int
    //crci = ic;                    // Won't compile as crci is a reference to const int

    ri = 2;
    //rci = 2;                      // Won't compile as rci is a reference to const int
    //ric = 2;                      // Won't compile as ric is a reference to const int
    //cri = 2;                      // Won't compile as cri is a reference to const int
    //crci = 2;                     // Won't compile as crci is a reference to const int

    return 0;
}