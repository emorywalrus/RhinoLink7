#pragma once

#include <string>
#include <msclr/marshal_cppstd.h>

using namespace msclr::interop;
using namespace RhinoCaller3;


__declspec(dllexport) void* launch_rhino(void* window_handle, void* ui_ptr) {
	return Caller3::launch_rhino(window_handle, ui_ptr);
}

__declspec(dllexport) void initialize_rhino() {
	Caller3::initialize_rhino();
}

__declspec(dllexport) void destroy_rhino(void* caller) {
	Caller3::destroy_rhino(caller);
}

__declspec(dllexport) std::string get_object_string() {
	return marshal_as<std::string>(Caller3::get_object_string());
}

__declspec(dllexport) void save_settings_to_file() {
	Caller3::save_settings_to_file();
}

__declspec(dllexport) void load_settings_from_file() {
	Caller3::load_settings_from_file();
}