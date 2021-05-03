using System;
using System.Collections.Generic;
using System.Drawing;
using System.Linq;
using System.Windows.Forms;
using EDCS.WF.BSL.Helpers;
using EDCS.WF.BSL.Method;
using EDCS.WF.BSL.Variable;
using EDCS.WF.DSL;
using Telerik.WinControls;
using Telerik.WinControls.UI;

namespace Configuration.Form.UserControls
{
    public partial class UserControlEDBValidationDefault : UserControlBase
    {
        private List<EntityEDB_CommonColumnDynamic> _columnDynamics;
        //private string[] _columnDynamics;
        private List<string> Regex = new List<string>();
        private Dictionary<string, string> Exceptions = new Dictionary<string, string>();

        public UserControlEDBValidationDefault()
        {
            try
            {
                this.InitializeComponent();
                this.Dock = DockStyle.Fill;

                // General
                this.MethodDesignForm();
                //this.MethodTheme();
                Regex.Add(string.Format("[- ~%'\\" + '"' + "\\n\\r]"));
                Regex.Add(string.Format("[ ~%\\n" + "\\r]"));
                Exceptions.Add("S", "Split(/)");
                Exceptions.Add("E", "EquipmentType");
                Exceptions.Add("BQ", "BQ Spec");
                Exceptions.Add("SP", "Spec & Size");
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
                var proxy = WcfHelper.CreateProxyNetTcp<IEdcsDac>(EDCS.WF.BSL.Variable.SharedVariable.IsIE);
                using (proxy as IDisposable)
                {
                    this.radGridViewValidation.PreviewKeyDown += radGridViewValidation_PreviewKeyDown;
                    //this.userControlCommonTagRule1.UserControlCommonTagRuleEDB(this._entityEDBInformation.Project.ProjectMasterID, this._entityEDBInformation.ListType.ListTypeID);
                    this.radDropDownListListType.ValueMember = "ListTypeID";
                    this.radDropDownListListType.DisplayMember = "ComplexNameView";
                    this.radDropDownListListType.DataSource = proxy.ReadListTypes(EDCS.WF.BSL.Variable.SharedVariable.IsIE, false);
                    this.radDropDownListListType.SelectedValue = this._entityEDBInformation.ListType.ListTypeID;
                    this.radDropDownListListType.SelectedIndexChanged += new Telerik.WinControls.UI.Data.PositionChangedEventHandler(radDropDownListListType_SelectedIndexChanged);
                    //this.radDropDownListListType.SelectedIndexChanging += new Telerik.WinControls.UI.Data.PositionChangingEventHandler(radDropDownListListType_SelectedIndexChanging);

                    this.MethodBindingRadGridView();
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

        //void radDropDownListListType_SelectedIndexChanging(object sender, Telerik.WinControls.UI.Data.PositionChangingCancelEventArgs e)
        //{
        //    this._entityEDBInformation.ListType = (EntityEDB_ListType)this.radDropDownListListType.SelectedItem.DataBoundItem;
        //    this.MethodBindingRadGridView();
        //}

        private void radGridViewValidation_PreviewKeyDown(object sender, PreviewKeyDownEventArgs e)
        {
            if (e.KeyValue.Equals(46)) // DELETE
            {
                if (this.radGridViewValidation.SelectedRows.Count > 0)
                {
                    for (int i = 0; i < this.radGridViewValidation.SelectedRows[0].Cells.Count; i++)
                    {
                        this.radGridViewValidation.SelectedRows[0].Cells[i].Value = string.Empty;
                    }


                    for (int nRow = 0; nRow < this.radGridViewValidation.RowCount; nRow++)
                    {
                        if (this.radGridViewValidation.Rows[nRow].Cells["Column"].Value != null)
                        {

                        }
                    }

                }

            }
        }

        void radDropDownListListType_SelectedIndexChanged(object sender, Telerik.WinControls.UI.Data.PositionChangedEventArgs e)
        {
            this._entityEDBInformation.ListType = (EntityEDB_ListType)this.radDropDownListListType.SelectedItem.DataBoundItem;
            this.MethodBindingRadGridView();
        }

        public override void SaveData()
        {
            try
            {
                var proxy = WcfHelper.CreateProxyNetTcp<IEdcsDac>(EDCS.WF.BSL.Variable.SharedVariable.IsIE);
                using (proxy as IDisposable)
                {
                    List<EDB_ValidationDynamicDefault> ValidationDynamics = new List<EDB_ValidationDynamicDefault>();
                    //var validationDynamics = proxy.ReadValidationDynamics(false, this._entityEDBInformation.Project.ProjectID, this._entityEDBInformation.ListType.ListTypeID);

                    int validationIdColumn = this.radGridViewValidation.Columns["ValidationDynamicDefaultID"].Index;
                    int columnlabelCoulmn = this.radGridViewValidation.Columns["ColumnLabel"].Index;
                    int regexColumn = this.radGridViewValidation.Columns["Regex"].Index;
                    int numberflagColumn = this.radGridViewValidation.Columns["NumberFlag"].Index;
                    int allownullColumn = this.radGridViewValidation.Columns["AllowNull"].Index;
                    int exceptionColumn = this.radGridViewValidation.Columns["Exception"].Index;

                    for (int i = 0; i < this.radGridViewValidation.Rows.Count; i++)
                    {
                        var validationDynamic = new EDB_ValidationDynamicDefault()
                        {
                            ListTypeID = this._entityEDBInformation.ListType.ListTypeID,
                            ColumnLabel = this.radGridViewValidation.Rows[i].Cells[columnlabelCoulmn].Value.ToString(),
                            ModifiedDate = DateTime.Now,
                            Regex = this.radGridViewValidation.Rows[i].Cells[regexColumn].Value.ToString(),
                            NumberFlag = (bool)this.radGridViewValidation.Rows[i].Cells[numberflagColumn].Value,
                            AllowNull = (bool)this.radGridViewValidation.Rows[i].Cells[allownullColumn].Value,
                            Exception = this.radGridViewValidation.Rows[i].Cells[exceptionColumn].Value.ToString(),
                        };
                        ValidationDynamics.Add(validationDynamic);
                    }

                    proxy.CreateValidationDynamicDefault(EDCS.WF.BSL.Variable.SharedVariable.IsIE, ValidationDynamics, this._entityEDBInformation.Project.ProjectID, this._entityEDBInformation.ListType.ListTypeID);
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
        private void MethodDesignForm()
        {
            // radGridView1
            this.radGridViewValidation.AllowAutoSizeColumns = false;
            this.radGridViewValidation.AllowColumnChooser = false;
            this.radGridViewValidation.AllowColumnHeaderContextMenu = false;
            this.radGridViewValidation.AllowColumnReorder = false;
            this.radGridViewValidation.AllowMultiColumnSorting = false;
            this.radGridViewValidation.AllowRowResize = false;
            this.radGridViewValidation.AutoGenerateColumns = false;
            this.radGridViewValidation.AutoSizeColumnsMode = GridViewAutoSizeColumnsMode.Fill;
            this.radGridViewValidation.EnableHotTracking = false;
            this.radGridViewValidation.ShowCellErrors = false;
            this.radGridViewValidation.ShowFilteringRow = false;
            this.radGridViewValidation.ShowNoDataText = false;
            this.radGridViewValidation.ShowRowErrors = false;
            this.radGridViewValidation.TableElement.TableHeaderHeight = 25;
            this.radGridViewValidation.TableElement.RowHeight = 30;
            //
            this.radGridViewValidation.MultiSelect = false;
            this.radGridViewValidation.EnableGrouping = false;
            this.radGridViewValidation.AllowAddNewRow = false;
            this.radGridViewValidation.AllowDeleteRow = true;
            this.radGridViewValidation.AllowRowReorder = false;
            this.radGridViewValidation.AllowAddNewRow = false;

            this.radGridViewValidation.RowCount = 11;
            this.radGridViewValidation.AllowCellContextMenu = false;
            this.radGridViewValidation.ColumnChooserSortOrder = RadSortOrder.None;
        }

        private void MethodBindingRadGridView()
        {

            var proxy = WcfHelper.CreateProxyNetTcp<IEdcsDac>(EDCS.WF.BSL.Variable.SharedVariable.IsIE);
            using (proxy as IDisposable)
            {
                this.radGridViewValidation.Columns.Clear();

                this._columnDynamics = proxy.ReadCommonColumnDynamics(EDCS.WF.BSL.Variable.SharedVariable.IsIE, false, this._entityEDBInformation.ListType.ListTypeID)
                        .Select(s => new EntityEDB_CommonColumnDynamic()
                        {
                            Name = s.Name,
                            NameView = s.NameView
                        }).ToList();

                this.radGridViewValidation.Columns.Add(new GridViewCheckBoxColumn()
                {
                    Name = "DeleteCheck",
                    HeaderTextAlignment = ContentAlignment.MiddleCenter,
                    TextAlignment = ContentAlignment.MiddleCenter,
                    IsVisible = true,
                    Width = 24,
                    MinWidth = 24,
                    MaxWidth = 24,
                    EnableHeaderCheckBox = true,
                });
                this.radGridViewValidation.Columns.Add(new GridViewTextBoxColumn()
                {
                    Name = "ValidationDynamicDefaultID",
                    IsVisible = false,
                    ReadOnly = true,
                });
                this.radGridViewValidation.Columns.Add(new GridViewComboBoxColumn()
                {
                    Name = "ColumnLabel",
                    HeaderText = "ColumnLabel",
                    HeaderTextAlignment = ContentAlignment.MiddleCenter,
                    TextAlignment = ContentAlignment.MiddleCenter,
                    DropDownStyle = RadDropDownStyle.DropDown,
                    DataSource = this._columnDynamics,
                    DisplayMember = "NameView",
                    ValueMember = "Name",
                    AllowSort = false,
                });
                //this.radGridViewValidation.Columns.Add(new GridViewComboBoxColumn()
                //{
                //    Name = "Regex",
                //    HeaderText = "Regex",
                //    HeaderTextAlignment = ContentAlignment.MiddleCenter,
                //    TextAlignment = ContentAlignment.MiddleCenter,
                //    DropDownStyle = RadDropDownStyle.DropDown,
                //    DataSource = Regex,
                //    AllowSort = false,
                //});
                this.radGridViewValidation.Columns.Add(new GridViewTextBoxColumn()
                {
                    Name = "Regex",
                    HeaderText = "Regex",
                    HeaderTextAlignment = ContentAlignment.MiddleCenter,
                    TextAlignment = ContentAlignment.MiddleCenter,
                    AllowSort = false,
                });
                this.radGridViewValidation.Columns.Add(new GridViewCheckBoxColumn()
                {
                    Name = "NumberFlag",
                    HeaderText = "NumberFlag",
                    HeaderTextAlignment = ContentAlignment.MiddleCenter,
                    TextAlignment = ContentAlignment.MiddleCenter,
                    IsVisible = true,
                    Width = 50,
                    MinWidth = 50,
                    MaxWidth = 50,
                });
                this.radGridViewValidation.Columns.Add(new GridViewCheckBoxColumn()
                {
                    Name = "AllowNull",
                    HeaderText = "AllowNull",
                    HeaderTextAlignment = ContentAlignment.MiddleCenter,
                    TextAlignment = ContentAlignment.MiddleCenter,
                    IsVisible = true,
                    Width = 50,
                    MinWidth = 50,
                    MaxWidth = 50,
                });
                this.radGridViewValidation.Columns.Add(new GridViewComboBoxColumn()
                {
                    Name = "Exception",
                    HeaderText = "Exception",
                    HeaderTextAlignment = ContentAlignment.MiddleCenter,
                    TextAlignment = ContentAlignment.MiddleCenter,
                    DropDownStyle = RadDropDownStyle.DropDown,
                    DataSource = Exceptions,
                    ValueMember = "Key",
                    DisplayMember = "Value",
                    AllowSort = false,
                });

                var ValidationDynamicDefaults = proxy.ReadValidationDynamicDefault(EDCS.WF.BSL.Variable.SharedVariable.IsIE, false, this._entityEDBInformation.ListType.ListTypeID);

                this.radGridViewValidation.RowCount = ValidationDynamicDefaults.Count;
                int nRow = 0;

                if (ValidationDynamicDefaults.Count > 0)
                {
                    foreach (var validationdynamicDefault in ValidationDynamicDefaults)
                    {
                        this.radGridViewValidation.Rows[nRow].Cells["ValidationDynamicDefaultID"].Value = validationdynamicDefault.ValidationDynamicDefaultID;
                        this.radGridViewValidation.Rows[nRow].Cells["ColumnLabel"].Value = validationdynamicDefault.ColumnLabel;
                        this.radGridViewValidation.Rows[nRow].Cells["Regex"].Value = validationdynamicDefault.Regex;
                        this.radGridViewValidation.Rows[nRow].Cells["NumberFlag"].Value = validationdynamicDefault.NumberFlag;
                        this.radGridViewValidation.Rows[nRow].Cells["AllowNull"].Value = validationdynamicDefault.AllowNull;
                        this.radGridViewValidation.Rows[nRow].Cells["Exception"].Value = validationdynamicDefault.Exception;
                        nRow++;
                    }
                    this.radGridViewValidation.RowCount = ValidationDynamicDefaults.Count;
                }
                else { this.radGridViewValidation.RowCount = 5; }
            }
        }

        private void radButtonUp_Click(object sender, EventArgs e)
        {
            SharedMethod.MethodRadGridViewMoveRow(this.radGridViewValidation, true);
        }

        private void radButtonDown_Click(object sender, EventArgs e)
        {
            SharedMethod.MethodRadGridViewMoveRow(this.radGridViewValidation, false);
        }
        private void radButtonInsertRow_Click(object sender, EventArgs e)
        {
            GridViewRowInfo oGridRow = this.radGridViewValidation.Rows.AddNew();
        }

        private void radButtonRemoveRow_Click(object sender, EventArgs e)
        {
            for (int i = this.radGridViewValidation.Rows.Count - 1; i > -1; i--)
            {
                if (this.radGridViewValidation.Rows[i].Cells["DeleteCheck"].Value != null && (bool)this.radGridViewValidation.Rows[i].Cells["DeleteCheck"].Value == true)
                {
                    this.radGridViewValidation.Rows.RemoveAt(i);
                }
            }
        }
    }
}
