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

namespace Configuration
{
	public partial class UserControlBorderSettingData : ThemeChangeable
	{
		public string HeaderText { get; set; }
        private Int64 ProjectUID{get;set;}

		public UserControlBorderSettingData()
		{
			InitializeComponent();
			ApplyTheme(this.Controls);
		}

		private void UserControlBorderSettingData_Load(object sender, EventArgs e)
		{
			radGroupBoxBorderSettingData.Text = HeaderText;

			if ("Iso Dwg Area" == HeaderText)
			{
				radGridViewBorderSettingData.RowCount = 3;

				radGridViewBorderSettingData.Columns.Insert(1, new Telerik.WinControls.UI.GridViewTextBoxColumn() { Name = "MinX", HeaderText = "MinX" });
				radGridViewBorderSettingData.Columns.Insert(2, new Telerik.WinControls.UI.GridViewTextBoxColumn() { Name = "MinY", HeaderText = "MinY" });
				radGridViewBorderSettingData.Columns.Insert(3, new Telerik.WinControls.UI.GridViewTextBoxColumn() { Name = "MaxX", HeaderText = "MaxX" });
				radGridViewBorderSettingData.Columns.Insert(4, new Telerik.WinControls.UI.GridViewTextBoxColumn() { Name = "MaxY", HeaderText = "MaxY" });
                radGridViewBorderSettingData.Columns.Insert(5, new Telerik.WinControls.UI.GridViewCommandColumn() { Name = "Area", HeaderText = "...", Width = 26 });

				radGridViewBorderSettingData.Rows[0].Cells[0].Value = "BORDER_AREA";
				radGridViewBorderSettingData.Rows[0].Cells[0].ReadOnly = true;

				radGridViewBorderSettingData.Rows[1].Cells[0].Value = "DRAWING_AREA";
				radGridViewBorderSettingData.Rows[1].Cells[0].ReadOnly = true;

				radGridViewBorderSettingData.Rows[2].Cells[0].Value = "BM_AREA";
				radGridViewBorderSettingData.Rows[2].Cells[0].ReadOnly = true;

				Telerik.WinControls.UI.GridViewCommandColumn oCommandColumn = radGridViewBorderSettingData.Columns[5] as Telerik.WinControls.UI.GridViewCommandColumn;
                //oCommandColumn.Image = Properties.Resources.selection;
			}
			else if ("Line Data Location" == HeaderText)
			{
				radGridViewBorderSettingData.Columns.Insert(1, new Telerik.WinControls.UI.GridViewTextBoxColumn() { Name = "X", HeaderText = "X" });
				radGridViewBorderSettingData.Columns.Insert(2, new Telerik.WinControls.UI.GridViewTextBoxColumn() { Name = "Y", HeaderText = "Y" });
                radGridViewBorderSettingData.Columns.Insert(3, new Telerik.WinControls.UI.GridViewCommandColumn() { Name = "PickPoint", HeaderText="..." });

                //Telerik.WinControls.UI.GridViewCommandColumn oCommandColumn = radGridViewBorderSettingData.Columns[3] as Telerik.WinControls.UI.GridViewCommandColumn;
                //oCommandColumn.Image = Properties.Resources.pick;

                radGridViewBorderSettingData.Columns.Insert(4, new Telerik.WinControls.UI.GridViewTextBoxColumn() { Name = "MinX", HeaderText = "MinX" });
                radGridViewBorderSettingData.Columns.Insert(5, new Telerik.WinControls.UI.GridViewTextBoxColumn() { Name = "MinY", HeaderText = "MinY" });
                radGridViewBorderSettingData.Columns.Insert(6, new Telerik.WinControls.UI.GridViewTextBoxColumn() { Name = "MaxX", HeaderText = "MaxX" });
                radGridViewBorderSettingData.Columns.Insert(7, new Telerik.WinControls.UI.GridViewTextBoxColumn() { Name = "MaxY", HeaderText = "MaxY" });
                radGridViewBorderSettingData.Columns.Insert(8, new Telerik.WinControls.UI.GridViewCommandColumn() { Name = "SelectBounds", HeaderText = "..."});
                //Telerik.WinControls.UI.GridViewCommandColumn oCommandColumn = radGridViewBorderSettingData.Columns[5] as Telerik.WinControls.UI.GridViewCommandColumn;
			}
			else if("Line Data Area" == HeaderText)
			{
				radGridViewBorderSettingData.Columns.Insert(1, new Telerik.WinControls.UI.GridViewTextBoxColumn() { Name = "MinX", HeaderText = "MinX" });
				radGridViewBorderSettingData.Columns.Insert(2, new Telerik.WinControls.UI.GridViewTextBoxColumn() { Name = "MinY", HeaderText = "MinY" });
				radGridViewBorderSettingData.Columns.Insert(3, new Telerik.WinControls.UI.GridViewTextBoxColumn() { Name = "MaxX", HeaderText = "MaxX" });
				radGridViewBorderSettingData.Columns.Insert(4, new Telerik.WinControls.UI.GridViewTextBoxColumn() { Name = "MaxY", HeaderText = "MaxY" });

				Telerik.WinControls.UI.GridViewCommandColumn oCommandColumn = radGridViewBorderSettingData.Columns[5] as Telerik.WinControls.UI.GridViewCommandColumn;
                //oCommandColumn.Image = Properties.Resources.selection;
			}
		}

