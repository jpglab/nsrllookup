/* Copyright (c) 2012-2013, Robert J. Hansen <rjh@sixdemonbag.org>
 *
 * Permission to use, copy, modify, and/or distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#include "common.hpp"
#include <vector>
#include <string>
#include <algorithm>

using std::string;
using std::vector;
using std::find;
using std::remove;
using std::ofstream;

extern ofstream* HIT_FILE;
extern ofstream* MISSES_FILE;
extern NetworkSocket* GLOBAL_SOCK;

vector<string>* tokenize(const string& line, const char delim)
{
    string::const_iterator begin = line.begin();
    string::const_iterator end = line.begin();
    vector<string>* rv = new vector<string>();

    while (begin != line.end()) {
        end = find(begin + 1, line.end(), delim);
        rv->push_back(string(begin, end));
        begin = end + (end == line.end() ? 0 : 1);
    }
    for (size_t idx = 0 ; idx < rv->size() ; ++idx) {
        string& line = rv->at(idx);
        line.erase(remove(line.begin(), line.end(), '\r'), line.end());
        line.erase(remove(line.begin(), line.end(), '\n'), line.end());
    }
    rv->erase(remove(rv->begin(), rv->end(), ""), rv->end());
    return rv;
}

/* This abomination comes to you courtesy of the Win32 API. */
void bomb(int code)
{
    if (GLOBAL_SOCK) {
        delete GLOBAL_SOCK;
    }
    if (HIT_FILE) {
        delete HIT_FILE;
    }
    if (MISSES_FILE) {
        delete MISSES_FILE;
    }
#ifdef WINDOWS
    WSACleanup();
    ExitProcess(code);
#else
    exit(code);
#endif
}
