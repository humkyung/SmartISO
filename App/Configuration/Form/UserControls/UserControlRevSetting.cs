using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Linq;
using System.Text;
using System.Windows.Forms;

using IDCS.DataTransferObjects;
using IDCS.Service.Interface;
using IDCS.Framework.Service;

namespace Admin.WF
{
	public partial class UserControlRevSetting : ThemeChangeable
	{
		public const string DisplayName = "Revision Setting";

		public UserControlRevSetting()
		{
			InitializeComponent();
			ApplyTheme(this.Controls);
		}

		private void UserControlRevSetting_Load(object sender, EventArgs e)
		{
			try
			{
				string sEndPointAddress = Properties.Settings.Default.WebServiceURL + "ProjectConfigService.svc";
				using (IProjectConfigService svcProxy = ServiceFactory.CreateServiceChannel<IProjectConfigService>(sEndPointAddress))
				{
					List<ProjectDTO> oProjectList = svcProxy.GetAllProjectList();
					foreach (ProjectDTO oPrjDTO in oProjectList)
					{
						Telerik.WinControls.UI.RadListDataItem oItem = new Telerik.WinControls.UI.RadListDataItem();
						{
							oItem.Text = string.Format("{0}({1})", oPrjDTO.Name, oPrjDTO.Desc);
							oItem.Tag = oPrjDTO;
						}
						radDropDownListProject.Items.Add(oItem);
					}
				}
			}
			catch (Exception ex)
			{
				MessageBox.Show(ex.Message, "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
			}
		}

		/// <summary>
		/// load setting from database
		/// </summary>
		/// <author>humkyung</author>
		public override void LoadData()
		{
			if (string.IsNullOrEmpty(UserInfo.ProjectCode)) return;

			try
			{
				radGridViewTextInfo.Rows.Clear();

				radGridViewTextInfo.RowCount = 1;
				Telerik.WinControls.UI.GridViewComboBoxColumn oComboBoxCol = radGridViewTextInfo.Columns["Justify"] as Telerik.WinControls.UI.GridViewComboBoxColumn;
				if (null != oComboBoxCol)
				{
					oComboBoxCol.DataSource = new List<string>() { "Left", "Right", "Center" };
				}
				oComboBoxCol = radGridViewTextInfo.Columns["TextColor"] as Telerik.WinControls.UI.GridViewComboBoxColumn;
				if (null != oComboBoxCol)
				{
					oComboBoxCol.DataSource = new List<string>() { "Red", "Yellow", "Blue" };
				}

				radGridViewRevDataPos.Rows.Clear();
				radGridViewRevDataPos.AllowAutoSizeColumns = true;

				radGridViewRevDataPos.RowCount = 6;
				radGridViewRevDataPos.Rows[0].Cells[0].Value = "Rev No";
				radGridViewRevDataPos.Rows[0].Cells[0].ReadOnly = true;
				radGridViewRevDataPos.Rows[1].Cells[0].Value = "Date";
				radGridViewRevDataPos.Rows[1].Cells[0].ReadOnly = true;
				radGridViewRevDataPos.Rows[2].Cells[0].Value = "Rev Desc";
				radGridViewRevDataPos.Rows[2].Cells[0].ReadOnly = true;
				radGridViewRevDataPos.Rows[3].Cells[0].Value = "Drawn By";
				radGridViewRevDataPos.Rows[3].Cells[0].ReadOnly = true;
				radGridViewRevDataPos.Rows[4].Cells[0].Value = "Checked By";
				radGridViewRevDataPos.Rows[4].Cells[0].ReadOnly = true;
				radGridViewRevDataPos.Rows[5].Cells[0].Value = "Approved";
				radGridViewRevDataPos.Rows[5].Cells[0].ReadOnly = true;

				#region clear controls value
				radTextBoxDisplayCount.Text = string.Empty;
				radTextBoxDistance.Text = string.Empty;
				radDropDownListSorting.SelectedIndex = 0;
				#endregion
				string sEndPointAddress = Properties.Settings.Default.WebServiceURL + "ProjectConfigService.svc";
				using (IProjectConfigService svcProxy = ServiceFactory.CreateServiceChannel<IProjectConfigService>(sEndPointAddress))
				{
					#region Revision Data Setting
					using (DataSet ds = svcProxy.GetConfig("Revision Data Setting", string.Empty))
					{
						foreach (DataRow oRow in ds.Tables[0].Rows)
						{
							object oValue = oRow["key"];
							string sKey = (null != oValue) ? oValue.ToString() : string.Empty;
							if ("Display Count" == sKey)
							{
								radTextBoxDisplayCount.Text = oRow["value"].ToString();
							}
							else if ("Dist" == sKey)
							{
								radTextBoxDistance.Text = oRow["value"].ToString();
							}
							else if ("Sorting" == sKey)
							{
								int at = radDropDownListSorting.FindString(oRow["value"].ToString());
								if (-1 != at) radDropDownListSorting.SelectedIndex = at;
								else radDropDownListSorting.SelectedIndex = 0;
							}
							else if ("Layer" == sKey)
							{
								radGridViewTextInfo.Rows[0].Cells[0].Value = oRow["value"].ToString();
							}
							else if ("TextStyle" == sKey)
							{
								radGridViewTextInfo.Rows[0].Cells[1].Value = oRow["value"].ToString();
							}
							else if ("TextHeight" == sKey)
							{
								radGridViewTextInfo.Rows[0].Cells[2].Value = oRow["value"].ToString();
							}
							else if ("Justify" == sKey)
							{
								radGridViewTextInfo.Rows[0].Cells[3].Value = oRow["value"].ToString();
							}
							else if ("Rotation" == sKey)
							{
								radGridViewTextInfo.Rows[0].Cells[4].Value = oRow["value"].ToString();
							}
							else if ("TextColor" == sKey)
							{
								radGridViewTextInfo.Rows[0].Cells[5].Value = oRow["value"].ToString();
							}
						}
					}
					#endregion

					#region clear controls value
					foreach (Telerik.WinControls.UI.GridViewRowInfo oGridRow in radGridViewRevDataPos.Rows)
					{
						radGridViewRevDataPos.Rows[oGridRow.Index].Cells["PosX"].Value = string.Empty;
						radGridViewRevDataPos.Rows[oGridRow.Index].Cells["PosY"].Value = string.Empty;
					}
					#endregion
					using (DataSet ds = svcProxy.GetConfig("Revision Pos Setting", string.Empty))
					{
						foreach (Telerik.WinControls.UI.GridViewRowInfo oGridRow in radGridViewRevDataPos.Rows)
						{
							string sKey = radGridViewRevDataPos.Rows[oGridRow.Index].Cells["Item"].Value.ToString();
							DataRow[] oRows = ds.Tables[0].Select(string.Format("key='{0}'", sKey));
							if (1 == oRows.Length)
							{
								string[] oTokens = oRows[0]["Value"].ToString().Split(',');
								if (2 == oTokens.Length)
								{
									radGridViewRevDataPos.Rows[oGridRow.Index].Cells["PosX"].Value = oTokens[0];
									radGridViewRevDataPos.Rows[oGridRow.Index].Cells["PosY"].Value = oTokens[1];
								}
							}
						}
					}

					#region rev nos
					#region clear controls value
					foreach (Telerik.WinControls.UI.GridViewRowInfo oGridRow in radGridViewRevNo.Rows)
					{
						radGridViewRevNo.Rows[oGridRow.Index].Cells["RevNo"].Value = string.Empty;
						radGridViewRevNo.Rows[oGridRow.Index].Cells["RevDesc"].Value = string.Empty;
					}
					#endregion

					List<RevisionNoDTO> oAllRevNos = svcProxy.GetAllRevNos();
					radGridViewRevNo.RowCount = oAllRevNos.Count;

					int iRow = 0;
					foreach (RevisionNoDTO oRevNo in oAllRevNos)
					{
						radGridViewRevNo.Rows[iRow].Cells["RevNo"].Value = oRevNo.RevNo;
						radGridViewRevNo.Rows[iRow++].Cells["RevDesc"].Value = oRevNo.RevDesc;
					}
					#endregion
				}

				radGridViewTextInfo.BestFitColumns();
			}
			catch (Exception ex)
			{
				MessageBox.Show(ex.Message, "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
			}
		}

		/// <summary>
		/// save revision settings
		/// </summary>
		/// <author>humkyung</author>
		/// <param name="sender"></param>
		/// <param name="e"></param>
		private void radButtonSave_Click(object sender, EventArgs e)
		{
			if (string.IsNullOrEmpty(UserInfo.ProjectCode)) return;

			string sEndPointAddress = Properties.Settings.Default.WebServiceURL + "ProjectConfigService.svc";
			using (IProjectConfigService svcProxy = ServiceFactory.CreateServiceChannel<IProjectConfigService>(sEndPointAddress))
			{
				List<ProjectConfigDTO> configs = new List<ProjectConfigDTO>();
				try
				{
					configs.Add(new ProjectConfigDTO() { Section = "Revision Data Setting", Key = "Display Count", Value = radTextBoxDisplayCount.Text });
					configs.Add(new ProjectConfigDTO() { Section = "Revision Data Setting", Key = "Sorting", Value = radDropDownListSorting.Text });
					configs.Add(new ProjectConfigDTO() { Section = "Revision Data Setting", Key = "Dist", Value = radTextBoxDistance.Text });
					{
						object oValue = radGridViewTextInfo.Rows[0].Cells[0].Value.ToString();
						string sValue = (null != oValue) ? oValue.ToString() : string.Empty;
						configs.Add(new ProjectConfigDTO() { Section = "Revision Data Setting", Key = "Layer", Value = sValue });
						oValue = radGridViewTextInfo.Rows[0].Cells[1].Value.ToString();
						sValue = (null != oValue) ? oValue.ToString() : string.Empty;
						configs.Add(new ProjectConfigDTO() { Section = "Revision Data Setting", Key = "TextStyle", Value = sValue });
						oValue = radGridViewTextInfo.Rows[0].Cells[2].Value.ToString();
						sValue = (null != oValue) ? oValue.ToString() : string.Empty;
						configs.Add(new ProjectConfigDTO() { Section = "Revision Data Setting", Key = "TextHeight", Value = sValue });
						oValue = radGridViewTextInfo.Rows[0].Cells[3].Value.ToString();
						sValue = (null != oValue) ? oValue.ToString() : string.Empty;
						configs.Add(new ProjectConfigDTO() { Section = "Revision Data Setting", Key = "Justify", Value = sValue });
						oValue = radGridViewTextInfo.Rows[0].Cells[4].Value.ToString();
						sValue = (null != oValue) ? oValue.ToString() : string.Empty;
						configs.Add(new ProjectConfigDTO() { Section = "Revision Data Setting", Key = "Rotation", Value = sValue });
						oValue = radGridViewTextInfo.Rows[0].Cells[5].Value.ToString();
						sValue = (null != oValue) ? oValue.ToString() : string.Empty;
						configs.Add(new ProjectConfigDTO() { Section = "Revision Data Setting", Key = "TextColor", Value = sValue });
					}

					string sSection = "Revision Pos Setting";
					foreach (Telerik.WinControls.UI.GridViewRowInfo oGridRow in radGridViewRevDataPos.Rows)
					{
						string sValue = string.Empty;
						try
						{
							sValue = string.Format("{0},{1}", radGridViewRevDataPos.Rows[oGridRow.Index].Cells["PosX"].Value.ToString(), radGridViewRevDataPos.Rows[oGridRow.Index].Cells["PosY"].Value.ToString());
						}
						catch { }

						configs.Add(new ProjectConfigDTO()
						{
							Section = sSection,
							Key = radGridViewRevDataPos.Rows[oGridRow.Index].Cells["Item"].Value.ToString(),
							Value = sValue
						});
					}
					svcProxy.SaveConfigs(configs);

					List<RevisionNoDTO> oRevNoDTOList = new List<RevisionNoDTO>();
					foreach (Telerik.WinControls.UI.GridViewRowInfo oGridRow in radGridViewRevNo.Rows)
					{
						object oValue = oGridRow.Cells["RevNo"].Value;
						if ((null == oValue) || (string.IsNullOrEmpty(oValue.ToString())))
						{
							MessageBox.Show("Rev No is Null or Empty", "Message", MessageBoxButtons.OK, MessageBoxIcon.Information);
							break;
						}

						oRevNoDTOList.Add(
							new RevisionNoDTO()
							{
								RevID = oGridRow.Index,
								RevNo = oGridRow.Cells["RevNo"].Value.ToString(),
								RevDesc = (null != oGridRow.Cells["RevDesc"].Value) ? oGridRow.Cells["RevDesc"].Value.ToString() : string.Empty
							}
						);
					}
					svcProxy.SaveAllRevNos(oRevNoDTOList);

				}
				catch (Exception ex)
				{
					MessageBox.Show(ex.Message, "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
				}
			}
		}

		private void radButtonDistance_Click(object sender, EventArgs e)
		{
			//radTextBoxDistance.Text = string.Format("{0:0.000}", CADAutomation.PF.GetDistance());
		}

		/// <summary>
		/// pick a point for revision text
		/// </summary>
		/// <author>humkyung</author>
		/// <date>2016.07.12</date>
		/// <param name="sender"></param>
		/// <param name="e"></param>
		private void radGridViewRevDataPos_CommandCellClick(object sender, EventArgs e)
		{
			Telerik.WinControls.UI.GridCommandCellElement cell = (Telerik.WinControls.UI.GridCommandCellElement)sender;

			//double[] res = CADAutomation.PF.GetCADPoint();
			//if ((null != res) && (3 == res.Length))
			//{
			//    string sHeight = "1.0";
			//    if (null != cell.Value)
			//    {
			//        string[] oTokens = cell.Value.ToString().Split(",".ToCharArray());
			//        if (3 == oTokens.Length) sHeight = oTokens[2];
			//    }

			//    radGridViewRevDataPos.Rows[cell.RowIndex].Cells["PosX"].Value = string.Format("{0:0.00}", ((double[])(res))[0]);
			//    radGridViewRevDataPos.Rows[cell.RowIndex].Cells["PosY"].Value = string.Format("{0:0.00}", ((double[])(res))[1]);
			//}
		}

		private void radDropDownListProject_SelectedIndexChanged(object sender, Telerik.WinControls.UI.Data.PositionChangedEventArgs e)
		{
			if (radDropDownListProject.SelectedIndex >= 0)
			{
				ProjectDTO oPrjDTO = radDropDownListProject.Items[radDropDownListProject.SelectedIndex].Tag as ProjectDTO;
				if (null != oPrjDTO)
				{
					UserInfo.ProjectCode = oPrjDTO.Id.ToString();
					LoadData();
				}
			}
		}
	}
}
