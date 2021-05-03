using System;
using System.Collections.Generic;
using System.Data;
using System.IO;
using System.Linq;
using System.Windows.Forms;
using Configuration.Properties;
using IDCS.DataTransferObjects;
using IDCS.Framework.Service;
using IDCS.Service.Interface;
using Telerik.WinControls.UI;

namespace Configuration
{
	public partial class UserControlPrjSetting : ThemeChangeable
	{
		public const string DisplayName = "Report";
		private List<string> _DrawingReportNameColl = new List<string>()
		{
			"One Sheet(4Rev)","One Sheet(7Rev)",
			"Multi Sheets(4Rev)","Multi Sheets(7Rev)",
			"One Sheet/One Row(4Rev)","One Sheet/One Row(7Rev)"
		};

		public string[] _SizeUnitColl = { "inch", "mm", "U X mm", "mm X mm" };
		public UserControlPrjSetting()
		{
			InitializeComponent();
			Program.OnIdleUpdate += new Program.IdleUpdateEvent(OnIdleUpdate);

			this.radGridViewDrawingReport.RowCount = this._DrawingReportNameColl.Count;
			foreach(GridViewRowInfo oGridRow in this.radGridViewDrawingReport.Rows)
			{
				this.radGridViewDrawingReport.Rows[oGridRow.Index].Cells["Name"].Value = this._DrawingReportNameColl[oGridRow.Index];
			}
			this.radGridViewDrawingReport.Columns[1].AutoSizeMode = BestFitColumnMode.AllCells;
			ApplyTheme(this.Controls);
		}

