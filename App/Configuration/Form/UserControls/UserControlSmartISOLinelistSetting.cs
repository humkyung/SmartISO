using System;
using System.Collections.Generic;
using System.Linq;
using System.Windows.Forms;
using Configuration.Form.RadForm;
using Configuration.Properties;
using System.Data;
using System.Xml;
using System.Configuration;

using SmartISO.Framework.Service;
using SmartISO.Service.Interface;
using Telerik.WinControls.UI;

using GemBox.Spreadsheet;
using System.ComponentModel;
using Telerik.WinControls;

namespace Configuration
{
	public partial class UserControlSmartISOLinelistSetting : ThemeChangeable
	{
		public const string DisplayName = "Line List";
        private List<string> KeyCols = new List<string>() { "UID", "PROJECT_UID" };

        private Int64 ProjectUID
        {
            get{ return Convert.ToInt64(this.radDropDownListProject.Items[this.radDropDownListProject.SelectedIndex].Tag); }
        }

        public IEnumerable<Int64> CheckedItems
        {
            get
            {
                var rows = (from GridViewRowInfo row in this.radGridViewCurrentLineList.Rows
                            where ((null != row.Cells["Check"].Value) && (true.ToString() == row.Cells["Check"].Value.ToString()))
                            select Convert.ToInt64(row.Cells["UID"].Value));
                return rows;
            }
        }

        public IEnumerable<int> CheckedIndices
        {
            get
            {
                var rows = (from GridViewRowInfo row in this.radGridViewCurrentLineList.Rows
                            where ((null != row.Cells["Check"].Value) && (true.ToString() == row.Cells["Check"].Value.ToString()))
                            select row.Index);
                return rows;
            }
        }

		public UserControlSmartISOLinelistSetting()
		{
			InitializeComponent();

            this.Disposed += new EventHandler(UserControlSmartISOLinelistSetting_Disposed);
			Program.OnIdleUpdate += new Program.IdleUpdateEvent(OnIdleUpdate);
			ApplyTheme(this.Controls);
		}

        void UserControlSmartISOLinelistSetting_Disposed(object sender, EventArgs e)
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
        /// update grid columns(set read only for key columns)
        /// </summary>
        /// <author>humkyung</author>
        /// <date>2017.05.20</date>
        private void UpdateGridColumns()
        {
            using (ISmartISOService svcProxy = ServiceFactory.CreateServiceChannel<ISmartISOService>(AppDocData.ServiceURL))
            {
                /// get LineList Key for project - 2017.05.15 added by humkyung
                using (DataSet dsParam = svcProxy.GetCommandParam("SmartISO.BSL", "GetConfigCommand"))
                {
                    DataRow drParm = dsParam.Tables["Params"].NewRow();
                    {
                        drParm["Ref_UID"] = ProjectUID;
                        drParm["Section"] = "LineList";
                        drParm["Key"] = "Key";
                    }
                    dsParam.Tables["Params"].Rows.Add(drParm);

                    using (DataSet ds = svcProxy.ExecuteCommand("SmartISO.BSL", "GetConfigCommand", dsParam))
                    {
                        string[] oTokens = ds.Tables[0].Rows[0]["Value"].ToString().Split('-');

                        foreach (GridViewColumn oGridCol in this.radGridViewCurrentLineList.Columns) oGridCol.ReadOnly = false;
                        foreach (string sToken in oTokens)
                        {
                            if (this.radGridViewCurrentLineList.Columns.Contains(sToken))
                                this.radGridViewCurrentLineList.Columns[sToken].ReadOnly = true;
                        }
                    }
                }
                /// up to here
            }
        }

        private void SetGridRowTagWithDataTable()
        {
            foreach (GridViewRowInfo oGridRow in this.radGridViewCurrentLineList.Rows)
            {
                oGridRow.Tag = new List<object>((this.radGridViewCurrentLineList.DataSource as DataTable).Rows[oGridRow.Index].ItemArray);
            }
        }

		/// <summary>
		/// load and display site setting
		/// </summary>
		/// <author>humkyung</author>
		/// <date>2016.08.11</date>
		public override void LoadData()
		{
            if (this.radDropDownListProject.SelectedIndex < 0) return;

            try
            {
                using (ISmartISOService svcProxy = ServiceFactory.CreateServiceChannel<ISmartISOService>(AppDocData.ServiceURL))
                {
                    using (DataSet dsParam = svcProxy.GetCommandParam("SmartISO.BSL", "GetAllLineListCommand"))
                    {
                        DataRow dr = dsParam.Tables["Params"].NewRow();
                        {
                            dr["Project_UID"] = ProjectUID;
                        }
                        dsParam.Tables["Params"].Rows.Add(dr);

                        using (DataSet ds = svcProxy.ExecuteCommand("SmartISO.BSL", "GetAllLineListCommand", dsParam))
                        {
                            this.radGridViewCurrentLineList.DataSource = ds.Tables[0];
                            foreach (string sColName in this.KeyCols)
                            {
                                if (this.radGridViewCurrentLineList.Columns.Contains(sColName)) this.radGridViewCurrentLineList.Columns[sColName].IsVisible = false;
                            }
                            this.radGridViewCurrentLineList.BestFitColumns();

                            SetGridRowTagWithDataTable();
                        }
                    }
                }

                UpdateGridColumns();
            }
            catch (Exception ex)
            {
                new TechSun.Theme.frmWarning(ex.Message).ShowDialog(this);
            }
            finally
            {
            }
		}

