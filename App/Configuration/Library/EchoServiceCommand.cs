using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows.Forms;

using SmartISO.Service.Interface;
using SmartISO.Framework.Service;

namespace Configuration.Library
{
	class EchoServiceCommand : IDisposable
	{
		public string ServiceURL { get; set; }
        private static Dictionary<int, string> _ServiceURLList = new Dictionary<int, string>() 
		{ 
		    {0,"http://localhost:1613/"},
            {1,"http://192.168.35.194/SmartISO/"}	
		};

		public void Dispose()
		{
			GC.SuppressFinalize(this);
		}

		protected virtual void Dispose(bool disposing)
		{
		}

		public bool CanExecute(object parameter)
		{
			return true;
		}

		public event EventHandler CanExecuteChanged;
		public void Execute(object parameter)
		{
			try
			{
#if DEBUG
#else
				string sServiceURL = EDCS.WF.DSL.WcfHelper.CreateUrl(EDCS.WF.BSL.Variable.SharedVariable.IsIE, "WSHttp", "IDCS", false);
				EchoServiceCommand._ServiceURLList.Insert(0, sServiceURL);
#endif
                foreach (KeyValuePair<int, string> pair in EchoServiceCommand._ServiceURLList)
				{
					try
					{
                        string sEndPointAddress = pair.Value + "SmartISOService.svc";
						using (ISmartISOService svcProxy = ServiceFactory.CreateServiceChannel<ISmartISOService>(sEndPointAddress))
						{
							string sMessage = svcProxy.Echo(parameter as string);
							if ((parameter as string) == sMessage)
							{
								this.ServiceURL = sEndPointAddress;
								break;
							}
						}
					}
					catch(Exception ex)
					{
						string sError = ex.Message;
					}
				}
			}
			catch (Exception ex)
			{
				new TechSun.Theme.frmWarning(ex.Message).ShowDialog();
			}
		}
	}
}
