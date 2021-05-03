using System;
using System.Collections.Generic;
using System.Linq;
using System.Windows.Forms;
using Configuration.Form.RadForm;
using Configuration.Properties;
using System.Data;
using System.Xml;

using SmartISO.Framework.Service;
using SmartISO.Service.Interface;
using Telerik.WinControls.UI;

namespace Configuration
{
	public partial class UserControlSmartISOSiteSetting : ThemeChangeable
	{
		public const string DisplayName = "Site";

        private Int64 ProjectUID
        {
            get { return Convert.ToInt64(this.radDropDownListProject.Items[this.radDropDownListProject.SelectedIndex].Tag); }
        }

        private Int64 SiteUID
        {
            get { return Convert.ToInt64(this.radGridViewSiteList.SelectedRows[0].Cells["UID"].Value.ToString()); }
        }

        public IEnumerable<int> CheckedMembers
        {
            get
            {
                var rows = (from GridViewRowInfo row in this.radGridViewMembers.Rows
                            where ((null != row.Cells["Check"].Value) && (true.ToString() == row.Cells["Check"].Value.ToString()))
                            select row.Index);
                return rows;
            }
        }

		public class EmailSetting
		{
			public string ID { get; set; }
			public string Name { get; set; }
			public string Address { get; set; }
		}

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

		public UserControlSmartISOSiteSetting()
		{
			InitializeComponent();

			Program.OnIdleUpdate += new Program.IdleUpdateEvent(OnIdleUpdate);
            this.Disposed += new EventHandler(UserControlSmartISOSiteSetting_Disposed);
			ApplyTheme(this.Controls);
		}

        void UserControlSmartISOSiteSetting_Disposed(object sender, EventArgs e)
        {
            Program.OnIdleUpdate -= new Program.IdleUpdateEvent(OnIdleUpdate); 
        }

		private void UserControlIDCSSiteSetting_Load(object sender, EventArgs e)
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
		}

		/// <summary>
		/// load and display site setting
		/// </summary>
		/// <author>humkyung</author>
		/// <date>2017.05211</date>
		public override void LoadData()
		{
			if (this.radDropDownListProject.SelectedIndex < 0) return;

            using (ISmartISOService svcProxy = ServiceFactory.CreateServiceChannel<ISmartISOService>(AppDocData.ServiceURL))
            {
                this.radGridViewSiteList.DataSource = null;

                using (DataSet dsParam = svcProxy.GetCommandParam("SmartISO.BSL", "GetSiteListCommand"))
                {
                    DataRow dr = dsParam.Tables["Params"].NewRow();
                    {
                        dr["Project_UID"] =  ProjectUID;
                    }
                    dsParam.Tables["Params"].Rows.Add(dr);

                    using (DataSet dsSite = svcProxy.ExecuteCommand("SmartISO.BSL", "GetSiteListCommand", dsParam))
                    {
                        this.radGridViewSiteList.DataSource = dsSite.Tables[0];
                        if (this.radGridViewSiteList.Columns.Contains("CopySetting"))
                        {
                            this.radGridViewSiteList.Columns.Remove("CopySetting");
                        }
                        this.radGridViewSiteList.Columns.Add(new GridViewCommandColumn("CopySetting", "CopySetting"));
                        {
                            this.radGridViewSiteList.Columns["UID"].IsVisible = false;
                            this.radGridViewSiteList.Columns["Project_UID"].IsVisible = false;
                            this.radGridViewSiteList.Columns["DeleteYN"].IsVisible = false;
                        }
                    }
                }

                foreach (GridViewRowInfo oGridRow in this.radGridViewSiteList.Rows)
                {
                    this.radGridViewSiteList.Rows[oGridRow.Index].Cells["CopySetting"].Value = "Copy Setting";
                }
                radGridViewSiteList.BestFitColumns();

                ///// get wbs filter for project - 2016.10.06 added by humkyung
                //using (DataSet dsParam = svcProxy.GetCommandParam("SP3D.BSL", "GetWBSFilterCommand"))
                //{
                //    DataSet dsWBS = svcProxy.ExecuteCommand("SP3D.BSL", "GetWBSFilterCommand", dsParam);
                //    if (null != dsWBS) this.radGridViewWBS.DataSource = dsWBS.Tables[0];
                //}
                //this.radGridViewWBS.BestFitColumns();
                //this.radGridViewWBS.Tag = null;
                //this._WBSColl.Clear();
                /// up to here
            }
		}

