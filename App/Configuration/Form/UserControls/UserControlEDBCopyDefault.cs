using System;
using System.Collections.Generic;
using System.Configuration;
using System.Drawing;
using System.Linq;
using System.Windows.Forms;
using Configuration.BSL.Enums;
using EDCS.WF.BSL.Helpers;
using EDCS.WF.BSL.Variable;
using EDCS.WF.DSL;
using Telerik.WinControls;
using Telerik.WinControls.UI.Data;

namespace Configuration
{
    public partial class UserControlEDBCopyDefault : UserControlBase
    {
        public UserControlEDBCopyDefault()
        {
            try
            {
                this.InitializeComponent();
                this.Dock = DockStyle.Fill;

            
                // General
                this.MethodDesignForm();
                this.Font = new Font("Tahoma", 8.25F);
            }
            catch (Exception exception)
            {
                new ErrorHandling(this, exception, this._entityEDBInformation.Employee, this._entityEDBInformation.Project, this._entityEDBInformation.ListType).ErrorMsgDatabaseSave();
            }
        }
        public override void LoadData()
        {
            try
            {
                new EDCS.WF.BSL.Helpers.CustomRadMessageBoxShowAndTheme("공사중").RadMessageBoxShow();
                return;
                //this.radTextBoxConnectionStringSource.Text = ConfigurationManager.ConnectionStrings["EDCSEntities"].ConnectionString;
                //this.radTextBoxConnectionStringDest.Text = ConfigurationManager.ConnectionStrings["EDCSEntities"].ConnectionString;

                this.radDropDownListListType.ValueMember = "ListTypeID";
                this.radDropDownListListType.DisplayMember = "ComplexNameView";
                var proxy = WcfHelper.CreateProxyNetTcp<IEdcsDac>(EDCS.WF.BSL.Variable.SharedVariable.IsIE);
                using (proxy as IDisposable)
                {
                    this.radDropDownListListType.DataSource = proxy.ReadListTypes(EDCS.WF.BSL.Variable.SharedVariable.IsIE, false);
                }

                this.radDropDownListListType.SelectedValue = this._entityEDBInformation.ListType.ListTypeID;

                this.radDropDownListListType.SelectedIndexChanged += new PositionChangedEventHandler(radDropDownListListType_SelectedIndexChanged);
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
            if (this._entityTagNode.EnumFormName == EnumFormName.AttributeDefaultCopy)
            {
                this.MethodSaveColumnDynamic();
            }
            else if (this._entityTagNode.EnumFormName == EnumFormName.CaseDynamicDefaultCopy)
            {
                this.MethodSaveCaseDynamic();
            }
        }
        private void radDropDownListListType_SelectedIndexChanged(object sender, PositionChangedEventArgs e)
        {
            this._entityEDBInformation.ListType = this.radDropDownListListType.SelectedItem.DataBoundItem as EntityEDB_ListType;
        }
        private void MethodDesignForm()
        {

        }

        private void MethodSaveColumnDynamic()
        {
            var proxy = WcfHelper.CreateProxyNetTcp<IEdcsDac>(EDCS.WF.BSL.Variable.SharedVariable.IsIE);
            using (proxy as IDisposable)
            {
                List<EntityEDB_ColumnDynamicDefault> oldColumnDynamicDefaults = null;
                //Connection String 이 제대로 입력되었는지만 확인함.
                try
                {
                    //확인하는김에 불러옴.
                    oldColumnDynamicDefaults = proxy.ReadColumnDynamicDefaults(EDCS.WF.BSL.Variable.SharedVariable.IsIE, this.radTextBoxConnectionStringSource.Text).Where(w => w.ListTypeID == Convert.ToInt32(this.radDropDownListListType.SelectedValue)).ToList();
                    //확인만 하고 끝
                    proxy.ReadColumnDynamicDefaults(EDCS.WF.BSL.Variable.SharedVariable.IsIE, this.radTextBoxConnectionStringDest.Text);
                }

                catch
                {
                    new CustomRadMessageBoxShowAndTheme("ConnectionString 이 잘못되었습니다").RadMessageBoxShow();
                    return;
                }
                var oldColumnDynamicParentDefaults = proxy.ReadColumnDynamicParentDefaults(EDCS.WF.BSL.Variable.SharedVariable.IsIE, false, p_connectionString: this.radTextBoxConnectionStringSource.Text).Where(s => s.ListTypeID == Convert.ToInt32(this.radDropDownListListType.SelectedValue)).ToList();
                if (oldColumnDynamicParentDefaults.Count() > 0)
                {
                    var newColumnDynamicParentDefaults = oldColumnDynamicParentDefaults.Select(s => new EDB_ColumnDynamicParentDefault()
                    {
                        ListTypeID = this._entityEDBInformation.ListType.ListTypeID,
                        ColumnHeaderMergeProperty = s.ColumnHeaderMergeProperty,
                        CaseDynamicDefaultName = s.CaseDynamicDefaultName,
                    }).ToList();
                    proxy.CreateColumnDynamicParentDefaultsCopy(EDCS.WF.BSL.Variable.SharedVariable.IsIE,  newColumnDynamicParentDefaults, this._entityEDBInformation.ListType.ListTypeID, p_connectionString: this.radTextBoxConnectionStringDest.Text);
                }

                if (oldColumnDynamicDefaults.Count() > 0)
                {
                    var newColumnDyamicDefaults = oldColumnDynamicDefaults.Select(s => new EDB_ColumnDynamicDefault()
                    {
                        VisibleFlag = s.VisibleFlag,
                        DeleteFlag = s.DeleteFlag,
                        ModifiedDate = s.ModifiedDate,
                        ListTypeID = s.ListTypeID,
                        CommonUnitID = s.CommonUnitID,
                        Orders = s.Orders,
                        ColumnIndex = s.ColumnIndex,
                        Label = s.Label,
                        LabelView = s.LabelView,
                        CellType = s.CellType,
                        Locked = s.Locked,
                        Width = s.Width,
                        CaseDynamicDefaultName = s.CaseDynamicDefaultName,
                    }).ToList();
                    proxy.CreateColumnDynamicDefaultsCopy(EDCS.WF.BSL.Variable.SharedVariable.IsIE, newColumnDyamicDefaults, Convert.ToInt32(this.radDropDownListListType.SelectedValue), this.radTextBoxConnectionStringDest.Text);
                }
                //new CustomRadMessageBoxShowAndTheme(Resources.Message10007).RadMessageBoxShow();
            }
        }
        private void MethodSaveCaseDynamic()
        {
            //Connection String 이 제대로 입력되었는지만 확인함.
            try
            {
                //확인만 하고 끝
                var proxy = WcfHelper.CreateProxyNetTcp<IEdcsDac>(EDCS.WF.BSL.Variable.SharedVariable.IsIE);
                using (proxy as IDisposable)
                {
                    proxy.ReadColumnDynamicDefaults(EDCS.WF.BSL.Variable.SharedVariable.IsIE, this.radTextBoxConnectionStringSource.Text);
                    proxy.ReadColumnDynamicDefaults(EDCS.WF.BSL.Variable.SharedVariable.IsIE, this.radTextBoxConnectionStringDest.Text);
                }
            }
            catch
            {
                new CustomRadMessageBoxShowAndTheme("ConnectionString 이 잘못되었습니다").RadMessageBoxShow();
                return;
            }

            var proxy1 = WcfHelper.CreateProxyNetTcp<IEdcsDac>(EDCS.WF.BSL.Variable.SharedVariable.IsIE);
            using (proxy1 as IDisposable)
            {
                var oldCaseDynamicDefaults = proxy1.ReadCaseDynamicDefaultsByVisibleFlag(EDCS.WF.BSL.Variable.SharedVariable.IsIE, false, p_connectionString: this.radTextBoxConnectionStringSource.Text).Where(s => s.ListTypeID == Convert.ToInt32(this.radDropDownListListType.SelectedValue)).ToList();
                if (oldCaseDynamicDefaults.Count() > 0)
                {
                    var newCaseDynamicDefaults = oldCaseDynamicDefaults.Select(s => new EDB_CaseDynamicDefault()
                    {
                        ListTypeID = this._entityEDBInformation.ListType.ListTypeID,
                        Orders = s.Orders,
                        Name = s.Name,
                        NameView = s.NameView
                    }).ToList();
                    proxy1.CreateCaseDynamicDefaultsCopy(EDCS.WF.BSL.Variable.SharedVariable.IsIE, newCaseDynamicDefaults, Convert.ToInt32(this.radDropDownListListType.SelectedValue), p_connectionString: this.radTextBoxConnectionStringDest.Text);
                }
                //new CustomRadMessageBoxShowAndTheme(Resources.Message10007).RadMessageBoxShow();
            }

           
        }



    }
}
