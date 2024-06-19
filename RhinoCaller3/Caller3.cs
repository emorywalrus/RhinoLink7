using System;
using System.Runtime.InteropServices;
using System.Threading;
using Rhino.Runtime.InProcess;
using System.IO;
using Rhino;



namespace RhinoCaller3
{
    unsafe
    public static class Caller3
    {
        public static RhinoCore core;
        public static void* window_handle;
        public static void* qtui_ptr;
        public static Thread rhino_thread;
        public static RhinoApp.CommandLineTextWriter command_writer;

        public static void initialize_rhino()
        {
            RhinoInside.Resolver.Initialize();
        }
        // static funtion to be ran from the linker
        // saves handle in caller class, which is then used to make the core.
        public static void launch_rhino(void* in_window_handle, void* in_qtui_ptr)
        {
            window_handle = in_window_handle;
            qtui_ptr = in_qtui_ptr;

            rhino_thread = new Thread(run_core);
            rhino_thread.IsBackground = true;
            rhino_thread.SetApartmentState(ApartmentState.STA);
            rhino_thread.Start();
        }
        
        [DllImport("QTUI.exe")]
        extern static void lock_rhino_time(void* ui_ptr, void* rhino_handle);
        // creates and runs core. only works when ran in a new STA configured thread
        static void run_core()
        {
            copy_settings();

            string[] args = new string[] { "/nosplash", "/Scheme=EmbedScheme" };
            core = new RhinoCore(args, WindowStyle.Hidden, (IntPtr)window_handle);

            lock_rhino_time(qtui_ptr, (void*)RhinoApp.MainWindowHandle());

            core.Run();
        }

        public static void destroy_rhino()
        {
            Action a = () =>
            {
                core.Dispose();
                core = null;
            };
            RhinoApp.Exit();
            if (core != null)
            {
                RhinoApp.InvokeOnUiThread(a);
            }
        }
        // copys xml files for EmbedScheme into the default rhino settings directory
        // if rhino is installed in a non-default way, and the settings directory is someplace else, this will break
        static void copy_settings()
        {
            string asm_loc = System.Reflection.Assembly.GetExecutingAssembly().Location;
            string root_folder = Path.GetDirectoryName(asm_loc);

            string settings_source = Path.Combine(
                root_folder,
                "settings"
                );

            string settings_target = Path.Combine(
                Environment.GetFolderPath(Environment.SpecialFolder.ApplicationData),
                "McNeel",
                "Rhinoceros",
                "8.0",
                "settings"
                );

            new Microsoft.VisualBasic.Devices.Computer().FileSystem.CopyDirectory(settings_source, settings_target, true);
        }
        public static void write_to_cmd_line(String line)
        {
            RhinoApp.SendKeystrokes(line, true);
        }

        public static String get_history()
        {
            return RhinoApp.CommandHistoryWindowText;
        }
    }
}
