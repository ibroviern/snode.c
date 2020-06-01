#ifndef DOXYGEN_SHOULD_SKIP_THIS

#include <errno.h>
#include <fcntl.h>
#include <unistd.h>

#endif /* DOXYGEN_SHOULD_SKIP_THIS */

#define MFREADSIZE 16384

#include "FileReader.h"
#include "Multiplexer.h"


FileReader::FileReader(int fd, const std::function<void(char* data, int len)>& junkRead, const std::function<void(int err)>& onError)
    : Descriptor(fd)
    , Reader(onError)
    , junkRead(junkRead)
    , writerOK(true) {
}


FileReader* FileReader::read(std::string path, const std::function<void(char* data, int len)>& junkRead,
                             const std::function<void(int err)>& onError) {
    FileReader* fileReader = 0;

    int fd = open(path.c_str(), O_RDONLY);

    if (fd >= 0) {
        fileReader = new FileReader(fd, junkRead, onError);
        Multiplexer::instance().getManagedReader().add(fileReader);
    } else {
        onError(errno);
    }

    return fileReader;
}


void FileReader::stop() {
    Multiplexer::instance().getManagedReader().remove(this);
}


void FileReader::unmanaged() {
    delete this;
}


void FileReader::readEvent() {
    char buffer[MFREADSIZE];

    int ret = ::read(this->getFd(), buffer, MFREADSIZE);

    if (writerOK) {
        if (ret > 0) {
            this->junkRead(buffer, ret);
        } else if (ret == 0) {
            this->stop();
            this->onError(0);
        } else {
            this->stop();
            this->onError(errno);
        }
    } else {
        this->stop();
    }
}


void FileReader::writerGone() {
    writerOK = false;
}
