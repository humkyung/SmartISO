using System;
using System.Collections.Generic;
using System.Data;
using System.IO;
using System.Linq;
using System.Windows.Forms;
using System.Xml;
using Configuration.Form.RadForm;
using Configuration.Properties;
using Microsoft.Win32;
using Telerik.WinControls.UI;
using SmartISO.Service.Interface;
using SmartISO.Framework.Service;

namespace Configuration
{
	public partial class UserControlRevSetting : ThemeChangeable
	{
		public const string NewOrEdit = "New or Edit...";
		public const string DisplayName = "Revision";
		private string BorderFilePath = string.Empty;

        private Int64 ProjectUID
        {
            get { return Convert.ToInt64(this.radDropDownListProject.Items[this.radDropDownListProject.SelectedIndex].Tag); }
        }
        private Int64 SiteUID
        {
            get { return Convert.ToInt64(this.radDropDownListSite.Items[this.radDropDownListSite.SelectedIndex].Tag); }
        }

		public IEnumerable<int> CheckedRevNoList
		{
			get
			{
				var rows = (from GridViewRowInfo row in radGridViewRevNo.Rows
							where ((null != row.Cells["Check"].Value) && (true.ToString() == row.Cells["Check"].Value.ToString()))
							select row.Index);
				return rows;
			}
		}
		public IEnumerable<int> CheckedIssueDateList
		{
			get
			{
				var rows = (from GridViewRowInfo row in radGridViewIssueDate.Rows
							where ((null != row.Cells["Check"].Value) && (true.ToString() == row.Cells["Check"].Value.ToString()))
							select row.Index);
				return rows;
			}
		}

		public class IssueDateDisplayFormat
		{
			public string Name { get; set; }
			public string Content { get; set; }
		};
		public List<IssueDateDisplayFormat> IssueDateDisplayFormatColl = new List<IssueDateDisplayFormat>();

		public UserControlRevSetting()
        {
            InitializeComponent();
            {
                this.radGridViewIssueDateDisplay.RowCount = 2;
                this.radGridViewIssueDateDisplay.Rows[0].Cells["DisplayName"].Value = "Drawing Report";
                this.radGridViewIssueDateDisplay.Rows[1].Cells["DisplayName"].Value = "ISO DWG";
            }

            Program.OnIdleUpdate += new Program.IdleUpdateEvent(OnIdleUpdate);
            this.Disposed += new EventHandler(UserControlRevSetting_Disposed);

            ApplyTheme(this.Controls);
        }

        void UserControlRevSetting_Disposed(object sender, EventArgs e)
        {
            Program.OnIdleUpdate -= new Program.IdleUpdateEvent(OnIdleUpdate);
        }

		private void UserControlRevSetting_Load(object sender, EventArgs e)
		{
			try
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

				LoadSiteData();
			}
			catch (Exception ex)
			{
				new TechSun.Theme.frmWarning(ex.Message).ShowDialog(this);
			}
		}

