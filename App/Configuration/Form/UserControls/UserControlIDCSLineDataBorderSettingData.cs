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
using System.Windows.Forms;

namespace Configuration
{
	public partial class UserControlLineDataBorderSettingData : ThemeChangeable
	{
		public string HeaderText { get; set; }
		private const string SmallBoreNotice = "Small Bore Notice";
		private const string SmallBoreSize = "Small Bore Size";

		private const string Col_LineDataID = "LineDataID";
		private const string Col_ProjectID = "ProjectID";
		private const string Col_SiteID = "SiteID";

		private string BorderFilePath
		{
			get { return (Parent.Parent.Parent as UserControlBorderSetting).BorderFilePath; }
		}

		public UserControlLineDataBorderSettingData()
		{
			InitializeComponent();
			ApplyTheme(this.Controls);
		}

		private void UserControlBorderSettingData_Load(object sender, EventArgs e)
		{
			radGroupBoxBorderSettingData.Text = HeaderText;

			GridViewCommandColumn oCommandColumn = radGridViewBorderSettingData.Columns[3] as GridViewCommandColumn;
			oCommandColumn.Image = Resources.pick;
			oCommandColumn = radGridViewBorderSettingData.Columns[8] as GridViewCommandColumn;
			oCommandColumn.Image = Resources.selection;
		}

