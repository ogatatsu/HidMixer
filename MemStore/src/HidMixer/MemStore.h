
#pragma once

#include <Adafruit_LittleFS.h>
#include <InternalFileSystem.h>

namespace HidMixer
{

  class MemStore
  {
  public:
    MemStore(const char *directory);
    bool load(const char *name, void *buf, size_t size);
    bool save(const char *name, const void *buf, size_t size);
    bool remove(const char *name);
    bool clear();

  private:
    bool begin();

    bool _begun;
    String _directory;
    Adafruit_LittleFS_Namespace::File _file;
  };

}
