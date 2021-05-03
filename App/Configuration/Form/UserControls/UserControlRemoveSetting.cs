using System;
using System.Collections.Generic;
using System.Data;
using System.IO;
using Configuration.Properties;
using SmartISO.Framework.Service;
using SmartISO.Service.Interface;
using Telerik.WinControls.UI;

namespace Configuration
{
	public partial class UserControlRemoveSetting : ThemeChangeable
	{

		public string HeaderText { get; set; }
        //private string BorderFilePath
        //{
        //    get { return (Parent.Parent.Parent as UserControlBorderSetting).BorderFilePath; }
        //}
		public UserControlRemoveSetting()
		{
			InitializeComponent();
			ApplyTheme(this.Controls);
		}

		private void UserControlIDCSRemoveSetting_Load(object sender, EventArgs e)
		{
			if ("Remove Area" == HeaderText)
			{
				radGridViewRemoveSetting.RowCount = 5;

				radGridViewRemoveSetting.Columns.Insert(1, new GridViewTextBoxColumn() { Name = "MinX", HeaderText = "MinX" });
				radGridViewRemoveSetting.Columns.Insert(2, new GridViewTextBoxColumn() { Name = "MinY", HeaderText = "MinY" });
				radGridViewRemoveSetting.Columns.Insert(3, new GridViewTextBoxColumn() { Name = "MaxX", HeaderText = "MaxX" });
				radGridViewRemoveSetting.Columns.Insert(4, new GridViewTextBoxColumn() { Name = "MaxY", HeaderText = "MaxY" });

				radGridViewRemoveSetting.Rows[0].Cells[0].Value = "REMOVE_AREA1";
				radGridViewRemoveSetting.Rows[0].Cells[0].ReadOnly = true;

				radGridViewRemoveSetting.Rows[1].Cells[0].Value = "REMOVE_AREA2";
				radGridViewRemoveSetting.Rows[1].Cells[0].ReadOnly = true;

				radGridViewRemoveSetting.Rows[2].Cells[0].Value = "REMOVE_AREA3";
				radGridViewRemoveSetting.Rows[2].Cells[0].ReadOnly = true;

				radGridViewRemoveSetting.Rows[3].Cells[0].Value = "REMOVE_AREA4";
				radGridViewRemoveSetting.Rows[3].Cells[0].ReadOnly = true;

				radGridViewRemoveSetting.Rows[4].Cells[0].Value = "REMOVE_AREA5";
				radGridViewRemoveSetting.Rows[4].Cells[0].ReadOnly = true;

				GridViewCommandColumn oCommandColumn = radGridViewRemoveSetting.Columns[5] as GridViewCommandColumn;
                //oCommandColumn.Image = Resources.selection;
			}
		}
		public override void LoadData()
		{
			if (string.IsNullOrEmpty(UserInfo.ProjectCode)) return;

			using (ISmartISOService svcProxy = ServiceFactory.CreateServiceChannel<ISmartISOService>(AppDocData.ServiceURL))
			{
				if ("Remove Area" == HeaderText)
				{
					#region clear controls value
					foreach (GridViewRowInfo oGridRow in radGridViewRemoveSetting.Rows)
					{
						for (int iCol = 1; iCol < oGridRow.Cells.Count; ++iCol)
						{
							radGridViewRemoveSetting.Rows[oGridRow.Index].Cells[iCol].Value = string.Empty;
						}
					}
					#endregion

					using (System.Data.DataSet dsParam = svcProxy.GetCommandParam("IDCS.BSL", "GetConfigCommand"))
					{
						dsParam.Tables["Params"].Rows.Clear();
						{
							System.Data.DataRow drParam = dsParam.Tables["Params"].NewRow();
							drParam["Section"] = HeaderText;
							drParam["Key"] = "REMOVE_AREA1";
							dsParam.Tables["Params"].Rows.Add(drParam);
						}
						using (DataSet ds = svcProxy.ExecuteCommand("IDCS.BSL", "GetConfigCommand", dsParam))
						{
							foreach (DataRow oRow in ds.Tables[0].Rows)
							{
								string[] oTokens = oRow["Value"].ToString().Split(',');
								if (4 == oTokens.Length)
								{
									radGridViewRemoveSetting.Rows[0].Cells["MinX"].Value = oTokens[0];
									radGridViewRemoveSetting.Rows[0].Cells["MinY"].Value = oTokens[1];
									radGridViewRemoveSetting.Rows[0].Cells["MaxX"].Value = oTokens[2];
									radGridViewRemoveSetting.Rows[0].Cells["MaxY"].Value = oTokens[3];
								}
							}
						}

						dsParam.Tables["Params"].Rows.Clear();
						{
							System.Data.DataRow drParam = dsParam.Tables["Params"].NewRow();
							drParam["Section"] = HeaderText;
							drParam["Key"] = "REMOVE_AREA2";
							dsParam.Tables["Params"].Rows.Add(drParam);
						}
						using (DataSet ds = svcProxy.ExecuteCommand("IDCS.BSL", "GetConfigCommand", dsParam))
						{
							foreach (DataRow oRow in ds.Tables[0].Rows)
							{
								string[] oTokens = oRow["Value"].ToString().Split(',');
								if (4 == oTokens.Length)
								{
									radGridViewRemoveSetting.Rows[1].Cells["MinX"].Value = oTokens[0];
									radGridViewRemoveSetting.Rows[1].Cells["MinY"].Value = oTokens[1];
									radGridViewRemoveSetting.Rows[1].Cells["MaxX"].Value = oTokens[2];
									radGridViewRemoveSetting.Rows[1].Cells["MaxY"].Value = oTokens[3];
								}
							}
						}

						dsParam.Tables["Params"].Rows.Clear();
						{
							System.Data.DataRow drParam = dsParam.Tables["Params"].NewRow();
							drParam["Section"] = HeaderText;
							drParam["Key"] = "REMOVE_AREA3";
							dsParam.Tables["Params"].Rows.Add(drParam);
						}
						using (DataSet ds = svcProxy.ExecuteCommand("IDCS.BSL", "GetConfigCommand", dsParam))
						{
							foreach (DataRow oRow in ds.Tables[0].Rows)
							{
								string[] oTokens = oRow["Value"].ToString().Split(',');
								if (4 == oTokens.Length)
								{
									radGridViewRemoveSetting.Rows[2].Cells["MinX"].Value = oTokens[0];
									radGridViewRemoveSetting.Rows[2].Cells["MinY"].Value = oTokens[1];
									radGridViewRemoveSetting.Rows[2].Cells["MaxX"].Value = oTokens[2];
									radGridViewRemoveSetting.Rows[2].Cells["MaxY"].Value = oTokens[3];
								}
							}
						}
						dsParam.Tables["Params"].Rows.Clear();
						{
							System.Data.DataRow drParam = dsParam.Tables["Params"].NewRow();
							drParam["Section"] = HeaderText;
							drParam["Key"] = "REMOVE_AREA4";
							dsParam.Tables["Params"].Rows.Add(drParam);
						}
						using (DataSet ds = svcProxy.ExecuteCommand("IDCS.BSL", "GetConfigCommand", dsParam))
						{
							foreach (DataRow oRow in ds.Tables[0].Rows)
							{
								string[] oTokens = oRow["Value"].ToString().Split(',');
								if (4 == oTokens.Length)
								{
									radGridViewRemoveSetting.Rows[2].Cells["MinX"].Value = oTokens[0];
									radGridViewRemoveSetting.Rows[2].Cells["MinY"].Value = oTokens[1];
									radGridViewRemoveSetting.Rows[2].Cells["MaxX"].Value = oTokens[2];
									radGridViewRemoveSetting.Rows[2].Cells["MaxY"].Value = oTokens[3];
								}
							}
						}
						dsParam.Tables["Params"].Rows.Clear();
						{
							System.Data.DataRow drParam = dsParam.Tables["Params"].NewRow();
							drParam["Section"] = HeaderText;
							drParam["Key"] = "REMOVE_AREA5";
							dsParam.Tables["Params"].Rows.Add(drParam);
						}
						using (DataSet ds = svcProxy.ExecuteCommand("IDCS.BSL", "GetConfigCommand", dsParam))
						{
							foreach (DataRow oRow in ds.Tables[0].Rows)
							{
								string[] oTokens = oRow["Value"].ToString().Split(',');
								if (4 == oTokens.Length)
								{
									radGridViewRemoveSetting.Rows[2].Cells["MinX"].Value = oTokens[0];
									radGridViewRemoveSetting.Rows[2].Cells["MinY"].Value = oTokens[1];
									radGridViewRemoveSetting.Rows[2].Cells["MaxX"].Value = oTokens[2];
									radGridViewRemoveSetting.Rows[2].Cells["MaxY"].Value = oTokens[3];
								}
							}
						}

						radGridViewRemoveSetting.BestFitColumns();
					}
				}
			}
		}