		public override void LoadData()
		{
            //if (string.IsNullOrEmpty(UserInfo.ProjectCode)) return;

            using (ISmartISOService svcProxy = ServiceFactory.CreateServiceChannel<ISmartISOService>(AppDocData.ServiceURL))
            {
                using (System.Data.DataSet dsParam = svcProxy.GetCommandParam("SmartISO.BSL", "GetConfigCommand"))
                {
                    if ("Iso Dwg Area" == HeaderText)
                    {
                        #region clear controls value
                        foreach (Telerik.WinControls.UI.GridViewRowInfo oGridRow in radGridViewBorderSettingData.Rows)
                        {
                            for (int iCol = 1; iCol < oGridRow.Cells.Count; ++iCol)
                            {
                                radGridViewBorderSettingData.Rows[oGridRow.Index].Cells[iCol].Value = string.Empty;
                            }
                        }
                        #endregion

                        DataRow dr = dsParam.Tables["Params"].NewRow();
                        dr["Ref_UID"] = ProjectUID;
                        dr["Section"] = HeaderText;
                        dr["Key"] = "BORDER_AREA";
                        dsParam.Tables["Params"].Rows.Add(dr);
                        using (DataSet ds = svcProxy.ExecuteCommand("SmartISO.BSL", "GetConfigCommand", dsParam))
                        {
                            foreach (DataRow oRow in ds.Tables[0].Rows)
                            {
                                string[] oTokens = oRow["Value"].ToString().Split(',');
                                if (4 == oTokens.Length)
                                {
                                    radGridViewBorderSettingData.Rows[0].Cells["MinX"].Value = oTokens[0];
                                    radGridViewBorderSettingData.Rows[0].Cells["MinY"].Value = oTokens[1];
                                    radGridViewBorderSettingData.Rows[0].Cells["MaxX"].Value = oTokens[2];
                                    radGridViewBorderSettingData.Rows[0].Cells["MaxY"].Value = oTokens[3];
                                }
                            }
                        }

                        dsParam.Tables["Params"].Rows.Clear();
                        dr = dsParam.Tables["Params"].NewRow();
                        dr["Ref_UID"] = ProjectUID;
                        dr["Section"] = HeaderText;
                        dr["Key"] = "DRAWING_AREA";
                        dsParam.Tables["Params"].Rows.Add(dr);
                        using (DataSet ds = svcProxy.ExecuteCommand("SmartISO.BSL", "GetConfigCommand", dsParam))
                        {
                            foreach (DataRow oRow in ds.Tables[0].Rows)
                            {
                                string[] oTokens = oRow["Value"].ToString().Split(',');
                                if (4 == oTokens.Length)
                                {
                                    radGridViewBorderSettingData.Rows[1].Cells["MinX"].Value = oTokens[0];
                                    radGridViewBorderSettingData.Rows[1].Cells["MinY"].Value = oTokens[1];
                                    radGridViewBorderSettingData.Rows[1].Cells["MaxX"].Value = oTokens[2];
                                    radGridViewBorderSettingData.Rows[1].Cells["MaxY"].Value = oTokens[3];
                                }
                            }
                        }

                        dsParam.Tables["Params"].Rows.Clear();
                        dr = dsParam.Tables["Params"].NewRow();
                        dr["Ref_UID"] = ProjectUID;
                        dr["Section"] = HeaderText;
                        dr["Key"] = "BM_AREA";
                        dsParam.Tables["Params"].Rows.Add(dr);
                        using (DataSet ds = svcProxy.ExecuteCommand("SmartISO.BSL", "GetConfigCommand", dsParam))
                        {
                            foreach (DataRow oRow in ds.Tables[0].Rows)
                            {
                                string[] oTokens = oRow["Value"].ToString().Split(',');
                                if (4 == oTokens.Length)
                                {
                                    radGridViewBorderSettingData.Rows[2].Cells["MinX"].Value = oTokens[0];
                                    radGridViewBorderSettingData.Rows[2].Cells["MinY"].Value = oTokens[1];
                                    radGridViewBorderSettingData.Rows[2].Cells["MaxX"].Value = oTokens[2];
                                    radGridViewBorderSettingData.Rows[2].Cells["MaxY"].Value = oTokens[3];
                                }
                            }
                        }
                    }
                    else if ("Line Data Location" == HeaderText)
                    {
                        this.radGridViewBorderSettingData.Rows.Clear();

                        using (DataSet ds = svcProxy.ExecuteCommand("SmartISO.BSL", "GetLineDataColInfoListCommand", null))
                        {
                            this.radGridViewBorderSettingData.RowCount = ds.Tables[0].Rows.Count;

                            int iRow = 0;
                            foreach (DataRow oRow in ds.Tables[0].Rows)
                            {
                                string sName = oRow["Name"].ToString();
                                if (!string.IsNullOrEmpty(sName))
                                {
                                    this.radGridViewBorderSettingData.Rows[iRow++].Cells["Name"].Value = sName;
                                }
                            }
                        }

                        //List<ColInfoDTO> oLineDataCols = svcProxy.GetLineDataColInfoList();
                        //radGridViewBorderSettingData.RowCount = oLineDataCols.Count;
                        //foreach (Telerik.WinControls.UI.GridViewRowInfo oGridRow in radGridViewBorderSettingData.Rows)
                        //{
                        //    radGridViewBorderSettingData.Rows[oGridRow.Index].Cells[0].Value = oLineDataCols[oGridRow.Index].Name;
                        //    radGridViewBorderSettingData.Rows[oGridRow.Index].Cells[0].ReadOnly = true;
                        //}

                        dsParam.Tables["Params"].Rows.Clear();
                        DataRow dr = dsParam.Tables["Params"].NewRow();
                        dr["Ref_UID"] = ProjectUID;
                        dr["Section"] = HeaderText;
                        dr["Key"] = string.Empty;
                        dsParam.Tables["Params"].Rows.Add(dr);
                        using (DataSet ds = svcProxy.ExecuteCommand("SmartISO.BSL", "GetConfigCommand", dsParam))
                        {
                            foreach (Telerik.WinControls.UI.GridViewRowInfo oGridRow in radGridViewBorderSettingData.Rows)
                            {
                                DataRow[] oRows = ds.Tables[0].Select(string.Format("Key='{0}'", radGridViewBorderSettingData.Rows[oGridRow.Index].Cells["Name"].Value.ToString()));
                                if (1 == oRows.Length)
                                {
                                    string[] oTokens = oRows[0]["Value"].ToString().Split(',');
                                    if (6 == oTokens.Length)
                                    {
                                        this.radGridViewBorderSettingData.Rows[oGridRow.Index].Cells["X"].Value = oTokens[0];
                                        this.radGridViewBorderSettingData.Rows[oGridRow.Index].Cells["Y"].Value = oTokens[1];
                                        this.radGridViewBorderSettingData.Rows[oGridRow.Index].Cells["MinX"].Value = oTokens[2];
                                        this.radGridViewBorderSettingData.Rows[oGridRow.Index].Cells["MinY"].Value = oTokens[3];
                                        this.radGridViewBorderSettingData.Rows[oGridRow.Index].Cells["MaxX"].Value = oTokens[4];
                                        this.radGridViewBorderSettingData.Rows[oGridRow.Index].Cells["MaxY"].Value = oTokens[5];
                                    }
                                }
                            }
                        }

                        this.radGridViewBorderSettingData.BestFitColumns();
                    }
                    else if ("Line Data Area" == HeaderText)
                    {
                        radGridViewBorderSettingData.Rows.Clear();

                        //List<ColInfoDTO> oLineDataCols = svcProxy.GetLineDataColInfoList();
                        //radGridViewBorderSettingData.RowCount = oLineDataCols.Count;
                        //foreach (Telerik.WinControls.UI.GridViewRowInfo oGridRow in radGridViewBorderSettingData.Rows)
                        //{
                        //    radGridViewBorderSettingData.Rows[oGridRow.Index].Cells[0].Value = oLineDataCols[oGridRow.Index].Name;
                        //    radGridViewBorderSettingData.Rows[oGridRow.Index].Cells[0].ReadOnly = true;
                        //}

                        dsParam.Tables["Params"].Rows.Clear();
                        DataRow dr = dsParam.Tables["Params"].NewRow();
                        dr["Ref_UID"] = ProjectUID;
                        dr["Section"] = HeaderText;
                        dr["Key"] = string.Empty;
                        dsParam.Tables["Params"].Rows.Add(dr);
                        using (DataSet ds = svcProxy.ExecuteCommand("SmartISO.BSL", "GetConfigCommand", dsParam))
                        {
                            foreach (Telerik.WinControls.UI.GridViewRowInfo oGridRow in radGridViewBorderSettingData.Rows)
                            {
                                DataRow[] oRows = ds.Tables[0].Select(string.Format("key='{0}'", radGridViewBorderSettingData.Rows[oGridRow.Index].Cells["Name"].Value.ToString()));
                                if (1 == oRows.Length)
                                {
                                    string[] oTokens = oRows[0]["Value"].ToString().Split(',');
                                    if (4 == oTokens.Length)
                                    {
                                        radGridViewBorderSettingData.Rows[oGridRow.Index].Cells["MinX"].Value = oTokens[0];
                                        radGridViewBorderSettingData.Rows[oGridRow.Index].Cells["MinY"].Value = oTokens[1];
                                        radGridViewBorderSettingData.Rows[oGridRow.Index].Cells["MaxX"].Value = oTokens[2];
                                        radGridViewBorderSettingData.Rows[oGridRow.Index].Cells["MaxY"].Value = oTokens[3];
                                    }
                                }
                            }
                        }

                        this.radGridViewBorderSettingData.BestFitColumns();
                    }
                }
            }
		}