		/// <summary>
		/// load WBS Filter for Site
		/// </summary>
		/// <author>humkyung</author>
		/// <date>2016.10.06</date>
		/// <param name="oSiteDTO"></param>
        //private void LoadSiteData(SiteDTO oSiteDTO)
        //{
        //    if (null != oSiteDTO)
        //    {
        //        int iSiteID = Convert.ToInt32(UserInfo.SiteID);
        //        UserInfo.SiteID = oSiteDTO.Id.ToString();
        //        try
        //        {
        //            if (!this._WBSColl.ContainsKey(oSiteDTO))
        //            {
        //                this._WBSColl.Add(oSiteDTO, new List<string>());

        //                using (ISmartISOService svcProxy = ServiceFactory.CreateServiceChannel<ISmartISOService>(AppDocData.ServiceURL))
        //                {
        //                    using (System.Data.DataSet dsParam = svcProxy.GetCommandParam("IDCS.BSL", "GetConfigCommand"))
        //                    {
        //                        System.Data.DataRow drParam = dsParam.Tables["Params"].NewRow();
        //                        drParam["Section"] = "WBS Filters";
        //                        drParam["Key"] = oSiteDTO.Id.ToString();
        //                        dsParam.Tables["Params"].Rows.Add(drParam);

        //                        using (DataSet ds = svcProxy.ExecuteCommand("IDCS.BSL","GetConfigCommand",dsParam))
        //                        {
        //                            if (1 == ds.Tables[0].Rows.Count)
        //                            {
        //                                string sValue = ds.Tables[0].Rows[0]["Value"].ToString();
        //                                XmlDocument oXDoc = new XmlDocument();
        //                                oXDoc.LoadXml(sValue);
        //                                XmlElement XmlRoot = oXDoc["WBS_Filter"];
        //                                foreach (XmlNode XmlDefinition in XmlRoot.ChildNodes)
        //                                {
        //                                    string sName = XmlDefinition.InnerText;
        //                                    this._WBSColl[oSiteDTO].Add(sName);
        //                                }
        //                            }
        //                        }
        //                    }
        //                }
        //            }

        //            /// uncheck all items
        //            foreach (GridViewRowInfo oGridRow in this.radGridViewWBS.Rows)
        //            {
        //                this.radGridViewWBS.Rows[oGridRow.Index].Cells["Check"].Value = false;
        //            }
        //            /// up to here
        //            foreach (string sWBS in this._WBSColl[oSiteDTO])
        //            {
        //                try
        //                {
        //                    var rows = from GridViewRowInfo row in this.radGridViewWBS.Rows
        //                               where (row.Cells["WBS"].Value.ToString() == sWBS)
        //                               select row;
        //                    foreach (var row in rows)
        //                    {
        //                        this.radGridViewWBS.Rows[(row as GridViewRowInfo).Index].Cells["Check"].Value = true;
        //                    }
        //                }
        //                catch { }
        //            }
        //            this.radGridViewWBS.Tag = oSiteDTO;
        //        }
        //        finally
        //        {
        //            UserInfo.SiteID = iSiteID.ToString();
        //        }
        //    }
        //    else
        //    {
        //        this.radGridViewWBS.Tag = null;
        //    }
        //}

