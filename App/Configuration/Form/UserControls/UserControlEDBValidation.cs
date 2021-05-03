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
using Telerik.WinControls.UI.Data;

namespace Configuration.Form.UserControls
{
    public partial class UserControlEDBValidation : UserControlBase
    {

        private List<EntityEDB_CommonColumnDynamic> _columnDynamics;
        //private string[] _columnDynamics;
        private List<string> Regex = new List<string>();
        private Dictionary<string, string> Exceptions = new Dictionary<string, string>();
        public UserControlEDBValidation()
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
                Regex.Add(string.Format("[^a-zA-Z0-9]"));
                Regex.Add(string.Format("[^a-zA-Z0-9-]"));
                Regex.Add(string.Format("[^a-zA-Z0-9/]"));
                Regex.Add(string.Format("[^a-zA-Z0-9/-]"));

                Exceptions.Add("S", "Split(/)");
                Exceptions.Add("CS", "Contain Size");
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
                    this.radDropDownListListType.SelectedIndexChanging += new Telerik.WinControls.UI.Data.PositionChangingEventHandler(radDropDownListListType_SelectedIndexChanging);

                    var projects = proxy.ReadProjects(EDCS.WF.BSL.Variable.SharedVariable.IsIE, false).Where(w => w.CaseMainFlag == true).OrderByDescending(obd => obd.No).ToList();
                    projects.ConvertAll(ca => ca.ComplexNameView = ca.ComplexNameView + " - " + ca.Type);
                    this.radDropDownListProject.Items.Clear();
                    this.radDropDownListProject.ValueMember = "ProjectID";
                    this.radDropDownListProject.DisplayMember = "ComplexNameView";
                    this.radDropDownListProject.DataSource = projects;
                    //this.radDropDownListProject.SelectedValue = null;
                    this.radDropDownListProject.SelectedValue = this._entityEDBInformation.Project.ProjectID;
                    this.radDropDownListProject.SelectedIndexChanged += new Telerik.WinControls.UI.Data.PositionChangedEventHandler(radDropDownListProject_SelectedIndexChanged);
                    this.radDropDownListProject.SelectedIndexChanging += new Telerik.WinControls.UI.Data.PositionChangingEventHandler(radDropDownListProject_SelectedIndexChanging);

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

        void radDropDownListProject_SelectedIndexChanging(object sender, PositionChangingCancelEventArgs e)
        {
            var newProject = (EntityEDB_Project)this.radDropDownListProject.Items[e.Position].DataBoundItem;
            var oldProject = (EntityEDB_Project)this.radDropDownListProject.SelectedItem.DataBoundItem;
            if (!newProject.ListTypeFlag.Contains(this._entityEDBInformation.ListType.ListTypeID))
            {
                //e.Cancel = true;
            }
        }

        void radDropDownListListType_SelectedIndexChanging(object sender, PositionChangingCancelEventArgs e)
        {
            var newListType = (EntityEDB_ListType)this.radDropDownListListType.Items[e.Position].DataBoundItem;
            var oldListType = (EntityEDB_ListType)this.radDropDownListListType.SelectedItem.DataBoundItem;
            if (!this._entityEDBInformation.Project.ListTypeFlag.Contains(newListType.ListTypeID))
            {
                e.Cancel = true;
            }
        }

        private void radGridViewValidation_PreviewKeyDown(object sender, PreviewKeyDownEventArgs e)
        {
            if (e.KeyValue.Equals(46)) // DELETE
            {
                if (this.radGridViewValidation.SelectedRows.Count > 0)
                {
                    for (int i = 0; i < this.radGridViewValidation.SelectedRows[0].Cells.Count; i++)
                    {
                        this.radGridViewValidation.SelectedRows[0].Cells[i].Value = null;
                    }
                }
            }
        }

