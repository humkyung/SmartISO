using System;
using System.Collections.Generic;
using System.Drawing;
using System.Linq;
using System.Security.Principal;
using System.Windows.Forms;
using Configuration.Properties;
using Telerik.WinControls.UI;

using TechSun.Theme;
using System.Diagnostics;
using System.Runtime.InteropServices;

using SmartISO.Service.Interface;
using SmartISO.Framework.Service;

using System.Data;

namespace Configuration
{
    public partial class FrmMain : KRadForm
    {
        public Font FONT_REGULAR = new Font("Tahoma", (float)8.25, FontStyle.Regular);
        public Font FONT_BOLD = new Font("Tahoma", (float)8.25, FontStyle.Bold);

        public FrmMain(AppDocData.UserInfo oUserInfo, TechSun.Theme.frmSplashScreen frmPopupWaitingBarCallMethod)
        {
            AppDocData.CurrentUser = oUserInfo;

            InitializeComponent();
            this.ShowIcon = true;
            this.Text = this.Text.Trim();

            try
            {
                InitAdminMenu();
                SetMenuControl("UserControlNoData");

                Program.OnIdleUpdate += new Program.IdleUpdateEvent(OnIdleUpdate);
            }
            catch (Exception ex)
            {
                new TechSun.Theme.frmWarning(ex.StackTrace).ShowDialog();
            }
            finally
            {
                frmPopupWaitingBarCallMethod.MethodClose();
            }

            this.TopMost = true;
        }

        private void OnIdleUpdate()
        {
            this.radMenuItemSA.Enabled = "SA" == AppDocData.CurrentUser.Role;
        }

        private void InitAdminMenu()
        {
            try
            {
                radTreeViewMenu.Nodes.Add("Configuration");
                radTreeViewMenu.Nodes[0].Image = Resources.SmallSetting;
                InitCommonMenu();
                InitSmartISOMenu();	/// 2017.05.09 added by humkyung

                radTreeViewMenu.Nodes[0].ExpandAll();
            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.ToString());
            }
        }

        private void InitCommonMenu()
        {
            RadTreeNode nod = radTreeViewMenu.Nodes[0].Nodes.Add("Common");
            nod.Image = Resources.SmallCommon;
            nod.Nodes.Add("UserControlCommonProject", "Project", 0);
            //nod.Nodes[0].Font = new Font("Tahoma", (float)8.25, FontStyle.Bold);
            //nod.Nodes[0].Selected = true;

            //RadTreeNode nodSub = nod.Nodes.Add("Role");

            //nodSub.Nodes.Add("UserControlCommonRole", "Role", 0);
            //// 161114 UserControlCommonRole 메뉴로 합침
            ////nodSub.Nodes.Add("UserControlCommonStandardRole", "Role by Department", 0);
            //nodSub.Nodes.Add("UserControlCommonRolebyMenu", "Role by Menu", 0);
            //nodSub.Nodes.Add("UserControlCommonEmployee", "User", 0);
            //nodSub.Nodes.Add("UserControlCommonEmplyeeByProject", "Project by User", 0);
            //nodSub = nod.Nodes.Add("Mapping");
            //nodSub.Nodes.Add("UserControlCommonPBSSetting", "PBS Setting", 0);
            //nodSub.Nodes.Add("UserControlCommonPBSMappingSetting", "PBS", 0);
            //nodSub.Nodes.Add("UserControlCommonAttributeMapping", "Attribute", 0);
            //nodSub.Nodes.Add("UserControlDocNoMapping", "DocNo", 0);
            //nodSub = nod.Nodes.Add("Rule");
            //nodSub.Nodes.Add("UserControlCommonTagRuleSetting", "Tag - Consistency", 0);
            //nodSub.Nodes.Add(new RadTreeNode() { Name = "UserControlCommonTagRuleSetting", Text = "Tag - Consistency", Tag = new EntityTagNode() { EnumFormName = EnumFormName.CaseDynamicDefault, FillFlag = false } });
            //nodSub.Nodes.Add("UserControlSP3DLineNoDisplayFormat", "Tag - Item Compare", 0);
            //nodSub.Nodes.Add("UserControlSPPIDInstrumentCompareItemSetting", "SPPID-SPI Compare Setting", 0);
            //nodSub.Nodes.Add("UserControlCommonValidation", "Validation", 0);
            //nod.Nodes.Add("UserControlCommonConnectLog", "Connect Log", 0);

            nod.ExpandAll();
        }
        
