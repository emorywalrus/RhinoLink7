using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Threading;
using Rhino.Runtime.InProcess;
using System.IO;
using System.Runtime.Serialization;
using System.Xml.Serialization;
using System.Configuration;
using System.Reflection;
using Rhino.Display;
using Rhino;
using System.Drawing;



namespace RhinoCaller3
{
    unsafe
    public static class Caller3
    {
        public static RhinoCore core;
        public static void* window_handle;
        public static void* qtui_ptr;

        public static Thread rhino_thread;

        public static void initialize_rhino()
        {
            RhinoInside.Resolver.Initialize();
        }
        // static funtion to be ran from the linker
        // saves handle in caller class, which is then used to make the core.
        public static void launch_rhino(void* handle, void* qtui_ptr)
        {
            Caller3.window_handle = handle;
            Caller3.qtui_ptr = qtui_ptr;

            Caller3.rhino_thread = new Thread(Caller3.run_core);
            Caller3.rhino_thread.IsBackground = true;
            Caller3.rhino_thread.SetApartmentState(ApartmentState.STA);
            Caller3.rhino_thread.Start();
        }
        
        [DllImport("QTUI.exe")]
        extern static void lock_rhino_time(void* ui_ptr, void* rhino_handle);
        // creates and runs core. only works when ran in a new STA configured thread
        static void run_core()
        {
            core = new RhinoCore(new string[] { "/nosplash", "/Scheme=EmbedScheme" }, WindowStyle.Hidden, (IntPtr)window_handle);

            lock_rhino_time(qtui_ptr, (void*)RhinoApp.MainWindowHandle());

            core.Run();
            core.Dispose();
        }

        public static void destroy_rhino()
        {
            RhinoApp.Exit();
            Caller3.rhino_thread.Join();
        }

        public static string get_data()
        {
            return root_folder();
        }

        public static string root_folder()
        {
            return Path.GetDirectoryName(System.Reflection.Assembly.GetExecutingAssembly().Location);
        }
    }
}

