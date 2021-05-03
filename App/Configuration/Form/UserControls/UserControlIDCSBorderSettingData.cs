using System;
using System.Collections.Generic;
using System.Data;
using System.IO;
using Configuration.Properties;
using CADAutomation;
using IDCS.DataTransferObjects;
using IDCS.Framework.Service;
using IDCS.Service.Interface;
using Telerik.WinControls.UI;

namespace Configuration
{
	public partial class UserControlBorderSettingData : ThemeChangeable
	{
		public string HeaderText { get; set; }
		private string BorderFilePath
		{
			get { return (Parent.Parent.Parent as UserControlBorderSetting).BorderFilePath; }
		}

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

				radGridViewBorderSettingData.Columns.Insert(1, new GridViewTextBoxColumn() { Name = "MinX", HeaderText = "MinX" });
				radGridViewBorderSettingData.Columns.Insert(2, new GridViewTextBoxColumn() { Name = "MinY", HeaderText = "MinY" });
				radGridViewBorderSettingData.Columns.Insert(3, new GridViewTextBoxColumn() { Name = "MaxX", HeaderText = "MaxX" });
				radGridViewBorderSettingData.Columns.Insert(4, new GridViewTextBoxColumn() { Name = "MaxY", HeaderText = "MaxY" });

				radGridViewBorderSettingData.Rows[0].Cells[0].Value = "BORDER_AREA";
				radGridViewBorderSettingData.Rows[0].Cells[0].ReadOnly = true;

				radGridViewBorderSettingData.Rows[1].Cells[0].Value = "DRAWING_AREA";
				radGridViewBorderSettingData.Rows[1].Cells[0].ReadOnly = true;

				radGridViewBorderSettingData.Rows[2].Cells[0].Value = "BM_AREA";
				radGridViewBorderSettingData.Rows[2].Cells[0].ReadOnly = true;

				GridViewCommandColumn oCommandColumn = radGridViewBorderSettingData.Columns[5] as GridViewCommandColumn;
				oCommandColumn.Image = Resources.selection;
			}
			else if ("Line Data Pos" == HeaderText)
			{
				radGridViewBorderSettingData.Columns.Insert(1, new GridViewTextBoxColumn() { Name = "X", HeaderText = "X" });
				radGridViewBorderSettingData.Columns.Insert(2, new GridViewTextBoxColumn() { Name = "Y", HeaderText = "Y" });

				GridViewCommandColumn oCommandColumn = radGridViewBorderSettingData.Columns[3] as GridViewCommandColumn;
				oCommandColumn.Image = Resources.pick;
			}
			else if("Line Data Area" == HeaderText)
			{
				radGridViewBorderSettingData.Columns.Insert(1, new GridViewTextBoxColumn() { Name = "MinX", HeaderText = "MinX" });
				radGridViewBorderSettingData.Columns.Insert(2, new GridViewTextBoxColumn() { Name = "MinY", HeaderText = "MinY" });
				radGridViewBorderSettingData.Columns.Insert(3, new GridViewTextBoxColumn() { Name = "MaxX", HeaderText = "MaxX" });
				radGridViewBorderSettingData.Columns.Insert(4, new GridViewTextBoxColumn() { Name = "MaxY", HeaderText = "MaxY" });

				GridViewCommandColumn oCommandColumn = radGridViewBorderSettingData.Columns[5] as GridViewCommandColumn;
				oCommandColumn.Image = Resources.selection;
			}
			//else if ("Line Data Area" == HeaderText)
			//{
			//	radGridViewBorderSettingData.Columns.Insert(1, new GridViewTextBoxColumn() { Name = "MinX", HeaderText = "MinX" });
			//	radGridViewBorderSettingData.Columns.Insert(2, new GridViewTextBoxColumn() { Name = "MinY", HeaderText = "MinY" });
			//	radGridViewBorderSettingData.Columns.Insert(3, new GridViewTextBoxColumn() { Name = "MaxX", HeaderText = "MaxX" });
			//	radGridViewBorderSettingData.Columns.Insert(4, new GridViewTextBoxColumn() { Name = "MaxY", HeaderText = "MaxY" });