		/// <summary>
		/// load config
		/// </summary>
		/// <param name="sender"></param>
		/// <param name="e"></param>
		private void UserControlPrjSetting_Load(object sender, EventArgs e)
		{
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
		/// load and display project setting
		/// </summary>
		/// <author>humkyung</author>
		/// <date>2016.07.13</date>
		public override void LoadData()
		{
			if (string.IsNullOrEmpty(UserInfo.ProjectCode) || string.IsNullOrEmpty(UserInfo.SiteID)) return;

			radGridViewBOMHeaderTextLoc.RowCount = 1;

			using (IIDCSService svcProxy = ServiceFactory.CreateServiceChannel<IIDCSService>(AppDocData.ServiceURL))
			{
				
				#region clear control values
				//radDropDownListSizeUnit.Items.Clear();
				//radDropDownListBoltSizeUnit.Items.Clear();
				radDropDownListSizeUnit.Text = string.Empty;
				radDropDownListBoltSizeUnit.Text = string.Empty;
				//radTextBoxControlSizeUnit.Text = string.Empty;
				//radTextBoxControlBoltSizeUnit.Text = string.Empty;
				radCheckBoxEnableHoldItemCheck.Checked = false;
				radTextBoxControlDelimiterForInsulation.Text = string.Empty;
				radTextBoxControlDeletedItemMessage.Text = string.Empty;
				radCheckBoxWriteLatestDescToRemarkColumn.Checked = false;

				foreach (string sItem in _SizeUnitColl)
				{
					radDropDownListSizeUnit.Items.Add(new RadListDataItem(){Text = sItem});
					radDropDownListBoltSizeUnit.Items.Add(new RadListDataItem() {Text = sItem });
				}
				foreach (GridViewRowInfo oGridRow in radGridViewBOMHeaderTextLoc.Rows)
				{
					foreach (GridViewCellInfo oGridCell in oGridRow.Cells)
					{
						radGridViewBOMHeaderTextLoc.Rows[oGridRow.Index].Cells[oGridCell.ColumnInfo.Index].Value = string.Empty;
					}
				}

				foreach (GridViewRowInfo oGridRow in this.radGridViewDrawingReport.Rows)
				{
					this.radGridViewDrawingReport.Rows[oGridRow.Index].Cells[1].Value = string.Empty;
				}
				#endregion

				using (System.Data.DataSet dsParam = svcProxy.GetCommandParam("IDCS.BSL", "GetConfigCommand"))
				{
					{
						System.Data.DataRow drParam = dsParam.Tables["Params"].NewRow();
						drParam["Section"] = "Prj Setting";
						drParam["Key"] = string.Empty;
						dsParam.Tables["Params"].Rows.Add(drParam);
					}
					using (DataSet ds = svcProxy.ExecuteCommand("IDCS.BSL","GetConfigCommand",dsParam))
					{
						foreach (DataRow oRow in ds.Tables[0].Rows)
						{
							object oValue = oRow["key"];
							string sKey = (null != oValue) ? oValue.ToString() : string.Empty;
							if ("Size Unit" == sKey)
							{
								radDropDownListSizeUnit.Text = oRow["value"].ToString();
							}
							else if ("Bolt Size Unit" == sKey)
							{
								radDropDownListBoltSizeUnit.Text = oRow["value"].ToString();
							}
							else if ("Bolt Set" == sKey)
							{
								radioButtonBoltSet.Checked = (oRow["value"].ToString() == "BoltSet");
								radioButtonBoltBm.Checked = (oRow["value"].ToString() == "BoltBM");
							}
							else if ("Enable Hold Item Check" == sKey)
							{
								radCheckBoxEnableHoldItemCheck.Checked = (true.ToString() == oRow["value"].ToString());
							}
							else if ("Delimiter for Insulation" == sKey)
							{
								radTextBoxControlDelimiterForInsulation.Text = oRow["value"].ToString();
							}
							else if ("Deleted Item Message" == sKey)
							{
								radTextBoxControlDeletedItemMessage.Text = oRow["value"].ToString();
							}
							else if ("Write latest desc to remark column" == sKey)
							{
								radCheckBoxWriteLatestDescToRemarkColumn.Checked = (true.ToString() == oRow["value"].ToString());
							}
						}
					}

					dsParam.Tables["Params"].Rows.Clear();
					{
						System.Data.DataRow drParam = dsParam.Tables["Params"].NewRow();
						drParam["Section"] = "Iso Edit Setting";
						drParam["Key"] = string.Empty;
						dsParam.Tables["Params"].Rows.Add(drParam);
					}
					using (DataSet ds = svcProxy.ExecuteCommand("IDCS.BSL","GetConfigCommand",dsParam))
					{
						foreach (DataRow oRow in ds.Tables[0].Rows)
						{
							if ("NO POS" == oRow["key"].ToString())
							{
								radGridViewBOMHeaderTextLoc.Rows[0].Cells["No"].Value = oRow["value"].ToString();
							}
							else if ("MATL DESC POS" == oRow["key"].ToString())
							{
								radGridViewBOMHeaderTextLoc.Rows[0].Cells["MATL_DESC"].Value = oRow["value"].ToString();
							}
							else if ("SIZE POS" == oRow["key"].ToString())
							{
								radGridViewBOMHeaderTextLoc.Rows[0].Cells["SIZE"].Value = oRow["value"].ToString();
							}
							else if ("MATL SPEC POS" == oRow["key"].ToString())
							{
								radGridViewBOMHeaderTextLoc.Rows[0].Cells["MATL_SPEC"].Value = oRow["value"].ToString();
							}
							else if ("SYM POS" == oRow["key"].ToString())
							{
								radGridViewBOMHeaderTextLoc.Rows[0].Cells["SYM"].Value = oRow["value"].ToString();
							}
							else if ("MATL CODE POS" == oRow["key"].ToString())
							{
								radGridViewBOMHeaderTextLoc.Rows[0].Cells["MATL_CODE"].Value = oRow["value"].ToString();
							}
							else if ("BOLT LEN POS" == oRow["key"].ToString())
							{
								radGridViewBOMHeaderTextLoc.Rows[0].Cells["BOLT_LEN"].Value = oRow["value"].ToString();
							}
							else if ("QTY POS" == oRow["key"].ToString())
							{
								radGridViewBOMHeaderTextLoc.Rows[0].Cells["QTY"].Value = oRow["value"].ToString();
							}
						}
						this.radGridViewBOMHeaderTextLoc.BestFitColumns();
					}
				}

				/// display excel template file path
				foreach (GridViewRowInfo oGridRow in this.radGridViewDrawingReport.Rows)
				{
					object oValue = this.radGridViewDrawingReport.Rows[oGridRow.Index].Cells["Name"].Value;
					string sValue = (null != oValue) ? oValue.ToString() : string.Empty;

					FileDTO oFileDTO = null;
					using (DataSet dsParam = svcProxy.GetCommandParam("IDCS.BSL", "DownloadExcelTemplateCommand"))
					{
						DataRow drParam = dsParam.Tables["Params"].NewRow();
						drParam["Key"] = sValue;
						dsParam.Tables["Params"].Rows.Add(drParam);

						using (DataSet ds = svcProxy.ExecuteCommand("IDCS.BSL", "DownloadExcelTemplateCommand", dsParam))
						{
							if (1 == ds.Tables["FileDTO"].Rows.Count)
							{
								oFileDTO = new FileDTO()
								{
									FilePath = ds.Tables["FileDTO"].Rows[0]["FilePath"].ToString(),
									FileContent = (byte[])(ds.Tables["FileDTO"].Rows[0]["FileContent"])
								};
							}
						}
					}
					if ((null != oFileDTO) && !string.IsNullOrEmpty(oFileDTO.FilePath))
					{
						this.radGridViewDrawingReport.Rows[oGridRow.Index].Cells["File"].Value = oFileDTO.FilePath;
					}
				}
				this.radGridViewDrawingReport.BestFitColumns();
				/// up to here
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
		/// save setting data
		/// </summary>
		/// <author>humkyung</author>
		/// <date>2016.09.01</date>
		public override void SaveData()
		{
			base.SaveData();

			if (string.IsNullOrEmpty(UserInfo.ProjectCode)) return;

			using (IIDCSService svcProxy = ServiceFactory.CreateServiceChannel<IIDCSService>(AppDocData.ServiceURL))
			{
				try
				{
					string sValue = string.Empty;
					using (DataSet dsParam = svcProxy.GetCommandParam("IDCS.BSL", "SaveConfigsCommand"))
					{
						{
							DataRow drParam = dsParam.Tables["Params"].NewRow();
							drParam["Section"] = "Prj Setting";
							drParam["Key"] = "Bolt Set";
							drParam["Value"] = radioButtonBoltSet.Checked ? "BoltSet" : "BoltBM";
							dsParam.Tables["Params"].Rows.Add(drParam);
						}
						{
							DataRow drParam = dsParam.Tables["Params"].NewRow();
							drParam["Section"] = "Prj Setting";
							drParam["Key"] = "Size Unit";
							drParam["Value"] = radDropDownListSizeUnit.Text;
							dsParam.Tables["Params"].Rows.Add(drParam);
						}
						{
							DataRow drParam = dsParam.Tables["Params"].NewRow();
							drParam["Section"] = "Prj Setting";
							drParam["Key"] = "Bolt Size Unit";
							drParam["Value"] = radDropDownListBoltSizeUnit.Text;
							dsParam.Tables["Params"].Rows.Add(drParam);
						}
						{
							DataRow drParam = dsParam.Tables["Params"].NewRow();
							drParam["Section"] = "Prj Setting";
							drParam["Key"] = "Enable Hold Item Check";
							drParam["Value"] = radCheckBoxEnableHoldItemCheck.Checked.ToString();
							dsParam.Tables["Params"].Rows.Add(drParam);
						}
						{
							DataRow drParam = dsParam.Tables["Params"].NewRow();
							drParam["Section"] = "Prj Setting";
							drParam["Key"] = "Delimiter for Insulation";
							drParam["Value"] = radTextBoxControlDelimiterForInsulation.Text;
							dsParam.Tables["Params"].Rows.Add(drParam);
						}
						{
							DataRow drParam = dsParam.Tables["Params"].NewRow();
							drParam["Section"] = "Prj Setting";
							drParam["Key"] = "Deleted Item Message";
							drParam["Value"] = radTextBoxControlDeletedItemMessage.Text;
							dsParam.Tables["Params"].Rows.Add(drParam);
						}
						{
							DataRow drParam = dsParam.Tables["Params"].NewRow();
							drParam["Section"] = "Prj Setting";
							drParam["Key"] = "Write latest desc to remark column";
							drParam["Value"] = radCheckBoxWriteLatestDescToRemarkColumn.Checked.ToString();
							dsParam.Tables["Params"].Rows.Add(drParam);
						}

						{
							DataRow drParam = dsParam.Tables["Params"].NewRow();
							drParam["Section"] = "Iso Edit Setting";
							drParam["Key"] = "NO POS";
							drParam["Value"] = (null != radGridViewBOMHeaderTextLoc.Rows[0].Cells["No"].Value ? radGridViewBOMHeaderTextLoc.Rows[0].Cells["No"].Value.ToString() : string.Empty);
							dsParam.Tables["Params"].Rows.Add(drParam);
						}
						{
							DataRow drParam = dsParam.Tables["Params"].NewRow();
							drParam["Section"] = "Iso Edit Setting";
							drParam["Key"] = "MATL DESC POS";
							drParam["Value"] = (null != radGridViewBOMHeaderTextLoc.Rows[0].Cells["MATL_DESC"].Value ? radGridViewBOMHeaderTextLoc.Rows[0].Cells["MATL_DESC"].Value.ToString() : string.Empty);
							dsParam.Tables["Params"].Rows.Add(drParam);
						}
						{
							DataRow drParam = dsParam.Tables["Params"].NewRow();
							drParam["Section"] = "Iso Edit Setting";
							drParam["Key"] = "SIZE POS";
							drParam["Value"] = (null != radGridViewBOMHeaderTextLoc.Rows[0].Cells["SIZE"].Value ? radGridViewBOMHeaderTextLoc.Rows[0].Cells["SIZE"].Value.ToString() : string.Empty);
							dsParam.Tables["Params"].Rows.Add(drParam);
						}
						{
							DataRow drParam = dsParam.Tables["Params"].NewRow();
							drParam["Section"] = "Iso Edit Setting";
							drParam["Key"] = "MATL SPEC POS";
							drParam["Value"] = (null != radGridViewBOMHeaderTextLoc.Rows[0].Cells["MATL_SPEC"].Value ? radGridViewBOMHeaderTextLoc.Rows[0].Cells["MATL_SPEC"].Value.ToString() : string.Empty);
							dsParam.Tables["Params"].Rows.Add(drParam);
						}
						{
							DataRow drParam = dsParam.Tables["Params"].NewRow();
							drParam["Section"] = "Iso Edit Setting";
							drParam["Key"] = "SYM POS";
							drParam["Value"] = (null != radGridViewBOMHeaderTextLoc.Rows[0].Cells["SYM"].Value ? radGridViewBOMHeaderTextLoc.Rows[0].Cells["SYM"].Value.ToString() : string.Empty);
							dsParam.Tables["Params"].Rows.Add(drParam);
						}
						{
							DataRow drParam = dsParam.Tables["Params"].NewRow();
							drParam["Section"] = "Iso Edit Setting";
							drParam["Key"] = "MATL CODE POS";
							drParam["Value"] = (null != radGridViewBOMHeaderTextLoc.Rows[0].Cells["MATL_CODE"].Value ? radGridViewBOMHeaderTextLoc.Rows[0].Cells["MATL_CODE"].Value.ToString() : string.Empty);
							dsParam.Tables["Params"].Rows.Add(drParam);
						}
						{
							DataRow drParam = dsParam.Tables["Params"].NewRow();
							drParam["Section"] = "Iso Edit Setting";
							drParam["Key"] = "BOLT LEN POS";
							drParam["Value"] = (null != radGridViewBOMHeaderTextLoc.Rows[0].Cells["BOLT_LEN"].Value ? radGridViewBOMHeaderTextLoc.Rows[0].Cells["BOLT_LEN"].Value.ToString() : string.Empty);
							dsParam.Tables["Params"].Rows.Add(drParam);
						}
						{
							DataRow drParam = dsParam.Tables["Params"].NewRow();
							drParam["Section"] = "Iso Edit Setting";
							drParam["Key"] = "QTY POS";
							drParam["Value"] = (null != radGridViewBOMHeaderTextLoc.Rows[0].Cells["QTY"].Value ? radGridViewBOMHeaderTextLoc.Rows[0].Cells["QTY"].Value.ToString() : string.Empty);
							dsParam.Tables["Params"].Rows.Add(drParam);
						}

						svcProxy.ExecuteCommand("IDCS.BSL", "SaveConfigsCommand", dsParam);
					}

					/// upload excel template for drawing report
					foreach (GridViewRowInfo oGridRow in this.radGridViewDrawingReport.Rows)
					{
						object oValue = this.radGridViewDrawingReport.Rows[oGridRow.Index].Cells["File"].Value;
						sValue = (null != oValue) ? oValue.ToString() : string.Empty;
						//if ((null != radBrowseEditorExcelTemplate.Value) && System.IO.File.Exists(radBrowseEditorExcelTemplate.Value))
						if (File.Exists(sValue))
						{
							/// open file with share mode to avoid share violation when file is open
							using (FileStream fs = File.Open(sValue, FileMode.Open, FileAccess.Read, FileShare.ReadWrite))
							{
								int iBytesToRead = Convert.ToInt32(fs.Length);
								byte[] oFileBytes = new byte[(iBytesToRead)];
								fs.Read(oFileBytes, 0, iBytesToRead);

								FileDTO oFileDTO = new FileDTO()
								{
									ComputerName = Environment.MachineName,
									FilePath = sValue,
									FileName = Path.GetFileName(sValue),
									FileExt = Path.GetExtension(sValue),
									FileContent = oFileBytes,
									FileLength = oFileBytes.Length,
									FileLastWriteTime = File.GetLastWriteTime(sValue)
								};

								oValue = this.radGridViewDrawingReport.Rows[oGridRow.Index].Cells["Name"].Value;
								sValue = (null != oValue) ? oValue.ToString() : string.Empty;
								using (DataSet dsParam = svcProxy.GetCommandParam("IDCS.BSL", "UploadExcelTemplateCommand"))
								{
									{
										DataRow drParam = dsParam.Tables["Key"].NewRow();
										drParam["Key"] = sValue;
										dsParam.Tables["Key"].Rows.Add(drParam);
									}
									{
										DataRow drParam = dsParam.Tables["Params"].NewRow();
										drParam["ComputerName"] = Environment.MachineName;
										drParam["FilePath"] = oFileDTO.FilePath;
										drParam["FileName"] = oFileDTO.FileName;
										drParam["FileExt"] = oFileDTO.FileExt;
										drParam["FileContent"] = oFileDTO.FileContent;
										drParam["FileLength"] = oFileDTO.FileLength;
										drParam["FileLastWriteTime"] = oFileDTO.FileLastWriteTime;
										dsParam.Tables["Params"].Rows.Add(drParam);
									}
									svcProxy.ExecuteCommand("IDCS.BSL", "UploadExcelTemplateCommand", dsParam);
								}
							}
						}
					}
					/// up to here
				}
				catch (Exception ex)
				{
					TechSun.Theme.frmWarning frm = new TechSun.Theme.frmWarning(ex.Message);
					frm.ShowDialog(this);
				}
			}
		}
		/// <summary>
		/// save settings to database
		/// </summary>
		/// <author>humkyung</author>
		/// <param name="sender"></param>
		/// <param name="e"></param>
		private void radButtonSave_Click(object sender, EventArgs e)
		{
			SaveData();
		}

		/// <summary>
		/// update project setting when user select a project
		/// </summary>
		/// <author>humkyung</author>
		/// <date>2016.07.13</date>
		/// <param name="sender"></param>
		/// <param name="e"></param>
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
				if ((oCtrl == radDropDownListProject) || (oCtrl == radDropDownListSite)) continue;
				oCtrl.Enabled = bEnabled;
			}
		}
	}
}
