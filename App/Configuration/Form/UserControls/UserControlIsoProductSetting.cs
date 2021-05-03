using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Linq;
using System.Text;
using System.Windows.Forms;

using Configuration.Properties;
//using CADAutomation;
using Microsoft.Win32;
using Telerik.WinControls.UI;
using SmartISO.Framework.Service;
using SmartISO.Service.Interface;

namespace Configuration
{
	public partial class UserControlIsoProductSetting : ThemeChangeable
	{
		private List<string> LineDataColNameList = new List<string>();

        private Int64 ProjectUID
        {
            get { return Convert.ToInt64(this.radDropDownListProject.Items[this.radDropDownListProject.SelectedIndex].Tag); }
        }
        private Int64 SiteUID
        {
            get { return Convert.ToInt64(this.radDropDownListSite.Items[this.radDropDownListSite.SelectedIndex].Tag); }
        }

		public string[] ProductFolderComBobox = { "PART", "PLANT", "AREA", "UNIT" };

		public const string DisplayName = "Update ISO";
		public UserControlIsoProductSetting()
		{
			InitializeComponent();
			Program.OnIdleUpdate += new Program.IdleUpdateEvent(OnIdleUpdate);
            this.Disposed += new EventHandler(UserControlIsoProductSetting_Disposed);
			ApplyTheme(this.Controls);
		}

        void UserControlIsoProductSetting_Disposed(object sender, EventArgs e)
        {
            Program.OnIdleUpdate -= new Program.IdleUpdateEvent(OnIdleUpdate);
        }
		private UserControlIsoProductSetting _oIsoProduct = null;

