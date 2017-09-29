/**
 * Copyright (c) 2017 Melown Technologies SE
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * *  Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */
/**
 * \desc: Zip file support
 * \file: zip.hpp
 * \author: Vaclav Blazek <vaclav.blazek@citationtech.net>
 */

#ifndef utility_zip_hpp_included_
#define utility_zip_hpp_included_

#include <cstdint>
#include <cstdlib>
#include <stdexcept>
#include <string>
#include <vector>
#include <limits>

#include <boost/filesystem/path.hpp>
#include <boost/iostreams/filtering_stream.hpp>

#include "./filedes.hpp"
#include "./substream.hpp"

namespace utility { namespace zip {

struct Error : std::runtime_error {
    Error(const std::string &e) : std::runtime_error(e) {}
};

struct BadSignature : Error {
    BadSignature(const std::string &e) : Error(e) {}
};

struct PluggedFile {
    boost::filesystem::path path;
    std::size_t uncompressedSize;
    bool seekable;

    PluggedFile(boost::filesystem::path path, std::size_t uncompressedSize
                , bool seekable)
        : path(path), uncompressedSize(uncompressedSize), seekable(seekable)
    {}
};

class Reader {
public:
    /** Opens ZIP file.
     *
     * If asked to paths are sanitizes:
     *     1) backslashes are converted to forward slashes (yes, there are
     *        such ZIP archvies...)
     *     2) multiple slashes are replaced with single slash
     *     3) any occurrence of dot and double-dot is resolved
     *     4) paths are fixed to start from root, i.e they start wih /
     *
     *  Rationale behind 4: ZIP archive works as a full filesystem with
     *  predictable paths.
     *
     * \param path path to archive
     * \param limit limit number of files read into file list
     * \param sanitizePaths sanities paths
     */
    Reader(const boost::filesystem::path &path
           , std::size_t limit = std::numeric_limits<std::size_t>::max()
           , bool sanitizePaths = true);

    /** File record.
     */
    struct Record {
        typedef std::vector<Record> list;

        std::size_t index;
        boost::filesystem::path path;
        std::size_t headerStart;

        Record(std::size_t index, const boost::filesystem::path &path
               , std::size_t headerStart)
            : index(index), path(path), headerStart(headerStart)
        {}
    };

    const Record::list& files() const { return records_; }

    /** Plug decompressing stream for file at given index at the end of the
     *  filtering_istream.
     */
    PluggedFile plug(std::size_t index
                     , boost::iostreams::filtering_istream &fis) const;

private:
    boost::filesystem::path path_;

    /** Open file descriptor.
     */
    Filedes fd_;

    /** Total lenght of file
     */
    std::size_t fileLength_;

    /** List of records.
     */
    Record::list records_;
};

} } // namespace utility::zip

#endif // utility_zip_hpp_included_
