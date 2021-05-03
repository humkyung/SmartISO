using System;
using System.Collections.Generic;
using System.Windows.Forms;
using IsoCreator.Forms;

namespace IsoCreator
{
    /// <summary>
    /// This is a project which transforms folders into iso-s and even better, can create an iso having nothing but 
    /// the directory structure. (Uncomment the "Application.Run( new VirtualIsoCreator() );" part, and comment the 
    /// "Application.Run( new MainForm() );" to check it out. 
    /// Of course, this is only a demo, but as you may see if you check it out, you can do a lot more with it. I haven't
    /// really had too much time to build a more comprehensive example. I hope this will do for now.
    /// These libraries come with no guaranties, feel free to use anything you find here. However, if you do like it,
    /// please send feedback and tell others ;)
    /// For more information about the iso9660, check out the main document i consulted for this library:
    /// http://alumnus.caltech.edu/~pje/iso9660.html (i included a copy in the project directory)
    /// For any information concerning anything here, please contact me at:
    /// florin_chelaru@yahoo.com
    /// </summary>
    static class Program
    {
        /// <summary>
        /// The main entry point for the application.
        /// </summary>
        [STAThread]
        static void Main(string[] args)
        {
            Application.EnableVisualStyles();
            Application.SetCompatibleTextRenderingDefault(false);
            if (3 == args.Length)
            {
                Application.Run(new MainForm(args[0] , args[1] , args[2]));
            }
            else
            {
                Application.Run(new MainForm());
            }
        }
    }
}