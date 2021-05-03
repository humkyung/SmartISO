using IDCS.Framework.Service;
using IDCS.Service.Interface;
using TechSun.Theme;
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using Telerik.WinControls;

namespace Configuration
{
    public partial class FrmTextStyle : KRadForm
    {
        public FrmTextStyle()
        {
            InitializeComponent();
        }

		private void FrmTextStyle_Load(object sender, EventArgs e)
		{
			string sApp="ACAD_TEXT_STYLE";

			using (IIDCSService svcProxy = ServiceFactory.CreateServiceChannel<IIDCSService>(AppDocData.ServiceURL))
			{
				using (System.Data.DataSet dsParam = svcProxy.GetCommandParam("IDCS.BSL", "GetConfigCommand"))
				{
					dsParam.Tables["Params"].Rows.Clear();
					{
						System.Data.DataRow drParam = dsParam.Tables["Params"].NewRow();
						drParam["Section"] = sApp;
						drParam["Key"] = string.Empty;
						dsParam.Tables["Params"].Rows.Add(drParam);
					}

					using (DataSet ds = svcProxy.ExecuteCommand("IDCS.BSL", "GetConfigCommand", dsParam))
					{
						{
							DataRow[] oRows = ds.Tables[0].Select(string.Format("KEY='{0}'", "Standard"));
							if (1 == oRows.Length)
							{
								string[] oTokens = oRows[0]["VALUE"].ToString().Split(',');
								if (5 == oTokens.Length)
								{
									this.radDropDownListStandardFont.Text = oTokens[0];
									this.radDropDownListStandardBigFont.Text = oTokens[1];
									this.radDropDownListStandardBigFont.Enabled = radCheckBoxStandard.Checked = ("True" == oTokens[2]);
									this.radTextBoxStandardHeight.Text = oTokens[3];
									this.radTextBoxStandardWidthFactor.Text = oTokens[4];
								}
							}
						}

						string sFont = string.Empty,sBigFont = string.Empty,sBigFontUse = string.Empty,sHeight = string.Empty,sWidthFactor = string.Empty;
						for (int i = 1; i <= 10; ++i)
						{
							DataRow[] oRows = ds.Tables[0].Select(string.Format("KEY='Style{0}'", i));
							if (1 == oRows.Length)
							{
								string[] oTokens = oRows[0]["VALUE"].ToString().Split(',');
								if (5 == oTokens.Length)
								{
									sFont = oTokens[0];
									sBigFont = oTokens[1];
									sBigFontUse = oTokens[2];
									sHeight = oTokens[3];
									sWidthFactor = oTokens[4];

									switch (i)
									{
										case 1:
											this.radDropDownListStyle1Font.Text = sFont;
											this.radDropDownListStyle1BigFont.Text = sBigFont;
											this.radDropDownListStyle1BigFont.Enabled = radCheckBoxStyle1.Checked = ("True" == sBigFontUse);
											this.radTextBoxStyle1Height.Text = sHeight;
											this.radTextBoxStyle1WidthFactor.Text = sWidthFactor;
											break;
										case 2:
											this.radDropDownListStyle2Font.Text = sFont;
											this.radDropDownListStyle2BigFont.Text = sBigFont;
											this.radDropDownListStyle2BigFont.Enabled = radCheckBoxStyle2.Checked = ("True" == sBigFontUse);
											this.radTextBoxStyle2Height.Text = sHeight;
											this.radTextBoxStyle2WidthFactor.Text = sWidthFactor;
											break;
										case 3:
											this.radDropDownListStyle3Font.Text = sFont;
											this.radDropDownListStyle3BigFont.Text = sBigFont;
											this.radDropDownListStyle3BigFont.Enabled = radCheckBoxStyle3.Checked = ("True" == sBigFontUse);
											this.radTextBoxStyle3Height.Text = sHeight;
											this.radTextBoxStyle3WidthFactor.Text = sWidthFactor;
											break;
										case 4:
											this.radDropDownListStyle4Font.Text = sFont;
											this.radDropDownListStyle4BigFont.Text = sBigFont;
											this.radDropDownListStyle4BigFont.Enabled = radCheckBoxStyle4.Checked = ("True" == sBigFontUse);
											this.radTextBoxStyle4Height.Text = sHeight;
											this.radTextBoxStyle4WidthFactor.Text = sWidthFactor;
											break;
										case 5:
											this.radDropDownListStyle5Font.Text = sFont;
											this.radDropDownListStyle5BigFont.Text = sBigFont;
											this.radDropDownListStyle5BigFont.Enabled = radCheckBoxStyle5.Checked = ("True" == sBigFontUse);
											this.radTextBoxStyle5Height.Text = sHeight;
											this.radTextBoxStyle5WidthFactor.Text = sWidthFactor;
											break;
										case 6:
											this.radDropDownListStyle6Font.Text = sFont;
											this.radDropDownListStyle6BigFont.Text = sBigFont;
											this.radDropDownListStyle6BigFont.Enabled = radCheckBoxStyle6.Checked = ("True" == sBigFontUse);
											this.radTextBoxStyle6Height.Text = sHeight;
											this.radTextBoxStyle6WidthFactor.Text = sWidthFactor;
											break;
										case 7:
											this.radDropDownListStyle7Font.Text = sFont;
											this.radDropDownListStyle7BigFont.Text = sBigFont;
											this.radDropDownListStyle7BigFont.Enabled = radCheckBoxStyle7.Checked = ("True" == sBigFontUse);
											this.radTextBoxStyle7Height.Text = sHeight;
											this.radTextBoxStyle7WidthFactor.Text = sWidthFactor;
											break;
										case 8:
											this.radDropDownListStyle8Font.Text = sFont;
											this.radDropDownListStyle8BigFont.Text = sBigFont;
											this.radDropDownListStyle8BigFont.Enabled = radCheckBoxStyle8.Checked = ("True" == sBigFontUse);
											this.radTextBoxStyle8Height.Text = sHeight;
											this.radTextBoxStyle8WidthFactor.Text = sWidthFactor;
											break;
										case 9:
											this.radDropDownListStyle9Font.Text = sFont;
											this.radDropDownListStyle9BigFont.Text = sBigFont;
											this.radDropDownListStyle9BigFont.Enabled = radCheckBoxStyle9.Checked = ("True" == sBigFontUse);
											this.radTextBoxStyle9Height.Text = sHeight;
											this.radTextBoxStyle9WidthFactor.Text = sWidthFactor;
											break;
										case 10:
											this.radDropDownListStyle10Font.Text = sFont;
											this.radDropDownListStyle10BigFont.Text = sBigFont;
											this.radDropDownListStyle10BigFont.Enabled = radCheckBoxStyle10.Checked = ("True" == sBigFontUse);
											this.radTextBoxStyle10Height.Text = sHeight;
											this.radTextBoxStyle10WidthFactor.Text = sWidthFactor;
											break;
									}
								}
							}
						}
					}
				}
			}
		}

