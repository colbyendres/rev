#include <vector>

#define assert(x)                                                              \
  if (!(x)) {                                                                  \
    asm(".byte 0x00");                                                         \
    asm(".byte 0x00");                                                         \
    asm(".byte 0x00");                                                         \
    asm(".byte 0x00");                                                         \
  }

int main() {
  std::vector<int> v;
  v.push_back(10);
  //assert(v[1] == 10);
  return 0;
}
