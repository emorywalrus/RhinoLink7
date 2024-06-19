#pragma once

#include <string>
#include <msclr/marshal_cppstd.h>
#include <Windows.h>

using namespace msclr::interop;
using namespace RhinoCaller3;

__declspec(dllexport) void launch_rhino(void* window_handle, void* ui_ptr) {
	Caller3::launch_rhino(window_handle, ui_ptr);
}

__declspec(dllexport) void initialize_rhino() {
	Caller3::initialize_rhino();
}

__declspec(dllexport) void destroy_rhino() {
	Caller3::destroy_rhino();
}

__declspec(dllexport) void write_to_cmd_line(std::string line) {
	Caller3::write_to_cmd_line(marshal_as<System::String^>(line));
}

__declspec(dllexport) std::string get_history() {
	return marshal_as<std::string>(Caller3::get_history());
}