		/// <summary>
		/// save setting data
		/// </summary>
		/// <author>humkyung</author>
		/// <date>2016.09.01</date>
		public override void SaveData()
		{
			base.SaveData();

			if (this.radDropDownListProject.SelectedIndex < 0) return;

			try
			{
                using (ISmartISOService svcProxy = ServiceFactory.CreateServiceChannel<ISmartISOService>(AppDocData.ServiceURL))
                {
                    using (DataSet dsParam = svcProxy.GetCommandParam("SmartISO.BSL", "SaveSiteListCommand"))
                    {
                        svcProxy.ExecuteCommand("SmartISO.BSL", "SaveSiteListCommand", (this.radGridViewSiteList.DataSource as DataTable).DataSet);
                    }

                    using (DataSet dsParam = svcProxy.GetCommandParam("SmartISO.BSL", "SaveProjectMemberListCommand"))
                    {
                        svcProxy.ExecuteCommand("SmartISO.BSL", "SaveProjectMemberListCommand", (this.radGridViewMembers.DataSource as DataTable).DataSet);
                    }

                    (this.radGridViewMembers.DataSource as DataTable).DataSet.AcceptChanges();
                }
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
                LoadData();
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
                try
                {
                    RadFormCopySetting frm = new RadFormCopySetting(ProjectUID, SiteUID);
                    frm.ShowDialog(this);
                }
                catch (Exception ex)
                {
                    new TechSun.Theme.frmWarning(ex.Message).ShowDialog(this);
                }
                finally
                {
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
                //List<int> oCheckedItemList = this.CheckedSiteList.ToList();
                //oCheckedItemList.Reverse();
                //foreach (int iRowIndex in oCheckedItemList)
                //{
                //    SiteDTO dto = radGridViewSiteList.Rows[iRowIndex].Tag as SiteDTO;
                //    if (null != dto) dto.DeleteYN = true;
                //    radGridViewSiteList.Rows.RemoveAt(iRowIndex);
                //}
			}
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
				if ((oCtrl is RadLabel) || (oCtrl is RadGridView)) continue;
				if (oCtrl == this.radDropDownListProject) continue;
				if (oCtrl == this.radGroupBoxSite)
				{
					this.radGroupBoxSite.Enabled = bEnabled;
				}
			}
			this.kRadFreeButtonExportDatabase.Enabled = bEnabled;
			this.kRadFreeButtonImportDatabase.Enabled = bEnabled;
		}

