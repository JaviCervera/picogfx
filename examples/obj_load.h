#pragma once

#include <string>
#include <vector>
#include "../include/picogfx.h"
#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader/tiny_obj_loader.h"

std::vector<geom_t*> obj_load(const char* filename) {
  std::vector<geom_t*> geoms;

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
    std::vector<vertex_t> vertices(attrib.vertices.size());
    std::vector<unsigned short> indices;
    const tinyobj::shape_t& shape = shapes[s];
    size_t index_offset = 0;
    for (int f = 0; f < shape.mesh.num_face_vertices.size(); ++f) {
      const int num_indices = shape.mesh.num_face_vertices[f];
      for (int i = 0; i < num_indices; ++i) {
        const tinyobj::index_t idx = shape.mesh.indices[index_offset + i];
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
        vertex_init_ex(&vertices[idx.vertex_index], vx, vy, vz, nx, ny, nz, tx, ty, tz, 0xffffffff, u, v, u, v);
      }
      indices.push_back(shape.mesh.indices[index_offset + 0].vertex_index);
      indices.push_back(shape.mesh.indices[index_offset + 2].vertex_index);
      indices.push_back(shape.mesh.indices[index_offset + 1].vertex_index);
      index_offset += num_indices;
    }
    geoms.push_back(geom_create(rm_triangles, &vertices[0], vertices.size(), &indices[0], indices.size()));
  }
  return geoms;
}
