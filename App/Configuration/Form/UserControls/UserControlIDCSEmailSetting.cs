using System;
using System.Collections.Generic;
using System.Data;
using System.Linq;
using System.Xml;
using Configuration.Properties;
using IDCS.DataTransferObjects;
using IDCS.Framework.Service;
using IDCS.Service.Interface;
using Telerik.WinControls.UI;

namespace Configuration
{
	public partial class UserControlIDCSEmailSetting : ThemeChangeable
	{
		private int _SiteID = -1;
		private string _SendTo = string.Empty;

		public IEnumerable<int> CheckedItemList
		{
			get
			{
				var rows = (from GridViewRowInfo row in radGridViewMailSetting.Rows
							where ((null != row.Cells["Check"].Value) && (true.ToString() == row.Cells["Check"].Value.ToString()))
							select row.Index);
				return rows;
			}
		}

		public UserControlIDCSEmailSetting(string sSendTo)
		{
			InitializeComponent();
			this._SendTo = sSendTo;
		}

		private void UserControlIDCSEmailSetting_Load(object sender, EventArgs e)
		{
			this.radLabelSendTo.Text = this._SendTo + " : ";
		}

		/// <summary>
		/// load email setting
		/// </summary>
		/// <author>humkyung</author>
		/// <date>2016.09.06</date>
		/// <param name="iSiteID"></param>
		public void LoadData(int iSiteID)
		{
			try
			{
				List<UserControlIDCSSiteSetting.EmailSetting> oEmailSettingList = new List<UserControlIDCSSiteSetting.EmailSetting>();

				using (IIDCSService svcProxy = ServiceFactory.CreateServiceChannel<IIDCSService>(AppDocData.ServiceURL))
				{
					UserInfo.SiteID = iSiteID.ToString();
					using (System.Data.DataSet dsParam = svcProxy.GetCommandParam("IDCS.BSL", "GetConfigCommand"))
					{
						System.Data.DataRow drParam = dsParam.Tables["Params"].NewRow();
						drParam["Section"] = "Email Setting";
						drParam["Key"] = this._SendTo;
						dsParam.Tables["Params"].Rows.Add(drParam);

						using (DataSet ds = svcProxy.ExecuteCommand("IDCS.BSL","GetConfigCommand",dsParam))
						{
							if (1 == ds.Tables[0].Rows.Count)
							{
								string sValue = ds.Tables[0].Rows[0]["Value"].ToString();

								XmlDocument oXDoc = new XmlDocument();
								oXDoc.LoadXml(sValue);
								XmlElement XmlRoot = oXDoc["Emails"];
								foreach (XmlNode XmlDefinition in XmlRoot.ChildNodes)
								{
									try
									{
										UserControlIDCSSiteSetting.EmailSetting oEmailSetting = new UserControlIDCSSiteSetting.EmailSetting()
										{
											ID = XmlDefinition["ID"].InnerText,
											Name = XmlDefinition["Name"].InnerText,
											Address = XmlDefinition["Address"].InnerText
										};
										oEmailSettingList.Add(oEmailSetting);
									}
									catch { }
								}
							}

							this.radGridViewMailSetting.RowCount = oEmailSettingList.Count;
							foreach (GridViewRowInfo oGridRow in this.radGridViewMailSetting.Rows)
							{
								this.radGridViewMailSetting.Rows[oGridRow.Index].Cells["ID"].Value = oEmailSettingList[oGridRow.Index].ID;
								this.radGridViewMailSetting.Rows[oGridRow.Index].Cells["Name"].Value = oEmailSettingList[oGridRow.Index].Name;
								this.radGridViewMailSetting.Rows[oGridRow.Index].Cells["Address"].Value = oEmailSettingList[oGridRow.Index].Address;
							}
							this.radGridViewMailSetting.BestFitColumns();
						}
					}
				}

				this._SiteID = iSiteID;
			}
			catch { }
		}

		/// <summary>
		/// save email setting
		/// </summary>
		/// <author>humkyung</author>
		/// <date>2016.09.06</date>
		public void SaveData()
		{
			try
			{
				UserInfo.SiteID = this._SiteID.ToString();

				using (IIDCSService svcProxy = ServiceFactory.CreateServiceChannel<IIDCSService>(AppDocData.ServiceURL))
				{
					using (DataSet dsParam = svcProxy.GetCommandParam("IDCS.BSL", "SaveConfigCommand"))
					{
						XmlDocument xDoc = new XmlDocument();
						XmlElement xmlRoot = xDoc.CreateElement("Emails");
						xDoc.AppendChild(xmlRoot);
						foreach (GridViewRowInfo oGridRow in this.radGridViewMailSetting.Rows)
						{
							XmlElement xml = xDoc.CreateElement("Email");
							xmlRoot.AppendChild(xml);
							{
								XmlElement oXmlEle = xDoc.CreateElement("ID");
								object oValue = this.radGridViewMailSetting.Rows[oGridRow.Index].Cells["ID"].Value;
								string sValue = (null != oValue) ? oValue.ToString() : string.Empty;
								oXmlEle.InnerText = sValue;
								xml.AppendChild(oXmlEle);

								oXmlEle = xDoc.CreateElement("Name");
								oValue = this.radGridViewMailSetting.Rows[oGridRow.Index].Cells["Name"].Value;
								sValue = (null != oValue) ? oValue.ToString() : string.Empty;
								oXmlEle.InnerText = sValue;
								xml.AppendChild(oXmlEle);

								oXmlEle = xDoc.CreateElement("Address");
								oValue = this.radGridViewMailSetting.Rows[oGridRow.Index].Cells["Address"].Value;
								sValue = (null != oValue) ? oValue.ToString() : string.Empty;
								oXmlEle.InnerText = sValue;
								xml.AppendChild(oXmlEle);
							}
						}

						{
							DataRow drParam = dsParam.Tables["Params"].NewRow();
							drParam["Section"] = "Email Setting";
							drParam["Key"] = this._SendTo;
							drParam["Value"] = xDoc.InnerXml;
							dsParam.Tables["Params"].Rows.Add(drParam);
						}
						svcProxy.ExecuteCommand("IDCS.BSL", "SaveConfigsCommand", dsParam);
					}
				}
			} catch { }
		}

		private void radButtonAdd_Click(object sender, EventArgs e)
		{
			GridViewRowInfo oGridRow = this.radGridViewMailSetting.Rows.AddNew();
		}

		private void radButtonDelete_Click(object sender, EventArgs e)
		{
			if (this.CheckedItemList.ToList().Count > 0)
			{
				List<int> oCheckedItemList = this.CheckedItemList.ToList();
				oCheckedItemList.Reverse();
				foreach (int iRowIndex in oCheckedItemList)
				{
					this.radGridViewMailSetting.Rows.RemoveAt(iRowIndex);
				}
			}
		}

		/// <summary>
		/// auto resize columns
		/// </summary>
		/// <author>humkyung</author>
		/// <date>2016.09.06</date>
		/// <param name="sender"></param>
		/// <param name="e"></param>
		private void radGridViewMailSetting_CellEndEdit(object sender, GridViewCellEventArgs e)
		{
			this.radGridViewMailSetting.BestFitColumns();
		}
	}
}
