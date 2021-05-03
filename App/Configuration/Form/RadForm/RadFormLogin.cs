using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using Telerik.WinControls;
using Telerik.WinControls.UI;
using TechSun.Theme;
using Telerik.WinControls.UI.Docking;

using LicWS.Framework.Service;
using LicWS.Service.Interface;
using System.Configuration;

namespace Configuration.Form.RadForm
{
    public partial class RadFormLogin : Telerik.WinControls.UI.RadForm
    {
        public string ID { get; set; }
        public string Password { get; set; }
        public string ActivateCode { get; set; }

        public RadFormLogin()
        {
            InitializeComponent();
            RadFormLogin.ApplyTheme(this.Controls);

            Program.OnIdleUpdate += new Program.IdleUpdateEvent(OnIdleUpdate);
        }

        private void OnIdleUpdate()
        {
            this.radButtonOK.Enabled = !string.IsNullOrEmpty(this.radTextBoxID.Text) && !string.IsNullOrEmpty(this.radTextBoxPassword.Text);
            this.radButtonRefresh.Enabled = this.radButtonOK.Enabled;
        }

        private void radButtonOK_Click(object sender, EventArgs e)
        {
            try
            {
                using (Library.LoginCommand cmd = new Library.LoginCommand())
                {
                    DataSet dsParam = new DataSet();
                    {
                        DataTable dtParam = new DataTable("Params");
                        {
                            dtParam.Columns.Add(new DataColumn("AppName", typeof(string)));
                            dtParam.Columns.Add(new DataColumn("ID", typeof(string)));
                            dtParam.Columns.Add(new DataColumn("Password", typeof(string)));
                        }
                        dsParam.Tables.Add(dtParam);

                        DataRow dr = dsParam.Tables[0].NewRow();
                        dr["AppName"] = "SmartISO";
                        dr["ID"] = this.radTextBoxID.Text;
                        dr["Password"] = this.radTextBoxPassword.Text;
                        dsParam.Tables[0].Rows.Add(dr);
                    }
                    cmd.Execute(dsParam);

                    if ((0 == cmd.Code) || (10 == cmd.Code))
                    {
                        this.ID = this.radTextBoxID.Text;
                        this.Password = this.radTextBoxPassword.Text;
                        this.ActivateCode = cmd.ActivateCode;

                        DialogResult = System.Windows.Forms.DialogResult.OK;
                    }
                    else
                    {
                        this.radLabelMessage.Text = cmd.Message;
                    }
                }
            }
            catch (Exception ex)
            {
                new TechSun.Theme.frmWarning(ex.Message).ShowDialog(this);
            }
        }

        private void radButtonCancel_Click(object sender, EventArgs e)
        {
            DialogResult = System.Windows.Forms.DialogResult.Cancel;
        }

