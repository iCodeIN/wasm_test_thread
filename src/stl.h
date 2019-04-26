#include <stdint.h>
#include <stdio.h>

#include <functional>
#include <string>
#include <vector>

namespace io_stl {
    class Point {
    public:
        Point();
        Point( float x, float y, float z );

        void WriteBinary( FILE* f ) const;
        float x() const;
        float y() const;
        float z() const;
        Point Unit() const;

    private:
        static constexpr int X          = 0;
        static constexpr int Y          = 1;
        static constexpr int Z          = 2;
        static constexpr int DIMENSIONS = 3;

        float values_[DIMENSIONS];
    };

    class Triangle {
    public:
        Triangle();
        Triangle(
            const Point&                normal,
            const Point&                a,
            const Point&                b,
            const Point&                c,
            const std::vector<uint8_t>& attribute = std::vector<uint8_t>() );

        void WriteBinary( FILE* f ) const;
        Point a() const;
        Point b() const;
        Point c() const;
        Point Normal() const;
        void FlipNormal( bool onlyToAgree = false );
        Point VertexNormal() const;
        void FlipWinding( bool onlyToAgree = false );
        float NormalDotVertexNormal() const;

    private:
        static constexpr int A        = 0;
        static constexpr int B        = 1;
        static constexpr int C        = 2;
        static constexpr int VERTICES = 3;

        Point                normal_;
        Point                vertices_[VERTICES];
        std::vector<uint8_t> attribute_;
    };

    class STL {
    public:
        STL( const std::vector<Triangle>& triangles = std::vector<Triangle>() );
        STL( const std::string&           name,
             const std::vector<Triangle>& triangles = std::vector<Triangle>() );
        STL( const std::vector<uint8_t>&  header,
             const std::vector<Triangle>& triangles = std::vector<Triangle>() );

        void SetTextHeader( const std::string& text );
        void WriteBinary( FILE* f ) const;
        void ForAllTriangles( std::function<void( Triangle& )> kernel );

    private:
        void Constructor(
            const std::string&           name,
            const std::vector<uint8_t>&  header,
            const std::vector<Triangle>& triangles );
        void FixHeader();

        static constexpr int  HEADER_LENGTH = 80;
        std::string           name_;
        std::vector<uint8_t>  header_;
        std::vector<Triangle> triangles_;
    };
}