		public override void SaveData()
		{
			using (ISmartISOService svcProxy = ServiceFactory.CreateServiceChannel<ISmartISOService>(AppDocData.ServiceURL))
			{
				using (DataSet dsParam = svcProxy.GetCommandParam("IDCS.BSL", "SaveConfigsCommand"))
				{
					if ("Remove Area" == HeaderText)
					{
						foreach (GridViewRowInfo oGridRow in radGridViewRemoveSetting.Rows)
						{
							string sValue = string.Empty;
							sValue = (null != radGridViewRemoveSetting.Rows[oGridRow.Index].Cells["MinX"].Value) ? radGridViewRemoveSetting.Rows[oGridRow.Index].Cells["MinX"].Value.ToString() : string.Empty;
							sValue += "," + ((null != radGridViewRemoveSetting.Rows[oGridRow.Index].Cells["MinY"].Value) ? radGridViewRemoveSetting.Rows[oGridRow.Index].Cells["MinY"].Value.ToString() : string.Empty);
							sValue += "," + ((null != radGridViewRemoveSetting.Rows[oGridRow.Index].Cells["MaxX"].Value) ? radGridViewRemoveSetting.Rows[oGridRow.Index].Cells["MaxX"].Value.ToString() : string.Empty);
							sValue += "," + ((null != radGridViewRemoveSetting.Rows[oGridRow.Index].Cells["MaxY"].Value) ? radGridViewRemoveSetting.Rows[oGridRow.Index].Cells["MaxY"].Value.ToString() : string.Empty);

							{
								DataRow drParam = dsParam.Tables["Params"].NewRow();
								drParam["Section"] = HeaderText;
								drParam["Key"] = radGridViewRemoveSetting.Rows[oGridRow.Index].Cells[0].Value.ToString();
								drParam["Value"] = sValue;
								dsParam.Tables["Params"].Rows.Add(drParam);
							}
						}
					}
					svcProxy.ExecuteCommand("IDCS.BSL", "SaveConfigsCommand", dsParam);
				}
			}
		}

