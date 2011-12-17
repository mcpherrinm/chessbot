#include <gl/glfw.h>
#include <iostream>
#include <fstream>
#include <map>
#include <string>
#include <vector>
#include <assert.h>

struct mesh_t {
  uint32_t vbo_id, vao_id;
};

typedef std::map<std::string, mesh_t> meshDB_t;

void load_meshes(meshDB_t& meshes, std::string filename) {
  std::ifstream fin(filename.c_str());

  char c;
  fin >> c;
  uint32_t offset = 1;
  do {
    assert(c == 'o');
    mesh_t m;
    std::string name;
    std::vector<vec3f_t> verts;
    std::vector<uint32_t> indicies;
    fin >> name;
    fin >> c;
    assert(c == 'v');
    do {
      vec3f_t v;
      fin >> v.x;
      fin >> v.y;
      fin >> v.z;
      verts.push_back(v);
      fin >> c;
    } while (c == 'v');

    assert(c == 'f');
    do {
      for (int i = 0; i < 3; i++) {
        int32_t x;
        fin >> x;
        assert(x >= 0; x - offset < verts.size());
        indicies.push_back(x - offset);
      }
    } while (c == 'f');

    // ...... something something opengl

    models.push_back(m);
  } while (!fin.eof());
}

int main () {
  meshDB_t meshes;
  load_meshes(meshes, "arm.obj");

  return 0;
}
