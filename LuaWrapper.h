//this is really skidded so i wouldn't copy and paste this in and sell it
// i dont mind you using it but the original creators may mind lol


#pragma once
#include <Windows.h>
#include <string>
#include <iostream>

#include "RBLXOffsets.h"
#include "ConsoleCol.h"

#define R_L_TNIL						 0
#define R_L_TLIGHTUSERDATA				 1
#define R_L_TNUMBER					     2
#define R_L_TBOOLEAN					 3
#define R_L_TSTRING					     4
#define R_L_TTHREAD					     5
#define R_L_TFUNCTION					 6
#define R_L_TTABLE						 7
#define R_L_TUSERDATA					 8
#define R_L_TPROTO						 9
#define R_L_TUPVALUE					 10
#define R_L_TNONE                        11

int FuncHandle(lua_State* L);

int rlregistry;


void Wrap(DWORD RLS, lua_State *LS, int Index) {
	//todo add the printing/couting of the type used but make it so it is stable enough to not crash!
	switch (r_l_type(RLS,Index)){
	case R_L_TNIL: {
		lua_pushnil(LS);
		break;
	}
	case R_L_TLIGHTUSERDATA: {
		lua_pushlightuserdata(LS, nullptr);
		break;
	}
	case R_L_TNUMBER: {
		lua_pushnumber(LS, r_l_tonumber(RLS , Index));
		break;
	}
	case R_L_TBOOLEAN: {
		lua_pushboolean(LS, r_l_toboolean(RLS, Index));
		break;
	}
	case R_L_TSTRING: {
		lua_pushstring(LS, r_l_tolstring(RLS, Index, NULL));
		break;
	}
	case R_L_TTHREAD: {
		lua_newthread(LS);
		break;
	}
	case R_L_TFUNCTION: {
		lua_pushnumber(LS, r_l_lref(RLS, LUA_REGISTRYINDEX));
		lua_pushcclosure(LS, FuncHandle, 1);
		break;
	}
	case R_L_TTABLE: {
		lua_newtable(LS);
		r_l_pushnil(RLS);
		while (r_l_next(RLS, -2) != 0) {
			r_l_pushvalue(RLS, -2);
			const char *name = r_l_tolstring(RLS, -1, NULL);
			r_l_pushvalue(RLS, -2);
			Wrap(RLS, LS, -1);
			lua_setfield(LS, -2, name);
			r_l_pop(RLS, 2);
		};
		r_l_pop(RLS, 1);
		break;
	};
	case R_L_TUSERDATA: {
		r_l_pushvalue(RLS, Index);
		r_l_pushstring(RLS, std::to_string(++rlregistry).c_str());
		r_l_pushvalue(RLS, -2);
		r_l_settable(RLS, LUA_REGISTRYINDEX);
		r_l_pop(RLS, 1);
		lua_newuserdata(LS, 0);
		lua_pushvalue(LS, -1);
		lua_pushstring(LS, std::to_string(rlregistry).c_str());
		lua_settable(LS, LUA_REGISTRYINDEX);
		r_l_getmetatable(RLS, Index);
		lua_setmetatable(LS, -2);
		Wrap(RLS, LS, -1);
		r_l_pop(RLS, 1);
		break;
	}
	case R_L_TPROTO: {
		break;
	}
	case R_L_TUPVALUE: {
		break;
	}
	case R_L_TNONE: {
		lua_pushnil(LS);
		break;
	}
	default: break;
	}
}

void LuaFuncHandler();

void Unwrap(lua_State *LS, DWORD RLS, int Index) {
	//todo once again add the printing/couting of the type used
	switch (lua_type(LS, Index)) {
	case LUA_TNIL: {
		r_l_pushnil(RLS);
		break;
	}
	case LUA_TLIGHTUSERDATA: {
		r_l_pushlightuserdata(RLS, nullptr);
		break;
	}
	case LUA_TNUMBER: {
		r_l_pushnumber(RLS, lua_tonumber(LS, Index));
		break;
	}
	case LUA_TBOOLEAN: {
		r_l_pushboolean(RLS, lua_toboolean(LS, Index));
		break;
	}
	case LUA_TSTRING: {
		r_l_pushstring(RLS, lua_tostring(LS, Index));
		break;
	}
	case LUA_TTHREAD: {
		r_l_newthread(RLS);
		break;
	}
	case LUA_TFUNCTION: {
		lua_pushvalue(LS, Index);
		LuaFuncHandler();
		r_l_pushnumber(RLS, luaL_ref(LS, LUA_REGISTRYINDEX));
		r_l_pushcclosure(RLS, breakPOINTINT3[0], 1);
		break;
	}
	case LUA_TTABLE: {
		lua_pushvalue(LS, Index);
		r_l_createtable(RLS, 0, 0);
		lua_pushnil(LS);
		while (lua_next(LS, -2) != LUA_TNIL)
		{
			Unwrap(LS, RLS, -2);
			Unwrap(LS, RLS, -1);
			r_l_settable(RLS, -3);
			lua_pop(LS, 1);
		}
		lua_pop(LS, 1);
		break;
	}
	case LUA_TUSERDATA: {
		lua_pushvalue(LS, Index);
		lua_gettable(LS, LUA_REGISTRYINDEX);
		if (!lua_isnil(LS, -1)) {
			r_l_getfield(RLS, LUA_REGISTRYINDEX, lua_tostring(LS, -1));
		}
		else {
			r_l_newuserdata(RLS, 0);
		}
		lua_pop(LS, 1);
		break;
	}
	//tproto
	//tupvalue
	case LUA_TNONE: {
		r_l_pushnil(RLS);
		break;
	}
	default:break;
	}
}

lua_State* A_LS;
int rkey;

void LuaFuncHandler() {
	A_LS = LS;
	rkey++;
	std::string key = std::to_string(rkey) + std::string("A");
	lua_setfield(LS, LUA_REGISTRYINDEX, key.c_str());
}


int FuncHandle(lua_State* LS)
{
	int a = lua_gettop(LS);
	int k = lua_tonumber(LS, lua_upvalueindex(1));
	r_l_rawgeti(RLS, LUA_REGISTRYINDEX, k);
	for (int i = a; i != 0; i--) {
		Unwrap(LS, RLS, i * -1);
	}
	r_l_pcall(RLS, a, 1, 0);
	Wrap(RLS, LS, -1);

	if (LUA_ERRRUN) {
		std::cout << "There Was An Error While Executing:" << lua_tostring(LS, -1) << "\n";
		return 1;
	}
	if (LUA_ERRSYNTAX) {
		std::cout << "There Was An Error In Your Syntax:" << lua_tostring(LS, -1) << "\n";
		return 1;
	}
	else
	{
		std::cout << "Script Executed With No Found Errors!\n";
	}
	return 1;
}
