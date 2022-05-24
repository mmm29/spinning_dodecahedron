#include <cassert>
#include <unordered_map>

#include "sfml_renderer.h"

using render::SFMLRenderer;


void SFMLRenderer::SetRenderTarget(sf::RenderTarget *render_target) {
    render_target_ = render_target;
}

void SFMLRenderer::BindVertexBuffer(const Vertex *buffer, uint32_t count, PrimitiveTopology topology) {
    buffer_ = buffer;
    vertices_count_ = count;
    primitive_topology_ = topology;
}

static sf::Color ColorToSfmlColor(const Color &color) {
    return sf::Color(color.r, color.g, color.b, color.a);
}

void SFMLRenderer::Draw(uint32_t vertex_count, uint32_t first_vertex) {
    assert(render_target_ != nullptr);
    assert(buffer_ != nullptr);
    assert(first_vertex + vertex_count <= vertices_count_);

    static const std::unordered_map<PrimitiveTopology, sf::PrimitiveType> primitive_type_assoc = {
            {kLines,     sf::PrimitiveType::Lines},
            {kTriangles, sf::PrimitiveType::Triangles}
    };

    assert(primitive_type_assoc.count(primitive_topology_) != 0);

    sf::PrimitiveType primitive_type = primitive_type_assoc.at(primitive_topology_);
    sf::VertexArray vertices(primitive_type, vertex_count);

    for (size_t vertex_idx = first_vertex; vertex_idx < vertex_count; vertex_idx++) {
        const Vertex &vertex = buffer_[vertex_idx];
        vertices[vertex_idx] = sf::Vertex{sf::Vector2f(vertex.position[0], vertex.position[1]),
                                          ColorToSfmlColor(vertex.color)};
    }

    render_target_->draw(vertices);
}