		/// <summary>
		/// display Project Member of selected site
		/// </summary>
		/// <author>humkyung</author>
		/// <date>2017.05.22</date>
		/// <param name="sender"></param>
		/// <param name="e"></param>
		private void radGridViewSiteList_SelectionChanged(object sender, EventArgs e)
		{
            if(1 == this.radGridViewSiteList.SelectedRows.Count)
            {
                Int64 iSiteUID = Convert.ToInt64(this.radGridViewSiteList.SelectedRows[0].Cells["UID"].Value.ToString());
                using (ISmartISOService svcProxy = ServiceFactory.CreateServiceChannel<ISmartISOService>(AppDocData.ServiceURL))
                {
                    using (DataSet dsParam = svcProxy.GetCommandParam("SmartISO.BSL", "GetProjectMemberListCommand"))
                    {
                        DataRow dr = dsParam.Tables["Params"].NewRow();
                        {
                            dr["Site_UID"] = iSiteUID;
                        }
                        dsParam.Tables["Params"].Rows.Add(dr);

                        using (DataSet ds = svcProxy.ExecuteCommand("SmartISO.BSL", "GetProjectMemberListCommand", dsParam))
                        {
                            if (ds.Tables.Count > 0)
                            {
                                this.radGridViewMembers.DataSource = ds.Tables[0];
                                {
                                    this.radGridViewMembers.Columns["UID"].IsVisible = false;
                                    this.radGridViewMembers.Columns["Site_UID"].IsVisible = false;
                                }
                            }
                        }
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

            //SiteDTO dto = this.radGridViewSiteList.Rows[e.RowIndex].Tag as SiteDTO;
            //if (null != dto)
            //{
            //    string sColName = this.radGridViewSiteList.Columns[e.ColumnIndex].Name;
            //    object oValue = this.radGridViewSiteList.Rows[e.RowIndex].Cells[e.ColumnIndex].Value;
            //    string sValue = (null != oValue) ? oValue.ToString() : string.Empty;
            //    switch(sColName)
            //    {
            //        case "Name":
            //            dto.Name = sValue;
            //            break;
            //        case "Desc":
            //            dto.Desc = sValue;
            //            break;
            //    }
            //}
		}

		private void radGridViewWBS_CellEndEdit(object sender, GridViewCellEventArgs e)
		{
            //SiteDTO oSiteDTO = this.radGridViewWBS.Tag as SiteDTO;
            //if (null != oSiteDTO)
            //{
            //    if (!this._WBSColl.ContainsKey(oSiteDTO)) this._WBSColl[oSiteDTO] = new List<string>();
            //    this._WBSColl[oSiteDTO].Clear();

            //    foreach (GridViewRowInfo oGridRow in this.radGridViewWBS.Rows)
            //    {
            //        object oValue = this.radGridViewWBS.Rows[oGridRow.Index].Cells["Check"].Value;
            //        if ((null == oValue) || (false.ToString() == oValue.ToString())) continue;
            //        this._WBSColl[oSiteDTO].Add(this.radGridViewWBS.Rows[oGridRow.Index].Cells["WBS"].Value.ToString());
            //    }
            //}
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
                //using (ISmartISOService svcProxy = ServiceFactory.CreateServiceChannel<ISmartISOService>(AppDocData.ServiceURL))
                //{
                //    using (DataSet dsParam = svcProxy.GetCommandParam("IDCS.BSL", "ExportTablesCommand"))
                //    {
                //        using (DataSet res = svcProxy.ExecuteCommand("IDCS.BSL", "ExportTablesCommand", dsParam))
                //        {
                //            SaveFileDialog oFileDialog = new SaveFileDialog();
                //            oFileDialog.FileName = DateTime.Now.ToString("yyyyMMdd") + ".xml";
                //            oFileDialog.DefaultExt = "xml";
                //            oFileDialog.Filter = "XML(*.xml)|*.xml|All Files(*.*)|*.*";
                //            oFileDialog.FilterIndex = 0;
                //            if (System.Windows.Forms.DialogResult.OK == oFileDialog.ShowDialog())
                //            {
                //                res.WriteXmlSchema(System.IO.Path.Combine(System.IO.Path.GetDirectoryName(oFileDialog.FileName), System.IO.Path.GetFileNameWithoutExtension(oFileDialog.FileName) + ".xsd"));
                //                res.WriteXml(oFileDialog.FileName);
                //            }
                //        }
                //    }
                //}
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

                                //using (ISmartISOService svcProxy = ServiceFactory.CreateServiceChannel<ISmartISOService>(AppDocData.ServiceURL))
                                //{
                                //    svcProxy.ExecuteCommand("IDCS.BSL", "ImportTablesCommand", dsParam);
                                //}
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

        private void kRadPlusImageButtonNewMember_Click(object sender, EventArgs e)
        {
            try
            {
                GridViewRowInfo oGridRow = this.radGridViewMembers.Rows.AddNew();
                {
                    oGridRow.Cells["Site_UID"].Value = SiteUID;
                }
            }
            catch { }
        }

        private void kRadCloseImageButtonDeleteMember_Click(object sender, EventArgs e)
        {
            Cursor.Current = Cursors.WaitCursor;
            try
            {
                if (DialogResult.No == new TechSun.Theme.frmInformationYesNo("Do you want to delete checked members").ShowDialog(this)) return;

                using (ISmartISOService svcProxy = ServiceFactory.CreateServiceChannel<ISmartISOService>(AppDocData.ServiceURL))
                {
                    using (DataSet dsParam = svcProxy.GetCommandParam("SmartISO.BSL", "DeleteProjectMemberListCommand"))
                    {
                        foreach (int index in CheckedMembers)
                        {
                            Int64 iUID = Convert.ToInt64(this.radGridViewMembers.Rows[index].Cells["UID"].Value.ToString());
                            DataRow dr = dsParam.Tables["Params"].NewRow();
                            {
                                dr["UID"] = iUID;
                            }
                            dsParam.Tables["Params"].Rows.Add(dr);
                        }
                        svcProxy.ExecuteCommand("SmartISO.BSL", "DeleteProjectMemberListCommand", dsParam);
                    }
                }

                this.radGridViewMembers.BeginUpdate();
                foreach (int index in CheckedMembers.Reverse())
                {
                    this.radGridViewMembers.Rows.RemoveAt(index);
                }
                this.radGridViewMembers.EndUpdate();

                (this.radGridViewMembers.DataSource as DataTable).DataSet.AcceptChanges();
            }
            catch (Exception ex)
            {
                new TechSun.Theme.frmWarning(ex.Message).ShowDialog();
            }
            finally
            {
                Cursor.Current = Cursors.Default;
            }
        }
	}
}
