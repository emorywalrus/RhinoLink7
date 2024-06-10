using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;

using System.Threading;

using Rhino.Runtime.InProcess;
using Rhino.Display;
using Rhino;
using System.Drawing;



namespace RhinoCaller3
{
    unsafe
    public class Caller3
    {
        ~Caller3 ()
        {
            Rhino.ApplicationSettings.AppearanceSettings.UpdateFromState(unembedded_settings);
        }
        public RhinoCore core;
        public void* window_handle;
        public void* qtui_ptr;
        public Rhino.ApplicationSettings.AppearanceSettingsState unembedded_settings;

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
            core = new RhinoCore(null, WindowStyle.Hidden, (IntPtr)window_handle);

            RhinoApp.RunScript("-WindowLayout Default", false);

            Rhino.ApplicationSettings.AppearanceSettings.CommandPromptPosition
                = Rhino.ApplicationSettings.CommandPromptPosition.Floating;

            string viewportName = "Embedded Viewport";
            DefinedViewportProjection projection = DefinedViewportProjection.Perspective;
            Rectangle locationAndSize = new Rectangle(0, 0, 0, 0);
            bool floating = true;
            RhinoDoc.ActiveDoc.Views.Add(viewportName, projection, locationAndSize, floating);

            unembedded_settings = Rhino.ApplicationSettings.AppearanceSettings.GetCurrentState();

            Rhino.ApplicationSettings.AppearanceSettings.UpdateFromState(Rhino.ApplicationSettings.AppearanceSettings.GetDefaultState());

            Rhino.ApplicationSettings.AppearanceSettings.CommandPromptBackgroundColor = Color.White;
            Rhino.ApplicationSettings.AppearanceSettings.CommandPromptTextColor = Color.Black;
            Rhino.ApplicationSettings.AppearanceSettings.ViewportBackgroundColor = Color.White;
            Rhino.ApplicationSettings.AppearanceSettings.GridThickLineColor = Color.LightGray;
            Rhino.ApplicationSettings.AppearanceSettings.GridThinLineColor = Color.White;
            Rhino.ApplicationSettings.AppearanceSettings.GridXAxisLineColor = Color.LightGray;
            Rhino.ApplicationSettings.AppearanceSettings.GridYAxisLineColor = Color.LightGray;
            Rhino.ApplicationSettings.AppearanceSettings.GridZAxisLineColor = Color.Empty;
            Rhino.ApplicationSettings.AppearanceSettings.FrameBackgroundColor = Color.White;

            RhinoApp.RunScript("-SaveWindowLayout embedded", false);
            RhinoApp.RunScript("-WindowLayout embedded", false);

            lock_rhino_time(qtui_ptr, (void*)Rhino.RhinoApp.MainWindowHandle());

            core.Run();
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
        public static void run_script(string script)
        {
            Action command = () =>
            {
                RhinoApp.RunScript(script, false);
            };
            RhinoApp.InvokeOnUiThread(command);
        }

        public static void exit_rhino(void* caller_ptr)
        {
            RhinoApp.Exit();
        }
    }
}
