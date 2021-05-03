using System;
using System.Collections.Generic;
using System.Linq;
using System.Windows.Forms;

namespace SmartISO
{
    static class Program
    {
        /// <summary>
        /// The main entry point for the application.
        /// </summary>
        [STAThread]
        static void Main(string []args)
        {
            if (2 == args.Length)
            {
                Application.EnableVisualStyles();
                Application.SetCompatibleTextRenderingDefault(false);

                Application.Run(new MDIParentIssueManager(args[0], args[1]));
                ///Application.Run(new IssueManager(args[0] , args[1]));
            }
        }
    }
}