		/// <summary>
		/// load setting from database
		/// </summary>
		/// <author>humkyung</author>
		public override void LoadData()
		{
			if ((this.radDropDownListProject.SelectedIndex < 0) || (this.radDropDownListSite.SelectedIndex < 0)) return;

			try
			{
				radGridViewTextInfo.Rows.Clear();

				radGridViewTextInfo.RowCount = 1;
				GridViewComboBoxColumn oComboBoxCol = radGridViewTextInfo.Columns["Justify"] as GridViewComboBoxColumn;
				if (null != oComboBoxCol)
				{
					oComboBoxCol.DataSource = new List<string>() { "Left", "Right", "Center" };
				}
				oComboBoxCol = radGridViewTextInfo.Columns["TextColor"] as GridViewComboBoxColumn;
				if (null != oComboBoxCol)
				{
					oComboBoxCol.DataSource = new List<string>() { "Red", "Yellow", "Blue" , "White" };
				}

				radGridViewRevDataPos.Rows.Clear();
				radGridViewRevDataPos.AllowAutoSizeColumns = true;

				radGridViewRevDataPos.RowCount = 6;
				radGridViewRevDataPos.Rows[0].Cells[0].Value = "REV_NO";
				radGridViewRevDataPos.Rows[0].Cells[0].ReadOnly = true;
				radGridViewRevDataPos.Rows[1].Cells[0].Value = "REV_DATE";
				radGridViewRevDataPos.Rows[1].Cells[0].ReadOnly = true;
				radGridViewRevDataPos.Rows[2].Cells[0].Value = "REV_DESC";
				radGridViewRevDataPos.Rows[2].Cells[0].ReadOnly = true;
				radGridViewRevDataPos.Rows[3].Cells[0].Value = "REV_DRWN_BY";
				radGridViewRevDataPos.Rows[3].Cells[0].ReadOnly = true;
				radGridViewRevDataPos.Rows[4].Cells[0].Value = "REV_CHECKED_BY";
				radGridViewRevDataPos.Rows[4].Cells[0].ReadOnly = true;
				radGridViewRevDataPos.Rows[5].Cells[0].Value = "REV_APPROVED_BY";
				radGridViewRevDataPos.Rows[5].Cells[0].ReadOnly = true;

				#region clear controls value
				radTextBoxDisplayCount.Text = string.Empty;
				radTextBoxDistance.Text = string.Empty;
                //this.radRadioButtonOffice.CheckState = CheckState.Checked;
				radDropDownListSorting.SelectedIndex = 0;
				#endregion
				using (ISmartISOService svcProxy = ServiceFactory.CreateServiceChannel<ISmartISOService>(AppDocData.ServiceURL))
				{
					#region Revision Data Setting
					using (System.Data.DataSet dsParam = svcProxy.GetCommandParam("SmartISO.BSL", "GetConfigCommand"))
					{
						dsParam.Tables["Params"].Rows.Clear();
						{
							DataRow dr = dsParam.Tables["Params"].NewRow();
                            dr["Ref_UID"] = SiteUID;
							dr["Section"] = "Revision Data Setting";
							dr["Key"] = string.Empty;
							dsParam.Tables["Params"].Rows.Add(dr);
						}
						using (DataSet ds = svcProxy.ExecuteCommand("SmartISO.BSL","GetConfigCommand",dsParam))
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
								else if ("Rev No Type" == sKey)
								{
                                    //if(oRow["value"].ToString() == "FIELD")
                                    //{
                                    //    this.radRadioButtonField.CheckState = CheckState.Checked;
                                    //}
                                    //else
                                    //{
                                    //    this.radRadioButtonOffice.CheckState = CheckState.Checked;
                                    //}
								}
							}
						}
					#endregion

						#region clear controls value
						foreach (GridViewRowInfo oGridRow in radGridViewRevDataPos.Rows)
						{
							radGridViewRevDataPos.Rows[oGridRow.Index].Cells["PosX"].Value = string.Empty;
							radGridViewRevDataPos.Rows[oGridRow.Index].Cells["PosY"].Value = string.Empty;
						}
						#endregion

						dsParam.Tables["Params"].Rows.Clear();
						{
							System.Data.DataRow drParam = dsParam.Tables["Params"].NewRow();
                            drParam["Ref_UID"] = SiteUID;
							drParam["Section"] = "Revision Pos Setting";
							drParam["Key"] = string.Empty;
							dsParam.Tables["Params"].Rows.Add(drParam);
						}
						using (DataSet ds = svcProxy.ExecuteCommand("SmartISO.BSL","GetConfigCommand",dsParam))
						{
							foreach (GridViewRowInfo oGridRow in radGridViewRevDataPos.Rows)
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
							this.radGridViewRevDataPos.BestFitColumns();
						}
					}

					#region load issue dates
					this.radGridViewIssueDate.Rows.Clear();

                    using (DataSet dsParam = svcProxy.GetCommandParam("SmartISO.BSL", "GetAllIssueDatesCommand"))
                    {
                        DataRow dr = dsParam.Tables["Params"].NewRow();
                        {
                            dr["Site_UID"] = SiteUID;
                        }
                        dsParam.Tables["Params"].Rows.Add(dr);

                        using (DataSet ds = svcProxy.ExecuteCommand("SmartISO.BSL", "GetAllIssueDatesCommand", dsParam))
                        {
                            this.radGridViewIssueDate.RowCount = ds.Tables[0].Rows.Count;

                            int iRow = 0;
                            foreach (DataRow oRow in ds.Tables[0].Rows)
                            {
                                this.radGridViewIssueDate.Rows[iRow].Cells["UID"].Value = oRow["UID"].ToString();
                                this.radGridViewIssueDate.Rows[iRow].Cells["IssueDate"].Value = oRow["IssueDate"].ToString();
                                this.radGridViewIssueDate.Rows[iRow].Cells["IssueDate"].ReadOnly = (true.ToString() == oRow["ApprovalStatus"].ToString());
                                this.radGridViewIssueDate.Rows[iRow].Cells["ApprovalDate"].Value = oRow["ApprovalDate"].ToString();
                                this.radGridViewIssueDate.Rows[iRow].Cells["ApprovalDate"].ReadOnly = (true.ToString() == oRow["ApprovalStatus"].ToString());
                                this.radGridViewIssueDate.Rows[iRow].Cells["ApprovalCount"].Value = oRow["ApprovalCount"].ToString();
                                this.radGridViewIssueDate.Rows[iRow].Cells["ApprovalCount"].ReadOnly = (true.ToString() == oRow["ApprovalStatus"].ToString());
                                this.radGridViewIssueDate.Rows[iRow].Cells["ApprovalStatus"].Value = oRow["ApprovalStatus"].ToString();
                                this.radGridViewIssueDate.Rows[iRow++].Cells["ApprovalStatus"].ReadOnly = (true.ToString() == oRow["ApprovalStatus"].ToString());
                            }
                        }
                    }
                    //this.radGridViewIssueDate.BestFitColumns();
					#endregion

					#region rev nos
					#region clear controls value
					foreach (GridViewRowInfo oGridRow in radGridViewRevNo.Rows)
					{
                        foreach (GridViewColumn oGridCol in this.radGridViewRevNo.Columns)
                        {
                            this.radGridViewRevNo.Rows[oGridRow.Index].Cells[oGridCol.Index].Value = string.Empty;
                        }
					}
					#endregion

					LoadRevNoData(svcProxy);
					#endregion

					#region issue date display format
					this.IssueDateDisplayFormatColl.Clear();
					using (System.Data.DataSet dsParam = svcProxy.GetCommandParam("SmartISO.BSL", "GetConfigCommand"))
					{
						dsParam.Tables["Params"].Rows.Clear();
						{
							System.Data.DataRow drParam = dsParam.Tables["Params"].NewRow();
                            drParam["Ref_UID"] = SiteUID;
							drParam["Section"] = "IssueDateDisplayFormat";
							drParam["Key"] = "List";
							dsParam.Tables["Params"].Rows.Add(drParam);
						}
						using (DataSet ds = svcProxy.ExecuteCommand("SmartISO.BSL","GetConfigCommand",dsParam))
						{
							if (1 == ds.Tables[0].Rows.Count)
							{
								string sValue = ds.Tables[0].Rows[0]["Value"].ToString();

								XmlDocument oXDoc = new XmlDocument();
								oXDoc.LoadXml(sValue);
								XmlElement XmlRoot = oXDoc["DisplayFormats"];
								foreach (XmlNode XmlDefinition in XmlRoot.ChildNodes)
								{
									try
									{
										IssueDateDisplayFormat oIssueDateDisplayFormat = new IssueDateDisplayFormat()
										{
											Name = XmlDefinition["Name"].InnerText,
											Content = XmlDefinition["Content"].InnerText
										};
										IssueDateDisplayFormatColl.Add(oIssueDateDisplayFormat);
									}
									catch { }
								}
							}
							IssueDateDisplayFormatColl.Add(new IssueDateDisplayFormat() { Name = "New or Edit...", Content = string.Empty });

							GridViewComboBoxColumn oDisplayFormatComboBoxCol = radGridViewIssueDateDisplay.Columns[1] as GridViewComboBoxColumn;
							{
								oDisplayFormatComboBoxCol.DataSource = IssueDateDisplayFormatColl;
								oDisplayFormatComboBoxCol.ValueMember = "Name";
							}
						}

						dsParam.Tables["Params"].Rows.Clear();
						{
							System.Data.DataRow drParam = dsParam.Tables["Params"].NewRow();
                            drParam["Ref_UID"] = SiteUID;
							drParam["Section"] = "IssueDateDisplayFormat";
							drParam["Key"] = "Setting";
							dsParam.Tables["Params"].Rows.Add(drParam);
						}
						using (DataSet ds = svcProxy.ExecuteCommand("SmartISO.BSL","GetConfigCommand",dsParam))
						{
							foreach (GridViewRowInfo oGridRow in this.radGridViewIssueDateDisplay.Rows)
							{
								this.radGridViewIssueDateDisplay.Rows[oGridRow.Index].Cells["DisplayFormat"].Value = string.Empty;
							}

							if (1 == ds.Tables[0].Rows.Count)
							{
								string sValue = ds.Tables[0].Rows[0]["Value"].ToString();

								XmlDocument oXDoc = new XmlDocument();
								oXDoc.LoadXml(sValue);
								XmlElement XmlRoot = oXDoc["Settings"];
								foreach (XmlNode XmlDefinition in XmlRoot.ChildNodes)
								{
									try
									{
										string sName = Name = XmlDefinition["Name"].InnerText;
										sValue = XmlDefinition["Value"].InnerText;
										var rows = from GridViewRowInfo row in radGridViewIssueDateDisplay.Rows
												   where ((null != row.Cells["DisplayName"].Value) && (sName == row.Cells["DisplayName"].Value.ToString()))
												   select row;
										if (1 == rows.ToList().Count)
										{
											rows.ToList()[0].Cells["DisplayFormat"].Value = sValue;
										}
									}
									catch { }
								}
							}
						}

						#region get border file path
						dsParam.Tables["Params"].Rows.Clear();
						{
							System.Data.DataRow drParam = dsParam.Tables["Params"].NewRow();
                            drParam["Ref_UID"] = SiteUID;
							drParam["Section"] = "Border Setting";
							drParam["Key"] = string.Empty;
							dsParam.Tables["Params"].Rows.Add(drParam);
						}
						using (DataSet ds = svcProxy.ExecuteCommand("SmartISO.BSL", "GetConfigCommand", dsParam))
						{
							foreach (DataRow oRow in ds.Tables[0].Rows)
							{
								object oValue = oRow["key"];
								string sKey = (null != oValue) ? oValue.ToString() : string.Empty;
								if ("Border File Path" == sKey)
								{
									oValue = oRow["Value"];
									this.BorderFilePath = (null != oValue) ? oValue.ToString() : string.Empty;
								}
							}
						}
						#endregion
					}
					#endregion
				}