		public override void SaveData()
		{
            try
            {
                using (ISmartISOService svcProxy = ServiceFactory.CreateServiceChannel<ISmartISOService>(AppDocData.ServiceURL))
                {
                    using (System.Data.DataSet dsParam = svcProxy.GetCommandParam("SmartISO.BSL", "SaveConfigsCommand"))
                    {
                        if ("Iso Dwg Area" == HeaderText)
                        {
                            foreach (Telerik.WinControls.UI.GridViewRowInfo oGridRow in radGridViewBorderSettingData.Rows)
                            {
                                string sValue = string.Empty;
                                sValue = (null != radGridViewBorderSettingData.Rows[oGridRow.Index].Cells["MinX"].Value) ? radGridViewBorderSettingData.Rows[oGridRow.Index].Cells["MinX"].Value.ToString() : string.Empty;
                                sValue += "," + ((null != radGridViewBorderSettingData.Rows[oGridRow.Index].Cells["MinY"].Value) ? radGridViewBorderSettingData.Rows[oGridRow.Index].Cells["MinY"].Value.ToString() : string.Empty);
                                sValue += "," + ((null != radGridViewBorderSettingData.Rows[oGridRow.Index].Cells["MaxX"].Value) ? radGridViewBorderSettingData.Rows[oGridRow.Index].Cells["MaxX"].Value.ToString() : string.Empty);
                                sValue += "," + ((null != radGridViewBorderSettingData.Rows[oGridRow.Index].Cells["MaxY"].Value) ? radGridViewBorderSettingData.Rows[oGridRow.Index].Cells["MaxY"].Value.ToString() : string.Empty);

                                DataRow dr = dsParam.Tables["Params"].NewRow();
                                dr["Ref_UID"] = ProjectUID;
                                dr["Section"] = HeaderText;
                                dr["Key"] = this.radGridViewBorderSettingData.Rows[oGridRow.Index].Cells[0].Value.ToString();
                                dr["Value"] = sValue;
                                dsParam.Tables["Params"].Rows.Add(dr);
                            }
                        }
                        else if("Line Data Location" == HeaderText)
                        {
                            foreach (Telerik.WinControls.UI.GridViewRowInfo oGridRow in radGridViewBorderSettingData.Rows)
                            {
                                string sValue = string.Empty;
                                sValue = (null != radGridViewBorderSettingData.Rows[oGridRow.Index].Cells["X"].Value) ? radGridViewBorderSettingData.Rows[oGridRow.Index].Cells["X"].Value.ToString() : string.Empty;
                                sValue += "," + ((null != radGridViewBorderSettingData.Rows[oGridRow.Index].Cells["Y"].Value) ? radGridViewBorderSettingData.Rows[oGridRow.Index].Cells["Y"].Value.ToString() : string.Empty);
                                sValue += "," + ((null != radGridViewBorderSettingData.Rows[oGridRow.Index].Cells["MinX"].Value) ? radGridViewBorderSettingData.Rows[oGridRow.Index].Cells["MinX"].Value.ToString() : string.Empty);
                                sValue += "," + ((null != radGridViewBorderSettingData.Rows[oGridRow.Index].Cells["MinY"].Value) ? radGridViewBorderSettingData.Rows[oGridRow.Index].Cells["MinY"].Value.ToString() : string.Empty);
                                sValue += "," + ((null != radGridViewBorderSettingData.Rows[oGridRow.Index].Cells["MaxX"].Value) ? radGridViewBorderSettingData.Rows[oGridRow.Index].Cells["MaxX"].Value.ToString() : string.Empty);
                                sValue += "," + ((null != radGridViewBorderSettingData.Rows[oGridRow.Index].Cells["MaxY"].Value) ? radGridViewBorderSettingData.Rows[oGridRow.Index].Cells["MaxY"].Value.ToString() : string.Empty);

                                DataRow dr = dsParam.Tables["Params"].NewRow();
                                dr["Ref_UID"] = ProjectUID;
                                dr["Section"] = HeaderText;
                                dr["Key"] = this.radGridViewBorderSettingData.Rows[oGridRow.Index].Cells[0].Value.ToString();
                                dr["Value"] = sValue;
                                dsParam.Tables["Params"].Rows.Add(dr);
                            }
                        }

                        svcProxy.ExecuteCommand("SmartISO.BSL", "SaveConfigsCommand", dsParam);
                    }
                }
            }
            catch (Exception ex)
            {
                new TechSun.Theme.frmWarning(ex.Message).ShowDialog(this);
            }
		}

