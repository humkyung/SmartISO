using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.IO;
using SmartISO.Service.Interface;
using SmartISO.Framework.Service;
using Telerik.WinControls.UI;

namespace Configuration
{
	public partial class UserControlBorderSetting : ThemeChangeable
	{
		public const string DisplayName = "Border Setting";

        private Int64 ProjectUID
        {
            get { return Convert.ToInt64(this.radDropDownListProject.Items[this.radDropDownListProject.SelectedIndex].Tag); }
        }

		private string TempFolder
		{
			get { return Environment.GetFolderPath(Environment.SpecialFolder.UserProfile); }
		}

		/// get autocad plugins folder
		private string AutoCADPluginsPath
		{
			get { return Path.Combine(Environment.GetFolderPath(Environment.SpecialFolder.CommonApplicationData), "Autodesk", "ApplicationPlugins"); }
		}

		private UserControlBorderSettingData _oIsoDwgArea = null;
        private UserControlRemoveSetting _oRemoveArea = null;
        private UserControlBorderSettingData _oLineDataLocation = null;

		public UserControlBorderSetting()
		{
			InitializeComponent();
            {
                Program.OnIdleUpdate += new Program.IdleUpdateEvent(OnIdleUpdate);
                this.Disposed += new EventHandler(UserControlBorderSetting_Disposed);

                (this.radBrowseEditorAutoCAD.Dialog as OpenFileDialog).Filter = "(*.exe)|*.exe";
                (this.radBrowseEditorBorderFile.Dialog as OpenFileDialog).Filter = "(*.dwg)|*.dwg";
            }
			ApplyTheme(this.Controls);
		}

        void UserControlBorderSetting_Disposed(object sender, EventArgs e)
        {
            Program.OnIdleUpdate -= new Program.IdleUpdateEvent(OnIdleUpdate);
        }

        public override void SaveData()
        {
            base.SaveData();

            if (this.radDropDownListProject.SelectedIndex < 0) return;

            using (ISmartISOService svcProxy = ServiceFactory.CreateServiceChannel<ISmartISOService>(AppDocData.ServiceURL))
            {
                string sValue = string.Empty;
                using (DataSet dsParam = svcProxy.GetCommandParam("SmartISO.BSL", "SaveConfigsCommand"))
                {
                    try
                    {
                        DataRow drParam = dsParam.Tables["Params"].NewRow();
                        drParam["Ref_UID"] = ProjectUID;
                        drParam["Section"] = "Border Setting";
                        drParam["Key"] = "Border File Path";
                        drParam["Value"] = (null != this.radBrowseEditorBorderFile.Value) ? this.radBrowseEditorBorderFile.Value.ToString() : string.Empty;
                        dsParam.Tables["Params"].Rows.Add(drParam);

                        svcProxy.ExecuteCommand("SmartISO.BSL", "SaveConfigsCommand", dsParam);
                    }
                    catch (Exception ex)
                    {
                        new TechSun.Theme.frmWarning(ex.Message).ShowDialog(this);
                    }
                }
            }

            if (null != this._oIsoDwgArea) this._oIsoDwgArea.SaveData();
            if (null != this._oRemoveArea) this._oRemoveArea.SaveData();
            if (null != this._oLineDataLocation) this._oLineDataLocation.SaveData();
        }

		private void radButtonSave_Click(object sender, EventArgs e)
		{
		}

		private void UserControlBorderSetting_Load(object sender, EventArgs e)
		{
			Telerik.WinControls.UI.SplitPanel oIsoDwgAreaPanel = new Telerik.WinControls.UI.SplitPanel();
			{
				_oIsoDwgArea = new UserControlBorderSettingData() { HeaderText = "Iso Dwg Area" };
				_oIsoDwgArea.Dock = DockStyle.Fill;
				oIsoDwgAreaPanel.Controls.Add(_oIsoDwgArea);
				oIsoDwgAreaPanel.SizeInfo.MinimumSize = oIsoDwgAreaPanel.SizeInfo.MaximumSize = new System.Drawing.Size(0,130);
				radSplitContainerBorderSetting.Controls.Add(oIsoDwgAreaPanel);
			}

            Telerik.WinControls.UI.SplitPanel oRemoveAreaPanel = new Telerik.WinControls.UI.SplitPanel();
            {
                _oRemoveArea = new UserControlRemoveSetting() { HeaderText = "Remove Area" };
                _oRemoveArea.Dock = DockStyle.Fill;
                oRemoveAreaPanel.Controls.Add(_oRemoveArea);
                oRemoveAreaPanel.SizeInfo.MinimumSize = oRemoveAreaPanel.SizeInfo.MaximumSize = new System.Drawing.Size(0, 180);
                radSplitContainerBorderSetting.Controls.Add(oRemoveAreaPanel);
            }

			Telerik.WinControls.UI.SplitPanel oLineDataPosPanel = new Telerik.WinControls.UI.SplitPanel();
			{
				_oLineDataLocation = new UserControlBorderSettingData() { HeaderText = "Line Data Location" };
                _oLineDataLocation.Dock = DockStyle.Fill;
                oLineDataPosPanel.Controls.Add(_oLineDataLocation);
				radSplitContainerBorderSetting.Controls.Add(oLineDataPosPanel);
			}

            using (ISmartISOService svcProxy = ServiceFactory.CreateServiceChannel<ISmartISOService>(AppDocData.ServiceURL))
            {
                using (DataSet dsParam = svcProxy.GetCommandParam("SmartISO.BSL", "GetAllProjectListCommand"))
                {
                    using (DataSet ds = svcProxy.ExecuteCommand("SmartISO.BSL", "GetAllProjectListCommand", dsParam))
                    {
                        foreach (DataRow oRow in ds.Tables[0].Rows)
                        {
                            if (true.ToString() == oRow["DeleteYN"].ToString()) continue;
                            RadListDataItem oItem = new RadListDataItem();
                            {
                                oItem.Text = string.Format("{0}({1})", oRow["ProjectNo"].ToString(), oRow["Desc"].ToString());
                                oItem.Tag = oRow["UID"].ToString();
                            }
                            this.radDropDownListProject.Items.Add(oItem);
                        }
                    }
                }
            }
		}