				radGridViewTextInfo.BestFitColumns();
			}
			catch (Exception ex)
			{
				new TechSun.Theme.frmWarning(ex.Message).ShowDialog(this);
			}
		}

		/// <summary>
		/// load rev no data
		/// </summary>
		/// <author>humkyung</author>
		/// <date>2016.08.31</date>
		/// <param name="svcProxy"></param>
		private void LoadRevNoData(ISmartISOService svcProxy)
		{
            using (DataSet dsParam = svcProxy.GetCommandParam("SmartISO.BSL", "GetAllRevNosCommand"))
            {
                DataRow dr = dsParam.Tables["Params"].NewRow();
                {
                    dr["Site_UID"] = SiteUID;
                }
                dsParam.Tables["Params"].Rows.Add(dr);
                using (DataSet ds = svcProxy.ExecuteCommand("SmartISO.BSL", "GetAllRevNosCommand", dsParam))
                {
                    if (ds.Tables[0].Rows.Count > 0)
                    {
                        Int64 iUID = Convert.ToInt64(ds.Tables[0].Rows[ds.Tables[0].Rows.Count-1]["UID"].ToString());
                        this.radGridViewRevNo.RowCount = Convert.ToInt32(iUID / 100) + 1;
                        foreach (DataRow oRow in ds.Tables[0].Rows)
                        {
                            int iRow = Convert.ToInt32(Convert.ToInt64(oRow["UID"])/100);
                            int iCol = Convert.ToInt32(Convert.ToInt64(oRow["UID"]) - iRow*100);
                            this.radGridViewRevNo.Rows[iRow].Cells[iCol+1].Value = oRow["RevNo"];
                        }
                    }
                }
            }
            this.radGridViewRevNo.BestFitColumns();
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

		/// <summary>
		/// save setting data
		/// </summary>
		/// <author>humkyung</author>
		/// <date>2016.09.01</date>
		public override void SaveData()
		{
			base.SaveData();

			if ((this.radDropDownListProject.SelectedIndex < 0) || (this.radDropDownListSite.SelectedIndex < 0)) return;

			try
			{
				Cursor.Current = Cursors.WaitCursor;

				using (ISmartISOService svcProxy = ServiceFactory.CreateServiceChannel<ISmartISOService>(AppDocData.ServiceURL))
				{
					try
					{
						using (DataSet dsParam = svcProxy.GetCommandParam("SmartISO.BSL", "SaveConfigsCommand"))
						{
							{
								DataRow drParam = dsParam.Tables["Params"].NewRow();
                                drParam["Ref_UID"] = SiteUID;
								drParam["Section"] = "Revision Data Setting";
								drParam["Key"] = "Display Count";
								drParam["Value"] = radTextBoxDisplayCount.Text;
								dsParam.Tables["Params"].Rows.Add(drParam);
							}
							{
								DataRow drParam = dsParam.Tables["Params"].NewRow();
                                drParam["Ref_UID"] = SiteUID;
								drParam["Section"] = "Revision Data Setting";
								drParam["Key"] = "Sorting";
								drParam["Value"] = radDropDownListSorting.Text;
								dsParam.Tables["Params"].Rows.Add(drParam);
							}
							{
								DataRow drParam = dsParam.Tables["Params"].NewRow();
                                drParam["Ref_UID"] = SiteUID;
								drParam["Section"] = "Revision Data Setting";
								drParam["Key"] = "Dist";
								drParam["Value"] = radTextBoxDistance.Text;
								dsParam.Tables["Params"].Rows.Add(drParam);
							}

							{
								object oValue = radGridViewTextInfo.Rows[0].Cells[0].Value;
								string sValue = (null != oValue) ? oValue.ToString() : string.Empty;
								{
									DataRow drParam = dsParam.Tables["Params"].NewRow();
                                    drParam["Ref_UID"] = SiteUID;
									drParam["Section"] = "Revision Data Setting";
									drParam["Key"] = "Layer";
									drParam["Value"] = sValue;
									dsParam.Tables["Params"].Rows.Add(drParam);
								}
								oValue = radGridViewTextInfo.Rows[0].Cells[1].Value;
								sValue = (null != oValue) ? oValue.ToString() : string.Empty;
								{
									DataRow drParam = dsParam.Tables["Params"].NewRow();
                                    drParam["Ref_UID"] = SiteUID;
									drParam["Section"] = "Revision Data Setting";
									drParam["Key"] = "TextStyle";
									drParam["Value"] = sValue;
									dsParam.Tables["Params"].Rows.Add(drParam);
								}
								oValue = radGridViewTextInfo.Rows[0].Cells[2].Value;
								sValue = (null != oValue) ? oValue.ToString() : string.Empty;
								{
									DataRow drParam = dsParam.Tables["Params"].NewRow();
                                    drParam["Ref_UID"] = SiteUID;
									drParam["Section"] = "Revision Data Setting";
									drParam["Key"] = "TextHeight";
									drParam["Value"] = sValue;
									dsParam.Tables["Params"].Rows.Add(drParam);
								}
								oValue = radGridViewTextInfo.Rows[0].Cells[3].Value;
								sValue = (null != oValue) ? oValue.ToString() : string.Empty;
								{
									DataRow drParam = dsParam.Tables["Params"].NewRow();
                                    drParam["Ref_UID"] = SiteUID;
									drParam["Section"] = "Revision Data Setting";
									drParam["Key"] = "Justify";
									drParam["Value"] = sValue;
									dsParam.Tables["Params"].Rows.Add(drParam);
								}
								oValue = radGridViewTextInfo.Rows[0].Cells[4].Value;
								sValue = (null != oValue) ? oValue.ToString() : string.Empty;
								{
									DataRow drParam = dsParam.Tables["Params"].NewRow();
                                    drParam["Ref_UID"] = SiteUID;
									drParam["Section"] = "Revision Data Setting";
									drParam["Key"] = "Rotation";
									drParam["Value"] = sValue;
									dsParam.Tables["Params"].Rows.Add(drParam);
								}
								oValue = radGridViewTextInfo.Rows[0].Cells[5].Value;
								sValue = (null != oValue) ? oValue.ToString() : string.Empty;
								{
									DataRow drParam = dsParam.Tables["Params"].NewRow();
                                    drParam["Ref_UID"] = SiteUID;
									drParam["Section"] = "Revision Data Setting";
									drParam["Key"] = "TextColor";
									drParam["Value"] = sValue;
									dsParam.Tables["Params"].Rows.Add(drParam);
								}
								//oValue = radGridViewTextInfo.Rows[0].Cells[5].Value;
								//sValue = (null != oValue) ? oValue.ToString() : string.Empty;
								{
									DataRow drParam = dsParam.Tables["Params"].NewRow();
                                    drParam["Ref_UID"] = SiteUID;
									drParam["Section"] = "Revision Data Setting";
									drParam["Key"] = "Rev No Type";
                                    //drParam["Value"] = (radRadioButtonOffice.CheckState == CheckState.Checked) ? "OFFICE" : "FIELD";
									dsParam.Tables["Params"].Rows.Add(drParam);
								}
							}

							string sSection = "Revision Pos Setting";
							foreach (GridViewRowInfo oGridRow in radGridViewRevDataPos.Rows)
							{
								string sValue = string.Empty;
								try
								{
									sValue = string.Format("{0},{1}", radGridViewRevDataPos.Rows[oGridRow.Index].Cells["PosX"].Value.ToString(), radGridViewRevDataPos.Rows[oGridRow.Index].Cells["PosY"].Value.ToString());
								}
								catch { }

								{
									DataRow drParam = dsParam.Tables["Params"].NewRow();
                                    drParam["Ref_UID"] = SiteUID;
									drParam["Section"] = sSection;
									drParam["Key"] = radGridViewRevDataPos.Rows[oGridRow.Index].Cells["Item"].Value.ToString();
									drParam["Value"] = sValue;
									dsParam.Tables["Params"].Rows.Add(drParam);
								}
							}

							#region save issue date display format
							{
								XmlDocument xDoc = new XmlDocument();
								XmlElement xmlRoot = xDoc.CreateElement("DisplayFormats");
								xDoc.AppendChild(xmlRoot);
								foreach (IssueDateDisplayFormat oIssueDateDisplayFormat in IssueDateDisplayFormatColl)
								{
									if (NewOrEdit == oIssueDateDisplayFormat.Name) continue;
									XmlElement xml = xDoc.CreateElement("DisplayFormat");
									xmlRoot.AppendChild(xml);
									{
										XmlElement oXmlEle = xDoc.CreateElement("Name");
										oXmlEle.InnerText = oIssueDateDisplayFormat.Name;
										xml.AppendChild(oXmlEle);

										oXmlEle = xDoc.CreateElement("Content");
										oXmlEle.InnerText = oIssueDateDisplayFormat.Content;
										xml.AppendChild(oXmlEle);
									}
								}

								{
									DataRow drParam = dsParam.Tables["Params"].NewRow();
                                    drParam["Ref_UID"] = SiteUID;
									drParam["Section"] = "IssueDateDisplayFormat";
									drParam["Key"] = "List";
									drParam["Value"] = xDoc.InnerXml;
									dsParam.Tables["Params"].Rows.Add(drParam);
								}
							}
							{
								XmlDocument xDoc = new XmlDocument();
								XmlElement xmlRoot = xDoc.CreateElement("Settings");
								xDoc.AppendChild(xmlRoot);
								foreach (GridViewRowInfo oGridRow in radGridViewIssueDateDisplay.Rows)
								{
									XmlElement xml = xDoc.CreateElement("Setting");
									xmlRoot.AppendChild(xml);
									{
										XmlElement oXmlEle = xDoc.CreateElement("Name");
										oXmlEle.InnerText = radGridViewIssueDateDisplay.Rows[oGridRow.Index].Cells[0].Value.ToString();
										xml.AppendChild(oXmlEle);

										oXmlEle = xDoc.CreateElement("Value");
										object oValue = radGridViewIssueDateDisplay.Rows[oGridRow.Index].Cells[1].Value;
										string sValue = ((null != oValue) && NewOrEdit != oValue.ToString()) ? oValue.ToString() : string.Empty;
										oXmlEle.InnerText = sValue;
										xml.AppendChild(oXmlEle);
									}
								}

								{
									DataRow drParam = dsParam.Tables["Params"].NewRow();
                                    drParam["Ref_UID"] = SiteUID;
									drParam["Section"] = "IssueDateDisplayFormat";
									drParam["Key"] = "Setting";
									drParam["Value"] = xDoc.InnerXml;
									dsParam.Tables["Params"].Rows.Add(drParam);
								}
							}
							#endregion

							svcProxy.ExecuteCommand("SmartISO.BSL", "SaveConfigsCommand", dsParam);
						}

						#region save issue dates
                        using (DataSet dsParam = svcProxy.GetCommandParam("SmartISO.BSL", "SaveAllIssueDatesCommand"))
                        {
                            foreach (GridViewRowInfo oGridRow in this.radGridViewIssueDate.Rows)
                            {
                                DataRow dr = dsParam.Tables["Params"].NewRow();

                                object oValue = oGridRow.Cells["UID"].Value;
                                Int64 iUID = ((null == oValue) || (string.IsNullOrEmpty(oValue.ToString()))) ? -1 : Convert.ToInt64(oValue);

                                oValue = oGridRow.Cells["IssueDate"].Value;
                                if ((null == oValue) || (string.IsNullOrEmpty(oValue.ToString())))
                                {
                                    new TechSun.Theme.frmInfomation("Rev No is Null or Empty").ShowDialog(this);
                                    break;
                                }

                                string sApprovalDate = string.Empty;
                                if ((null != oGridRow.Cells["ApprovalDate"].Value) && !string.IsNullOrEmpty(oGridRow.Cells["ApprovalDate"].Value.ToString()))
                                {
                                    sApprovalDate = Convert.ToDateTime(oGridRow.Cells["ApprovalDate"].Value.ToString()).ToString("yyyy-MM-dd");
                                }

                                int iApprovalCount = 0;
                                if (null != oGridRow.Cells["ApprovalCount"].Value)
                                {
                                    int.TryParse(oGridRow.Cells["ApprovalCount"].Value.ToString(), out iApprovalCount);
                                }
                                bool bApproved = false;
                                if (null != oGridRow.Cells["ApprovalStatus"].Value)
                                {
                                    bool.TryParse(oGridRow.Cells["ApprovalStatus"].Value.ToString(), out bApproved);
                                }

                                dr["UID"] = iUID;
                                dr["Site_UID"] = SiteUID;
                                dr["IssueDate"] = oGridRow.Cells["IssueDate"].Value.ToString();
                                dr["ApprovalDate"] = sApprovalDate;
                                dr["ApprovalCount"] = iApprovalCount;
                                dr["ApprovalStatus"] = bApproved;
                                dsParam.Tables["Params"].Rows.Add(dr);
                            }
                        
                            svcProxy.ExecuteCommand("SmartISO.BSL", "SaveAllIssueDatesCommand", dsParam);
                        }
						#endregion

						#region save rev no datas
						try
						{
                            using (DataSet dsParam = svcProxy.GetCommandParam("SmartISO.BSL", "SaveAllRevNosCommand"))
                            {
                                foreach (GridViewRowInfo oGridRow in this.radGridViewRevNo.Rows)
                                {
                                    foreach (GridViewColumn oGridCol in this.radGridViewRevNo.Columns)
                                    {
                                        object oValue = oGridRow.Cells[oGridCol.Index].Value;
                                        if ((null == oValue) || (string.IsNullOrEmpty(oValue.ToString()))) continue;

                                        string sValue = (null != oValue) ? oValue.ToString() : string.Empty;
                                        DataRow dr = dsParam.Tables["Params"].NewRow();
                                        {
                                            dr["Site_UID"] = SiteUID;
                                            dr["UID"] = oGridRow.Index * 100 + (oGridCol.Index-1);
                                            dr["RevNo"] = sValue;
                                            dr["RevDesc"] = string.Empty;
                                        }
                                        dsParam.Tables["Params"].Rows.Add(dr);
                                    }
                                }

                                svcProxy.ExecuteCommand("SmartISO.BSL", "SaveAllRevNosCommand", dsParam);
                            }
						}
						finally
						{
							LoadRevNoData(svcProxy);
						}
						#endregion
					}
					catch (Exception ex)
					{
						new TechSun.Theme.frmWarning(ex.Message).ShowDialog(this);
					}
				}
			}
			finally
			{
				Cursor.Current = Cursors.Default;
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
			SaveData();
		}

		private void radButtonDistance_Click(object sender, EventArgs e)
		{
            try
            {
                using (PlugInServiceGetDistanceCommand cmd = new PlugInServiceGetDistanceCommand() { ControlName = this.Name })
                {
                    if (true == cmd.Execute())
                    {
                        this.radTextBoxDistance.Text = cmd.Distance.ToString();
                    }
                }
            }
            catch (Exception ex)
            {
                new TechSun.Theme.frmWarning(ex.Message).ShowDialog(this);
            }
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
			GridCommandCellElement cell = (GridCommandCellElement)sender;

            //try
            //{
            //    PF.Open(this.BorderFilePath);
            //    double[] res = PF.GetCADPoint();
            //    if ((null != res) && (3 == res.Length))
            //    {
            //        string sHeight = "1.0";
            //        if (null != cell.Value)
            //        {
            //            string[] oTokens = cell.Value.ToString().Split(",".ToCharArray());
            //            if (3 == oTokens.Length) sHeight = oTokens[2];
            //        }

            //        radGridViewRevDataPos.Rows[cell.RowIndex].Cells["PosX"].Value = string.Format("{0:0.00}", ((double[])(res))[0]);
            //        radGridViewRevDataPos.Rows[cell.RowIndex].Cells["PosY"].Value = string.Format("{0:0.00}", ((double[])(res))[1]);
            //    }
            //    else
            //    {
            //        radGridViewRevDataPos.Rows[cell.RowIndex].Cells["PosX"].Value = string.Empty;
            //        radGridViewRevDataPos.Rows[cell.RowIndex].Cells["PosY"].Value = string.Empty;
            //    }
            //}
            //catch(Exception ex)
            //{
            //    new TechSun.Theme.frmWarning(ex.Message).ShowDialog(this);
            //}
		}

		private void radDropDownListProject_SelectedIndexChanged(object sender, Telerik.WinControls.UI.Data.PositionChangedEventArgs e)
		{
            if (radDropDownListProject.SelectedIndex >= 0)
            {
                LoadSiteData();
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
				string sSiteID = radDropDownListSite.Items[radDropDownListSite.SelectedIndex].Tag as string;
				if (!string.IsNullOrEmpty(sSiteID))
				{
                    UserInfo.SiteID = sSiteID;
					LoadData();
				}
			}
		}

		private void radGridViewIssueDate_UserAddedRow(object sender, GridViewRowEventArgs e)
		{
			radGridViewIssueDate.Rows[e.Row.Index].Cells["ApprovalStatus"].Value = false.ToString();
		}

		/// <summary>
		/// backup line data of selected issue date
		/// </summary>
		/// <author>humkyung</author>
		/// <date>2016.08.12</date>
		/// <param name="sender"></param>
		/// <param name="e"></param>
		private void radGridViewIssueDate_CellDoubleClick(object sender, GridViewCellEventArgs e)
		{
            //if ("ApprovalStatus" == radGridViewIssueDate.Columns[e.ColumnIndex].Name)
            //{
            //    DialogResult res = MessageBox.Show("Do you want to approve line data", "Message", MessageBoxButtons.YesNo, MessageBoxIcon.Warning);
            //    if (DialogResult.Yes == res)
            //    {
            //        string sValue = Convert.ToDateTime(radGridViewIssueDate.Rows[e.RowIndex].Cells["IssueDate"].Value.ToString()).ToString("yyyy-MM-dd");

            //        if (!string.IsNullOrEmpty(sValue))
            //        {
            //            using (ISmartISOService svcProxy = ServiceFactory.CreateServiceChannel<ISmartISOService>(AppDocData.ServiceURL))
            //            {
            //                using (DataSet dsParam = svcProxy.GetCommandParam("SmartISO.BSL", "ApproveLineDataCommand"))
            //                {
            //                    try
            //                    {
            //                        DataRow drParam = dsParam.Tables["RevDate"].NewRow();
            //                        drParam["RevDate"] = sValue;
            //                        dsParam.Tables["RevDate"].Rows.Add(drParam);
            //                        svcProxy.ExecuteCommand("SmartISO.BSL", "ApproveLineDataCommand", dsParam);

            //                        if (radGridViewIssueDate.Rows[e.RowIndex].Tag is IssueDateDTO)
            //                        {
            //                            IssueDateDTO dto = radGridViewIssueDate.Rows[e.RowIndex].Tag as IssueDateDTO;
            //                            dto.Approved = true;
            //                            radGridViewIssueDate.Rows[e.RowIndex].InvalidateRow();
            //                        }
            //                    }
            //                    catch (Exception ex)
            //                    {
            //                        TechSun.Theme.frmWarning frm = new TechSun.Theme.frmWarning(ex.Message);
            //                        frm.ShowDialog(this);
            //                        return;
            //                    }
            //                }
            //            }
            //        }

            //        TechSun.Theme.frmInfomation frmInfo = new TechSun.Theme.frmInfomation("Approve linedata is complete");
            //        frmInfo.ShowDialog(this);
            //    }
            //}
		}

		/// <summary>
		/// set enable to false if approval status is True
		/// </summary>
		/// <author>humkyung</author>
		/// <date>2016.08.23</date>
		/// <param name="sender"></param>
		/// <param name="e"></param>
		private void radGridViewIssueDate_CellFormatting(object sender, CellFormattingEventArgs e)
		{
            //e.CellElement.Enabled = true;
            //if (e.RowIndex > -1 && e.Column.Name == "ApprovalStatus")
            //{
            //    if (radGridViewIssueDate.Rows[e.RowIndex].Tag is IssueDateDTO)
            //    {
            //        IssueDateDTO dto = radGridViewIssueDate.Rows[e.RowIndex].Tag as IssueDateDTO;
            //        e.CellElement.Enabled = !dto.Approved;
            //    }
            //}
		}

		private void radGridViewIssueDateDisplay_CellEditorInitialized(object sender, GridViewCellEventArgs e)
		{
			{
				var editor = e.ActiveEditor as RadDropDownListEditor;
				if (editor != null)
				{
					editor.ValueChanged -= new EventHandler(radGridViewIssueDateDisplay_ComboBoxValueChanged);
					editor.ValueChanged += new EventHandler(radGridViewIssueDateDisplay_ComboBoxValueChanged);
				}
			}
		}

		/// <summary>
		/// show issue date display format form when user select "New or Edit"
		/// </summary>
		/// <author>humkyung</author>
		/// <date>2016.08.29</date>
		/// <param name="sender"></param>
		/// <param name="e"></param>
		void radGridViewIssueDateDisplay_ComboBoxValueChanged(object sender, EventArgs e)
		{
			var editor = radGridViewIssueDateDisplay.ActiveEditor as RadDropDownListEditor;
            if (editor != null)
            {
                var editorElement = editor.EditorElement as RadDropDownListEditorElement;
                if (editorElement.SelectedIndex == -1)
                {
                    editorElement.SelectedIndex = IssueDateDisplayFormatColl.Count;
                    radGridViewIssueDateDisplay.CancelEdit();
                }
                else
                {
                    string sName = editorElement.Items[editorElement.SelectedIndex].Text;
                    radGridViewIssueDateDisplay.EndEdit();
                    if (NewOrEdit == sName)
                    {
                        RadFormIssueDateDisplayFormat frm = new RadFormIssueDateDisplayFormat(sName, this.IssueDateDisplayFormatColl);
                        if (DialogResult.OK == frm.ShowDialog(this))
                        {
                            IssueDateDisplayFormat res = IssueDateDisplayFormatColl.Find(param => param.Name == frm.DisplayName);
                            if (null != res)
                            {
                                radGridViewIssueDateDisplay.CurrentCell.Value = res.Name;
                            }
                            else
                            {
                                IssueDateDisplayFormatColl.Add(new IssueDateDisplayFormat()
                                {
                                    Name = frm.DisplayName,
                                    Content = frm.Value
                                });
                                radGridViewIssueDateDisplay.CurrentCell.Value = frm.DisplayName;
                            }
                        }
                        else
                        {
                            IssueDateDisplayFormat res = IssueDateDisplayFormatColl.Find(param => param.Name != NewOrEdit);
                            if (null != res)
                            {
                                radGridViewIssueDateDisplay.CurrentCell.Value = res.Name;
                            }
                            else
                            {
                                radGridViewIssueDateDisplay.CurrentCell.Value = string.Empty;
                            }
                        }
                    }
                }
            }
		}

		/// <summary>
		/// delete a new issue date
		/// </summary>
		/// <author>humkyung</author>
		/// <date>2016.08.30</date>
		/// <param name="sender"></param>
		/// <param name="e"></param>
		private void radButtonAdd_Click(object sender, EventArgs e)
		{
			GridViewRowInfo oGridRow = radGridViewIssueDate.Rows.AddNew();
			radGridViewIssueDate.Rows[oGridRow.Index].Cells["ApprovalStatus"].Value = false.ToString();
			radGridViewIssueDate.Rows[oGridRow.Index].Cells["ApprovalStatus"].ReadOnly = false;
		}

		/// <summary>
		/// delete checked issue dates
		/// </summary>
		/// <author>humkyung</author>
		/// <date>2016.08.30</date>
		/// <param name="sender"></param>
		/// <param name="e"></param>
		private void radButtonDelete_Click(object sender, EventArgs e)
		{
			if (this.CheckedIssueDateList.ToList().Count > 0)
			{
				List<int> oCheckedItemList = this.CheckedIssueDateList.ToList();
				oCheckedItemList.Reverse();
				foreach (int iRowIndex in oCheckedItemList)
				{
					radGridViewIssueDate.Rows.RemoveAt(iRowIndex);
				}
			}
		}

		/// <summary>
		/// add a new rev no
		/// </summary>
		/// <author>humkyung</author>
		/// <date>2016.08.30</date>
		/// <param name="sender"></param>
		/// <param name="e"></param>
		private void radButtonAddRevNo_Click(object sender, EventArgs e)
		{
            GridViewRowInfo oGridRow = this.radGridViewRevNo.Rows.AddNew();
            foreach(GridViewColumn oGridCol in this.radGridViewRevNo.Columns)
            {
                oGridRow.Cells[oGridCol.Name].Tag = oGridRow.Index*100 + oGridCol.Index;
            }
		}

		/// <summary>
		/// delete checked items
		/// </summary>
		/// <author>humkyung</author>
		/// <date>2016.08.30</date>
		/// <param name="sender"></param>
		/// <param name="e"></param>
		private void radButtonDeleteRevNo_Click(object sender, EventArgs e)
		{
            //if (this.radRadioButtonOffice.CheckState == CheckState.Checked)
            //{
            //    if (this.CheckedRevNoList.ToList().Count > 0)
            //    {
            //        List<int> oCheckedItemList = this.CheckedRevNoList.ToList();
            //        oCheckedItemList.Reverse();
            //        foreach (int iRowIndex in oCheckedItemList)
            //        {
            //            if (this.radGridViewRevNo.Rows[iRowIndex].Tag is RevisionNoDTO)
            //            {
            //                RevisionNoDTO oRevNoDTO = this.radGridViewRevNo.Rows[iRowIndex].Tag as RevisionNoDTO;
            //                oRevNoDTO.Deleted = true;
            //            }

            //            this.radGridViewRevNo.Rows.RemoveAt(iRowIndex);
            //        }
            //    }
            //}
            //else
            //{
            //    if (this.CheckedFieldRevNoList.ToList().Count > 0)
            //    {
            //        List<int> oCheckedItemList = this.CheckedFieldRevNoList.ToList();
            //        oCheckedItemList.Reverse();
            //        foreach (int iRowIndex in oCheckedItemList)
            //        {
            //            if (this.radGridViewRevNoField.Rows[iRowIndex].Tag is RevisionNoDTO)
            //            {
            //                RevisionNoDTO oRevNoDTO = this.radGridViewRevNoField.Rows[iRowIndex].Tag as RevisionNoDTO;
            //                oRevNoDTO.Deleted = true;
            //            }

            //            this.radGridViewRevNoField.Rows.RemoveAt(iRowIndex);
            //        }
            //    }
            //}
		}

		/// <summary>
		/// update enabled property of control
		/// </summary>
		/// <author>humkyung</author>
		/// <date>2016.08.31</date>
		public void OnIdleUpdate()
		{
            bool bEnabled = (this.radDropDownListProject.SelectedIndex >= 0) && (this.radDropDownListSite.SelectedIndex >= 0);
			foreach (Control oCtrl in this.Controls)
			{
				if (oCtrl is RadLabel) continue;
				if ((oCtrl == radDropDownListProject) || (oCtrl == radDropDownListSite) || (oCtrl == this.radButtonDisplayBorderFile)) continue;
				oCtrl.Enabled = bEnabled;
			}

            //if ((true == this.radButtonDisplayBorderFile.Enabled) && (false == AutoCADLauncher.AutoCAD2014Installed))
            //{
            //    this.radButtonDisplayBorderFile.Enabled = false;
            //}
		}

		/// <summary>
		/// update rev no or rev desc
		/// </summary>
		/// <author>humkyung</author>
		/// <date>2016.08.31</date>
		/// <param name="sender"></param>
		/// <param name="e"></param>
		private void radGridViewRevNo_CellEndEdit(object sender, GridViewCellEventArgs e)
		{
            //string sColumnName = this.radGridViewRevNo.Columns[e.ColumnIndex].Name;
            //if (this.radGridViewRevNo.Rows[e.RowIndex].Tag is RevisionNoDTO)
            //{
            //    RevisionNoDTO oRevNoDTO = this.radGridViewRevNo.Rows[e.RowIndex].Tag as RevisionNoDTO;

            //    object oValue = this.radGridViewRevNo.Rows[e.RowIndex].Cells[e.ColumnIndex].Value;
            //    string sValue = (null != oValue) ? oValue.ToString() : string.Empty;
            //    if ("RevNo" == sColumnName)
            //    {
            //        oRevNoDTO.RevNo = sValue;
            //    }
            //    else if ("RevDesc" == sColumnName)
            //    {
            //        oRevNoDTO.RevDesc = sValue;
            //    }
            //}
		}
		private void radGridViewRevNoField_CellEndEdit(object sender, GridViewCellEventArgs e)
		{
            //string sColumnName = this.radGridViewRevNoField.Columns[e.ColumnIndex].Name;
            //if (this.radGridViewRevNoField.Rows[e.RowIndex].Tag is RevisionNoDTO)
            //{
            //    RevisionNoDTO oRevNoDTO = this.radGridViewRevNoField.Rows[e.RowIndex].Tag as RevisionNoDTO;

            //    object oValue = this.radGridViewRevNoField.Rows[e.RowIndex].Cells[e.ColumnIndex].Value;
            //    string sValue = (null != oValue) ? oValue.ToString() : string.Empty;
            //    if ("RevNo" == sColumnName)
            //    {
            //        oRevNoDTO.RevNo = sValue;
            //    }
            //    else if ("RevDesc" == sColumnName)
            //    {
            //        oRevNoDTO.RevDesc = sValue;
            //    }
            //}
		}

		/// <summary>
		/// open border file
		/// </summary>
		/// <author>humkyung</author>
		/// <date>2016.09.05</date>
		/// <param name="sender"></param>
		/// <param name="e"></param>
		private void radButtonDisplayBorderFile_Click(object sender, EventArgs e)
		{
            //if (false == AutoCADLauncher.LaunchIfPlugInInAppServiceNotAvailable(this)) return;

            //try
            //{
            //    string sTempFilePath = Path.Combine(Path.GetTempPath(), Path.GetFileName(this.BorderFilePath));
            //    if (!File.Exists(sTempFilePath))
            //    {
            //        File.Copy(this.BorderFilePath, sTempFilePath);
            //    }
            //    PF.Open(sTempFilePath);
            //    PF.gbl_doc = PF.gbl_app.ActiveDocument;
            //    string sAutoCADVer = PF.GetVersionNumber();
            //    PF.gbl_doc.Application.ZoomExtents();
            //}
            //catch(Exception ex)
            //{
            //    new TechSun.Theme.frmWarning(ex.Message).ShowDialog();
            //}
		}

		private void radTextBoxDisplayCount_KeyPress(object sender, KeyPressEventArgs e)
		{
			if(!(char.IsDigit(e.KeyChar) || e.KeyChar == Convert.ToChar(Keys.Back)))
			{
				e.Handled = true;
			}
		}
    }
}
