using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using EDCS.WF.DSL;

using Telerik.WinControls.UI;
using EDCS.WF.DSL.Entities;

namespace Configuration
{
    public partial class UserControlCommonTagRuleSetting : UserControlBase
    {
        Dictionary<string, UserControlCommonTagRule> mdicUserControlCommonTagRule = new Dictionary<string, UserControlCommonTagRule>();

        public UserControlCommonTagRuleSetting()
        {
            InitializeComponent();
        }

        public override void LoadData()
        {
            InitDropDownControl();
        }

        private void InitDropDownControl()
        {
            GeneralUtil.SetRadDropDownListProjectNo(EDCS.WF.BSL.Variable.SharedVariable.IsIE, ref radDropDownListProject, this._entityEDBInformation, ToolType.Admin);

            var proxy = WcfHelper.CreateProxyNetTcp<IEdcsDac>(EDCS.WF.BSL.Variable.SharedVariable.IsIE);
            using (proxy as IDisposable)
            {
                radDropDownListListType.Items.Clear();
                var lstListTypes = proxy.ReadListTypes(EDCS.WF.BSL.Variable.SharedVariable.IsIE, false).Where(w => w.TagNamingRuleFlag == true).ToList();

                foreach (var vListTypes in lstListTypes)
                {
                    if (vListTypes.Name == "Instrument3DDimension" || vListTypes.Name == "InstrumentIOList")
                        continue;

                    RadListDataItem item = new RadListDataItem();
                    item.Text = vListTypes.NameView;
                    item.Value = vListTypes.ListTypeID;
                    item.Tag = vListTypes;

                    radDropDownListListType.Items.Add(item);
                }

                if (radDropDownListListType.Items.Count > 0)
                {
                    radDropDownListListType.Items[0].Selected = true;
                    //radDropDownListListType.SelectedValue = this._entityEDBInformation.ListType.ListTypeID;
                }
            }
        }

        private void InitProjectSetting(EntityCommon_ProjectMaster cpProject, EntityEDB_ListType eListType)
        {
            radSplitContainerTagRule.Controls.Clear();
            mdicUserControlCommonTagRule.Clear();

            // EDB Setting
            if (cpProject.EDBFlag == true && eListType != null)
            {
                InitTagRuleSetting(ToolType.EDB, cpProject.ProjectMasterID, eListType);
            }

            // SPPID Setting
            if (cpProject.SPPIDFlag == true && eListType != null)
            {
                InitTagRuleSetting(ToolType.SPPID, cpProject.ProjectMasterID, eListType);
            }

            // SP3D Setting
            if (cpProject.SP3DFlag == true && eListType != null)
            {
                InitTagRuleSetting(ToolType.SP3D, cpProject.ProjectMasterID, eListType);
            }

            // SPI Setting
            //if (cpProject.SPIFlag == true && eListType != null && eListType.MappingSPI == "Instrument")
            //{
            //    InitTagRuleSetting(ToolType.SPI, cpProject.ProjectMasterID, eListType);
            //}
        }

        private void InitTagRuleSetting(string sToolName, long lProjectMasterID, EntityEDB_ListType eListType)
        {
            SplitPanel splitPanel = new SplitPanel();

            UserControlCommonTagRule tagRule = new UserControlCommonTagRule(EDCS.WF.BSL.Variable.SharedVariable.IsIE, sToolName, lProjectMasterID, eListType);
            mdicUserControlCommonTagRule.Add(sToolName, tagRule);

            tagRule.Dock = DockStyle.Fill;
            splitPanel.Controls.Add(tagRule);
            radSplitContainerTagRule.Controls.Add(splitPanel);
        }

        private void radDropDownListProject_SelectedIndexChanged(object sender, Telerik.WinControls.UI.Data.PositionChangedEventArgs e)
        {
            if (radDropDownListProject.SelectedItem != null && radDropDownListListType.SelectedItem != null)
            {
                EntityEDB_ListType eListType = this.radDropDownListListType.SelectedItem.Tag as EntityEDB_ListType;

                EntityCommon_ProjectMaster cpProject = (EntityCommon_ProjectMaster)radDropDownListProject.SelectedItem.DataBoundItem;
                InitProjectSetting(cpProject, eListType);

                GeneralUtil.SaveRadDropDownListProjectNo(radDropDownListProject, this._entityEDBInformation, ToolType.Admin);
            }
        }

        private void radDropDownListListType_SelectedIndexChanged(object sender, Telerik.WinControls.UI.Data.PositionChangedEventArgs e)
        {
            if (radDropDownListProject.SelectedItem != null && radDropDownListListType.SelectedItem != null)
            {
                EntityEDB_ListType eListType = this.radDropDownListListType.SelectedItem.Tag as EntityEDB_ListType;

                EntityCommon_ProjectMaster cpProject = (EntityCommon_ProjectMaster)radDropDownListProject.SelectedItem.DataBoundItem;
                InitProjectSetting(cpProject, eListType);

                GeneralUtil.SaveRadDropDownListProjectNo(radDropDownListProject, this._entityEDBInformation, ToolType.Admin);
            }
        }

        //public override void SaveData()
        //{
        //    //MessageBox.Show(mdicUserControlCommonTagRule.Count.ToString());
        //    string sMessage = string.Empty;
        //    StringBuilder sbErrorMessage = new StringBuilder();
        //    foreach (KeyValuePair<string, UserControlCommonTagRule> uc in mdicUserControlCommonTagRule)
        //    {
        //        bool bResult = uc.Value.SaveTagRule(ref sMessage);

        //        if (!bResult)
        //        {
        //            sbErrorMessage.AppendLine(sMessage);
        //        }
        //    }

        //    if (sbErrorMessage.Length > 0)
        //        MessageBox.Show(sbErrorMessage.ToString());
        //    else
        //        MessageBox.Show(this, "Saved successfully!", this.Text);
        //}
    }
}