		/// <summary>
		/// load setting data from database
		/// </summary>
		/// <author>humkyung</author>
		public override void LoadData()
		{
			if (this.radDropDownListProject.SelectedIndex < 0) return;

			#region clear controls value
			this.radBrowseEditorBorderFile.Value = string.Empty;
			#endregion

			try
			{
				using (ISmartISOService svcProxy = ServiceFactory.CreateServiceChannel<ISmartISOService>(AppDocData.ServiceURL))
				{
                    using (System.Data.DataSet dsParam = svcProxy.GetCommandParam("SmartISO.BSL", "GetConfigCommand"))
                    {
                        DataRow dr = dsParam.Tables["Params"].NewRow();
                        dr["Ref_UID"] = ProjectUID;
                        dr["Section"] = "Border Setting";
                        dr["Key"] = string.Empty;
                        dsParam.Tables["Params"].Rows.Add(dr);

                        using (DataSet ds = svcProxy.ExecuteCommand("SmartISO.BSL", "GetConfigCommand", dsParam))
                        {
                            foreach (DataRow oRow in ds.Tables[0].Rows)
                            {
                                object oValue = oRow["Key"];
                                string sKey = (null != oValue) ? oValue.ToString() : string.Empty;
                                if ("Border File Path" == sKey)
                                {
                                    this.radBrowseEditorBorderFile.Value = oRow["Value"].ToString();
                                }
                            }
                        }
                    }
				}

				if (null != this._oIsoDwgArea) this._oIsoDwgArea.LoadData();
                if (null != this._oRemoveArea) this._oRemoveArea.LoadData();
                if (null != this._oLineDataLocation) this._oLineDataLocation.LoadData();
			}
			catch (Exception ex)
			{
				new TechSun.Theme.frmWarning(ex.Message).ShowDialog(this);
			}
		}

		/// <summary>
		/// display border setting on autocad
		/// </summary>
		/// <author>humkyung</author>
		/// <date>2016.07.12</date>
		/// <param name="sender"></param>
		/// <param name="e"></param>
		private void radButtonDisplayOnAutoCAD_Click(object sender, EventArgs e)
		{
            bool? res = AutoCADLauncher.LaunchIfPlugInInAppServiceNotAvailable(this);
            if (res.HasValue && (true == res))
            {
                using (PlugInServiceOpenDwgCommand cmd = new PlugInServiceOpenDwgCommand() { FilePath = this.radBrowseEditorBorderFile.Value.ToString()})
                {
                    cmd.Execute();
                }
            }
		}

		private void radDropDownListProject_SelectedIndexChanged(object sender, Telerik.WinControls.UI.Data.PositionChangedEventArgs e)
		{
			if (radDropDownListProject.SelectedIndex >= 0)
			{
			    LoadData();
			}
		}

        private void radBrowseEditorAutoCAD_ValueChanged(object sender, EventArgs e)
        {
            AppDocData.AutoCADFilePath = this.radBrowseEditorAutoCAD.Value.ToString();
        }

        /// <summary>
        /// update enabled property of control
        /// </summary>
        /// <author>humkyung</author>
        /// <date>2016.08.31</date>
        public void OnIdleUpdate()
        {
            bool bEnabled = (this.radDropDownListProject.SelectedIndex >= 0);
            foreach (Control oCtrl in this.Controls)
            {
                if (oCtrl is RadLabel) continue;
                if ((oCtrl == radDropDownListProject) || (oCtrl == this.radButtonDisplayOnAutoCAD)) continue;
                oCtrl.Enabled = bEnabled;
            }

            //if ((true == this.radButtonDisplayOnAutoCAD.Enabled) && (string.IsNullOrEmpty(this.radBrowseEditorAutoCAD.Value) || string.IsNullOrEmpty(this.radBrowseEditorBorderFile.Value)))
            //{
            //    this.radButtonDisplayOnAutoCAD.Enabled = false;
            //}
        }
	}
}
