using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using Telerik.WinControls;

using SmartISO.Service.Interface;
using SmartISO.Framework.Service;

namespace Configuration.Form.RadForm
{
    public partial class RadFormSA : Telerik.WinControls.UI.RadForm
    {
        public DataSet dsUser { get; set; }
        private string _SA = string.Empty;

        public RadFormSA()
        {
            InitializeComponent();
        }

        /// <summary>
        /// display user list
        /// </summary>
        /// <author>humkyung</author>
        /// <date>2017.05.05</date>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void RadFormSA_Load(object sender, EventArgs e)
        {
            DataRow[] drUser = dsUser.Tables[0].Select("Role = 'SA'");
            if ((null != drUser) && (drUser.Length > 0))
            {
                foreach (DataRow dr in drUser)
                {
                    _SA = dr["ID"].ToString();
                    dsUser.Tables[0].Rows.Remove(dr);
                }
            }
            this.radDropDownListSA.DisplayMember = "ID";
            this.radDropDownListSA.DataSource = dsUser.Tables[0];
        }

        /// <summary>
        /// change system administator as selected user
        /// </summary>
        /// <author>humkyung</author>
        /// <date>2017.05.05</date>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void radButtonOK_Click(object sender, EventArgs e)
        {
            try
            {
                if ((null != this.radDropDownListSA.SelectedItem) && !string.IsNullOrEmpty(this.radDropDownListSA.SelectedItem.Text))
                {
                    using (ISmartISOService svcProxy = ServiceFactory.CreateServiceChannel<ISmartISOService>(AppDocData.ServiceURL, "wsHttp"))
                    {
                        using (DataSet dsParam = svcProxy.GetCommandParam("SmartISO.BSL", "RegisterUserCommand"))
                        {
                            DataRow dr = dsParam.Tables[0].NewRow();
                            {
                                dr["ID"] = this.radDropDownListSA.SelectedItem.Text;
                                dr["Role"] = "SA";
                                dsParam.Tables[0].Rows.Add(dr);
                            }

                            svcProxy.ExecuteCommand("SmartISO.BSL", "RegisterUserCommand", dsParam);

                            dsParam.Tables[0].Rows.Clear();
                            dr = dsParam.Tables[0].NewRow();
                            {
                                dr["ID"] = _SA;
                                dr["Role"] = "USER";
                                dsParam.Tables[0].Rows.Add(dr);
                            }
                            svcProxy.ExecuteCommand("SmartISO.BSL", "RegisterUserCommand", dsParam);
                        }
                    }
                }
                DialogResult = System.Windows.Forms.DialogResult.OK;
            }
            catch (Exception ex)
            {
                new TechSun.Theme.frmWarning(ex.Message).ShowDialog(this);
            }
        }

        private void radButtonCancel_Click(object sender, EventArgs e)
        {
            DialogResult = System.Windows.Forms.DialogResult.Cancel;
        }
    }
}