        /// <summary>
        /// init SmartISO menu
        /// </summary>
        /// <author>humkyung</author>
        /// <date>2017.05.09</date>
        private void InitSmartISOMenu()
        {
            RadTreeNode oSmartISOTreeNode = radTreeViewMenu.Nodes[0].Nodes.Add("SmartISO");
            oSmartISOTreeNode.Image = Resources.SmallPlant;

            {
                RadTreeNode oLinelistSettingNode = oSmartISOTreeNode.Nodes.Add(UserControlSmartISOLinelistSetting.DisplayName);
                RadTreeNode oSiteSettingNode = oSmartISOTreeNode.Nodes.Add(UserControlSmartISOSiteSetting.DisplayName);
                RadTreeNode oPrjSettingNode = oSmartISOTreeNode.Nodes.Add(UserControlPrjSetting.DisplayName);
                RadTreeNode oBorderSettingNode = oSmartISOTreeNode.Nodes.Add(UserControlBorderSetting.DisplayName);
                RadTreeNode oRevSettingNode = oSmartISOTreeNode.Nodes.Add(UserControlRevSetting.DisplayName);
                RadTreeNode oIsoProductSettingNode = oSmartISOTreeNode.Nodes.Add(UserControlIsoProductSetting.DisplayName);
            }

            oSmartISOTreeNode.ExpandAll();
        }

        private void radTreeViewMenu_SelectedNodeChanged(object sender, RadTreeViewEventArgs e)
        {
            RadTreeNode node = this.radTreeViewMenu.SelectedNode;
            SetMenuNode(node);
        }

        private void SetMenuNode(RadTreeNode node)
        {
            UserControlBase oCtrl = ControlFactory.Create(node.Name.ToString());

            if (null != oCtrl)
            {
                if (radTreeViewMenu.Nodes.Count > 0) ResetMenuList(radTreeViewMenu.Nodes[0]);

                node.Font = FONT_BOLD;

                HideAllUserControl();
                var control = oCtrl.AsUserControl();

            //    if (node.Name.ToString() == "UserControlCommonTagRuleSetting")
            //    {
            //        control.Anchor = (AnchorStyles.Left | AnchorStyles.Top | AnchorStyles.Bottom);
            //    }

                control.Dock = DockStyle.Fill;
                radPanelUserControl.Controls.Add(control);
            //    var nodeTag = (EntityTagNode)node.Tag;
            //    if (nodeTag == null)
            //    {
            //        control.Dock = DockStyle.Fill;
            //    }
            //    else if (nodeTag.FillFlag == true)
            //    {
            //        control.Dock = DockStyle.Fill;
            //    }
            //    else
            //    {
            //        control.Dock = DockStyle.None;
            //    }

            //    mucbaseControl.InitializeForm(SharedVariable.IsIE, nodeTag, this._entityEdbInformation);
            //    mucbaseControl.InitializeControl();
                oCtrl.LoadData();
                node.Tag = oCtrl;
            }
            else
            {
                new TechSun.Theme.frmInfomation(string.Format("{0} Meun는 준비중입니다.", node.Value.ToString())).ShowDialog(this);
            }
        }

