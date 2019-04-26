#include <endian.h>
#include <math.h>
#include <string.h>

#include "stl.h"

namespace io_stl {
    Point::Point() {
        for( auto& value : values_ ) {
            value = 0.0f;
        }
    }

    Point::Point( float x, float y, float z ) {
        values_[X] = x;
        values_[Y] = y;
        values_[Z] = z;
    }

    void Point::WriteBinary( FILE* f ) const {
        for( const auto& value : values_ ) {
            uint32_t pun = htole32( *reinterpret_cast<const uint32_t*>( &value ) );
            fwrite( &pun, sizeof( uint32_t ), 1, f );
        }
    }

    float Point::x() const {
        return values_[X];
    }

    float Point::y() const {
        return values_[Y];
    }

    float Point::z() const {
        return values_[Z];
    }

    Point Point::Unit() const {
        float length = sqrt(
            pow( x(), 2 ) +
            pow( y(), 2 ) +
            pow( z(), 2 ) );

        return Point(
            x() / length,
            y() / length,
            z() / length );
    }

    Triangle::Triangle() {
    }

    Triangle::Triangle(
        const Point&                normal,
        const Point&                a,
        const Point&                b,
        const Point&                c,
        const std::vector<uint8_t>& attribute ) {
        normal_      = normal;
        vertices_[A] = a;
        vertices_[B] = b;
        vertices_[C] = c;
    }

    void Triangle::WriteBinary( FILE* f ) const {
        normal_.WriteBinary( f );
        for( const Point& vertex : vertices_ ) {
            vertex.WriteBinary( f );
        }

        uint16_t value = htole16( attribute_.size() );
        fwrite( &value, sizeof( uint16_t ), 1, f );
    }

    Point Triangle::a() const {
        return vertices_[A];
    }

    Point Triangle::b() const {
        return vertices_[B];
    }

    Point Triangle::c() const {
        return vertices_[C];
    }

    Point Triangle::Normal() const {
        return normal_;
    }

    void Triangle::FlipNormal( bool onlyToAgree ) {
        if( onlyToAgree || ( 0 >= NormalDotVertexNormal() ) ) {
            return;
        }

        normal_ = Point( -normal_.x(), -normal_.y(), -normal_.z() );
    }

    Point Triangle::VertexNormal() const {
        Point vb(
            a().x() - b().x(),
            a().y() - b().y(),
            a().z() - b().z() );
        Point va(
            c().x() - b().x(),
            c().y() - b().y(),
            c().z() - b().z() );
        return Point(
            va.y() * vb.z() - va.z() * vb.y(),
            va.z() * vb.x() - va.x() * vb.z(),
            va.x() * vb.y() - va.y() * vb.x() );
    }

    void Triangle::FlipWinding( bool onlyToAgree ) {
        if( onlyToAgree || ( 0 >= NormalDotVertexNormal() ) ) {
            return;
        }

        Point k      = vertices_[A];
        vertices_[A] = vertices_[C];
        vertices_[C] = k;
    }

    float Triangle::NormalDotVertexNormal() const {
        Point stored = Normal();
        Point vertex = VertexNormal();
        return (
            stored.x() * vertex.x() +
            stored.y() * vertex.y() +
            stored.z() * vertex.z() );
    }

    STL::STL( const std::vector<Triangle>& triangles ) {
        Constructor( "", {}, triangles );
    }

    STL::STL( const std::string&           name,
              const std::vector<Triangle>& triangles ) {
        Constructor( name, {}, triangles );
    }

    STL::STL( const std::vector<uint8_t>&  header,
              const std::vector<Triangle>& triangles ) {
        Constructor( "", header, triangles );
    }

    void STL::SetTextHeader( const std::string& text ) {
        std::vector<uint8_t> header;
        header.resize( text.size() );
        memcpy( reinterpret_cast<char*>( header.data() ), text.c_str(), text.size() );
        header_ = header;
        FixHeader();
    }

    void STL::WriteBinary( FILE* f ) const {
        fwrite( header_.data(), sizeof( uint8_t ), header_.size(), f );

        uint32_t triangle_count = htole32( triangles_.size() );
        fwrite( &triangle_count, sizeof( uint32_t ), 1, f );

        for( const Triangle& triangle : triangles_ ) {
            triangle.WriteBinary( f );
        }
    }

    void STL::ForAllTriangles( std::function<void( Triangle& )> kernel ) {
        for( auto& triangle : triangles_ ) {
            kernel( triangle );
        }
    }

    void STL::Constructor(
        const std::string&           name,
        const std::vector<uint8_t>&  header,
        const std::vector<Triangle>& triangles ) {
        name_      = name;
        header_    = header;
        triangles_ = triangles;
        FixHeader();
    }

    void STL::FixHeader() {
        header_.resize( HEADER_LENGTH, ' ' );
    }
}
