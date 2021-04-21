#pragma once

#include <string>
#include <vector>
#include "../include/picogfx.h"
#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader/tiny_obj_loader.h"

std::vector<picogfx::Geom*> LoadObj(const char* filename) {
    std::vector<picogfx::Geom*> geoms;

    // Load obj
    tinyobj::attrib_t attrib;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;
    std::string err;
    bool ret = tinyobj::LoadObj(&attrib, &shapes, &materials, &err, filename);
    if (!ret) {
        puts(err.c_str());
        return geoms;
    }

    // Parse vertices and indices from shapes
    for (int s = 0; s < shapes.size(); ++s) {
        std::vector<picogfx::Vertex> vertices(attrib.vertices.size());
        std::vector<unsigned short> indices;
        const tinyobj::shape_t& shape = shapes[s];
        size_t indexOffset = 0;
        for (int f = 0; f < shape.mesh.num_face_vertices.size(); ++f) {
            const int numIndices = shape.mesh.num_face_vertices[f];
            for (int i = 0; i < numIndices; ++i) {
                const tinyobj::index_t idx = shape.mesh.indices[indexOffset + i];
                const tinyobj::real_t vx = attrib.vertices[3*idx.vertex_index+0];
                const tinyobj::real_t vy = attrib.vertices[3*idx.vertex_index+1];
                const tinyobj::real_t vz = -attrib.vertices[3*idx.vertex_index+2];
                const tinyobj::real_t nx = idx.normal_index > -1 ? attrib.normals[3*idx.normal_index+0] : 0;
                const tinyobj::real_t ny = idx.normal_index > -1 ? attrib.normals[3*idx.normal_index+1] : 0;
                const tinyobj::real_t nz = idx.normal_index > -1 ? -attrib.normals[3*idx.normal_index+2] : -1;
                const tinyobj::real_t tx = 1;
                const tinyobj::real_t ty = 0;
                const tinyobj::real_t tz = 0;
                const tinyobj::real_t u = idx.texcoord_index > -1 ? attrib.texcoords[2*idx.texcoord_index+0] : 0;
                const tinyobj::real_t v = idx.texcoord_index > -1 ? -attrib.texcoords[2*idx.texcoord_index+1] : 0;
                vertices[idx.vertex_index] = picogfx::Vertex(vx, vy, vz, nx, ny, nz, tx, ty, tz, 0xffffffff, u, v);
            }
            indices.push_back(shape.mesh.indices[indexOffset + 0].vertex_index);
            indices.push_back(shape.mesh.indices[indexOffset + 2].vertex_index);
            indices.push_back(shape.mesh.indices[indexOffset + 1].vertex_index);
            indexOffset += numIndices;
        }
        geoms.push_back(picogfx::Geom::Create(&vertices[0], vertices.size(), &indices[0], indices.size()));
    }
    return geoms;
}
