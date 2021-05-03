using System;
using System.Collections.Generic;
using System.Linq;
using System.Windows.Forms;
using EDCS.WF.DSL;
using Telerik.WinControls.UI;
using Configuration.Properties;
using TechSun.Theme;

namespace Configuration
{
    public partial class FrmCommonRole_Create : KRadForm
    {
        private long mRoleID = -1;
        private bool bModifyFlag = true;

        public string RoleName
        {
            get { return radTextBoxName.Text; }
        }

        public string RoleNameView
        {
            get { return radTextBoxNameView.Text; }
        }

        public bool RoleVisible
        {
            get { return radCheckBoxVisible.Checked; }
        }

        public FrmCommonRole_Create()
        {
            InitializeComponent();
        }

        public FrmCommonRole_Create(bool isIE, long lRoleID)
        {
            InitializeComponent();

            var proxy = WcfHelper.CreateProxyNetTcp<IEdcsDac>(EDCS.WF.BSL.Variable.SharedVariable.IsIE);
            using (proxy as IDisposable)
            {
                mRoleID = lRoleID;

                if (mRoleID == -1)
                    bModifyFlag = false;

                if (bModifyFlag)
                {
                    var Common_Role = proxy.ReadCommon_Role(EDCS.WF.BSL.Variable.SharedVariable.IsIE).Where(w => w.RoleID == mRoleID).FirstOrDefault();

                    if (Common_Role != null)
                    {
                        radTextBoxName.Text = Common_Role.Name;
                        radTextBoxNameView.Text = Common_Role.NameView;
                        radCheckBoxVisible.Checked = Common_Role.VisibleFlag;
                    }
                }
            }
        }

        private void radButtonClose_Click(object sender, EventArgs e)
        {
            this.DialogResult = DialogResult.Cancel;
        }

        private void radButtonCreate_Click(object sender, EventArgs e)
        {
            try
            {
                var proxy = WcfHelper.CreateProxyNetTcp<IEdcsDac>(EDCS.WF.BSL.Variable.SharedVariable.IsIE);
                using (proxy as IDisposable)
                {
                    var lstCommon_Role = proxy.ReadCommon_Role(EDCS.WF.BSL.Variable.SharedVariable.IsIE);

                    string sRoleName = radTextBoxName.Text;
                    string sNameView = radTextBoxNameView.Text;

                    if (string.IsNullOrEmpty(sRoleName))
                    {
                        MessageBox.Show(string.Format(Resources.Message10011, "Role"), this.Text);
                        return;
                    }

                    if (string.IsNullOrEmpty(sNameView))
                    {
                        MessageBox.Show(string.Format(Resources.Message10011, "Description"), this.Text);
                        return;
                    }

                    if (lstCommon_Role.Where(w => w.NameView == sRoleName && w.RoleID != mRoleID).Count() > 0)
                    {
                        MessageBox.Show(string.Format(Resources.Message10011, "Role"), this.Text);
                        return;
                    }

                    this.DialogResult = DialogResult.OK;
                }
            }
            catch (System.ServiceModel.FaultException ex)
            {
                
                new TechSun.Theme.frmWarning(ex.Message.ToString()).ShowDialog(this);
            }
            catch (Exception ex)
            {
                
                new TechSun.Theme.frmWarning(ex.Message.ToString()).ShowDialog(this);
            }




        }
    }
}
