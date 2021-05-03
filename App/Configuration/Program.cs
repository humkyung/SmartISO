using System;
using System.Collections.Generic;
using System.Runtime.InteropServices;
using System.Windows.Forms;
using System.Data;

using LicWS.Service.Interface;
using LicWS.Framework.Service;
using System.Configuration;
using System.Net.Sockets;
using System.Net;
using System.Threading;
using System.Text;
using System.IO;

namespace Configuration
{
    static class Program
    {
        public delegate void IdleUpdateEvent();	        /// declare delegage
        public static IdleUpdateEvent OnIdleUpdate = null;
        public static FrmMain mf = null;

        private static Control _ctrl = null;
        private static Library.TcpServer _TcpServer = null;
        delegate void RequestDispatcher(DataSet dsParam);

        private static void SetupTcp(int iPort)
        {
            _ctrl = new Control();
            _ctrl.CreateControl();

            _TcpServer = new Library.TcpServer(iPort);
            _TcpServer.RequestProcess += new Library.TcpServer.RequestProcessEvent(DoAcceptTcpClientCallback);
            _TcpServer.Start();
        }

        private static void DoAcceptTcpClientCallback(TcpClient client)
        {
            // Get a stream object for reading and writing
            using (NetworkStream stream = client.GetStream())
            {
                string sParam = string.Empty;
                byte[] buffer = new byte[4];
                stream.Read(buffer, 0, 4);
                Int32 iDataSize = BitConverter.ToInt32(buffer, 0);
                {
                    buffer = new byte[iDataSize];
                    stream.Read(buffer, 0, iDataSize);
                    sParam = Encoding.UTF8.GetString(buffer);
                    while (sParam.Length < buffer.Length)
                    {
                        byte[] temp = new byte[1024];
                        stream.Read(temp, 0, 1024);
                        sParam += Encoding.UTF8.GetString(temp);
                    }
                }
                DataSet dsParam = new DataSet();
                dsParam.ReadXml(new StringReader(sParam));

                _ctrl.Invoke(new RequestDispatcher(DispatchRequest), new object[] { dsParam });
            }
        }

        /// <summary>
        /// get command from manager app
        /// </summary>
        /// <author>humkyung</author>
        /// <date>2016.11.09</date>
        private static void DispatchRequest(DataSet dsParam)
        {
            try
            {
                //mf.UpdateControl(dsParam);
            }
            catch (Exception ex)
            {
                new TechSun.Theme.frmWarning(ex.Message).ShowDialog(mf);
            }
            finally
            { }
        }

        /// <summary>
        /// The main entry point for the application.
        /// </summary>
        [STAThread]
        static void Main()
        {
            Application.ApplicationExit += new EventHandler(Application_ApplicationExit);
            Application.EnableVisualStyles();
            Application.SetCompatibleTextRenderingDefault(false);

            try
            {
                Program.SetupTcp(9328);

                Form.RadForm.RadFormLogin frmLogin = new Form.RadForm.RadFormLogin();
                if (DialogResult.OK == frmLogin.ShowDialog())
                {
                    var frmPopupWaitingBarCallMethod = new TechSun.Theme.frmSplashScreen(1, new List<string>() { "Loading..." }); // 생성 및 단계 수 설정
                    frmPopupWaitingBarCallMethod.MethodSetStart(); // 시작
                    frmPopupWaitingBarCallMethod.MethodStatusUpdateText("Loading...  Please Wait."); // 단계 업데이트

                    mf = new FrmMain(new AppDocData.UserInfo() { ID = frmLogin.ID, Password = frmLogin.Password, ActivateCode = frmLogin.ActivateCode, Role = "SA" }, frmPopupWaitingBarCallMethod);
                    Application.Idle += OnApplicationIdle;
                    Application.Run(mf);
                }

                _TcpServer.Stop();
            }
            finally
            {
                  #region Logout
                string sLicWSURL = ConfigurationManager.AppSettings["LicWS"];
                using (ILicWSService svcProxy = LicWS.Framework.Service.ServiceFactory.CreateServiceChannel<ILicWSService>(sLicWSURL, "wsHttp"))
                {
                    Library.EncryptDecryptQueryString crypt = new Library.EncryptDecryptQueryString();
                    using (DataSet dsParam = svcProxy.GetCommandParam("LicWS.BSL", "LogoutCommand"))
                    {
                        DataRow dr = dsParam.Tables[0].NewRow();
                        {
                            dr["ID"] = crypt.Encrypt(AppDocData.CurrentUser.ID);
                            dr["Password"] = crypt.Encrypt(AppDocData.CurrentUser.Password);
                            dr["AppName"] = crypt.Encrypt("SmartISO");
                            dr["ActivateCode"] = crypt.Encrypt(AppDocData.CurrentUser.ActivateCode);

                            string[] oTokens = System.Windows.Forms.Application.ProductVersion.Split('.');
                            if (4 == oTokens.Length)
                            {
                                dr["Major"] = oTokens[0];
                                dr["Minor"] = oTokens[1];
                                dr["Maintenance"] = oTokens[2];
                                dr["Build"] = oTokens[3];
                            }
                        }
                        dsParam.Tables[0].Rows.Add(dr);

                        svcProxy.ExecuteCommand("LicWS.BSL", "LogoutCommand", dsParam);
                    }
                }
                  #endregion
            }
        }

        static EventHandler OnApplicationIdle = new EventHandler(Application_Idle);
        static void Application_Idle(object sender, EventArgs e)
        {
            if (null != OnIdleUpdate) OnIdleUpdate();
        }
        static void Application_ApplicationExit(object sender, EventArgs e)
        {
            Application.Idle -= OnApplicationIdle;
        }
    }
}