using System;
using System.Collections.Generic;
using System.Data;
using System.Drawing;
using System.IO;
using System.Linq;
using System.Windows.Forms;
using Configuration.Properties;
using CADAutomation;
using IDCS.DataTransferObjects;
using IDCS.Framework.Service;
using IDCS.Service.Interface;
using Microsoft.Win32;
using Telerik.WinControls.UI;

namespace Configuration
{
	public partial class UserControlBorderSetting : ThemeChangeable
	{
		public const string DisplayName = "Border";
		public string BorderFilePath
		{
			get { return radTextBoxControlBorderFile.Text; }	
		}

		private string TempFolder
		{
			get { return Environment.GetFolderPath(Environment.SpecialFolder.UserProfile); }
		}

		private UserControlBorderSettingData _oIsoDwgArea = null;
		private UserControlIDCSRemoveSetting _oRemoveArea = null;
		private UserControlLineDataBorderSettingData _oLineDataSetting = null;

		public UserControlBorderSetting()
		{
			InitializeComponent();
			Program.OnIdleUpdate += new Program.IdleUpdateEvent(OnIdleUpdate);
			ApplyTheme(this.Controls);
		}

		private void radButtonBorderFile_Click(object sender, EventArgs e)
		{
			OpenFileDialog oFileDialog = new OpenFileDialog();
			oFileDialog.Filter = "AutoCAD File(*.dwg)|*.dwg";
			if (DialogResult.OK == oFileDialog.ShowDialog())
			{
				radTextBoxControlBorderFile.Text = oFileDialog.FileName;
			}
		}

		private void radButtonSave_Click(object sender, EventArgs e)
		{
			SaveData();
		}

