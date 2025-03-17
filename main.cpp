/*
*   Ribe Katedralskole SRP 2025
*   Skrevet af Thomas Seeberg Hansen 3.x  
*/

#include <iostream>
#include <random>



// PowMod er hurtigere og mere præcis end standard pow(). Udregninger sker med heltal, fremfor floating point matematik.    Kilde: https://github.com/cslarsen/miller-rabin/blob/master/miller-rabin.cpp
// "Exponentiation by squaring" virker især hurtigt på meget store heltal.                                                  Kilde: https://en.wikipedia.org/wiki/Exponentiation_by_squaring
uint64_t PowMod(uint64_t a, uint64_t x, uint64_t n)
{
    uint64_t r = 1;

    while (x)
    {
        if ((x & 1) == 1)
            r = a * r % n;

        x >>= 1;
        a = a * a % n;
    }

    return r;
}

bool MillerRabin_CheckPrime(uint64_t n, uint8_t rounds)
{
    // Trin 1, de simple test kriterier
    if (n < 2) 
        return false;       // Primtal er st�rre end eller ligmed 2
    if (n == 2 || n == 3)   
        return true;        // Vi ved på forhånd disse tal 2 & 3 er primtal. 
    if (n % 2 == 0) 
        return false;       // Tallet skal være ulige, ellers er det ikke et primtal

    // Trin 2. k & m bestemmes ud fra ligningen: n - 1 = 2^k * m
    uint64_t m = n - 1;
    uint64_t k = 0;

    // m halveres indtil m er ulige
    while (m % 2 == 0) 
    {
        m /= 2;
        k++;
    }

    // Setup til at generere forskelige og tilfældige testværdier(a). Indenfor intervallet 1 < a < n - 2
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<uint64_t> dist(2, n - 2);

    // Trin 3. Udførsel af t antal runder
    for (int i = 0; i < rounds; i++)
    {
        // Tilfældig valg af a. Kriterier: [2, n - 2]
        uint64_t a = dist(gen);

        // Trin 4. Eksponentiation ved kvadrering. x=a^m (mod n)
        uint64_t x = PowMod(a, m, n);

        if (x == 1 || x == n - 1)
            continue;

        // Trin 5. 
        bool prime = false;

        while(m != n-1)
        {
            x = (x * x) % n;
            m = m * 2;

            if (x == n - 1) 
                return true;
            if (x == 1)
                return false;
        }

        return false;
    }

    return true;
}

int main()
{
   int check[] = {2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47, 53, 59, 61, 67, 71, 73, 79, 83, 89, 97};
    int c = 0;

    for (int i = 2; i < 1000; i++)
    {
        if (MillerRabin_CheckPrime(i, 40))
        {
           std::cout << "PRIMTAL: " << i << "\n";

           if(check[c] != i && i < sizeof(check) / sizeof(int))
                throw(-1);
            else
                std::cout << "GOD\n";

           c++;
        }
    }

    /*for (int i = 0; i < 500; i++)
    {
        bool a = MillerRabin_CheckPrime(561, 1);

        if (a)
            throw(-1);
        else
            std::cout << "SAMMENSAT";
    }*/


    return 0;
}
