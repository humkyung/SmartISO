using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Linq;
using System.Text;
using System.Windows.Forms;

using SmartISO.Service.Interface;
using SmartISO.Framework.Service;
using Telerik.WinControls.UI;

namespace Configuration
{
	public partial class UserControlPrjSetting : ThemeChangeable
	{
		public const string DisplayName = "Project Setting";

        private Int64 ProjectUID
        {
            get { return Convert.ToInt64(this.radDropDownListProject.Items[this.radDropDownListProject.SelectedIndex].Tag); }
        }

		public UserControlPrjSetting()
		{
			InitializeComponent();
			ApplyTheme(this.Controls);
		}

		/// <summary>
		/// load config
		/// </summary>
		/// <param name="sender"></param>
		/// <param name="e"></param>
		private void UserControlPrjSetting_Load(object sender, EventArgs e)
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
		/// load and display project setting
		/// </summary>
		/// <author>humkyung</author>
		/// <date>2016.07.13</date>
		public override void LoadData()
		{
			if (this.radDropDownListProject.SelectedIndex < 0) return;

			this.radGridViewBOMHeaderTextLoc.RowCount = 1;

            try
            {
                using (ISmartISOService svcProxy = ServiceFactory.CreateServiceChannel<ISmartISOService>(AppDocData.ServiceURL))
                {
                    #region clear control values
                    radTextBoxControlSizeUnit.Text = string.Empty;
                    radTextBoxControlBoltSizeUnit.Text = string.Empty;
                    radListControlWBSItems.Items.Clear();
                    radCheckBoxEnableHoldItemCheck.Checked = false;
                    radTextBoxControlDelimiterForInsulation.Text = string.Empty;
                    radTextBoxControlDeletedItemMessage.Text = string.Empty;
                    radCheckBoxWriteLatestDescToRemarkColumn.Checked = false;
                    foreach (Telerik.WinControls.UI.GridViewRowInfo oGridRow in radGridViewBOMHeaderTextLoc.Rows)
                    {
                        foreach (Telerik.WinControls.UI.GridViewCellInfo oGridCell in oGridRow.Cells)
                        {
                            radGridViewBOMHeaderTextLoc.Rows[oGridRow.Index].Cells[oGridCell.ColumnInfo.Index].Value = string.Empty;
                        }
                    }
                    #endregion

                    using (DataSet dsParam = svcProxy.GetCommandParam("SmartISO.BSL", "GetConfigCommand"))
                    {
                        DataRow dr = dsParam.Tables["Params"].NewRow();
                        {
                            dr["Ref_UID"] = ProjectUID;
                            dr["Section"] = "Prj Setting";
                        }
                        dsParam.Tables["Params"].Rows.Add(dr);

                        using (DataSet ds = svcProxy.ExecuteCommand("SmartISO.BSL", "GetConfigCommand", dsParam))
                        {
                            foreach (DataRow oRow in ds.Tables[0].Rows)
                            {
                                object oValue = oRow["Key"];
                                string sKey = (null != oValue) ? oValue.ToString() : string.Empty;
                                if ("Size Unit" == sKey)
                                {
                                    radTextBoxControlSizeUnit.Text = oRow["Value"].ToString();
                                }
                                else if ("Bolt Size Unit" == sKey)
                                {
                                    radTextBoxControlBoltSizeUnit.Text = oRow["Value"].ToString();
                                }
                                else if ("WBS Items" == sKey)
                                {
                                    string[] oTokens = oRow["Value"].ToString().Split(',');
                                    foreach (string sToken in oTokens)
                                    {
                                        radListControlWBSItems.Items.Add(sToken);
                                    }
                                }
                                else if ("Enable Hold Item Check" == sKey)
                                {
                                    radCheckBoxEnableHoldItemCheck.Checked = (true.ToString() == oRow["Value"].ToString());
                                }
                                else if ("Delimiter for Insulation" == sKey)
                                {
                                    radTextBoxControlDelimiterForInsulation.Text = oRow["Value"].ToString();
                                }
                                else if ("Deleted Item Message" == sKey)
                                {
                                    radTextBoxControlDeletedItemMessage.Text = oRow["Value"].ToString();
                                }
                                else if ("Write latest desc to remark column" == sKey)
                                {
                                    radCheckBoxWriteLatestDescToRemarkColumn.Checked = (true.ToString() == oRow["Value"].ToString());
                                }
                            }
                        }

                        dsParam.Tables["Params"].Rows[0]["Section"] = "Iso Edit Setting";
                        using (DataSet ds = svcProxy.ExecuteCommand("SmartISO.BSL","GetConfigCommand",dsParam))
                        {
                            foreach (DataRow oRow in ds.Tables[0].Rows)
                            {
                                if ("NO POS" == oRow["key"].ToString())
                                {
                                    radGridViewBOMHeaderTextLoc.Rows[0].Cells["No"].Value = oRow["value"].ToString();
                                }
                                else if ("MATL DESC POS" == oRow["key"].ToString())
                                {
                                    radGridViewBOMHeaderTextLoc.Rows[0].Cells["MATL_DESC"].Value = oRow["value"].ToString();
                                }
                                else if ("SIZE POS" == oRow["key"].ToString())
                                {
                                    radGridViewBOMHeaderTextLoc.Rows[0].Cells["SIZE"].Value = oRow["value"].ToString();
                                }
                                else if ("MATL SPEC POS" == oRow["key"].ToString())
                                {
                                    radGridViewBOMHeaderTextLoc.Rows[0].Cells["MATL_SPEC"].Value = oRow["value"].ToString();
                                }
                                else if ("SYM POS" == oRow["key"].ToString())
                                {
                                    radGridViewBOMHeaderTextLoc.Rows[0].Cells["SYM"].Value = oRow["value"].ToString();
                                }
                                else if ("MATL CODE POS" == oRow["key"].ToString())
                                {
                                    radGridViewBOMHeaderTextLoc.Rows[0].Cells["MATL_CODE"].Value = oRow["value"].ToString();
                                }
                                else if ("BOLT LEN POS" == oRow["key"].ToString())
                                {
                                    radGridViewBOMHeaderTextLoc.Rows[0].Cells["BOLT_LEN"].Value = oRow["value"].ToString();
                                }
                                else if ("QTY POS" == oRow["key"].ToString())
                                {
                                    radGridViewBOMHeaderTextLoc.Rows[0].Cells["QTY"].Value = oRow["value"].ToString();
                                }
                            }
                        }
                    }
                }
            }
            catch (Exception ex)
            {
                new TechSun.Theme.frmWarning(ex.Message).ShowDialog(this);
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
			if (string.IsNullOrEmpty(UserInfo.ProjectCode)) return;

            //string sEndPointAddress = "http://localhost:13371/ProjectConfigService.svc";
            //using (IProjectConfigService svcProxy = ServiceFactory.CreateServiceChannel<IProjectConfigService>(sEndPointAddress))
            //{
            //    List<ProjectConfigDTO> configs = new List<ProjectConfigDTO>();
            //    try
            //    {
            //        string sValue = string.Empty;
            //        configs.Add(new ProjectConfigDTO() { Section = "Prj Setting", Key = "Size Unit", Value = radTextBoxControlSizeUnit.Text });
            //        configs.Add(new ProjectConfigDTO() { Section = "Prj Setting", Key = "Bolt Size Unit", Value = radTextBoxControlBoltSizeUnit.Text });
            //        {
            //            sValue = string.Empty;
            //            foreach (Telerik.WinControls.UI.RadListDataItem oItem in radListControlWBSItems.Items)
            //            {
            //                if (!string.IsNullOrEmpty(sValue)) sValue += ",";
            //                sValue += oItem.Text;
            //            }
            //            configs.Add(new ProjectConfigDTO() { Section = "Prj Setting", Key = "WBS Items", Value = sValue });
            //        }
            //        configs.Add(new ProjectConfigDTO() { Section = "Prj Setting", Key = "Enable Hold Item Check", Value = radCheckBoxEnableHoldItemCheck.Checked.ToString() });
            //        configs.Add(new ProjectConfigDTO() { Section = "Prj Setting", Key = "Delimiter for Insulation", Value = radTextBoxControlDelimiterForInsulation.Text });
            //        configs.Add(new ProjectConfigDTO() { Section = "Prj Setting", Key = "Deleted Item Message", Value = radTextBoxControlDeletedItemMessage.Text });
            //        configs.Add(new ProjectConfigDTO() { Section = "Prj Setting", Key = "Write latest desc to remark column", Value = radCheckBoxWriteLatestDescToRemarkColumn.Checked.ToString() });

            //        configs.Add(new ProjectConfigDTO() { Section = "Iso Edit Setting", Key = "NO POS", Value = (null != radGridViewBOMHeaderTextLoc.Rows[0].Cells["No"].Value ? radGridViewBOMHeaderTextLoc.Rows[0].Cells["No"].Value.ToString() : string.Empty) });
            //        configs.Add(new ProjectConfigDTO() { Section = "Iso Edit Setting", Key = "MATL DESC POS", Value = (null != radGridViewBOMHeaderTextLoc.Rows[0].Cells["MATL_DESC"].Value ? radGridViewBOMHeaderTextLoc.Rows[0].Cells["MATL_DESC"].Value.ToString() : string.Empty) });
            //        configs.Add(new ProjectConfigDTO() { Section = "Iso Edit Setting", Key = "SIZE POS", Value = (null != radGridViewBOMHeaderTextLoc.Rows[0].Cells["SIZE"].Value ? radGridViewBOMHeaderTextLoc.Rows[0].Cells["SIZE"].Value.ToString() : string.Empty) });
            //        configs.Add(new ProjectConfigDTO() { Section = "Iso Edit Setting", Key = "MATL SPEC POS", Value = (null != radGridViewBOMHeaderTextLoc.Rows[0].Cells["MATL_SPEC"].Value ? radGridViewBOMHeaderTextLoc.Rows[0].Cells["MATL_SPEC"].Value.ToString() : string.Empty) });
            //        configs.Add(new ProjectConfigDTO() { Section = "Iso Edit Setting", Key = "SYM POS", Value = (null != radGridViewBOMHeaderTextLoc.Rows[0].Cells["SYM"].Value ? radGridViewBOMHeaderTextLoc.Rows[0].Cells["SYM"].Value.ToString() : string.Empty) });
            //        configs.Add(new ProjectConfigDTO() { Section = "Iso Edit Setting", Key = "MATL CODE POS", Value = (null != radGridViewBOMHeaderTextLoc.Rows[0].Cells["MATL_CODE"].Value ? radGridViewBOMHeaderTextLoc.Rows[0].Cells["MATL_CODE"].Value.ToString() : string.Empty) });
            //        configs.Add(new ProjectConfigDTO() { Section = "Iso Edit Setting", Key = "BOLT LEN POS", Value = (null != radGridViewBOMHeaderTextLoc.Rows[0].Cells["BOLT_LEN"].Value ? radGridViewBOMHeaderTextLoc.Rows[0].Cells["BOLT_LEN"].Value.ToString() : string.Empty) });
            //        configs.Add(new ProjectConfigDTO() { Section = "Iso Edit Setting", Key = "QTY POS", Value = (null != radGridViewBOMHeaderTextLoc.Rows[0].Cells["QTY"].Value ? radGridViewBOMHeaderTextLoc.Rows[0].Cells["QTY"].Value.ToString() : string.Empty) });

            //        svcProxy.SaveConfigs(configs);
            //    }
            //    catch (Exception ex)
            //    {
            //        MessageBox.Show(ex.Message, "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
            //    }
            //}
		}

		private void radButtonAddWBSItem_Click(object sender, EventArgs e)
		{
			if (string.IsNullOrEmpty(radTextBoxWBSItem.Text))
			{
				MessageBox.Show("WBS Item is empty", "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
				return;
			}

			Telerik.WinControls.UI.RadListDataItem oItem = radListControlWBSItems.Items.FirstOrDefault(param => param.Text == radTextBoxWBSItem.Text);
			if (null != oItem)
			{
				MessageBox.Show("There is same WBS Item", "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
				return;
			}

			radListControlWBSItems.Items.Add(radTextBoxWBSItem.Text);
		}

		/// <summary>
		/// remove selected wbs item
		/// </summary>
		/// <param name="sender"></param>
		/// <param name="e"></param>
		private void radButtonDeleteWBSItem_Click(object sender, EventArgs e)
		{
			if (radListControlWBSItems.SelectedIndex >= 0)
			{
				radListControlWBSItems.Items.RemoveAt(radListControlWBSItems.SelectedIndex);
			}
		}

		/// <summary>
		/// update project setting when user select a project
		/// </summary>
		/// <author>humkyung</author>
		/// <date>2016.07.13</date>
		/// <param name="sender"></param>
		/// <param name="e"></param>
		private void radDropDownListProject_SelectedIndexChanged(object sender, Telerik.WinControls.UI.Data.PositionChangedEventArgs e)
		{
			if (radDropDownListProject.SelectedIndex >= 0) LoadData();
		}
	}
}