        public static void ApplyTheme(Control.ControlCollection oControlColl)
        {
            foreach (Control oCtrl in oControlColl)
            {
                if (oCtrl is RadGroupBox)
                {
                    RadGroupBox oRadGroupBox = oCtrl as RadGroupBox;
                    oRadGroupBox.ThemeName = KRadTheme.Office2010Silver;
                    ApplyTheme(oRadGroupBox.Controls);
                }
                else if (oCtrl is DocumentTabStrip)
                {
                    DocumentTabStrip oDocumentTabStrip = oCtrl as DocumentTabStrip;
                    oDocumentTabStrip.ThemeName = KRadTheme.Office2010Silver;
                }
                else if (oCtrl is FlowLayoutPanel)
                {
                    FlowLayoutPanel oFlowLayoutPanel = oCtrl as FlowLayoutPanel;
                    ApplyTheme(oFlowLayoutPanel.Controls);
                }
                else if (oCtrl is TabControl)
                {
                    TabControl oTabControl = oCtrl as TabControl;
                    ApplyTheme(oTabControl.Controls);
                }
                else if (oCtrl is TabPage)
                {
                    TabPage oTabPage = oCtrl as TabPage;
                    ApplyTheme(oTabPage.Controls);
                }
                else if (oCtrl is RadPageView)
                {
                    RadPageView oRadPageView = oCtrl as RadPageView;
                    oRadPageView.ThemeName = KRadTheme.Office2010Silver;
                    ApplyTheme(oRadPageView.Controls);
                }
                else if (oCtrl is RadPageViewPage)
                {
                    RadPageViewPage oRadPageViewPage = oCtrl as RadPageViewPage;
                    ApplyTheme(oRadPageViewPage.Controls);
                }
                else if (oCtrl is SplitPanel)
                {
                    SplitPanel oSplitPanel = oCtrl as SplitPanel;
                    oSplitPanel.ThemeName = KRadTheme.Office2010Silver;
                    ApplyTheme(oSplitPanel.Controls);
                }
                else if (oCtrl is RadSplitContainer)
                {
                    RadSplitContainer oSplitContainer = oCtrl as RadSplitContainer;
                    oSplitContainer.ThemeName = KRadTheme.Office2010Silver;
                    ApplyTheme(oSplitContainer.Controls);
                }
                else if (oCtrl is RadTreeView)
                {
                    RadTreeView oTreeView = oCtrl as RadTreeView;
                    oTreeView.ThemeName = KRadTheme.Windows8;
                }
                else if (oCtrl is RadGridView)
                {
                    RadGridView oGridView = oCtrl as RadGridView;
                    oGridView.ThemeName = KRadTheme.Office2010Silver;

                    if (oCtrl.Parent is ToolWindow)
                    {
                        ToolWindow toolwindow = oCtrl.Parent as ToolWindow;
                        if ("Connection" == toolwindow.Text.Trim() || "Status" == toolwindow.Text.Trim())
                        {
                            oGridView.ThemeName = KRadTheme.Windows8;
                        }
                    }
                    else if (oCtrl.Parent is UserControl) // EDB Status grid를 위한
                    {
                        if ("ucWorkingStatus" == oCtrl.Parent.Name)
                        {
                            oGridView.ThemeName = KRadTheme.Windows8;
                        }
                    }
                }
                else if (oCtrl is RadListView)
                {
                    RadListView oListView = oCtrl as RadListView;
                    oListView.ThemeName = KRadTheme.Office2010Silver;
                }
                else if (oCtrl is RadDropDownList)
                {
                    RadDropDownList oRadDropDownList = oCtrl as RadDropDownList;
                    oRadDropDownList.ThemeName = KRadTheme.Windows8;
                    oRadDropDownList.ListElement.Font = new System.Drawing.Font("Tahoma", 8.25f);
                    oRadDropDownList.ListElement.ForeColor = System.Drawing.Color.Black;
                    oRadDropDownList.ForeColor = System.Drawing.Color.Black;
                }
                else if (oCtrl is Telerik.WinControls.UI.Docking.RadDock)
                {
                    Telerik.WinControls.UI.Docking.RadDock oRadCtrl = oCtrl as Telerik.WinControls.UI.Docking.RadDock;
                    oRadCtrl.ThemeName = KRadTheme.Office2010Silver;
                }
                else if (oCtrl is RadControl)
                {
                    RadControl oRadCtrl = oCtrl as RadControl;
                    oRadCtrl.ThemeName = KRadTheme.Windows8;

                    if (!(oCtrl is RadLabel))
                    {
                        oRadCtrl.Font = new System.Drawing.Font("Tahoma", 8.25f);
                    }
                }
                else if (oCtrl is UserControl)
                {
                    ApplyTheme((oCtrl as UserControl).Controls);
                }
            }
        }

        /// <summary>
        /// clear activate code
        /// </summary>
        /// <author>humkyung</author>
        /// <date>2017.05.07</date>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void radButtonRefresh_Click(object sender, EventArgs e)
        {
            try
            {
                string sLicWSURL = ConfigurationManager.AppSettings["LicWS"];
                using (ILicWSService svcProxy = ServiceFactory.CreateServiceChannel<ILicWSService>(sLicWSURL, "wsHttp"))
                {
                    Library.EncryptDecryptQueryString crypt = new Library.EncryptDecryptQueryString();

                    using (DataSet dsParam = svcProxy.GetCommandParam("LicWS.BSL", "ClearActivateCodeCommand"))
                    {
                        DataRow dr = dsParam.Tables[0].NewRow();
                        {
                            dr["ID"] = crypt.Encrypt(this.radTextBoxID.Text);
                            dr["Password"] = crypt.Encrypt(this.radTextBoxPassword.Text);
                            dr["AppName"] = crypt.Encrypt("SmartISO");

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

                        svcProxy.ExecuteCommand("LicWS.BSL", "ClearActivateCodeCommand", dsParam);
                    }
                }
            }
            catch (Exception ex)
            {
                new TechSun.Theme.frmWarning(ex.Message).ShowDialog(this);
            }
        }

        /// <summary>
        /// call radButtonOK_Click when user input enter key
        /// </summary>
        /// <author>humkyung</author>
        /// <date>2017.05.09</date>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void radTextBoxPassword_KeyDown(object sender, KeyEventArgs e)
        {
            if (Keys.Enter == e.KeyCode)
            {
                radButtonOK_Click(null, null);
            }
        }
    }
}