		/// <summary>
		/// write border setting to file
		/// </summary>
		/// <author>humkyung</author>
		/// <date>2016.07.12</date>
		/// <param name="oStreamWriter"></param>
		public void Write(StreamWriter oStreamWriter)
		{
			if (("Iso Dwg Area" == HeaderText) || ("Line Data Area" == HeaderText))
			{
				foreach (Telerik.WinControls.UI.GridViewRowInfo oGridRow in radGridViewBorderSettingData.Rows)
				{
					object oValue = radGridViewBorderSettingData.Rows[oGridRow.Index].Cells["MinX"].Value;
					double dMinX = Convert.ToDouble((null != oValue) ? oValue.ToString() : "0");
					oValue = radGridViewBorderSettingData.Rows[oGridRow.Index].Cells["MinY"].Value;
					double dMinY = Convert.ToDouble((null != oValue) ? oValue.ToString() : "0");
					oValue = radGridViewBorderSettingData.Rows[oGridRow.Index].Cells["MaxX"].Value;
					double dMaxX = Convert.ToDouble((null != oValue) ? oValue.ToString() : "0");
					oValue = radGridViewBorderSettingData.Rows[oGridRow.Index].Cells["MaxY"].Value;
					double dMaxY = Convert.ToDouble((null != oValue) ? oValue.ToString() : "0");

					/// X1,Y1,Z1,X2,Y2,Z2|Layer,Color,LType,LScale,LWeight
					oStreamWriter.WriteLine(string.Format("<LINE>|{0},{1},0,{2},{3},0|0,Red,0,0,0", dMinX, dMinY, dMaxX, dMinY));
					oStreamWriter.WriteLine(string.Format("<LINE>|{0},{1},0,{2},{3},0|0,Red,0,0,0", dMaxX, dMinY, dMaxX, dMaxY));
					oStreamWriter.WriteLine(string.Format("<LINE>|{0},{1},0,{2},{3},0|0,Red,0,0,0", dMaxX, dMaxY, dMinX, dMaxY));
					oStreamWriter.WriteLine(string.Format("<LINE>|{0},{1},0,{2},{3},0|0,Red,0,0,0", dMinX, dMaxY, dMinX, dMinY));
				}
			}
			else if ("Iso Data Pos" == HeaderText)
			{
				foreach (Telerik.WinControls.UI.GridViewRowInfo oGridRow in radGridViewBorderSettingData.Rows)
				{
					object oValue = radGridViewBorderSettingData.Rows[oGridRow.Index].Cells["X"].Value;
					double dX = Convert.ToDouble((null != oValue) ? oValue.ToString() : "0");
					oValue = radGridViewBorderSettingData.Rows[oGridRow.Index].Cells["Y"].Value;
					double dY = Convert.ToDouble((null != oValue) ? oValue.ToString() : "0");

					/// X,Y,Z,Height,Width,Angle,String,Justify,Oblique|Layer,Color,TextStyle,LWeight
					oStreamWriter.WriteLine(string.Format("<TEXT>|{0},{1},0,{2},{2},0,{3},Center Center,0|0,Red,ByLayer,0",
							dX, dY, 12, radGridViewBorderSettingData.Rows[oGridRow.Index].Cells[0].ToString()));
				}
			}
		}

