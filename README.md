This is a proof-of-concept of embedding Rhino functionality into a QT UI. The idea is to take advantage of Rhino’s relatively cheap price tag, bundle Rhino with Itasca software, and embed some of Rhino’s UI and functionality into it.

External run requirements:
Use master branch, others are all old at this point, deleting things just scares me
Rhino installed, settings folder in default location relative to %AppData%

On launch, this will set up the initial QT UI, then automatically launch and attach an instance of the Rhino UI to a QT widget. Nothing about this has to happen immediately on startup, it was just easier during debugging to not have to hit a “Launch Rhino” button every time.

Important build details:
  Scheme files for the desired configuration of rhino in the settings folder of the RhinoCaller3 project, saved with the scheme name “EmbedScheme”. To change this configuration, launch rhino using ‘<path to rhino>/Rhino.exe           /Scheme=EmbedScheme, change things around, close rhino, and copy all files with EmbedScheme in the name from C:\Users\<username>\AppData\Roaming\McNeel\Rhinoceros\8.0\settings to the settings folder of the RhinoCaller3 project.
  All projects building for 64 bit systems
  QTUI references RhinoLink, which references RhinoCaller3
