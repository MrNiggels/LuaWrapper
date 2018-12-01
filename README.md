# LuaWrapper
A C++ lua wrapper for the game "Roblox", these types of wrappers are typically used in executing scripts forcefully on the client via an external source

# How?
you have the luastate of roblox and the lua state of your external source
the metamethods are wrapped so whatever is done in yours in done in roblox

# Can I Use This?
sure feel free idc

# Warnings
wrapping is by far the easiest method of achiving script execution on the roblox client, but it is also the most suseptable to lag/memory leaks but if you can actually code you should be able to plug them

edit: this is not tested but should work

this still requires you to write out the typedefs and the definitions for the roblox lua functions and requires you to update them

this also still requires you to get an int3breakpoint handler
=

CREDITS TO AUTUMN FOR THE METHOD OF WRAPPING!