		private void radGridViewBorderSettingData_CommandCellClick(object sender, EventArgs e)
		{
			Telerik.WinControls.UI.GridCommandCellElement cell = (Telerik.WinControls.UI.GridCommandCellElement)sender;

            if (("Iso Dwg Area" == HeaderText) || ("SelectBounds" == this.radGridViewBorderSettingData.Columns[cell.ColumnIndex].Name))
            {
                bool? res = AutoCADLauncher.LaunchIfPlugInInAppServiceNotAvailable(this);
                if (res.HasValue && (true == res))
                {
                    using (PlugInServiceSelectBoundsCommand cmd = new PlugInServiceSelectBoundsCommand() { ControlName = this.Name })
                    {
                        if (true == cmd.Execute())
                        {
                            this.radGridViewBorderSettingData.Rows[cell.RowIndex].Cells[cell.ColumnIndex - 4].Value = cmd.MinPoint.X;
                            this.radGridViewBorderSettingData.Rows[cell.RowIndex].Cells[cell.ColumnIndex - 3].Value = cmd.MinPoint.Y;
                            this.radGridViewBorderSettingData.Rows[cell.RowIndex].Cells[cell.ColumnIndex - 2].Value = cmd.MaxPoint.X;
                            this.radGridViewBorderSettingData.Rows[cell.RowIndex].Cells[cell.ColumnIndex - 1].Value = cmd.MaxPoint.Y;
                        }
                    }
                }
            }
            else if ("Line Data Location" == HeaderText)
            {
                bool? res = AutoCADLauncher.LaunchIfPlugInInAppServiceNotAvailable(this);
                if (res.HasValue && (true == res))
                {
                    if ("PickPoint" == this.radGridViewBorderSettingData.Columns[cell.ColumnIndex].Name)
                    {
                        using (PlugInServicePickPointCommand cmd = new PlugInServicePickPointCommand() { ControlName = this.Name })
                        {
                            if (true == cmd.Execute())
                            {
                                this.radGridViewBorderSettingData.Rows[cell.RowIndex].Cells[cell.ColumnIndex - 2].Value = cmd.Point.X;
                                this.radGridViewBorderSettingData.Rows[cell.RowIndex].Cells[cell.ColumnIndex - 1].Value = cmd.Point.Y;
                            }
                        }
                    }
                }
            }

            this.radGridViewBorderSettingData.BestFitColumns();
		}
	}
}
