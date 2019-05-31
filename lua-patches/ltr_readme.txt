http://lua-users.org/lists/lua-l/2008-11/msg00331.html

Hi,

The purpose of this patch is to reduce the RAM footprint of Lua. I
made it to help my eLua project (http://www.eluaproject.net), which is
basically Lua running on very RAM-constrained systems (at most 64k in
general).

Before diving into more details, please note that this is my first Lua
patch, and even more, my first attempt to understand the Lua code, so
the end result might look strange (or even downright freaky:)) to
those of you truly familiar with the Lua code. Thus, if you think that
there's something wrong with my code, chances are that you're right,
so please let me know and I'll try to fix it. Thank you.

================================================================================

Audience
========

For people that use Lua on desktops, this patch is probably of no
interest, as its purpose is to minimize RAM footprint (at the expense
of speed). If, on the other hand, you're trying to run Lua on a
resource contrained system or in a sandboxed environment, and you're
more concerned with memory usage than with speed, you might find this
patch useful.

Motivation
==========

The main thing that drove me to write this patch is Lua's memory
consumption at startup (obtained by running lua -e
"print(collectgarbage'count')"). It's about 17k for regular Lua 5.1.4,
and more than 25k for some of eLua's platforms. These figures are
mainly a result of registering many different modules to Lua. Each
time you register a module (via luaL_register) you create a new table
and populate it with the module's methods. But a table is a read/write
datatype, so luaL_register is quite inefficient if you don't plan to
do any write operations on that table later (adding new elements or
manipulating existing ones). I found that I almost never have to do
any such operations on a module's table after it was created, I just
query it for its elements. So, from the perspective of someone worried
about memory usage, I'd rather have a different type of table in this
case, one that wouldn't need any RAM at all, since it would be read
only, so it could reside entirely in ROM.
And there's one more thing related to this context: Lua's functions.
While Lua does have the concept of C functions, they still require
data structures that need to be allocated (see lua_pushcclosure in
lapi.c for details). Once again, this isn't something I use often.
Most of the times my functions (especially the ones in a C module) are
very simple, and they don't need upvalues or environments at all. In
conclusion, having a "simpler" function type would improve memory
usage.

Details
=======

The patch adds two new data types to Lua. Both or them are based on
the lightuserdata type already found in Lua, and they share the same
basic attributes: they don't need to be dynamically allocated and
they're compared in the same way lightuserdatas are compared (by
value). And of course, they are not collectable, so the garbage
collector won't have anything to do with them. The new types are:

1. lightfunctions: these are "simple" functions, in the sense that
they can't have upvalues or environments. They are just pointers to
regular C functions. Other than that, you can use them from Lua just
as you'd use any other function.

2. rotables: maybe a better name for them would've been "romtables",
but I found that to be a little too technical. They are read-only
tables, but unlike the read-only tables that one can already implement
in Lua with metamethods, they have a very specific property: they
don't need any RAM at all. They are fully constant, so they can be
read directly from ROM. Their only purpose is to allow one to register
a module without having to allocate a table in RAM, and because they
are such a "specialized" data type they have many restrictions:

- rotables can only contain values of type "lightfunction" or
lua_Number at the moment. "Embedded" rotables (rotables containing
references to other rotables) are planned for a future release.
- you can't iterate over rotables with pairs()
- you can't add/delete/modify elements from rotables (obviously)
- you can't set a rotable as a metatable (but I'd like to change this
in a future release if possible).
- rotables can't have metatables.

Just as with lightuserdata, you can only create lightfunctions and
rotables from C code, never from Lua itself.

How to use it
=============

The patch must be applied to an unmodified Lua 5.1.4 source tree. Once
you unpack the source tree and download the patch, do this:

$ cd lua-5.1.4
$ cp <patchlocation>/lua_tiny_ram.patch.gz .
$ gunzip lua_tiny_ram.patch.gz
$ patch -p1 < lua_tiny_ram.patch

Then edit lua-5.1.4/src/Makefile to set the optimization level for the patch:

CFLAGS= -O2 -Wall $(MYCFLAGS) -DLUA_OPTIMIZE_MEMORY=x, where x can be:

0: this doesn't actually optimize anything, in just tries to keep the
patched version as close as possible to the unpatched version in terms
of speed and functionality. This passes the full Lua 5.1 test suite
(see "Testing" below for details).

1: this applies a moderate optimization, the resulting code takes less
memory and still passes the full test suite. In this mode most of the
standard libraries are modified to use lightfunctions instead of
"regular" functions, and some of them are "converted" to use rotables
instead of regular tables.

2: aggresive optimizations, this modifies the standard libraries even
more than it does at level 1. The IO library is modified to use the
registry instead of environments, thus making it more
resource-friendly (the side effect being that this mode doesn't
support pipes). It also leaves the _G table with a single method
(__index) and sets it as its own metatable, so all the "standard" Lua
functions are now hidden behind a __index access. This mode passes
almost all the tests in the Lua 5.1 test suite, except for a few tests
from "db.lua" (which fail because the functions in "io" are now
lightfunctions and they can't have upvalues anymore) and a few more
from "nextvar.lua" (which fail because of reasons I don't fully
understand, but they have to do with the fact that _G's only method is
now __index).

To use it in your own code, call "luaL_register_light" instead of
"luaL_register" to register your module with lightfunctions istead of
regular functions. Also, you can completeley forget about registering
your module if you use rotables. See linit.c, mathlib.c and oslib.c
for an example of how to turn your regular code into code that uses
rotables.

Results
=======

All of the results come from running "lua -e
"print(collectgarbage'count')"" from the command line.

Regular Lua 5.1.4:           17.0966
Patched Lua 5.1.4, OPT = 0 : 17.0849 (the difference comes from a few
small changes in loadlib.c)
Patched Lua 5.1.4, OPT = 1 :  9.2011
Patched Lua 5.1.4, OPT = 2 :  5.8984

Ironically, I didn't yet have a chance to use this patch on eLua
itself, but I expect even better results there, since I'm registering
a lot more modules at Lua startup.

Before you ask: I don't have any information about how the patch
alters the execution speed, and I don't plan to do any soon, as this
is not at all something I'm concerned with at this point. Clearly the
virtual machine would be slower, but I have no idea how much slower.
Of course, you're more than welcomed to test this yourself and post
your results.

Testing
=======

I tested my patch with the Lua 5.1 test suite
(http://lua-users.org/lists/lua-l/2006-03/msg00723.html). I had to
modify it a bit to take into account the new datatypes, the modified
test suite is available from ftp://ftp.berlios.de/pub/elua
(anonymous). The test suite was an excellent testing tool. I thought I
had the patch ready until I found the test suite and ran it. After
another week of work, I had something that could be called functional
:)

I tested everything via "make generic", which is how I always build
Lua for my embedded environments. This means (among other things) that
I didn't test pipes and dynamic module loading, although I don't see
why they wouldn't work (except for pipes, which won't work at
optmization level 2, as already explained).

I never tested the patch in a multithreaded environment with more
threads running different lua_States. I never even used regular Lua
like this, so I can't make asumptions about how my patch would behave
in a multithreaded environment. It doesn't use any global or static
variables, but you might encounter other problems with it.

================================================================================

Hope you'll find my patch interesting, and please don't refrain
yourself from commenting on it.

Best,
Bogdan
