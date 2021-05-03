using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace Configuration
{
	public class AppDocData
	{
        public class UserInfo
        {
            public string ID { get; set; }
            public string Password { get; set; }
            public string ActivateCode { get; set; }
            public string Role { get; set; }
        };

        public static UserInfo CurrentUser { get; set; }

		/// get ServiceURL
		private static string _ServiceURL = string.Empty;
        public static string ServiceURL
        {
            get
            {
                if (string.IsNullOrEmpty(AppDocData._ServiceURL))
                {
                    //SmartISO.Framework.Service.UserInfo.IsInternal = EDCS.WF.BSL.Variable.SharedVariable.IsIE;
                    using (Library.EchoServiceCommand cmd = new Library.EchoServiceCommand())
                    {
                        cmd.Execute("Echo");
                        AppDocData._ServiceURL = cmd.ServiceURL;
                    }
                }

                return AppDocData._ServiceURL;
            }
        }
		/// up to here

        public static string AutoCADFilePath { get; set; }
	}
}
