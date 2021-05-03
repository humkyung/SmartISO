using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.IO;
using System.Diagnostics;
using System.Threading;
using System.Net.Sockets;
using System.ServiceModel;
using System.Configuration;
using System.Text.RegularExpressions;
using Microsoft.Win32;

namespace Configuration
{
    public class AutoCADLauncher
    {
		public static string AutoCADPluginsPath
		{
			get
			{
				return Path.Combine(Environment.GetFolderPath(Environment.SpecialFolder.CommonApplicationData), "Autodesk", "ApplicationPlugins");
			}
		}

        public static bool? LaunchIfPlugInInAppServiceNotAvailable(IWin32Window owner)
        {
            try
            {
                Configuration.PlugInServiceEchoCommand cmd = new Configuration.PlugInServiceEchoCommand();
                if(true == cmd.Execute())
                {
                    return true;
                }
            }
            catch
            {
                return _launch(owner, 30);
            }

            return false;
        }

        private static bool _launch(IWin32Window owner, int nMaxRetryCount)
        {
            bool res = false;

            //var frmPopupWaitingBarCallMethod = new TechSun.Theme.frmSplashScreen(nMaxRetryCount, EDCS.WF.BSL.Variable.SharedVariable.ToolTip); // 생성 및 단계 수 설정 
            {
                try
                {
                    #region
                    //frmPopupWaitingBarCallMethod.MethodSetStart();
                    //frmPopupWaitingBarCallMethod.MethodStatusUpdateText("Loading");

                    Application.DoEvents();

					string sAutoCADFileVersion = AutoCADLauncher._getAutoCADFileVersion();
					#region strCLRVersionSuffix
					if (Regex.IsMatch(sAutoCADFileVersion, @"^R(18)\."))
					{
						sAutoCADFileVersion = "18";
					}
					else if (Regex.IsMatch(sAutoCADFileVersion, @"^R(19)\."))
					{
						sAutoCADFileVersion = "19";
					}
					else if (Regex.IsMatch(sAutoCADFileVersion, @"^R(20)\."))
					{
						sAutoCADFileVersion = "19";
					}
					else
					{
						new TechSun.Theme.frmInfomation("Unable to run AutoCAD.\nYou must upgrade the program.").ShowDialog();
						throw new System.Exception("Unable to run AutoCAD.\nYou must upgrade the program.");
					}
					#endregion

					string sStartUpFilePath = Path.Combine(AutoCADLauncher.AutoCADPluginsPath,"SmartISO" , sAutoCADFileVersion , "StartUp.scr");
					if(File.Exists(sStartUpFilePath)) File.Delete(sStartUpFilePath);
					using (StreamWriter sw = File.CreateText(sStartUpFilePath))
                    {
                        sw.WriteLine(string.Format("(command \"netload\" \"{0}\")", Path.Combine(AutoCADLauncher.AutoCADPluginsPath, "SmartISO",sAutoCADFileVersion,"SmartISO.NET.dll").Replace(@"\", @"\\")));
                    }

                    Process procAutoCAD = new Process();
                    procAutoCAD.StartInfo.FileName = AppDocData.AutoCADFilePath;
					procAutoCAD.StartInfo.Arguments = string.Format("/b \"{0}\"", sStartUpFilePath);
                    procAutoCAD.Start();

                    _delay(1000);

                    int nRetryCount = 0;
                    while (nRetryCount < nMaxRetryCount)
                    {
                        #region
                        _delay(1000);
                        ++nRetryCount;

                        //frmPopupWaitingBarCallMethod.MethodStatusUpdateText("Loading");
                        Application.DoEvents();

                        try
                        {
                            Configuration.PlugInServiceEchoCommand cmd = new Configuration.PlugInServiceEchoCommand();
                            if (true == cmd.Execute())
                            {
                                res = true;
                                break;
                            }
                        }
                        catch
                        {
                            continue;
                        }
                        #endregion
                    }
                    #endregion
                }
                catch(Exception ex)
                {
                    new TechSun.Theme.frmWarning(ex.Message).ShowDialog(Program.mf);
                }
                finally
                {
                    //frmPopupWaitingBarCallMethod.MethodClose();
                }
            }

            return res;
        }

        private static string _getAutoCADFileVersion()
        {
			FileVersionInfo fviAutoCAD = FileVersionInfo.GetVersionInfo(AppDocData.AutoCADFilePath);
            return fviAutoCAD.FileVersion;
        }

        private static DateTime _delay(int milliSecond)
        {
            DateTime tmNow = DateTime.Now;
            DateTime tmLimits = tmNow.AddMilliseconds(milliSecond);

            while (tmNow <= tmLimits)
            {
                Thread.Sleep(10);
                System.Windows.Forms.Application.DoEvents();
                tmNow = DateTime.Now;
            }

            return DateTime.Now;
        }
    }
}