        private void radGridViewRemoveSetting_CommandCellClick(object sender, EventArgs e)
        {
            Telerik.WinControls.UI.GridCommandCellElement cell = (Telerik.WinControls.UI.GridCommandCellElement)sender;

            bool? res = AutoCADLauncher.LaunchIfPlugInInAppServiceNotAvailable(this);
            if (res.HasValue && (true == res))
            {
                using (PlugInServiceSelectBoundsCommand cmd = new PlugInServiceSelectBoundsCommand() { ControlName = this.Name })
                {
                    if (true == cmd.Execute())
                    {
                        this.radGridViewRemoveSetting.Rows[cell.RowIndex].Cells[cell.ColumnIndex - 4].Value = cmd.MinPoint.X;
                        this.radGridViewRemoveSetting.Rows[cell.RowIndex].Cells[cell.ColumnIndex - 3].Value = cmd.MinPoint.Y;
                        this.radGridViewRemoveSetting.Rows[cell.RowIndex].Cells[cell.ColumnIndex - 2].Value = cmd.MaxPoint.X;
                        this.radGridViewRemoveSetting.Rows[cell.RowIndex].Cells[cell.ColumnIndex - 1].Value = cmd.MaxPoint.Y;
                    }
                }
            }
            
            this.radGridViewRemoveSetting.BestFitColumns();
        }
	}
}