		public void OnIdleUpdate()
		{
			bool bEnabled = (this.radDropDownListProject.SelectedIndex >= 0) && (this.radDropDownListSite.SelectedIndex >= 0);
			foreach (Control oCtrl in this.Controls)
			{
                if (oCtrl is RadLabel) continue;
                if ((oCtrl == this.radDropDownListProject) || (oCtrl == this.radDropDownListSite)) continue;
                oCtrl.Enabled = bEnabled;
			}
		}
		/// <summary>
		/// Form Setting Column List
		/// </summary>
		/// <author>yongs</author>
		/// <date>2017.02.20</date>
		private void GetLineDataColNameList()
		{
			try
			{
				LineDataColNameList.Clear();

				using (ISmartISOService svcProxy = ServiceFactory.CreateServiceChannel<ISmartISOService>(AppDocData.ServiceURL))
				{
					using (DataSet dsParam = svcProxy.GetCommandParam("SmartISO.BSL", "GetLineDataColInfoListCommand"))
					{
						using (DataSet ds = svcProxy.ExecuteCommand("SmartISO.BSL", "GetLineDataColInfoListCommand", dsParam))
						{
							foreach (DataRow oRow in ds.Tables["ColInfo"].Rows)
							{
								LineDataColNameList.Add(Name = oRow["Name"].ToString());
							}
						}
					}
				}
				LineDataColNameList.Add("LAST_REV_NO");

				LineDataColNameList.Sort();
			}
			catch (Exception ex)
			{
				MessageBox.Show(ex.Message, "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
			}
			/// up to here
		}
		/// <summary>
		/// load site setting data from database
		/// </summary>
		/// <author>humkyung</author>
		/// <date>2016.08.11</date>
		private void LoadSiteData()
		{
            if (this.radDropDownListProject.SelectedIndex < 0) return;

            try
            {
                using (ISmartISOService svcProxy = ServiceFactory.CreateServiceChannel<ISmartISOService>(AppDocData.ServiceURL))
                {
                    /// load and display site list
                    this.radDropDownListSite.Items.Clear();

                    using (DataSet dsParam = svcProxy.GetCommandParam("SmartISO.BSL", "GetSiteListCommand"))
                    {
                        DataRow dr = dsParam.Tables["Params"].NewRow();
                        {
                            dr["Project_UID"] = ProjectUID;
                        }
                        dsParam.Tables["Params"].Rows.Add(dr);
                        using (DataSet ds = svcProxy.ExecuteCommand("SmartISO.BSL", "GetSiteListCommand", dsParam))
                        {
                            foreach (DataRow oRow in ds.Tables[0].Rows)
                            {
                                RadListDataItem oItem = new RadListDataItem();
                                {
                                    oItem.Text = string.Format("{0}", oRow["SiteNo"].ToString());
                                    oItem.Tag = oRow["UID"].ToString();
                                }
                                this.radDropDownListSite.Items.Add(oItem);
                            }
                        }
                    }

                    if (!string.IsNullOrEmpty(UserInfo.SiteID))
                    {
                        try
                        {
                            RadListDataItem oItem = radDropDownListSite.Items.FirstOrDefault(param => (param.Tag as string) == UserInfo.SiteID);
                            if (null != oItem)
                            {
                                this.radDropDownListSite.SelectedItem = oItem;
                            }
                        }
                        catch
                        {
                            UserInfo.SiteID = string.Empty;
                        }
                    }
                    /// up to here
                }
            }
            catch (Exception ex)
            {
                new TechSun.Theme.frmWarning(ex.Message).ShowDialog(this);
            }
		}

		private void UserControlIsoProductSetting_Load(object sender, EventArgs e)
		{
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

            foreach (string sItem in ProductFolderComBobox)
            {
                RadListDataItem oItem = new RadListDataItem();
                {
                    oItem.Text = sItem;
                }
                radDropDownListProductFolder.Items.Add(oItem);
            }

            //    if (!string.IsNullOrEmpty(UserInfo.ProjectCode))
            //    {
            //        RadListDataItem oItem = radDropDownListProject.Items.First(param => (param.Tag as ProjectDTO).Id.ToString() == UserInfo.ProjectCode);
            //        if (null != oItem) radDropDownListProject.SelectedItem = oItem;
            //    }
            //}
			GetLineDataColNameList();
			LoadSiteData();
		}

		private void radDropDownListProject_SelectedIndexChanged(object sender, Telerik.WinControls.UI.Data.PositionChangedEventArgs e)
		{
            if (radDropDownListProject.SelectedIndex >= 0)
            {
                LoadSiteData();
            }
		}

		private void radDropDownListSite_SelectedIndexChanged(object sender, Telerik.WinControls.UI.Data.PositionChangedEventArgs e)
		{
            if (radDropDownListSite.SelectedIndex >= 0)
            {
                string sSiteID = radDropDownListSite.Items[radDropDownListSite.SelectedIndex].Tag as string;
                if (!string.IsNullOrEmpty(sSiteID))
                {
                    UserInfo.SiteID = sSiteID;
                    LoadData();
                }
            }
		}

		/// <summary>
		/// load and display project setting
		/// </summary>
		/// <author>humkyung</author>
		/// <date>2016.07.13</date>
		public override void LoadData()
		{
			if ((this.radDropDownListProject.SelectedIndex < 0) || (this.radDropDownListSite.SelectedIndex < 0)) return;

			using (ISmartISOService svcProxy = ServiceFactory.CreateServiceChannel<ISmartISOService>(AppDocData.ServiceURL))
			{
				#region clear control values
				radTextBoxControlIsoDwgFolder.Text = string.Empty;
				radTextBoxControlPcdFileFolder.Text = string.Empty;
				radDropDownListProductFolder.Text = string.Empty;
				radTextBoxControlSourceFileFormat.Text = string.Empty;
				radTextBoxControlProductFileFormat.Text = string.Empty;
				radTextBoxControlProductFileExtFormat.Text = string.Empty;
				#endregion

				using (System.Data.DataSet dsParam = svcProxy.GetCommandParam("SmartISO.BSL", "GetConfigCommand"))
				{
					{
						System.Data.DataRow drParam = dsParam.Tables["Params"].NewRow();
                        drParam["Ref_UID"] = SiteUID;
						drParam["Section"] = "Iso Product Setting";
						drParam["Key"] = string.Empty;
						dsParam.Tables["Params"].Rows.Add(drParam);
					}
					using (DataSet ds = svcProxy.ExecuteCommand("SmartISO.BSL", "GetConfigCommand", dsParam))
					{
						foreach (DataRow oRow in ds.Tables[0].Rows)
						{
							object oValue = oRow["key"];
							string sKey = (null != oValue) ? oValue.ToString() : string.Empty;
							if ("ISO Dwg Folder" == sKey)
							{
								radTextBoxControlIsoDwgFolder.Text = oRow["Value"].ToString();
							}
							else if ("PCD File Folder" == sKey)
							{
								radTextBoxControlPcdFileFolder.Text = oRow["Value"].ToString();
							}
							else if ("Product Folder" == sKey)
							{
								radDropDownListProductFolder.Text = oRow["Value"].ToString();
							}
							else if ("Source ISO File Name Format" == sKey)
							{
								radTextBoxControlSourceFileFormat.Text = oRow["Value"].ToString();
							}
							else if ("Product ISO File Name Format" == sKey)
							{
								radTextBoxControlProductFileFormat.Text = oRow["Value"].ToString();
							}
							else if ("Product ISO File Ext Format" == sKey)
							{
								radTextBoxControlProductFileExtFormat.Text = oRow["Value"].ToString();
							}
							else if ("Draw No Format" == sKey)
							{
								radTextBoxControlDrawNo.Text = oRow["Value"].ToString();
							}
						}
					}
				}
				/// up to here
			}
		}
		/// <summary>
		/// save setting data
		/// </summary>
		/// <author>humkyung</author>
		/// <date>2016.09.01</date>
		public override void SaveData()
		{
			base.SaveData();

			if ((this.radDropDownListProject.SelectedIndex < 0) || (this.radDropDownListSite.SelectedIndex < 0)) return;

			using (ISmartISOService svcProxy = ServiceFactory.CreateServiceChannel<ISmartISOService>(AppDocData.ServiceURL))
			{
				try
				{
					string sValue = string.Empty;
					using (DataSet dsParam = svcProxy.GetCommandParam("SmartISO.BSL", "SaveConfigsCommand"))
					{
						{
							DataRow drParam = dsParam.Tables["Params"].NewRow();
                            drParam["Ref_UID"] = SiteUID;
							drParam["Section"] = "Iso Product Setting";
							drParam["Key"] = "ISO Dwg Folder";
							drParam["Value"] = radTextBoxControlIsoDwgFolder.Text;
							dsParam.Tables["Params"].Rows.Add(drParam);
						}
						{
							DataRow drParam = dsParam.Tables["Params"].NewRow();
                            drParam["Ref_UID"] = SiteUID;
							drParam["Section"] = "Iso Product Setting";
							drParam["Key"] = "PCD File Folder";
							drParam["Value"] = radTextBoxControlPcdFileFolder.Text;
							dsParam.Tables["Params"].Rows.Add(drParam);
						}
						{
							DataRow drParam = dsParam.Tables["Params"].NewRow();
                            drParam["Ref_UID"] = SiteUID;
							drParam["Section"] = "Iso Product Setting";
							drParam["Key"] = "Product Folder";
							drParam["Value"] = radDropDownListProductFolder.Text;
							dsParam.Tables["Params"].Rows.Add(drParam);
						}
						{
							DataRow drParam = dsParam.Tables["Params"].NewRow();
                            drParam["Ref_UID"] = SiteUID;
							drParam["Section"] = "Iso Product Setting";
							drParam["Key"] = "Source ISO File Name Format";
							drParam["Value"] = radTextBoxControlSourceFileFormat.Text;
							dsParam.Tables["Params"].Rows.Add(drParam);
						}
						{
							DataRow drParam = dsParam.Tables["Params"].NewRow();
                            drParam["Ref_UID"] = SiteUID;
							drParam["Section"] = "Iso Product Setting";
							drParam["Key"] = "Product ISO File Name Format";
							drParam["Value"] = radTextBoxControlProductFileFormat.Text;
							dsParam.Tables["Params"].Rows.Add(drParam);
						}
						{
							DataRow drParam = dsParam.Tables["Params"].NewRow();
                            drParam["Ref_UID"] = SiteUID;
							drParam["Section"] = "Iso Product Setting";
							drParam["Key"] = "Product ISO File Ext Format";
							drParam["Value"] = radTextBoxControlProductFileExtFormat.Text;
							dsParam.Tables["Params"].Rows.Add(drParam);
						}
						{
							DataRow drParam = dsParam.Tables["Params"].NewRow();
                            drParam["Ref_UID"] = SiteUID;
							drParam["Section"] = "Iso Product Setting";
							drParam["Key"] = "Draw No Format";
							drParam["Value"] = radTextBoxControlDrawNo.Text;
							dsParam.Tables["Params"].Rows.Add(drParam);
						}

						svcProxy.ExecuteCommand("SmartISO.BSL", "SaveConfigsCommand", dsParam);
					}
					/// up to here
				}
				catch (Exception ex)
				{
					new TechSun.Theme.frmWarning(ex.Message).ShowDialog(this);
				}
			}
		}

		private void radButtonSourceISOFileNameFormat_Click(object sender, EventArgs e)
		{
			RadFormSmartISOFormFormatting aFormattingDlg = new RadFormSmartISOFormFormatting(this.radTextBoxControlSourceFileFormat.Text, this.LineDataColNameList);
			if (DialogResult.OK == aFormattingDlg.ShowDialog(this))
			{
				this.radTextBoxControlSourceFileFormat.Text = aFormattingDlg.Result;
			}
		}

		private void kRadButtonProductISOFileNameFormat_Click(object sender, EventArgs e)
		{
			RadFormSmartISOFormFormatting aFormattingDlg = new RadFormSmartISOFormFormatting(this.radTextBoxControlProductFileFormat.Text, this.LineDataColNameList);
			if (DialogResult.OK == aFormattingDlg.ShowDialog(this))
			{
				this.radTextBoxControlProductFileFormat.Text = aFormattingDlg.Result;
			}
		}

		private void kRadButtonProductISOFileExtFormat_Click(object sender, EventArgs e)
		{
			List<string> oInputList = new List<string> { "FLUID","LINE_NO","REV NO" };
			RadFormSmartISOFormFormatting aFormattingDlg = new RadFormSmartISOFormFormatting(this.radTextBoxControlProductFileExtFormat.Text, oInputList);
			if (DialogResult.OK == aFormattingDlg.ShowDialog(this))
			{
				this.radTextBoxControlProductFileExtFormat.Text = aFormattingDlg.Result;
			}
		}

		private void radButtonDrawNo_Click(object sender, EventArgs e)
		{
			RadFormSmartISOFormFormatting aFormattingDlg = new RadFormSmartISOFormFormatting(this.radTextBoxControlDrawNo.Text, this.LineDataColNameList);
			if (DialogResult.OK == aFormattingDlg.ShowDialog(this))
			{
				this.radTextBoxControlDrawNo.Text = aFormattingDlg.Result;
			}
		}
	}
}