        private void SetMenuControl(string ControlName)
        {
            //IControl IControl = ControlFactory.Create(node.Name.ToString());
            //mucbaseControl = ControlFactory.Create(ControlName);

            //if (mucbaseControl != null)
            //{
            //    if (radTreeViewMenu.Nodes.Count > 0)
            //        ResetMenuList(radTreeViewMenu.Nodes[0]);

            //    HideAllUserControl();
            //    var control = mucbaseControl.AsUserControl();
            //    control.Dock = DockStyle.Fill;
            //    radPanelUserControl.Controls.Add(control);

            //    mucbaseControl.InitializeForm(SharedVariable.IsIE, null, this._entityEdbInformation);
            //    mucbaseControl.LoadData();
            //}
            //else
            //{
            //    //MessageBox.Show(string.Format("{0} Meun는 준비중입니다.", node.Value.ToString()));
            //}
        }

        private void HideAllUserControl()
        {
            if (this.radPanelUserControl.Controls.Count > 0)
            {
                for (int i = this.radPanelUserControl.Controls.Count - 1; i >= 0; i--)
                {
                    this.radPanelUserControl.Controls[i].Dispose();
                }
            }

            radPanelUserControl.Controls.Clear();
        }

        private void ResetMenuList(RadTreeNode tn)
        {
            tn.Font = FONT_REGULAR;
            foreach (RadTreeNode child in tn.Nodes)
            {
                child.Font = FONT_REGULAR;
                ResetMenuList(child);
            }
        }

        private void FrmMain_FormClosing(object sender, FormClosingEventArgs e)
        {

        }

        private void FrmMain_Load(object sender, EventArgs e)
        {
            this.TopMost = false;
            this.BringToFront();
        }

        private void FrmMain_Shown(object sender, EventArgs e)
        {
            this.TopMost = false;
            this.BringToFront();
        }

        private void commandBarButtonSaveData_Click(object sender, EventArgs e)
        {
            try
            {
                this.Cursor = Cursors.WaitCursor;
                if (null != this.radTreeViewMenu.SelectedNode.Tag) (this.radTreeViewMenu.SelectedNode.Tag as UserControlBase).SaveData();
            }
            finally
            {
                this.Cursor = Cursors.Default;
            }
        }


        private void commandBarButtonExportExcel_Click(object sender, EventArgs e)
        {
            try
            {
                this.Cursor = Cursors.WaitCursor;
                //mucbaseControl.ExportToExcel();
            }
            finally
            {
                this.Cursor = Cursors.Default;
            }
        }

        /// <summary>
        /// exit application
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void radMenuItemExit_Click(object sender, EventArgs e)
        {
            Application.Exit();
        }

        /// <summary>
        /// change system administrator
        /// </summary>
        /// <author>humkyung</author>
        /// <date>2017.05.05</date>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void radMenuItemSA_Click(object sender, EventArgs e)
        {
            using (ISmartISOService svcProxy = SmartISO.Framework.Service.ServiceFactory.CreateServiceChannel<ISmartISOService>(AppDocData.ServiceURL, "wsHttp"))
            {
                using (DataSet ds = svcProxy.ExecuteCommand("SmartISO.BSL", "GetUserListCommand", null))
                {
                    if (ds.Tables[0].Rows.Count > 0)
                    {
                        using (Form.RadForm.RadFormSA frm = new Form.RadForm.RadFormSA() { dsUser = ds })
                        {
                            if (DialogResult.OK == frm.ShowDialog(this))
                            {
                                using (DataSet _ds = svcProxy.ExecuteCommand("SmartISO.BSL", "GetUserListCommand", null))
                                {
                                    DataRow[] dr = _ds.Tables[0].Select(string.Format("ID='{0}'", AppDocData.CurrentUser.ID.Replace("'", "''")));
                                    if ((null != dr) && (1 == dr.Length))
                                    {
                                        AppDocData.CurrentUser.Role = dr[0]["Role"].ToString();
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }

        /// <summary>
        /// Logout
        /// </summary>
        /// <author>humkyung</author>
        /// <date>2017.05.07</date>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void FrmMain_FormClosed(object sender, FormClosedEventArgs e)
        {
        }
    }
}

