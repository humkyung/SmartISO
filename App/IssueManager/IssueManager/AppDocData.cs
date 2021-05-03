using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using framework.core;

namespace SmartISO
{
	public struct tagFtpServerSetting
	{
		public string sHost;
		public string sId;
		public string sPassword;
		public string sRemoteDirectory;
	};

	class AppDocData
	{
		public static FrameworkCore frameworkCore = null;

		public static tagFtpServerSetting FtpServerSetting;

        public static string GemBoxLicense = "EXK0-347T-N5GE-K8Y1"; /// 2013.02.16 added by humkyung
	}
}
