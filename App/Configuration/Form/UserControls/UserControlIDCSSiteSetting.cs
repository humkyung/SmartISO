using System;
using System.Collections.Generic;
using System.Linq;
using System.Windows.Forms;
using Configuration.Form.RadForm;
using Configuration.Properties;
using System.Data;
using System.Xml;

using IDCS.DataTransferObjects;
using IDCS.Framework.Service;
using IDCS.Service.Interface;
using Telerik.WinControls.UI;

namespace Configuration
{
	public partial class UserControlIDCSSiteSetting : ThemeChangeable
	{
		public const string DisplayName = "Site";

		private List<SiteDTO> _SiteList = null;
		private Dictionary<SiteDTO,List<string>> _WBSColl = new Dictionary<SiteDTO,List<string>>();

		public class EmailSetting
		{
			public string ID { get; set; }
			public string Name { get; set; }
			public string Address { get; set; }
		}

		UserControlIDCSEmailSetting _oToEmailSetting = null;
		UserControlIDCSEmailSetting _oCCEmailSetting = null;

		public IEnumerable<int> CheckedSiteList
		{
			get
			{
				var rows = (from GridViewRowInfo row in radGridViewSiteList.Rows
							where ((null != row.Cells["Check"].Value) && (true.ToString() == row.Cells["Check"].Value.ToString()))
							select row.Index);
				return rows;
			}
		}

		public UserControlIDCSSiteSetting()
		{
			InitializeComponent();
			{
				SplitPanel oToPanel = new SplitPanel();
				{
					this._oToEmailSetting = new UserControlIDCSEmailSetting("To");
					this._oToEmailSetting.Dock = DockStyle.Fill;
					oToPanel.Controls.Add(this._oToEmailSetting);
				}
				SplitPanel oCCPanel = new SplitPanel();
				{
					this._oCCEmailSetting = new UserControlIDCSEmailSetting("CC");
					this._oCCEmailSetting.Dock = DockStyle.Fill;
					oCCPanel.Controls.Add(this._oCCEmailSetting);
				}
				this.radSplitContainerEmailSetting.Controls.Add(oToPanel);
				this.radSplitContainerEmailSetting.Controls.Add(oCCPanel);
			}

			Program.OnIdleUpdate += new Program.IdleUpdateEvent(OnIdleUpdate);
			ApplyTheme(this.Controls);
		}

		private void UserControlIDCSSiteSetting_Load(object sender, EventArgs e)
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
									Enabled = (true.ToString() == oRow["SP3DFlag"].ToString()),
									Tool = oRow["Type3D"].ToString()
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

