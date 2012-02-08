/* update.h
 *
 * Copyright (c) 2011, 2012 Chantilly Robotics <chantilly612@gmail.com>
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

/*
 * Provide registry for update functions.  Just make a function and it'll
 * automagically get called when you register it!!!
 */
 
#ifndef UPDATE_H_INC
#define UPDATE_H_INC

#include <vector>

class update_registry {
public:
    typedef void(*update_func)(void*);
    void register_func(update_func, void*);
    void unregister_func(update_func, void*);
    void update();
private:
    class registry_entry {
    private:
        update_func func;
        void * arg;
    public:
        void operator()() {
            func(arg);
        }
        bool isNULL() {
            return (func == NULL);
        }
        bool operator==(const registry_entry& other) const {
            return (func == other.func && arg == other.arg);
        }
        bool operator!=(const registry_entry& other) const {
            return !(*this == other);
        }
        registry_entry(update_func f, void * v) : func(f), arg(v) {}
    };
    std::vector<registry_entry> registry;
    std::vector<registry_entry>::iterator find_entry(const registry_entry&);
};

update_registry& registry();

#endif
