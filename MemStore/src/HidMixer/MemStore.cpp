
#include "HidMixer/MemStore.h"
#include "verify.h"

namespace HidMixer
{

  MemStore::MemStore(const char *directory) : _begun(false), _directory(directory), _file(InternalFS)
  {
  }

  bool MemStore::begin()
  {
    if (_begun)
    {
      return true;
    }

    VERIFY(InternalFS.begin());
    if (InternalFS.exists(_directory.c_str()) == false)
    {
      VERIFY(InternalFS.mkdir(_directory.c_str()));
    }
    _begun = true;

    return true;
  }

  bool MemStore::load(const char *name, void *buf, size_t size)
  {
    VERIFY(begin());

    bool result = false;

    String path = _directory + '/' + name;

    VERIFY(_file.open(path.c_str(), Adafruit_LittleFS_Namespace::FILE_O_READ));

    if (size == _file.size())
    {
      _file.read(buf, size);
      result = true;
    }
    _file.close();

    return result;
  }

  bool MemStore::save(const char *name, const void *buf, size_t size)
  {
    VERIFY(begin());

    String path = _directory + '/' + name;

    if (InternalFS.exists(path.c_str()))
    {
      VERIFY(InternalFS.remove(path.c_str()));
    }

    VERIFY(_file.open(path.c_str(), Adafruit_LittleFS_Namespace::FILE_O_WRITE));
    _file.write(static_cast<const uint8_t *>(buf), size);
    _file.close();

    return true;
  }

  bool MemStore::remove(const char *name)
  {
    VERIFY(begin());

    String path = _directory + '/' + name;

    if (InternalFS.exists(path.c_str()))
    {
      return InternalFS.remove(path.c_str());
    }
    return false;
  }

  bool MemStore::clear()
  {
    VERIFY(begin());

    VERIFY(InternalFS.rmdir_r(_directory.c_str()));

    VERIFY(InternalFS.mkdir(_directory.c_str()));

    return true;
  }

}
