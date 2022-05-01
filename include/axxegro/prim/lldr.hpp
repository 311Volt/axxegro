#ifndef INCLUDE_AXXEGRO_PRIM_LLDR
#define INCLUDE_AXXEGRO_PRIM_LLDR

#include "axxegro/resources/Bitmap.hpp"
#include "axxegro/resources/Resource.hpp"
#include <axxegro/math/math.hpp>
#include <axxegro/Color.hpp>

#include <cstddef>
#include <memory>
#include <optional>
#include <tcb/span.hpp>
#include <allegro5/allegro_primitives.h>

#include "CustomVertex.hpp"


namespace al {
    
    class Vertex: public ALLEGRO_VERTEX {
        Vertex(const Vec3<> pos, const Vec2<> tex, Color color);

        void setPos(const Vec3<> pos);
        void setUV(const Vec2<> pos);
        void setColor(const Color color);

        Vec3<> getPos() const;
        Vec2<> getUV() const;
        Color getColor() const;
    };

    

    inline void DrawPrim(
        const tcb::span<Vertex> vertices, 
        const Bitmap* texture, 
        ALLEGRO_PRIM_TYPE type = ALLEGRO_PRIM_TRIANGLE_LIST,
        int start = 0,
        int end = -1
    )
    {
        al_draw_prim(
            vertices.data(), 
            nullptr, 
            texture->constPtr(), 
            start, 
            end==-1 ? vertices.size() : end, 
            type
        );
    }

    inline void DrawIndexedPrim(
        const tcb::span<Vertex> vertices, 
        const tcb::span<int> indices, 
        const Bitmap* texture, 
        ALLEGRO_PRIM_TYPE type = ALLEGRO_PRIM_TRIANGLE_LIST
    )
    {
        al_draw_indexed_prim(
            vertices.data(), 
            nullptr, 
            texture->constPtr(), 
            indices.data(), 
            vertices.size(), 
            type
        );
    }

    template<typename VDecl>
    void DrawPrim(
        const tcb::span<typename VDecl::VertexT> vertices,
        const Bitmap* texture,
        ALLEGRO_PRIM_TYPE type = ALLEGRO_PRIM_TRIANGLE_LIST,
        int start = 0,
        int end = -1
    )
    {
        static std::optional<VertexDecl> vd = std::nullopt;
        if(!vd) {
            vd = VertexDecl::Init<VDecl>();
        }
        al_draw_prim(
            vertices.data(), 
            vd->get(), 
            texture->constPtr(), 
            start, 
            end==-1 ? vertices.size() : end, 
            type
        );
    }

    template<typename VDecl>
    void DrawIndexedPrim(
        const tcb::span<typename VDecl::VertexT> vertices,
        const tcb::span<int> indices,
        const Bitmap* texture,
        ALLEGRO_PRIM_TYPE type = ALLEGRO_PRIM_TRIANGLE_LIST
    )
    {
        static std::optional<VertexDecl> vd = std::nullopt;
        if(!vd) {
            vd = VertexDecl::Init<VDecl>();
        }
        al_draw_indexed_prim(
            vertices.data(), 
            nullptr, 
            texture->constPtr(), 
            indices.data(), 
            vertices.size(), 
            type
        );
    }
/*
    class VertexBufferDeleter {
    public:
        void operator()(ALLEGRO_VERTEX_BUFFER* p){al_destroy_vertex_buffer(p);}
    };

    class IndexBufferDeleter {
    public:
        void operator()(ALLEGRO_INDEX_BUFFER* p){al_destroy_index_buffer(p);}
    };

    class IndexBuffer: Resource<ALLEGRO_INDEX_BUFFER, IndexBufferDeleter> {
    public:
        IndexBuffer(const tcb::span<int> indices, int flags = ALLEGRO_PRIM_BUFFER_STATIC);
    };

    class VertexBuffer: Resource<ALLEGRO_VERTEX_BUFFER, VertexBufferDeleter> {
    public:
        VertexBuffer(const tcb::span<Vertex> vertices, int flags = ALLEGRO_PRIM_BUFFER_STATIC);

    };
*/

}

#endif //INCLUDE_AXXEGRO_PRIM_LLDR