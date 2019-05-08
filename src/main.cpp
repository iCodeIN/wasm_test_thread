#include <cstdlib>
#include <emscripten.h>
#include <pthread.h>
#include <stdio.h>

pthread_mutex_t lock;

void* crack( void* arg ) {
    unsigned int id = *( static_cast<unsigned int*>( arg ) );
    srand( id );

    unsigned int key = -1;
    while( true ) {
        unsigned int v = rand();
        if( v == ( v & key ) ) {
            if( pthread_mutex_lock( &lock ) ) {
                printf( "mutex lock failed\n" );
                return nullptr;
            }
            printf( "thread=%u key=%x value=%x\n", id, key, v );
            if( pthread_mutex_unlock( &lock ) ) {
                printf( "mutex unlock failed\n" );
                return nullptr;
            }
            key = key << 1;
        }
    }
}

int main() {
    if( pthread_mutex_init( &lock, NULL ) != 0 ) {
        printf( "mutex init failed\n" );
        return -1;
    }

    for( unsigned int i = 0; i < 2; ++i ) {
        pthread_t thread;
        if( pthread_create( &thread, nullptr, crack, static_cast<void*>( new unsigned int( i ) ) ) ) {
            fprintf( stderr, "Failed to start thread.\n" );
        }
    }

    return 0;
}
