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

namespace Configuration
{
    public partial class UserControlShareFrmSplit_LIB : UserControlBase
    {
        public UserControlShareFrmSplit_LIB()
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
                if (i != 1)
                {
                    RadListDataItem item = new RadListDataItem();
                    item.Value = i;
                    item.Text = suit.ToString();
                    radDropDownListListType.Items.Add(item);
                }
                i++;
            }

            radDropDownListListType.SelectedIndexChanged += new Telerik.WinControls.UI.Data.PositionChangedEventHandler(radDropDownListListType_SelectedIndexChanged);
        }

        void radDropDownListListType_SelectedIndexChanged(object sender, Telerik.WinControls.UI.Data.PositionChangedEventArgs e)
        {
            try
            {
                if (this.radDropDownListListType.SelectedItem != null)
                {
                    int iShareType = int.Parse(this.radDropDownListListType.SelectedItem.Value.ToString());
                    ucShareFrmSplit_LIB1.MethodBindingFpSpread(iShareType);
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
                ucShareFrmSplit_LIB1.MethodSaveShareFpSpread();
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
