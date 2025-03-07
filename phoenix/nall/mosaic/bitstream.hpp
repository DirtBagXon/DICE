#ifdef NALL_MOSAIC_INTERNAL_HPP

namespace nall {
namespace mosaic {

struct bitstream {
  filemap fp;
  uint8_t* data = nullptr;
  unsigned size = 0;
  bool readonly = false;
  bool endian = 1;

  bool read(uint64_t addr) const {
    if(data == nullptr || (addr >> 3) >= size) return 0;
    unsigned mask = endian == 0 ? (0x01 << (addr & 7)) : (0x80 >> (addr & 7));
    return data[addr >> 3] & mask;
  }

  void write(uint64_t addr, bool value) {
    if(data == nullptr || readonly == true || (addr >> 3) >= size) return;
    unsigned mask = endian == 0 ? (0x01 << (addr & 7)) : (0x80 >> (addr & 7));
    if(value == 0) data[addr >> 3] &= ~mask;
    if(value == 1) data[addr >> 3] |=  mask;
  }

  bool open(const string& filename) {
    readonly = false;
    if(fp.open(filename, filemap::mode::readwrite) == false) {
      readonly = true;
      if(fp.open(filename, filemap::mode::read) == false) {
        return false;
      }
    }
    data = fp.data();
    size = fp.size();
    return true;
  }

  void close() {
    fp.close();
    data = nullptr;
  }

  bitstream() {
  }

  ~bitstream() {
    close();
  }
};

}
}

#endif
