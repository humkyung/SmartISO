using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using EDCS.WF.BSL.Enums;
using EDCS.WF.DSL;
using Telerik.WinControls.UI;
using EDCS.WF.BSL.Entities;
using EDCS.WF.BSL.Method;

namespace Configuration
{
    public partial class UserControlShareFrmSplit : UserControlBase
    {
        public UserControlShareFrmSplit()
        {
            InitializeComponent();

            //EDCS.WF.BSL.SubForms.UCShare.UCShareFrmSplit ucShareFrmSplit1 = new EDCS.WF.BSL.SubForms.UCShare.UCShareFrmSplit((int)EnumShareType.NEC_LIB, null);
            //ucShareFrmSplit1.Dock = System.Windows.Forms.DockStyle.Fill;
            //this.Controls.Add(ucShareFrmSplit1);

            MethodBindingRadDropDownList();


        }

        public void MethodBindingRadDropDownList()
        {
            this.radDropDownListListType.Items.Clear();

            int i = 1;
            foreach (var suit in Enum.GetValues(typeof(EnumShareType)))
            {
                if (i == 1)
                {
                    RadListDataItem item = new RadListDataItem();
                    item.Value = i;
                    item.Text = suit.ToString();
                    radDropDownListListType.Items.Add(item);
                }
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
                this.radDropDownListProject.Enabled = true;
                ucShareFrmSplit1.MethodBindingFpSpread(iShareType, int.Parse(this.radDropDownListProject.SelectedItem.Value.ToString()));

                var data = SharedMethod.GetShareData(iShareType, int.Parse(this.radDropDownListProject.SelectedItem.Value.ToString()));
            }
        }

        void radDropDownListListType_SelectedIndexChanged(object sender, Telerik.WinControls.UI.Data.PositionChangedEventArgs e)
        {
            try
            {
                if (this.radDropDownListProject.SelectedItem != null && this.radDropDownListListType.SelectedItem != null)
                {
                    int iShareType = int.Parse(this.radDropDownListListType.SelectedItem.Value.ToString());
                    this.radDropDownListProject.Enabled = true;
                    ucShareFrmSplit1.MethodBindingFpSpread(iShareType, int.Parse(this.radDropDownListProject.SelectedItem.Value.ToString()));
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
            try
            {
                ucShareFrmSplit1.SaveData();
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
