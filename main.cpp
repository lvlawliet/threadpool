#include <iostream>
#include <vector>
#include <chrono>
#include <cstdio>
#include <atomic>

#include "ThreadPool.h"

std::atomic_flag lock = ATOMIC_FLAG_INIT;
int main()
{
    ThreadPool pool( 2 );
    std::vector< std::future< int > > results;

    for ( int i = 0; i < 8; ++i )
    {
        results.emplace_back(
            pool.AddTask(
                [i]
                {
                    //std::cout << "hello" << i << std::endl;
                    while ( lock.test_and_set() );
                    printf( "hello %d\n", i );
                    lock.clear();
                    std::this_thread::sleep_for( std::chrono::seconds( 1 ) );
                    //std::cout << "world" << i << std::endl;
                    while ( lock.test_and_set() );
                    printf( "world %d\n", i );
                    lock.clear();
                    return i * i;
                }
            )
        );
    }
    /*

    for ( auto && result : results )
        std::cout << result.get() << " ";
    std::cout << std::endl;*/
    return 0;
}
