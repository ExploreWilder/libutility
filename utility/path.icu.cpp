/**
 * Copyright (c) 2018 Melown Technologies SE
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

#include <unicode/translit.h>
#include <unicode/unistr.h>
#include <unicode/ucnv.h>

#include "dbglog/dbglog.hpp"

#include "./path.hpp"

namespace utility {

namespace {

std::unique_ptr<icu::Transliterator> createSanitizer()
{
    const std::string id("utility-path-sanitizer");

    // register transliterator
    const char *rules(R"RAW(
:: ANY-Latin ;     # convert everything to latin script
[^[:L:][:N:]]+ > '-' ;  # replace every non-letter sequence with dash
:: NFD ;           # canonical decomposition (á -> a´)
:: [:M:] Remove ;  # remove all accent marks
)RAW");

    UErrorCode status(U_ZERO_ERROR);
    UParseError parseError;
    std::unique_ptr<icu::Transliterator>
        normalizer(icu::Transliterator::createFromRules
                   (id.c_str(), rules, UTRANS_FORWARD, parseError, status));
    if (U_FAILURE(status)) {
        std::string preContext;
        icu::UnicodeString(parseError.preContext, U_PARSE_CONTEXT_LEN)
            .toUTF8String(preContext);
        std::string postContext;
        icu::UnicodeString(parseError.postContext, U_PARSE_CONTEXT_LEN)
            .toUTF8String(postContext);

        LOGTHROW(err1, std::runtime_error)
            << "Cannot parse path sanitization transliteration rules: "
            << u_errorName(status)
            << " at line " << parseError.line
            << " offset " << parseError.offset
            << " pre-context: \"" << preContext
            << "\", post-context: \"" << postContext
            << "\".";
    }

    return normalizer;
}

} // namespace

boost::filesystem::path sanitizePath(const boost::filesystem::path &path)
{
    boost::filesystem::path out;

    auto normalizer(createSanitizer());

    for (const auto &part : path) {
        if ((part == ".") || (part == "..")) {
            out /= part;
            continue;
        }

        auto uPart(icu::UnicodeString::fromUTF8(part.string()));
        normalizer->transliterate(uPart);

        std::string tPart;
        uPart.toUTF8String(tPart);
        out /= tPart;
    }

    return out;
}

} // namespace utility
