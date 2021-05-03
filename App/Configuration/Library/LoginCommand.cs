using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Runtime.InteropServices;
using System.Data;

using LicWS.Framework.Service;
using LicWS.Service.Interface;
using System.Configuration;

namespace Configuration.Library
{
    public class LoginCommand : IDisposable
    {
        private const string WebServiceURL = "http://www.solutionware.co.kr/LicWebService/LicWebService.asmx?WSDL";
        private const string InstanceName = "Login";

        public byte Code = 0;
        public string ID { get; set; }
        public string Password { get; set; }
        public string ActivateCode { get; set; }
        public string Message
        {
            get
            {
                string res = string.Empty;

                switch(this.Code)
                {
                    case 0:
                        res = "Success";
                        break;
                    case 1:
                        res = "Wrong Password";
                        break;
                    case 2:
                        res = "No User";
                        break;
                    case 3:
                        res = "Already Logged in";
                        break;
                    case 4:
                        res = "Can't access database";
                        break;
                    case 5:
                        res = "Invalid activate code";
                        break;
                    case 6:
                        res = "Invalid request";
                        break;  
                    case 7:
                        res = "Corrupted packet";
                        break;
                    case 8:
                        res = "Fail to reset activate code";
                        break;
                    case 9:
                        res = "Disconnected";
                        break;
                    case 10:
                        res = "New version available";
                        break;
                    case 11:
                        res = "License is expired";
                        break;
                }

                return res;
            }
        }

        [StructLayout(LayoutKind.Sequential)]
        public struct Packet
        {
            public Byte Code;
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = 32)]
            public Byte[] ID;
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = 32)]
            public Byte[] Password;
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = 32)]
            public Byte[] AppName;
            public Byte Major;
            public Byte Minor;
            public Byte Maintenance;
            public Byte Build;
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = 48)]
            public Byte[] ActivateCode;
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = 256)]
            public Byte[] URL;
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = 1024)]
            public Byte[] Message;
        };

        [StructLayout(LayoutKind.Sequential)]
        public struct LMSStuctType
        {
            public int size;
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = 4096)]
            public Byte[] buf;
        };

        [DllImport(".\\SmartLMSLib_vc100.dll", CallingConvention = CallingConvention.StdCall, CharSet = CharSet.Ansi, EntryPoint = "_InitializePacket@4")]
        static extern int InitializePacket(ref Packet param);
        [DllImport(".\\SmartLMSLib_vc100.dll", CallingConvention = CallingConvention.StdCall, CharSet = CharSet.Ansi, EntryPoint = "_Encode@4")]
        static extern int Encode(ref LMSStuctType param);
        [DllImport(".\\SmartLMSLib_vc100.dll", CallingConvention = CallingConvention.StdCall, CharSet = CharSet.Ansi, EntryPoint = "_Decode@4")]
        static extern int Decode(ref LMSStuctType param);

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
                if (!(parameter is DataSet) || (0 == (parameter as DataSet).Tables.Count) || (0 == (parameter as DataSet).Tables[0].Rows.Count)) return;
                string sAppName = (parameter as DataSet).Tables[0].Rows[0]["AppName"].ToString();
                string sID = (parameter as DataSet).Tables[0].Rows[0]["ID"].ToString();
                string sPassword = (parameter as DataSet).Tables[0].Rows[0]["Password"].ToString();

                string sLicWSURL = ConfigurationManager.AppSettings["LicWS"];
                using (ILicWSService svcProxy = ServiceFactory.CreateServiceChannel<ILicWSService>(sLicWSURL, "wsHttp"))
                {
                    EncryptDecryptQueryString crypt = new EncryptDecryptQueryString();

                    using (DataSet dsParam = svcProxy.GetCommandParam("LicWS.BSL", "LoginCommand"))
                    {
                        DataRow dr = dsParam.Tables[0].NewRow();
                        {
                            dr["ID"] = crypt.Encrypt(sID);
                            dr["Password"] = crypt.Encrypt(sPassword);
                            dr["AppName"] = crypt.Encrypt(sAppName);
                            
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

                        using (DataSet ds = svcProxy.ExecuteCommand("LicWS.BSL", "LoginCommand", dsParam))
                        {
                            this.Code = Convert.ToByte(ds.Tables[0].Rows[0]["Code"]);
                            this.ID = crypt.Decrypt(ds.Tables[0].Rows[0]["ID"].ToString());
                            this.Password = crypt.Decrypt(ds.Tables[0].Rows[0]["Password"].ToString());
                            this.ActivateCode = crypt.Decrypt(ds.Tables[0].Rows[0]["ActivateCode"].ToString());
                        }
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