			//	GridViewCommandColumn oCommandColumn = radGridViewBorderSettingData.Columns[5] as GridViewCommandColumn;
			//	oCommandColumn.Image = Resources.selection;
			//}
		}

		public override void LoadData()
		{
			if (string.IsNullOrEmpty(UserInfo.ProjectCode)) return;

			using (IIDCSService svcProxy = ServiceFactory.CreateServiceChannel<IIDCSService>(AppDocData.ServiceURL))
			{
				if ("Iso Dwg Area" == HeaderText)
				{
					#region clear controls value
					foreach (GridViewRowInfo oGridRow in radGridViewBorderSettingData.Rows)
					{
						for (int iCol = 1; iCol < oGridRow.Cells.Count; ++iCol)
						{
							radGridViewBorderSettingData.Rows[oGridRow.Index].Cells[iCol].Value = string.Empty;
						}
					}
					#endregion

					using (System.Data.DataSet dsParam = svcProxy.GetCommandParam("IDCS.BSL", "GetConfigCommand"))
					{
						dsParam.Tables["Params"].Rows.Clear();
						{
							System.Data.DataRow drParam = dsParam.Tables["Params"].NewRow();
							drParam["Section"] = HeaderText;
							drParam["Key"] = "BORDER_AREA";
							dsParam.Tables["Params"].Rows.Add(drParam);
						}
						using (DataSet ds = svcProxy.ExecuteCommand("IDCS.BSL", "GetConfigCommand", dsParam))
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
						{
							System.Data.DataRow drParam = dsParam.Tables["Params"].NewRow();
							drParam["Section"] = HeaderText;
							drParam["Key"] = "DRAWING_AREA";
							dsParam.Tables["Params"].Rows.Add(drParam);
						}
						using (DataSet ds = svcProxy.ExecuteCommand("IDCS.BSL", "GetConfigCommand", dsParam))
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
						{
							System.Data.DataRow drParam = dsParam.Tables["Params"].NewRow();
							drParam["Section"] = HeaderText;
							drParam["Key"] = "BM_AREA";
							dsParam.Tables["Params"].Rows.Add(drParam);
						}
						using (DataSet ds = svcProxy.ExecuteCommand("IDCS.BSL", "GetConfigCommand", dsParam))
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

						radGridViewBorderSettingData.BestFitColumns();
					}
				}
				else if ("Line Data Pos" == HeaderText)
				{
					radGridViewBorderSettingData.Rows.Clear();

					List<ColInfoDTO> oLineDataCols = new List<ColInfoDTO>();
					using (DataSet dsParam = svcProxy.GetCommandParam("IDCS.BSL", "GetLineDataColInfoListCommand"))
					{
						using (DataSet ds = svcProxy.ExecuteCommand("IDCS.BSL", "GetLineDataColInfoListCommand", dsParam))
						{
							foreach (DataRow oRow in ds.Tables["ColInfo"].Rows)
							{
								oLineDataCols.Add(
									new ColInfoDTO() { Name = oRow["Name"].ToString(), DataType = oRow["DataType"].ToString() }
								);
							}
						}
					}
					radGridViewBorderSettingData.RowCount = oLineDataCols.Count;
					foreach (GridViewRowInfo oGridRow in radGridViewBorderSettingData.Rows)
					{
						radGridViewBorderSettingData.Rows[oGridRow.Index].Cells[0].Value = oLineDataCols[oGridRow.Index].Name;
						radGridViewBorderSettingData.Rows[oGridRow.Index].Cells[0].ReadOnly = true;
					}

					using (System.Data.DataSet dsParam = svcProxy.GetCommandParam("IDCS.BSL", "GetConfigCommand"))
					{
						dsParam.Tables["Params"].Rows.Clear();
						{
							System.Data.DataRow drParam = dsParam.Tables["Params"].NewRow();
							drParam["Section"] = HeaderText;
							drParam["Key"] = string.Empty;
							dsParam.Tables["Params"].Rows.Add(drParam);
						}
						using (DataSet ds = svcProxy.ExecuteCommand("IDCS.BSL","GetConfigCommand",dsParam))
						{
							foreach (GridViewRowInfo oGridRow in radGridViewBorderSettingData.Rows)
							{
								DataRow[] oRows = ds.Tables[0].Select(string.Format("key='{0}'", radGridViewBorderSettingData.Rows[oGridRow.Index].Cells["Name"].Value.ToString()));
								if (1 == oRows.Length)
								{
									string[] oTokens = oRows[0]["Value"].ToString().Split(',');
									if (2 == oTokens.Length)
									{
										radGridViewBorderSettingData.Rows[oGridRow.Index].Cells["X"].Value = oTokens[0];
										radGridViewBorderSettingData.Rows[oGridRow.Index].Cells["Y"].Value = oTokens[1];
									}
								}
							}
						}
					}

					radGridViewBorderSettingData.BestFitColumns();
				}
				else if ("Line Data Area" == HeaderText)
				{
					radGridViewBorderSettingData.Rows.Clear();

					List<ColInfoDTO> oLineDataCols = new List<ColInfoDTO>();
					using (DataSet dsParam = svcProxy.GetCommandParam("IDCS.BSL", "GetLineDataColInfoListCommand"))
					{
						using (DataSet ds = svcProxy.ExecuteCommand("IDCS.BSL", "GetLineDataColInfoListCommand", dsParam))
						{
							foreach (DataRow oRow in ds.Tables["ColInfo"].Rows)
							{
								oLineDataCols.Add(
									new ColInfoDTO() { Name = oRow["Name"].ToString(), DataType = oRow["DataType"].ToString() }
								);
							}
						}
					}
					radGridViewBorderSettingData.RowCount = oLineDataCols.Count;
					foreach (GridViewRowInfo oGridRow in radGridViewBorderSettingData.Rows)
					{
						radGridViewBorderSettingData.Rows[oGridRow.Index].Cells[0].Value = oLineDataCols[oGridRow.Index].Name;
						radGridViewBorderSettingData.Rows[oGridRow.Index].Cells[0].ReadOnly = true;
					}

					using (System.Data.DataSet dsParam = svcProxy.GetCommandParam("IDCS.BSL", "GetConfigCommand"))
					{
						dsParam.Tables["Params"].Rows.Clear();
						{
							System.Data.DataRow drParam = dsParam.Tables["Params"].NewRow();
							drParam["Section"] = HeaderText;
							drParam["Key"] = string.Empty;
							dsParam.Tables["Params"].Rows.Add(drParam);
						}
						using (DataSet ds = svcProxy.ExecuteCommand("IDCS.BSL","GetConfigCommand",dsParam))
						{
							foreach (GridViewRowInfo oGridRow in radGridViewBorderSettingData.Rows)
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
					}

					radGridViewBorderSettingData.BestFitColumns();
				}
			}
		}

		public override void SaveData()
		{
			using (IIDCSService svcProxy = ServiceFactory.CreateServiceChannel<IIDCSService>(AppDocData.ServiceURL))
			{
				List<ProjectConfigDTO> configs = new List<ProjectConfigDTO>();
				using (DataSet dsParam = svcProxy.GetCommandParam("IDCS.BSL", "SaveConfigsCommand"))
				{
					if (("Iso Dwg Area" == HeaderText) || ("Line Data Area" == HeaderText))
					{
						foreach (GridViewRowInfo oGridRow in radGridViewBorderSettingData.Rows)
						{
							string sValue = string.Empty;
							sValue = (null != radGridViewBorderSettingData.Rows[oGridRow.Index].Cells["MinX"].Value) ? radGridViewBorderSettingData.Rows[oGridRow.Index].Cells["MinX"].Value.ToString() : string.Empty;
							sValue += "," + ((null != radGridViewBorderSettingData.Rows[oGridRow.Index].Cells["MinY"].Value) ? radGridViewBorderSettingData.Rows[oGridRow.Index].Cells["MinY"].Value.ToString() : string.Empty);
							sValue += "," + ((null != radGridViewBorderSettingData.Rows[oGridRow.Index].Cells["MaxX"].Value) ? radGridViewBorderSettingData.Rows[oGridRow.Index].Cells["MaxX"].Value.ToString() : string.Empty);
							sValue += "," + ((null != radGridViewBorderSettingData.Rows[oGridRow.Index].Cells["MaxY"].Value) ? radGridViewBorderSettingData.Rows[oGridRow.Index].Cells["MaxY"].Value.ToString() : string.Empty);

							{
								DataRow drParam = dsParam.Tables["Params"].NewRow();
								drParam["Section"] = HeaderText;
								drParam["Key"] = radGridViewBorderSettingData.Rows[oGridRow.Index].Cells[0].Value.ToString();
								drParam["Value"] = sValue;
								dsParam.Tables["Params"].Rows.Add(drParam);
							}
						}
					}
					else
					{
						foreach (GridViewRowInfo oGridRow in radGridViewBorderSettingData.Rows)
						{
							string sValue = string.Empty;
							sValue = (null != radGridViewBorderSettingData.Rows[oGridRow.Index].Cells["X"].Value) ? radGridViewBorderSettingData.Rows[oGridRow.Index].Cells["X"].Value.ToString() : string.Empty;
							sValue += "," + ((null != radGridViewBorderSettingData.Rows[oGridRow.Index].Cells["Y"].Value) ? radGridViewBorderSettingData.Rows[oGridRow.Index].Cells["Y"].Value.ToString() : string.Empty);

							{
								DataRow drParam = dsParam.Tables["Params"].NewRow();
								drParam["Section"] = HeaderText;
								drParam["Key"] = radGridViewBorderSettingData.Rows[oGridRow.Index].Cells[0].Value.ToString();
								drParam["Value"] = sValue;
								dsParam.Tables["Params"].Rows.Add(drParam);
							}
						}
					}

					svcProxy.ExecuteCommand("IDCS.BSL", "SaveConfigsCommand", dsParam);
				}
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
				foreach (GridViewRowInfo oGridRow in radGridViewBorderSettingData.Rows)
				{
					object oValue = radGridViewBorderSettingData.Rows[oGridRow.Index].Cells["MinX"].Value;
					double dMinX = 0;
					double.TryParse((null != oValue) ? oValue.ToString() : "0",out dMinX);
					oValue = radGridViewBorderSettingData.Rows[oGridRow.Index].Cells["MinY"].Value;
					double dMinY = 0;
					double.TryParse((null != oValue) ? oValue.ToString() : "0", out dMinY);
					double dMaxX = 0;
					oValue = radGridViewBorderSettingData.Rows[oGridRow.Index].Cells["MaxX"].Value;
					double.TryParse((null != oValue) ? oValue.ToString() : "0", out dMaxX);
					double dMaxY = 0;
					oValue = radGridViewBorderSettingData.Rows[oGridRow.Index].Cells["MaxY"].Value;
					double.TryParse((null != oValue) ? oValue.ToString() : "0", out dMaxY);

					/// X1,Y1,Z1,X2,Y2,Z2|Layer,Color,LType,LScale,LWeight
					oStreamWriter.WriteLine(string.Format("<LINE>|{0},{1},0,{2},{3},0|0,Red,0,0,0", dMinX, dMinY, dMaxX, dMinY));
					oStreamWriter.WriteLine(string.Format("<LINE>|{0},{1},0,{2},{3},0|0,Red,0,0,0", dMaxX, dMinY, dMaxX, dMaxY));
					oStreamWriter.WriteLine(string.Format("<LINE>|{0},{1},0,{2},{3},0|0,Red,0,0,0", dMaxX, dMaxY, dMinX, dMaxY));
					oStreamWriter.WriteLine(string.Format("<LINE>|{0},{1},0,{2},{3},0|0,Red,0,0,0", dMinX, dMaxY, dMinX, dMinY));
				}
			}
			else if ("Iso Data Pos" == HeaderText)
			{
				foreach (GridViewRowInfo oGridRow in radGridViewBorderSettingData.Rows)
				{
					object oValue = radGridViewBorderSettingData.Rows[oGridRow.Index].Cells["X"].Value;
					double dX = 0;
					double.TryParse((null != oValue) ? oValue.ToString() : "0", out dX);
					oValue = radGridViewBorderSettingData.Rows[oGridRow.Index].Cells["Y"].Value;
					double dY = 0;
					double.TryParse((null != oValue) ? oValue.ToString() : "0", out dY);

					/// X,Y,Z,Height,Width,Angle,String,Justify,Oblique|Layer,Color,TextStyle,LWeight
					oStreamWriter.WriteLine(string.Format("<TEXT>|{0},{1},0,{2},{2},0,{3},Center Center,0|0,Red,ByLayer,0",
							dX, dY, 12, radGridViewBorderSettingData.Rows[oGridRow.Index].Cells[0].ToString()));
				}
			}
		}

		private void radGridViewBorderSettingData_CommandCellClick(object sender, EventArgs e)
		{
			GridCommandCellElement cell = (GridCommandCellElement)sender;

			if (("Iso Dwg Area" == HeaderText) || ("Line Data Area" == HeaderText))
			{
				PF.Open(this.BorderFilePath);
				object[] res = PF.GetCADFence();
				if ((null != res) && (2 == res.Length))
				{
					double dMinX = ((double[])(res[0]))[0];
					double dMinY = ((double[])(res[0]))[1];
					double dMaxX = ((double[])(res[1]))[0];
					double dMaxY = ((double[])(res[1]))[1];
					if (dMinX > dMaxX)
					{
						double dTemp = dMinX;
						dMinX = dMaxX;
						dMaxX = dTemp;
					}
					if (dMinY > dMaxY)
					{
						double dTemp = dMinY;
						dMinY = dMaxY;
						dMaxY = dTemp;
					}

					radGridViewBorderSettingData.Rows[cell.RowIndex].Cells["MinX"].Value = string.Format("{0:0.00}", dMinX);
					radGridViewBorderSettingData.Rows[cell.RowIndex].Cells["MinY"].Value = string.Format("{0:0.00}", dMinY);
					radGridViewBorderSettingData.Rows[cell.RowIndex].Cells["MaxX"].Value = string.Format("{0:0.00}", dMaxX);
					radGridViewBorderSettingData.Rows[cell.RowIndex].Cells["MaxY"].Value = string.Format("{0:0.00}", dMaxY);
				}
				else
				{
					radGridViewBorderSettingData.Rows[cell.RowIndex].Cells["MinX"].Value = string.Empty;
					radGridViewBorderSettingData.Rows[cell.RowIndex].Cells["MinY"].Value = string.Empty;
					radGridViewBorderSettingData.Rows[cell.RowIndex].Cells["MaxX"].Value = string.Empty;
					radGridViewBorderSettingData.Rows[cell.RowIndex].Cells["MaxY"].Value = string.Empty;
				}
			}
			else if ("Line Data Pos" == HeaderText)
			{
				PF.Open(this.BorderFilePath);
				double[] res = PF.GetCADPoint();
				if ((null != res) && (3 == res.Length))
				{
					radGridViewBorderSettingData.Rows[cell.RowIndex].Cells["X"].Value = string.Format("{0:0.00}", ((double[])(res))[0]);
					radGridViewBorderSettingData.Rows[cell.RowIndex].Cells["Y"].Value = string.Format("{0:0.00}", ((double[])(res))[1]);
				}
				else
				{
					radGridViewBorderSettingData.Rows[cell.RowIndex].Cells["X"].Value = string.Empty;
					radGridViewBorderSettingData.Rows[cell.RowIndex].Cells["Y"].Value = string.Empty;
				}
			}
		}
	}
}
