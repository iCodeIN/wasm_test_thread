#include <stdio.h>

#ifdef EMSCRIPTEN
#include <emscripten.h>
#endif

#include "stl.h"

#ifdef EMSCRIPTEN
#define BASE_PATH "/indexdb/"
#define EXPORT EMSCRIPTEN_KEEPALIVE
#else
#define BASE_PATH "./"
#define EXPORT
#endif

extern "C" void EXPORT make_stl() {
    using namespace io_stl;

    float z = +0.0;
    float p = +1.0;
    float n = -1.0;

    Point p0( n, n, n );
    Point p1( n, n, p );
    Point p2( n, p, n );
    Point p3( n, p, p );
    Point p4( p, n, n );
    Point p5( p, n, p );
    Point p6( p, p, n );
    Point p7( p, p, p );

    Point px( p, z, z );
    Point nx( n, z, z );
    Point py( z, p, z );
    Point ny( z, n, z );
    Point pz( z, z, p );
    Point nz( z, z, n );

    STL stl( {
        // +X
        Triangle( px, p4, p6, p7 ),
        Triangle( px, p4, p7, p5 ),
        // -X
        Triangle( nx, p0, p1, p3 ),
        Triangle( nx, p0, p3, p2 ),
        // +Y
        Triangle( py, p6, p2, p3 ),
        Triangle( py, p6, p3, p7 ),
        // -Y
        Triangle( ny, p0, p4, p5 ),
        Triangle( ny, p0, p5, p1 ),
        // +Z
        Triangle( pz, p1, p5, p7 ),
        Triangle( pz, p1, p7, p3 ),
        // -Z
        Triangle( nz, p0, p2, p6 ),
        Triangle( nz, p0, p6, p4 ),
    } );
    stl.SetTextHeader( "STL File" );

    FILE* f = fopen( BASE_PATH "mycube.stl", "wb" );
    if( f ) {
        stl.WriteBinary( f );
        fclose( f );

#ifdef EMSCRIPTEN
        EM_ASM(
            FS.syncfs( false, function( err ) {
                assert( !err );
                console.log( "Synced /indexdb contents." );
            } ); );
#endif
    } else {
        fprintf( stderr, "Failed to open STL file for writing.\n" );
    }
}

int main( int, char** ) {
#ifdef EMSCRIPTEN
    EM_ASM(
        FS.mkdir( '/indexdb' );
        FS.mount( IDBFS, {}, '/indexdb' );

        FS.syncfs( true, function( err ) {
            assert( !err );
            console.log( "Created /indexdb." );
            ccall( 'make_stl', 'v' );
        } ); );
    emscripten_exit_with_live_runtime();
#else
    make_stl();
#endif
}
