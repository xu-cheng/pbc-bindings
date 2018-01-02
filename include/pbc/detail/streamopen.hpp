/*
  The MIT License (MIT)
  Copyright (c) 2016-2018 Xu Cheng
  See licensing at:
    https://opensource.org/licenses/MIT
*/

#pragma once

#include <cstdio>
#include <sstream>

namespace pbc
{
    namespace streamopen
    {
#if defined(__APPLE__) || defined(__FreeBSD__)
        typedef struct stream_fd {
            FILE* fd;
        } stream_fd_t, *stream_fd_ptr;

        inline int streamwritefn(void* cookie, const char* data, int size)
        {
            std::ostream* buf = static_cast<std::ostream*>(cookie);
            buf->write(data, size);
            return size;
        }

        inline stream_fd_ptr streamopen(std::ostream& buf)
        {
            stream_fd_ptr sfd = new stream_fd_t;
            sfd->fd = ::fwopen(&buf, streamwritefn);
            return sfd;
        }

        inline void streamclose(stream_fd_ptr sfd)
        {
            std::fflush(sfd->fd);
            std::fclose(sfd->fd);
            delete sfd;
        }
#elif defined(__linux__)
        typedef struct stream_fd {
            FILE* fd;
            std::ostream* buf;
            char* data;
            size_t size;
        } stream_fd_t, *stream_fd_ptr;

        inline stream_fd_ptr streamopen(std::ostream& buf)
        {
            stream_fd_ptr sfd = new stream_fd_t;
            sfd->fd = ::open_memstream(&sfd->data, &sfd->size);
            sfd->buf = &buf;
            return sfd;
        }

        inline void streamclose(stream_fd_ptr sfd)
        {
            std::fflush(sfd->fd);
            std::fclose(sfd->fd);
            sfd->buf->write(sfd->data, sfd->size);
            delete[] sfd->data;
            delete sfd;
        }
#else
        typedef struct stream_fd {
            FILE* fd;
            std::ostream* buf;
        } stream_fd_t, *stream_fd_ptr;

        inline stream_fd_ptr streamopen(std::ostream& buf)
        {
            stream_fd_ptr sfd = new stream_fd_t;
            sfd->fd = std::tmpfile();
            sfd->buf = &buf;
            return sfd;
        }

        inline void streamclose(stream_fd_ptr sfd)
        {
            char buf[1024];
            int count;
            std::fflush(sfd->fd);
            std::rewind(sfd->fd);
            while ((count = std::fread(buf, sizeof(char), 1024, sfd->fd)) > 0)
                sfd->buf->write(buf, count);
            std::fclose(sfd->fd);
            delete sfd;
        }
#endif
    }  // namespace streamopen
}  // namespace pbc