		private void radCheckBoxStandard_ToggleStateChanged(object sender, Telerik.WinControls.UI.StateChangedEventArgs args)
		{
			radDropDownListStandardBigFont.Enabled = (radCheckBoxStandard.ToggleState == Telerik.WinControls.Enumerations.ToggleState.On);
			radCheckBoxStandard.Text = (radCheckBoxStandard.ToggleState == Telerik.WinControls.Enumerations.ToggleState.On) ? "ON" : "OFF";
		}

		private void radCheckBoxStyle1_ToggleStateChanged(object sender, Telerik.WinControls.UI.StateChangedEventArgs args)
		{
			bool on = (radCheckBoxStyle1.ToggleState == Telerik.WinControls.Enumerations.ToggleState.On);
			radDropDownListStyle1BigFont.Enabled = on;
			radCheckBoxStyle1.Text = (on) ? "ON" : "OFF";
		}

		private void radCheckBoxStyle2_ToggleStateChanged(object sender, Telerik.WinControls.UI.StateChangedEventArgs args)
		{
			bool on = (radCheckBoxStyle2.ToggleState == Telerik.WinControls.Enumerations.ToggleState.On);
			radDropDownListStyle2BigFont.Enabled = on;
			radCheckBoxStyle2.Text = (on) ? "ON" : "OFF";
		}

