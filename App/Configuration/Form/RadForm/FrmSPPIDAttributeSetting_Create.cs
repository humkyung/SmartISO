using System;
using System.Collections.Generic;
using System.Drawing;
using System.Linq;
using System.Windows.Forms;
using EDCS.WF.DSL;
using Telerik.WinControls.UI;
using Configuration.Properties;
using TechSun.Theme;

namespace Configuration
{
    public partial class FrmSPPIDAttributeSetting_Create : KRadForm
    {
        private Information mInformation;

        private string m_ItemType;

        private string m_NewAttributeSetName = string.Empty;

        public string AttributeSetName
        {
            get { return m_NewAttributeSetName; }
        }

        private long m_DefaultAttributeGroupID = -1;

        public long DefaultAttributeGroupID
        {
            get { return m_DefaultAttributeGroupID; }
        }

        private bool mAllFlag = false;

        public bool AllFlag
        {
            get { return mAllFlag; }
        }

        public FrmSPPIDAttributeSetting_Create()
        {
            InitializeComponent();
        }

        public FrmSPPIDAttributeSetting_Create(bool isIE, string itemType)
        {
            InitializeComponent();

            mInformation = Information.GetInstance();

            try
            {
                m_ItemType = itemType;
                InitAttributeSet(itemType);
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

        private void InitAttributeSet(string itemType)
        {
            if (mInformation.Project != null)
            {

                var proxy = WcfHelper.CreateProxyNetTcp<IEdcsDac>(EDCS.WF.BSL.Variable.SharedVariable.IsIE);
                using (proxy as IDisposable)
                {
                    List<EntitySPPID_DataColumnGroup> lstSPToolColumnGroup = proxy.ReadDataColumnGroup(EDCS.WF.BSL.Variable.SharedVariable.IsIE, mInformation.Project.ProjectID, itemType, "Admin", true, true);
                    RadListDataItem oItemDefault = null;
                    RadListDataItem oItemSelectItem = null;

                    if (lstSPToolColumnGroup != null)
                    {
                        radDropDownListAttributeSet.Items.Clear();

                        RadListDataItem item = new RadListDataItem("All");
                        item.Font = Information.FONT_REGULAR;
                        item.Tag = "All";
                        item.ForeColor = Color.Blue;
                        radDropDownListAttributeSet.Items.Add(item);

                        foreach (var result in lstSPToolColumnGroup)
                        {
                            item = new RadListDataItem(result.Name);
                            item.Font = Information.FONT_REGULAR;
                            item.Tag = result;
                            if (result.AdminFlag == true)
                            {
                                item.ForeColor = Color.Blue;
                            }

                            radDropDownListAttributeSet.Items.Add(item);

                            if (result.Name == "Default")
                            {
                                oItemDefault = item;
                            }
                        }
                    }

                    if (oItemSelectItem != null)
                        oItemSelectItem.Selected = true;
                    else if (oItemDefault != null)
                        oItemDefault.Selected = true;
                    else if (radDropDownListAttributeSet.Items.Count == 1)
                    {
                        radDropDownListAttributeSet.Items[0].Selected = true;
                    }

                    lstSPToolColumnGroup.Clear();
                }
            }
        }

        private void radButtonCreate_Click(object sender, EventArgs e)
        {
            try
            {
                var proxy = WcfHelper.CreateProxyNetTcp<IEdcsDac>(EDCS.WF.BSL.Variable.SharedVariable.IsIE);
                using (proxy as IDisposable)
                {
                    List<EntitySPPID_DataColumnGroup> lstSPToolColumnGroup = proxy.ReadDataColumnGroup(EDCS.WF.BSL.Variable.SharedVariable.IsIE, mInformation.Project.ProjectID, m_ItemType, "Admin", false, true);
                    string strAttributeSetName = radTextBoxNewAttributeSet.Text;

                    if (string.IsNullOrEmpty(strAttributeSetName))
                    {
                        MessageBox.Show(string.Format(Resources.Message10011, "Attribute Set"), this.Text);
                        return;
                    }

                    if (lstSPToolColumnGroup.Where(w => w.Name == strAttributeSetName).Count() > 0)
                    {
                        MessageBox.Show(string.Format(Resources.Message10012, "Attribute Set"), this.Text);
                        return;
                    }

                    if (strAttributeSetName.ToUpper() == "ALL")
                    {
                        MessageBox.Show(string.Format(Resources.Message10012, "Attribute Set"), this.Text);
                        return;
                    }

                    m_NewAttributeSetName = strAttributeSetName;

                    if (radDropDownListAttributeSet.SelectedItem != null && radDropDownListAttributeSet.SelectedItem.Tag != null)
                    {
                        SPPID_DataColumnGroup group = radDropDownListAttributeSet.SelectedItem.Tag as SPPID_DataColumnGroup;
                        if (group != null)
                            m_DefaultAttributeGroupID = group.ColumnGroupID;
                        else
                        {
                            string sAll = radDropDownListAttributeSet.SelectedItem.Tag as string;
                            if (sAll == "All")
                            {
                                mAllFlag = true;
                            }
                        }
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

        private void radButtonClose_Click(object sender, EventArgs e)
        {
            this.DialogResult = DialogResult.Cancel;
        }


    }
}
