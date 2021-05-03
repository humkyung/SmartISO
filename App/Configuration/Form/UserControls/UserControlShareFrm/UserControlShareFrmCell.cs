using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using EDCS.WF.BSL.Enums;
using Telerik.WinControls.UI;
using EDCS.WF.DSL;

namespace Configuration
{
    public partial class UserControlShareFrmCell : UserControlBase
    {
        public UserControlShareFrmCell()
        {
            InitializeComponent();

            MethodBindingRadDropDownList();
        }

        public void MethodBindingRadDropDownList()
        {
            this.radDropDownListListType.Items.Clear();

            int i = 1;
            foreach (var suit in Enum.GetValues(typeof(EnumShareType)))
            {
                RadListDataItem item = new RadListDataItem();
                item.Value = i;
                item.Text = suit.ToString();
                radDropDownListListType.Items.Add(item);
                i++;
            }

            radDropDownListListType.SelectedIndexChanged += new Telerik.WinControls.UI.Data.PositionChangedEventHandler(radDropDownListListType_SelectedIndexChanged);

            var proxy = WcfHelper.CreateProxyNetTcp<IEdcsDac>(EDCS.WF.BSL.Variable.SharedVariable.IsIE);
            using (proxy as IDisposable)
            {
                if (this.radDropDownListProject.Items.Count == 0)
                {
                    var projects = proxy.ReadProjects(EDCS.WF.BSL.Variable.SharedVariable.IsIE, false).Where(w => w.CaseMainFlag == true).OrderByDescending(ob => ob.No).ToList();
                    projects.ConvertAll(ca => ca.ComplexNameView = ca.ComplexNameView + " - " + ca.Type);
                    this.radDropDownListProject.Items.Clear();
                    this.radDropDownListProject.ValueMember = "ProjectID";
                    this.radDropDownListProject.DisplayMember = "ComplexNameView";
                    //projects.RemoveAll(ra => !ra.ListTypeFlag.Contains(this._entityEDBInformation.ListType.ListTypeID));
                    this.radDropDownListProject.DataSource = projects;
                    //this.radDropDownListProject.SelectedValue = null;

                    this.radDropDownListProject.SelectedIndexChanged += new Telerik.WinControls.UI.Data.PositionChangedEventHandler(radDropDownListProject_SelectedIndexChanged);
                    //this.radDropDownListProject.SelectedIndexChanging += new Telerik.WinControls.UI.Data.PositionChangingEventHandler(radDropDownListProject_SelectedIndexChanging);
                }

            }
        }

        void radDropDownListProject_SelectedIndexChanged(object sender, Telerik.WinControls.UI.Data.PositionChangedEventArgs e)
        {
            if (this.radDropDownListProject.SelectedItem != null && this.radDropDownListListType.SelectedItem != null)
            {
                int iShareType = int.Parse(this.radDropDownListListType.SelectedItem.Value.ToString());
                if (iShareType == 1)
                {
                    this.radDropDownListProject.Enabled = true;

                    ucShareFrmCell1.MethodBindingFpSpread(iShareType, int.Parse(this.radDropDownListProject.SelectedItem.Value.ToString()));
                }
                else
                {
                    this.radDropDownListProject.Enabled = false;
                    ucShareFrmCell1.MethodBindingFpSpread(iShareType, null);
                }
            }
        }

        void radDropDownListListType_SelectedIndexChanged(object sender, Telerik.WinControls.UI.Data.PositionChangedEventArgs e)
        {
            try
            {
                if (this.radDropDownListProject.SelectedItem != null && this.radDropDownListListType.SelectedItem != null)
                {
                    int iShareType = int.Parse(this.radDropDownListListType.SelectedItem.Value.ToString());
                    if (iShareType == 1)
                    {
                        this.radDropDownListProject.Enabled = true;

                        ucShareFrmCell1.MethodBindingFpSpread(iShareType, int.Parse(this.radDropDownListProject.SelectedItem.Value.ToString()));
                    }
                    else
                    {
                        this.radDropDownListProject.Enabled = false;
                        ucShareFrmCell1.MethodBindingFpSpread(iShareType, null);
                    }
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

        public override void SaveData()
        {
            string sLoadingTexts = "Save data.  Please wait.";
            var frmPopupWaitingBarCallMethod = new TechSun.Theme.frmSplashScreen(1, EDCS.WF.BSL.Variable.SharedVariable.ToolTip);
            frmPopupWaitingBarCallMethod.MethodSetStart();
            frmPopupWaitingBarCallMethod.MethodStatusUpdateText(sLoadingTexts);

            try
            {
                ucShareFrmCell1.MethodSaveShareFpSpread();

                frmPopupWaitingBarCallMethod.MethodClose();
            }
            catch (System.ServiceModel.FaultException ex)
            {
                frmPopupWaitingBarCallMethod.MethodClose();
                new TechSun.Theme.frmWarning(ex.Message.ToString()).ShowDialog(this);
            }
            catch (Exception ex)
            {
                frmPopupWaitingBarCallMethod.MethodClose();
                new TechSun.Theme.frmWarning(ex.Message.ToString()).ShowDialog(this);
            }

        }

    }
}