		/// <summary>
		/// save setting data
		/// </summary>
		/// <author>humkyung</author>
		/// <date>2016.09.01</date>
		public override void SaveData()
		{
 			base.SaveData();

			using (IIDCSService svcProxy = ServiceFactory.CreateServiceChannel<IIDCSService>(AppDocData.ServiceURL))
			{
				try
				{
					using (DataSet dsParam = svcProxy.GetCommandParam("IDCS.BSL", "SaveConfigsCommand"))
					{
						{
							DataRow drParam = dsParam.Tables["Params"].NewRow();
							drParam["Section"] = "Border Setting";
							drParam["Key"] = "Border File Path";
							drParam["Value"] = radTextBoxControlBorderFile.Text;
							dsParam.Tables["Params"].Rows.Add(drParam);
						}
						svcProxy.ExecuteCommand("IDCS.BSL", "SaveConfigsCommand", dsParam);
					}

					if (null != this._oIsoDwgArea) this._oIsoDwgArea.SaveData();
					if (null != this._oLineDataSetting) this._oLineDataSetting.SaveData();
					if (null != this._oRemoveArea) this._oRemoveArea.SaveData();
				}
				catch (Exception ex)
				{
					MessageBox.Show(ex.Message, "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
				}
			}
		}

		private void UserControlBorderSetting_Load(object sender, EventArgs e)
		{
			{
				_oIsoDwgArea = new UserControlBorderSettingData() { HeaderText = "Iso Dwg Area" };
				_oIsoDwgArea.Dock = DockStyle.Fill;
				splitPanelBorderSetting.Controls.Add(_oIsoDwgArea);
			}
			{
				_oRemoveArea = new UserControlIDCSRemoveSetting() { HeaderText = "Remove Area" };
				_oRemoveArea.Dock = DockStyle.Fill;
				splitPanelRemoveSetting.Controls.Add(_oRemoveArea);
			}

			{
				_oLineDataSetting = new UserControlLineDataBorderSettingData() { HeaderText = "Line Data Setting" };
				_oLineDataSetting.Dock = DockStyle.Fill;
				splitPanelLineDataSetting.Controls.Add(_oLineDataSetting);
			}

			using (IIDCSService svcProxy = ServiceFactory.CreateServiceChannel<IIDCSService>(AppDocData.ServiceURL))
			{
				List<ProjectDTO> oProjectList = new List<ProjectDTO>();
				using (DataSet dsParam = svcProxy.GetCommandParam("IDCS.BSL", "GetAllProjectListCommand"))
				{
					using (DataSet ds = svcProxy.ExecuteCommand("IDCS.BSL", "GetAllProjectListCommand", dsParam))
					{
						foreach (DataRow oRow in ds.Tables[0].Rows)
						{
							oProjectList.Add(
								new ProjectDTO()
								{
									Id = Convert.ToInt32(oRow["ProjectMasterID"]),
									Name = oRow["ProjectNo"].ToString(),
									Desc = oRow["NameView"].ToString(),
									Enabled = (true.ToString() == oRow["SP3DFlag"].ToString())
								}
							);
						}
					}
				}
			    foreach (ProjectDTO oPrjDTO in oProjectList)
			    {
					if (!oPrjDTO.Enabled) continue;
			        RadListDataItem oItem = new RadListDataItem();
			        {
			            oItem.Text = string.Format("{0}({1})", oPrjDTO.Name, oPrjDTO.Desc);
			            oItem.Tag = oPrjDTO;
			        }
			        radDropDownListProject.Items.Add(oItem);
			    }

				if (!string.IsNullOrEmpty(UserInfo.ProjectCode))
				{
					RadListDataItem oItem = radDropDownListProject.Items.First(param => (param.Tag as ProjectDTO).Id.ToString() == UserInfo.ProjectCode);
					if (null != oItem) radDropDownListProject.SelectedItem = oItem;
				}
			}

			LoadSiteData();
		}

		/// <summary>
		/// load setting data from database
		/// </summary>
		/// <author>humkyung</author>
		public override void LoadData()
		{
			if (string.IsNullOrEmpty(UserInfo.ProjectCode) || string.IsNullOrEmpty(UserInfo.SiteID)) return;

			#region clear controls value
			radTextBoxControlBorderFile.Text = string.Empty;
			#endregion

			try
			{
				using (IIDCSService svcProxy = ServiceFactory.CreateServiceChannel<IIDCSService>(AppDocData.ServiceURL))
				{
					using (System.Data.DataSet dsParam = svcProxy.GetCommandParam("IDCS.BSL", "GetConfigCommand"))
					{
						dsParam.Tables["Params"].Rows.Clear();
						{
							System.Data.DataRow drParam = dsParam.Tables["Params"].NewRow();
							drParam["Section"] = "Border Setting";
							drParam["Key"] = string.Empty;
							dsParam.Tables["Params"].Rows.Add(drParam);
						}
						using (DataSet ds = svcProxy.ExecuteCommand("IDCS.BSL","GetConfigCommand",dsParam))
						{
							foreach (DataRow oRow in ds.Tables[0].Rows)
							{
								object oValue = oRow["key"];
								string sKey = (null != oValue) ? oValue.ToString() : string.Empty;
								if ("Border File Path" == sKey)
								{
									radTextBoxControlBorderFile.Text = oRow["value"].ToString();
								}
							}
						}
					}
				}

				if (null != this._oIsoDwgArea) this._oIsoDwgArea.LoadData();
				if (null != this._oLineDataSetting) this._oLineDataSetting.LoadData();
				if (null != this._oRemoveArea) this._oRemoveArea.LoadData();
			}
			catch (Exception ex)
			{
				MessageBox.Show(ex.Message, "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
			}
		}

		/// <summary>
		/// load site setting data from database
		/// </summary>
		/// <author>humkyung</author>
		/// <date>2016.08.11</date>
		private void LoadSiteData()
		{
			if (string.IsNullOrEmpty(UserInfo.ProjectCode)) return;

			try
			{
				using (IIDCSService svcProxy = ServiceFactory.CreateServiceChannel<IIDCSService>(AppDocData.ServiceURL))
				{
					/// load and display site list
					radDropDownListSite.Items.Clear();

					List<SiteDTO> oSiteList = new List<SiteDTO>();
					using (DataSet dsParam = svcProxy.GetCommandParam("IDCS.BSL", "GetSiteListCommand"))
					{
						List<SiteDTO> oSiteDTOColl = new List<SiteDTO>();
						using (DataSet dsSite = svcProxy.ExecuteCommand("IDCS.BSL", "GetSiteListCommand", dsParam))
						{
							foreach (DataRow oRow in dsSite.Tables[0].Rows)
							{
								oSiteList.Add(
									new SiteDTO()
									{
										Id = Convert.ToInt32(oRow["SiteID"]),
										Name = oRow["Name"].ToString(),
										Desc = oRow["Desc"].ToString(),
										DeleteYN = false
									}
								);
							}
						}
					}

					foreach (SiteDTO dto in oSiteList)
					{
						RadListDataItem oItem = new RadListDataItem();
						{
							oItem.Text = string.Format("{0}({1})", dto.Name, dto.Desc);
							oItem.Tag = dto;
						}
						radDropDownListSite.Items.Add(oItem);
					}
					if (!string.IsNullOrEmpty(UserInfo.SiteID))
					{
						RadListDataItem oItem = radDropDownListSite.Items.FirstOrDefault(param => (param.Tag as SiteDTO).Id.ToString() == UserInfo.SiteID);
						if (null != oItem)
						{
							radDropDownListSite.SelectedItem = oItem;
						}
						else
						{
							UserInfo.SiteID = string.Empty;
						}
					}
					/// up to here
				}
			}
			catch (Exception ex)
			{
				MessageBox.Show(ex.Message, "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
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
			try
			{
				if (false == AutoCADLauncher.LaunchIfPlugInInAppServiceNotAvailable(this)) return;

				string sTempFilePath = Path.Combine(Path.GetTempPath(), Path.GetFileName(radTextBoxControlBorderFile.Text));
				if (File.Exists(radTextBoxControlBorderFile.Text) && !File.Exists(sTempFilePath))
				{
					File.Copy(radTextBoxControlBorderFile.Text, sTempFilePath);
				}
				PF.Open(sTempFilePath);
				PF.gbl_doc = PF.gbl_app.ActiveDocument;
				string sAutoCADVer = PF.GetVersionNumber();
				PF.gbl_doc.Application.ZoomExtents();

				

				//#region Write output file for AutoCAD
				//string sOutputFilePath = Path.Combine(TempFolder, "tmp.sdwf");
				//using (StreamWriter oStreamWriter = new StreamWriter(sOutputFilePath))
				//{
				//	if (null != this._oIsoDwgArea) this._oIsoDwgArea.Write(oStreamWriter);
				//	if (null != this._oLineDataSetting) this._oLineDataSetting.Write(oStreamWriter);
				//	//if (null != this._oLineDataArea) this._oLineDataArea.Write(oStreamWriter);
				//	oStreamWriter.Close();
				//}
				//#endregion

				//string sDllFilePath = Path.Combine(AutoCADLauncher.AutoCADPluginsPath, "IDCS4SP3D", sAutoCADVer, "aDwgWriter.dll");
				//{
				//	sDllFilePath = sDllFilePath.Replace("\\", "/");
				//	PF.gbl_app.ActiveDocument.SendCommand(string.Format("(command \"netload\" \"{0}\") ", sDllFilePath.Replace(@"\",@"\\")));
				//}
				//try
				//{
				//	PF.gbl_app.ActiveDocument.SendCommand(string.Format("sdwf \"{0}\" ", sOutputFilePath));
				//}
				//catch (Exception ex)
				//{
				//	MessageBox.Show(ex.Message, "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
				//}
			}
			catch(Exception ex)
			{
				new TechSun.Theme.frmWarning(ex.Message).ShowDialog(this);
			}
		}

		private void radDropDownListProject_SelectedIndexChanged(object sender, Telerik.WinControls.UI.Data.PositionChangedEventArgs e)
		{
			if (radDropDownListProject.SelectedIndex >= 0)
			{
				ProjectDTO oPrjDTO = radDropDownListProject.Items[radDropDownListProject.SelectedIndex].Tag as ProjectDTO;
				if (null != oPrjDTO)
				{
				    UserInfo.ProjectCode = oPrjDTO.Id.ToString();
				    LoadSiteData();
				}
			}
		}

		/// <summary>
		/// load setting of given site
		/// </summary>
		/// <author>humkyung</author>
		/// <date>2016.08.11</date>
		/// <param name="sender"></param>
		/// <param name="e"></param>
		private void radDropDownListSite_SelectedIndexChanged(object sender, Telerik.WinControls.UI.Data.PositionChangedEventArgs e)
		{
			if (radDropDownListSite.SelectedIndex >= 0)
			{
				SiteDTO oSiteDTO = radDropDownListSite.Items[radDropDownListSite.SelectedIndex].Tag as SiteDTO;
				if (null != oSiteDTO)
				{
					UserInfo.SiteID = oSiteDTO.Id.ToString();
					LoadData();
				}
			}
		}

		/// <summary>
		/// update enabled property of control
		/// </summary>
		/// <author>humkyung</author>
		/// <date>2016.08.31</date>
		public void OnIdleUpdate()
		{
			bool bEnabled = (!string.IsNullOrEmpty(UserInfo.ProjectCode) && !string.IsNullOrEmpty(UserInfo.SiteID));
			foreach (Control oCtrl in this.Controls)
			{
				if (oCtrl is RadLabel) continue;
				if ((oCtrl == radDropDownListProject) || (oCtrl == radDropDownListSite) || (oCtrl == this.radButtonDisplayOnAutoCAD)) continue;
				oCtrl.Enabled = bEnabled;
			}

			if ((true == this.radButtonDisplayOnAutoCAD.Enabled) && (false == Configuration.AutoCADLauncher.AutoCAD2014Installed))
			{
				this.radButtonDisplayOnAutoCAD.Enabled = false;
			}
		}
	}
}
