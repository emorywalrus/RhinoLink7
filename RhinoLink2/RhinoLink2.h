#pragma once

#include <string>
#include <msclr/marshal_cppstd.h>
#include <Windows.h>

using namespace msclr::interop;
using namespace RhinoCaller3;


// pass window handle and pointer to the QTUI to C#, so Rhino knows what to attach to. I'm sure theres a better way than passing generic pointers.
__declspec(dllexport) void launch_rhino(void* window_handle, void* ui_ptr) {
	Caller3::launch_rhino(window_handle, ui_ptr);
}

__declspec(dllexport) void initialize_rhino() {
	Caller3::initialize_rhino();
}

__declspec(dllexport) void destroy_rhino() {
	Caller3::destroy_rhino();
}

__declspec(dllexport) std::string get_history() {
	return marshal_as<std::string>(Caller3::get_history());
}

__declspec(dllexport) void run_command(std::string command) {
	Caller3::run_command(marshal_as<System::String^>(command));
}

__declspec(dllexport) std::string get_prompt() {
	return marshal_as<std::string>(Caller3::get_prompt());
}