		private void radCheckBoxStyle3_ToggleStateChanged(object sender, Telerik.WinControls.UI.StateChangedEventArgs args)
		{
			bool on = (radCheckBoxStyle3.ToggleState == Telerik.WinControls.Enumerations.ToggleState.On);
			radDropDownListStyle3BigFont.Enabled = on;
			radCheckBoxStyle3.Text = (on) ? "ON" : "OFF";
		}

		private void radCheckBoxStyle4_ToggleStateChanged(object sender, Telerik.WinControls.UI.StateChangedEventArgs args)
		{
			bool on = (radCheckBoxStyle4.ToggleState == Telerik.WinControls.Enumerations.ToggleState.On);
			radDropDownListStyle4BigFont.Enabled = on;
			radCheckBoxStyle4.Text = (on) ? "ON" : "OFF";
		}

		private void radCheckBoxStyle5_ToggleStateChanged(object sender, Telerik.WinControls.UI.StateChangedEventArgs args)
		{
			bool on = (radCheckBoxStyle5.ToggleState == Telerik.WinControls.Enumerations.ToggleState.On);
			radDropDownListStyle5BigFont.Enabled = on;
			radCheckBoxStyle5.Text = (on) ? "ON" : "OFF";
		}

		private void radCheckBoxStyle6_ToggleStateChanged(object sender, Telerik.WinControls.UI.StateChangedEventArgs args)
		{
			bool on = (radCheckBoxStyle6.ToggleState == Telerik.WinControls.Enumerations.ToggleState.On);
			radDropDownListStyle6BigFont.Enabled = on;
			radCheckBoxStyle6.Text = (on) ? "ON" : "OFF";
		}

		private void radCheckBoxStyle7_ToggleStateChanged(object sender, Telerik.WinControls.UI.StateChangedEventArgs args)
		{
			bool on = (radCheckBoxStyle7.ToggleState == Telerik.WinControls.Enumerations.ToggleState.On);
			radDropDownListStyle7BigFont.Enabled = on;
			radCheckBoxStyle7.Text = (on) ? "ON" : "OFF";
		}

		private void radCheckBoxStyle8_ToggleStateChanged(object sender, Telerik.WinControls.UI.StateChangedEventArgs args)
		{
			bool on = (radCheckBoxStyle8.ToggleState == Telerik.WinControls.Enumerations.ToggleState.On);
			radDropDownListStyle8BigFont.Enabled = on;
			radCheckBoxStyle8.Text = (on) ? "ON" : "OFF";
		}

		private void radCheckBoxStyle9_ToggleStateChanged(object sender, Telerik.WinControls.UI.StateChangedEventArgs args)
		{
			bool on = (radCheckBoxStyle9.ToggleState == Telerik.WinControls.Enumerations.ToggleState.On);
			radDropDownListStyle9BigFont.Enabled = on;
			radCheckBoxStyle9.Text = (on) ? "ON" : "OFF";
		}

		private void radCheckBoxStyle10_ToggleStateChanged(object sender, Telerik.WinControls.UI.StateChangedEventArgs args)
		{
			bool on = (radCheckBoxStyle10.ToggleState == Telerik.WinControls.Enumerations.ToggleState.On);
			radDropDownListStyle10BigFont.Enabled = on;
			radCheckBoxStyle10.Text = (on) ? "ON" : "OFF";
		}