        void radDropDownListProject_SelectedIndexChanged(object sender, Telerik.WinControls.UI.Data.PositionChangedEventArgs e)
        {
            this._entityEDBInformation.Project = (EntityEDB_Project)this.radDropDownListProject.SelectedItem.DataBoundItem;
            this.MethodBindingRadGridView();
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
                    List<EDB_ValidationDynamic> ValidationDynamics = new List<EDB_ValidationDynamic>();
                    //var validationDynamics = proxy.ReadValidationDynamics(false, this._entityEDBInformation.Project.ProjectID, this._entityEDBInformation.ListType.ListTypeID);

                    int validationIdColumn = this.radGridViewValidation.Columns["ValidationDynamicID"].Index;
                    int columnlabelCoulmn = this.radGridViewValidation.Columns["ColumnLabel"].Index;
                    int regexColumn = this.radGridViewValidation.Columns["Regex"].Index;
                    int numberflagColumn = this.radGridViewValidation.Columns["NumberFlag"].Index;
                    int allownullColumn = this.radGridViewValidation.Columns["AllowNull"].Index;
                    int exceptionColumn = this.radGridViewValidation.Columns["Exception"].Index;

                    for (int i = 0; i < this.radGridViewValidation.Rows.Count; i++)
                    {
                        var validationDynamic = new EDB_ValidationDynamic()
                        {
                            //ValidationDynamicID = int.Parse(this.radGridViewValidation.Rows[i].Cells[validationIdColumn].Value.ToString()),
                            ProjectID = this._entityEDBInformation.Project.ProjectID,
                            ListTypeID = this._entityEDBInformation.ListType.ListTypeID,
                            ColumnLabel = this.radGridViewValidation.Rows[i].Cells[columnlabelCoulmn].Value.ToString(),
                            ModifiedDate = DateTime.Now,
                            Regex = this.radGridViewValidation.Rows[i].Cells[regexColumn].Value == null ? "" : this.radGridViewValidation.Rows[i].Cells[regexColumn].Value.ToString(),
                            NumberFlag = this.radGridViewValidation.Rows[i].Cells[numberflagColumn].Value == null ? false : (bool)this.radGridViewValidation.Rows[i].Cells[numberflagColumn].Value,
                            AllowNull = this.radGridViewValidation.Rows[i].Cells[allownullColumn].Value == null ? false : (bool)this.radGridViewValidation.Rows[i].Cells[allownullColumn].Value,
                            Exception = this.radGridViewValidation.Rows[i].Cells[exceptionColumn].Value == null ? "" : this.radGridViewValidation.Rows[i].Cells[exceptionColumn].Value.ToString(),
                        };
                        ValidationDynamics.Add(validationDynamic);
                    }

                    proxy.CreateValidationDynamic(EDCS.WF.BSL.Variable.SharedVariable.IsIE, ValidationDynamics, this._entityEDBInformation.Project.ProjectID, this._entityEDBInformation.ListType.ListTypeID);

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
                            NameView = s.NameView,
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
                    Name = "ValidationDynamicID",
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

                var ValidationDynamics = proxy.ReadValidationDynamics(EDCS.WF.BSL.Variable.SharedVariable.IsIE, false, this._entityEDBInformation.Project.ProjectID, this._entityEDBInformation.ListType.ListTypeID);

                this.radGridViewValidation.RowCount = ValidationDynamics.Count;
                int nRow = 0;

                if (ValidationDynamics.Count > 0)
                {
                    foreach (var validationdynamic in ValidationDynamics)
                    {
                        this.radGridViewValidation.Rows[nRow].Cells["ValidationDynamicID"].Value = validationdynamic.ValidationDynamicID;
                        this.radGridViewValidation.Rows[nRow].Cells["ColumnLabel"].Value = validationdynamic.ColumnLabel;
                        this.radGridViewValidation.Rows[nRow].Cells["Regex"].Value = validationdynamic.Regex;
                        this.radGridViewValidation.Rows[nRow].Cells["NumberFlag"].Value = validationdynamic.NumberFlag;
                        this.radGridViewValidation.Rows[nRow].Cells["AllowNull"].Value = validationdynamic.AllowNull;
                        this.radGridViewValidation.Rows[nRow].Cells["Exception"].Value = validationdynamic.Exception;
                        nRow++;
                    }
                    this.radGridViewValidation.RowCount = ValidationDynamics.Count;
                }
                else this.radGridViewValidation.RowCount = 5;
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