		/// <summary>
		/// load data from database
		/// </summary>
		public override void LoadData()
		{
			if (string.IsNullOrEmpty(UserInfo.ProjectCode)) return;

			using (IIDCSService svcProxy = ServiceFactory.CreateServiceChannel<IIDCSService>(AppDocData.ServiceURL))
			{
				radTextBoxControlSmallBoreNotice.Text = string.Empty;
				radGridViewBorderSettingData.Rows.Clear();
				{
					List<ColInfoDTO> oLineDataCols = new List<ColInfoDTO>();
					using (DataSet dsParam = svcProxy.GetCommandParam("IDCS.BSL", "GetLineDataColInfoListCommand"))
					{
						using (DataSet ds = svcProxy.ExecuteCommand("IDCS.BSL", "GetLineDataColInfoListCommand", dsParam))
						{
							foreach (DataRow oRow in ds.Tables["ColInfo"].Rows)
							{
								string sName = oRow["Name"].ToString();
								if ((Col_LineDataID == sName) || (Col_ProjectID == sName) || (Col_SiteID == sName)) continue;
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
							drParam["Section"] = "Line Data Pos";
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

							{
								DataRow[] oRows = ds.Tables[0].Select(string.Format("KEY='{0}'", SmallBoreNotice));
								if (1 == oRows.Length)
								{
									radTextBoxControlSmallBoreNotice.Text = oRows[0]["VALUE"].ToString();
								}

								this.radTextBoxSmallBoreSize.Text = "1.5";
								oRows = ds.Tables[0].Select(string.Format("KEY='{0}'", SmallBoreSize));
								if (1 == oRows.Length)
								{
									this.radTextBoxSmallBoreSize.Text = oRows[0]["VALUE"].ToString();
								}
							}

							#region LineData의 Font,Size 설정 기능(http://172.19.253.16:9001/redmine/issues/111)
							{
								DataRow[] oRows = ds.Tables[0].Select(string.Format("KEY='{0}'", "ACAD_TEXT_STYLE"));
								if (1 == oRows.Length)
								{
									string[] oTokens = oRows[0]["VALUE"].ToString().Split(',');
									string sTextColor = (oTokens.Length > 0) ? oTokens[0] : "Red";
									string sTextStyle = (oTokens.Length > 1) ? oTokens[1] : "Standard";
									string sTextHeight = (oTokens.Length > 2) ? oTokens[2] : "3";
									this.radLabelPropertyColor.Tag = sTextColor;
									this.radLabelPropertyColor.BackColor = RadFormColorTable.GetRGBFromIndex(sTextColor);
									this.radLabelPropertyStyle.Text = string.Format("{0},{1}", sTextStyle, sTextHeight);
								}
							}
							#endregion
						}

						dsParam.Tables["Params"].Rows.Clear();
						{
							System.Data.DataRow drParam = dsParam.Tables["Params"].NewRow();
							drParam["Section"] = "Line Data Area";
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

		/// <summary>
		/// save data
		/// </summary>
		/// <author>humkyung</author>
		public override void SaveData()
		{
			using (IIDCSService svcProxy = ServiceFactory.CreateServiceChannel<IIDCSService>(AppDocData.ServiceURL))
			{
				using (DataSet dsParam = svcProxy.GetCommandParam("IDCS.BSL", "SaveConfigsCommand"))
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
							drParam["Section"] = "Line Data Area";
							drParam["Key"] = radGridViewBorderSettingData.Rows[oGridRow.Index].Cells[0].Value.ToString();
							drParam["Value"] = sValue;
							dsParam.Tables["Params"].Rows.Add(drParam);
						}

						sValue = string.Empty;
						sValue = (null != radGridViewBorderSettingData.Rows[oGridRow.Index].Cells["X"].Value) ? radGridViewBorderSettingData.Rows[oGridRow.Index].Cells["X"].Value.ToString() : string.Empty;
						sValue += "," + ((null != radGridViewBorderSettingData.Rows[oGridRow.Index].Cells["Y"].Value) ? radGridViewBorderSettingData.Rows[oGridRow.Index].Cells["Y"].Value.ToString() : string.Empty);
						{
							DataRow drParam = dsParam.Tables["Params"].NewRow();
							drParam["Section"] = "Line Data Pos";
							drParam["Key"] = radGridViewBorderSettingData.Rows[oGridRow.Index].Cells[0].Value.ToString();
							drParam["Value"] = sValue;
							dsParam.Tables["Params"].Rows.Add(drParam);
						}
					}

					{
						DataRow drParam = dsParam.Tables["Params"].NewRow();
						drParam["Section"] = "Line Data Pos";
						drParam["Key"] = SmallBoreNotice;
						drParam["Value"] = radTextBoxControlSmallBoreNotice.Text;
						dsParam.Tables["Params"].Rows.Add(drParam);
					}
					{
						DataRow drParam = dsParam.Tables["Params"].NewRow();
						drParam["Section"] = "Line Data Pos";
						drParam["Key"] = SmallBoreSize;
						drParam["Value"] = this.radTextBoxSmallBoreSize.Text;
						dsParam.Tables["Params"].Rows.Add(drParam);
					}

					{
						DataRow drParam = dsParam.Tables["Params"].NewRow();
						drParam["Section"] = "Line Data Pos";
						drParam["Key"] = "ACAD_TEXT_STYLE";
						string sTextColor = (null != radLabelPropertyColor.Tag) ? this.radLabelPropertyColor.Tag.ToString() : "Red";
						string[] oTokens = this.radLabelPropertyStyle.Text.Split(',');
						string sTextStyle = (oTokens.Length > 0) ? oTokens[0] : "Standard";
						string sTextHeight= (oTokens.Length > 1) ? oTokens[1] : "3";
						drParam["Value"] = string.Format("{0},{1},{2}",sTextColor,sTextStyle,sTextHeight);
						dsParam.Tables["Params"].Rows.Add(drParam);
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

		private void radGridViewBorderSettingData_CommandCellClick(object sender, EventArgs e)
		{
			GridCommandCellElement cell = (GridCommandCellElement)sender;

			if ("Selection" == radGridViewBorderSettingData.Columns[cell.ColumnIndex].Name)
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
			else if ("Pick" == radGridViewBorderSettingData.Columns[cell.ColumnIndex].Name)
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

		/// <summary>
		/// pick small bore notice dwg position
		/// </summary>
		/// <author>humkyung</author>
		/// <date>2016.07.28</date>
		/// <param name="sender"></param>
		/// <param name="e"></param>
		private void radButtonSmallBoreNotice_Click(object sender, EventArgs e)
		{
			PF.Open(this.BorderFilePath);
			double[] res = PF.GetCADPoint();
			if ((null != res) && (3 == res.Length))
			{
				radTextBoxControlSmallBoreNotice.Text = string.Format("{0:0.00},{0:0.00}", ((double[])(res))[0], ((double[])(res))[1]);
			}
			else
			{
				radTextBoxControlSmallBoreNotice.Text = string.Empty;
			}
		}

		private void radButtonProperty_Click(object sender, EventArgs e)
		{
			RadFormTextProperty frm = new RadFormTextProperty();
			{
				frm.TextColor = (null != radLabelPropertyColor.Tag) ? radLabelPropertyColor.Tag.ToString() : "Red";
				string[] oTokens = radLabelPropertyStyle.Text.Split(',');
				frm.TextStyle = (oTokens.Length > 0) ? oTokens[0] : "Standard";
				frm.TextHeight = (oTokens.Length > 1) ? oTokens[1] : "3";
			}
			if (DialogResult.OK == frm.ShowDialog(this))
			{
				radLabelPropertyColor.Tag = frm.TextColor;
				radLabelPropertyColor.BackColor = RadFormColorTable.GetRGBFromIndex(frm.TextColor);
				radLabelPropertyStyle.Text = string.Format("{0},{1}",frm.TextStyle,frm.TextHeight);
			}
		}
	}
}