		private void radButtonOK_Click(object sender, EventArgs e)
		{
			string sApp = "ACAD_TEXT_STYLE";

			using (IIDCSService svcProxy = ServiceFactory.CreateServiceChannel<IIDCSService>(AppDocData.ServiceURL))
			{
				using (DataSet dsParam = svcProxy.GetCommandParam("IDCS.BSL", "SaveConfigsCommand"))
				{
					{
						DataRow drParam = dsParam.Tables["Params"].NewRow();
						drParam["Section"] = sApp;
						drParam["Key"] = "Standard";
						drParam["Value"] = string.Format("{0},{1},{2},{3},{4}", 
							this.radDropDownListStandardFont.Text, this.radDropDownListStandardBigFont.Text, this.radCheckBoxStandard.Checked.ToString(), 
							this.radTextBoxStandardHeight.Text , this.radTextBoxStandardWidthFactor.Text);
						dsParam.Tables["Params"].Rows.Add(drParam);
					}

					for (int i = 1; i <= 10; ++i)
					{
						DataRow drParam = dsParam.Tables["Params"].NewRow();
						drParam["Section"] = sApp;
						drParam["Key"] = string.Format("Style{0}",i);
						switch (i)
						{
							case 1:
								drParam["Value"] = string.Format("{0},{1},{2},{3},{4}", 
									this.radDropDownListStyle1Font.Text, this.radDropDownListStyle1BigFont.Text, 
									this.radCheckBoxStyle1.Checked.ToString(), this.radTextBoxStyle1Height.Text, this.radTextBoxStyle1WidthFactor.Text);
								break;
							case 2:
								drParam["Value"] = string.Format("{0},{1},{2},{3},{4}", this.radDropDownListStyle2Font.Text, this.radDropDownListStyle2BigFont.Text, 
									this.radCheckBoxStyle2.Checked.ToString(), this.radTextBoxStyle2Height.Text, this.radTextBoxStyle2WidthFactor.Text);
								break;
							case 3:
								drParam["Value"] = string.Format("{0},{1},{2},{3},{4}", this.radDropDownListStyle3Font.Text, this.radDropDownListStyle3BigFont.Text,
									this.radCheckBoxStyle3.Checked.ToString(), this.radTextBoxStyle3Height.Text, this.radTextBoxStyle3WidthFactor.Text);
								break;
							case 4:
								drParam["Value"] = string.Format("{0},{1},{2},{3},{4}", this.radDropDownListStyle4Font.Text, this.radDropDownListStyle4BigFont.Text,
									this.radCheckBoxStyle4.Checked.ToString(), this.radTextBoxStyle4Height.Text, this.radTextBoxStyle4WidthFactor.Text);
								break;
							case 5:
								drParam["Value"] = string.Format("{0},{1},{2},{3},{4}", this.radDropDownListStyle5Font.Text, this.radDropDownListStyle5BigFont.Text,
									this.radCheckBoxStyle5.Checked.ToString(), this.radTextBoxStyle5Height.Text, this.radTextBoxStyle5WidthFactor.Text);
								break;
							case 6:
								drParam["Value"] = string.Format("{0},{1},{2},{3},{4}", this.radDropDownListStyle6Font.Text, this.radDropDownListStyle6BigFont.Text,
									this.radCheckBoxStyle6.Checked.ToString(), this.radTextBoxStyle6Height.Text, this.radTextBoxStyle6WidthFactor.Text);
								break;
							case 7:
								drParam["Value"] = string.Format("{0},{1},{2},{3},{4}", this.radDropDownListStyle7Font.Text, this.radDropDownListStyle7BigFont.Text,
									this.radCheckBoxStyle7.Checked.ToString(), this.radTextBoxStyle7Height.Text, this.radTextBoxStyle7WidthFactor.Text);
								break;
							case 8:
								drParam["Value"] = string.Format("{0},{1},{2},{3},{4}", this.radDropDownListStyle8Font.Text, this.radDropDownListStyle8BigFont.Text,
									this.radCheckBoxStyle8.Checked.ToString(), this.radTextBoxStyle8Height.Text, this.radTextBoxStyle8WidthFactor.Text);
								break;
							case 9:
								drParam["Value"] = string.Format("{0},{1},{2},{3},{4}", this.radDropDownListStyle9Font.Text, this.radDropDownListStyle9BigFont.Text,
									this.radCheckBoxStyle9.Checked.ToString(), this.radTextBoxStyle9Height.Text, this.radTextBoxStyle9WidthFactor.Text);
								break;
							case 10:
								drParam["Value"] = string.Format("{0},{1},{2},{3},{4}", this.radDropDownListStyle10Font.Text, this.radDropDownListStyle10BigFont.Text,
									this.radCheckBoxStyle10.Checked.ToString(), this.radTextBoxStyle10Height.Text, this.radTextBoxStyle10WidthFactor.Text);
								break;
						}
						dsParam.Tables["Params"].Rows.Add(drParam);
					}

					svcProxy.ExecuteCommand("IDCS.BSL", "SaveConfigsCommand", dsParam);
				}
			}

			DialogResult = DialogResult.OK;
		}

		private void radButtonClose_Click(object sender, EventArgs e)
		{
			DialogResult = DialogResult.Cancel;
		}
    }
}
