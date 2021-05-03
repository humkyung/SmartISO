using System;
using System.Collections.Generic;
using System.Text;
using System.Windows.Forms;
using EDCS.WF.DSL;

using Telerik.WinControls.UI;
using Configuration.Properties;
using EDCS.WF.DSL.Entities;

namespace Configuration
{
    public partial class UserControlSPPIDMarkTagNoSetting : UserControlBase
    {
        Dictionary<string, UserControlSPPIDMarkTagNo> mdicUserControlCommonTagRule = new Dictionary<string, UserControlSPPIDMarkTagNo>();

        private string ItemType
        {
            get
            {
                if (comboTreeBoxItemType.Nodes.Count > 0)
                    return comboTreeBoxItemType.Text;

                return string.Empty;
            }
        }

        public UserControlSPPIDMarkTagNoSetting()
        {
            InitializeComponent();
        }

        public override void LoadData()
        {
            InitDropdownListProject();
            InitDropdownListItemType(string.Empty);

            if (radDropDownListProjectList.SelectedItem != null)
            {
                var cpProject = (EntitySPTool_Project)radDropDownListProjectList.SelectedItem.DataBoundItem;
                InitTagSetting(cpProject, ItemType);
            }
        }

        private void InitDropdownListProject()
        {
            GeneralUtil.SetRadDropDownListProjectNo(EDCS.WF.BSL.Variable.SharedVariable.IsIE, ref radDropDownListProjectList, this._entityEDBInformation, ToolType.SPPID);
        }

        private void InitDropdownListItemType(string sItemType)
        {
            GeneralUtil.SetComboTreeBoxItemType(ref comboTreeBoxItemType);

            if (!string.IsNullOrEmpty(sItemType))
                comboTreeBoxItemType.Text = sItemType;
            else
                comboTreeBoxItemType.Text = Information.DRAWING;
        }

        private void radDropDownListProjectList_SelectedIndexChanged(object sender, Telerik.WinControls.UI.Data.PositionChangedEventArgs e)
        {

            if (radDropDownListProjectList.SelectedItem != null)
            {
                var cpProject = (EntitySPTool_Project)radDropDownListProjectList.SelectedItem.DataBoundItem;

                InitTagSetting(cpProject, ItemType);

                GeneralUtil.SaveRadDropDownListProjectNo(radDropDownListProjectList, this._entityEDBInformation, ToolType.SPPID);
            }
        }

        private void comboTreeBoxItemType_SelectedChanged(object sender, EventArgs e)
        {
            if (radDropDownListProjectList.SelectedItem != null)
            {
                var cpProject = (EntitySPTool_Project)radDropDownListProjectList.SelectedItem.DataBoundItem;
                InitTagSetting(cpProject, ItemType);
            }
        }

        private void InitTagSetting(EntitySPTool_Project cpProject, string sItemType)
        {
            radSplitContainerTagRule.Controls.Clear();
            mdicUserControlCommonTagRule.Clear();

            SplitPanel splitPanel = new SplitPanel();
            UserControlSPPIDMarkTagNo tagRule = new UserControlSPPIDMarkTagNo(EDCS.WF.BSL.Variable.SharedVariable.IsIE, cpProject.ProjectNo, TagNoSetting.TagNo, cpProject.ProjectID, sItemType, true);
            mdicUserControlCommonTagRule.Add(TagNoSetting.TagNo, tagRule);

            tagRule.Dock = DockStyle.Fill;
            splitPanel.Controls.Add(tagRule);
            radSplitContainerTagRule.Controls.Add(splitPanel);
        }

        public override void SaveData()
        {
            string sMessage = string.Empty;
            StringBuilder sbErrorMessage = new StringBuilder();
            foreach (KeyValuePair<string, UserControlSPPIDMarkTagNo> uc in mdicUserControlCommonTagRule)
            {
                bool bResult = uc.Value.SaveTagRule(ref sMessage);

                if (!bResult)
                {
                    sbErrorMessage.AppendLine(sMessage);
                }
            }

            if (sbErrorMessage.Length > 0)
                MessageBox.Show(sbErrorMessage.ToString());
            else
                MessageBox.Show(this, Resources.Message10000, this.Text);
        }

    }
}