		/// <summary>
		/// save line list modified by user
		/// </summary>
		/// <author>humkyung</author>
		/// <date>2017.05.20</date>
		public override void SaveData()
		{
            Cursor.Current = Cursors.WaitCursor;
            try
            {
                using (ISmartISOService svcProxy = ServiceFactory.CreateServiceChannel<ISmartISOService>(AppDocData.ServiceURL))
                {
                    svcProxy.ExecuteCommand("SmartISO.BSL", "UpdateLineListCommand", (this.radGridViewCurrentLineList.DataSource as DataTable).DataSet);
                }

                SetGridRowTagWithDataTable();
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

		/// <summary>
		/// Import linelist
		/// </summary>
		/// <author>humkyung</author>
		/// <param name="sender"></param>
		/// <param name="e"></param>
		private void radButtonImport_Click(object sender, EventArgs e)
		{
            try
            {
                Configuration.Form.RadForm.RadFormImportLineList frm = new RadFormImportLineList()
                {
                    ProjectUID = Convert.ToInt64(this.radDropDownListProject.Items[this.radDropDownListProject.SelectedIndex].Tag)
                };
                if (DialogResult.OK == frm.ShowDialog(this))
                {
                }
            }
            catch (Exception ex)
            {
                new TechSun.Theme.frmWarning(ex.Message).ShowDialog(this);
            }
            finally
            {
                LoadData();
            }
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
		/// update enabled property of control
		/// </summary>
		/// <author>humkyung</author>
		/// <date>2017.05.15</date>
		public void OnIdleUpdate()
		{
            bool bPrjSelected = (-1 != this.radDropDownListProject.SelectedIndex);
            this.radGroupBoxCurrentLineList.Enabled = bPrjSelected;
            this.radButtonImport.Enabled = bPrjSelected;
            this.kRadFreeButtonDelete.Enabled = (CheckedItems.Count() > 0);
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
            //if (UserInfo.Tool == "SP3D")
            //{ 
            //    if (1 == this.radGridViewSiteList.SelectedRows.Count)
            //    {
            //        int iSiteID = 0;
            //        int.TryParse(UserInfo.SiteID,out iSiteID);
            //        try
            //        {
            //            SiteDTO oSiteDTO = this.radGridViewSiteList.SelectedRows[0].Tag as SiteDTO;
            //            if ((null != oSiteDTO) && (-1 != oSiteDTO.Id))
            //            {
            //                LoadEmailSetting(oSiteDTO.Id);
            //            }
            //            else
            //            {
            //            }

            //            LoadSiteData(oSiteDTO);
            //        }
            //        finally
            //        {
            //            UserInfo.SiteID = iSiteID.ToString();
            //        }
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

        }

        private void radGridViewCurrentLineList_ViewCellFormatting(object sender, CellFormattingEventArgs e)
        {
            if (e.ColumnIndex < 0 || e.RowIndex < 0 || ("Check" == this.radGridViewCurrentLineList.Columns[e.ColumnIndex].Name)) return;

            if (this.radGridViewCurrentLineList.Columns[e.ColumnIndex].ReadOnly)
            {
                e.CellElement.DrawFill = true;
                e.CellElement.NumberOfColors = 1;
                e.CellElement.BackColor = System.Drawing.Color.LightGray;
            }
            else if ((null != this.radGridViewCurrentLineList.Rows[e.RowIndex].Tag ) && 
                (this.radGridViewCurrentLineList.Rows[e.RowIndex].Cells[e.ColumnIndex].Value.ToString() != (this.radGridViewCurrentLineList.Rows[e.RowIndex].Tag as List<object>)[e.ColumnIndex - 1].ToString()))
            {
                e.CellElement.DrawFill = true;
                e.CellElement.NumberOfColors = 1;
                e.CellElement.BackColor = System.Drawing.Color.Yellow;
            }
            else
            {
                e.CellElement.ResetValue(LightVisualElement.DrawFillProperty, ValueResetFlags.All);
                e.CellElement.ResetValue(LightVisualElement.ForeColorProperty, ValueResetFlags.All);
                e.CellElement.ResetValue(LightVisualElement.NumberOfColorsProperty, ValueResetFlags.All);
                e.CellElement.ResetValue(LightVisualElement.BackColorProperty, ValueResetFlags.All);
            }
        }

        private void kRadFreeButtonDelete_Click(object sender, EventArgs e)
        {
            Cursor.Current = Cursors.WaitCursor;
            try
            {
                using (ISmartISOService svcProxy = ServiceFactory.CreateServiceChannel<ISmartISOService>(AppDocData.ServiceURL))
                {
                    using (DataSet dsParam = svcProxy.GetCommandParam("SmartISO.BSL", "DeleteLineListCommand"))
                    {
                        foreach (Int64 uid in CheckedItems)
                        {
                            DataRow dr = dsParam.Tables["Params"].NewRow();
                            {
                                dr["UID"] = uid;
                            }
                            dsParam.Tables["Params"].Rows.Add(dr);
                        }
                        svcProxy.ExecuteCommand("SmartISO.BSL", "DeleteLineListCommand", dsParam);
                    }
                }

                this.radGridViewCurrentLineList.BeginUpdate();
                foreach(int index in CheckedIndices.Reverse())
                {
                    this.radGridViewCurrentLineList.Rows.RemoveAt(index);
                }
                this.radGridViewCurrentLineList.EndUpdate();
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