				if(!string.IsNullOrEmpty(UserInfo.ProjectCode))
				{
					RadListDataItem oItem = radDropDownListProject.Items.First(param => (param.Tag as ProjectDTO).Id.ToString() == UserInfo.ProjectCode);
					if (null != oItem) radDropDownListProject.SelectedItem = oItem;
				}
			}
		}

		/// <summary>
		/// load and display site setting
		/// </summary>
		/// <author>humkyung</author>
		/// <date>2016.08.11</date>
		public override void LoadData()
		{
			if (string.IsNullOrEmpty(UserInfo.ProjectCode)) return;
			
			using (IIDCSService svcProxy = ServiceFactory.CreateServiceChannel<IIDCSService>(AppDocData.ServiceURL))
			{
				radGridViewSiteList.Rows.Clear();

				this._SiteList = new List<SiteDTO>();
				if (null != this._SiteList) this._SiteList.Clear();
				using (DataSet dsParam = svcProxy.GetCommandParam("IDCS.BSL", "GetSiteListCommand"))
				{
					List<SiteDTO> oSiteDTOColl = new List<SiteDTO>();
					using (DataSet dsSite = svcProxy.ExecuteCommand("IDCS.BSL", "GetSiteListCommand", dsParam))
					{
						foreach (DataRow oRow in dsSite.Tables[0].Rows)
						{
							this._SiteList.Add(
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

				radGridViewSiteList.Rows.Clear();
				radGridViewSiteList.RowCount = this._SiteList.Count;

				int iRow = 0;
				foreach (SiteDTO dto in this._SiteList)
				{
					radGridViewSiteList.Rows[iRow].Tag = dto;
					radGridViewSiteList.Rows[iRow].Cells["Name"].Value = dto.Name;
					radGridViewSiteList.Rows[iRow].Cells["Desc"].Value = dto.Desc;
					radGridViewSiteList.Rows[iRow].Cells["CopySetting"].Value = "Copy Setting";
					iRow++;
				}
				radGridViewSiteList.BestFitColumns();

				/// get wbs filter for project - 2016.10.06 added by humkyung
				using (DataSet dsParam = svcProxy.GetCommandParam("SP3D.BSL", "GetWBSFilterCommand"))
				{
					DataSet dsWBS = svcProxy.ExecuteCommand("SP3D.BSL", "GetWBSFilterCommand",dsParam);
					if(null != dsWBS) this.radGridViewWBS.DataSource = dsWBS.Tables[0];
				}
				this.radGridViewWBS.BestFitColumns();
				this.radGridViewWBS.Tag = null;
				this._WBSColl.Clear();
				/// up to here
			}
		}

		/// <summary>
		/// load WBS Filter for Site
		/// </summary>
		/// <author>humkyung</author>
		/// <date>2016.10.06</date>
		/// <param name="oSiteDTO"></param>
		private void LoadSiteData(SiteDTO oSiteDTO)
		{
			if (null != oSiteDTO)
			{
				int iSiteID = Convert.ToInt32(UserInfo.SiteID);
				UserInfo.SiteID = oSiteDTO.Id.ToString();
				try
				{
					if (!this._WBSColl.ContainsKey(oSiteDTO))
					{
						this._WBSColl.Add(oSiteDTO, new List<string>());

						using (IIDCSService svcProxy = ServiceFactory.CreateServiceChannel<IIDCSService>(AppDocData.ServiceURL))
						{
							using (System.Data.DataSet dsParam = svcProxy.GetCommandParam("IDCS.BSL", "GetConfigCommand"))
							{
								System.Data.DataRow drParam = dsParam.Tables["Params"].NewRow();
								drParam["Section"] = "WBS Filters";
								drParam["Key"] = oSiteDTO.Id.ToString();
								dsParam.Tables["Params"].Rows.Add(drParam);

								using (DataSet ds = svcProxy.ExecuteCommand("IDCS.BSL","GetConfigCommand",dsParam))
								{
									if (1 == ds.Tables[0].Rows.Count)
									{
										string sValue = ds.Tables[0].Rows[0]["Value"].ToString();
										XmlDocument oXDoc = new XmlDocument();
										oXDoc.LoadXml(sValue);
										XmlElement XmlRoot = oXDoc["WBS_Filter"];
										foreach (XmlNode XmlDefinition in XmlRoot.ChildNodes)
										{
											string sName = XmlDefinition.InnerText;
											this._WBSColl[oSiteDTO].Add(sName);
										}
									}
								}
							}
						}
					}

					/// uncheck all items
					foreach (GridViewRowInfo oGridRow in this.radGridViewWBS.Rows)
					{
						this.radGridViewWBS.Rows[oGridRow.Index].Cells["Check"].Value = false;
					}
					/// up to here
					foreach (string sWBS in this._WBSColl[oSiteDTO])
					{
						try
						{
							var rows = from GridViewRowInfo row in this.radGridViewWBS.Rows
									   where (row.Cells["WBS"].Value.ToString() == sWBS)
									   select row;
							foreach (var row in rows)
							{
								this.radGridViewWBS.Rows[(row as GridViewRowInfo).Index].Cells["Check"].Value = true;
							}
						}
						catch { }
					}
					this.radGridViewWBS.Tag = oSiteDTO;
				}
				finally
				{
					UserInfo.SiteID = iSiteID.ToString();
				}
			}
			else
			{
				this.radGridViewWBS.Tag = null;
			}
		}

		/// <summary>
		/// load email setting of given site
		/// </summary>
		/// <author>humkyung</author>
		/// <date>2016.09.06</date>
		/// <param name="iSiteID"></param>
		private void LoadEmailSetting(int iSiteID)
		{
			this._oToEmailSetting.LoadData(iSiteID);
			this._oCCEmailSetting.LoadData(iSiteID);
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

			try
			{
				using (IIDCSService svcProxy = ServiceFactory.CreateServiceChannel<IIDCSService>(AppDocData.ServiceURL))
				{
					using (DataSet dsParam = svcProxy.GetCommandParam("IDCS.BSL", "SaveSiteListCommand"))
					{
						foreach (SiteDTO oSiteDTO in this._SiteList)
						{
							DataRow drParam = dsParam.Tables["Params"].NewRow();
							drParam["SiteID"] = oSiteDTO.Id;
							drParam["Name"] = oSiteDTO.Name;
							drParam["Desc"] = oSiteDTO.Desc;
							drParam["DeleteYN"] = oSiteDTO.DeleteYN;
							dsParam.Tables["Params"].Rows.Add(drParam);
						}
						svcProxy.ExecuteCommand("IDCS.BSL","SaveSiteListCommand",dsParam);
					}

					/// save wbs filter for site
					if (!string.IsNullOrEmpty(UserInfo.SiteID))
					{
						int iSiteID = Convert.ToInt32(UserInfo.SiteID);
						try
						{
							foreach (KeyValuePair<SiteDTO, List<string>> pair in this._WBSColl)
							{
								List<ProjectConfigDTO> configs = new List<ProjectConfigDTO>();

								if (-1 == pair.Key.Id) continue;

								UserInfo.SiteID = pair.Key.Id.ToString();
								using (DataSet dsParam = svcProxy.GetCommandParam("IDCS.BSL", "SaveConfigsCommand"))
								{
									XmlDocument xDoc = new XmlDocument();
									XmlElement xmlRoot = xDoc.CreateElement("WBS_Filter");
									xDoc.AppendChild(xmlRoot);
									foreach (string sWBS in pair.Value)
									{
										XmlElement xml = xDoc.CreateElement("Name");
										xml.InnerText = sWBS;
										xmlRoot.AppendChild(xml);
									}

									{
										DataRow drParm = dsParam.Tables["Params"].NewRow();
										drParm["Section"] = "WBS Filters";
										drParm["Key"] = pair.Key.Id.ToString();
										drParm["Value"] = xDoc.InnerXml;
										dsParam.Tables["Params"].Rows.Add(drParm);
									}
									svcProxy.ExecuteCommand("IDCS.BSL", "SaveConfigsCommand", dsParam);
								}
							}
							/// up to here
						}
						catch (Exception ex)
						{
							new TechSun.Theme.frmWarning(ex.Message).ShowDialog();
						}
						finally
						{
							UserInfo.SiteID = iSiteID.ToString();
						}
					}
				}

				this._oToEmailSetting.SaveData();
				this._oCCEmailSetting.SaveData();
			}
			catch(Exception ex)
			{
				new TechSun.Theme.frmWarning(ex.Message).ShowDialog();
			}
			finally
			{
				LoadData();
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
		/// update site setting when user select a project
		/// </summary>
		/// <author>humkyung</author>
		/// <date>2016.08.11</date>
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
					UserInfo.Tool = oPrjDTO.Tool.ToString();
					LoadData();
				}
			}
		}

		/// <summary>
		/// copy setting from selected site
		/// </summary>
		/// <author>humkyung</author>
		/// <date>2016.08.25</date>
		/// <param name="sender"></param>
		/// <param name="e"></param>
		private void radGridViewSiteList_CommandCellClick(object sender, EventArgs e)
		{
			GridCommandCellElement oCell = (GridCommandCellElement)sender;
			if (oCell.RowIndex > -1)
			{
				ProjectDTO oPrjDTO = radDropDownListProject.SelectedItem.Tag as ProjectDTO;
				SiteDTO oSiteDTO = radGridViewSiteList.Rows[oCell.RowIndex].Tag as SiteDTO;
				try
				{
                    RadFormIDCSCopySetting frm = new RadFormIDCSCopySetting(EDCS.WF.BSL.Variable.SharedVariable.IsIE, oPrjDTO, oSiteDTO);
					frm.ShowDialog(this);
				}
				catch (Exception ex)
				{
					MessageBox.Show(ex.Message, "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
				}
				finally
				{
					UserInfo.ProjectCode = oPrjDTO.Id.ToString();
				}
			}
		}

		/// <summary>
		/// add a new site
		/// </summary>
		/// <author>humkyung</author>
		/// <date>2016.08.30</date>
		/// <param name="sender"></param>
		/// <param name="e"></param>
		private void radButtonAdd_Click(object sender, EventArgs e)
		{
			try
			{
				GridViewRowInfo oGridRow = this.radGridViewSiteList.Rows.AddNew();
				this.radGridViewSiteList.Rows[oGridRow.Index].Cells["CopySetting"].Value = "Copy Setting";
				{
					SiteDTO dto = new SiteDTO() { Id=-1,DeleteYN = false };
					oGridRow.Tag = dto;
					this._SiteList.Add(dto);
				}

				this.radGridViewSiteList_SelectionChanged(null, null);
			}
			catch { }
		}

		/// <summary>
		/// delete a new site
		/// </summary>
		/// <author>humkyung</author>
		/// <date>2016.08.30</date>
		/// <param name="sender"></param>
		/// <param name="e"></param>
		private void radButtonDelete_Click(object sender, EventArgs e)
		{
			if (this.CheckedSiteList.ToList().Count > 0)
			{
				List<int> oCheckedItemList = this.CheckedSiteList.ToList();
				oCheckedItemList.Reverse();
				foreach (int iRowIndex in oCheckedItemList)
				{
					SiteDTO dto = radGridViewSiteList.Rows[iRowIndex].Tag as SiteDTO;
					if (null != dto) dto.DeleteYN = true;
					radGridViewSiteList.Rows.RemoveAt(iRowIndex);
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
			bool bEnabled = (!string.IsNullOrEmpty(UserInfo.ProjectCode));
			foreach (Control oCtrl in this.Controls)
			{
				if ((oCtrl is RadLabel) || (oCtrl is RadGridView)) continue;
				if (oCtrl == this.radDropDownListProject) continue;
				if (oCtrl == this.radGroupBoxSite)
				{
					this.radGroupBoxSite.Enabled = bEnabled;
				}
				else if(oCtrl is RadGroupBox)
				{
					SiteDTO oSiteDTO = null;
					if(this.radGridViewSiteList.SelectedRows.Count > 0) 
						oSiteDTO = this.radGridViewSiteList.SelectedRows[0].Tag as SiteDTO;
					if (UserInfo.Tool == "PDMS" && oCtrl.Text == "WBS")
					{
						oCtrl.Enabled = false;
						continue;
					}
					oCtrl.Enabled = bEnabled && ((null != oSiteDTO) && (-1 != oSiteDTO.Id));
				}
			}
			this.kRadFreeButtonExportDatabase.Enabled = bEnabled;
			this.kRadFreeButtonImportDatabase.Enabled = bEnabled;
		}

		/// <summary>
		/// display email setting of selected site
		/// </summary>
		/// <author>humkyung</author>
		/// <date>2016.09.06</date>
		/// <param name="sender"></param>
		/// <param name="e"></param>
		private void radGridViewSiteList_SelectionChanged(object sender, EventArgs e)
		{
			if (UserInfo.Tool == "SP3D")
			{ 
				if (1 == this.radGridViewSiteList.SelectedRows.Count)
				{
					int iSiteID = 0;
					int.TryParse(UserInfo.SiteID,out iSiteID);
					try
					{
						SiteDTO oSiteDTO = this.radGridViewSiteList.SelectedRows[0].Tag as SiteDTO;
						if ((null != oSiteDTO) && (-1 != oSiteDTO.Id))
						{
							LoadEmailSetting(oSiteDTO.Id);
						}
						else
						{
						}

						LoadSiteData(oSiteDTO);
					}
					finally
					{
						UserInfo.SiteID = iSiteID.ToString();
					}
				}
			}
		}

		/// <summary>
		/// update site information
		/// </summary>
		/// <author>humkyung</author>
		/// <date>2016.09.26</date>
		/// <param name="sender"></param>
		/// <param name="e"></param>
		private void radGridViewSiteList_CellEndEdit(object sender, GridViewCellEventArgs e)
		{
			if ((e.RowIndex < 0) || (0 == e.ColumnIndex)) return;

			SiteDTO dto = this.radGridViewSiteList.Rows[e.RowIndex].Tag as SiteDTO;
			if (null != dto)
			{
				string sColName = this.radGridViewSiteList.Columns[e.ColumnIndex].Name;
				object oValue = this.radGridViewSiteList.Rows[e.RowIndex].Cells[e.ColumnIndex].Value;
				string sValue = (null != oValue) ? oValue.ToString() : string.Empty;
				switch(sColName)
				{
					case "Name":
						dto.Name = sValue;
						break;
					case "Desc":
						dto.Desc = sValue;
						break;
				}
			}
		}

		private void radGridViewWBS_CellEndEdit(object sender, GridViewCellEventArgs e)
		{
			SiteDTO oSiteDTO = this.radGridViewWBS.Tag as SiteDTO;
			if (null != oSiteDTO)
			{
				if (!this._WBSColl.ContainsKey(oSiteDTO)) this._WBSColl[oSiteDTO] = new List<string>();
				this._WBSColl[oSiteDTO].Clear();

				foreach (GridViewRowInfo oGridRow in this.radGridViewWBS.Rows)
				{
					object oValue = this.radGridViewWBS.Rows[oGridRow.Index].Cells["Check"].Value;
					if ((null == oValue) || (false.ToString() == oValue.ToString())) continue;
					this._WBSColl[oSiteDTO].Add(this.radGridViewWBS.Rows[oGridRow.Index].Cells["WBS"].Value.ToString());
				}
			}
		}

		/// <summary>
		/// export iso drawing control database to xml file
		/// </summary>
		/// <author>humkyung</author>
		/// <date>2017.03.28</date>
		/// <param name="sender"></param>
		/// <param name="e"></param>
		private void kRadFreeButtonExportDatabase_Click(object sender, EventArgs e)
		{
			try
			{
				Cursor.Current = Cursors.WaitCursor;
				using (IIDCSService svcProxy = ServiceFactory.CreateServiceChannel<IIDCSService>(AppDocData.ServiceURL))
				{
					using (DataSet dsParam = svcProxy.GetCommandParam("IDCS.BSL", "ExportTablesCommand"))
					{
						using (DataSet res = svcProxy.ExecuteCommand("IDCS.BSL", "ExportTablesCommand", dsParam))
						{
							SaveFileDialog oFileDialog = new SaveFileDialog();
							oFileDialog.FileName = DateTime.Now.ToString("yyyyMMdd") + ".xml";
							oFileDialog.DefaultExt = "xml";
							oFileDialog.Filter = "XML(*.xml)|*.xml|All Files(*.*)|*.*";
							oFileDialog.FilterIndex = 0;
							if (System.Windows.Forms.DialogResult.OK == oFileDialog.ShowDialog())
							{
								res.WriteXmlSchema(System.IO.Path.Combine(System.IO.Path.GetDirectoryName(oFileDialog.FileName), System.IO.Path.GetFileNameWithoutExtension(oFileDialog.FileName) + ".xsd"));
								res.WriteXml(oFileDialog.FileName);
							}
						}
					}
				}
			}
			catch(Exception ex)
			{
				new TechSun.Theme.frmWarning(ex.Message);
			}
			finally
			{
				Cursor.Current = Cursors.Default;
			}
		}

		/// <summary>
		/// import iso drawing control database to xml file
		/// </summary>
		/// <author>humkyung</author>
		/// <date>2017.03.28</date>
		/// <param name="sender"></param>
		/// <param name="e"></param>
		private void kRadFreeButtonImportDatabase_Click(object sender, EventArgs e)
		{
			try
			{
				if (DialogResult.Yes == MessageBox.Show("Do you want to restore?", "Message", MessageBoxButtons.YesNo, MessageBoxIcon.Question))
				{
					OpenFileDialog oFileDialog = new OpenFileDialog();
					oFileDialog.Filter = "XML(*.xml)|*.xml|All Files(*.*)|*.*";
					oFileDialog.FilterIndex = 0;
					if (System.Windows.Forms.DialogResult.OK == oFileDialog.ShowDialog())
					{
						TechSun.Theme.frmSplashScreen frmPopupWaitingBarCallMethod = new TechSun.Theme.frmSplashScreen(1, new List<string>(){"Doing restore..."});
						try
						{
							Cursor.Current = Cursors.WaitCursor;
							frmPopupWaitingBarCallMethod.MethodSetStart();

							using (DataSet dsParam = new DataSet())
							{
								dsParam.ReadXmlSchema(System.IO.Path.Combine(System.IO.Path.GetDirectoryName(oFileDialog.FileName), System.IO.Path.GetFileNameWithoutExtension(oFileDialog.FileName) + ".xsd"));
								dsParam.ReadXml(oFileDialog.FileName);

								frmPopupWaitingBarCallMethod.MethodStatusUpdateText("Doing restore...");
								Application.DoEvents();

								using (IIDCSService svcProxy = ServiceFactory.CreateServiceChannel<IIDCSService>(AppDocData.ServiceURL))
								{
									svcProxy.ExecuteCommand("IDCS.BSL", "ImportTablesCommand", dsParam);
								}
							}
						}
						catch(Exception ex)
						{
							new TechSun.Theme.frmWarning(ex.Message);
						}
						finally
						{
							frmPopupWaitingBarCallMethod.MethodClose();
							Cursor.Current = Cursors.Default;
						}
					}
				}
			}
			finally
			{
			}
		}
	}
}
