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
    public class Caller3
    {
        public RhinoCore core;
        public void* window_handle;
        public void* qtui_ptr;

        public Thread rhino_thread;

        public static void initialize_rhino()
        {
            RhinoInside.Resolver.Initialize();
        }
        // static funtion to be ran from the linker
        // saves handle in caller class, which is then used to make the core.
        public static void* launch_rhino(void* handle, void* qtui_ptr)
        {
            Caller3 caller = new Caller3();
            caller.window_handle = handle;
            caller.qtui_ptr = qtui_ptr;

            caller.rhino_thread = new Thread(caller.run_core);
            caller.rhino_thread.IsBackground = true;
            caller.rhino_thread.SetApartmentState(ApartmentState.STA);
            caller.rhino_thread.Start();

            return (void*)&caller;
        }
        
        [DllImport("QTUI.exe")]
        extern static void lock_rhino_time(void* ui_ptr, void* rhino_handle);
        // creates and runs core. only works when ran in a new STA configured thread
        void run_core()
        {
            core = new RhinoCore(null, WindowStyle.Normal, (IntPtr)window_handle);

            RhinoApp.RunScript("-WindowLayout Default", false);

            Rhino.ApplicationSettings.AppearanceSettings.CommandPromptPosition
                = Rhino.ApplicationSettings.CommandPromptPosition.Floating;

            string viewportName = "Embedded Viewport";
            DefinedViewportProjection projection = DefinedViewportProjection.Perspective;
            Rectangle locationAndSize = new Rectangle(0, 0, 500, 500);
            bool floating = true;

            RhinoDoc.ActiveDoc.Views.Add(viewportName, projection, locationAndSize, floating);

            RhinoApp.RunScript("-SaveWindowLayout embedded", false);
            RhinoApp.RunScript("-WindowLayout embedded", false);

            lock_rhino_time(qtui_ptr, (void*)Rhino.RhinoApp.MainWindowHandle());
            
            core.Run();
            core.Dispose();
        }
        public void Dispose()
        {
            core.Dispose();
        }
        public static void destroy_rhino(void* caller)
        {
            Rhino.RhinoApp.Exit();
            ((Caller3*)caller)->rhino_thread.Join();
        }
        public static string get_object_string()
        {
            List<Rhino.DocObjects.RhinoObject> object_list = Rhino.RhinoDoc.ActiveDoc.Objects.GetObjectList(Rhino.DocObjects.ObjectType.AnyObject).ToList();
            string ret = "";
            for (int i = 0; i < object_list.Count; i++)
            {
                ret += object_list[i].Geometry.GetType().ToString() + "\n";
            }
            return ret;
        }
        public static void save_settings_to_file()
        {
            string root_path = System.IO.Path.GetDirectoryName(Assembly.GetEntryAssembly().Location);
        }
        public static void load_settings_from_file()
        {
            string root_path = System.IO.Path.GetDirectoryName(Assembly.GetEntryAssembly().Location);
        }
    }
}
