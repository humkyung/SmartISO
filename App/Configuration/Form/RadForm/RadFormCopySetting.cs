using System;
using System.Collections.Generic;
using System.Linq;
using System.Windows.Forms;
using Configuration.Properties;
using SmartISO.Framework.Service;
using SmartISO.Service.Interface;
using Telerik.WinControls.UI;
using System.Data;

namespace Configuration.Form.RadForm
{
    public partial class RadFormCopySetting : TechSun.Theme.KRadForm
    {
		private Int64 ProjectUID{ get; set; }
		private Int64 SiteUID{ get; set; }

        private Int64 FromProjectUID
        {
            get { return Convert.ToInt64(this.radDropDownListProject.Items[this.radDropDownListProject.SelectedIndex].Tag); }
        }
        private Int64 FromSiteUID
        {
            get { return Convert.ToInt64(this.radDropDownListSite.Items[this.radDropDownListSite.SelectedIndex].Tag); }
        }

        public RadFormCopySetting(Int64 iProjectUID, Int64 iSiteUID)
        {
            InitializeComponent();

			ProjectUID = iProjectUID;
			SiteUID = iSiteUID;
        }

		private void RadFormIDCSCopySetting_Load(object sender, EventArgs e)
		{
			this.CenterToParent();

			using (ISmartISOService svcProxy = ServiceFactory.CreateServiceChannel<ISmartISOService>(AppDocData.ServiceURL))
			{
				using (DataSet dsParam = svcProxy.GetCommandParam("IDCS.BSL", "GetAllProjectListCommand"))
				{
					using (DataSet ds = svcProxy.ExecuteCommand("IDCS.BSL", "GetAllProjectListCommand", dsParam))
					{
						foreach (DataRow oRow in ds.Tables[0].Rows)
						{
                            if ((true.ToString() == oRow["DeleteYN"].ToString()) || (ProjectUID == Convert.ToInt64(oRow["UID"].ToString()))) continue;
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

		private void radButtonClose_Click(object sender, EventArgs e)
		{
			DialogResult = DialogResult.Cancel;
		}

		/// <summary>
		/// copy project setting
		/// </summary>
		/// <author>humkyung</author>
		/// <date>2016.08.25</date>
		/// <param name="sender"></param>
		/// <param name="e"></param>
		private void radButtonCopy_Click(object sender, EventArgs e)
		{
			try
			{
				if ((null != this.radDropDownListProject.SelectedItem) && (null != this.radDropDownListSite.SelectedItem))
				{
					try
					{
						Cursor.Current = Cursors.WaitCursor;
						using (ISmartISOService svcProxy = ServiceFactory.CreateServiceChannel<ISmartISOService>(AppDocData.ServiceURL))
						{
							using (DataSet dsParam = svcProxy.GetCommandParam("IDCS.BSL", "CopyConfigsCommand"))
							{
								{
									DataRow drParam = dsParam.Tables["Params"].NewRow();
                                    drParam["ToPrjUID"] = ProjectUID;
                                    drParam["ToSiteUID"] = SiteUID;
                                    drParam["FromPrjUID"] = FromProjectUID;
									drParam["FromSiteUID"] = FromSiteUID;
									dsParam.Tables["Params"].Rows.Add(drParam);
								}
								svcProxy.ExecuteCommand("IDCS.BSL", "CopyConfigsCommand", dsParam);
							}
						}
					}
					catch (Exception ex)
					{
                        new TechSun.Theme.frmWarning(ex.Message).ShowDialog(this);
					}
					finally
					{
						Cursor.Current = Cursors.Default;
					}
				}
			}
			catch(Exception ex)
			{
                new TechSun.Theme.frmWarning(ex.Message).ShowDialog(this);
			}

			DialogResult = DialogResult.OK;
		}

		/// <summary>
		/// select site when project is selected
		/// </summary>
		/// <param name="sender"></param>
		/// <param name="e"></param>
		private void radDropDownListProject_SelectedIndexChanged(object sender, Telerik.WinControls.UI.Data.PositionChangedEventArgs e)
		{
			using (ISmartISOService svcProxy = ServiceFactory.CreateServiceChannel<ISmartISOService>(AppDocData.ServiceURL))
			{
				using (DataSet dsParam = svcProxy.GetCommandParam("IDCS.BSL", "GetSiteListCommand"))
				{
                    DataRow dr = dsParam.Tables["Params"].NewRow();
                    {
                        dr["Project_UID"] = FromProjectUID;
                    }
                    dsParam.Tables["Params"].Rows.Add(dr);
					using (DataSet dsSite = svcProxy.ExecuteCommand("IDCS.BSL", "GetSiteListCommand", dsParam))
					{
                        foreach (DataRow oRow in dsSite.Tables[0].Rows)
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
                //if (!string.IsNullOrEmpty(UserInfo.SiteID))
                //{
                //    try
                //    {
                //        RadListDataItem oItem = radDropDownListSite.Items.FirstOrDefault(param => (param.Tag as SiteDTO).Id.ToString() == UserInfo.SiteID);
                //        if (null != oItem)
                //        {
                //            radDropDownListSite.SelectedItem = oItem;
                //        }
                //    }
                //    catch { }
                //}
			}
		}
    }
}
